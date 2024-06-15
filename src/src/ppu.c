#include "../include/ppu.h"
#include "../include/memory.h"
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

/*-----------------------------------------------------------------------*/

// Here is utils -- will make into a separate header file eventually.
// First we create a stack for pixels.

// For each scanline we want to output key things:
// How many cycles did it take?
// How many pixels did we output?
// How many sprites were pushed to the sprite buffer?
// How many times did we fill the background fifo.
// How many times did we fill the sprite fifo.
struct framelogger {
    int cycles;
    int pixels;
    int sprites;
    int bg_fifo;
    int spr_fifo;
    int sprite_top;
};

struct framelogger logger[154] = {0};

struct ppu_state* ppu;

// Sprite Stack here:

struct sprite_stack {
    struct sprite data[10];
    int top;
};

void sprite_stack_init(struct sprite_stack* stack) {
    stack->top = -1;
}

int sprite_stack_is_empty(struct sprite_stack* stack) {
    return stack->top == -1;
}

int sprite_stack_is_full(struct sprite_stack* stack) {
    return stack->top == 10 - 1;
}

void sprite_stack_push(struct sprite_stack* stack, struct sprite sprite) {
    if (sprite_stack_is_full(stack)) {
        printf("STACK OVERFLOW\n");
        return;
    }
    stack->data[++stack->top] = sprite;
}

struct sprite sprite_stack_pop(struct sprite_stack* stack) {
    if (sprite_stack_is_empty(stack)) {
        printf("STACK UNDERFLOW\n");
        struct sprite empty_sprite = {0}; // Return an empty sprite
        return empty_sprite;
    }
    return stack->data[stack->top--];
}

//Background FIFO Stack

struct pixel_stack {
    struct pixel data[8];
    int top;
};

void pixel_stack_init(struct pixel_stack* stack) {
    stack->top = -1;
}

int pixel_stack_is_empty(struct pixel_stack* stack) {
    return stack->top == -1;
}

int pixel_stack_is_full(struct pixel_stack* stack) {
    return stack->top == 8 - 1;
}

void pixel_stack_push(struct pixel_stack* stack, struct pixel pixel) {
    if (pixel_stack_is_full(stack)) {
        printf("STACK OVERFLOW\n");
        return;
    }
    stack->data[++stack->top] = pixel;
}

struct pixel pixel_stack_pop(struct pixel_stack* stack) {
    if (pixel_stack_is_empty(stack)) {
        printf("STACK UNDERFLOW\n");
        struct pixel empty_pixel = {0}; // Return an empty pixel
        return empty_pixel;
    }
    return stack->data[stack->top--];
}

struct sprite_stack sprite_buffer;
struct pixel_stack background_fifo;
struct pixel_stack sprite_fifo;

/*-----------------------------------------------------------------------*/

void ppu_init() {
    
    // struct sprite_stack sprite_buffer = {0};
    // struct pixel_stack background_fifo = {0};
    // struct pixel_stack sprite_fifo = {0};

    // Init PPU struct.
    ppu = malloc(sizeof(struct ppu_state));

    // Init the PPU framebuffer.
    ppu->framebuffer = malloc(sizeof(u8) * GB_LENGTH * GB_WIDTH);

    // Init the PPU mode.
    ppu->STAT = &memory[0xFF41];

    ppu->bg_fetch = b_FETCH_TILE_NUM;

    ppu->spr_fetch = s_NONE;

    // Init the PPU mode countdown.
    ppu->current_mode_countdown = 2;

    // Init the PPU fetch countdown.
    ppu->current_fetch_countdown = 80;

    // Init the PPU scanline counter.
    ppu->scanline_counter = 0;

    // End of rame flag starts at 0.
    ppu->eof = 0; 

    ppu->lcd_enabled = 1;

    ppu->LCDC = &memory[0xFF40];

    ppu->LY = &memory[0xFF44];

    ppu->LYC = &memory[0xFF45];

    ppu->SCY = &memory[0xFF42];

    ppu->SCX = &memory[0xFF43];

    ppu->WY = &memory[0xFF4A];

    ppu->WX = &memory[0xFF4B];

    ppu->BGP = &memory[0xFF47];

    ppu->OBP0 = &memory[0xFF48];

    ppu->OBP1 = &memory[0xFF49];

    ppu->IE = &memory[0xFFFF];
    ppu->IF = &memory[0xFF0F];

    ppu->currentScanline = 0;

    ppu->OAM = &memory[0xFE00];

    //SUS
    ppu->sprites = (struct sprite*)ppu->OAM;

    ppu->md_3_x = 0;

    for (int i = 0; i < 40; i++) {
        ppu->sprite_buffer[i].y = 0;
        ppu->sprite_buffer[i].x = 0;
        ppu->sprite_buffer[i].tile = 0;
        ppu->sprite_buffer[i].flag = 0;

    }

    sprite_stack_init(&sprite_buffer);
    pixel_stack_init(&background_fifo);
    pixel_stack_init(&sprite_fifo);
}

void oam_scan() {

    // Are we in 8 or 16 pixel length sprite mode?
    u8 pixellength = 8;
    if ((memory[0xFF40] & 0b00000100) >> 2) pixellength = 16; 

    // Loop through each obj in OAM and push to sprite buffer if needed.
    for (int i = 0; i < 40; i++) {
        struct sprite temp = {0};
        temp.y = ppu->sprites[i].y;
        temp.x = ppu->sprites[i].x;
        temp.tile = ppu->sprites[i].tile;
        temp.flag = ppu->sprites[i].flag;

        // Check if the sprite is on the current scanline.
        if (temp.y - 16 <= *ppu->LY && temp.y + pixellength - 16 > *ppu->LY) {
            if (temp.x == 0) continue;
            if (temp.y == 0) continue;
            if (sprite_stack_is_full(&sprite_buffer)) {
                continue;
            }
            // Add to logger[*ppu->LY]
            logger[*ppu->LY].sprites = temp.x;
            sprite_stack_push(&sprite_buffer, temp);
        }
    }

    // // Sort the sprite buffer by x position.
    // for (int i = 0; i < sprite_buffer.top + 1; i++) {
    //     for (int j = 0; j < sprite_buffer.top - i; j++) {
    //         if (sprite_buffer.data[j].x > sprite_buffer.data[j+1].x) {
    //             struct sprite temp = sprite_buffer.data[j];
    //             sprite_buffer.data[j] = sprite_buffer.data[j+1];
    //             sprite_buffer.data[j+1] = temp;
    //         }
    //     }
    // }

    if (sprite_buffer.data[0].x > 0) write_8(0x3333, 0x33);

}

void push8_bg_fifo() {

    // Add to logger[*ppu->LY]
    logger[*ppu->LY].bg_fifo++;

    // Not thinking about windows for now!

    // What tilemap should we use?
    // Depends on LCDC register.
    u16 tile_data_area; 
    u16 tile_map_area;
    if ((*ppu->LCDC & 0b00010000) >> 4 == 0) tile_data_area = 0x8800;
    else tile_data_area = 0x8000;
    if ((*ppu->LCDC & 0b00001000) >> 3 == 0) tile_map_area = 0x9800;
    else tile_map_area = 0x9C00;

    printf ("Tile data area: %x\n", tile_data_area);
    printf ("Tile map area: %x\n", tile_map_area);

    // X offset is just our current x-coordinate.
    // Need to add scrolling functionality later.
    u8 fetcherX = floor((ppu->md_3_x & 0xFF) / 8.0);
    u8 fetchery = floor((*ppu->LY & 0xFF) / 8.0);

    printf ("FetcherX: %d\n", fetcherX);
    printf ("FetcherY: %d\n", fetchery);

    // Read from memory the appropriate title.
    u8 title_offset = read_8(tile_map_area + fetchery * 32 + fetcherX);

    // printf("Title offset: %d\n", title_offset);

    // Tile low data
    u8 tile_low_data = read_8(tile_data_area + 2 * (title_offset));

    // // PRINT WHERE WE READING FROM
    // printf("Reading from: %x\n", tile_data_area + 2 * (title_offset));

    // Tile high data
    u8 tile_high_data = read_8(tile_data_area + 2 * (title_offset) + 1);

    // // PRINT LOW AND HIGH TILE 
    // printf("Tile low data: %x\n", tile_low_data);
    // printf("Tile high data: %x\n", tile_high_data);

    // Push to fifo
    for (int i=0; i<8; i++) {
        struct pixel temp = {0};
        temp.lbit = (tile_low_data & (0x1 << i)) >> i;
        temp.hbit = (tile_high_data & (0x1 << i)) >> i;
        //print l and h bit
        printf("L bit: %d\n", temp.lbit);
        printf("H bit: %d\n", temp.hbit);
        pixel_stack_push(&background_fifo, temp);
    }

    // // print from background fifo
    // for (int i = 0; i < 8; i++) {
    //     printf("Background pixel %d: %d\n", i, background_fifo.data[i].hbit << 1 | background_fifo.data[i].lbit);
    // }

    // // print bracgroud fifo top
    // printf("Background fifo top: %d\n", background_fifo.top);

}

void push8_sprite_fifo() {
    
    // Add to logger[*ppu->LY]
    logger[*ppu->LY].spr_fifo++;

    struct sprite current_sprite = sprite_stack_pop(&sprite_buffer);

    // LCDC register does not affect the addressing for the objects.
    u16 tile_data_area = 0x8000;

    // Muliply by 32 as tiles are 32 bits.
    u8 tile_low_data = read_8(tile_data_area + current_sprite.tile * 32);

    u8 tile_high_data = read_8(tile_data_area + current_sprite.tile * 32 + 1);

    // Push to fifo.
    for (int i=0; i<8; i++) {
        struct pixel temp = {0};
        temp.lbit = (tile_low_data | (0x1 << i)) >> i;
        temp.hbit = (tile_high_data | (0x1 << i)) >> i;
        if (current_sprite.flags.priority == 1) temp.priority = 1;
        else temp.priority = 0;
        pixel_stack_push(&sprite_fifo, temp);
    }

}

u8 render_pixel() {

    // Increment logger[*ppu->LY]:
    logger[*ppu->LY].pixels++;

    // Pop both background pixel and object pixel.
    // If sprite pixel = 0, then use background pixel.
    // If BG-to-OBJ priority is 1 and color number of background pixel is anything but 0,
    // push the background pixel.
    // Otherwise push the sprite pixel to the screen.

    // Print background_fifo
    for (int i = 0; i < 8; i++) {
        printf("Background pixel %d: %d\n", i, background_fifo.data[i].hbit << 1 | background_fifo.data[i].lbit);
    }

    struct pixel background_pixel = {0};
    struct pixel obj_pixel = {0};

    if (!pixel_stack_is_empty(&background_fifo)) background_pixel = pixel_stack_pop(&background_fifo);
    if (!pixel_stack_is_empty(&sprite_fifo)) obj_pixel = pixel_stack_pop(&sprite_fifo);

    u8 background_pixel_num = background_pixel.hbit << 1 | background_pixel.lbit;
    u8 obj_pixel_num = obj_pixel.hbit << 1 | obj_pixel.lbit;

    //printf ("Background pixel num: %d\n");

    if (obj_pixel_num == 0x0) return background_pixel_num;
    if (obj_pixel.priority == 1 && background_pixel_num != 0) return background_pixel_num;


    return obj_pixel_num; 
}

void ppu_cycle() {

    // Increment logger[*ppu->LY]
    logger[*ppu->LY].cycles++;
    
    // First check what mode we are in.
    switch (*ppu->STAT & 0b00000011) {
        case 0: // Hblank
            // increment the scanline counter.
            ppu->scanline_counter++;

            // Check if we are at the end of the scanline.
            if (ppu->scanline_counter == 456) {
                ppu->scanline_counter = 0;
                *ppu->LY = *ppu->LY + 1;

                // Check if we should go to the Vblank mode.
                if (*ppu->LY == 144) {
                    *ppu->STAT = (*ppu->STAT & 0xFC) | 1;

                    // Set the Vblank interrupt.
                    *ppu->IF |= 0x1;

                } else {
                    // Go back to the OAM scan mode.
                    *ppu->STAT = (*ppu->STAT & 0xFC) | 2;
                }
            }
        break;
        case 1: // Vblank 
            // increment the scanline counter
           ppu->scanline_counter++;

            // Check if we are at the end of the scanline.
            if (ppu->scanline_counter == 456) {
                ppu->scanline_counter = 0;
                *ppu->LY = *ppu->LY + 1;
            }
            
            // Check if this is the last scanline.
            if (*ppu->LY == 154) {
                ppu->eof = 1;
                // Back to OAM scan mode.
                *ppu->STAT = (*ppu->STAT & 0xFC) | 2;
                *ppu->LY = 0;
                // Print out the logger.

                FILE* file = fopen("framelog.log", "w");
                if (file == NULL) {
                    printf("Failed to open framelog.log\n");
                    return;
                }
                for (int i = 0; i < 154; i++) {
                    fprintf(file, "-------------Row: %d--------------\n", i);
                    fprintf(file, "Scanline: %d\n", i);
                    fprintf(file, "Cycles: %d\n", logger[i].cycles);
                    fprintf(file, "Pixels: %d\n", logger[i].pixels);
                    fprintf(file, "Sprites: %d\n", logger[i].sprites);
                    fprintf(file, "BG FIFO: %d\n", logger[i].bg_fifo);
                    fprintf(file, "SPR FIFO: %d\n", logger[i].spr_fifo);
                    fprintf(file, "Sprite top: %d\n", logger[i].sprite_top);
                }

            }
        break;
        case 2: // OAM scan
            // On the first OAM scan cycle we read the objects.
            ppu->scanline_counter++;
            if(ppu->scanline_counter == 1) oam_scan();
            // Now check if OAM is finished.
            if(ppu->scanline_counter == 80) {
                // Go to the pixel transfer
                *ppu->STAT = (*ppu->STAT & 0xFC) | 3;
                ppu->scanline_counter = 0;
            }
        break;
        case 3: // Pixel transfer

            // First have we found a sprite?
            // This is a slow ass search probably.
            // This method of sprite search prioritises the closest sprite found and ignores all else.
            // If theres a tie the first sprite found will be used (lowest index).

            if(ppu->spr_fetch == s_NONE) {
                
                // Check the x pos for sprites.
                logger[*ppu->LY].sprite_top = sprite_buffer.top;
                // Print sprite buffer top
                printf("Sprite Buffer top = %d", sprite_buffer.top);
                for (int i = 0; i < sprite_buffer.top+1; i++) {
                    if((ppu->md_3_x >= sprite_buffer.data[i].x - 8) && (ppu->md_3_x < sprite_buffer.data[i].x)) {
                        // Note we aren't going through each fetch stage here.
                        printf("SPRITE FOUND \n");
                        ppu->current_mode_countdown = 1;
                        ppu->scanline_counter++;
                        ppu->lcd_enabled = 0;
                        ppu->spr_fetch = s_FETCH_TILE_NUM;
                        push8_sprite_fifo();
                        return;
                    }
                } 
            }

            // Now are we in a mode other than fifo push?
           if(ppu->bg_fetch!=b_FIFO_PUSH && ppu->spr_fetch!=s_FIFO_PUSH) {
                // printf("Current mode countdown: %d\n", ppu->current_mode_countdown);
                ppu->current_mode_countdown--;
                if (!ppu->current_mode_countdown) {
                    // printf("fetching\n");
                    // printf("bg_fetch: %d\n", ppu->bg_fetch);
                    // printf("spr_fetch: %d\n", ppu->spr_fetch);
                    // printf("spr_fetch: %d\n", s_NONE);
                    if(ppu->spr_fetch == s_NONE) ppu->bg_fetch++; 
                    else ppu->spr_fetch++;
                    ppu->current_mode_countdown = 2;
                }
            }

            // If we get here we are in fifo push.
            else {
                // We only get to restart bg fetch when fifo is empty.
                if(ppu->spr_fetch == s_NONE) {
                    if (pixel_stack_is_empty(&background_fifo)) {
                        push8_bg_fifo();
                        ppu->current_mode_countdown = 2;
                        ppu->bg_fetch = b_FETCH_TILE_NUM;
                    }
                }
                else {
                    // If we get here we are on push to fifo on the obj fetch.
                    // Stop sprite fetch and start using the object buffer.
                    ppu->lcd_enabled = 1;
                    ppu->current_mode_countdown = 2;
                    ppu->spr_fetch = s_NONE;
                }
                
            }

            // Now we can attempt to output a pixel!
            // Don't output a pixel here if our fifo is empty.
            ppu->scanline_counter++;

            if (!ppu->lcd_enabled) return; 

            if (pixel_stack_is_empty(&background_fifo)) return;

            if (*ppu->LY * GB_WIDTH + ppu->md_3_x > GB_LENGTH * GB_WIDTH) {
                printf("Out of bounds index on framebuffer!!");
            }
            else {
                u8 usedPixel = render_pixel();
                printf("Used pixel: %d\n", usedPixel);
                ppu->framebuffer[*ppu->LY * GB_WIDTH + ppu->md_3_x] = usedPixel;
            }

            // Increment xcoord and check if we need to go to hblank.
            ppu->md_3_x++;
            if (ppu->md_3_x > 159) {
                *ppu->STAT = (*ppu->STAT & 0xFC) | 0;
                ppu->md_3_x = 0;
                ppu->current_mode_countdown = 2;
                ppu->bg_fetch = b_FETCH_TILE_NUM;

            }
            break;
        }
    }           

#ifndef PPU_H
#define PPU_H

#include <stdlib.h>

#define GB_LENGTH 144
#define GB_WIDTH 160

typedef u_int8_t u8;
typedef u_int16_t u16;

struct pixel {
    u8 lbit;
    u8 hbit;
    u8 priority;
};

struct sprite {
    u8 y;
    u8 x;
    u8 tile;
    union {
        u8 flag;
        struct __attribute__((packed)) {
            u8 cgonly0:1;
            u8 cgonly1:1;
            u8 cgonly2:1;
            u8 cgonly3:1;
            u8 palette:1;
            u8 xflip:1;
            u8 yflip:1;
            u8 priority:1;
        } flags;
    };
};

struct ppu_state {

    // Our mock LCD (where we will draw our pixels to)
    u8* framebuffer;

    enum background_fetch_state {
        b_FETCH_TILE_NUM,
        b_FETCH_TILE_DATA_LOW,
        b_FETCH_TILE_DATA_HIGH,
        b_FIFO_PUSH
    } bg_fetch;

    enum sprite_fetch_state {
        s_NONE,
        s_FETCH_TILE_NUM,
        s_FETCH_TILE_DATA_LOW,
        s_FETCH_TILE_DATA_HIGH,
        s_FIFO_PUSH
    } spr_fetch;

    // Displays the countdown in terms of cycles that the current mode has.
    int current_mode_countdown;

    // Displays the countdown in terms of cycles that the current fetch mode has.
    int current_fetch_countdown;

    // Displays the scanline counter.
    int scanline_counter;
    
    // Mode 3s current xposition.
    u8 md_3_x;

    // Whether the lcd is enabled 
    u8 lcd_enabled;

    // LCD control needs to point to the corresponding memory address
    u8* LCDC;
    
    // LY is for the current Y position. Also the current scanline.
    u8* LY;
    u8* LYC;

    // Window position registers
    u8* SCY;
    u8* SCX;
    u8* WY;
    u8* WX;

    // Pallete registers.
    u8* BGP;
    u8* OBP0;
    u8* OBP1;

    // To keep count of the frame count and current scanline. 
    u8 currentScanline;

    // Interrupt handlers
    u8* IE;
    u8* IF;
    u8* STAT;

    // To check if we've reached the end of the frame.
    u8 eof;

    // OAM Memory
    u8* OAM;

    struct sprite* sprites;

    // Sprite Buffer
    struct sprite sprite_buffer[40];

};

/* --------------------------------------------- */


/* --------------------------------------------- */

// Background FIFO Stack


extern struct ppu_state* ppu;

void ppu_init();

void ppu_cycle();

#endif // PPU_H

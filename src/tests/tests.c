
#include "../include/cpu.h"
#include "../include/ppu.h"
#include "../include/memory.h"
#include <stdio.h>



#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>
#include "GLFW/linmath.h"


#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 400;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"

    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"

    "void main()"
    "{"
        "gl_Position = vec4(aPos, 1.0);"
        "ourColor = aColor;"
        "TexCoord = aTexCoord;"
    "}";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"

    "uniform sampler2D ourTexture;\n"

    "void main()\n"
    "{\n"
    "   FragColor = texture(ourTexture, TexCoord);\n"
    "}\n\0";

void initialiseAll() {
    initialise_memory();
    initialise_cpu();
    ppu_init();
}

void backgroundTiles() {
    printf("Initialising background tiles\n");
    // Test background will create all tiles as pixels 0x01.
    write_8(0x8800, 0xFF);
    write_8(0x8801, 0x00);
    write_8(0x8802, 0xFF);
    write_8(0x8803, 0xFF);
    
    printf("Background tiles initialised\n");

    for (int i = 0x9800; i < 0x9BFF; i++) {
        if (i == 0x9800) {
            write_8(i, 0x00);
        } else {
            write_8(i, 0x01);
        }
    }

    write_8(0x8000, 0xFF);
    write_8(0x8001, 0xFF);

    // Initialise an object
    ppu->sprites[0].y =    0x20;
    ppu->sprites[0].x =    0x40;
    ppu->sprites[0].tile = 0x00;
    ppu->sprites[0].flag = 0x00;

}

void completeCycle() {
    //execute_cycle();
    ppu_cycle();
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {

        // Positions           // Colors           // Texture Coords
        1.0f, 1.0f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

    }; 
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

 unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Set up textures
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create a pixel buffer
    int width = 160; // number of pixels in each row
    int height = 144; // number of pixels in each column
    GLubyte pixels[width * height * 4]; // each pixel has 4 components (R, G, B, A)

    // Initialise GB
    initialiseAll();

  
    // Initialise the background tiles
    backgroundTiles();

    // Keep executing cycles unit we real the end of the frame.
    *ppu->STAT = (*ppu->STAT & 0xFC) | 2;
    for (int i = 0; i < 100000; i++) {
        completeCycle();
        if (ppu->eof == 1) {
            break;
        }

        // if(ppu->md_3_x == 65 && *ppu->LY == 16) {
        //     break;
        // }
 
        // printf("New Cycle\n");
        printf("Current Y Pos = %d\n", *ppu->LY);
        printf("Currnet X Pos = %d\n", ppu->md_3_x);
        printf("Current Mode = %d\n", *ppu->STAT & 0x3);
        printf("Current BG Fetch Mode = %d\n", ppu->bg_fetch);
        printf("Current SPR Fetch Mode = %d\n", ppu->spr_fetch);
        printf("Current Scanline Counter = %d\n", ppu->scanline_counter);
        // print spritebuffer top 
        
    }

    //print the first sprite in the oam
    printf("Sprite 0 = %x\n", ppu->OAM[0]);
    printf("Sprite 0 = %x\n", ppu->OAM[1]);
    printf("Sprite 0 = %x\n", ppu->OAM[2]);
    printf("Sprite 0 = %x\n", ppu->OAM[3]);

    printf("Sprite 0 = %x\n", read_8(0x3333));

    // print first obj in OAM

    //If end of frame print the framebuffer.
    // for (int i = 0; i < width * height; i++) {
    //     printf("%x\n", ppu->framebuffer[i]);
    // }   

    // print framebuffer
    // for(int i = 0; i < width * height; i++) {
    //     printf("%x\n", ppu->framebuffer[i]);
    // }

    // Initialize the pixel buffer with some data
    for (int i = 0; i < width * height * 4; i+=4) {
        if(ppu->framebuffer[i/4] == 0b01) {
            pixels[i] = 0x10;   // R
            pixels[i+1] = 0x21;   // G
            pixels[i+2] = 0x3D;   // B
            pixels[i+3] = 0; // A
        }
        else if (ppu->framebuffer[i/4] == 0b10) {
            pixels[i] = 0;   // R
            pixels[i+1] = 0x11;   // G
            pixels[i+2] = 0x2D;   // B
            pixels[i+3] = 0; // A
        }
        else if (ppu->framebuffer[i/4] == 0b11) {
            pixels[i] = 0;   // R
            pixels[i+1] = 0;   // G
            pixels[i+2] = 0;   // B
            pixels[i+3] = 0; // A
        }
        else {
            pixels[i] = 0x20;   // R
            pixels[i+1] = 0x30;   // G
            pixels[i+2] = 0;   // B
            pixels[i+3] = 0; // A
        }

    }

    // Upload the pixel buffer to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    // check shader logs
    

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    int increment = 0;

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // Upload the pixel buffer to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/*------------------------------------------------------------------------------------------


    Testing Framework.

    The idea of the testing framework is to show that every cpu operation behaves appropriately.
    There will be a single unit test for the following:

        - Every memory operation, reads writes pushes and pops.
        - Each register, to test the functionality of the onion operation etc.
        - Every cpu instruction group, load the op code + data and PC and run, examine the output.
        - Also for each ppu register and function to test union shenanigans.
        - Individual frames for the ppu to render, to test speed. 

------------------------------------------------------------------------------------------*/

// void dump_cpu() {

//     // Function to dump the contents of the CPU registers for error checking purposes.
//     printf("Dumping CPU registers:\n");
//     printf("AF: 0x%04X\n", gb->reg16.AF);
//     printf("BC: 0x%04X\n", gb->reg16.BC);
//     printf("DE: 0x%04X\n", gb->reg16.DE);
//     printf("HL: 0x%04X\n", gb->reg16.HL);
//     printf("PC: 0x%04X\n", gb->PC);
//     printf("SP: 0x%04X\n", gb->SP);
//     printf("HF: 0x%04X\n", gb->flags.HF);
//     printf("NF: 0x%04X\n", gb->flags.NF);
//     printf("CF: 0x%04X\n", gb->flags.CF);
//     printf("ZF: 0x%04X\n", gb->flags.ZF);

// }

// void reset_cpu() {

//     // Function to reset the CPU registers to 0.
//     gb->reg16.AF = 0x0000;
//     gb->reg16.BC = 0x0000;
//     gb->reg16.DE = 0x0000;
//     gb->reg16.HL = 0x0000;
//     gb->PC = 0x0000;
//     gb->SP = 0xFFFE;

// }

// int main() {

//     initialise_memory();
//     initialise_cpu();

//     // Test Group 1 - Registers.
//     gb->reg16.AF = 0x1234;

//     if (gb->reg16.AF == 0x1234 && gb->reg8.A == 0x12 && gb->reg8.F == 0x34) {
//         printf("Test Group 1a - Registers: Passed\n");
//     } else {
//         printf("Test Group 1a - Registers: Failed\n");
//         dump_cpu();
//     }

//     gb->reg16.BC = 0x5678;

//     if (gb->reg16.BC == 0x5678 && gb->reg8.B == 0x56 && gb->reg8.C == 0x78) {
//         printf("Test Group 1b - Registers: Passed\n");
//     } else {
//         printf("Test Group 1b - Registers: Failed\n");
//         dump_cpu();
//     }

//     gb->reg16.DE = 0x9ABC;

//     if (gb->reg16.DE == 0x9ABC && gb->reg8.D == 0x9A && gb->reg8.E == 0xBC) {
//         printf("Test Group 1c - Registers: Passed\n");
//     } else {
//         printf("Test Group 1c - Registers: Failed\n");
//         dump_cpu();
//     }

//     gb->reg16.HL = 0xDEF0;

//     if (gb->reg16.HL == 0xDEF0 && gb->reg8.H == 0xDE && gb->reg8.L == 0xF0) {
//         printf("Test Group 1d - Registers: Passed\n");
//     } else {
//         printf("Test Group 1d - Registers: Failed\n");
//         dump_cpu();
//     }


//     gb->SP = 0xFFFE;

//     // Test Group 2 - Memory Operations.

//     write_8(0x1234, 0x56);

//     if (read_8(0x1234) == 0x56) {
//         printf("Test Group 2a - Memory Operations: Passed\n");
//     } else {
//         printf("Test Group 2a - Memory Operations: Failed\n");
//         dump_cpu();
//     }

//     write_16(0x1234, 0x5678);

//     if (read_8(0x1234) == 0x78 && read_8(0x1235) == 0x56) {
//         printf("Test Group 2b - Memory Operations: Passed\n");
//     } else {
//         printf("Test Group 2b - Memory Operations: Failed\n");
//         dump_cpu();
//     }

//     push_16(&gb->SP, 0x1234);

//     if (gb->SP == 0xFFFC && read_8(0xFFFD) == 0x12 && read_8(0xFFFE) == 0x34) {
//         printf("Test Group 2c - Memory Operations: \033[32mPassed\033\[0m\n");
//     } else {
//         printf("Test Group 2c - Memory Operations: Failed\n");
//         dump_cpu();
//     }

//     u16 temp = pop_16(&gb->SP);

//     if (gb->SP == 0xFFFE && temp == 0x1234) {
//         printf("Test Group 2d - Memory Operations: Passed\n");
//     } else {
//         printf("Test Group 2d - Memory Operations: Failed\n");
//         printf("Expected: 0x1234, Got: 0x%04X\n", temp);
//         dump_cpu();
//     }

//     // Test Group 3 - CPU Instructions.
//     // Write into the PC each type of instruction.
//     // Set all registers to 0.

//     reset_cpu();

//     // test 1: NOP
//     write_8(0x0000, 0x00);
//     u8 cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg16.AF == 0x0000 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3a - CPU Instructions: \033[32mNOP Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3a - CPU Instructions: NOP Failed\n");
//         dump_cpu();
//     }

//     // test 2: RLCA
//     reset_cpu();

//     write_8(0x0000, 0x07);
//     gb->reg8.A = 0b10010011;
//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0b00100111 &&
//         gb->flags.CF == 0x1 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3b - CPU Instructions: \033[32mRLCA Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3b - CPU Instructions: RLCA Failed\n");
//         dump_cpu();
//     }

//     // test 3: RRCA
//     reset_cpu();

//     write_8(0x0000, 0x0F);

//     gb->reg8.A = 0b10010011;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0b11001001 &&
//         gb->flags.CF == 0x1 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3c - CPU Instructions: \033[32mRRCA Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3c - CPU Instructions: RRCA Failed\n");
//         dump_cpu();
//     }

//     // test 4: RLA
//     reset_cpu();

//     write_8(0x0000, 0x17);

//     gb->reg8.A = 0b10010011;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0b00100110 &&
//         gb->flags.CF == 0x1 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3d - CPU Instructions: \033[32mRLA Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3d - CPU Instructions: RLA Failed\n");
//         dump_cpu();
//     }

//     // test 5: RRA
//     reset_cpu();

//     write_8(0x0000, 0x1F);

//     gb->reg8.A = 0b10010011;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0b01001001 &&
//         gb->flags.CF == 0x1 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3e - CPU Instructions: \033[32mRRA Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3e - CPU Instructions: RRA Failed\n");
//         dump_cpu();
//     }

//     // test 6: DAA - UNTESTED UNMADE
//     reset_cpu();

//     write_8(0x0000, 0x27);

//     gb->reg8.A = 0x45;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0x45 &&
//         gb->flags.CF == 0x0 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3f - CPU Instructions: \033[32mDAA Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3f - CPU Instructions: DAA Failed\n");
//         dump_cpu();
//     }

//     // test 7: CPL
//     reset_cpu();

//     write_8(0x0000, 0x2F);

//     gb->reg8.A = 0b01100110;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0001 &&
//         gb->reg8.A == 0b10011001 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x1 &&
//         gb->flags.NF == 0x1 &&
//         gb->flags.ZF == 0x0 &&
//         gb->reg16.BC == 0x0000 &&
//         gb->reg16.DE == 0x0000 &&
//         gb->reg16.HL == 0x0000 &&
//         gb->SP == 0xFFFE &&
//         cycle == 4
//     ) {
//         printf("Test Group 3g - CPU Instructions: \033[32mCPL Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3g - CPU Instructions: CPL Failed\n");
//         dump_cpu();
//     }

//     // test 8: JMP IMM8

//     reset_cpu();

//     write_8(0x0000, 0xC3);

//     write_8(0x0001, 0x34);

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0034 &&
//         cycle == 16
//     ) {
//         printf("Test Group 3h - CPU Instructions: \033[32mJMP IMM8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3h - CPU Instructions: JMP IMM8 Failed\n");
//         dump_cpu();
//     }

//     // test 9: LD_IMM16_SP
//     reset_cpu();

//     write_8(0x0000, 0x08);   

//     write_8(0x0001, 0x34);

//     write_8(0x0002, 0x12);

//     cycle = execute_instruction();

//     if (
//         read_8(0x1234) == 0xFE &&
//         cycle == 20 &&
//         read_8(0x1234+1) == 0xFF
//     ) {
//         printf("Test Group 3i - CPU Instructions: \033[32mLD IMM16 SP Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3i - CPU Instructions: LD IMM16 SP Failed\n");
//         dump_cpu();
//     }

//     // test 10: ADD_HL_r16
//     reset_cpu();
    
//     write_8(0x0000, 0x09);

//     gb->reg16.HL = 0x1234;

//     gb->reg16.BC = 0x5678;

//     cycle = execute_instruction();

//     if (
//         gb->reg16.HL == 0x68AC &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 8
//     ) {
//         printf("Test Group 3j - CPU Instructions: \033[32mADD HL r16 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3j - CPU Instructions: ADD HL r16 Failed\n");
//         dump_cpu();
//     }

//     // Test 10 LD R16 IMM16

//     reset_cpu();

//     write_8(0x0000, 0x01);

//     write_8(0x0001, 0x34);

//     write_8(0x0002, 0x12);

//     cycle = execute_instruction();

//     if (
//         gb->reg16.BC == 0x1234 &&
//         cycle == 12
//     ) {
//         printf("Test Group 3k - CPU Instructions: \033[32mLD R16 IMM16 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3k - CPU Instructions: LD R16 IMM16 Failed\n");
//         dump_cpu();
//     }   

//     // Test 11 LD r16 mem a

//     reset_cpu();

//     write_8(0x0000, 0x22);

//     gb->reg16.HL = 0x0010;

//     gb->reg8.A = 0x05;

//     cycle = execute_instruction();

//     if (
//         gb->reg16.HL == 0x0011 &&
//         read_8(0x0010) == 0x05 &&
//         cycle == 8
//     ) {
//         printf("Test Group 3l - CPU Instructions: \033[32mLD r16 mem a Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3l - CPU Instructions: LD r16 mem a Failed\n");
//         dump_cpu();
//     }

//     // Test 12 DEC r8

//     reset_cpu();

//     write_8(0x0000, 0x3D);

//     gb->reg8.A = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x00 &&
//         gb->flags.ZF == 0x1 &&
//         gb->flags.NF == 0x1 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3m - CPU Instructions: \033[32mDEC r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3m - CPU Instructions: DEC r8 Failed\n");
//         dump_cpu();
//     }
    
//    // Test 13 JR_COND_IMM8

//    reset_cpu();

//    write_8(0x0000, 0x20);

//    write_8(0x0001, 0x02);

//     gb->flags.ZF = 0x0;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0004 &&
//         cycle == 12
//     ) {
//         printf("Test Group 3n - CPU Instructions: \033[32mJR COND IMM8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3n - CPU Instructions: JR COND IMM8 Failed\n");
//         dump_cpu();
//     }

//     // Test 14 LD r8 r8

//     reset_cpu();

//     write_8(0x0000, 0x78);

//     gb->reg8.B = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x01 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3o - CPU Instructions: \033[32mLD r8 r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3o - CPU Instructions: LD r8 r8 Failed\n");
//         dump_cpu();
//     }

//     // Test 15 ADD_A_r8

//     reset_cpu();

//     write_8(0x0000, 0x84);

//     gb->reg8.A = 0x01;

//     gb->reg8.H = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x02 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3p - CPU Instructions: \033[32mADD A r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3p - CPU Instructions: ADD A r8 Failed\n");
//         dump_cpu();
//     }

//     // Test 16 ADC_A_r8

//     reset_cpu();

//     write_8(0x0000, 0x8C);

//     gb->reg8.A = 0x01;

//     gb->reg8.H = 0x01;

//     gb->flags.CF = 0x1;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x03 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3q - CPU Instructions: \033[32mADC A r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3q - CPU Instructions: ADC A r8 Failed\n");
//         dump_cpu();
//     }

//     // Test 17 SUB_A_r8

//     reset_cpu();

//     write_8(0x0000, 0x90);

//     gb->reg8.A = 0x02;

//     gb->reg8.B = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x01 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3r - CPU Instructions: \033[32mSUB A r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3r - CPU Instructions: SUB A r8 Failed\n");
//         dump_cpu();
//     }

//     // Test 18 CP_A_R8

//     reset_cpu();

//     write_8(0x0000, 0xB8);

//     gb->reg8.A = 0x02;

//     gb->reg8.B = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x02 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 4
//     ) {
//         printf("Test Group 3s - CPU Instructions: \033[32mCP A r8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3s - CPU Instructions: CP A r8 Failed\n");
//         dump_cpu();
//     }

//     // Test 19 ADD_A_IMM8

//     reset_cpu();

//     write_8(0x0000, 0xC6);

//     write_8(0x0001, 0x01);

//     gb->reg8.A = 0x01;

//     cycle = execute_instruction();

//     if (
//         gb->reg8.A == 0x02 &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 8
//     ) {
//         printf("Test Group 3t - CPU Instructions: \033[32mADD A IMM8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3t - CPU Instructions: ADD A IMM8 Failed\n");
//         dump_cpu();
//     }

//     // Test 20 RET

//     reset_cpu();

//     write_8(0x0000, 0xC9);

//     gb->SP = 0xFFFD;

//     write_16(0xFFFE, 0x1234);

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x3412 &&
//         cycle == 16
//     ) {
//         printf("Test Group 3u - CPU Instructions: \033[32mRET Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3u - CPU Instructions: RET Failed\n");
//         dump_cpu();
//     }

//     // Test 21 JP_IMM16

//     reset_cpu();

//     write_8(0x0000, 0xC3);

//     write_8(0x0001, 0x34);

//     write_8(0x0002, 0x12);

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x1234 &&
//         cycle == 16
//     ) {
//         printf("Test Group 3v - CPU Instructions: \033[32mJP IMM16 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3v - CPU Instructions: JP IMM16 Failed\n");
//         dump_cpu();
//     }

//     //TEST 22 CALL IMM16

//     reset_cpu();

//     write_8(0x0000, 0xCD);

//     write_8(0x0001, 0x34);

//     write_8(0x0002, 0x12);

//     gb->SP = 0xFFFD;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x1234 &&
//         read_8(0xFFFD) == 0x01 &&
//         read_8(0xFFFC) == 0x00 &&
//         cycle == 24
//     ) {
//         printf("Test Group 3w - CPU Instructions: \033[32mCALL IMM16 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3w - CPU Instructions: CALL IMM16 Failed\n");
//         dump_cpu();
//     }

//     //TEST 23 ADD_SP_IMM8

//     reset_cpu();

//     write_8(0x0000, 0xE8);

//     write_8(0x0001, 0x01);

//     gb->SP = 0xFFFE;

//     cycle = execute_instruction();

//     if (
//         gb->SP == 0xFFFF &&
//         gb->flags.CF == 0x0 &&
//         gb->flags.HF == 0x0 &&
//         cycle == 16
//     ) {
//         printf("Test Group 3x - CPU Instructions: \033[32mADD SP IMM8 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3x - CPU Instructions: ADD SP IMM8 Failed\n");
//         dump_cpu();
//     }   

//     //TEST 23 RET_COND

//     reset_cpu();

//     write_8(0x0000, 0xC0);

//     gb->SP = 0xFFFD;

//     write_16(0xFFFE, 0x3412);

//     gb->flags.ZF = 0x0;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x1234 &&
//         cycle == 20
//     ) {
//         printf("Test Group 3y - CPU Instructions: \033[32mRET COND Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3y - CPU Instructions: RET COND Failed\n");
//         dump_cpu();
//     }

//     // TEST 24 CALL_COND_IMM16

//     reset_cpu();

//     write_8(0x0100, 0xC7);

//     write_8(0x0000, 0x34);

//     write_8(0x0001, 0x12);

//     gb->SP = 0xFFFD;

//     gb->PC = 0x0100;

//     gb->flags.ZF = 0x0;

//     cycle = execute_instruction();

//     if (
//         gb->PC == 0x0034 &&
//         read_8(0xFFFD) == 0x01 &&
//         read_8(0xFFFC) == 0x01 &&
//         cycle == 16
//     ) {
//         printf("Test Group 3z - CPU Instructions: \033[32mCALL COND IMM16 Passed\033\[0m\n");
//     } else {
//         printf("Test Group 3z - CPU Instructions: CALL COND IMM16 Failed\n");
//         dump_cpu();
//     }



// }




#include "HardWare.h"




static int handler(void* user, const char* section, const char* name, const char* value)
{
    Chip8* pchip = (Chip8*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    // Video Section
    if (MATCH("Video", "SCREEN_WIDTH")) {
        pchip->config.screenWidth = atoi(value);
    }
    else if (MATCH("Video", "SCREEN_HEIGHT")) {
        pchip->config.screenHeight = atoi(value);
    }
    else if (MATCH("Video", "USE_CLIPPING")) {
        pchip->config.useClipping = (atoi(value) != 0);
    }

    // CPU Section
    else if (MATCH("CPU", "CLOCK_SPEED")) {
        pchip->config.clockSpeed = atoi(value);
    }
    else if (MATCH("CPU", "TARGET_FPS")) {
        pchip->config.targetFPS = atoi(value);
    }
    else if (MATCH("CPU", "CHIP_QUIRKS")) {
        pchip->config.chipQuirks = atoi(value);
    }

    // Appearance Section (Parsing Hex)
    else if (MATCH("Appearance", "COLOR_PIXEL")) {
        pchip->config.colorPixel = (uint32_t)strtoul(value, NULL, 0);
    }
    else if (MATCH("Appearance", "COLOR_BG")) {
        pchip->config.colorBG = (uint32_t)strtoul(value, NULL, 0);
    }

    else {
        return 0;  /* Unknown section/name */
    }
    return 1;
}


void initChip8(Chip8* chip) {
    

    if (chip->screen != NULL) {
        free(chip->screen);
    }

    memset(chip, 0, sizeof(Chip8));
    loadConfig(chip, "hardWare.ini");
    chip->PC = PROGRAM_START_ADDRESS;

    //allocate memory for dynamic screen size
    int area = chip->config.screenWidth * chip->config.screenHeight;
    if (area <= 0) area = 64 * 32;
    chip->screen = calloc(area, sizeof(bit8));

   
}




bool loadConfig(Chip8* chip, const char* filename) {
    FILE* file = fopen(filename, "r");

    //default params
    chip->config.screenWidth = 64;
    chip->config.screenHeight = 32;
    chip->config.useClipping = true;
    chip->config.clockSpeed = 500;
    chip->config.targetFPS = 60;

    if (ini_parse(filename, handler, chip) < 0) {
        printf("Can't load 'hardWare.ini'\n");
        return false;
    }

    return true;
}


bool loadFontToChip(bool debugMode, Chip8* chip)
{
    FILE* ptr = fopen("font.bin", "rb");

    if (ptr == NULL) {
        if (debugMode) { printf("Error: Could not open font.bin\n"); }
        return true; 
    }

    size_t bytesRead = fread(&chip->RAM[FONT_ADDRESS_START], sizeof(bit8), 80, ptr);

    if (bytesRead < 80) {
        if (debugMode) { printf("Error: Font file is smaller than expected (read %zu bytes)\n", bytesRead); }
        return false;
    }

    fclose(ptr);
    if (debugMode) { printf("Font loaded successfully to address 0x%03X\n", FONT_ADDRESS_START); } 
}


bool loadRom(bool debugMode, Chip8* chip, FILE* ptr) {
    if (ptr == NULL) {
        if (debugMode) { printf("Error: Could not open ROM (NULL pointer)\n"); }
        return false;
    }

    size_t bytesRead = fread(&chip->RAM[PROGRAM_START_ADDRESS], sizeof(bit8), (RAM_SIZE - PROGRAM_START_ADDRESS), ptr);

    if (bytesRead == 0) {
        if (debugMode) { printf("Error: ROM file is empty or could not be read\n"); }
        fclose(ptr);
        return false;
    }

    if (debugMode) {
        if (debugMode) { printf("ROM loaded successfully. Size: %zu bytes\n", bytesRead); }
    }

    fclose(ptr);
    return true;
}



/*
*   Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
    Each row of 8 pixels is read as bit-coded starting from memory location I; 
    I value does not change after the execution of this instruction. 
    As described above, 
    VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
    and to 0 if that does not happen.
*/
void drawSprite(Chip8* chip, bit8 x, bit8 y, bit8 height) {
    bit16 spriteAddr = chip->I;
    chip->V[0xF] = 0; 

    for (int i = 0; i < height; i++) {
        
        bit8 spriteRow = chip->RAM[spriteAddr + i];

        for (int bit = 0; bit < 8; bit++) {
            
            bit8 pixel = (spriteRow >> (7 - bit)) & 0x1;
            int targetX = x + bit;
            int targetY = y + i;

            if (chip->config.useClipping) { xorPixelWithClipping(chip, pixel, targetX, targetY); }
            else { xorPixel(chip, pixel, targetX, targetY); }
        
        }
    }
}

void xorPixelWithClipping(Chip8* chip, bit8 pixel , int targetX, int targetY)
{
    if (pixel == 0)
        return;

    if (targetX >= chip->config.screenWidth || targetY >= chip->config.screenHeight || targetX < 0 || targetY < 0) {
        return;
    }

    int index = targetX + (targetY * chip->config.screenWidth);


    if (chip->screen[index] == 1) {
        chip->V[0xF] = 1;
    }


    chip->screen[index] ^= 1;
}

void xorPixel(Chip8* chip, bit8 pixel, int targetX, int targetY) {
    if (pixel == 0)
        return;

    int screen_width = chip->config.screenWidth;
    int screen_heigth = chip->config.screenHeight;
    int wrappedX = (targetX % 2*screen_width) % screen_width;
    int wrappedY = (targetY % screen_heigth + screen_heigth) % screen_heigth;

    int index = wrappedX + (wrappedY * screen_width);

    if (chip->screen[index] == 1) {
        chip->V[0xF] = 1;
    }

    chip->screen[index] ^= 1;
}

void handleKeyPress(bool debugMode, Chip8* chip, SDL_Event* e)
{
    if (e->type == SDL_EVENT_KEY_DOWN) {
        if (debugMode) { printf("-------------- key pressed %02X---------------", e->key.key); }
        switch (e->key.key) {
        case SDLK_1: chip->keys[0x1] = 1; break;
        case SDLK_2: chip->keys[0x2] = 1; break;
        case SDLK_3: chip->keys[0x3] = 1; break;
        case SDLK_4: chip->keys[0xC] = 1; break;

        case SDLK_Q: chip->keys[0x4] = 1; break;
        case SDLK_W: chip->keys[0x5] = 1; break;
        case SDLK_E: chip->keys[0x6] = 1; break;
        case SDLK_R: chip->keys[0xD] = 1; break;

        case SDLK_A: chip->keys[0x7] = 1; break;
        case SDLK_S: chip->keys[0x8] = 1; break;
        case SDLK_D: chip->keys[0x9] = 1; break;
        case SDLK_F: chip->keys[0xE] = 1; break;

        case SDLK_Z: chip->keys[0xA] = 1; break;
        case SDLK_X: chip->keys[0x0] = 1; break;
        case SDLK_C: chip->keys[0xB] = 1; break;
        case SDLK_V: chip->keys[0xF] = 1; break;
        }
    }

    // Handle Key Up (Released)
    if (e->type == SDL_EVENT_KEY_UP) {
        switch (e->key.key) {
        case SDLK_1: chip->keys[0x1] = 0; break;
        case SDLK_2: chip->keys[0x2] = 0; break;
        case SDLK_3: chip->keys[0x3] = 0; break;
        case SDLK_4: chip->keys[0xC] = 0; break;

        case SDLK_Q: chip->keys[0x4] = 0; break;
        case SDLK_W: chip->keys[0x5] = 0; break;
        case SDLK_E: chip->keys[0x6] = 0; break;
        case SDLK_R: chip->keys[0xD] = 0; break;

        case SDLK_A: chip->keys[0x7] = 0; break;
        case SDLK_S: chip->keys[0x8] = 0; break;
        case SDLK_D: chip->keys[0x9] = 0; break;
        case SDLK_F: chip->keys[0xE] = 0; break;

        case SDLK_Z: chip->keys[0xA] = 0; break;
        case SDLK_X: chip->keys[0x0] = 0; break;
        case SDLK_C: chip->keys[0xB] = 0; break;
        case SDLK_V: chip->keys[0xF] = 0; break;
        }
    }
}
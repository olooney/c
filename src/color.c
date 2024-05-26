#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} RGB;

float pack_rgb_into_float(const RGB* color) {
    if (!color) return 0.0f; // Safety check

    // Adjust the red component from (0, 255) to (-128, 127)
    int32_t adjusted_red = (int32_t)color->red - 128;

    // Pack the adjusted red, green, and blue into a 32-bit integer
    int32_t packed = (adjusted_red << 16) | ((int32_t)color->green << 8) | (int32_t)color->blue;

    // Convert the packed integer to a float
    return (float)packed;
}

RGB unpack_float_into_rgb(float packed_float) {
    int32_t packed_int = (int32_t)packed_float;

    RGB color;
    color.red = (uint8_t)((packed_int >> 16) + 128);
    color.green = (uint8_t)(packed_int >> 8);
    color.blue = (uint8_t)packed_int;

    return color;
}

int main() {
    srand(time(NULL));  // Seed the random number generator

    for (int i = 0; i < 10; i++) {
        // Generate random RGB values
        RGB color = {
            .red = rand() % 256,
            .green = rand() % 256,
            .blue = rand() % 256
        };

        // Pack the color into a float
        float packed_float = pack_rgb_into_float(&color);
        
        // Unpack the float back into an RGB struct
        RGB unpacked_color = unpack_float_into_rgb(packed_float);

        // Print the original and unpacked RGB values
        printf("Original rgb(%u, %u, %u) - Packed float: %f - Unpacked rgb(%u, %u, %u)\n",
               color.red, color.green, color.blue,
               packed_float,
               unpacked_color.red, unpacked_color.green, unpacked_color.blue);

        // Check if the unpacked color matches the original
        if (color.red != unpacked_color.red || 
            color.green != unpacked_color.green || 
            color.blue != unpacked_color.blue) {
            printf("Error: Mismatch between original and unpacked colors!\n");
        }
    }

    return 0;
}

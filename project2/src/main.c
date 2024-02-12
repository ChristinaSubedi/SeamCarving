#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "energy.h"
#include "image.h"
#include "util.h"

/**
 * Compute the brightness and print the statistics of @p `img`,
 * i.e. width, height & brightness.
 */
void statistics(struct image* img) {
    // TODO implement (assignment 3.1)
    /* implement and use the function:
     * `image_brightness`
     */
    printf("width: %u\n", img->w); 
    printf("height: %u\n", img->h); 
    printf("brightness: %u\n", image_brightness(img));
}

/**
 * Find & print the minimal path of @p `img`.
 */
void find_print_min_path(struct image* img) {
    // TODO implement (assignment 3.2)
    /* implement and use the functions:
     * - `calculate_energy`
     * - `calculate_min_energy_column`
     * - `calculate_optimal_path`
     * in `energy.c`
     */
    uint32_t* energy = calloc(img->w * img->h, sizeof(uint32_t));
    calculate_energy(energy, img, img->w);
    int x=calculate_min_energy_column(energy,img->w,img->w,img->h);
    uint32_t* seam = calloc(img->h, sizeof(uint32_t));
    calculate_optimal_path(energy, img->w, img->w, img->h, x,seam);
    
    for (int x=0; x<img->h; ++x){
        int indice=seam[x];
        printf("%d\n",indice);
    }
    free(energy);
    energy=NULL;
    free(seam);
    seam=NULL;
}

/**
 * Find & carve out @p `n` minimal paths in @p `img`.
 * The image size stays the same, instead for every carved out path there is a
 * column of black pixels appended to the right.
 */
void find_and_carve_path(struct image* const img, int n) {
    // TODO implement (assignment 3.3)
    /* implement and use the functions from assignment 3.2 and:
     * - `carve_path`
     * - `image_write_to_file`
     * in `image.c`.
     */
    uint32_t* energy = calloc(img->w * img->h, sizeof(uint32_t));
    
    
    uint32_t* seam = calloc(img->h, sizeof(uint32_t));
    
    int width=img->w;
    for (int x=0; x<n; ++x){
        calculate_energy(energy, img, width);
        int y=calculate_min_energy_column(energy,img->w,width,img->h);
        calculate_optimal_path(energy, img->w, width, img->h, y,seam);
        carve_path(img, width,seam);

    width-=1;
    }
    const char* filename="out.ppm";
    image_write_to_file(img, filename);

    free(energy);
    energy=NULL;
    free(seam);
    seam=NULL;
}

/**
 * Parse the arguments and call the appropriate functions as specified by the
 * arguments.
 */
int main(int const argc, char** const argv) {
    // DO NOT EDIT
    bool show_min_path = false;
    bool show_statistics = false;
    int n_steps = -1;

    char const* const filename =
        parse_arguments(argc, argv, &show_min_path, &show_statistics, &n_steps);
    if (!filename) return EXIT_FAILURE;

    struct image* img = image_read_from_file(filename);

    if (show_statistics) {
        statistics(img);
        image_destroy(img);
        return EXIT_SUCCESS;
    }

    if (show_min_path) {
        find_print_min_path(img);
    } else {
        if (n_steps < 0 || n_steps > img->w) n_steps = img->w;

        find_and_carve_path(img, n_steps);
    }

    image_destroy(img);
    return EXIT_SUCCESS;
}

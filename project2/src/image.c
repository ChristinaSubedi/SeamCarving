#include "image.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

/**
 * Initialize the image @p `img` with width @p `w` and height @p `h`.
 */
struct image* image_init(int const w, int const h) {
    // DO NOT EDIT
    struct image* img = malloc(sizeof(struct image));
    img->w = w;
    img->h = h;
    img->pixels = malloc(w * h * sizeof(*img->pixels));
    memset(img->pixels, 0, w * h * sizeof(*img->pixels));
    return img;
}

/**
 * Destroy the image @p `img` by freeing its pixels field and by freeing @p
 * `img` itself. Don't use @p img afterwards.
 */
void image_destroy(struct image* img) {
    free(img->pixels);
    free(img);
}

/**
 * Read an image from the file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 * @returns the image that was read.
 */
struct image* image_read_from_file(const char* filename) {
    // DO NOT EDIT
    FILE* f = fopen(filename, "r");
    if (f == NULL) exit(EXIT_FAILURE);

    if (fscanf(f, "P3") == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    int w, h;
    if (fscanf(f, "%d %d 255 ", &w, &h) == EOF) {
        fclose(f);
        exit(EXIT_FAILURE);
    }
    if (w <= 0 || h <= 0) {
        fclose(f);
        exit(EXIT_FAILURE);
    }

    struct image* img = image_init(w, h);
    struct pixel* pixels = img->pixels;

    for (int y = 0; y < img->h; ++y) {
        unsigned int r, g, b;
        for (int x = 0; x < img->w; ++x, ++pixels) {
            if (fscanf(f, "%u %u %u ", &r, &g, &b) == EOF) {
                image_destroy(img);
                fclose(f);
                exit(EXIT_FAILURE);
            }
            pixels->r = r;
            pixels->g = g;
            pixels->b = b;
        }
    }

    if (fgetc(f) != EOF) {
        image_destroy(img);
        fclose(f);
        exit(EXIT_FAILURE);
    }

    fclose(f);
    return img;
}

/**
 * Write the image @p `img` to file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 */
void image_write_to_file(struct image* img, const char* filename) {
    // TODO implement (assignment 3.3)
    FILE* f = fopen(filename, "w");
    if (f == NULL) exit(EXIT_FAILURE);
    fprintf(f,"P3");
    fprintf(f, "\n");
    fprintf(f,"%d %d", img->w, img->h);
    fprintf(f, "\n");
    fprintf(f,"255");
    fprintf(f, "\n");

    int counter=0;

    for (int y = 0; y < img->h; ++y) {
        for (int x = 0; x < img->w; ++x) {
            fprintf(f," %d %d %d", img->pixels[counter].r, 
            img->pixels[counter].g,
            img->pixels[counter].b );
            ++counter;
            fprintf(f, "\n");
           // printf("%d %d %d", img->pixels[counter].r, 
            //img->pixels[counter].g,
            //img->pixels[counter].b );
        }
    }

    fclose(f);




}

/**
 * Compute the brightness of the image @p `img`.
 */

uint8_t image_brightness(struct image* img) {
    // TODO implement (assignment 3.1)
    int sum=0;
    int i=0;
    for (int y = 0; y < img->h; ++y) {
    
        for (int x = 0; x < img->w; ++x) {
            
            sum+=(img->pixels[i].r+ img->pixels[i].g+ img->pixels[i].b)/3;
            ++i;
        }
        
        
}
        sum=sum/(i);
        uint8_t final = sum;
        return final;
}


/**
 * Carve out the path @p `seam` from the image @p `img`,
 * where only the @p `w` left columns are considered.
 * Move all pixels right of it one to the left and fill the rightmost row with
 * black (0,0,0). Columns with index >= `w` are not considered as part of the
 * image.
 */

void move_one(struct image* img, int const w, int index, int height){
    
    int lowpoint=((height-1)*((img->w)))+index;

    
    for(;index+1 <w; ++index){
        
        img->pixels[lowpoint].r=img->pixels[lowpoint+1].r;
        img->pixels[lowpoint].g=img->pixels[lowpoint+1].g;
        img->pixels[lowpoint].b=img->pixels[lowpoint+1].b;
        lowpoint+=1;

    }
     img->pixels[lowpoint].r=0;
        img->pixels[lowpoint].g=0;
        img->pixels[lowpoint].b=0;
}



void carve_path(struct image* const img, int const w,
                uint32_t const* const seam) {
                    int height=img->h;
    for (int x=0; x<img->h;++x){
        move_one(img, w , seam[height-1],height);

        height-=1;
        
    
    }
    
}


#include "energy.h"

#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

uint32_t min(uint32_t a, uint32_t b) { return a < b ? a : b; }

/**
 * Calculate the difference of two color values @p a and @p b.
 * The result is the sum of the squares of the differences of the three (red,
 * green and blue) color components.
 * */
inline uint32_t diff_color(struct pixel const a, struct pixel const b) {
    // TODO implement (assignment 3.2)
    uint32_t diffr, diffb, diffg;
    uint32_t sum;
    diffr= a.r-b.r;
    diffb= a.b-b.b;
    diffg=a.g-b.g;
    diffr= diffr*diffr;
    diffg=diffg*diffg;
    diffb=diffb*diffb;
    sum=diffr+diffb+diffg;
    return sum;
}



uint32_t minimum (uint32_t x, uint32_t y){
    if (x<y){
        return x;
    }
    return y;
}


uint32_t state_energy_diff(struct image* const img, int index){
    uint32_t energytop, energyleft;
    if ((index-(img->w))<0){
        energytop=0;
        
    }else{
    uint32_t indextop= index-(img->w);
        energytop=(diff_color(img->pixels[index],img->pixels[indextop]));
    }
        
    if ((index)%(img->w)==0){
            energyleft=0;
        }
    else{
        uint32_t indexleft= index-1;
        energyleft=(diff_color(img->pixels[index],img->pixels[indexleft]));
}

    return (energyleft+energytop);
    }




void localenergy(uint32_t* const energy, struct image* const img,
                      int const w){
    int index=0;
    for (int y = 0; y < img->h; ++y) {
        for (int x = 0; x < img->w; ++x ){
            if (x<w){
            energy[index]=state_energy_diff(img,index);
            
            
            }
            index+=1;
            
        }
        
    }
    }

void total_energy_diff(uint32_t* const energy, int index, int const w, int const actualwidth){
    uint32_t topleft, topright, top;
    if (index<(w)){
        top=0;
        topright=0;
        topleft=0;
    } else{
        if (((index)%actualwidth)+1>=w){
            topright=energy[index-actualwidth];
        } else {
            topright=energy[index+1-actualwidth];
        }
        if ((index%actualwidth)==0){
            topleft=energy[index-actualwidth];
        }else{
            topleft=energy[index-actualwidth-1];
        }
        top=energy[index-actualwidth];

        

    }

    uint32_t final=minimum(top, minimum(topleft,topright));
    energy[index]=energy[index]+final;


}

void totalenergy(uint32_t* const energy, struct image* const img,
                      int const w){
    int index=0;
    for (int y = 0; y < img->h; ++y) {
        for (int x = 0; x < img->w; ++x ){
            if (x<w){
            total_energy_diff(energy,index,w, img->w);}
            index+=1;
           // printf("The local total at %d is %d\n", index, energy[index]);
            
            }
            
        }
        
    }



void calculate_energy(uint32_t* const energy, struct image* const img, int const w) {
    // TODO implement (assignment 3.2)
    localenergy(energy, img, w );
    totalenergy(energy, img,w );

}

/**
 * Calculate the index of the column with the least energy in bottom row.
 * Expects that @p `energy` holds the energy of every pixel of @p `img` up to
 * column (excluding) @p `w`. Columns with index `>= w` are not considered as
 * part of the image.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */

int calculate_min_energy_column(uint32_t const* const energy, int const w0,
                                int const w, int const h) {
    // TODO implement (assignment 3.2)
    uint32_t index=(h*w0)-w0;
    int result=index;
    for(int x =0; x<w; ++x){
        if (energy[index]<energy[result]){
            result=index;
            
        }
        
        index++;


    }
    result=(result%w0);
    return (result);

    }

/**
 * Calculate the optimal path (i.e. least energy), according to the energy
 * entries in @p `energy` up to (excluding) column @p `w`. The path is stored in
 * @p `seam`. Columns with index `>= w` are not considered as part of the image.
 * @p `x` is the index in the bottom row where the seam starts.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */

int boxabove(int height, uint32_t* const seam,
            uint32_t const* const energy,
            int const widthmarked,
            int const actualwidth, 
             int bottomindex){

                int index, topright, topleft, top;


        index=(height*actualwidth)+bottomindex;
        if ((bottomindex+1>=widthmarked)){
            topright=energy[index-actualwidth];
        } else {
            topright=energy[index+1-actualwidth];
        }
        
        if (bottomindex==0){
            topleft=energy[index-actualwidth];
        }else{
            topleft=energy[index-actualwidth-1];
        }
        top=energy[index-actualwidth];
        
        int minimum= min(topleft, min (top, topright));


        if (minimum==top) { 
            bottomindex=bottomindex+0;
        } else {
            if (minimum==topleft){
                bottomindex=bottomindex-1;
            } else {
                bottomindex=bottomindex+1;
            }

        }

        //printf("The values of topleft, top, and topright are %d %d %d\n", topleft, top, topright);
            seam[height-1]=bottomindex;
            return bottomindex;
            }

        
void calculate_optimal_path(uint32_t const* const energy, int const w0,
                            int const w, int const h, int x,
                            uint32_t* const seam) {
    // TODO implement (assignment 3.2)
    int bottomindex= x;
    int height=h;
    seam[h-1]=bottomindex;
   
    
    for(int y=1; y<h; ++y){
        height=height-1;
        bottomindex=boxabove(height,seam, energy, w, w0, bottomindex);
        }
    }

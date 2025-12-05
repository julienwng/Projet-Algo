/************     binôme leandre.lemoine@universite-paris-saclay.fr
 *                       julien.weng@universite-paris-saclay.fr    ******/
#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} bool; //type booléen

typedef struct bloc_image
{ 
    bool blanc ;
    struct bloc_image * Im[4] ;
} bloc_image ;
typedef bloc_image *image ;

image Wht(){
    bloc_image* b = (bloc_image*) malloc(sizeof(bloc_image));
    b->blanc = true;
    return b;
}

image Blk(){
    return NULL;
}

image Cut(image i0, image i1, image i2, image i3){
    bloc_image *res = (bloc_image*) malloc(sizeof(bloc_image));
    res = false;
    res ->Im[0] = i0;
    res ->Im[1] = i1;
    res ->Im[2] = i2;
    res ->Im[3] = i3;
    return res;
}

void Affiche(image im){

    if(im == NULL) {
    printf("Z");
    return;
    }
    else if (im->blanc == true){
    printf("o");
    return;
    }
    else{
        printf("*");
        for(int i = 0;i < 4; i++) Affiche(im->Im[i]);
    }
}

void ProfAffiche(image im){
    return;

}
int main(){
    bloc_image * im = (bloc_image*) malloc(sizeof(bloc_image));
    im->blanc = false;
    int p = 3;
    for(int i = 0; i < 4; i++){ //Image avec 4 pixels noirs au milieu
        bloc_image *tmp = (bloc_image*) malloc(sizeof(bloc_image));
        im->Im[i] = tmp;
        im->Im[i]->blanc = false;
        for(int j = 0; j < 4; j++){
            if(j == p) im->Im[i]->Im[j] = Blk();
            else im->Im[i]->Im[j] = Wht();
        }
        p--;
    }
    Affiche(im);
    return 0;
}
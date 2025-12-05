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
    for(int i = 0; i < 4; i++){
        b->Im[i] = NULL;
    }
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

void Affiche_bis(image im){
    if(im == NULL){
        printf("Z");
    }
    else if (im->blanc == true){
        printf("o");
    }
    else{
        printf("*");
        for(int i = 0;i < 4; i++){
            Affiche_bis(im->Im[i]);
        }
    }
}

void Affiche(image im){
    Affiche_bis(im);
    printf("\n");
}

void ProfAfficheBis(image im, int cpt){
    if(im == NULL){
        printf("Z%d ", cpt);
    }
    else if (im->blanc == true){
        printf("o%d ", cpt);
    }
    else{
        printf("*%d ", cpt);
        for(int i = 0;i < 4; i++){
            ProfAfficheBis(im->Im[i], cpt+1);
        }
    }
}

void ProfAffiche(image im){
    ProfAfficheBis(im, 0);
    printf("\n");

}

image Lecture(){
    return NULL;
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
    printf("-----Affichage normal------\n");
    Affiche(im);
    printf("-----Affichage Profondeur------\n");
    ProfAffiche(im);
    return 0;
}

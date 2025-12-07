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
    res->blanc = false;
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
image LectureBis(){
    char c;
    do{
        c = getchar();
    } while(c != 'o' && c != 'Z' && c != '*');
    if(c == 'o'){
        return Wht();
    }
    else if(c == 'Z'){
        return Blk();
    }
    else{
        image hg = LectureBis();
        image hd = LectureBis();
        image bg = LectureBis();
        image bd = LectureBis();
        return Cut(hg, hd, bg, bd);
    }
}

image Lecture(){
    printf("Entrez votre image: ");
    return LectureBis();
}

image Copie(image im){
    if(im == NULL) return NULL;

    bloc_image* new = (bloc_image*)malloc(sizeof(bloc_image));
    new->blanc = im->blanc;
    
    for(int i = 0; i < 4; i++){
        new->Im[i] = Copie(im->Im[i]);
    }
    return new;
}

image Diagonale(int p){
    if(p == 0) return NULL;
    bloc_image * res = (bloc_image*) malloc(sizeof(bloc_image));
    res->blanc = false;
    res->Im[1] = Wht();
    res->Im[2] = Wht();
    res->Im[0] = Diagonale(p-1);
    res->Im[3] = Diagonale(p-1);
    return res;
}

image SimplifieProfP(image im, int p){
    if(im == NULL) return NULL;
    if(im->blanc == true) return im;
    for(int i = 0; i<4; i++){
        im->Im[i] = SimplifieProfP(im->Im[i], p-1);
    }
    if(p <= 0 && im->Im[0] == NULL && im->Im[1] == NULL && im->Im[2] == NULL && im->Im[3] == NULL){
        free(im);
        return NULL;
    }
    return im;

}

/*
* Renvoie true si une image est totalement noire
*/
bool EstNoir(image im){
    if(im == NULL) return true;
    if(im ->blanc == true) return false;
    return (EstNoir(im->Im[0]) && EstNoir(im->Im[1]) && EstNoir(im->Im[2]) && EstNoir(im->Im[3]));
}

bool Incluse(image im1, image im2){
    //Cas où im1 est noire, il faut que im2 soit totalement noire aussi
    if(im1 == NULL){
    return (EstNoir(im2));
    }

    //Cas où im1 est blanc ou im2 est noire, toujours vrai pour n'importe quel im2 ou im1
    if (im1->blanc == true || im2 == NULL) return true;

    //Cas où im2 est blanc et im1 n'est pas blanc, toujours faux
    if(im2->blanc == true) return false;

    return Incluse(im1->Im[0], im2->Im[0]) && Incluse(im1->Im[1], im2->Im[1]) && Incluse(im1->Im[2], im2->Im[2]) && Incluse(im1->Im[3], im2->Im[3]);
    
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
    image im2 = Lecture();
    printf("-----Affichage normal------\n");
    Affiche(im);
    Affiche(im2);
    printf("-----Affichage Profondeur------\n");
    ProfAffiche(im);
    ProfAffiche(im2);
    printf("-----Affichage normal------\n");
    Affiche(im);
    printf("-----Affichage Profondeur------\n");
    ProfAffiche(im);
    ProfAffiche(im2);

    image im3 = Copie(im);
    printf("----Affichage de la copie de im----\n");
    Affiche(im3);
    printf("------Affiche une image diagonale de profondeur p----\n entrez la valeur de l'entier p :\n");
    int profondeur;
    scanf("%d",&profondeur);
    image imdiag = Diagonale(profondeur);
    Affiche(imdiag);

    printf("----Test SimplifieProfP----\n");
    int SPP;
    printf("Entrez l'entier pour SimplifieProfP : \n");
    scanf("%d",&SPP); //Exemple 2
    image imProf = Lecture(); //Exemple *(*ZZZZ)(*ZoZo)(*ZoZ(*ZoZ(*oooo)))(*oooo)
    Affiche(SimplifieProfP(imProf,SPP));


    printf("----Test Incluse----\n");
    image imgIncluse1, imgIncluse2;
    printf("Entrez les images 1 et 2\n");
    imgIncluse1 = Lecture(); //Exemple : ***ooooZoZoZ**ooZZoo*ZooZ
    imgIncluse2 = Lecture(); //Exemple : **oZZZ*ooZo*ZZZZ*ZoZ*ZZZo renvoit faux
    printf("%d\n", Incluse(imgIncluse1,imgIncluse2));

    return 0;
}

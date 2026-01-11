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

bool DessinNoir(image im){
    if(im == NULL){
        return true;
    }
    else if(im->blanc == true){
        return false;
    }
    else{
        return DessinNoir(im->Im[0]) && DessinNoir(im->Im[1]) && DessinNoir(im->Im[2]) && DessinNoir(im->Im[3]);
    }
}

bool DessinBlanc(image im){
    if(im == NULL){
        return false;
    }
    else if(im->blanc == true){
        return true;
    }
    else{
        return DessinBlanc(im->Im[0]) && DessinBlanc(im->Im[1]) && DessinBlanc(im->Im[2]) && DessinBlanc(im->Im[3]);
    }
}

float Quota(image im){
    if(im == NULL){
        return 1.0f;
    }
    else if(im->blanc == true){
        return 0.0f;
    }
    else{
        return (Quota(im->Im[0])+Quota(im->Im[1])
                +Quota(im->Im[2])+Quota(im->Im[3])) / 4.0f;
    }
}
image Copie(image im){
    if(im == NULL) return Blk();
    
    if(im->blanc) return Wht();
    
    return Cut(Copie(im->Im[0]), Copie(im->Im[1]), Copie(im->Im[2]), Copie(im->Im[3]));
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

/*
* Permet de free (surtout le cas où im->blanc est false)
*/
void freeTout(image im){
    if(im == NULL) return;
    if(!im->blanc){
        for(int i = 0; i < 4; i++){
            freeTout(im->Im[i]);
        }
    }
    free(im);
}

image SimplifieProfP(image im, int p){
    if(im == NULL) return NULL;
    if(im->blanc == true) return im;
    for(int i = 0; i<4; i++){
        im->Im[i] = SimplifieProfP(im->Im[i], p-1);
    }
    if(p <= 0){
        if(DessinNoir(im)){
            freeTout(im);
            return NULL;
        }
        if(DessinBlanc(im)){
            for(int i = 0; i < 4; i++){
                freeTout(im);
                im->Im[i] = NULL;
            }
            im->blanc = true;
            return im;
        }
    }
    return im;
}

bool Incluse(image im1, image im2){
    //Cas où im1 est noire, il faut que im2 soit totalement noire aussi
    if(im1 == NULL){
        return (DessinNoir(im2));
    }

    //Cas où im1 est blanc ou im2 est noire, toujours vrai pour n'importe quel im2 ou im1
    if (im1->blanc == true || im2 == NULL){
        return true;
    }

    //Cas où im2 est blanc et im1 n'est pas blanc, si im1 est totalement blanc alors vrai, sinon faux
    if(im2->blanc == true){
        return DessinBlanc(im1);
    }

    return Incluse(im1->Im[0], im2->Im[0]) && Incluse(im1->Im[1], im2->Im[1]) &&
           Incluse(im1->Im[2], im2->Im[2]) && Incluse(im1->Im[3], im2->Im[3]);
    
}

void CompteQuotaGris(image im, int *cpt, float *quota){
    if(im == NULL){
        *quota = 1.0f;
    }
    else if(im->blanc == true){
        *quota = 0.0f;
    }
    else{
        float q0, q1, q2, q3;
        CompteQuotaGris(im->Im[0], cpt, &q0);
        CompteQuotaGris(im->Im[1], cpt, &q1);
        CompteQuotaGris(im->Im[2], cpt, &q2);
        CompteQuotaGris(im->Im[3], cpt, &q3);
        *quota = (q0 + q1 + q2 + q3)/4.0f;
    if(*quota >= 1.0f/3.0f && *quota <= 2.0f/3.0f) (*cpt)++;
    }
}

int CompteSousImagesGris(image im){
    int cpt = 0;
    float quota;
    CompteQuotaGris(im, &cpt, &quota);
    return cpt;
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

    printf("------Test Image Noire--------\n");
    bool resNoir = DessinNoir(im2);
    printf(resNoir ? "true\n" : "false\n");
    
    printf("------Test Image Blanche--------\n");
    bool resBlanc = DessinBlanc(im2);
    printf(resBlanc ? "true\n" : "false\n");
    
    printf("-----------Test Quota------------\n");
    printf("Exemple : *Z*oZooZ*ZZZo, quota attendu 0.75\n");
    image im3 = Lecture();
    ProfAffiche(im3);
    printf("Quota = %.2f\n", Quota(im3));
    
    image im4 = Copie(im);
    printf("----Affichage de la copie de im----\n");
    Affiche(im4);
    printf("------Affiche une image diagonale de profondeur p----\n");
    printf("entrez la valeur de l'entier p: ");
    int profondeur;
    scanf("%d",&profondeur);
    image imdiag = Diagonale(profondeur);
    Affiche(imdiag);
    
    printf("----Test SimplifieProfP----\n");
    printf("Exemple pour p = 2 et\n * (*ZZZZ) (*Zo(*Z(*Z(*ZZZZ)(*ZZZZ)");
    printf("(*ZZZZ))ZZ)o)(*ZoZ(*ZoZ(*oooo))) (*oo(*oooo)o) deviens :\n");
    printf("*(*ZZZZ)(*ZoZo)(*ZoZ(*ZoZ(*oooo)))(*oooo)\n");
    int SPP;
    printf("Entrez l'entier pour SimplifieProfP: ");
    scanf("%d",&SPP); //Exemple 2
    image imProf = Lecture(); //Exemple *(*ZZZZ)(*ZoZo)(*ZoZ(*ZoZ(*oooo)))(*oooo)
    Affiche(SimplifieProfP(imProf,SPP));


    printf("----Test Incluse----\n");
    image imgIncluse1, imgIncluse2;
    printf("Entrez les images 1 et 2\n");
    printf("(Exemple : ***ooooZoZoZ**ooZZoo*ZooZ et **oZZZ*ooZo*ZZZZ*ZoZ*ZZZo)\n");
    printf("(Doit renvoyer faux)\n");
    printf("Image 1\n");
    imgIncluse1 = Lecture(); //Exemple : ***ooooZoZoZ**ooZZoo*ZooZ
    printf("Image 2\n");
    imgIncluse2 = Lecture(); //Exemple : **oZZZ*ooZo*ZZZZ*ZoZ*ZZZo renvoit faux
    printf(Incluse(imgIncluse1,imgIncluse2) ? "true\n" : "false\n");

    printf("----Test CompteSousImagesGris-----\n");
    printf("Exemple : *oZ*Z*oZooo*Zooo *Z*oZooo*ZZoo renvoie 4\n");
    image testgris = Lecture();
    ProfAffiche(testgris);
    printf("Nb de sous images grises : %d\n", CompteSousImagesGris(testgris));

    return 0;
}

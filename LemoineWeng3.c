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

float QuotaNoir(image im){
    if(im == NULL){
        return 1.0f;
    }
    else if(im->blanc == true){
        return 0.0f;
    }
    else{
        return (QuotaNoir(im->Im[0])+QuotaNoir(im->Im[1])
                +QuotaNoir(im->Im[2])+QuotaNoir(im->Im[3])) / 4.0f;
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
                if(im->Im[i] != NULL){
                    freeTout(im->Im[i]);
                }
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

//Calcul de la profondeur max de l'image
int Profondeur(image im){
    if(im == NULL || im->blanc == true){
        return 0;
    }
    int maxP = 0;
    for(int i = 0; i < 4; i++){
        int p = Profondeur(im->Im[i]);
        if(p > maxP){
            maxP = p;
        }
    }
    return maxP + 1;
}

//Remplit la grille correspondant à l'image
void ToGrille(image im, int **grille, int x, int y, int taille){
    if(im == NULL){ 
        for(int i = 0; i < taille; i++){
            for(int j = 0; j < taille; j++){
                grille[x+i][y+j] = 0; //mur (noir)
            }
        }
    }
    else if(im -> blanc == true){
        for(int i = 0; i < taille; i++){
            for(int j = 0; j < taille; j++){
                grille[x+i][y+j] = 1; //chemin (blanc)
            }
        }
    }
    else{ //On divise en 4 sous grilles et on appelle récursivement
        int n_taille = taille/2;
        ToGrille(im->Im[0], grille, x, y, n_taille);
        ToGrille(im->Im[1], grille, x, y + n_taille, n_taille);
        ToGrille(im->Im[2], grille, x + n_taille, y, n_taille);
        ToGrille(im->Im[3], grille, x + n_taille, y + n_taille, n_taille);
    }
}

//On parcours en profondeur la grille pour chercher un chemin
bool DFS(int **grille, int **visite, int x, int y, int N){
    if(x < 0 || y < 0 || x >= N || y >= N){ //On est hors de la grille
        return false;
    }
    if(grille[x][y] == 0 || visite[x][y] == 1){ //mur ou case déjà visitée
        return false;
    }
    if(x == N-1 && y == N-1){ //On a atteint la sortie
        return true;
    }

    visite[x][y] = 1; //case visitée

    //On explore les 4 directions en priorisant droite et bas
    if(DFS(grille, visite, x+1, y, N)) return true;
    if(DFS(grille, visite, x, y+1, N)) return true;
    if(DFS(grille, visite, x-1, y, N)) return true;
    if(DFS(grille, visite, x, y-1, N)) return true;

    return false; //Aucun chemin trouvé à partir de cette case
}

bool Labyrinthe(image im){
    if(im == NULL){
        return false;
    }
    if(im->blanc == true){
        return true;
    }

    int p = Profondeur(im);
    int N = 1 << p; //Décalage de p bits à gauche pour trouver 2^p
                    //On divise p fois en 2 donc 2^p cases par coté

    int **grille = (int**)malloc(N*sizeof(int*));
    int **visite = (int**)malloc(N*sizeof(int*));
    for(int i = 0; i < N; i++){
        grille[i] = (int*)malloc(N*sizeof(int));
        visite[i] = (int*)malloc(N*sizeof(int));
    }

    ToGrille(im, grille, 0, 0, N); //On remplit la grille

    bool res = DFS(grille, visite, 0, 0, N); //On applique le DFS pour chercher un chemin

    //On libère la mémoire allouée
    for(int i = 0; i < N; i++){
        free(grille[i]);
        free(visite[i]);
    }
    free(grille);
    free(visite);

    return res;
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
    printf("Affichage de l'exemple donné dans l'énonce :\n");
    Affiche(im);
    printf("Affichage de l'image entrée :\n");
    Affiche(im2);
    printf("-----Affichage Profondeur------\n");
    printf("Affichage de l'exemple donné dans l'énonce :\n");
    ProfAffiche(im);
    printf("Affichage de l'image entrée :\n");
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
    printf("Quota = %.2f\n", QuotaNoir(im3));
    
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

    printf("----Test Labyrinthe-----\n");
    printf("Exemple 1: ***ooZo**ZZoooZZ*Zoo*ZooZZ***ooZZoZZ*o*ooZoo*Zooo*oZ*oooZo*Z*oZoZoo*Z**ooZoooZZ*oooZ**oZZZZ*oooZ*oZoo");
    printf(" Renvoie vrai\n");
    printf("Exemple 2: ***ooZo**ZZoooZZ*Zoo*ZooZZ***ooZZoZZ*o*ooZoo*Zooo*oZ*ZooZo*Z*oZoZoo*Z**ooZoooZZ*oooZ**oZZZZ*oooZ*oZoo");
    printf(" Renvoie faux\n");
    image laby1 = Lecture();
    ProfAffiche(laby1);
    printf("Le labyrinthe possède une solution : %s", Labyrinthe(laby1) ? "vrai\n" : "faux\n");
    image laby2 = Lecture();
    ProfAffiche(laby2);
    printf("Le labyrinthe possède une solution : %s", Labyrinthe(laby2) ? "vrai\n" : "faux\n");

    freeTout(im);
    freeTout(im2);
    freeTout(im3);
    freeTout(im4);
    freeTout(imdiag);
    freeTout(imProf);
    freeTout(imgIncluse1);
    freeTout(imgIncluse2);
    freeTout(testgris);
    freeTout(laby1);
    freeTout(laby2);
    
    return 0;
}

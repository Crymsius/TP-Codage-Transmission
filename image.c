#include "image.h"


/*
 * Lecture d'une ligne du fichier.
 * On saute les lignes commençant par un "#" (commentaire)
 * On simplifie en considérant que les lignes ne dépassent pas MAXLIGNE
 */

void lire_ligne(FILE *f, char *ligne)
{
    do {
        fgets (ligne, MAXLIGNE, f);
    } while (ligne[0] == '#');
}

/*
 * Allocation d'une image
 */

struct image* allocation_image(int hauteur, int largeur)
{
    struct image* img;
    ALLOUER (img, 1);
    img->hauteur = hauteur;
    img->largeur = largeur;
    ALLOUER(img->pixels, hauteur);
    for (int i=0; i < hauteur; ++i){
        ALLOUER(img->pixels[i], largeur);
    }
    return img;
}

/*
 * Libération image
 */

void liberation_image(struct image* image)
{
    for (int i=0; i < image->hauteur; ++i){
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}

/*
 * Allocation et lecture d'un image au format PGM.
 * (L'entête commence par "P5\nLargeur Hauteur\n255\n"
 * Avec des lignes de commentaire possibles avant la dernière.
 */

struct image* lecture_image(FILE *f)
{
    char ligne[MAXLIGNE];
    int hauteur, largeur;
    lire_ligne(f, ligne);
    lire_ligne(f, ligne);

    sscanf(ligne,"%d %d",&largeur, &hauteur);
    lire_ligne(f, ligne);

    struct image* img = allocation_image(hauteur, largeur);

    for (int i = 0; i < hauteur; i++) {
        fread(img->pixels[i], 1, largeur, f);
    }
    return img;
}

/*
 * Écriture de l'image (toujours au format PGM)
 */

void ecriture_image(FILE *f, const struct image *image)
{
    int largeur = image->largeur;
    int hauteur = image->hauteur;
    fprintf(f, "P5\n%d %d\n255\n",largeur ,hauteur);

    for (int i = 0; i < hauteur; i++) {
        fwrite(image->pixels[i], 1, largeur, f);
    }
}

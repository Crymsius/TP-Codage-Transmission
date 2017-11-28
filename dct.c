#include "bases.h"
#include "matrice.h"
#include "dct.h"

/*
 * La fonction calculant les coefficients de la DCT (et donc de l'inverse)
 * car la matrice de l'inverse DCT est la transposée de la matrice DCT
 *
 * Cette fonction prend beaucoup de temps.
 * il faut que VOUS l'utilisiez le moins possible (UNE SEULE FOIS)
 *
 * FAITES LES CALCULS EN "double"
 *
 * La valeur de Pi est : M_PI
 *
 * Pour ne pas avoir de problèmes dans la suite du TP, indice vos tableau
 * avec [j][i] et non [i][j].
 */

void coef_dct(Matrice *table)
{
	int n = table->height;
	int i,j;
	for (j=0; j < n; j++) {
		for (i=0; i < n; i++) {
				table->t[j][i] = (j == 0 ? 1 : sqrt(2) * cos(j*M_PI*(2*i+1)/(2*n))) /sqrt(n);
		}
	}
}

/*
 * La fonction calculant la DCT ou son inverse.
 *
 * Cette fonction va être appelée très souvent pour faire
 * la DCT du son ou de l'image (nombreux paquets).
 */

void dct(int   inverse,		/* ==0: DCT, !=0 DCT inverse */
	 int nbe,		/* Nombre d'échantillons  */
	 const float *entree,	/* Le son avant transformation (DCT/INVDCT) */
	 float *sortie		/* Le son après transformation */
	 )
{
	Matrice * coefs;
	coefs = allocation_matrice_float(nbe,nbe);
	coef_dct(coefs);

	if(inverse){
		Matrice * inv_coefs;
		inv_coefs = allocation_matrice_float(nbe,nbe);
		transposition_matrice(coefs, inv_coefs);
		produit_matrice_vecteur(inv_coefs, entree, sortie);
	} else {
		produit_matrice_vecteur(coefs, entree, sortie);
	}
}

#include "bits.h"
#include "entier.h"

/*
 * Les fonctions de ce fichier permette d'encoder et de décoder
 * des entiers en utilisant des codes statiques.
 */

/*
 * Codage d'un entier (entre 0 et 32767 inclus) en une chaine de bits
 * qui est écrite dans le bitstream.
 *
 * Le nombre est codé par la concaténation du PREFIXE et SUFFIXE
 * Le suffixe est en fait le nombre entier sauf le premier bit a 1
 * 
 * Nombre de bits |    PRÉFIXE     | nombres codés | SUFFIXE
 *       0        |       00       |      0        |
 *     	 1        |       010      |  1 (pas 0)    |
 *     	 2        |       011      |     2-3       | 2=0 3=1
 *     	 3        |      1000      |     4-7       | 4=00 5=01 6=10 7=11
 *     	 4        |      1001      |     8-15      | 8=000 ... 15=111
 *     	 5        |      1010      |    16-31      | 16=0000 ... 31=1111
 *     	 6        |      1011      |    32-63      |
 *     	 7        |      11000     |    64-127     |
 *     	 8        |      11001     |   128-255     |
 *     	 9        |      11010     |   256-511     |
 *     	 10       |      11011     |   512-1023    |
 *     	 11       |      11100     |  1024-2047    |
 *     	 12       |      11101     |  2048-4097    |
 *     	 13       |      11110     |  4096-8191    |
 *     	 14       |      111110    |  8192-16383   |
 *     	 15       |      111111    | 16384-32767   |
 *
 * Je vous conseille de faire EXIT si l'entier est trop grand.
 *
 */

static char *prefixes[] = { "00", "010", "011", "1000", "1001", "1010", "1011",
			    "11000", "11001", "11010", "11011", "11100",
			    "11101", "11110", "111110", "111111" } ;

void put_entier(struct bitstream *b, unsigned int f)
{
	unsigned int a = nb_bits_utile(f);
	if (a > 15)
		EXIT;
	put_bit_string(b, prefixes[a]);
	put_bits(b,a-1,f);
}

/*
 * Cette fonction fait l'inverse de la précédente.
 *
 * Un implémentation propre, extensible serait d'utiliser
 * un arbre binaire comme pour le décodage d'Huffman.
 * Ou bien parcourir l'arbre des états 8 bits par 8 bits (voir le cours)
 * Mais je ne vous le demande pas
 */

unsigned int get_entier(struct bitstream *b)
{
	unsigned int prefixe = get_bits(b, 2);
	unsigned int nb_bits = 0;
	unsigned int result = 0;
	unsigned int temp = 0;

	if (prefixe == 0)
		return 0;
	else if (prefixe == 1) {
		prefixe = get_bits(b, 1);
		if (prefixe == 0)
			return 1;
		else
			nb_bits = 1;
	} else if (prefixe == 2) {
		prefixe = get_bits(b, 2);
		temp = 0;
		nb_bits = 2;
		while (temp < prefixe) {
			temp++;
			nb_bits++;
		}
	} else if (prefixe == 3) {
		prefixe = get_bits(b, 3);
		temp = 0;
		nb_bits = 6;
		while (temp < prefixe && temp < 7) {
			temp++;
			nb_bits++;
		}
		if (prefixe == 7) {
			prefixe = get_bits(b, 1);
			temp = 0;
			while (temp < prefixe) {
				temp++;
				nb_bits++;
			}
		}
	}
	result = get_bits(b, nb_bits);
	result = pose_bit(result, nb_bits, Vrai);
	return result;
}

/*
 * Operation sur des entiers signés
 *
 * Si l'entier est signé, il est précédé d'un bit à 1:negatif et 0:positif
 * On considère que l'entier 0 est positif donc on pourra ajouter
 * 1 aux nombres négatif pour récupérer la place du zero négatif.
 *    2 --> 0 2
 *    1 --> 0 1
 *    0 --> 0 0
 *   -1 --> 1 0
 *   -2 --> 1 1
 *   -3 --> 1 2
 *
 */

void put_entier_signe(struct bitstream *b, int i)
{
	if (i >= 0) {
		put_bit(b, Faux);
		put_entier(b, (unsigned int) i);
	} else {
		i = -i;
		put_bit(b, Vrai);
		put_entier(b, (unsigned int) i-1);
	}
}
/*
 *
 */
int get_entier_signe(struct bitstream *b)
{
	unsigned int sign = get_bits(b, 1);
	if (sign == 0)
		return get_entier(b);
	else
		return - get_entier(b) - 1;
}

#include <stdio.h>
#include <stdlib.h>
#include "css_optimizer.c"

int main (int argc, char **argv) 
{
	// Test si le nombre d'arguments entres est suffisant
	if (argc < 2) {
		printf("\nYour number of parameters is inadequate\n\n");
		return 1;
	}
	// Test si le fichier des destination est deja existant
	FILE *fichier = fopen(argv[argc-1], "r");
	if (fichier != NULL) {	
		printf("\nYour destination file already exists\n\n");
		fclose(fichier);
		return 1;
	}
    int *verif_b = malloc (sizeof(int));
	// Verification si on a -b ou -h
    int i, j = verif_option (argc, argv, verif_b);
    if (j == 0)
        return 0;
    struct_css *css = NULL, *css2 = NULL; 
	fichier = NULL;
	// Test si les fichiers à tester saisis existent
 	css = lecture (fichier, argv[j]);
    for (i=j+1; i<argc-1; i++) {
        css2 = lecture (fichier, argv[i]);
		if (css2 == NULL) {
			printf("\nYour file doesn't exist\n\n");
			return 0;
		}
        css_fusion (css, css2);
    } 
	//  Rangement des balises et propriétes, avec suppression des doublons, dans liste
    css = css_optimisation (css);
	// Ecriture dans le fichier destination des elements rangés
	css_ecriture_fichier (css, argv[argc-1], *verif_b);
	printf("\nYour optimization has finished correctly\n\n");
	return 0;
}

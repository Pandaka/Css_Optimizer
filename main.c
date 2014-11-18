#include <stdio.h>
#include <stdlib.h>
#include "css_optimizer.c"

int main (int argc, char **argv) 
{
	if (argc < 2) {
		printf("\nYour number of parameters is inadequate\n\n");
		return 1;
	}
	FILE *fichier = fopen(argv[argc-1], "r");
	if (fichier != NULL) {	
		printf("\nYour destination file already exists\n\n");
		fclose(fichier);
		return 1;
	}
    int *verif_b = malloc (sizeof(int));
    int i, j = verif_option (argc, argv, verif_b);
    if (j == 0)
        return 0;
    struct_css *css = NULL, *css2 = NULL; 
	fichier = NULL;
 	css = lecture (fichier, argv[j]);
    for (i=j+1; i<argc-1; i++) {
        css2 = lecture (fichier, argv[i]);
		if (css2 == NULL) {
			printf("\nYour file doesn't exist\n\n");
			return 0;
		}
        css_fusion (css, css2);
    }  
    css = css_optimisation (css);
	css_ecriture_fichier (css, argv[argc-1], *verif_b);
	printf("\nYour optimization is finished correctly\n\n");
	return 0;
}

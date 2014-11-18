#include <stdio.h>
#include <stdlib.h>
#include "css_optimizer.h"

struct_css *lecture (FILE *fichier, char *chemin) {
    struct_chaine *balise = NULL, *regle = NULL;
    fichier = fopen(chemin, "r");
    int i, curseur_actuel = 0, curseur_precedent=0, commentaire=0;
    struct_css *css = NULL;
    t_list *tmp = creation_liste(1), *tmp_chaine = creation_liste(1), *tmp_struct_chaine = creation_liste(1), *tmp_balise = NULL, *tmp_regles = NULL;
    if (fichier != NULL) {
        do {
            curseur_actuel = fgetc(fichier);
            if (curseur_actuel == '*' && curseur_precedent == '/')
            	commentaire = 1;
            if (curseur_actuel == '/' && curseur_precedent == '*')
            	commentaire = 0;
            curseur_precedent = curseur_actuel;
            if (commentaire == 1 || curseur_actuel == '/')
            	continue;
            if (curseur_actuel == '{') {
            	tmp_balise = creation_liste(tmp_struct_chaine->longueur);           	
            	for (i=0; i<tmp_struct_chaine->longueur; i++) {
            		ajout_liste(tmp_balise, tmp_struct_chaine->elements[i]);
            	}
            	for (i=0; i<tmp_balise->longueur; i++) {
            		if (tmp_struct_chaine->elements[i] == '}' || tmp_struct_chaine->elements[i] < 32 || tmp_struct_chaine->elements[i] == ' ' )
            			continue;
                    ajout_liste(tmp_chaine, tmp_balise->elements[i]);
            	}
				tmp_struct_chaine->longueur = 0;
                balise = NULL;
                for (i=0; i<tmp_chaine->longueur; i++) {
                    if (tmp_chaine->elements[i] == ',') {
                        balise = ajout_chaine(balise, tmp->elements);
                        tmp = creation_liste(1);
                        continue;
                    }
                    ajout_liste(tmp, tmp_chaine->elements[i]);                            
                }
                balise = ajout_chaine(balise, tmp->elements);
                tmp = creation_liste(1);
                tmp_chaine = creation_liste(1);                     
            }
            if (curseur_actuel == '}') {
             	tmp_regles = creation_liste(tmp_struct_chaine->longueur);
            	for (i=0; i<tmp_struct_chaine->longueur; i++) {
            		ajout_liste(tmp_regles, tmp_struct_chaine->elements[i]);
            	}
            	for (i=0; i<tmp_regles->longueur; i++) {
            		if (tmp_struct_chaine->elements[i] == '{' || tmp_struct_chaine->elements[i] == 9 || tmp_struct_chaine->elements[i] == ' ' || tmp_struct_chaine->elements[i] == 10)
            			continue;
            		ajout_liste(tmp_chaine, tmp_regles->elements[i]);
            	}
            	tmp_struct_chaine->longueur = 0; 
                regle = NULL;
                for (i=0; i<tmp_chaine->longueur; i++) {                           
                    if (tmp_chaine->elements[i] == ';') {
                        regle = ajout_chaine(regle,tmp->elements);
                        tmp = creation_liste(1);
                        continue;
                    }
                    ajout_liste(tmp, tmp_chaine->elements[i]);                            
                }                
                tmp = creation_liste(1);
                tmp_chaine = creation_liste(1);     
                css=css_ajout(css, regle, balise);
            }
            if (curseur_actuel != EOF)
            ajout_liste(tmp_struct_chaine, curseur_actuel);
        } while (curseur_actuel != EOF);
        return css;
    }
    else return NULL;
}

int verif_option (int argc, char **argv, int *verif_b) {
    int i, j=1;
        for (i=1; i<argc; i++) {
		    if (chaine_comparaison(argv[i], "-b") == 1) {
		        *verif_b = 1;
				j++;
        }
        if (chaine_comparaison(argv[i], "-h") == 1) {
            printf("\nNAME\n"
                "    css_optimize - Reads css files and optimizes it in the new css file\n\n"
				"SYNOPSIS\n"
				"    css_optimize [OPTION] [SOURCES ...] [DESTINATION]\n\n"
				"DESCRIPTION\n"
                "    -b, --beautify\n"
                "        Reads css files, optimizes and formates it in the new css file\n"
                "    -h, --help\n"
                "        Displays this help\n\n");    
            return 0;  
        }
    }
    return j;
}

char *allocation (char *chaine) {
	char *nouvelle_chaine;
	if (chaine) {
		nouvelle_chaine = malloc(sizeof(char)*chaine_longueur(chaine));
		nouvelle_chaine = chaine;
		return nouvelle_chaine;
	}
	return NULL;
}

int chaine_longueur (char *chaine) {
	int cpt = 0;	
	if (chaine) {
		while (chaine[cpt] != '\0')
			cpt++;
		return cpt;
	}
	else {
		return 0;
	}
}

int chaine_comparaison (char *chaine1, char *chaine2) {
	int cpt = 0;
	if (!chaine1) {
		return 0;
	}
	if (!chaine2) {
		return 0;
	}
	while (chaine1[cpt] != '\0') {
		if(chaine1[cpt] != chaine2[cpt])
			return 0;
		cpt++;
	}
	if(chaine1[cpt] != chaine2[cpt])
		return 0;
	return 1;
}

int chaine_propriete (char *propriete1, char *propriete2) {
	int i;
	t_list *tmp1 = creation_liste(1), *tmp2 = creation_liste(1);
	for (i=0; propriete1[i]!=':'; i++)
		ajout_liste (tmp1, propriete1[i]);
	for (i=0; propriete2[i]!=':'; i++)
		ajout_liste (tmp2, propriete2[i]);
	if (chaine_comparaison(tmp1->elements, tmp2->elements) == 1)
		return 1;
	return 0;
}

struct_chaine *creation_chaine (char *chaine) {
	struct_chaine * tmp = malloc(sizeof(struct_chaine));
	tmp->chaine = chaine;
	tmp->next = NULL;
	return tmp;
}

struct_chaine *ajout_chaine (struct_chaine *liste, char *chaine) {
	if (!liste)
        return (struct_chaine*)creation_chaine(chaine);
   	struct_chaine *tmp;
    	tmp = liste;
   	while (tmp->next != NULL)
    	tmp = tmp->next;
    tmp->next = creation_chaine(chaine);
    return liste;
}

struct_chaine *concat_chaine (struct_chaine *source1, struct_chaine *source2) {
	struct_chaine *tmp1 = NULL, *tmp2 = NULL;
	if (source1) {
		tmp1 = source1;
		while (tmp1->next != NULL)
			tmp1 = tmp1->next;
	}
	if (source2) {
	   	tmp2=source2;
	   	while (tmp2->next != NULL) {
			source1 = ajout_chaine(source1,tmp2->chaine); 		
			tmp2 = tmp2->next;
	   	}
		source1 = ajout_chaine(source1, allocation(tmp2->chaine));	
	}
	return source1;
}

int chaine_comp (struct_chaine *source1, struct_chaine *source2) {
	if (chaine_longueur_liste(source1) != chaine_longueur_liste(source2))
		return 0;
	int recherche;
	struct_chaine * tmp;
	tmp = source1;
	while (tmp->next != NULL) {
		recherche = chaine_recherche(source2, tmp->chaine);
		if (recherche == -1)
			return 0;
	    tmp=tmp->next;
	}
	recherche = chaine_recherche(source2, tmp->chaine);
	if (recherche == -1)
		return 0;
	else return 1;
}

int chaine_longueur_liste (struct_chaine *liste) {
	struct_chaine *tmp;
	tmp = liste;
	int cpt = 0;
	while (tmp->next != NULL) {
		cpt++;
	    tmp = tmp->next;
	}
	cpt++;
	return cpt;
}

int chaine_recherche (struct_chaine *liste, char *chaine) {
	struct_chaine *tmp = liste;
	int cpt = 0;
	while (tmp->next != NULL) {
		if (chaine_comparaison(chaine, tmp->chaine))
			return cpt;
		cpt++;
	    tmp = tmp->next;
	}
	if (chaine_comparaison(chaine, tmp->chaine))
		return cpt;
	return -1;
}

struct_chaine *supprime_chaine (struct_chaine *liste, int position) {
	if (0 <= position && chaine_longueur_liste(liste) > position) {	
		int cpt=0;
		struct_chaine *tmp, *precedent;
		if (position == 0)
			liste = liste->next;
		else {
			precedent = liste;
			tmp = liste;
			while (cpt < position) {
				cpt++;
				precedent = tmp;
				tmp = tmp->next;
			}
			precedent->next = tmp->next;
		}
	}
	return liste;
}

int chaine_cherche_propriete (struct_chaine *liste, char *chaine) {
	struct_chaine *tmp;
	tmp = liste;
	int cpt = 0;
	while (tmp->next != NULL) {
		if (chaine_propriete(chaine, tmp->chaine))
			return cpt;	
		cpt++;
	    tmp = tmp->next;
	}
	if (chaine_propriete(chaine, tmp->chaine))
		return cpt;
	return -1;
}

struct_css *css_creation (struct_chaine *propriete, struct_chaine *balise) {
	struct_css *tmp = malloc(sizeof(struct_css));
	tmp->propriete = propriete;
	tmp->balise = balise;
	tmp->next = NULL;
	return tmp;
}

struct_css *css_fusion (struct_css *contenu, struct_css *ajout) {
	while (contenu->next != NULL)
    	contenu=contenu->next;
   	contenu->next = ajout;
   	return contenu;
}

struct_css *css_ajout (struct_css *liste, struct_chaine *propriete, struct_chaine *balise) {
	if (!liste)
        return (struct_css*)css_creation(propriete, balise);
   	struct_css *tmp = liste;
   	while (tmp->next != NULL)
    		tmp=tmp->next;
    tmp->next = css_creation(propriete, balise);
    return liste;
}

struct_css *css_optimisation (struct_css *css) {
	struct_css *optimisation = NULL, *tmp = css;
   	while (tmp->next != NULL) {
		optimisation = css_optimisation_bloc(tmp, optimisation);
		tmp = tmp->next;
   	}
	optimisation = css_optimisation_bloc(tmp, optimisation);
	return optimisation;
}

struct_css *css_optimisation_bloc (struct_css *css, struct_css *optimisation) {
	struct_chaine *tmp;
   	tmp = css->propriete;
   	while (tmp->next != NULL) {
		optimisation = css_optimisation_propriete(css, optimisation, tmp->chaine);
     	tmp = tmp->next;
   	}
	optimisation = css_optimisation_propriete(css, optimisation, tmp->chaine);
	return optimisation;
}

struct_css *css_optimisation_propriete (struct_css *css, struct_css *optimisation, char *propriete) {
	struct_chaine *balises = NULL;
 	balises = concat_chaine (balises, css->balise);
	int cmp = 0, recherche;
	struct_css *tmp = css->next;
	if (tmp) {
		while (tmp->next != NULL) {
			recherche = chaine_recherche(tmp->propriete, propriete);
			if (recherche != -1) {
				balises = concat_chaine (balises, tmp->balise);
				tmp->propriete = supprime_chaine(tmp->propriete, recherche);
			}
			tmp = tmp->next;
		}
		recherche = chaine_recherche(tmp->propriete, propriete);
		if (recherche != -1) {
			balises = concat_chaine (balises, tmp->balise);
			tmp->propriete = supprime_chaine(tmp->propriete, recherche);
		}
	}	
	tmp = optimisation;
	if (optimisation) {
		while (tmp->next != NULL) {
			cmp = chaine_comp(balises, tmp->balise);
			if (cmp) {
				tmp->propriete = supprime_chaine(tmp->propriete, chaine_cherche_propriete(tmp->propriete, propriete));
				tmp->propriete = ajout_chaine(tmp->propriete, allocation(propriete));		
				break;
			}
			tmp = tmp->next;
		}	
		if (!cmp) {
			cmp = chaine_comp(balises, tmp->balise);
			if (cmp) {
				tmp->propriete = supprime_chaine(tmp->propriete, chaine_cherche_propriete(tmp->propriete, propriete));
				tmp->propriete = ajout_chaine(tmp->propriete, allocation(propriete));		
			}
		}
	}
	if (!cmp)
		optimisation = css_ajout(optimisation, creation_chaine(allocation(propriete)), balises);
	return optimisation;
}

void css_ecriture_fichier (struct_css *css, char *nom_fichier, int option) {
	if (css) {
		FILE *fichier = fopen(nom_fichier, "w");
		if (fichier == NULL) {
			printf("\nThere is a problem with the creation of your destination file\n\n");
			return ;
		}
		struct_css *tmp1 = css;
		struct_chaine *tmp2;
	    while (tmp1->next != NULL) {
			tmp2 = tmp1->balise;			
			while (tmp2->next != NULL) {
				fputs(tmp2->chaine, fichier);
				fputc(',', fichier);
				tmp2 = tmp2->next;
			}
			fputs(tmp2->chaine, fichier);
			if (option == 1)
				fputc('\n', fichier);
			fputc('{', fichier);
			if (option ==1 )
				fputc('\n', fichier);
			tmp2 = tmp1->propriete;
			while (tmp2->next != NULL) {
				if (option == 1)
					fputc('\t', fichier);
				fputs(tmp2->chaine, fichier);
				fputc(';', fichier);
				if (option == 1)
					fputc('\n', fichier);
				tmp2 = tmp2->next;	
			}
			if (option == 1)
				fputc('\t', fichier);
			fputs(tmp2->chaine, fichier);
			fputc(';', fichier);
			if (option == 1)
				fputc('\n', fichier);
			fputc('}', fichier);
			if (option == 1)
				fputc('\n', fichier);		
			tmp1 = tmp1->next;
		}
	  	tmp2 = tmp1->balise;
	  	while (tmp2->next != NULL) {
			fputs(tmp2->chaine, fichier );
			fputc(',', fichier);
			tmp2 = tmp2->next;
	  	}
		fputs(tmp2->chaine, fichier);
		if (option == 1)
			fputc('\n', fichier);
		fputc('{', fichier);
		if (option == 1)
			fputc('\n', fichier);
		tmp2 = tmp1->propriete;
		while (tmp2->next != NULL) {
			if (option == 1)
				fputc('\t', fichier);
			fputs(tmp2->chaine, fichier);
			fputc(';', fichier);
			if (option == 1)
				fputc('\n', fichier);
			tmp2 = tmp2->next;	
		}
		if (option == 1)
			fputc('\t', fichier);
		fputs(tmp2->chaine, fichier);
		fputc(';', fichier);
		if (option == 1)
			fputc('\n',fichier);
		fputc('}', fichier);
		if(option==1)
			fputc('\n', fichier);
	}
	else return ;
}

t_list *creation_liste (int max) {
    t_list *liste = malloc(sizeof(t_list));
    liste->elements = malloc(sizeof(int)*max);
    liste->max = max;
    liste->longueur = 0;
	return liste;
}

void ajout_liste (t_list *liste, char valeur) {
    int i;
    if (liste->longueur == liste->max) {
        t_list *tmp = creation_liste(liste->max+1);
        for(i=0; i<liste->longueur; i++)
            tmp->elements[i] = liste->elements[i];
        tmp->longueur = liste->longueur+1;
        tmp->elements[liste->longueur] = valeur;
        tmp->elements[liste->longueur+1] = '\0';
        *liste = *tmp;
    }
    else {
        liste->elements[liste->longueur] = valeur;
        liste->elements[liste->longueur+1] = '\0';
        liste->longueur = liste->longueur+1;
    }
}

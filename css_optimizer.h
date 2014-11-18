// Structure pour ranger chaine recuperee
typedef struct struct_chaine {
	char *chaine;
	struct struct_chaine *next;
} struct_chaine;

// Structrure pour ranger les chaines triees par type
typedef struct struct_css {
	struct_chaine *propriete;
	struct_chaine *balise;
	struct struct_css *next;
} struct_css;

// Liste pour ranger les elements
typedef struct s_list {
	char *elements ;
	int longueur;
	int max;
} t_list;

// Prototypes
struct_css *lecture (FILE *fichier, char *chemin);
int verif_option (int argc, char **argv, int *verif_b);
t_list *creation_liste (int max); 
void ajout_liste (t_list *liste, char valeur);
char *allocation (char *chaine);
struct_chaine *creation_chaine (char *chaine);
struct_chaine *ajout_chaine (struct_chaine *liste, char *chaine);
struct_chaine *concat_chaine (struct_chaine *source1, struct_chaine *source2);
struct_chaine *supprime_chaine (struct_chaine *liste, int position);
int chaine_longueur (char *chaine);
int chaine_comparaison (char *chaine1, char *chaine2);
int chaine_propriete (char *propriete1, char *propriete2);
int chaine_comp (struct_chaine *source1, struct_chaine *source2);
int chaine_longueur_liste (struct_chaine *liste);
int chaine_recherche (struct_chaine *liste, char *chaine);
int chaine_cherche_propriete (struct_chaine *liste, char *chaine);
struct_css *css_creation (struct_chaine *propriete, struct_chaine *balise);
struct_css *css_fusion (struct_css *contenu, struct_css *ajout);
struct_css *css_ajout(struct_css *liste, struct_chaine *propriete, struct_chaine *balise);
struct_css *css_optimisation (struct_css *css);
struct_css *css_optimisation_bloc (struct_css *css, struct_css *optimisation);
struct_css *css_optimisation_propriete (struct_css *css, struct_css *optimisation, char *propriete);
void css_ecriture_fichier (struct_css *css, char *nom_fichier, int option);

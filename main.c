#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "afficheHeader.h"
#include "contenuSection.h"
#include "afficheSectionTable.h"

int main(int argc, char* argv[]) {

	if (argc<3 || !strcmp(argv[1],"-h") || (argc!=4 && !strcmp(argv[1],"-s")))
		printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	else if (!strcmp(argv[1],"-e")) afficheHeader(argv[2]); // affichage en-tete
	else if (!strcmp(argv[1],"-sh")) afficheSectionTable(argv[2]); // affichage table en-tete section
	else if (!strcmp(argv[1],"-s")) affichageSection(argv[2], atoi(argv[3])); // affichage contenu d'une section
	else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

	return 0;
}

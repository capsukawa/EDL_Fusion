#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "afficheHeader.h"
#include "contenuSection.h"
#include "afficheSectionTable.h"

int main(int argc, char* argv[]) {

	if (argc<3 || !strcmp(argv[1],"-h"))
		printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

	else if (!strcmp(argv[1],"-e")) // affichage en-tete
		afficheHeader(argv[2]);

	else if (!strcmp(argv[1],"-sh")) // affichage table en-tete section
		afficheSectionTable(argv[2]);

	else if (!strcmp(argv[1],"-s")) { // affichage contenu d'une section
		if (argc==4) affichageSection(argv[2], atoi(argv[3]));
		else printf("Usage : \"./edl_fusion -s [nom_fichier] [numero_section]\"\n");
	}

	else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

	return 0;
}

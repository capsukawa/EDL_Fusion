#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "afficheHeader.h"
#include "header.h"
#include "util.h"


void affichageSection(Elf32_Ehdr* header, FILE* f, char *nom, char* sectionNames[header->e_shnum]) {
	long avancement;
	Elf32_Off adresse = SHT_NOBITS;
	Elf32_Word taille = SHT_NOBITS;

	int nbSection = header->e_shnum;	// Nombre de section du header
	int num, i;

	if(nom[0]=='.'){
		i=0;
		while ((i!=header->e_shnum) && ((strcmp(sectionNames[i],nom)) != 0)) {
			i=i+1;
		}
			num=i;
	}else{
		num=atoi(nom);
	}

	if (num<nbSection) {

		avancement = (header->e_shoff) + 40*(num) + 16 ;
		// On va jusqu'a l'offset de la section demandee
		fseek(f,avancement,SEEK_SET);

		fread(&adresse,4,1,f); 	// sauvegarde de l'adresse de la section
		fread(&taille,4,1,f);	// sauvegarde de la taille de la section

		if (header->e_ident[5]==2) {	// traitement du big endian
			taille = reverse_4(taille);
			adresse = reverse_4(adresse);
		}

		unsigned char var[taille];

		fseek(f,adresse,SEEK_SET);	// On va au debut du contenu de la section

		fread(&var, 1, taille, f);

		printf("  0x00000000 ");

		for (int i=0; i < taille; i=i+4) { // affichage
			printf("%02x",var[i]);
			printf("%02x",var[i+1]);
			printf("%02x",var[i+2]);
			printf("%02x ",var[i+3]);
		}
		printf("\n");
	}
}

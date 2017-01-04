#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "afficheHeader.h"


void affichageSection(char* fadr, int num) {

	Elf32_Ehdr* header;
	long avancement;
	FILE *f;
	Elf32_Off adresse = SHT_NOBITS;
	Elf32_Word taille = SHT_NOBITS;
	header = initHeader(fadr);

	int nbSection = header->e_shnum;	// Nombre de section du header
	//int lsb = header->e_ident[5];	// little or big endian

	if (num<=nbSection) {
		f = fopen(fadr,"r");
		avancement = (header->e_shoff) + 40*(num-1) + 32 ;	
		// On va jusqu'a l'offset de la section demandee 
		fseek(f,avancement,SEEK_SET);

		fread(&adresse,4,1,f); 	// sauvegarde de l'adresse de la section
		fread(&taille,4,1,f);	// sauvegarde de la taille de la section

		char var[taille];
			printf("\n%lx",avancement);
			printf("\n%x",taille);
			printf("\n%x\n",adresse);

		fseek(f,adresse,SEEK_SET);	// On va au debut du contenu de la section

		fread(&var, 1, taille, f);
		for (int i=0; i < taille; i++) {
			printf("%x",var[i]);

		}
		
		
		//fwrite(f,1,taille,stdout); // affichage moche

	}
		printf("fin");
}

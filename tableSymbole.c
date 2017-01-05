#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "util.h"


void afficherTableSymbole(Elf32_Ehdr* header, FILE* f) {
	Elf32_Word oct = SHT_NOBITS;
	Elf32_Off adresse = SHT_NOBITS;
	Elf32_Word taille = SHT_NOBITS;
	int nbSection, i, j;
	long avancement;

	Elf32_Word name ;		//st_name
	Elf32_Addr value ;		//st_value
	Elf32_Word size ;		//st_size
	unsigned char info ;	//st_info
	unsigned char other ;	//st_other
	Elf32_Half shndx ;		//st_shndx

	nbSection = header->e_shnum;	// Nombre de section du header

	avancement = (header->e_shoff) + 4 ;
	fseek(f,avancement,SEEK_SET);
	fread(&oct,4,1,f); 	// sauvegarde de sh_type (2e paquet d'octet = 2 pour sht_symtab)
	i = 0;
	while (i!=nbSection && oct!=2){
		i=i+1;
		avancement = avancement + 40;
		fseek(f,avancement,SEEK_SET);
		fread(&oct,4,1,f); 	// mise a jour de sh_type
	}

	if (i<=nbSection) {
		avancement = avancement + 12;
		fseek(f,avancement,SEEK_SET);	// avance jusqu'a l'adresse
		fread(&adresse,4,1,f); 			// sauvegarde de l'adresse de symtab
		fread(&taille,4,1,f);			// sauvegarde de la taille de symtab

		if (header->e_ident[5]==2) {	// traitement du big endian
			taille = reverse_4(taille);
			adresse = reverse_4(adresse);
		}

		taille = taille / 16;

		fseek(f,adresse,SEEK_SET);	// on va au debut de symtab
		j = 0;

		printf("Name	Value	Size	Type		Vis		Ndx\n");

		while(j<taille){
			fread(&name,4,1,f); 	// mise a jour de name
			fread(&value,4,1,f); 	// mise a jour de value
			fread(&size,4,1,f); 	// mise a jour de size
			fread(&info,1,1,f); 	// mise a jour de info
			fread(&other,1,1,f); 	// mise a jour de other
			fread(&shndx,2,1,f); 	// mise a jour de shndx

			// name
			if(name==0){ printf("	"); }
			//else if {
			//		printf("%x	", name);
			//}
			else {
					printf("%x	", name);
			}


			printf("%i	", value);	// value
			printf("%i	", size);	// size

			// type
			if (info == 0) { printf("NOTYPE"); }
			else if (info == 1) { printf("OBJECT"); }
			else if (info == 2) { printf("FUNC"); }
			else if (info == 3) { printf("SECTION"); }
			else if (info == 4) { printf("FILE"); }
			else { printf("PROC"); }
			printf("		");

			// vis
			printf("DEFAULT		");

			//index
			if (shndx == 0) { printf("UND"); }
			else if (shndx == 0xfff1) { printf("ABS"); }
			else { printf("%x", shndx); }

			j=j+1;

			printf("\n");
		}
	}
}

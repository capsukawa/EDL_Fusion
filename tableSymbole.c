#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
//#include "afficheHeader.h"
#include "header.h"
//#include "util.h"




void afficherTableSymbole(char* fadr) {
	FILE *f;
	Elf32_Ehdr* header;
	Elf32_Word oct = SHT_NOBITS;
	Elf32_Off adresse = SHT_NOBITS;
	Elf32_Word taille = SHT_NOBITS;
	int nbSection, i, j;
	long avancement;

	Elf32_Word name ; //st_name 
	Elf32_Addr value ; //st_value
	Elf32_Word size ; //st_size
	unsigned char info ; //st_info
	unsigned char other ; //st_other
	Elf32_Half shndx ; //st_shndx 

	header = initHeader(fadr);

	nbSection = header->e_shnum;	// Nombre de section du header

	f = fopen(fadr,"r");
	
	i = 0;

	avancement = (header->e_shoff) + 8 ;

	fseek(f,avancement,SEEK_SET);
	fread(&oct,4,1,f); 	// sauvegarde de sh_type
	while (i!=nbSection && oct!=2){
		i=i+1;
		fseek(f,40,SEEK_CUR);
		fread(&oct,4,1,f); 	// mise a jour de sh_type
	}
	
	if (i<=nbSection) {
		fseek(f,12,SEEK_CUR);	// avance jusqu'a l'adresse
		fread(&adresse,4,1,f); 	// sauvegarde de l'adresse de symtab
		fread(&taille,4,1,f);	// sauvegarde de la taille de symtab

		fseek(f,adresse,SEEK_SET); // on va au debut de symtab
		j = 0;
		while(j!=taille){
			fread(&name,4,1,f); 	// mise a jour de name
			fread(&value,4,1,f); 	// mise a jour de value
			fread(&size,4,1,f); 	// mise a jour de size
			fread(&info,1,1,f); 	// mise a jour de info
			fread(&other,1,1,f); 	// mise a jour de other
			fread(&shndx,2,1,f); 	// mise a jour de shndx

			printf("%x\n", name);
			j=j+1;
			//fwrite(f,,,stdout);
		}

	}
}

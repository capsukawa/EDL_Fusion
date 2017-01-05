#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"
#include "util.h"



int rechercheSectionHeader(char nom[8],Elf32_Ehdr* header,char* sectionNames[header->e_shnum]) {
	int i;

	i = 0;
	while ((i!=header->e_shnum) && ((strcmp(sectionNames[i],nom)) != 0)) {
		i=i+1;
	}

	return i;
}



void afficherTableSymbole(Elf32_Ehdr* header, FILE* f,Elf32_Shdr* sectionTable[header->e_shnum], char* sectionNames[header->e_shnum], char **tabNomSym) {
	Elf32_Off adresse = SHT_NOBITS, adresseStr = SHT_NOBITS;
	Elf32_Word taille = SHT_NOBITS;
	char c; 
	int nbSection, i, j, k;
	char nom[10] ;	//nom des variables dans strtab

	Elf32_Word name ;		//st_name
	Elf32_Addr value ;		//st_value
	Elf32_Word size ;		//st_size
	unsigned char info ;	//st_info
	unsigned char other ;	//st_other
	Elf32_Half shndx ;		//st_shndx

	nbSection = header->e_shnum;	// Nombre de section du header

	i = rechercheSectionHeader(".strtab",header,sectionNames);
	if (i!=header->e_shnum) {		// recupere adresse debut strtab
		adresseStr = (sectionTable[i]->sh_offset);
	}
	if (header->e_ident[5]==2) {	// traitement du big endian
		adresseStr = reverse_4(adresseStr);
	}

	i = rechercheSectionHeader(".symtab",header,sectionNames);
	if (i!=header->e_shnum) {	
		adresse = (sectionTable[i]->sh_offset); // recupere adresse debut symtab
		taille = (sectionTable[i]->sh_size);	// recupere taille symtab
	}
	if (header->e_ident[5]==2) {	// traitement du big endian
		taille = reverse_4(taille);
		adresse = reverse_4(adresse);
	}
	taille = taille / 16;

	//printf("%x \n",taille);
	//printf("%x \n",adresse);

	if (i<=nbSection) {
		fseek(f,adresse,SEEK_SET);	// on va au debut de symtab

		printf("%-16s%-8s%-8s%-16s%-16s%-8s", "Name","Value","Size","Type","Vis","Ndx");
		printf("\n");

		j = 0;
		while(j<taille){
			fseek(f,adresse+16*j,SEEK_SET);	// on va au debut de symtab
			fread(&name,4,1,f); 	// mise a jour de name
			fread(&value,4,1,f); 	// mise a jour de value
			fread(&size,4,1,f); 	// mise a jour de size
			fread(&info,1,1,f); 	// mise a jour de info
			fread(&other,1,1,f); 	// mise a jour de other
			fread(&shndx,2,1,f); 	// mise a jour de shndx

			if (header->e_ident[5]==2) {	// traitement du big endian 
				name = reverse_4(name);
				value = reverse_4(value);
				size = reverse_4(size);
				shndx = reverse_2(shndx);
			}


			// --AFFICHAGE--

			// name
			if (name==0) {
				printf("%-16s","");
				tabNomSym[j] = malloc(sizeof(char));
				tabNomSym[j][0] = '\0';
			}
			else {
				fseek(f,adresseStr+name,SEEK_SET);	// avance jusqu'a l'adresse strtab + name 
				i=0;
				fread(&c,1,1,f);
				while(i!=10){
					nom[i]='\0';
					i=i+1;
				}
				i=0;
				while(c != 0x00) {
					nom[i]=c;
					fread(&c,1,1,f);
					i=i+1;
				}
				printf("%-16s", nom);
				// Remplissage de tabNomSym
				tabNomSym[j] = malloc(sizeof(char)*i);
				for (k=0; nom[k]!='\0'; k++) {
					tabNomSym[j][k] = nom[k];
				}
				tabNomSym[j][k] = '\0';
			}

			// value
			printf("%-8i", value);

			// size
			printf("%-8i", size);

			// type
			if (info == 0) { printf("%-16s","NOTYPE"); }
			else if (info == 1) { printf("%-16s","OBJECT"); }
			else if (info == 2) { printf("%-16s","FUNC"); }
			else if (info == 3) { printf("%-16s","SECTION"); }
			else if (info == 4) { printf("%-16s","FILE"); }
			else { printf("%-16s","PROC"); }

			// vis
			printf("%-16s","DEFAULT");

			//index
			if (shndx == 0) { printf("%-8s","UND"); }
			else if (shndx == 0xfff1) { printf("%-8s","ABS"); }
			else { printf("%-8x", shndx); }

			j=j+1;
			printf("\n");
		}
	}
}

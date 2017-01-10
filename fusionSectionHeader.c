#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elfStruct.h"
#include "header.h"
#include "util.h"
#include "sections.h"


void preTraitementF1(ElfFileStruct* elf1,ElfFileStruct* elf2, int *decalageSection){
	// PRE-TRAITEMENT DE F1
	int nombre = 0;
	int i=0;
	int j=0;
	int nbSection1 = elf1->header->e_shnum;	// Nombre de section de f1
	int nbSection2 = elf2->header->e_shnum;	// Nombre de section de f2
	char *nomSection1 ;
	char *nomSection2 ;
	decalageSection[i]=0;
	while(i!=nbSection1) {	// nombre de section avant le debut des sections header
		if(elf1->sections[i]->header->sh_offset < elf1->header->e_shoff){
			nombre = nombre+1;
		}
		nomSection1 = elf1->sections[j]->name; // On recupere le nom de la section	
		while(j!=nbSection2){
			nomSection2 = elf2->sections[i]->name; // On recupere le nom de la section	
			if(strcmp(nomSection1,nomSection2)==0){
				decalageSection = realloc(decalageSection,sizeof(int)*(i+1)); 
				decalageSection[i+1] = decalageSection[i] + elf2->sections[j]->header->sh_size; 
			}
			j=j+1;
		}
		i=i+1;
	}
}


void calculShOff(ElfFileStruct* elf1,ElfFileStruct* elf2 , int* addTailleSection, int* decalageTotal, int* sectionEnPlus) {
	int i = 0;
	int j = 0;
	char *nomSection1 ;
	char *nomSection2 ;
	int nbSection1 = elf1->header->e_shnum;	// Nombre de section de f1
	int nbSection2 = elf2->header->e_shnum;	// Nombre de section de f2
	*sectionEnPlus = 0;
	*decalageTotal = 0; 		// decalage total de la ou on est dans fichier 
	int *indice = malloc(sizeof(int));  // tableau indices section f2 pas ds f1 pr les rajouter
	*addTailleSection = 0;				// taille additionnee des sections f2 pas ds f1
	while(i!=nbSection2){	// on cherche dans f2 leur presence dans f1  
		nomSection2 = elf2->sections[i]->name; // On recupere le nom de la section	
		j=0;
		nomSection1 = elf1->sections[j]->name; // On recupere le nom de la section
		while(j!=nbSection1-1 && strcmp(nomSection1,nomSection2) != 0){
			j=j+1;
			nomSection1 = elf1->sections[j]->name; // On recupere le nom de la section	
		}
		if((strcmp(nomSection1,nomSection2) == 0)&&(elf1->header->e_shoff <= elf1->sections[j]->header->sh_offset)){	// cas des sections avant deb section header
				*decalageTotal = *decalageTotal + elf2->sections[i]->header->sh_size;
		}
		if ((j==nbSection1-1)&&(strcmp(nomSection1,nomSection2) != 0)){	// cas sections f2 pas f1
			indice = realloc(indice,sizeof(int)*(*sectionEnPlus+1));
			indice[*sectionEnPlus] = i;						// tableau des indices
			*sectionEnPlus = *sectionEnPlus + 1;
			*addTailleSection = *addTailleSection + elf2->sections[i]->header->sh_size;
		}
		// decalage de chaque section de f1 une fois les sections de f2 concatenees	
	
		i=i+1;
	}
}


void fusionSectionHeader(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf) {
	
	// Modification du header 

	int i, ind, k;
	int addTailleSection;
	int decalageTotal; 
	int sectionEnPlus;
	int *decalageSection = malloc(sizeof(int));
	char* nom;

	calculShOff(elf1, elf2, &addTailleSection, &decalageTotal, &sectionEnPlus);
	preTraitementF1(elf1, elf2, decalageSection);

	elf->header = malloc(sizeof(Elf32_Ehdr));
	for(i=0; i<EI_NIDENT; i++){
		elf->header->e_ident[i] = elf1->header->e_ident[i];
	}



	elf->header->e_type = elf1->header->e_type;
	elf->header->e_machine = elf1->header->e_machine;
	elf->header->e_version = elf1->header->e_version;
	elf->header->e_entry = elf1->header->e_entry;
	elf->header->e_phoff = 0;
	elf->header->e_shoff = decalageTotal + elf1->header->e_shoff;
	elf->header->e_flags = elf1->header->e_flags;
	elf->header->e_ehsize = elf1->header->e_ehsize;
	elf->header->e_phentsize = 0;
	elf->header->e_phnum = 0;
	elf->header->e_shentsize = elf1->header->e_shentsize;
	elf->header->e_shnum = sectionEnPlus + elf1->header->e_shnum;
	elf->header->e_shstrndx = elf1->header->e_shstrndx;


	// Modification des headers de sections
	elf->sections = malloc(sizeof(Elf_Section)*elf->header->e_shnum);
	ind = 0;
	i = 0;
	while (i < elf1->header->e_shnum) {	//recopie et eventuel changement des sections header de f1
		nom = elf1->sections[i]->name ;
		ind = findSectionHeader(nom,elf2);
		if(ind!=-1) {	// on change le section header si section ds f2
			elf->sections[i] = malloc(sizeof(Elf_Section));
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			elf->sections[i]->header->sh_name = elf1->sections[i]->header->sh_name;
			elf->sections[i]->header->sh_type = elf1->sections[i]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf1->sections[i]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf1->sections[i]->header->sh_addr;
			elf->sections[i]->header->sh_offset = decalageSection[i] + elf1->sections[i]->header->sh_offset;
			elf->sections[i]->header->sh_size = elf1->sections[i]->header->sh_size + elf2->sections[ind]->header->sh_size;
			elf->sections[i]->header->sh_link = elf1->sections[i]->header->sh_link;
			elf->sections[i]->header->sh_info = elf1->sections[i]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf1->sections[i]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf1->sections[i]->header->sh_entsize;
		}
		else {
			elf->sections[i] = malloc(sizeof(Elf_Section));
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			elf->sections[i]->header->sh_name = elf1->sections[i]->header->sh_name;
			elf->sections[i]->header->sh_type = elf1->sections[i]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf1->sections[i]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf1->sections[i]->header->sh_addr;
			elf->sections[i]->header->sh_offset = elf1->sections[i]->header->sh_offset;
			elf->sections[i]->header->sh_size = elf1->sections[i]->header->sh_size;
			elf->sections[i]->header->sh_link = elf1->sections[i]->header->sh_link;
			elf->sections[i]->header->sh_info = elf1->sections[i]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf1->sections[i]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf1->sections[i]->header->sh_entsize;
		}
		i++;
	}

	k=0;	// sections header des sections ds f2 absentes de f1
	while(k!=sectionEnPlus){	// parcours du tableau des indices : sections de f2 pas dans f1
			elf->sections[i] = malloc(sizeof(Elf_Section));
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			elf->sections[i]->header->sh_name = elf2->sections[k]->header->sh_name;
			elf->sections[i]->header->sh_type = elf2->sections[k]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf2->sections[k]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf2->sections[k]->header->sh_addr;
			elf->sections[i]->header->sh_offset = elf2->sections[k]->header->sh_offset;
			elf->sections[i]->header->sh_size = elf2->sections[k]->header->sh_size;
			elf->sections[i]->header->sh_link = elf2->sections[k]->header->sh_link;
			elf->sections[i]->header->sh_info = elf2->sections[k]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf2->sections[k]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf2->sections[k]->header->sh_entsize;

		k=k+1;
	}

printf("%x\n",elf->header->e_shoff);
printf("%x\n",elf->header->e_shnum);
printf("%x\n",elf->sections[1]->header->sh_offset);

	// Modification des sections
	
}

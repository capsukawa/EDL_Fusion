#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elfStruct.h"
#include "header.h"
#include "util.h"
#include "sections.h"
#include "fusionHeader.h"


void fusionSectionHeader(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf) {

	// Modification des headers de sections
	int ind = 0, i = 0, k = 0;
	int indShstrtab;
	int addTailleSection;
	int decalageTotal; 
	int sectionEnPlus;
	int sectionsPrecedentes = 0;
	int *decalageSection = malloc(sizeof(int)*elf1->header->e_shnum+1);
	if (decalageSection==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	int *indice = malloc(sizeof(int)*elf2->header->e_shoff);	// tableau indices section f2 pas ds f1 pr les rajouter
	if (indice==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	char* nom;

	calculShOff(elf1, elf2, &addTailleSection, &decalageTotal, &sectionEnPlus, indice);

	preTraitementF1(elf1, elf2, decalageSection);

	elf->sections = malloc(sizeof(Elf_Section)*elf->header->e_shnum);
	if (elf->sections==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	while (i < elf1->header->e_shnum) {	//recopie et eventuel changement des sections header de f1
		nom = elf1->sections[i]->name ;

		if(strcmp(nom,".shstrtab")==0){
			indShstrtab = i;
		}

		ind = findSectionHeader(nom,elf2);
		if(ind!=-1) {	// on change le section header si section ds f2

			elf->sections[i] = malloc(sizeof(Elf_Section));
			if (elf->sections[i]==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			if (elf->sections[i]->header==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			elf->sections[i]->header->sh_name = elf1->sections[i]->header->sh_name;
			elf->sections[i]->header->sh_type = elf1->sections[i]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf1->sections[i]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf1->sections[i]->header->sh_addr;
			if(elf1->sections[i]->header->sh_offset > elf->header->e_shoff){
				elf->sections[i]->header->sh_offset = decalageSection[i] + elf1->sections[i]->header->sh_offset + sectionEnPlus*elf->header->e_shentsize;	
			}else {
				elf->sections[i]->header->sh_offset = decalageSection[i] + elf1->sections[i]->header->sh_offset;

			}
			elf->sections[i]->header->sh_size = elf1->sections[i]->header->sh_size + elf2->sections[ind]->header->sh_size;
			elf->sections[i]->header->sh_link = elf1->sections[i]->header->sh_link;
			elf->sections[i]->header->sh_info = elf1->sections[i]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf1->sections[i]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf1->sections[i]->header->sh_entsize;
		}
		else {	//recopie section header f1 si section pas dans f2
			elf->sections[i] = malloc(sizeof(Elf_Section));
			if (elf->sections[i]==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			if (elf->sections[i]->header==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			elf->sections[i]->header->sh_name = elf1->sections[i]->header->sh_name;
			elf->sections[i]->header->sh_type = elf1->sections[i]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf1->sections[i]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf1->sections[i]->header->sh_addr;

			if(elf1->sections[i]->header->sh_offset > elf->header->e_shoff){
				elf->sections[i]->header->sh_offset = decalageSection[i] + elf1->sections[i]->header->sh_offset + sectionEnPlus*elf->header->e_shentsize;	
			}
			else {
				elf->sections[i]->header->sh_offset = decalageSection[i] + elf1->sections[i]->header->sh_offset;	
			}
 			elf->sections[i]->header->sh_size = elf1->sections[i]->header->sh_size;
			elf->sections[i]->header->sh_link = elf1->sections[i]->header->sh_link;
			elf->sections[i]->header->sh_info = elf1->sections[i]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf1->sections[i]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf1->sections[i]->header->sh_entsize;
		}
		i++;
	}

	k=0;	// sections header des sections ds f2 absentes de f1
	while ( (i < elf->header->e_shnum)&&(k!=sectionEnPlus) ) {	// parcours du tableau des indices : sections de f2 pas dans f1
			elf->sections[i] = malloc(sizeof(Elf_Section));
			if (elf->sections[i]==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
			if (elf->sections[i]->header==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			//name = name de f2 + taille de shstrtab de f1 
			elf->sections[i]->header->sh_name = elf2->sections[indice[k]]->header->sh_name + elf1->sections[indShstrtab]->header->sh_size;
			elf->sections[i]->header->sh_type = elf2->sections[indice[k]]->header->sh_type;
			elf->sections[i]->header->sh_flags = elf2->sections[indice[k]]->header->sh_flags;
			elf->sections[i]->header->sh_addr = elf2->sections[indice[k]]->header->sh_addr; 

			// addr des sections ajoutees = debut des section header + tous les sections header + sections ajoutees precedemment
			elf->sections[i]->header->sh_offset = elf->header->e_shoff + (elf->header->e_shnum*elf->header->e_shentsize) + sectionsPrecedentes;
			sectionsPrecedentes = sectionsPrecedentes + elf2->sections[indice[k]]->header->sh_size;
			elf->sections[i]->header->sh_size = elf2->sections[indice[k]]->header->sh_size;
			elf->sections[i]->header->sh_link = elf2->sections[indice[k]]->header->sh_link;
			elf->sections[i]->header->sh_info = elf2->sections[indice[k]]->header->sh_info;
			elf->sections[i]->header->sh_addralign = elf2->sections[indice[k]]->header->sh_addralign;
			elf->sections[i]->header->sh_entsize = elf2->sections[indice[k]]->header->sh_entsize;

		k=k+1;
		i=i+1;
	}
}


void fusionSection(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elf){
	int i, j, ind;
	int addTailleSection, decalageTotal, sectionEnPlus;
	int *indice = malloc(sizeof(int)*elf2->header->e_shoff);	// tableau indices section f2 pas ds f1 pr les rajouter
	if (indice==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	char *nom;
	int d, sizeName;
	int e,g;
	char c;
	
	fusionSectionHeader(elf1, elf2, elf);

	calculShOff(elf1, elf2 , &addTailleSection, &decalageTotal, &sectionEnPlus, indice);	



	i = 0;
	while (i < elf1->header->e_shnum) {	//recopie et eventuel changement des sections de f1
		nom = elf1->sections[i]->name;
		ind = findSectionHeader(nom,elf2);



		if(ind!=-1) {	// on change le contenu des section si section ds f2
			elf->sections[i]->content = malloc(sizeof(char)*elf->sections[i]->header->sh_size);
			if (elf->sections[i]->content==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			d=0;
			e=0;	
			g=0;
			while(d< elf->sections[i]->header->sh_size){	
				if (d< elf1->sections[i]->header->sh_size){ // recopie
					elf->sections[i]->content[d] = elf1->sections[i]->content[g];
					g=g+1;
				}
				else {// concat
					elf->sections[i]->content[d] = elf2->sections[ind]->content[e];
					e=e+1;
				}
			d = d+1;
			}
		}
		else {	// recopie du contenu de f1 si rien a concatener
			elf->sections[i]->content = malloc(sizeof(char)*elf->sections[i]->header->sh_size);
			if (elf->sections[i]->content==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			d=0;
			while(d< elf->sections[i]->header->sh_size){	// concat
				elf->sections[i]->content[d] = elf1->sections[i]->content[d];
				d=d+1;
			}
		}
		sizeName = 0;
		c = elf1->sections[i]->name[sizeName];
		while (c != '\0') {
			sizeName = sizeName+1;
			c = elf1->sections[i]->name[sizeName];
		}
		elf->sections[i]->name = malloc(sizeName);
		if (elf->sections[i]->name==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		elf->sections[i]->name= elf1->sections[i]->name; //recopie du nom des sections de f1

		i = i+1;
	}

	// cas des sections de f2 absentes de f1	
	j=0;
	while ( (i < elf->header->e_shnum)&&(j!=sectionEnPlus) ) {	//recopie et eventuel changement des sections header de f1
		elf->sections[i]->name = malloc(sizeof(elf2->sections[indice[j]]->name));
		if (elf->sections[i]->name==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		elf->sections[i]->name = elf2->sections[indice[j]]->name;
		elf->sections[i]->content = malloc(sizeof(elf2->sections[indice[j]]->content));
		if (elf->sections[i]->content==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		elf->sections[i]->content = elf2->sections[indice[j]]->content;
		j = j+1;
		i = i+1;
	}



}

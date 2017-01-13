#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "elfStruct.h"
#include "sections.h"

void addSymbol(ElfFileStruct* elf, Elf_Symbol* sym) {
	elf->symbols = realloc(elf->symbols,sizeof(Elf_Symbol)*elf->nbSym+1);
		if (elf->symbols==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	elf->symbols[elf->nbSym] = malloc(sizeof(Elf_Symbol));
		if (elf->symbols[elf->nbSym]==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	elf->symbols[elf->nbSym]->sym = malloc(sizeof(Elf32_Sym));
		if (elf->symbols[elf->nbSym]->sym==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	elf->symbols[elf->nbSym]->name = malloc(sizeof(sym->name));
		if (elf->symbols[elf->nbSym]->name==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	elf->symbols[elf->nbSym]->name = sym->name;
	memcpy(elf->symbols[elf->nbSym]->sym,sym->sym,sizeof(Elf32_Sym));
	elf->nbSym++;
}

void fusionSymbole(ElfFileStruct* elf1, ElfFileStruct* elf2, ElfFileStruct* elff) {
	int i, j, w, k = 0;
	int symTrouve = 0;
	int indiceTraite[elf2->nbSym];
	elff->symbols = malloc(sizeof(Elf_Symbol)*elff->nbSym+1);
	if (elff->symbols==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
	// Symboles locaux
	for (i=0;i<elf1->nbSym;i++) {
		if (ELF32_ST_BIND(elf1->symbols[i]->sym->st_info) == STB_LOCAL) {
			addSymbol(elff,elf1->symbols[i]);
		}
	}
	for (j=0;j<elf2->nbSym;j++) {
		if (ELF32_ST_BIND(elf2->symbols[j]->sym->st_info) == STB_LOCAL) {
			addSymbol(elff,elf2->symbols[j]);
		}
	}
	// Symboles globaux
	for (i=0;i<elf1->nbSym;i++) {
		symTrouve = 0;
		if (ELF32_ST_BIND(elf1->symbols[i]->sym->st_info) == STB_GLOBAL) {
			for(j=0;j<elf2->nbSym;j++) {
				if(!symTrouve && strcmp(elf1->symbols[i]->name, elf2->symbols[j]->name) == 0) {
					symTrouve = 1;
					indiceTraite[k]=j;
					k++;
					if (elf1->symbols[i]->sym->st_shndx != SHN_UNDEF && elf2->symbols[j]->sym->st_shndx != SHN_UNDEF) {
						printf("Erreur : Deux symboles globaux de même nom !\n");
						abort();
					} else if (elf1->symbols[i]->sym->st_shndx != SHN_UNDEF && elf2->symbols[j]->sym->st_shndx == SHN_UNDEF)
						addSymbol(elff,elf1->symbols[i]);
					else if (elf1->symbols[i]->sym->st_shndx == SHN_UNDEF && elf2->symbols[j]->sym->st_shndx != SHN_UNDEF) {
							addSymbol(elff,elf2->symbols[j]);
					}
					else addSymbol(elff,elf1->symbols[i]);
				} else if (symTrouve) break;
			}
			if (!symTrouve) addSymbol(elff,elf1->symbols[i]);
		}
	}
	for (j=0;j<elf2->nbSym;j++) {
		symTrouve = 0;
		for (w=0;w<k;w++)
			if (indiceTraite[w]==j)
				symTrouve = 1;
		if (ELF32_ST_BIND(elf2->symbols[j]->sym->st_info) == STB_GLOBAL && !symTrouve) {
			addSymbol(elff,elf2->symbols[j]);
		}
	}

	//modifie le header section de symtab pour y affecter la nouvelle taille.
	k = findSectionHeader(".symtab", elff);
	elff->sections[k]->header->sh_size = sizeof(Elf32_Sym)*elff->nbSym;

	//compelete strtab
	j = findSectionHeader(".strtab", elff);
	i = 1;
	int iName;
	int size = 1;
	elff->sections[j]->content = realloc(elff->sections[j]->content,size);
	elff->sections[j]->content = 0x00;
	elff->sections[j]->header->sh_size = size;

	char c;
	for(k=0; k<elff->nbSym; k++) {
		if(strcmp(elff->symbols[k]->name,"")) {
			iName = 0;
			c = elff->symbols[k]->name[iName];
			while(c != 0x00) {
				iName ++;
				c = elff->symbols[k]->name[iName];
			}
			iName++; // pour l'ajout de 0x00
			elff->sections[j]->content = realloc(elff->sections[j]->content, (size + iName + 1));
			for(w=0; w<iName; w++) {
				elff->sections[j]->content[size] = elff->symbols[k]->name[w];
				size++;
			}
			elff->sections[j]->content[size] = 0x00;
			size++;

			elff->symbols[k]->sym->st_name = i;
			i = i + iName;
		}
	}
}

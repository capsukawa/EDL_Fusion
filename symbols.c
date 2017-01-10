#include <elf.h>
#include <stdlib.h>
#include "elfStruct.h"
#include "sections.h"
#include "util.h"

void reverseBytesSymbole(Elf32_Sym* symbole) {
	symbole->st_name = reverse_4(symbole->st_name);
	symbole->st_value = reverse_4(symbole->st_value);
	symbole->st_size = reverse_4(symbole->st_size);
	symbole->st_shndx = reverse_2(symbole->st_shndx);
}

void initSymbols(ElfFileStruct* elf, FILE* f) {
	Elf32_Off adresseSym = SHT_NOBITS, adresseStr = SHT_NOBITS;
	Elf32_Word sizeOfSymTab = SHT_NOBITS;
	char c;

	int i = findSectionHeader(".strtab", elf);
	adresseStr = elf->sections[i]->header->sh_offset;
	i = findSectionHeader(".symtab", elf);
	adresseSym = elf->sections[i]->header->sh_offset; 	// recupere adresse debut symtab
	sizeOfSymTab = elf->sections[i]->header->sh_size;	// recupere taille symtab

	if (elf->header->e_ident[5]==2) {	// traitement du big endian
		adresseStr = reverse_4(adresseStr);
		sizeOfSymTab = reverse_4(sizeOfSymTab);
		adresseSym = reverse_4(adresseSym);
	}
	elf->nbSym = sizeOfSymTab / 16;

	int sizeOfSym = sizeof(Elf32_Sym);
	elf->symbols = malloc(sizeof(Elf_Symbol)*elf->nbSym);
	if (elf->symbols==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}

	fseek(f,adresseSym,SEEK_SET);

	for(int j = 0; j < elf->nbSym; j++) {
		//sym of Elf_Symbol struct
		elf->symbols[j] = malloc(sizeof(Elf_Symbol));
		if (elf->symbols[j]==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		elf->symbols[j]->sym = malloc(sizeOfSym);
		if (elf->symbols[j]->sym==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
		fseek(f,adresseSym+(sizeOfSym*j),SEEK_SET);	// on va au debut de symtab + symbole passé
		fread(elf->symbols[j]->sym,1,sizeOfSym,f); 	// mise a jour de symbole

		if (elf->header->e_ident[5]==2) {	// traitement du big endian
			reverseBytesSymbole(elf->symbols[j]->sym);
		}

		//TODO : Faire attention <<<<<<<<<<<<<<<<<<<<
		//name of Elf_Symbol struct
		//~ if (elf->symbols[j]->sym->st_name==0) {
			//~ elf->symbols[j]->name = malloc(sizeof(char));
			//~ elf->symbols[j]->name[0] = '\0';
		//~ }
		//~ else {
			Elf32_Word currentAddr = adresseStr+elf->symbols[j]->sym->st_name; // adresse du symbole courant dans strtab
			fseek(f,currentAddr,SEEK_SET);	// avance jusqu'a l'adresse strtab + name
			i=0;
			fread(&c,1,1,f);

			while(c != 0x00) {
				fread(&c,1,1,f);
				i=i+1;
			}

			elf->symbols[j]->name = malloc(sizeof(char)*i);
			if (elf->symbols[j]->name==NULL) {printf("Echec d'allocation mémoire.\n"); abort();}
			fseek(f,-i-1,SEEK_CUR);
			fread(elf->symbols[j]->name, 1, i, f);
		//~ }
	}

}

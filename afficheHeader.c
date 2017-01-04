#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

void afficheHeader(char* nf) {
	Elf32_Ehdr* header;
	header = initHeader(nf);

	//- Identification -----------------------------------------------------------
	printf("Identification : ");
	for (int i=0;i<EI_NIDENT;i++) printf("%x ",header->e_ident[i]);

	printf("\n - Architecture -> ");
	if (header->e_ident[4]==0) printf("None");
	else if (header->e_ident[4]==1) printf("32bits");
	else if (header->e_ident[4]==2) printf("64bits");

	printf("\n - Endianness -> ");
	if (header->e_ident[5]==0) printf("None");
	else if (header->e_ident[5]==1) printf("LSB");
	else if (header->e_ident[5]==2) printf("MSB");

	printf("\n - Header Version -> ");
	if (header->e_ident[6]==0) printf("0 (old)");
	else if (header->e_ident[6]==1) printf("1 (current)");

	printf("\n - OS/ABI -> ");
	switch(header->e_ident[7]) {
		case 0:
			printf("Unix System V"); break;
		case 3:
			printf("Linux"); break;
		case 64:
			printf("ARM EABI"); break;
		case 97:
			printf("ARM"); break;
		default:
			printf("Unknown"); break;
	}
	//- Type de fichier ----------------------------------------------------------
	printf("\nFile type : ");
	switch(header->e_type) {
		case 0:
			printf("None"); break;
		case 1:
			printf("Relocatable"); break;
		case 2:
			printf("Executable"); break;
		case 3:
			printf("Shared"); break;
		case 4:
			printf("Core"); break;
	}
	//- Machine cible ------------------------------------------------------------
	printf("\nTarget architecture : ");
	switch(header->e_machine) {
		case 40:
			printf("ARM"); break;
		case 62:
			printf("x64"); break;
		default:
			printf("Other"); break;
	}
	//- Version ------------------------------------------------------------------
	printf("\nVersion : %d\n",header->e_version);
	//- Autres -----------------------------------------------------------
	printf("Entry point : %d\n",header->e_entry);
	printf("Program Header Offset : %d\n",header->e_phoff);
	printf("Section Header Offset : %d\n",header->e_shoff);
	printf("Flags : %d\n",header->e_flags);
	printf("ELF Header Size : %d\n",header->e_ehsize);
	printf("Program Header Entry Size : %d\n",header->e_phentsize);
	printf("Program Header Entry Number : %d\n",header->e_phnum);
	printf("Section Header Entry Size : %d\n",header->e_shentsize);
	printf("Section Header Entry Number : %d\n",header->e_shnum);
	printf("Index of .shstrtab Section Header : %d\n",header->e_shstrndx);
}

#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "elfStruct.h"

void reverseBytesHeader(Elf32_Ehdr* header) {
	header->e_type = reverse_2(header->e_type);
	header->e_machine = reverse_2(header->e_machine);
	header->e_version = reverse_4(header->e_version);
	header->e_entry = reverse_4(header->e_entry);
	header->e_phoff = reverse_4(header->e_phoff);
	header->e_shoff = reverse_4(header->e_shoff);
	header->e_flags = reverse_4(header->e_flags);
	header->e_ehsize = reverse_2(header->e_ehsize);
	header->e_phentsize = reverse_2(header->e_phentsize);
	header->e_phnum = reverse_2(header->e_phnum);
	header->e_shentsize = reverse_2(header->e_shentsize);
	header->e_shnum = reverse_2(header->e_shnum);
	header->e_shstrndx = reverse_2(header->e_shstrndx);
}

int verifELF(FILE *f) {
	fseek(f,0,SEEK_SET);
	char magicTable[4] = {0x7F,0x45,0x4C,0x46};
	char c;
	int cmt = 0;
	int valid = 1;

	fread(&c,1,1,f);
	while (c!=EOF && cmt<4 && valid) {
		if (c!=magicTable[cmt]) valid = 0;
		else cmt++;
		fread(&c,1,1,f);
	}
	return (valid && cmt==4);
}

void initHeader(ElfFileStruct* elf,FILE* f) {
	fseek(f,0,SEEK_SET);
	elf->header = malloc(sizeof(Elf32_Ehdr));
	if (elf->header==NULL) printf("Echec d'allocation mémoire\n");
	else {
		fread(elf->header,1,sizeof(Elf32_Ehdr),f);
		if (elf->header->e_ident[5]==2) reverseBytesHeader(elf->header);
	}
}

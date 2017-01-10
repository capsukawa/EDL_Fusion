#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include "elfStruct.h"
#include "util.h"

void reverseBytesShdr(Elf32_Shdr* section) {
	section->sh_name = reverse_4(section->sh_name);
	section->sh_type = reverse_4(section->sh_type);
	section->sh_flags = reverse_4(section->sh_flags);
	section->sh_addr = reverse_4(section->sh_addr);
	section->sh_offset = reverse_4(section->sh_offset);
	section->sh_size = reverse_4(section->sh_size);
	section->sh_link = reverse_4(section->sh_link);
	section->sh_info = reverse_4(section->sh_info);
	section->sh_addralign = reverse_4(section->sh_addralign);
	section->sh_entsize = reverse_4(section->sh_entsize);
}

int findSectionHeader(char* name, ElfFileStruct* elf) {
	int i;
	i = 0;
	while ((i!=elf->header->e_shnum) && ((strcmp(elf->sections[i]->name,name)) != 0)) {
		i=i+1;
	}
	return i;
}

void initSectionHeader(ElfFileStruct* elf, FILE* f) {
	fseek(f,elf->header->e_shoff, SEEK_SET);
	int i;
	for(i=0; i< elf->header->e_shnum; i++) {
		elf->sections[i] = malloc(sizeof(Elf_Section));
		elf->sections[i]->header = malloc(sizeof(Elf32_Shdr));
		fread(elf->sections[i]->header,1,sizeof(Elf32_Shdr),f);
		if (elf->header->e_ident[5]==2) reverseBytesShdr(elf->sections[i]->header);
	}
}

void initSectionNames(ElfFileStruct* elf, FILE* f) {
	fseek(f, elf->header->e_shoff, SEEK_SET);
	int i;
	char c;
	int cmt = 0;
	int curPlace;
	// Remplissage de sectionNames
	for (i=0; i<elf->header->e_shnum;i++) {
		curPlace = elf->sections[elf->header->e_shstrndx]->header->sh_offset + elf->sections[i]->header->sh_name;
		fseek(f, curPlace, SEEK_SET);
		fread(&c,1,1,f);
		while(c != 0x00) {
			cmt++;
			fread(&c,1,1,f);
		}
		elf->sections[i]->name = malloc(sizeof(char)*cmt);
		cmt = 0;
		fseek(f, curPlace, SEEK_SET);
		fread(&c,1,1,f);
		while(c != 0x00) {
			elf->sections[i]->name[cmt] = c;
			cmt++;
			fread(&c,1,1,f);
		}
	}
}

void initSectionContent(ElfFileStruct* elf, FILE* f) {
	Elf32_Word size = SHT_NOBITS;
	for (int i=0; i<elf->header->e_shnum;i++) {
		size = elf->sections[i]->header->sh_size;
		elf->sections[i]->content = malloc(sizeof(unsigned char)*size);
		fseek(f,elf->sections[i]->header->sh_offset,SEEK_SET);
		fread(elf->sections[i]->content,1,size,f);
	}
}

void initSections(ElfFileStruct* elf, FILE* f) {
	elf->sections = malloc(sizeof(Elf_Section)*elf->header->e_shnum);
	if (elf->sections != NULL) {
		initSectionHeader(elf,f);
		initSectionNames(elf,f);
		initSectionContent(elf,f);
	}
}

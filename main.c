#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"

void reverseBytes(Elf32_Ehdr* header) {
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

Elf32_Ehdr* initHeader(char* fadr) {
	FILE *f;
	f = fopen(fadr,"r");
	if (f==NULL) {
		printf("Echec d'ouverture du fichier\n");
		return NULL;
	} else {
		Elf32_Ehdr* header;
		header = malloc(sizeof(Elf32_Ehdr));
		if (header==NULL) printf("Echec d'allocation mémoire\n");
		else {
			fread(header->e_ident,1,16,f);
			fread(&header->e_type,2,1,f);
			fread(&header->e_machine,2,1,f);
			fread(&header->e_version,4,1,f);
			fread(&header->e_entry,4,1,f);
			fread(&header->e_phoff,4,1,f);
			fread(&header->e_shoff,4,1,f);
			fread(&header->e_flags,4,1,f);
			fread(&header->e_ehsize,2,1,f);
			fread(&header->e_phentsize,2,1,f);
			fread(&header->e_phnum,2,1,f);
			fread(&header->e_shentsize,2,1,f);
			fread(&header->e_shnum,2,1,f);
			fread(&header->e_shstrndx,2,1,f);
			if (header->e_ident[5]==2) reverseBytes(header);
		}
		fclose(f);
		return header;
	}
}

int main(int argc, char* argv[]) {
	if (argc<2) printf("Pas de fichier en argument\n");
	else {
		Elf32_Ehdr* header;
		header = initHeader(argv[1]);
		printf("e_ident : %s\n",header->e_ident);	
		printf("e_type : %d\n",header->e_type);
		printf("e_machine : %d\n",header->e_machine);
		printf("e_version : %d\n",header->e_version);
		printf("e_entry : %d\n",header->e_entry);
		printf("e_phoff : %d\n",header->e_phoff);
		printf("e_shoff : %d\n",header->e_shoff);
		printf("e_flags : %d\n",header->e_flags);
		printf("e_ehsize : %d\n",header->e_ehsize);
		printf("e_phentsize : %d\n",header->e_phentsize);
		printf("e_phnum : %d\n",header->e_phnum);
		printf("e_shentsize : %d\n",header->e_shentsize);
		printf("e_shnum : %d\n",header->e_shnum);
		printf("e_shstrndx : %d\n",header->e_shstrndx);
	}
		
}

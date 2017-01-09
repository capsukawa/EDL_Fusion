#include <elf.h>
#include <stdio.h>
#include <string.h>
#include "elfStruct.h"

void afficheHeader(ElfFileStruct* elf) {
	//- Identification -----------------------------------------------------------
	printf("Identification : ");
	for (int i=0;i<EI_NIDENT;i++) printf("%x ",elf->header->e_ident[i]);

	printf("\n - Architecture -> ");
	if (elf->header->e_ident[4]==0) printf("None");
	else if (elf->header->e_ident[4]==1) printf("32bits");
	else if (elf->header->e_ident[4]==2) printf("64bits");

	printf("\n - Endianness -> ");
	if (elf->header->e_ident[5]==0) printf("None");
	else if (elf->header->e_ident[5]==1) printf("LSB");
	else if (elf->header->e_ident[5]==2) printf("MSB");

	printf("\n - Header Version -> ");
	if (elf->header->e_ident[6]==0) printf("0 (old)");
	else if (elf->header->e_ident[6]==1) printf("1 (current)");

	printf("\n - OS/ABI -> ");
	switch(elf->header->e_ident[7]) {
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
	switch(elf->header->e_type) {
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
	switch(elf->header->e_machine) {
		case 40:
			printf("ARM"); break;
		case 62:
			printf("x64"); break;
		default:
			printf("Other"); break;
	}
	//- Version ------------------------------------------------------------------
	printf("\nVersion : %d\n",elf->header->e_version);
	//- Autres -------------------------------------------------------------------
	printf("Entry point : %d\n",elf->header->e_entry);
	printf("Program Header Offset : %d\n",elf->header->e_phoff);
	printf("Section Header Offset : %d\n",elf->header->e_shoff);
	printf("Flags : %d\n",elf->header->e_flags);
	printf("ELF Header Size : %d\n",elf->header->e_ehsize);
	printf("Program Header Entry Size : %d\n",elf->header->e_phentsize);
	printf("Program Header Entry Number : %d\n",elf->header->e_phnum);
	printf("Section Header Entry Size : %d\n",elf->header->e_shentsize);
	printf("Section Header Entry Number : %d\n",elf->header->e_shnum);
	printf("Index of .shstrtab Section Header : %d\n",elf->header->e_shstrndx);
}

void afficheType(unsigned int type) {
	char* chaine = "";
	switch(type) {
		case SHT_NULL:
			chaine = "SHT_NULL";
			break;
		case SHT_PROGBITS:
			chaine = "SHT_PROGBITS";
			break;
		case SHT_SYMTAB:
			chaine = "SHT_SYMTAB";
			break;
		case SHT_STRTAB:
			chaine = "SHT_STRTAB";
			break;
		case SHT_RELA:
			chaine = "SHT_RELA";
			break;
		case SHT_HASH:
			chaine = "SHT_HASH";
			break;
		case SHT_DYNAMIC:
			chaine = "SHT_DYNAMIC";
			break;
		case SHT_NOTE:
			chaine = "SHT_NOTE";
			break;
		case SHT_NOBITS:
			chaine = "SHT_NOBITS";
			break;
		case SHT_REL:
			chaine = "SHT_REL";
			break;
		default:
			printf("%-20x", type);
	}
	if (strcmp(chaine,"")) {
		printf("%-20s", chaine);
	}
}

void afficheFanions(unsigned int flag) {
	char chaine[10] = "";
	int i = 0;

	if(flag & 0x1) {
		chaine[i] = 'W';
		i++;
	}
	if(flag & 0x2) {
		chaine[i] = 'A';
		i++;
	}
	if(flag & 0x4) {
		chaine[i] = 'X';
		i++;
	}
	if(flag & 0x10) {
		chaine[i] = 'M';
		i++;
	}
	if(flag & 0x20) {
		chaine[i] = 'S';
		i++;
	}
	if(flag & 0x40) {
		chaine[i] = 'I';
		i++;
	}
	if(flag & 0x80) {
		chaine[i] = 'L';
		i++;
	}
	if(flag & 0x100) {
		chaine[i] = 'O';
		i++;
	}
	if(flag & 0x200) {
		chaine[i] = 'G';
		i++;
	}
	if(flag & 0x400) {
		chaine[i] = 'T';
		i++;
	}
	if(flag & 0x0ff00000) {
		chaine[i] = 'o';
		i++;
	}
	if(flag & 0x80000000) {
		chaine[i] = 'E';
		i++;
	}
	if(flag & 0xf0000000) {
		chaine[i] = 'p';
		i++;
	}
	printf("%-10s", chaine);
}

void afficheSectionHeaderTable(ElfFileStruct* elf) {
	printf("%-4s%-20s%-20s%-10s%-8s %-8s %-8s %-8s %-8s %-8s %-8s\n",
			"Num", "Name", "Type", "Flags", "Adr", "Offset", "Size", "Link", "Info", "Adalign", "Entsize");

	int i;
	for(i=0; i<elf->header->e_shnum; i++) {
		printf("%-4d",i);
		printf("%-20s", elf->sections[i]->name);
		afficheType(elf->sections[i]->header->sh_type);
		afficheFanions(elf->sections[i]->header->sh_flags);
		printf("%08x ", elf->sections[i]->header->sh_addr);
		printf("%08x ", elf->sections[i]->header->sh_offset);
		printf("%08x ", elf->sections[i]->header->sh_size);
		printf("%08x ", elf->sections[i]->header->sh_link);
		printf("%08x ", elf->sections[i]->header->sh_info);
		printf("%-8x ", elf->sections[i]->header->sh_addralign);
		printf("%-8x\n", elf->sections[i]->header->sh_entsize);
	}
	printf(" W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)\n I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu)\n O (traiterment additionnel requis pour l\'OS) o (spécifique à l\'OS), p (spécifique au processeur)\n");
}

void afficheSectionContent(ElfFileStruct* elf,int numSection) {
	printf("  0x00000000 ");
	int taille = elf->sections[numSection]->header->sh_size;
	for (int i=0; i < taille; i=i+4) { // affichage
		printf("%02x",elf->sections[numSection]->content[i]);
		if (i+1 < taille) printf("%02x",elf->sections[numSection]->content[i+1]);
		if (i+2 < taille)	printf("%02x",elf->sections[numSection]->content[i+2]);
		if (i+3 < taille) printf("%02x ",elf->sections[numSection]->content[i+3]);
	}
	printf("\n");
}

void afficheTypeSymbole(unsigned char type) {
	char* chaine = "";
	switch(type) {
		case STT_NOTYPE:
			chaine="NOTYPE";
			break;
		case STT_OBJECT:
			chaine = "OBJECT";
			break;
		case STT_FUNC:
			chaine = "FUNC";
			break;
		case STT_SECTION:
			chaine = "SECTION";
			break;
		case STT_FILE:
			chaine = "FILE";
			break;
		default:
			printf("%-16x", type);
	}
	if (strcmp(chaine,"")) printf("%-16s", chaine);
}

void afficheSymbolTable(ElfFileStruct* elf) {
	printf("%-4s%-16s%-8s%-8s%-16s%-16s%-8s","N", "Name","Value","Size","Type","Vis","Ndx");
	printf("\n");

	int i;
	for(i=0; i<elf->nbSym; i++) {
		// --AFFICHAGE--

		printf("%-4d", i); // index
		printf("%-16s", elf->symbols[i]->name); // name
		printf("%-8i", elf->symbols[i]->sym->st_value); // value
		printf("%-8i", elf->symbols[i]->sym->st_size); // size
		afficheTypeSymbole(elf->symbols[i]->sym->st_info); // type
		printf("%-16s","DEFAULT");	// vis

		//index
		if (elf->symbols[i]->sym->st_shndx == 0) { printf("%-8s","UND"); }
		else if (elf->symbols[i]->sym->st_shndx == 0xfff1) { printf("%-8s","ABS"); }
		else { printf("%-8x", elf->symbols[i]->sym->st_shndx); }
		printf("\n");
	}
}

void afficheTypeRel(Elf32_Rel* rel) {
	char* chaine = "";
	switch(ELF32_R_TYPE(rel->r_info)) {
		case R_ARM_ABS32:
			chaine = "R_ARM_ABS32";
			break;
		case R_ARM_CALL:
			chaine = "R_ARM_CALL";
			break;
		default:
			printf("%-16x ", ELF32_R_TYPE(rel->r_info));
	}
	if (strcmp(chaine,"")) printf("%-16s ", chaine);
}

void afficheRelTable(ElfFileStruct* elf) {
	int i;
	unsigned char infoSymRel;
	unsigned char typeSymRel;
	Elf32_Half indexSectionSymRel;

	printf("%-8s %-8s %-16s %-20s\n", "Offset","Info","Type","Sym");
	for(i=0;i<elf->nbRel;i++) {
		printf(" %08x %08x ", elf->relTab[i]->r_offset, elf->relTab[i]->r_info);
		afficheTypeRel(elf->relTab[i]);
		infoSymRel = ELF32_R_SYM(elf->relTab[i]->r_info);
		typeSymRel = ELF32_ST_TYPE(elf->symbols[infoSymRel]->sym->st_info);
		if(typeSymRel == STT_SECTION) {
			indexSectionSymRel = elf->symbols[infoSymRel]->sym->st_shndx;
			printf("%-20s", elf->sections[indexSectionSymRel]->name);
		} else printf("%-20s", elf->symbols[infoSymRel]->name);
		printf("\n");
	}
}

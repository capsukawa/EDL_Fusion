#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

//affiche le type d'une entrée de la table de rel
void afficheTypeRel(Elf32_Rel* rel) {
	char* chaine = "";
	switch(ELF32_R_TYPE(rel->r_info))
	{
		case R_ARM_ABS32:
			chaine = "R_ARM_ABS32";
			break;
		case R_ARM_CALL:
			chaine = "R_ARM_CALL";
			break;
		default:
			printf("%-16x ", ELF32_R_TYPE(rel->r_info));
	}
	if (strcmp(chaine,""))
	{
		printf("%-16s ", chaine);
	}
}

//affiche le symbole correspondant à une entrée de la table de rel (besoin de la table des symboles)
void afficheSymRel(Elf32_Rel* rel, Elf32_Sym** symTab, char** symNames, char** sectionNames) {
	unsigned char infoSymRel = ELF32_R_SYM(rel->r_info);
	unsigned char typeSymRel = ELF32_ST_TYPE(symTab[infoSymRel]->st_info);
	if(typeSymRel == STT_SECTION) {
		Elf32_Half indexSectionSymRel = symTab[ELF32_R_SYM(rel->r_info)]->st_shndx;
		printf("%-20s", sectionNames[indexSectionSymRel]);
	} else {
		printf("%-20s", symNames[ELF32_R_SYM(rel->r_info)]);
	}
}

int getNbRelEntries(Elf32_Shdr** sectionTable, Elf32_Ehdr* header)
{
	int i, nbEntries=0;
	for (i=0;i<header->e_shnum;i++) //pour chaque section
	{
		//REL
		if(sectionTable[i]->sh_type == SHT_REL)
		{
			nbEntries += sectionTable[i]->sh_size/sectionTable[i]->sh_entsize;
		}
	}
	return nbEntries;
}

void readRel(Elf32_Shdr** sectionTable, Elf32_Ehdr* header, Elf32_Rel** relTable, FILE* f)
{
	int i, j=0;
	for (i=0;i<header->e_shnum;i++) //pour chaque section
	{
		//REL
		if(sectionTable[i]->sh_type == SHT_REL)
		{
			int nbEntries = sectionTable[i]->sh_size/sectionTable[i]->sh_entsize;
			int tailleEntry = sizeof(Elf32_Rel);
			for(j=j;j<nbEntries;j++) { //pour chaque entrée dans la table de relocation
				fseek(f,sectionTable[i]->sh_offset+j*tailleEntry,SEEK_SET); //seek au bon endroit
				relTable[j]=malloc(sizeof(Elf32_Rel)); //allocation memoire
				fread(relTable[j], 1, tailleEntry, f); //lecture dans le fichier et affectation dans la structure
				
				//endianess
				if (header->e_ident[5]==2) 
				{
					relTable[j]->r_offset = reverse_4(relTable[j]->r_offset);
					relTable[j]->r_info = reverse_4(relTable[j]->r_info);
				}
			}
		}
	}
}


//affiche une entrée de la table de relocation (besoin de la table des symboles)
void afficheRel(Elf32_Rel* rel, Elf32_Sym** symTab, char** symNames, char** sectionNames) {
	printf(" %08x %08x ", rel->r_offset, rel->r_info);
	afficheTypeRel(rel);
	afficheSymRel(rel, symTab, symNames, sectionNames);
	printf("\n");
}

//affiche la table de relocation passée en paramètre (besoin de la table des symboles)
void afficheTableRel(Elf32_Rel** relTab, Elf32_Sym** symTab, char** symNames, char** sectionNames, int nbElem) {
	int i;
	printf("%-8s %-8s %-16s %-20s\n", "Offset","Info","Type","Sym");
	for(i=0;i<nbElem;i++) {
		afficheRel(relTab[i], symTab, symNames, sectionNames);
	}
}

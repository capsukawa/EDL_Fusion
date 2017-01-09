#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"
#include "header.h"
#include "sectionTable.h"
#include "sectionNames.h"
#include "afficheHeader.h"
#include "contenuSection.h"
#include "afficheSectionTable.h"
#include "tableSymbole.h"
#include "fusionSection.h"
#include "relocation.h"

typedef struct
{
	Elf32_Ehdr* header;
	
	Elf32_Shdr** sectionTable;
	char** sectionNames;
	
	Elf32_Sym** symTab;
	char** tabNomSym;
	int nbSym;
	
	Elf32_Rel** relTab;
	int nbRel;
	
	unsigned char** section;
} ElfFileStruct;

void remplirStruct(FILE * f, ElfFileStruct * elf)
{
	//remplissage header
	elf->header = initHeader(f);

	//remplissage sectionTable & sectionNames
	elf->sectionTable = malloc(sizeof(Elf32_Shdr)*elf->header->e_shnum);
	elf->sectionNames = malloc(sizeof(char)* elf->header->e_shnum);//
	
	readSection(elf->sectionTable, elf->header, f);
	readNames(elf->sectionNames, elf->header, f, elf->sectionTable);

	// remplissage de tabNomSym & symTab
	int i = rechercheSectionHeader(".symtab",elf->header,elf->sectionNames);
	elf->nbSym = (elf->sectionTable[i]->sh_size)/16;
	elf->symTab = malloc(sizeof(Elf32_Sym)*elf->nbSym);
	elf->tabNomSym = malloc(sizeof(char)*elf->nbSym);
	
	readSymTab(elf->header, f, elf->sectionTable, elf->sectionNames, elf->symTab, elf->tabNomSym);

//printf("%d\n\n",elf->symTab[0]->st_size);
	elf->nbRel = getNbRelEntries(elf->sectionTable, elf->header);
	elf->relTab = malloc(sizeof(Elf32_Rel)*elf->nbRel);
	
	readRel(elf->sectionTable, elf->header, elf->relTab, f);
}

int main(int argc, char* argv[]) {
	if (argc<=1) printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	else {
		int option = recupererOption(argv[1]);
		if (argc<3 || option==OPT_H)
			printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");

		else if (argc!=4 && option==OPT_S)
			printf("Usage : \"./edl_fusion -s [nom_fichier] [numero_section]\"\n");

		else if (option!=NO_OPT){
			ElfFileStruct elf1;
			FILE *f;
			FILE *f2;
			f = fopen(argv[2],"r");
			if (f==NULL) printf("Echec d'ouverture du fichier\n");
			else if (!verifELF(f)) printf("Le fichier donné en paramètre n'est pas un fichier ELF.\n");
			else {
				//~ //remplissage header
				//~ elf1.header = initHeader(f);

				//~ //remplissage sectionTable & sectionNames
				//~ elf1.sectionTable = malloc(sizeof(Elf32_Shdr)*elf1.header->e_shnum);
				//~ elf1.sectionNames = malloc(sizeof(char)* elf1.header->e_shnum);//
				
				//~ readSection(elf1.sectionTable, elf1.header, f);
				//~ readNames(elf1.sectionNames, elf1.header, f, elf1.sectionTable);

				//~ // remplissage de tabNomSym & symTab
				//~ int i = rechercheSectionHeader(".symtab",elf1.header,elf1.sectionNames);
				//~ int nbSym = (elf1.sectionTable[i]->sh_size)/16;
				//~ elf1.symTab = malloc(sizeof(Elf32_Sym)*nbSym);
				//~ elf1.tabNomSym = malloc(sizeof(char)*nbSym);

				//~ readSymTab(elf1.header, f, elf1.sectionTable, elf1.sectionNames, elf1.symTab, elf1.tabNomSym);

				//~ int nbRel = getNbRelEntries(elf1.sectionTable, elf1.header);
				//~ elf1.relTab = malloc(sizeof(Elf32_Rel)*nbRel);
				
				//~ readRel(elf1.sectionTable, elf1.header, elf1.relTab, f);
				
				remplirStruct(f, &elf1);

				switch(option) {
					case OPT_E:
						afficheHeader(elf1.header);
						break;
					case OPT_S:
						affichageSection(elf1.header,f,argv[3],elf1.sectionNames);
						break;
					case OPT_SH:
						afficheSectionTable(elf1.sectionTable, elf1.header, elf1.sectionNames);
						break;
					case OPT_TS:
						afficherTableSymbole(elf1.symTab,elf1.tabNomSym,elf1.nbSym);
						break;
					case OPT_R:
						afficheTableRel(elf1.relTab, elf1.symTab, elf1.tabNomSym, elf1.sectionNames, elf1.nbRel);
						break;
					case OPT_F:
						f2 = fopen(argv[3],"r");
						Elf32_Ehdr* header2 = initHeader(f2);
						Elf32_Shdr* sectionTable2[elf1.header->e_shnum];
						char* sectionNames2[elf1.header->e_shnum];
						readSection(sectionTable2, header2, f2);
						readNames(sectionNames2, header2, f2, sectionTable2);
						fusionSection(elf1.header,header2, f, f2, elf1.sectionNames, sectionNames2, elf1.sectionTable, sectionTable2);
						fclose(f2);
						break;
				}
				fclose(f);
			}
		}
		else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
	}
	return 0;
}

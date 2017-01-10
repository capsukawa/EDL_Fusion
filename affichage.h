#ifndef AFFICHAGE_H
#define AFFICHAGE_H

/*
Affiche l’entête d’un fichier ELF.
*/
void afficheHeader(ElfFileStruct* elf);

/*
Affiche tous les entête de section sous la forme de tableau.
S’aide des procédures ‘afficheType’ et ‘afficheFanions’.
*/
void afficheSectionHeaderTable(ElfFileStruct* elf);

/*
Affiche en hexadécimal le contenu de la section d’index donné en paramètre.
*/
void afficheSectionContent(ElfFileStruct* elf,char* idSection);

/*
Affiche la table des symboles
*/
void afficheSymbolTable(ElfFileStruct* elf);

/*
Affiche la table de ré implémentation
*/
void afficheRelTable(ElfFileStruct* elf);


#endif

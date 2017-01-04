#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "afficheHeader.h"

int main(int argc, char* argv[]) {
	switch(argv[1]) {
    case "-h": //Aide
      break;
    case "-e": //Affichage du header
      afficheHeader(argv[2]);
      break;
    default:
      printf("Commande non reconnue\n");
      break;
  }
}

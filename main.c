#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "afficheHeader.h"

int main(int argc, char* argv[]) {
 if (argc>=2 && !strcmp(argv[1],"-h")) printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
 else if (argc==3) {
   if (!strcmp(argv[1],"-e")) afficheHeader(argv[2]);
 }
 else printf("Usage : \"./edl_fusion [-option] [nom_fichier]\"\n");
 return 0;
}

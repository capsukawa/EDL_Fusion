CC=gcc
FLAGS=-Wall -Werror

edl_fusion: main.o afficheHeader.o header.o contenuSection.o afficheSectionTable.o sectionTable.o tableSymbole.o options.o sectionNames.o fusionSection.o relocation.o
	$(CC) -o $@ $^

%.o: %.c %.h header.h
	$(CC) $(FLAGS) -c $<

main.o: sectionTable.h afficheSectionTable.h sectionTable.h tableSymbole.h relocation.h
afficheSectionTable.o: sectionTable.h

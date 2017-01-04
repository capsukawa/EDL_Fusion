CC=gcc
FLAGS=-Wall -Werror

edl_fusion: main.o afficheHeader.o header.o contenuSection.o afficheSectionTable.o sectionTable.o tableSymbole.o
	$(CC) -o $@ $^

main.o: main.c afficheHeader.h header.h tableSymbole.h
	$(CC) $(FLAGS) -c $<

afficheHeader.o: afficheHeader.c afficheHeader.h header.h
	$(CC) $(FLAGS) -c $<

header.o: header.c header.h
	$(CC) $(FLAGS) -c $<

contenuSection.o: contenuSection.c contenuSection.h
	$(CC) $(FLAGS) -c $<

afficheSectionTable.o: afficheSectionTable.c sectionTable.h header.h
	$(CC) $(FLAGS) -c $<

sectionTable.o: sectionTable.c sectionTable.h header.h
	$(CC) $(FLAGS) -c $<

tableSymbole.o: tableSymbole.c tableSymbole.h header.h
	$(CC) $(FLAGS) -c $<

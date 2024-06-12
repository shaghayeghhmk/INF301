CC=clang
CFLAGS=-g -Wall -Wextra -Werror -gdwarf-4

EXEC=client-introduction client-tutoriel client-interactif client-tutorielFORT client-projetX crypteMove BayOfPigs crypteSeq planB

all: $(EXEC)

client-interactif: client-interactif.o client.o

client-tutoriel: client-tutoriel.o client.o

client-tutorielFORT: client-tutorielFORT.o client.o

client-introduction: client-introduction.o client.o

client-projetX: client-projetX.o client.o

crypteMove: crypteMove.o client.o

BayOfPigs: BayOfPigs.o client.o

crypteSeq: crypteSeq.o client.o

planB: planB.o client.o

client.o: client.c client.h

clean:
	rm -f *.o

clear:
	rm -f $(EXEC)

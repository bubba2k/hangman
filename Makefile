SRCFILES := $(wildcard src/*.cpp)
CFLAGS   := -Wall -Wextra

default:
	g++ $(CFLAGS) $(SRCFILES) -o hangman

debug:
	g++ $(CFLAGS) -g $(SRCFILES) -o hangman

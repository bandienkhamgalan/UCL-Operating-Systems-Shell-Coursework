C := gcc
CFLAGS := -Wall -Werror -std=c99 -g

Src := main Shell HashTable Helpers
Obj := $(addsuffix .o, $(Src))

default: $(Obj)
	@$(C) $(CFLAGS) -o shell  $^

test: test.o HashTable.o Helpers.o Shell.o
	@$(C) $(CFLAGS) -o ShellTest $^
	@./ShellTest || true

clean: 
	@rm -rf Shell
	@rm -rf ShellTest
	@rm -rf *.o

test.o: HashTableTest.h HelpersTest.h ShellTest.h
	@$(C) $(CFLAGS) -o $@ -c $(addsuffix .c, $(basename $@))

%.o: %.c
	@$(C) $(CFLAGS) -o $@ -c $<
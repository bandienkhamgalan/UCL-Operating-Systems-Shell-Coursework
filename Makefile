C := gcc
CFLAGS := -Wall -Werror

Src := main Array Profile HashTable Helpers
Obj := $(addsuffix .o, $(Src))

release: $(Obj)
	@$(C) $(CFLAGS) -o Shell  $^

debug: $(Obj)
	@$(C) $(CFLAGS) -g -o Shell $^

#SUTs := Array
#TestObj := $(addsuffix Test.o, ${SUTs}) $(addsuffix .o, )
test: test.o Array.o HashTable.o Helpers.o
	@$(C) $(CFLAGS) -g -o ShellTest $^
	@./ShellTest || true

clean: 
	@rm -rf Shell
	@rm -rf ShellTest
	@rm -rf *.o

%.o: %.c
	@$(C) $(CFLAGS) -o $@ -c $<

test.o: HashTableTest.h ArrayTest.h HelpersTest.h
	@$(C) $(CFLAGS) -o $@ -c $(addsuffix .c, $(basename $@))
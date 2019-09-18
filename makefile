all: leib pleib clone fork

leib: leib.c
	gcc -o leib leib.c
	
pleib: pleib.c
	gcc  -pthread -o pleib pleib.c 
	
clone: clone.c
	gcc -o clone clone.c

fork: fork.c
	gcc -o fork fork.c

clear: 
	rm leib pleib clone fork
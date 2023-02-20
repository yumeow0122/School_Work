compile = gcc

shellDest = ./src/


shell.exe: shell.o
	${compile} -o shell.exe shell.o
shell.o: ${shellDest}shell.c
	${compile} -c ${shellDest}shell.c

clean:
	rm -rf *.o
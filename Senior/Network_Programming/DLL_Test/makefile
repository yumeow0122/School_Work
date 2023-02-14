compile = gcc

call.exe: call.o
	${compile} -o call.exe call.o dll.o

call.o: call.c dll.c
	${compile} -c call.c dll.c

dll.o: dll.c
	${compile} -c dll.c

clean:
	rm -rf *.o
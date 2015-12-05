all: test1 test2 test3 test4 test5 

# linkeditado

test1: tso_lib.o test1.o
	gcc -o test1 tso_lib.o test1.o

test2: tso_lib.o test2.o
	gcc -o test2 tso_lib.o test2.o

test3: tso_lib.o test3.o
	gcc -o test3 tso_lib.o test3.o

test4: tso_lib.o test4.o
	gcc -o test4 tso_lib.o test4.o

test5: tso_lib.o test5.o
	gcc -o test5 tso_lib.o test5.o

#test6: test6.o
#	gcc -o test6 tso_lib/tso_lib.o test6.o

# compilado

tso_lib.o: tso_lib/tso_lib.c 
	gcc -c tso_lib/tso_lib.c

test1.o: test1.c 
	gcc -c test1.c

test2.o: test2.c 
	gcc -c test2.c

test3.o: test3.c 
	gcc -c test3.c

test4.o: test4.c 
	gcc -c test4.c 

test5.o: test5.c 
	gcc -c test5.c

#test6.o: test6.c 
#	gcc -c test6.c	 

clean: 
	rm test1 test2 test3 test4 test5 test1.o tso_lib.o test2.o test3.o test4.o test5.o 
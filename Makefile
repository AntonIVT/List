test:
	g++ -o test test.cpp list.c -lgtest -lpthread
tst:
	gcc -o tst test.c list.c
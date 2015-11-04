gcc -c queue.c -o queue.o
gcc -c graph.c queue.o -o graph.o
gcc -c functions.c graph.o queue.o -o functions.o
gcc main.c functions.o graph.o queue.o -o main.exe
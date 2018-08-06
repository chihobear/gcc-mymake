# gcc-mymake
mimic function "make"
Mimic Unix function "make".
Take in an make file with the format as followed.
Example make file:
@target file : file.c file.h 
@cmd gcc -Wall -c file.c -o file

Algorithm: graph and post traversal.

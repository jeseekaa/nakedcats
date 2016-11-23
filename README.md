# nakedcats

I maked the submission files that are required but yeah

Compression Algorithm:

If the number of files requested does not evenly divide into the length of the string, the remaining leftover length is added to the first file. For example, a string of length 11 divided into three files would result in the strings to be compressed into 5 3 3.

Numerical characters result in a warning message printed out; they are skipped.

Whitespace is skipped (by Professor Francisco's spec).

Expected input is txt file.

Output is in the form of .txt files.

Program will exit if input file already has compressed files in directory (it will not compress test.txt if test_LOLS.txt exists).

Threaded Implementation
compressT_LOLs.c

To call the threaded implementation of LOLS, it is assumed that the user is working in terminal. Compile compressT_LOLS.c.
Input format should be filename, # of files wanted. 

Ex: ./a.out fileToBeCompressed.txt 3


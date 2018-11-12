


                 Read Me


*******************************************************
*  Description of the program
*******************************************************

This program read the matrix dimension and matrix from
text file and multiply using pthread. it also print the
thread id and resulting matrix

*******************************************************
*  Source files
*******************************************************

Name:  main.c
   Main program.  This is the driver program. we can
change the number of thread we want create on global
variable numberOfThread.
   
*******************************************************
*  Circumstances of programs
*******************************************************

   The program runs successfully.  
   
   The program was developed and tested in clion 2017.2.2  
   It was also compiled, run, and tested in ucd csegrid


*******************************************************
*  How to build and run the program
*******************************************************

1. Uncompress the homework.  The homework file is compressed.  
   To uncompress it use the following commands 
       % unzip [sapkotaLab1]

   Now you should see a directory named homework with the files:
        main.cpp
	input.txt
        Readme.txt

2. Build the program.

   gcc -lpthread -o main main.c

3. Run the program by:
    ./main

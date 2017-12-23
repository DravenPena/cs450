# cs450 Operating Systems 

This repository contains work done for my Operating Systems class where we used c and c++ to create functional programs.

sudoku.c
  This program when compiled with gcc sudoku.c will take an input 9x9 sudoku board and tell whether it is a valid board. 
  Ex)
  valid:              invalid:
    123 456 789         111 111 111
    234 567 891         222 222 222
    345 678 912         333 333 333
    
    456 789 123         444 444 444
    567 891 234         555 555 555 
    678 912 345         666 666 666
    
    789 123 456         777 777 777
    891 234 567         888 888 888
    912 345 678         999 999 999
  If the board is valid the program will output "valid sudoku board" other wise it will out put which section there was an error in.

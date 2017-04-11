#include <stdio.h>

//function
int doAdd(int, int);
int doMinus(int, int);

int main(void) {
   // function pointer
   int (*my_func_ptr)(int, int);

   //function pointer indicate to doAdd
   my_func_ptr = doAdd;
   printf("function pointer indicate to doAdd => %d\n", (*my_func_ptr)(5, 3));    //8

   //function pointer indicate to doMinus
   my_func_ptr = doMinus;
   printf("function pointer indicate to doMinus => %d\n", (*my_func_ptr)(5, 3));  //2

   return 0;
}   //end main


int doAdd(int a, int b) {
   return a + b;
}   //end doAdd

int doMinus(int a, int b) {
   return a - b;
}   //end doMinus

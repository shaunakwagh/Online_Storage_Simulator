#include <string.h>
#include <stdlib.h>

char *s;


   main(){


   	s = (char *) malloc(20);
   	int i = 4;
   	char grt[6]  = "Hello";
   	char name[8]  = "Shaunak";

   	printf("1111\n");
   	memset(s+0, i, 4);
   	printf("2222\n");
   	memset(s+5,grt, 5);
   	printf("3333\n");
   	memset(s+11, name, 7);
   	printf("s is %s", s);
   }
#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[])
{
   FILE *in;
   int c;

   assert(argc == 2);
   in = fopen(argv[1], "r");
   assert(in);
   while ((c = fgetc(in)) != EOF) {
      if (c == '\n') {
         putchar('$');
      }
      putchar(c);
   }
   return 0;
}

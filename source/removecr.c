// takes input.txt and copies it to output.txt, but uses newline (0x0A)
// instead of carriage return + line feed (like when you hit enter in 
// a text file in windows) 0x0D 0x0A

#include <stdlib.h>
#include <stdio.h>



void usage(){
   fprintf(stderr, "Usage: removecrlf input.txt output.txt");
}


int main(int argc, char* argv[]){
   FILE *in, *out;

   if (argc != 3) usage();
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "wb");

   int byte = 0x00;
   int prev_byte = 0x00;


   // use fgets and then search for 0x0D followed by 0x0A
   int MAX_LEN = 250;
   char line[MAX_LEN];
   while( fgets(line, MAX_LEN, in) != NULL){
      int i=0;
      for (;i<MAX_LEN;i++){ // search for windows carriage return/line feed
         if (line[i-1]==0x0D && line[i]==0x0A){
            line[i-1]=0x0A;
            line[i]='\0';
         }
      }

      fprintf(out, "%s", line);
   }

   fclose(in);
   fclose(out);

   return 0;
}

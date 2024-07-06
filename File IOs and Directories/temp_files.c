/* This program creates the new temporary files using basic two GNU
   C library funtions: 'mkstemp()' and 'tmpfile()'. 
*/

#include "../utils.h"

#define BUF_SIZE     100
 
void main(int argc, char *argv[]) 
{
   int fd; 

   /* mkstemp() */ 

   char temp_file[BUF_SIZE] = "/tmp/canXXXXXX";

   fd = mkstemp(temp_file);
   if (fd == -1)
      raiseError("mkstemp() did not work", file, line); 

   printf("Generated filename was: %s\n", temp_file);
  
   unlink(temp_file);   /* Name disappears immediately, but the file
                           is removed only after close() */
   
   if (close(fd) == -1)
      raiseError("close() did not work", file, line); 

   exit(EXIT_SUCCESS); 
}
 
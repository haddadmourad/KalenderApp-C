#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
     // dest is a buffer with a size of 10
  char dest[10] ={0};

  // src is a string that is longer than the dest buffer
  const char *src = "This string is too long for the destination buffer";

  // The call to strcpy will result in a buffer overrun, causing undefined behavior
  strcpy(dest, src);

  // Print the contents of the dest buffer, which may contain arbitrary data due to the buffer overrun
  printf("Copied string: %s\n", dest);

}




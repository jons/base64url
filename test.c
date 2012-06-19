/**
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codec.h"


int main (int argc, char **argv)
{
  char   dest[4096] = {'\0'},
         undo[4096] = {'\0'};
  size_t slen = 0,
         dlen = 0,
         ulen = 0;
  int    r;

  if (argc > 1)
    slen = strlen(argv[1]);

  r = base64url_encode(dest, 4095, argv[1], slen, &dlen);

  printf("[%d] \"%s\" (%zu)\n", r, dest, dlen);

  r = base64url_decode(undo, 4095, dest, dlen, &ulen);

  printf("[%d] \"%s\" (%zu)\n", r, undo, ulen);

  return 0;
}

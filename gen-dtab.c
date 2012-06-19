/**
 */
#include <stdio.h>
#include "etab.h"

int main (int argc, char **argv)
{
  char base64url_dtab[256];
  int i, j, k = 0;
  for (i = 0; i < 64; i++)
    base64url_dtab[base64url_etab[i]] = i;

  printf("char base64url_dtab[256] = {\n");
  for (i = 0; i < 16; i++) {
    printf("  ");
    for (j = 0; j < 16; j++)
      printf("'\\x%02hhx', ", base64url_dtab[k++]);
    printf("\n");
  }
  printf("};\n");
  return 0;
}

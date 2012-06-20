/**
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


char base64url_etab[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '-', '_'};

int main (int argc, char **argv)
{
  char base64url_dtab[256];
  int i, j, k = 0;
  for (i = 0; i < 64; i++)
    base64url_dtab[(uint8_t)base64url_etab[i]] = i;

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

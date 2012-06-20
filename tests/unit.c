/**
 * test vectors from the RFC
 *
 * BASE64("") = ""
 * BASE64("f") = "Zg=="
 * BASE64("fo") = "Zm8="
 * BASE64("foo") = "Zm9v"
 * BASE64("foob") = "Zm9vYg=="
 * BASE64("fooba") = "Zm9vYmE="
 * BASE64("foobar") = "Zm9vYmFy"
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "librb64u.h"


int main (int argc, char **argv)
{
  const char *evector[7] = {
    "",
    "f",
    "fo",
    "foo",
    "foob",
    "fooba",
    "foobar"};
  const char *dvector[7] = {
    "",
    "Zg",
    "Zm8",
    "Zm9v",
    "Zm9vYg",
    "Zm9vYmE",
    "Zm9vYmFy"};

  char   dest[64] = {'\0'};
  size_t dlen = 0;
  int    r, i;

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_encode(dest, 63, evector[i], strlen(evector[i]), &dlen);
    if (r < 0) {
      printf("FAIL encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strlen(dvector[i]) != dlen) {
      printf("FAIL encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strncmp(dvector[i], dest, dlen)) {
      printf("FAIL encode \"%s\"\n", evector[i]);
      continue;
    }
    printf("PASS encode \"%s\" => \"%s\"\n", evector[i], dest);
  }

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_decode(dest, 63, dvector[i], strlen(dvector[i]), &dlen);
    if (r < 0) {
      printf("FAIL decode \"%s\" r=%d\n", dvector[i], r);
      continue;
    }
    if (strlen(evector[i]) != dlen) {
      printf("FAIL decode \"%s\" => \"%s\"(%zu)\n", dvector[i], dest, dlen);
      continue;
    }
    if (strncmp(evector[i], dest, dlen)) {
      printf("FAIL decode \"%s\" =>\"%s\"\n", dvector[i], dest);
      continue;
    }
    printf("PASS decode \"%s\" => \"%s\"\n", dvector[i], dest);
  }

  return 0;
}

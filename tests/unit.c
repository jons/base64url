/**
 * unit tests for 'make check'
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "librb64u.h"


/**
 * test vector from github user https://github.com/davidcsi
 * https://github.com/jons/base64url/issues/2
 *
 * in the ticket the output was the same, except:
 *   - byte 32 was \x41 not \x40 as expected
 *   - there is an excess byte, \xe3
 */
int davidcsi()
{
  /* 44 input chars               1         2         3         4     */
  /*                     12345678901234567890123456789012345678901234 */
  const char *dvector = "DgvkCrkyBNSy9yHP1Yznyc2DkHTIUXaO6NlEw4CSq0E=";

  /* 32 bytes from `base64 -d` assumed to be the original input */
  const unsigned char *evector =
    "\x0e\x0b\xe4\x0a\xb9\x32\x04\xd4\xb2\xf7\x21\xcf\xd5\x8c\xe7\xc9\xcd\x83\x90\x74\xc8\x51\x76\x8e\xe8\xd9\x44\xc3\x80\x92\xab\x41";

  char   dest[128] = {'\0'};
  size_t dlen = 0;
  int    r;

  r = base64url_decode(dest, 127, dvector, 44, &dlen);
  if (r < 0) {
    printf("FAIL davidcsi r=%d\n", r);
    return -1;
  }

  /* excess byte should not appear; length is exact match */
  if (32 != dlen) {
    printf("FAIL davidcsi dlen=%lu\n", dlen);
    printf("     inspect with:\n");
    printf("echo -n \"%s\" | ./codec/rb64ud | hexdump -C\n", dvector);
    printf("echo -n \"%s\" | base64 -d | hexdump -C\n", dvector);
    return -1;
  }

  /* buffer is exact match */
  if (memcmp(evector, dest, 32)) {
    printf("FAIL davidcsi memcmp\n");
    printf("     inspect with:\n");
    printf("echo -n \"%s\" | ./codec/rb64ud | hexdump -C\n", dvector);
    printf("echo -n \"%s\" | base64 -d | hexdump -C\n", dvector);
    return -1;
  }

  printf("PASS davidcsi\n");
  return 0;
}


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
 */
int rfc()
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
  int    t = 0, r, i;

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_encode(dest, 63, evector[i], strlen(evector[i]), &dlen);
    if (r < 0) {
      t = -1;
      printf("FAIL rfc encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strlen(dvector[i]) != dlen) {
      t = -1;
      printf("FAIL rfc encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strncmp(dvector[i], dest, dlen)) {
      t = -1;
      printf("FAIL rfc encode \"%s\"\n", evector[i]);
      continue;
    }
  }

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_decode(dest, 63, dvector[i], strlen(dvector[i]), &dlen);
    if (r < 0) {
      t = -1;
      printf("FAIL rfc decode \"%s\" r=%d\n", dvector[i], r);
      continue;
    }
    if (strlen(evector[i]) != dlen) {
      t = -1;
      printf("FAIL rfc decode \"%s\" => \"%s\"(%zu)\n", dvector[i], dest, dlen);
      continue;
    }
    if (strncmp(evector[i], dest, dlen)) {
      t = -1;
      printf("FAIL rfc decode \"%s\" =>\"%s\"\n", dvector[i], dest);
      continue;
    }
  }

  printf("PASS rfc\n");
  return t;
}

/**
 * test vectors from the RFC, but with padding
 */
int padded()
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
    "Zg==",
    "Zm8=",
    "Zm9v",
    "Zm9vYg==",
    "Zm9vYmE=",
    "Zm9vYmFy"};

  char   dest[64] = {'\0'};
  size_t dlen = 0;
  int    t = 0, r, i;

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_encode_padded(dest, 63, evector[i], strlen(evector[i]), &dlen);
    if (r < 0) {
      t = -1;
      printf("FAIL padded encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strlen(dvector[i]) != dlen) {
      t = -1;
      printf("FAIL padded encode \"%s\"\n", evector[i]);
      continue;
    }
    if (strncmp(dvector[i], dest, dlen)) {
      t = -1;
      printf("FAIL padded encode \"%s\"\n", evector[i]);
      continue;
    }
  }

  for (i = 0; i < 7; i++) {
    memset(dest, 0, 64);
    r = base64url_decode(dest, 63, dvector[i], strlen(dvector[i]), &dlen);
    if (r < 0) {
      t = -1;
      printf("FAIL padded decode \"%s\" r=%d\n", dvector[i], r);
      continue;
    }
    if (strlen(evector[i]) != dlen) {
      t = -1;
      printf("FAIL padded decode \"%s\" => \"%s\"(%zu)\n", dvector[i], dest, dlen);
      continue;
    }
    if (strncmp(evector[i], dest, dlen)) {
      t = -1;
      printf("FAIL padded decode \"%s\" =>\"%s\"\n", dvector[i], dest);
      continue;
    }
  }

  printf("PASS padded\n");
  return t;
}

/**
 * base64url_encode_padded() tested above does not exercise the re-entrant
 * padding method, so this is done separately here.
 */
int reentrant_padding()
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
    "Zg==",
    "Zm8=",
    "Zm9v",
    "Zm9vYg==",
    "Zm9vYmE=",
    "Zm9vYmFy"};

  int i, j, r;
  char dest[64];
  size_t k;
  b64ue_t s;

  for (i = 0; i < 7; i++)
  {
    k = 0;
    memset(dest, 0, 64);

    /* init */
    base64url_encode_reset(&s);
    /* loop */
    for (j = 0; j < strlen(evector[i]); j++)
    {
      r = base64url_encode_ingest(&s, evector[i][j]);
      if (r < 0) {
        printf("FAIL reentrant_padding encode ingest r=%d\n", r);
        return -1;
      }
      while (r-- > 0) dest[k++] = base64url_encode_getc(&s);
    }
    /* finalize */
    r = base64url_encode_finish(&s);
    if (r < 0) {
      printf("FAIL reentrant_padding encode finalize r=%d\n", r);
      return -1;
    }
    while (r-- > 0) dest[k++] = base64url_encode_getc(&s);
    /* pad */
    r = base64url_encode_pad(&s);
    if (r < 0) {
      printf("FAIL reentrant_padding encode pad r=%d\n", r);
      return -1;
    }
    while (r-- > 0) dest[k++] = base64url_encode_getc(&s);

    /* expected exact length */
    if (k != strlen(dvector[i])) {
      printf("FAIL reentrant_padding length k=%lu dvector=\"%s\"\n", k, dvector[i]);
      return -1;
    }

    /* expected exact match */
    if (memcmp(dest, dvector[i], k)) {
      printf("FAIL reentrant_padding buffer dest=\"%s\" dvector=\"%s\"\n", dest, dvector[i]);
      return -1;
    }
  }

  printf("PASS reentrant_padding\n");
  return 0;
}

/**
 * verify_all helper (see below)
 */
int verify_next(char *source, size_t len, size_t max)
{
  char interim[16] = { '\0' };
  char dvector[256][64] = { '\0' };
  char evector[256][64] = { '\0' };

  size_t k, dlen = 0, elen = 0;
  int    t = 0, r, i;

  memcpy(interim, source, len);

  for (i = 0; i < 256; i++)
  {
    interim[len] = i;

    memset(dvector[i], 0, 64);
    r = base64url_encode(dvector[i], 63, interim, len+1, &dlen);
    if (r < 0)
    {
      t = -1;
      printf("FAIL encode(");
      for (k = 0; k <= len; k++) printf("\\x%hhx", interim[k]);
      printf(") r=%d\n", r);
    }
    else
    {
      memset(evector[i], 0, 64);
      r = base64url_decode(evector[i], 63, dvector[i], dlen, &elen);
      if (r < 0)
      {
        t = -1;
        printf("FAIL decode(\"%s\") r=%d\n", dvector[i], r);
      }
      else if (elen != (len+1))
      {
        t = -1;
        printf("FAIL encode(");
        for (k = 0; k <= len; k++) printf("\\x%hhx", interim[k]);
        printf(") decode(\"%s\") elen=%lu\n", dvector[i], elen);
        /* debug buffer */
        printf("[");
        for (k = 0; k < elen && k < 64; k++) printf("\\x%hhx", evector[i][k]);
        printf("]\n");
      }
      else if (memcmp(interim, evector[i], elen))
      {
        t = -1;
        /* debug both buffers */
        printf("FAIL mismatch\n[");
        for (k = 0; k <= len; k++) printf("\\x%hhx", interim[k]);
        printf("]\n[");
        for (k = 0; k < elen; k++) printf("\\x%hhx", evector[i][k]);
        printf("]\n");
      }
      else if (len+1 < max)
      {
        /* pass so far, but step into next stack frame */
        t += verify_next(interim, len+1, max);
      }
    }
  }
  return t;
}


/**
 * verify all possible N-byte buffers up to... however long it seems
 * reasonable to wait for this to finish. max=4 is too big. max=3 is
 * kind of unnecessary.
 */
int verify_all(char *source, size_t len, size_t max)
{
  if (0 == verify_next(source, len, max))
  {
    printf("PASS verify_all\n");
    return 0;
  }
  return -1;
}


/**
 */
int main (int argc, char **argv)
{
  int r = 0;
  if (rfc())
    r = -1;
  if (padded())
    r = -1;
  if (reentrant_padding())
    r = -1;
  if (davidcsi())
    r = -1;
  if (verify_all("", 0, 2)) /* max=2 */
    r = -1;
  return r;
}
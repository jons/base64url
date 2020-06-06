/**
 * base64url stream encoder
 * read from stdin, encode, and write to stdout.
 * specify -p to include the standard padding.
 * @author jon <jon@wroth.org>
 * CC-BY-4.0
 */
#include <stdio.h>
#include <string.h>
#include "librb64u.h"

/**
 */
int main(int argc, char **argv)
{
  int c, r;
  b64ue_t s;
  base64url_encode_reset(&s);
  for (;;)
  {
    c = getchar();
    if (EOF == c) break;
    r = base64url_encode_ingest(&s, c);
    if (r < 0) return -1;
    while (r-- > 0) printf("%c", base64url_encode_getc(&s));
  }
  r = base64url_encode_finish(&s);
  if (r < 0) return -1;
  while (r-- > 0) printf("%c", base64url_encode_getc(&s));
  if (argc > 1 && !strcmp("-p", argv[0]))
  {
    r = base64url_encode_pad(&s);
    if (r < 0) return -1;
    while (r-- > 0) printf("%c", base64url_encode_getc(&s));
  }
  return 0;
}
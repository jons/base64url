/**
 * base64url stream decoder
 * read from stdin, decode, and write to stdout.
 * @author jon <jon@wroth.org>
 * CC-BY-4.0
 */

#include <stdio.h>
#include "librb64u.h"


/**
 */
int main(int argc, char **argv)
{
  int c, r;
  b64ud_t s;

  base64url_decode_reset(&s);
  for (;;)
  {
    c = getchar();
    if (EOF == c) break;
    r = base64url_decode_ingest(&s, c);
    if (r < 0) return -1;
    if (r > 0) printf("%c", base64url_decode_getc(&s));
  }
  for (;;) {
    r = base64url_decode_finish(&s);
    if (r < 0) return -1;
    if (r == 0) break;
    printf("%c", base64url_decode_getc(&s));
  }
  return 0;
}

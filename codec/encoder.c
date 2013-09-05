/**
 * base64url stream encoder
 * read from stdin, encode, and write to stdout.
 * @author jon@wroth.org
 *
 */

#include <stdio.h>
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
    if (r > 0) printf("%c", base64url_encode_getc(&s));
  }
  for (;;)
  {
    r = base64url_encode_finish(&s);
    if (r < 0) return -1;
    if (r == 0) break;
    printf("%c", base64url_encode_getc(&s));
  }
  return 0;
}

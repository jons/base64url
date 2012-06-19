/**
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "codec.h"
#include "etab.h"
#include "dtab.h"


int base64url_encode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen)
{
  int r;
  size_t i,
         lost = 0,
         dsz = 0;
  b64ue_t s;

  *dlen = 0;
  base64url_encode_reset(&s);
  for (i = 0; i < len; i++)
  {
    r = base64url_encode_ingest(&s, src[i]);
    if (r < 0) return -1;
    if (r > 0) {
      if (maxlen > *dlen)
        dest[dsz++] = base64url_encode_getc(&s);
      else
        lost++;
    }
  }
  for (;;) {
    r = base64url_encode_finish(&s);
    if (r < 0) return -1;
    if (r > 0) {
      if (maxlen > *dlen)
        dest[dsz++] = base64url_encode_getc(&s);
      else
        lost++;
    }
    else
      break;
  }
  if (lost > 0) {
    fprintf(stderr, "base64url_encode: dropped %zu bytes to avoid output buffer overrun.", lost);
    return -1;
  }
  *dlen = dsz;
  return 0;
}


int base64url_decode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen)
{
  int r;
  size_t i,
         lost = 0,
         dsz = 0;
  b64ud_t s;

  *dlen = 0;
  base64url_decode_reset(&s);
  for (i = 0; i < len; i++)
  {
    r = base64url_decode_ingest(&s, src[i]);
    if (r < 0) return -1;
    if (r > 0) {
      if (maxlen > *dlen)
        dest[dsz++] = base64url_decode_getc(&s);
      else
        lost++;
    }
  }
  for (;;) {
    r = base64url_decode_finish(&s);
    if (r < 0) return -1;
    if (r > 0) {   
      if (maxlen > *dlen)
        dest[dsz++] = base64url_decode_getc(&s);
      else
        lost++;
    }
    else
      break;
  }
  if (lost > 0) {
    fprintf(stderr, "base64url_decode: dropped %zu bytes to avoid output buffer overrun.", lost);
    return -1;
  }
  *dlen = dsz;
  return 0;
}


/* re-entrant methods *********************************************************/


void base64url_encode_reset(b64ue_t *state)
{
  state->f = 1;    /* unnamed state flag */
  state->k = 4;    /* triple shift */
  state->t = 0;    /* triple */
  state->i = 0;    /* write index */
  state->j = 0;    /* read index */
  state->q = 0;    /* total bytes read */
  state->n = 0;    /* bytes in buffer, b */
  state->b[0] = 0; /* read buffer */
  state->b[1] = 0;
  state->b[2] = 0;
  state->b[3] = 0;
}


int base64url_encode_getc(b64ue_t *state)
{
  int r = state->r;
  state->r = 0;
  return r;
}


int base64url_encode_ingest(b64ue_t *state, char c)
{
  size_t    i, j, n;
  uint32_t  t, k;

  i = state->i;
  state->b[i] = c;
  state->i = (i > BSZ-2) ? 0 : i + 1;
  state->n++;
  state->q++;

  if (state->f > 0) {
    if (state->n < 3)
      return 0;
    state->f = 0;
  }

  t = state->t;
  k = state->k;
  if (k > 3) {
    k = 3;
    n = state->n;
    assert (n > 2);
    j = state->j;
    t =  (state->b[j] << 0x10);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    t += (state->b[j] << 0x08);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    t += (state->b[j]);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    state->j = j;
    state->t = t;
    state->n = n - 3;
  }

  state->r = base64url_etab[(t >> k * 6) & 0x3f];
  state->k = k - 1;
  return 1;
}


int base64url_encode_finish(b64ue_t *state)
{
  size_t    j, n;
  uint32_t  t, k;
  uint8_t   m;

  n = state->n;

  if (state->f > 0) {
    if (n < 1)
      return 0; 
    state->f = 0;
  }

  t = state->t;
  k = state->k;
  if (k > 3) {
    if (n < 1)
      return 0;
    k = 3;
    j = state->j;
    t =  (state->b[j] << 0x10);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    t += (state->b[j] << 0x08);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    t += (state->b[j]);
    state->b[j] = 0;
    j = (j > BSZ-2) ? 0 : j + 1;
    state->j = j;
    state->t = t;
    state->n = 0;
  }

  if (state->n == 0) {
    m = state->q % 3;
    if (((m == 1) && (k <= 1)) ||
        ((m == 2) && (k < 1)))
      return 0;
  }

  state->r = base64url_etab[(t >> k * 6) & 0x3f];
  state->k = k - 1;
  return 1;
}


void base64url_decode_reset(b64ud_t *state)
{
  state->f = 0; /* unnamed state flag */
  state->s = 0; /* buffer state */
  state->r = 0; /* next return char */
  state->a = 0; /* buffer */
  state->b = 0; /* buffer */
  state->c = 0; /* buffer */
}


int base64url_decode_getc(b64ud_t *state)
{
  int r = state->r;
  state->r = 0;
  return r;
}


int base64url_decode_ingest(b64ud_t *state, unsigned char c)
{
  switch (state->s)
  {
  case 0:
    state->a = base64url_dtab[c];
    state->r = (state->t >> 1 * 8) & 0xff;
    state->k = 0;
    state->s = 1;
    return state->f;

  case 1:
    state->b = base64url_dtab[c];
    state->r = (state->t >> 0 * 8) & 0xff;
    state->k = 2;
    state->s = 2;
    return state->f;

  case 2:
    state->c = base64url_dtab[c];
    state->s = 3;
    return 0;

  case 3:
    state->f = 1;
    state->t = (state->a << 3 * 6)
             + (state->b << 2 * 6)
             + (state->c << 1 * 6)
             + (base64url_dtab[c] << 0 * 6);
    state->a = 0;
    state->b = 0;
    state->c = 0;
    state->r = (state->t >> 2 * 8) & 0xff;
    state->k = 1;
    state->s = 0;
    return 1;
  }
  assert(0);
}


int base64url_decode_finish(b64ud_t *state)
{
  uint8_t k;
  k = state->k;
  if (k > 2)
    return 0;
  if (k > 1) {
    state->t = (state->a << 3 * 6)
             + (state->b << 2 * 6)
             + (state->c << 1 * 6);
  }
  state->r = (state->t >> k * 8) & 0xff;
  state->k--;
  return 1;
}

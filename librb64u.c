/**
 * re-entrant base64url
 * @author jon <jon@wroth.org>
 * CC-BY-4.0
 */
#include <stdlib.h>
#include <stdint.h>
#include "librb64u.h"

/**
 * encoder table
 */
static const unsigned char base64url_etab[64] =
{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '-', '_'};

/**
 * decoder table
 */
static const unsigned char base64url_dtab[256] = {
  '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x01', '\x00', '\x00', '\x00', '\x5c', '\x08', '\x00', '\x00', '\xc0', '\x99', '\x76', '\xb7', '\xe0', '\x96', '\x76', '\xb7',
  '\x45', '\x82', '\x04', '\x08', '\x8c', '\x1a', '\x61', '\xb7', '\xc8', '\x81', '\x04', '\x08', '\x01', '\x3e', '\x00', '\x00',
  '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3a', '\x3b', '\x3c', '\x3d', '\x78', '\xb7', '\xe0', '\xe3', '\xff', '\xbf',
  '\xcf', '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e',
  '\x0f', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', '\x18', '\x19', '\xb7', '\x01', '\x00', '\x00', '\x3f',
  '\x00', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f', '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28',
  '\x29', '\x2a', '\x2b', '\x2c', '\x2d', '\x2e', '\x2f', '\x30', '\x31', '\x32', '\x33', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\xd0', '\xe3', '\xff', '\xbf', '\xc4', '\xe3', '\xff', '\xbf', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x00', '\x00', '\x00', '\x00', '\x10', '\xe4', '\xff', '\xbf', '\x68', '\x66', '\x78', '\xb7', '\x45', '\x82', '\x04', '\x08',
  '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
  '\x66', '\xf5', '\xff', '\xbf', '\x1e', '\x31', '\x67', '\xb7', '\x79', '\x0b', '\x72', '\xb7', '\xbc', '\x96', '\x04', '\x08',
  '\xb8', '\xe3', '\xff', '\xbf', '\xec', '\x82', '\x04', '\x08', '\xf4', '\xbf', '\x75', '\xb7', '\xbc', '\x96', '\x04', '\x08'};


/**
 */
int base64url_encode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen)
{
  int r;
  size_t i, dsz = 0;
  b64ue_t s;
  if (NULL != dlen) *dlen = 0;
  base64url_encode_reset(&s);
  for (i = 0; i < len; i++)
  {
    r = base64url_encode_ingest(&s, src[i]);
    if (r < 0) {
      if (NULL != dlen) *dlen = dsz;
      return -1;
    }
    while (r > 0) {
      if (maxlen <= dsz) {
        if (NULL != dlen) *dlen = dsz;
        return -1;
      }
      dest[dsz++] = base64url_encode_getc(&s);
      r--;
    }
  }
  r = base64url_encode_finish(&s);
  if (r < 0) {
    if (NULL != dlen) *dlen = dsz;
    return -1;
  }
  while (r > 0) {
    if (maxlen <= dsz) {
      if (NULL != dlen) *dlen = dsz;
      return -1;
    }
    dest[dsz++] = base64url_encode_getc(&s);
    r--;
  }
  if (NULL != dlen) *dlen = dsz;
  return s.n;
}

/**
 */
int base64url_encode_padded(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen)
{
  size_t tlen;
  int r;
  if (maxlen < 2)
    return -1;
  r = base64url_encode(dest, maxlen-2, src, len, &tlen);
  if (NULL != dlen) *dlen = tlen;
  if (r < 0) return r;
  if (r > 0) {
    dest[tlen++] = '=';
    if (r < 2)
      dest[tlen++] = '=';
  }
  if (NULL != dlen) *dlen = tlen;
  return 0;
}

/**
 */
int base64url_decode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen)
{
  int r;
  size_t i, dsz = 0;
  b64ud_t s;
  if (NULL != dlen) *dlen = 0;
  base64url_decode_reset(&s);
  for (i = 0; i < len; i++)
  {
    r = base64url_decode_ingest(&s, src[i]);
    if (r < 0) {
      if (NULL != dlen) *dlen = dsz;
      return -1;
    }
    if (r > 0) {
      if (maxlen <= dsz) {
        if (NULL != dlen) *dlen = dsz;
        return -1;
      }
      dest[dsz++] = base64url_decode_getc(&s);
    }
  }
  if (NULL != dlen) *dlen = dsz;
  return 0;
}


/* re-entrant methods *********************************************************/

/**
 */
void base64url_encode_reset(b64ue_t *state)
{
  state->n = 0;  /* current state */
  state->b = 0;  /* last-read buffer */
  state->r1 = 0; /* retval1 */
  state->r2 = 0; /* retval2 */
}

/**
 */
int base64url_encode_getc(b64ue_t *state)
{
  int r = state->r1;
  state->r1 = 0;
  if (r > 0)
    return r;

  r = state->r2;
  state->r2 = 0;
  return r;
}

/**
 */
int base64url_encode_ingest(b64ue_t *state, unsigned char c)
{
  uint8_t i, j, t, n;

  n = state->n;
  switch (n)
  {
    case 0:
      t = (c & 0xfc) >> 2; /* top six */
      state->b = c;
      state->r1 = base64url_etab[t & 0x3f];
      state->n = 1;
      return 1;

    case 1:
      i = (state->b & 0x03) << 4; /* bottom 2 */
      j = (c & 0xf0) >> 4; /* top 4 */
      t = i | j;
      state->b = c;
      state->r1 = base64url_etab[t & 0x3f];
      state->n = 2;
      return 1;

    case 2:
      i = (state->b & 0x0f) << 2; /* bottom 4 */
      j = (c & 0xc0) >> 6; /* top 2 */
      t = i | j;
      state->r1 = base64url_etab[t & 0x3f];
      state->r2 = base64url_etab[c & 0x3f]; /* bottom 6 */
      state->n = 0;
      return 2;
  }
  return -1;
}

/**
 */
int base64url_encode_finish(b64ue_t *state)
{
  uint8_t t;

  switch (state->n)
  {
    case 0:
      return 0;

    case 1:
      t = (state->b & 0x03) << 4; /* bottom 2 */
      state->r1 = base64url_etab[t & 0x3f];
      return 1;

    case 2:
      t = (state->b & 0x0f) << 2; /* bottom 4 */
      state->r1 = base64url_etab[t & 0x3f];
      return 1;
  }
  return -1;
}

/**
 */
int base64url_encode_pad(b64ue_t *state)
{
  switch (state->n)
  {
    case 0:
      return 0;

    case 1:
      state->r1 = '=';
      state->r2 = '=';
      return 2;

    case 2:
      state->r1 = '=';
      return 1;
  }
  return -1;
}

/**
 */
void base64url_decode_reset(b64ud_t *state)
{
  state->n = 0; /* state */
  state->f = 1; /* finishing flag */
  state->b = 0; /* last-read buffer */
  state->r = 0; /* retval */
}

/**
 */
int base64url_decode_getc(b64ud_t *state)
{
  int r = state->r;
  state->r = 0;
  return r;
}

/**
 */
int base64url_decode_ingest(b64ud_t *state, unsigned char c)
{
  uint8_t i, j, t, n, f;

  f = state->f;
  if (c == '=') /* toggle finishing flag */
    state->f = 0;

  n = state->n;
  switch (n)
  {
    case 0:
      state->b = base64url_dtab[c] & 0x3f; /* buffer first 6 bits */
      state->n = 1;
      return 0;

    case 1:
      t = base64url_dtab[c] & 0x3f;
      i = state->b << 2; /* buffered 6 bits, to top */
      j = t >> 4; /* top 2 bits, to bottom */
      state->r = (i & 0xfc) | (j & 0x03);
      state->b = t & 0x0f; /* bottom 4 bits, saved */
      state->n = 2;
      return f;

    case 2:
      t = base64url_dtab[c] & 0x3f;
      i = state->b << 4; /* bottom 4 bits, to top */
      j = t >> 2; /* top 4 bits, to bottom */
      state->r = (i & 0xf0) | (j & 0x0f);
      state->b = t & 0x03; /* bottom 2 bits, saved */
      state->n = 3;
      return state->f;

    case 3:
      t = base64url_dtab[c] & 0x3f; /* bottom 6 bits */
      i = state->b << 6; /* bottom 2 bits, to top */
      state->r = (i & 0xc0) | t;
      state->b = 0;
      state->n = 0;
      return state->f;
  }
  return -1;
}
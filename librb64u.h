/**
 * re-entrant base64url
 * @author jon <jon@wroth.org>
 * CC-BY-4.0
 */
#ifndef LIB_RB64U_H
#define LIB_RB64U_H
#include <stdlib.h>
#include <stdint.h>

typedef struct b64ue b64ue_t;
typedef struct b64ud b64ud_t;

/**
 * encoder state
 */
struct b64ue
{
  uint8_t n;        /* current state */
  unsigned char b;  /* last-read buffer */
  int r1;           /* retval1 */
  int r2;           /* retval1 */
};

/**
 * decoder state
 */
struct b64ud
{
  uint8_t n;        /* current state */
  uint8_t f;        /* finishing flag */
  unsigned char b;  /* last-read buffer */
  int r;            /* retval */
};


/**
 * base64url encode string src with length len into buffer dest, writing up to maxlen bytes.
 * set dlen to the number of bytes written, regardless of success.
 * returns the final encoder state (a non-negative integer) on success, a negative value on failure.
 */
int base64url_encode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);


/**
 * similar to base64url_encode() however also includes the appropriate number of padding characters.
 *
 * calls base64url_encode() with maxlen reduced by 2 in order to ensure space for padding will be
 * available, regardless of whether it is called-for. the caller should account for this. if maxlen
 * is less than 2, returns -1 to indicate failure and does no work.
 *
 * if the base64url_encode() call fails, padding is not added.
 *
 * return zero on success, a negative value on failure.
 */
int base64url_encode_padded(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);


/**
 * base64url decode string src with length len into buffer dest, writing up to maxlen bytes.
 * handles padding, if present.
 * set dlen to the number of bytes written, regardless of success.
 * return zero on success, a negative value on failure.
 */
int base64url_decode(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);


/** re-entrant methods *******************************************************/


/**
 * prepare encoder state.
 * use before ingesting any characters.
 * an encoder state object may be reset at any time, either to abandon a
 * current encoding process or to begin a new one or both.
 */
void base64url_encode_reset(b64ue_t *state);


/**
 * retrieve the next encoded character available. cannot distinguish between
 * success and failure.
 * call this method as many times as base64url_encode_ingest() tells you, and do
 * so before calling base64url_encode_ingest() again.
 */
int base64url_encode_getc(b64ue_t *state);


/**
 * update the encoder state with the next raw input character.
 *
 * on success, returns the number of characters ready for output; call
 * base64url_encode_getc() the specified number of times before calling this
 * method again.
 *
 * on failure, returns a negative value.
 */
int base64url_encode_ingest(b64ue_t *state, unsigned char c);


/**
 * finish ingestion after input is complete.
 *
 * after the last input byte is ingested, and base64url_encode_getc() has
 * been called the prescribed number of times, call this method to transition
 * the encoder state for the final time, and prepare any return values.
 *
 * does not add padding.
 *
 * on success, returns the number of characters ready for output; call
 * base64url_encode_getc() the specified number of times. encoding is now
 * complete and you may optionally add padding.
 *
 * on failure, returns a negative value.
 */
int base64url_encode_finish(b64ue_t *state);


/**
 * set padding characters in encoder state.
 *
 * after completing the finish/getc steps to complete the encoding, call once
 * to prepare the encoder state to output the correct number of padding
 * characters for the data thus far.
 *
 * on success, returns the number of padding characters ready for output; call
 * base64url_encode_getc() the specified number of times to get them.
 *
 * on failure, returns a negative value.
 */
int base64url_encode_pad(b64ue_t *state);


/**
 * prepare decoder state.
 * use before ingesting any characters.
 * a decoder state object may be reset at any time, either to abandon a
 * current decoding process or to begin a new one or both.
 */
void base64url_decode_reset(b64ud_t *state);


/**
 * retrieve the next decoded character available.
 * cannot distinguish between success and failure.
 * call this method as many times as base64url_decode_ingest() tells you, and do
 * so before calling base64url_decode_ingest() again.
 */
int base64url_decode_getc(b64ud_t *state);


/**
 * update the decoder state with the next raw input character.
 *
 * on success, returns the number of characters ready for output; call
 * base64url_decode_getc() the specified number of times before (optionally)
 * calling this method again.
 *
 * on failure, returns a negative value.
 */
int base64url_decode_ingest(b64ud_t *state, unsigned char c);

#endif
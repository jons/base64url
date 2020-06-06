base64url
=========

<p>RFC 4648 compliant, re-entrant, base64url codec, written in ANSI C. includes streaming encoder/decoder tool. includes functions for encoding/decoding static buffers.</p>

      $ ./configure
      $ make
      $ make check
      $ echo -n foobar | ./codec/rb64ue ; echo
      $ echo -n Zm9vYmFy | ./codec/rb64ud ; echo


SYNOPSIS
--------

```c
  #include <rb64u.h>
    
  int base64url_encode (char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);

  int base64url_encode_padded(char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);

  int base64url_decode (char *dest, const size_t maxlen, const char *src, const size_t len, size_t *dlen);
    
  void base64url_encode_reset  (b64ue_t *state);
  int  base64url_encode_getc   (b64ue_t *state);
  int  base64url_encode_ingest (b64ue_t *state, unsigned char c);
  int  base64url_encode_finish (b64ue_t *state);
  int  base64url_encode_pad    (b64ue_t *state);
    
  void base64url_decode_reset  (b64ud_t *state);
  int  base64url_decode_getc   (b64ud_t *state);
  int  base64url_decode_ingest (b64ud_t *state, unsigned char c);
```

DESCRIPTION
-----------

**base64url_encode()** **base64url_encode_pad()** and **base64url_decode()** are
convenience functions for operating on static buffers where streaming input is
not needed.

these functions read up to _len_ bytes from an input buffer _src_ and write no
more than _maxlen_ bytes to an output buffer _dest_.

if **NULL** is specified for the parameter _dlen_ then it is ignored, otherwise,
the number of bytes written to _dest_ will be stored in the given location just
before returning, regardless of success or failure.

the re-entrant encoder functions are used in three phases -- initialization,
the read/write loop, and finalization -- with an optional fourth phase for
padding the output hash.

the decoder functions require only initialization followed by a read/write loop
which will handle padding if it is found.

a state vector can only encode/decode a single stream or buffer.


EXAMPLES
--------

initialization of a state vector is achieved by calling the reset function.

an encoder or decoder state vector can be initialized at any time, but doing so
will discard all previous operations performed on it, abandoning processing.

```c
  b64ue_t se;
  b64ud_t sd;
  base64url_encode_reset(&se);
  base64url_decode_reset(&sd);
```

the read/write loop shall call the ingest function once per incoming character,
and it should terminate only when there are no more characters, or if the ingest
function returns a negative value.

if the ingest function returns a negative value then the state vector becomes
undefined and no more processing should be done.

if the ingest function returns zero, there is nothing in the output buffer to
retrieve, but processing may continue.

if the ingest function returns a positive value, then there is at least one
character in the output buffer, and the getc function must be called to acquire
all of them before calling ingest again.

an example read/write loop for stdin/stdout:

```c
  while (EOF != (c = getchar())) {
    r = base64url_encode_ingest(&s, c);
    if (r < 0) return -1;
    while (r > 0) printf("%c", base64url_encode_getc(&s));
  }
```

while encoding, the encoder state vector can be finalized at any time by calling
the finish function, but doing so necessitates ending the read/write loop for
that vector and not returning to it until re-initialized. when no more input is
available to ingest and after all output has been acquired with getc, and no
other errors occurred, finalization should be performed.

simply call the finish function and treat the result the same as ingest:

```c
  r = base64url_encode_finish(&s);
  if (r < 0) return -1;
  while (r > 0) printf("%c", base64url_encode_getc(&s));
```

after finalization, optionally, add the correct number of padding characters:

```c
  r = base64url_encode_pad(&s);
  if (r < 0) return -1;
  while (r > 0) printf("%c", base64url_encode_getc(&s));
```


RETURN VALUES
-------------

all functions return a negative value if an error occurred.

all non-negative return values indicate success.

on error, the convenience functions have completed some of the work before
terminating.

on error, the re-entrant functions cannot continue processing with their
state vector; the state vector must be reset or discarded. failure to stop
processing can result in an infinite loop.

**base64url_encode_ingest()** **base64url_encode_finish()** and **base64url_encode_pad()**
return a positive value if **base64url_encode_getc()** must be called to avoid
data loss; the value specifies the number of calls required. additional calls
produce NULLs (probably not desired).

likewise, **base64url_decode_ingest()** returns a positive value if **base64url_decode_getc()**
must be called. the conditions are the same.

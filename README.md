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

**`#include <rb64u.h>`**

<code>**int base64url\_encode(char \***_dest_**, const size\_t** _maxlen_**, const char \***_src_**, const size\_t** _len_**, size\_t \***_dlen_**);**</code>

<code>**int base64url\_decode(char \***_dest_**, const size\_t** _maxlen_**, const char \***_src_**, const size\_t** _len_**, size\_t \***_dlen_**);**</code>
<br />

<code>**void base64url\_encode\_reset(b64ue\_t \***_state_**);**</code>

<code>**int base64url\_encode\_ingest(b64ue\_t \***_state_**, char** _c_**);**</code>

<code>**int base64url\_encode\_getc(b64ue\_t \***_state_**);**</code>

<code>**int base64url\_encode\_finish(b64ue\_t \***_state_**);**</code>
<br />

<code>**void base64url\_decode\_reset(b64ud\_t \***_state_**);**</code>

<code>**int base64url\_decode\_ingest(b64ud\_t \***_state_**, unsigned char** _c_**);**</code>

<code>**int base64url\_decode\_getc(b64ud\_t \***_state_**);**</code>

<code>**int base64url\_decode\_finish(b64ud\_t \***_state_**);**</code>


DESCRIPTION
-----------

<code>**`base64url_encode()`** and **`base64url_decode()`** are convenience functions, provided for operating on static buffers, where streaming functionality is not needed.</code>

<code>both functions read up to _`len`_ bytes from an input buffer _`src`_ and write no more than _`maxlen`_ bytes to an output buffer _`dest`_.</code>

<code>if **NULL** is specified for the parameter _`dlen`_ then it is ignored, otherwise, the number of bytes written to _`dest`_ before terminating (either in success or in error) will be stored in the given location.</code>

<code>use the re-entrant functions entails of three steps or phases: initialization, the read/write loop, and finalization. a state vector can be initialized at any time, but by doing so any previous operations on it are lost; a vector can only encode/decode a single stream or buffer. a state vector can be finalized at any time, but doing so necessitates ending the read/write loop for that vector and not returning to it until re-initialized. the read-write loop comes between the two and can be run indefinitely provided that incoming characters (ingest) are matched by outgoing characters (getc) in the appropriate count, by honoring the return codes.</code>

<code>initialization is achieved by calling the reset function.</code>

        int c, r;
        b64ue_t s;
        base64url_encode_reset(&s);

<code>the read/write loop shall call the ingest function once per incoming character, and it should terminate only when there are no more characters, or if the ingest function returns a negative value. if the ingest function returns a negative value then the state vector is undefined and finalization should be skipped. for each pass of the read/write loop, if the ingest function returns a positive value, then there is a character in the buffer that the getc function can acquire. if the getc function is not called, the character will be lost, but no other side effects are incurred.</code>

        while (EOF != (c = getchar())) {
            r = base64url_encode_ingest(&s, c);
            if (r < 0) return -1;
            if (r > 0) printf("%c", base64url_encode_getc(&s));
        }

<code>when no more input is available to ingest and after all output has been acquired with getc, and no other errors occurred, finalization should be performed. this is achieved with a loop that calls the finish function until it returns zero, or a negative value (indicating error). for each positive value the finish function returns, getc should be called once to acquire the character waiting in the buffer. if it is not, the character will be lost, but no other side effects are incurred.</code>

        for (;;) {
            r = base64url_encode_finish(&s);
            if (r < 0) return -1;
            if (r == 0) break;
            printf("%c", base64url_encode_getc(&s));
        }

<code>the example code provided is for stream-oriented operations using _`stdin`_ and _`stdout`_. for buffer-oriented operations, use the convenience functions.</code>

RETURN VALUE
------------

<code>all functions return a negative value if an error occurred.</code>

<code>on error, the convenience functions may have completed some of the work before terminating.</code>

<code>on error, the re-entrant functions cannot continue on the object state vector; the state vector must be reset or discarded.</code>

<code>**`base64url_encode_ingest()`** and **`base64url_encode_finish()`** return a positive value if **`base64url_encode_getc()`** must be called to avoid data loss. likewise **`base64url_decode_ingest()`** and **`base64url_decode_finish()`** return a positive value if **`base64url_decode_getc()`** must be called to avoid data loss.</code>

<code>**`base64url_encode_ingest()`** will return zero if there is no data available yet and the read/write loop should continue. if the loop terminates before the input stream is complete but after ingest last returned zero, calling the finish function picks up where ingest left off, so that output characters coalesce normally. likewise for the decoder operations.</code>

<code>**`base64url_encode_finish()`** will return zero when the encoder finishes successfully, likewise for the decoder.</code>

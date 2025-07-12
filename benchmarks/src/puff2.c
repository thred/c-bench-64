/*
 * puff.c
 * Copyright (C) 2002-2013 Mark Adler
 * For conditions of distribution and use, see copyright notice in puff.h
 * version 2.3, 21 Jan 2013
 *
 * puff.c is a simple inflate written to be an unambiguous way to specify the
 * deflate format.  It is not written for speed but rather simplicity.  As a
 * side benefit, this code might actually be useful when small code is more
 * important than speed, such as bootstrap applications.  For typical deflate
 * data, zlib's inflate() is about four times as fast as puff().  zlib's
 * inflate compiles to around 20K on my machine, whereas puff.c compiles to
 * around 4K on my machine (a PowerPC using GNU cc).  If the faster decode()
 * function here is used, then puff() is only twice as slow as zlib's
 * inflate().
 *
 * All dynamically allocated memory comes from the stack.  The stack required
 * is less than 2K bytes.  This code is compatible with 16-bit int's and
 * assumes that long's are at least 32 bits.  puff.c uses the short data type,
 * assumed to be 16 bits, for arrays in order to conserve memory.  The code
 * works whether integers are stored big endian or little endian.
 *
 * In the comments below are "Format notes" that describe the inflate process
 * and document some of the less obvious aspects of the format.  This source
 * code is meant to supplement RFC 1951, which formally describes the deflate
 * format:
 *
 *    http://www.zlib.org/rfc-deflate.html
 */

/*
 * Change history:
 *
 * 1.0  10 Feb 2002     - First version
 * 1.1  17 Feb 2002     - Clarifications of some comments and notes
 *                      - Update puff() dest and source pointers on negative
 *                        errors to facilitate debugging deflators
 *                      - Remove longest from struct huffman -- not needed
 *                      - Simplify offs[] index in construct()
 *                      - Add input size and checking, using longjmp() to
 *                        maintain easy readability
 *                      - Use short data type for large arrays
 *                      - Use pointers instead of long to specify source and
 *                        destination sizes to avoid arbitrary 4 GB limits
 * 1.2  17 Mar 2002     - Add faster version of decode(), doubles speed (!),
 *                        but leave simple version for readabilty
 *                      - Make sure invalid distances detected if pointers
 *                        are 16 bits
 *                      - Fix fixed codes table error
 *                      - Provide a scanning mode for determining size of
 *                        uncompressed data
 * 1.3  20 Mar 2002     - Go back to lengths for puff() parameters [Gailly]
 *                      - Add a puff.h file for the interface
 *                      - Add braces in puff() for else do [Gailly]
 *                      - Use indexes instead of pointers for readability
 * 1.4  31 Mar 2002     - Simplify construct() code set check
 *                      - Fix some comments
 *                      - Add FIXLCODES #define
 * 1.5   6 Apr 2002     - Minor comment fixes
 * 1.6   7 Aug 2002     - Minor format changes
 * 1.7   3 Mar 2003     - Added test code for distribution
 *                      - Added zlib-like license
 * 1.8   9 Jan 2004     - Added some comments on no distance codes case
 * 1.9  21 Feb 2008     - Fix bug on 16-bit integer architectures [Pohland]
 *                      - Catch missing end-of-block symbol error
 * 2.0  25 Jul 2008     - Add #define to permit distance too far back
 *                      - Add option in TEST code for puff to write the data
 *                      - Add option in TEST code to skip input bytes
 *                      - Allow TEST code to read from piped stdin
 * 2.1   4 Apr 2010     - Avoid variable initialization for happier compilers
 *                      - Avoid unsigned comparisons for even happier compilers
 * 2.2  25 Apr 2010     - Fix bug in variable initializations [Oberhumer]
 *                      - Add const where appropriate [Oberhumer]
 *                      - Split if's and ?'s for coverage testing
 *                      - Break out test code to separate file
 *                      - Move NIL to puff.h
 *                      - Allow incomplete code only if single code length is 1
 *                      - Add full code coverage test to Makefile
 * 2.3  21 Jan 2013     - Check for invalid code length codes in dynamic blocks
 */

// #include <setjmp.h>             /* for setjmp(), longjmp(), and jmp_buf */
// #include "puff.h"               /* prototype for puff() */
// #include <stdio.h>
// #include <stdlib.h>
#include "out.h"

#define local static /* for local function definitions */
#define NIL 0

void stop() {}
#define SLOW

/*
 * Maximums for allocations and loops.  It is not useful to change these --
 * they are fixed by the deflate format.
 */
#define MAXBITS 15                       /* maximum bits in a code */
#define MAXLCODES 286                    /* maximum number of literal/length codes */
#define MAXDCODES 30                     /* maximum number of distance codes */
#define MAXCODES (MAXLCODES + MAXDCODES) /* maximum codes lengths to read */
#define FIXLCODES 288                    /* number of fixed literal/length codes */

/* input and output state */
struct state
{
    /* output state */
    unsigned char *out;   /* output buffer */
    unsigned long outlen; /* available space at out */
    unsigned long outcnt; /* bytes written to out so far */

    /* input state */
    const unsigned char *in; /* input buffer */
    unsigned long inlen;     /* available input at in */
    unsigned long incnt;     /* bytes read so far */
    int bitbuf;              /* bit buffer */
    int bitcnt;              /* number of bits in bit buffer */

    /* input limit error return state for bits() and decode() */
    //    jmp_buf env;
};

/*
 * Return need bits from the input stream.  This always leaves less than
 * eight bits in the buffer.  bits() works properly for need == 0.
 *
 * Format notes:
 *
 * - Bits are stored in bytes from the least significant bit to the most
 *   significant bit.  Therefore bits are dropped from the bottom of the bit
 *   buffer, using shift right, and new bytes are appended to the top of the
 *   bit buffer, using shift left.
 */
local int bits(struct state *s, int need)
{
    long val; /* bit accumulator (can use up to 20 bits) */

    /* load at least need bits into val */
    val = s->bitbuf;
    while (s->bitcnt < need)
    {
        if (s->incnt == s->inlen)
            stop();
        //            longjmp(s->env, 1);         /* out of input */
        val |= (long)(s->in[s->incnt++]) << s->bitcnt; /* load eight bits */
        s->bitcnt += 8;
    }

    /* drop need bits and update buffer, always zero to seven bits left */
    s->bitbuf = (int)(val >> need);
    s->bitcnt -= need;

    /* return need bits, zeroing the bits above that */
    return (int)(val & ((1L << need) - 1));
}

/*
 * Process a stored block.
 *
 * Format notes:
 *
 * - After the two-bit stored block type (00), the stored block length and
 *   stored bytes are byte-aligned for fast copying.  Therefore any leftover
 *   bits in the byte that has the last bit of the type, as many as seven, are
 *   discarded.  The value of the discarded bits are not defined and should not
 *   be checked against any expectation.
 *
 * - The second inverted copy of the stored block length does not have to be
 *   checked, but it's probably a good idea to do so anyway.
 *
 * - A stored block can have zero length.  This is sometimes used to byte-align
 *   subsets of the compressed data for random access or partial recovery.
 */
local int stored(struct state *s)
{
    unsigned len; /* length of stored block */

    /* discard leftover bits from current byte (assumes s->bitcnt < 8) */
    s->bitbuf = 0;
    s->bitcnt = 0;

    /* get length and check against its one's complement */
    if (s->incnt + 4 > s->inlen)
        return 2; /* not enough input */
    len = s->in[s->incnt++];
    len |= s->in[s->incnt++] << 8;
    if (s->in[s->incnt++] != (~len & 0xff) ||
        s->in[s->incnt++] != ((~len >> 8) & 0xff))
        return -2; /* didn't match complement! */

    /* copy len bytes from in to out */
    if (s->incnt + len > s->inlen)
        return 2; /* not enough input */
    if (s->out != NIL)
    {
        if (s->outcnt + len > s->outlen)
            return 1; /* not enough output space */
        while (len--)
            s->out[s->outcnt++] = s->in[s->incnt++];
    }
    else
    { /* just scanning */
        s->outcnt += len;
        s->incnt += len;
    }

    /* done with a valid stored block */
    return 0;
}

/*
 * Huffman code decoding tables.  count[1..MAXBITS] is the number of symbols of
 * each length, which for a canonical code are stepped through in order.
 * symbol[] are the symbol values in canonical order, where the number of
 * entries is the sum of the counts in count[].  The decoding process can be
 * seen in the function decode() below.
 */
struct huffman
{
    short *count;  /* number of symbols of each length */
    short *symbol; /* canonically ordered symbols */
};

/*
 * Decode a code from the stream s using huffman table h.  Return the symbol or
 * a negative value if there is an error.  If all of the lengths are zero, i.e.
 * an empty code, or if the code is incomplete and an invalid code is received,
 * then -10 is returned after reading MAXBITS bits.
 *
 * Format notes:
 *
 * - The codes as stored in the compressed data are bit-reversed relative to
 *   a simple integer ordering of codes of the same lengths.  Hence below the
 *   bits are pulled from the compressed data one at a time and used to
 *   build the code value reversed from what is in the stream in order to
 *   permit simple integer comparisons for decoding.  A table-based decoding
 *   scheme (as used in zlib) does not need to do this reversal.
 *
 * - The first code for the shortest length is all zeros.  Subsequent codes of
 *   the same length are simply integer increments of the previous code.  When
 *   moving up a length, a zero bit is appended to the code.  For a complete
 *   code, the last code of the longest length will be all ones.
 *
 * - Incomplete codes are handled by this decoder, since they are permitted
 *   in the deflate format.  See the format notes for fixed() and dynamic().
 */
#ifdef SLOW
local int decode(struct state *s, const struct huffman *h)
{
    int len;   /* current number of bits in code */
    int code;  /* len bits being decoded */
    int first; /* first code of length len */
    int count; /* number of codes of length len */
    int index; /* index of first code of length len in symbol table */

    code = first = index = 0;
    for (len = 1; len <= MAXBITS; len++)
    {
        code |= bits(s, 1); /* get next bit */
        count = h->count[len];
        if (code - count < first) /* if length len, return symbol */
            return h->symbol[index + (code - first)];
        index += count; /* else update for next length */
        first += count;
        first <<= 1;
        code <<= 1;
    }
    return -10; /* ran out of codes */
}

/*
 * A faster version of decode() for real applications of this code.   It's not
 * as readable, but it makes puff() twice as fast.  And it only makes the code
 * a few percent larger.
 */
#else  /* !SLOW */
local int decode(struct state *s, const struct huffman *h)
{
    int len;     /* current number of bits in code */
    int code;    /* len bits being decoded */
    int first;   /* first code of length len */
    int count;   /* number of codes of length len */
    int index;   /* index of first code of length len in symbol table */
    int bitbuf;  /* bits from stream */
    int left;    /* bits left in next or left to process */
    short *next; /* next number of codes */

    bitbuf = s->bitbuf;
    left = s->bitcnt;
    code = first = index = 0;
    len = 1;
    next = h->count + 1;
    while (1)
    {
        while (left--)
        {
            code |= bitbuf & 1;
            bitbuf >>= 1;
            count = *next++;
            if (code - count < first)
            { /* if length len, return symbol */
                s->bitbuf = bitbuf;
                s->bitcnt = (s->bitcnt - len) & 7;
                return h->symbol[index + (code - first)];
            }
            index += count; /* else update for next length */
            first += count;
            first <<= 1;
            code <<= 1;
            len++;
        }
        left = (MAXBITS + 1) - len;
        if (left == 0)
            break;
        if (s->incnt == s->inlen)
            stop();
        //    longjmp(s->env, 1);         /* out of input */
        bitbuf = s->in[s->incnt++];
        if (left > 8)
            left = 8;
    }
    return -10; /* ran out of codes */
}
#endif /* SLOW */

/*
 * Given the list of code lengths length[0..n-1] representing a canonical
 * Huffman code for n symbols, construct the tables required to decode those
 * codes.  Those tables are the number of codes of each length, and the symbols
 * sorted by length, retaining their original order within each length.  The
 * return value is zero for a complete code set, negative for an over-
 * subscribed code set, and positive for an incomplete code set.  The tables
 * can be used if the return value is zero or positive, but they cannot be used
 * if the return value is negative.  If the return value is zero, it is not
 * possible for decode() using that table to return an error--any stream of
 * enough bits will resolve to a symbol.  If the return value is positive, then
 * it is possible for decode() using that table to return an error for received
 * codes past the end of the incomplete lengths.
 *
 * Not used by decode(), but used for error checking, h->count[0] is the number
 * of the n symbols not in the code.  So n - h->count[0] is the number of
 * codes.  This is useful for checking for incomplete codes that have more than
 * one symbol, which is an error in a dynamic block.
 *
 * Assumption: for all i in 0..n-1, 0 <= length[i] <= MAXBITS
 * This is assured by the construction of the length arrays in dynamic() and
 * fixed() and is not verified by construct().
 *
 * Format notes:
 *
 * - Permitted and expected examples of incomplete codes are one of the fixed
 *   codes and any code with a single symbol which in deflate is coded as one
 *   bit instead of zero bits.  See the format notes for fixed() and dynamic().
 *
 * - Within a given code length, the symbols are kept in ascending order for
 *   the code bits definition.
 */
local int construct(struct huffman *h, const short *length, int n)
{
    int symbol;              /* current symbol when stepping through length[] */
    int len;                 /* current length when stepping through h->count[] */
    int left;                /* number of possible codes left of current length */
    short offs[MAXBITS + 1]; /* offsets in symbol table for each length */

    /* count number of codes of each length */
    for (len = 0; len <= MAXBITS; len++)
        h->count[len] = 0;
    for (symbol = 0; symbol < n; symbol++)
        (h->count[length[symbol]])++; /* assumes lengths are within bounds */
    if (h->count[0] == n)             /* no codes! */
        return 0;                     /* complete, but decode() will fail */

    /* check for an over-subscribed or incomplete set of lengths */
    left = 1; /* one possible code of zero length */
    for (len = 1; len <= MAXBITS; len++)
    {
        left <<= 1;            /* one more bit, double codes left */
        left -= h->count[len]; /* deduct count from possible codes */
        if (left < 0)
            return left; /* over-subscribed--return negative */
    } /* left > 0 means incomplete */

    /* generate offsets into symbol table for each length for sorting */
    offs[1] = 0;
    for (len = 1; len < MAXBITS; len++)
        offs[len + 1] = offs[len] + h->count[len];

    /*
     * put symbols in table sorted by length, by symbol order within each
     * length
     */
    for (symbol = 0; symbol < n; symbol++)
        if (length[symbol] != 0)
            h->symbol[offs[length[symbol]]++] = symbol;

    /* return zero for complete set, positive for incomplete set */
    return left;
}

/*
 * Decode literal/length and distance codes until an end-of-block code.
 *
 * Format notes:
 *
 * - Compressed data that is after the block type if fixed or after the code
 *   description if dynamic is a combination of literals and length/distance
 *   pairs terminated by and end-of-block code.  Literals are simply Huffman
 *   coded bytes.  A length/distance pair is a coded length followed by a
 *   coded distance to represent a string that occurs earlier in the
 *   uncompressed data that occurs again at the current location.
 *
 * - Literals, lengths, and the end-of-block code are combined into a single
 *   code of up to 286 symbols.  They are 256 literals (0..255), 29 length
 *   symbols (257..285), and the end-of-block symbol (256).
 *
 * - There are 256 possible lengths (3..258), and so 29 symbols are not enough
 *   to represent all of those.  Lengths 3..10 and 258 are in fact represented
 *   by just a length symbol.  Lengths 11..257 are represented as a symbol and
 *   some number of extra bits that are added as an integer to the base length
 *   of the length symbol.  The number of extra bits is determined by the base
 *   length symbol.  These are in the static arrays below, lens[] for the base
 *   lengths and lext[] for the corresponding number of extra bits.
 *
 * - The reason that 258 gets its own symbol is that the longest length is used
 *   often in highly redundant files.  Note that 258 can also be coded as the
 *   base value 227 plus the maximum extra value of 31.  While a good deflate
 *   should never do this, it is not an error, and should be decoded properly.
 *
 * - If a length is decoded, including its extra bits if any, then it is
 *   followed a distance code.  There are up to 30 distance symbols.  Again
 *   there are many more possible distances (1..32768), so extra bits are added
 *   to a base value represented by the symbol.  The distances 1..4 get their
 *   own symbol, but the rest require extra bits.  The base distances and
 *   corresponding number of extra bits are below in the static arrays dist[]
 *   and dext[].
 *
 * - Literal bytes are simply written to the output.  A length/distance pair is
 *   an instruction to copy previously uncompressed bytes to the output.  The
 *   copy is from distance bytes back in the output stream, copying for length
 *   bytes.
 *
 * - Distances pointing before the beginning of the output data are not
 *   permitted.
 *
 * - Overlapped copies, where the length is greater than the distance, are
 *   allowed and common.  For example, a distance of one and a length of 258
 *   simply copies the last byte 258 times.  A distance of four and a length of
 *   twelve copies the last four bytes three times.  A simple forward copy
 *   ignoring whether the length is greater than the distance or not implements
 *   this correctly.  You should not use memcpy() since its behavior is not
 *   defined for overlapped arrays.  You should not use memmove() or bcopy()
 *   since though their behavior -is- defined for overlapping arrays, it is
 *   defined to do the wrong thing in this case.
 */

static const short lens[29] = {/* Size base for length codes 257..285 */
                               3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                               35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
static const short lext[29] = {/* Extra bits for length codes 257..285 */
                               0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                               3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
static const short dists[30] = {/* Offset base for distance codes 0..29 */
                                1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
                                257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
                                8193, 12289, 16385, 24577};
static const short dext[30] = {/* Extra bits for distance codes 0..29 */
                               0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                               7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
                               12, 12, 13, 13};

local int codes(struct state *s,
                const struct huffman *lencode,
                const struct huffman *distcode)
{
    int symbol;    /* decoded symbol */
    int len;       /* length for copy */
    unsigned dist; /* distance for copy */

    /* decode literals and length/distance pairs */
    do
    {
        symbol = decode(s, lencode);
        if (symbol < 0)
            return symbol; /* invalid symbol */
        if (symbol < 256)
        { /* literal: symbol is the byte */
            /* write out the literal */
            if (s->out != NIL)
            {
                if (s->outcnt == s->outlen)
                    return 1;
                s->out[s->outcnt] = symbol;
            }
            s->outcnt++;
        }
        else if (symbol > 256)
        { /* length */
            /* get and compute length */
            symbol -= 257;
            if (symbol >= 29)
                return -10; /* invalid fixed code */
            len = lens[symbol] + bits(s, lext[symbol]);

            /* get and check distance */
            symbol = decode(s, distcode);
            if (symbol < 0)
                return symbol; /* invalid symbol */
            dist = dists[symbol] + bits(s, dext[symbol]);
#ifndef INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR
            if (dist > s->outcnt)
                return -11; /* distance too far back */
#endif

            /* copy length bytes from distance bytes back */
            if (s->out != NIL)
            {
                if (s->outcnt + len > s->outlen)
                    return 1;
                while (len--)
                {
                    s->out[s->outcnt] =
#ifdef INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR
                        dist > s->outcnt ? 0 :
#endif
                                         s->out[s->outcnt - dist];
                    s->outcnt++;
                }
            }
            else
                s->outcnt += len;
        }
    } while (symbol != 256); /* end of block symbol */

    /* done with a valid fixed or dynamic block */
    return 0;
}

/*
 * Process a fixed codes block.
 *
 * Format notes:
 *
 * - This block type can be useful for compressing small amounts of data for
 *   which the size of the code descriptions in a dynamic block exceeds the
 *   benefit of custom codes for that block.  For fixed codes, no bits are
 *   spent on code descriptions.  Instead the code lengths for literal/length
 *   codes and distance codes are fixed.  The specific lengths for each symbol
 *   can be seen in the "for" loops below.
 *
 * - The literal/length code is complete, but has two symbols that are invalid
 *   and should result in an error if received.  This cannot be implemented
 *   simply as an incomplete code since those two symbols are in the "middle"
 *   of the code.  They are eight bits long and the longest literal/length\
 *   code is nine bits.  Therefore the code must be constructed with those
 *   symbols, and the invalid symbols must be detected after decoding.
 *
 * - The fixed distance codes also have two invalid symbols that should result
 *   in an error if received.  Since all of the distance codes are the same
 *   length, this can be implemented as an incomplete code.  Then the invalid
 *   codes are detected while decoding.
 */

static short lencnt[MAXBITS + 1], lensym[FIXLCODES];
static short distcnt[MAXBITS + 1], distsym[MAXDCODES];
static short lengths[FIXLCODES];

local int fixed(struct state *s)
{
    static int virgin = 1;
    static struct huffman lencode, distcode;

    /* build fixed huffman tables if first call (may not be thread safe) */
    if (virgin)
    {
        int symbol;

        /* construct lencode and distcode */
        lencode.count = lencnt;
        lencode.symbol = lensym;
        distcode.count = distcnt;
        distcode.symbol = distsym;

        /* literal/length table */
        for (symbol = 0; symbol < 144; symbol++)
            lengths[symbol] = 8;
        for (; symbol < 256; symbol++)
            lengths[symbol] = 9;
        for (; symbol < 280; symbol++)
            lengths[symbol] = 7;
        for (; symbol < FIXLCODES; symbol++)
            lengths[symbol] = 8;
        construct(&lencode, lengths, FIXLCODES);

        /* distance table */
        for (symbol = 0; symbol < MAXDCODES; symbol++)
            lengths[symbol] = 5;
        construct(&distcode, lengths, MAXDCODES);

        /* do this just once */
        virgin = 0;
    }

    /* decode data until end-of-block code */
    return codes(s, &lencode, &distcode);
}

/*
 * Process a dynamic codes block.
 *
 * Format notes:
 *
 * - A dynamic block starts with a description of the literal/length and
 *   distance codes for that block.  New dynamic blocks allow the compressor to
 *   rapidly adapt to changing data with new codes optimized for that data.
 *
 * - The codes used by the deflate format are "canonical", which means that
 *   the actual bits of the codes are generated in an unambiguous way simply
 *   from the number of bits in each code.  Therefore the code descriptions
 *   are simply a list of code lengths for each symbol.
 *
 * - The code lengths are stored in order for the symbols, so lengths are
 *   provided for each of the literal/length symbols, and for each of the
 *   distance symbols.
 *
 * - If a symbol is not used in the block, this is represented by a zero as
 *   as the code length.  This does not mean a zero-length code, but rather
 *   that no code should be created for this symbol.  There is no way in the
 *   deflate format to represent a zero-length code.
 *
 * - The maximum number of bits in a code is 15, so the possible lengths for
 *   any code are 1..15.
 *
 * - The fact that a length of zero is not permitted for a code has an
 *   interesting consequence.  Normally if only one symbol is used for a given
 *   code, then in fact that code could be represented with zero bits.  However
 *   in deflate, that code has to be at least one bit.  So for example, if
 *   only a single distance base symbol appears in a block, then it will be
 *   represented by a single code of length one, in particular one 0 bit.  This
 *   is an incomplete code, since if a 1 bit is received, it has no meaning,
 *   and should result in an error.  So incomplete distance codes of one symbol
 *   should be permitted, and the receipt of invalid codes should be handled.
 *
 * - It is also possible to have a single literal/length code, but that code
 *   must be the end-of-block code, since every dynamic block has one.  This
 *   is not the most efficient way to create an empty block (an empty fixed
 *   block is fewer bits), but it is allowed by the format.  So incomplete
 *   literal/length codes of one symbol should also be permitted.
 *
 * - If there are only literal codes and no lengths, then there are no distance
 *   codes.  This is represented by one distance code with zero bits.
 *
 * - The list of up to 286 length/literal lengths and up to 30 distance lengths
 *   are themselves compressed using Huffman codes and run-length encoding.  In
 *   the list of code lengths, a 0 symbol means no code, a 1..15 symbol means
 *   that length, and the symbols 16, 17, and 18 are run-length instructions.
 *   Each of 16, 17, and 18 are follwed by extra bits to define the length of
 *   the run.  16 copies the last length 3 to 6 times.  17 represents 3 to 10
 *   zero lengths, and 18 represents 11 to 138 zero lengths.  Unused symbols
 *   are common, hence the special coding for zero lengths.
 *
 * - The symbols for 0..18 are Huffman coded, and so that code must be
 *   described first.  This is simply a sequence of up to 19 three-bit values
 *   representing no code (0) or the code length for that symbol (1..7).
 *
 * - A dynamic block starts with three fixed-size counts from which is computed
 *   the number of literal/length code lengths, the number of distance code
 *   lengths, and the number of code length code lengths (ok, you come up with
 *   a better name!) in the code descriptions.  For the literal/length and
 *   distance codes, lengths after those provided are considered zero, i.e. no
 *   code.  The code length code lengths are received in a permuted order (see
 *   the order[] array below) to make a short code length code length list more
 *   likely.  As it turns out, very short and very long codes are less likely
 *   to be seen in a dynamic code description, hence what may appear initially
 *   to be a peculiar ordering.
 *
 * - Given the number of literal/length code lengths (nlen) and distance code
 *   lengths (ndist), then they are treated as one long list of nlen + ndist
 *   code lengths.  Therefore run-length coding can and often does cross the
 *   boundary between the two sets of lengths.
 *
 * - So to summarize, the code description at the start of a dynamic block is
 *   three counts for the number of code lengths for the literal/length codes,
 *   the distance codes, and the code length codes.  This is followed by the
 *   code length code lengths, three bits each.  This is used to construct the
 *   code length code which is used to read the remainder of the lengths.  Then
 *   the literal/length code lengths and distance lengths are read as a single
 *   set of lengths using the code length codes.  Codes are constructed from
 *   the resulting two sets of lengths, and then finally you can start
 *   decoding actual compressed data in the block.
 *
 * - For reference, a "typical" size for the code description in a dynamic
 *   block is around 80 bytes.
 */

local int dynamic(struct state *s)
{
    int nlen, ndist, ncode;           /* number of lengths in descriptor */
    int index;                        /* index of lengths[] */
    int err;                          /* construct() return value */
    struct huffman lencode, distcode; /* length and distance codes */
    static const short order[19] =    /* permutation of code length codes */
        {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

    /* construct lencode and distcode */
    lencode.count = lencnt;
    lencode.symbol = lensym;
    distcode.count = distcnt;
    distcode.symbol = distsym;

    /* get number of lengths in each table, check lengths */
    nlen = bits(s, 5) + 257;
    ndist = bits(s, 5) + 1;
    ncode = bits(s, 4) + 4;
    if (nlen > MAXLCODES || ndist > MAXDCODES)
        return -3; /* bad counts */

    /* read code length code lengths (really), missing lengths are zero */
    for (index = 0; index < ncode; index++)
        lengths[order[index]] = bits(s, 3);
    for (; index < 19; index++)
        lengths[order[index]] = 0;

    /* build huffman table for code lengths codes (use lencode temporarily) */
    err = construct(&lencode, lengths, 19);
    if (err != 0) /* require complete code set here */
        return -4;

    /* read length/literal and distance code length tables */
    index = 0;
    while (index < nlen + ndist)
    {
        int symbol; /* decoded value */
        int len;    /* last length to repeat */

        symbol = decode(s, &lencode);
        if (symbol < 0)
            return symbol; /* invalid symbol */
        if (symbol < 16)   /* length in 0..15 */
            lengths[index++] = symbol;
        else
        {            /* repeat instruction */
            len = 0; /* assume repeating zeros */
            if (symbol == 16)
            { /* repeat last length 3..6 times */
                if (index == 0)
                    return -5;            /* no last length! */
                len = lengths[index - 1]; /* last length */
                symbol = 3 + bits(s, 2);
            }
            else if (symbol == 17) /* repeat zero 3..10 times */
                symbol = 3 + bits(s, 3);
            else /* == 18, repeat zero 11..138 times */
                symbol = 11 + bits(s, 7);
            if (index + symbol > nlen + ndist)
                return -6;   /* too many lengths! */
            while (symbol--) /* repeat last or zero symbol times */
                lengths[index++] = len;
        }
    }

    /* check for end-of-block code -- there better be one! */
    if (lengths[256] == 0)
        return -9;

    /* build huffman table for literal/length codes */
    err = construct(&lencode, lengths, nlen);
    if (err && (err < 0 || nlen != lencode.count[0] + lencode.count[1]))
        return -7; /* incomplete code ok only for single length 1 code */

    /* build huffman table for distance codes */
    err = construct(&distcode, lengths + nlen, ndist);
    if (err && (err < 0 || ndist != distcode.count[0] + distcode.count[1]))
        return -8; /* incomplete code ok only for single length 1 code */

    /* decode data until end-of-block code */
    return codes(s, &lencode, &distcode);
}

/*
 * Inflate source to dest.  On return, destlen and sourcelen are updated to the
 * size of the uncompressed data and the size of the deflate data respectively.
 * On success, the return value of puff() is zero.  If there is an error in the
 * source data, i.e. it is not in the deflate format, then a negative value is
 * returned.  If there is not enough input available or there is not enough
 * output space, then a positive error is returned.  In that case, destlen and
 * sourcelen are not updated to facilitate retrying from the beginning with the
 * provision of more input data or more output space.  In the case of invalid
 * inflate data (a negative error), the dest and source pointers are updated to
 * facilitate the debugging of deflators.
 *
 * puff() also has a mode to determine the size of the uncompressed output with
 * no output written.  For this dest must be (unsigned char *)0.  In this case,
 * the input value of *destlen is ignored, and on return *destlen is set to the
 * size of the uncompressed output.
 *
 * The return codes are:
 *
 *   2:  available inflate data did not terminate
 *   1:  output space exhausted before completing inflate
 *   0:  successful inflate
 *  -1:  invalid block type (type == 3)
 *  -2:  stored block length did not match one's complement
 *  -3:  dynamic block code description: too many length or distance codes
 *  -4:  dynamic block code description: code lengths codes incomplete
 *  -5:  dynamic block code description: repeat lengths with no first length
 *  -6:  dynamic block code description: repeat more than specified lengths
 *  -7:  dynamic block code description: invalid literal/length code lengths
 *  -8:  dynamic block code description: invalid distance code lengths
 *  -9:  dynamic block code description: missing end-of-block code
 * -10:  invalid literal/length or distance code in fixed or dynamic block
 * -11:  distance is too far back in fixed or dynamic block
 *
 * Format notes:
 *
 * - Three bits are read for each block to determine the kind of block and
 *   whether or not it is the last block.  Then the block is decoded and the
 *   process repeated if it was not the last block.
 *
 * - The leftover bits in the last byte of the deflate data after the last
 *   block (if it was a fixed or dynamic block) are undefined and have no
 *   expected values to check.
 */
int puff(unsigned char *dest,         /* pointer to destination pointer */
         unsigned long *destlen,      /* amount of output space */
         const unsigned char *source, /* pointer to source data pointer */
         unsigned long *sourcelen)    /* amount of input available */
{
    struct state s; /* input/output state */
    int last, type; /* block information */
    int err;        /* return value */

    /* initialize output state */
    s.out = dest;
    s.outlen = *destlen; /* ignored if dest is NIL */
    s.outcnt = 0;

    /* initialize input state */
    s.in = source;
    s.inlen = *sourcelen;
    s.incnt = 0;
    s.bitbuf = 0;
    s.bitcnt = 0;

    /* return if bits() or decode() tries to read past available input */
    //    if (setjmp(s.env) != 0)             /* if came back here via longjmp() */
    //     err = 2;                        /* then skip do-loop, return error */
    //    else {
    /* process blocks until last block or error */
    do
    {
        last = bits(&s, 1);                                                                      /* one if last block */
        type = bits(&s, 2);                                                                      /* block type 0..3 */
        err = type == 0 ? stored(&s) : (type == 1 ? fixed(&s) : (type == 2 ? dynamic(&s) : -1)); /* type == 3, invalid */
        if (err != 0)
            break; /* return with error */
    } while (!last);
    //    }

    /* update the lengths and return */
    if (err <= 0)
    {
        *destlen = s.outcnt;
        *sourcelen = s.incnt;
    }
    return err;
}

unsigned long CRC32(const unsigned char *data, unsigned int length)
{
    /* CRC-32/CKSUM */
    unsigned char extract;
    unsigned int i;
    unsigned char j;
    unsigned long crc;
    //  const unsigned char *end=data+length;

    crc = 0;

    for (i = 0; i < length; i++)
    {
        //  while(data!=end) {
        extract = *data;
        crc ^= (((unsigned long)extract) << 24);

        for (j = 8; j; j--)
        {
            if ((crc & 0x80000000) != 0)
            {
                //       crc=(crc<<1)^0x04c11db7;
                crc <<= 1;
                crc ^= 0x04c11db7;
            }
            else
                crc <<= 1;
        }
        data++;
    }
    crc ^= 0xffffffff;
    return crc;
}

const unsigned char table[];

unsigned char out[4096];

void benchmark(void)
{
    int r;
    unsigned long slen, dlen;

    slen = 3407;
    dlen = 0x1000;

    r = puff(out,    /* pointer to destination pointer */
             &dlen,  /* amount of output space */
             table,  /* pointer to source data pointer */
             &slen); /* amount of input available */

    print("RES=");
    print_int(r, 0);
    print("\n");

    print("slen=");
    print_int(slen, 0);
    print(" dlen=");
    print_int(dlen, 0);
    print("\n");
}

void benchmark_name(void)
{
    print("puff.c\n");
    print("inflate compressed data\n");
}

#define EXPECTED 0x28ed93c7

unsigned char benchmark_check(void)
{
    unsigned long crc = CRC32(out, 4096);

    print("CRC32=");
    print_hex(crc, 8);

    if (crc == EXPECTED)
    {
        print(" [OK]");
        return 0;
    }

    print(" [FAIL] - expected ");
    print_hex(EXPECTED, 8);
    return 1;
}

/* automatically generated from: kernal4.bin.deflate */

const unsigned char table[3407] = {
    0x7D,
    0x54,
    0x07,
    0x78,
    0x13,
    0x49,
    0xB2,
    0x1E,
    0xF5,
    0x38,
    0xC9,
    0x58,
    0xB6,
    0x08,
    0x07,
    0x32,
    0xB1,
    0x1F,
    0x9B,
    0x6C,
    0x6C,
    0xF6,
    0x74,
    0x9B,
    0xEC,
    0x8B,
    0x64,
    0xAF,
    0xCC,
    0x0D,
    0x0B,
    0x47,
    0xF4,
    0x5E,
    0x1C,
    0xB2,
    0x78,
    0x91,
    0x70,
    0x81,
    0x8C,
    0xF9,
    0xD0,
    0x08,
    0xAF,
    0xB1,
    0x7C,
    0x12,
    0xE1,
    0x48,
    0x0B,
    0x27,
    0x84,
    0x46,
    0xB8,
    0x8D,
    0x65,
    0xE4,
    0xC5,
    0x64,
    0xB4,
    0x19,
    0xA1,
    0x11,
    0xA9,
    0xBD,
    0xB2,
    0xE5,
    0xF0,
    0xCC,
    0xA2,
    0x35,
    0xB6,
    0xB0,
    0x45,
    0x9A,
    0x25,
    0xF3,
    0x36,
    0xCC,
    0xAB,
    0x91,
    0x2F,
    0xA7,
    0xF9,
    0xF4,
    0xB5,
    0xBA,
    0xFF,
    0xAE,
    0xAA,
    0xFE,
    0xFF,
    0xAA,
    0xEA,
    0x16,
    0x66,
    0xE3,
    0xAC,
    0xB3,
    0xE2,
    0x5C,
    0xA9,
    0xCC,
    0xCE,
    0xE2,
    0xC6,
    0xD3,
    0xF8,
    0xE2,
    0x59,
    0x31,
    0xD5,
    0xB0,
    0x64,
    0xA3,
    0x7C,
    0xBF,
    0xB0,
    0x47,
    0x63,
    0x72,
    0x25,
    0x1F,
    0x5B,
    0x52,
    0x3F,
    0x77,
    0xC7,
    0xDC,
    0xED,
    0x4B,
    0x42,
    0xFA,
    0x87,
    0xE2,
    0x6C,
    0x61,
    0x29,
    0x9E,
    0x71,
    0x12,
    0xD7,
    0xBF,
    0x47,
    0x3E,
    0x76,
    0xBE,
    0x87,
    0xDF,
    0x8C,
    0x12,
    0x46,
    0xF8,
    0x1F,
    0x33,
    0x3E,
    0x75,
    0x9A,
    0x17,
    0x96,
    0x59,
    0x96,
    0xE3,
    0xD0,
    0x19,
    0x32,
    0x07,
    0xE7,
    0x9C,
    0xC6,
    0x3F,
    0x8B,
    0x92,
    0x39,
    0x4E,
    0x86,
    0xCB,
    0x39,
    0x9D,
    0x40,
    0xCF,
    0x9F,
    0xA9,
    0x5B,
    0x26,
    0x2C,
    0x76,
    0x2F,
    0x0B,
    0xEE,
    0xA2,
    0x28,
    0xB6,
    0x5C,
    0x58,
    0xE6,
    0x5E,
    0x0E,
    0x36,
    0x22,
    0xFC,
    0x19,
    0x96,
    0x24,
    0xDB,
    0x35,
    0xC1,
    0x84,
    0xCD,
    0xE2,
    0x93,
    0xE4,
    0xA7,
    0x4E,
    0x26,
    0xB0,
    0x98,
    0x76,
    0xF2,
    0xBB,
    0x5E,
    0x9D,
    0xB8,
    0x9A,
    0x31,
    0xE7,
    0xD4,
    0x15,
    0x31,
    0x38,
    0xEF,
    0xAC,
    0xB1,
    0x80,
    0x62,
    0x7C,
    0x5F,
    0xB1,
    0x8E,
    0xB4,
    0x3C,
    0xE3,
    0x4C,
    0xAA,
    0x1B,
    0x29,
    0xCC,
    0x0B,
    0xC2,
    0xB0,
    0xC0,
    0x99,
    0x06,
    0xE3,
    0x7C,
    0x75,
    0xBA,
    0x90,
    0xBB,
    0x1E,
    0x25,
    0x5B,
    0x9C,
    0x0C,
    0x76,
    0x9D,
    0x21,
    0x36,
    0x67,
    0x14,
    0x02,
    0x93,
    0xAD,
    0xCE,
    0x28,
    0x84,
    0xF3,
    0x2C,
    0x14,
    0xE7,
    0x09,
    0x0B,
    0xAD,
    0xF3,
    0x3C,
    0xF3,
    0xC5,
    0x05,
    0xC2,
    0x7C,
    0xEB,
    0x02,
    0x20,
    0xBA,
    0x48,
    0x9C,
    0x2B,
    0x2C,
    0x25,
    0xA5,
    0xC2,
    0x5C,
    0xDC,
    0x7C,
    0xD2,
    0x05,
    0x3E,
    0x5C,
    0xE3,
    0x19,
    0x49,
    0xA6,
    0xA9,
    0x96,
    0x42,
    0x6B,
    0x01,
    0x37,
    0xDF,
    0x53,
    0x4D,
    0x91,
    0x6B,
    0x38,
    0x57,
    0xE0,
    0xC6,
    0x9F,
    0x28,
    0xBE,
    0x6E,
    0x1E,
    0x5F,
    0x51,
    0x7C,
    0x1D,
    0x3C,
    0xF6,
    0x76,
    0xFA,
    0xDA,
    0x79,
    0x1C,
    0x50,
    0x7C,
    0x11,
    0x1E,
    0x77,
    0x2A,
    0x3E,
    0xCA,
    0xE3,
    0x72,
    0x2F,
    0x7E,
    0x7C,
    0x82,
    0x7C,
    0x66,
    0x22,
    0x45,
    0x26,
    0x6F,
    0x16,
    0x6B,
    0xF2,
    0x66,
    0xB0,
    0xB5,
    0x3A,
    0xB6,
    0x26,
    0x93,
    0xCD,
    0xF9,
    0xCF,
    0x01,
    0x4C,
    0x9A,
    0x2D,
    0x83,
    0xAD,
    0xD4,
    0xB1,
    0x15,
    0x99,
    0xAC,
    0xD9,
    0x96,
    0xC5,
    0xF2,
    0xB8,
    0xF1,
    0x33,
    0xCF,
    0x68,
    0xF7,
    0x8B,
    0x24,
    0x0F,
    0xB7,
    0x28,
    0xBE,
    0x1D,
    0x3C,
    0xD1,
    0xE4,
    0x03,
    0x89,
    0x74,
    0x40,
    0xC5,
    0x74,
    0x4F,
    0x9E,
    0x3B,
    0x1F,
    0x37,
    0x29,
    0xBE,
    0x39,
    0x62,
    0xBA,
    0x3C,
    0xC8,
    0x35,
    0x14,
    0x9F,
    0x50,
    0x72,
    0xF5,
    0x74,
    0x90,
    0xB8,
    0x5A,
    0x42,
    0x72,
    0x2A,
    0x59,
    0xE0,
    0x3C,
    0xC8,
    0x0D,
    0x3F,
    0xCC,
    0xAB,
    0xE0,
    0x7B,
    0x72,
    0xB2,
    0x6B,
    0x18,
    0x90,
    0x12,
    0xD7,
    0x48,
    0x88,
    0x66,
    0x5A,
    0x47,
    0x5B,
    0x5E,
    0x24,
    0xBF,
    0x76,
    0x1E,
    0xC4,
    0xC3,
    0x0F,
    0x73,
    0xA3,
    0x44,
    0x5C,
    0xE9,
    0xC1,
    0x2F,
    0x8B,
    0xDC,
    0x6F,
    0x3C,
    0x38,
    0xBB,
    0x03,
    0xBF,
    0xAF,
    0xF8,
    0xFA,
    0xF0,
    0x30,
    0x11,
    0x8B,
    0xF1,
    0x47,
    0x8A,
    0xFD,
    0x23,
    0xEE,
    0x29,
    0x54,
    0x06,
    0xFB,
    0x15,
    0x97,
    0x06,
    0x52,
    0x73,
    0x5A,
    0xC1,
    0x29,
    0x1D,
    0x78,
    0x4E,
    0x87,
    0x3A,
    0x32,
    0x1D,
    0x4E,
    0xC6,
    0x5A,
    0xDC,
    0x0B,
    0xC1,
    0xA2,
    0xBC,
    0xCA,
    0x53,
    0xCC,
    0xC1,
    0x22,
    0xB3,
    0x83,
    0xDB,
    0x7F,
    0x02,
    0xAF,
    0x62,
    0x28,
    0x32,
    0x9B,
    0x79,
    0xFC,
    0x55,
    0xAD,
    0x3A,
    0x7D,
    0xCC,
    0xA5,
    0x1D,
    0x49,
    0x04,
    0xC1,
    0x7D,
    0x3B,
    0xF0,
    0xFE,
    0x13,
    0xD6,
    0xE2,
    0xF2,
    0x3F,
    0x47,
    0x03,
    0x57,
    0xEB,
    0x64,
    0x27,
    0x23,
    0x16,
    0x47,
    0x59,
    0xBB,
    0xA6,
    0xEC,
    0x2F,
    0xC3,
    0x4D,
    0x16,
    0xFF,
    0x18,
    0x3D,
    0x13,
    0xBC,
    0xBC,
    0xF8,
    0xE0,
    0x71,
    0xCF,
    0x48,
    0xF7,
    0x33,
    0x9C,
    0x5F,
    0x21,
    0x51,
    0x67,
    0x07,
    0xD4,
    0x05,
    0x67,
    0x9C,
    0x25,
    0x5D,
    0xCE,
    0x0E,
    0xCF,
    0x7F,
    0xE3,
    0xD4,
    0x33,
    0x30,
    0x87,
    0x5E,
    0x53,
    0x5B,
    0x08,
    0x7A,
    0x8A,
    0xC4,
    0x61,
    0x7F,
    0xDA,
    0x49,
    0x71,
    0x91,
    0x49,
    0xAF,
    0xC3,
    0xC5,
    0xF0,
    0x6F,
    0xD4,
    0x8A,
    0xFD,
    0x8A,
    0x15,
    0xA1,
    0x9F,
    0xDA,
    0x6C,
    0xF1,
    0x84,
    0xAF,
    0x59,
    0xCF,
    0xAA,
    0x8B,
    0xBB,
    0xCE,
    0x0E,
    0x6E,
    0x42,
    0x54,
    0x6D,
    0x36,
    0x06,
    0x76,
    0xFF,
    0xA3,
    0xC3,
    0xF5,
    0x06,
    0xD0,
    0x7A,
    0x94,
    0x68,
    0x39,
    0x68,
    0x83,
    0x44,
    0xA1,
    0xFB,
    0xE1,
    0xF6,
    0x0E,
    0x02,
    0x30,
    0x97,
    0x75,
    0xC6,
    0xC4,
    0xED,
    0xEB,
    0xD8,
    0x58,
    0x9C,
    0xD5,
    0xE6,
    0xDA,
    0xA4,
    0x0E,
    0x1B,
    0x18,
    0xF8,
    0x92,
    0x2D,
    0xB1,
    0xC1,
    0xA3,
    0x87,
    0x58,
    0x73,
    0x52,
    0xBF,
    0x78,
    0xB2,
    0x79,
    0xB7,
    0xF9,
    0x2D,
    0xCD,
    0xE6,
    0x67,
    0x5E,
    0xBD,
    0xF6,
    0x99,
    0x55,
    0xFC,
    0xD9,
    0x8D,
    0x9C,
    0x84,
    0x0D,
    0x50,
    0x48,
    0x1C,
    0x24,
    0xCE,
    0x35,
    0x49,
    0x1B,
    0xED,
    0xA9,
    0xE4,
    0xAC,
    0xF3,
    0x14,
    0xCE,
    0x3A,
    0x43,
    0x7E,
    0xE0,
    0xBC,
    0x8E,
    0x27,
    0x44,
    0xCD,
    0x09,
    0x28,
    0x9A,
    0xE0,
    0x0A,
    0x7C,
    0xB8,
    0xFA,
    0xF7,
    0xF8,
    0x3E,
    0xBF,
    0x3E,
    0xBA,
    0xC9,
    0x1F,
    0x5E,
    0x35,
    0xFC,
    0x81,
    0xE7,
    0xE1,
    0x9A,
    0x4D,
    0x5F,
    0xFA,
    0xF4,
    0x6B,
    0x33,
    0x46,
    0x2C,
    0x0E,
    0xAD,
    0xFD,
    0x74,
    0xC6,
    0x85,
    0x0F,
    0xD6,
    0x0D,
    0x58,
    0xB0,
    0x94,
    0x5B,
    0x77,
    0x22,
    0xFE,
    0xA3,
    0xD5,
    0xEB,
    0xE6,
    0x1B,
    0xCB,
    0xD6,
    0xAF,
    0xDF,
    0x3A,
    0x71,
    0xF7,
    0x77,
    0xD6,
    0x73,
    0x17,
    0x1D,
    0xE7,
    0x37,
    0x54,
    0x57,
    0x57,
    0xF7,
    0xDF,
    0xA8,
    0x72,
    0xC1,
    0x17,
    0x15,
    0x60,
    0xDC,
    0x1F,
    0xAF,
    0xF6,
    0x94,
    0xB8,
    0x4A,
    0xFF,
    0x93,
    0x61,
    0xCB,
    0x8D,
    0x2C,
    0xF7,
    0x1D,
    0x37,
    0xF7,
    0x4B,
    0x37,
    0x9E,
    0xD1,
    0x89,
    0xDF,
    0xBB,
    0x8E,
    0x47,
    0x76,
    0xBA,
    0xBE,
    0xD8,
    0x43,
    0x3B,
    0x63,
    0xAB,
    0xE1,
    0xEE,
    0xAC,
    0xF1,
    0xF2,
    0x71,
    0xE9,
    0x3B,
    0xBE,
    0x74,
    0x09,
    0xCB,
    0x77,
    0x0B,
    0x7B,
    0x8C,
    0x85,
    0x3D,
    0x94,
    0x2F,
    0x9D,
    0x7A,
    0x7E,
    0x7A,
    0x09,
    0xE1,
    0x84,
    0x99,
    0x36,
    0x3D,
    0x4B,
    0x4C,
    0xCE,
    0x77,
    0x6C,
    0x7D,
    0xD9,
    0x8A,
    0x7E,
    0x2C,
    0x71,
    0x38,
    0x8F,
    0x0A,
    0xC9,
    0x96,
    0x14,
    0x52,
    0xED,
    0xAC,
    0x13,
    0x58,
    0x4B,
    0x92,
    0x6B,
    0xA8,
    0xDF,
    0x75,
    0x7D,
    0xC7,
    0x8A,
    0x90,
    0xFE,
    0x09,
    0x61,
    0x85,
    0x19,
    0x70,
    0x9E,
    0x59,
    0xE8,
    0x2F,
    0x18,
    0x5C,
    0x9A,
    0xCA,
    0xAF,
    0x34,
    0x95,
    0x5F,
    0x6A,
    0x5C,
    0xD9,
    0xD6,
    0x81,
    0x85,
    0xF8,
    0x6D,
    0xC5,
    0x9A,
    0x67,
    0xC9,
    0x2F,
    0xC4,
    0xBB,
    0x15,
    0x6B,
    0x01,
    0x5C,
    0x89,
    0x97,
    0x2D,
    0xAF,
    0x38,
    0x99,
    0x5D,
    0x8E,
    0xBC,
    0x58,
    0x1E,
    0x1C,
    0x9C,
    0xCF,
    0x8B,
    0x6A,
    0xC3,
    0xA6,
    0xB9,
    0xC9,
    0x0A,
    0x67,
    0x27,
    0x74,
    0x9F,
    0x58,
    0x50,
    0xD8,
    0x95,
    0x57,
    0x2D,
    0x16,
    0x76,
    0xE5,
    0xE3,
    0x4B,
    0x7E,
    0xC2,
    0x27,
    0x30,
    0x6E,
    0xA2,
    0x67,
    0xCB,
    0xF5,
    0x4D,
    0xEE,
    0xB5,
    0xE2,
    0xE0,
    0x43,
    0x9D,
    0x87,
    0xAC,
    0xB5,
    0xAE,
    0x3E,
    0xFE,
    0xD7,
    0x3B,
    0xF7,
    0x31,
    0x6C,
    0x48,
    0xFF,
    0x98,
    0x67,
    0xF0,
    0xF8,
    0x71,
    0x33,
    0x8A,
    0x27,
    0xE0,
    0xF1,
    0x6F,
    0xCE,
    0x9C,
    0x34,
    0x03,
    0x17,
    0x4D,
    0x9F,
    0x34,
    0x49,
    0xC7,
    0x6C,
    0xD3,
    0x61,
    0xF8,
    0x46,
    0xC1,
    0x87,
    0x27,
    0x4C,
    0x9D,
    0x32,
    0x65,
    0xEA,
    0xC4,
    0xA9,
    0xD3,
    0x27,
    0xE1,
    0xD7,
    0x5E,
    0xF9,
    0x83,
    0xE5,
    0xEC,
    0x97,
    0x12,
    0x5B,
    0x3A,
    0x1D,
    0x40,
    0x3F,
    0xC4,
    0xD3,
    0xC7,
    0x4D,
    0xC1,
    0x33,
    0xDE,
    0x9C,
    0x31,
    0x73,
    0xD2,
    0x14,
    0x8C,
    0x99,
    0x8D,
    0x26,
    0x2C,
    0x29,
    0xD5,
    0x66,
    0xBB,
    0xA6,
    0x9C,
    0xAF,
    0xFE,
    0x17,
    0x9F,
    0x40,
    0x88,
    0x46,
    0x38,
    0xCC,
    0x7B,
    0xAD,
    0xC8,
    0x71,
    0x9F,
    0x5F,
    0x82,
    0xDC,
    0x8E,
    0x20,
    0x4D,
    0x3A,
    0x77,
    0x80,
    0x3E,
    0xE6,
    0xB3,
    0x9F,
    0x9F,
    0x98,
    0x3D,
    0xB8,
    0x6F,
    0xB7,
    0x6E,
    0x69,
    0x46,
    0x4A,
    0x4A,
    0x03,
    0x2A,
    0xD0,
    0xD4,
    0x32,
    0x4B,
    0x18,
    0x17,
    0xE3,
    0x6C,
    0xE7,
    0x5D,
    0x39,
    0xCE,
    0x6C,
    0xDE,
    0x97,
    0x6A,
    0x8D,
    0x58,
    0xC2,
    0xF8,
    0x3F,
    0xBB,
    0x3C,
    0x11,
    0x77,
    0x98,
    0xC7,
    0xCE,
    0x2E,
    0xC2,
    0xD8,
    0x1C,
    0x48,
    0xB8,
    0x42,
    0x4C,
    0xB6,
    0xDF,
    0x22,
    0x72,
    0xD3,
    0x66,
    0x47,
    0x24,
    0xDD,
    0xF6,
    0x16,
    0xB2,
    0x55,
    0x20,
    0x92,
    0x69,
    0xB3,
    0x22,
    0x92,
    0x64,
    0xDB,
    0x82,
    0x48,
    0x86,
    0x70,
    0x49,
    0xB8,
    0xE8,
    0x2D,
    0x43,
    0xDA,
    0xD2,
    0x2A,
    0xC2,
    0x54,
    0x6F,
    0x6F,
    0x35,
    0x2C,
    0xC9,
    0x81,
    0x77,
    0xAD,
    0x3B,
    0x3A,
    0x98,
    0xDE,
    0x27,
    0xCA,
    0x8E,
    0x56,
    0x97,
    0x01,
    0x2B,
    0x3D,
    0x21,
    0xFD,
    0xFF,
    0x39,
    0x19,
    0x4B,
    0xD8,
    0x12,
    0xF1,
    0x44,
    0xC4,
    0x70,
    0x7D,
    0xAB,
    0x31,
    0x0D,
    0x8C,
    0x84,
    0x70,
    0x48,
    0xFF,
    0x00,
    0xCB,
    0x3D,
    0xE4,
    0x85,
    0x6E,
    0x40,
    0x52,
    0x00,
    0xE9,
    0xD6,
    0x3F,
    0x12,
    0x9A,
    0xB8,
    0x67,
    0xE3,
    0x9D,
    0x92,
    0xCC,
    0x72,
    0xB7,
    0x63,
    0x7C,
    0x1C,
    0x18,
    0x70,
    0x8B,
    0xBA,
    0xA0,
    0x6A,
    0x7B,
    0xA0,
    0x6A,
    0xBB,
    0x5D,
    0xDF,
    0xF4,
    0x9F,
    0xBC,
    0xB5,
    0x4F,
    0xB9,
    0x12,
    0x02,
    0x1D,
    0x35,
    0xBF,
    0x41,
    0x2E,
    0xC6,
    0xBF,
    0x12,
    0xED,
    0xFB,
    0x0D,
    0xEA,
    0xEE,
    0x0C,
    0xD0,
    0x87,
    0x81,
    0xC0,
    0xAE,
    0x12,
    0x03,
    0x8F,
    0x07,
    0xDE,
    0x10,
    0x03,
    0xC2,
    0x45,
    0xDB,
    0x56,
    0x24,
    0x3F,
    0x5E,
    0x29,
    0x5E,
    0x91,
    0x33,
    0xC4,
    0xCB,
    0xB5,
    0x9B,
    0x11,
    0x9C,
    0x7C,
    0x05,
    0xF7,
    0x8F,
    0xE3,
    0xFA,
    0x2E,
    0x69,
    0x13,
    0xD5,
    0xBB,
    0xB4,
    0x2B,
    0xAD,
    0x01,
    0x7F,
    0xEC,
    0xD6,
    0xBE,
    0x5F,
    0x23,
    0x08,
    0x24,
    0x5F,
    0x91,
    0x74,
    0x34,
    0xE8,
    0x6E,
    0xB2,
    0xD0,
    0xBA,
    0x06,
    0x09,
    0x53,
    0xB6,
    0x8C,
    0x3E,
    0x0E,
    0x5A,
    0x82,
    0x4E,
    0xA6,
    0x62,
    0x2B,
    0x02,
    0xBA,
    0x8D,
    0xA2,
    0x64,
    0x1C,
    0xE2,
    0x89,
    0x60,
    0x47,
    0x57,
    0xA7,
    0x44,
    0xFB,
    0x89,
    0x21,
    0x21,
    0x7C,
    0x2E,
    0x68,
    0x4F,
    0xF7,
    0xE5,
    0xED,
    0x32,
    0x95,
    0x9B,
    0x44,
    0x2A,
    0x6F,
    0x73,
    0x87,
    0xEB,
    0x1A,
    0x84,
    0xE6,
    0xDC,
    0x31,
    0x29,
    0xCD,
    0xCF,
    0x36,
    0xEB,
    0x21,
    0x01,
    0xF6,
    0x24,
    0x4F,
    0x23,
    0x4D,
    0x5A,
    0x8A,
    0xB4,
    0x63,
    0x63,
    0x61,
    0x6C,
    0x89,
    0x7D,
    0x1C,
    0xA4,
    0x83,
    0x80,
    0x38,
    0x25,
    0x3A,
    0xCF,
    0xEE,
    0x28,
    0x2B,
    0xA7,
    0x78,
    0xF6,
    0xC0,
    0xC8,
    0x02,
    0x49,
    0xA2,
    0x13,
    0x9A,
    0xCD,
    0xD5,
    0xE6,
    0x2A,
    0xB1,
    0x59,
    0xFA,
    0x94,
    0x22,
    0xA2,
    0x18,
    0x78,
    0x69,
    0x24,
    0x4D,
    0x13,
    0x1B,
    0x8B,
    0x35,
    0x42,
    0x23,
    0x19,
    0xC9,
    0x6B,
    0xC7,
    0x7A,
    0xCE,
    0xCB,
    0x10,
    0xCE,
    0xD3,
    0x22,
    0xA3,
    0x40,
    0x4B,
    0xAD,
    0x15,
    0xA9,
    0xEC,
    0x8F,
    0xC7,
    0xC0,
    0x1E,
    0x80,
    0xA2,
    0x46,
    0xF0,
    0x34,
    0x94,
    0xF0,
    0xF8,
    0x68,
    0x77,
    0x2C,
    0x2C,
    0x36,
    0x9D,
    0x0B,
    0xFB,
    0xC6,
    0x48,
    0x53,
    0xE5,
    0x97,
    0xBC,
    0xA0,
    0x99,
    0xE5,
    0x16,
    0xF7,
    0x78,
    0x22,
    0xD1,
    0x6C,
    0x7B,
    0x2A,
    0x8E,
    0x77,
    0x07,
    0x20,
    0xD7,
    0x03,
    0x5B,
    0x67,
    0xB7,
    0x76,
    0x1F,
    0x6B,
    0xD5,
    0x96,
    0xEE,
    0x68,
    0x0D,
    0x89,
    0x4D,
    0x6A,
    0xCE,
    0x9B,
    0x8E,
    0xB5,
    0x1A,
    0xD9,
    0x10,
    0x7D,
    0xCA,
    0xC9,
    0x3D,
    0x81,
    0x08,
    0x5E,
    0xDB,
    0x0D,
    0xF4,
    0xC2,
    0xBC,
    0x27,
    0x42,
    0x53,
    0xAC,
    0x61,
    0xB3,
    0x99,
    0xEE,
    0x0B,
    0x59,
    0x23,
    0x50,
    0x7D,
    0x48,
    0x47,
    0x98,
    0x37,
    0x09,
    0xCD,
    0xE5,
    0xA6,
    0x5D,
    0x26,
    0x95,
    0xBF,
    0x3B,
    0x2C,
    0x36,
    0xC3,
    0x65,
    0x6F,
    0xBC,
    0x01,
    0xB9,
    0x62,
    0x39,
    0x47,
    0xB7,
    0x84,
    0xED,
    0x7A,
    0x89,
    0xB7,
    0x27,
    0xE5,
    0x5E,
    0xA3,
    0x28,
    0x77,
    0x0C,
    0x08,
    0xE5,
    0xB6,
    0xC5,
    0x80,
    0x2B,
    0xCB,
    0xED,
    0x8C,
    0x49,
    0x03,
    0xE8,
    0x8B,
    0xBB,
    0x68,
    0x0A,
    0xD6,
    0xDC,
    0xE0,
    0x56,
    0xDC,
    0xC0,
    0x07,
    0xBA,
    0xCB,
    0xA0,
    0x9D,
    0x9E,
    0x8D,
    0x07,
    0xEB,
    0x1A,
    0xCA,
    0x1C,
    0x0D,
    0xC1,
    0xBA,
    0xFB,
    0x65,
    0x8E,
    0xFB,
    0xC1,
    0x8F,
    0x9B,
    0xE8,
    0x5D,
    0x82,
    0x1D,
    0x0D,
    0x89,
    0xE6,
    0xD4,
    0x4F,
    0xF1,
    0x34,
    0xF6,
    0x3A,
    0xF6,
    0xA3,
    0x48,
    0x38,
    0x2F,
    0xF5,
    0xA7,
    0x2C,
    0x5E,
    0xD4,
    0x25,
    0x0D,
    0xA3,
    0x83,
    0x82,
    0x20,
    0xD0,
    0x12,
    0x2E,
    0xFB,
    0xB8,
    0xC9,
    0xAE,
    0x4D,
    0x30,
    0x55,
    0xFB,
    0x87,
    0xAB,
    0x8A,
    0x49,
    0x7D,
    0xE9,
    0x30,
    0xC3,
    0xAE,
    0x25,
    0x39,
    0x55,
    0xB1,
    0xC8,
    0xB9,
    0x26,
    0xFB,
    0x2D,
    0x39,
    0x1E,
    0x88,
    0xF4,
    0xE4,
    0xD8,
    0x93,
    0x84,
    0x30,
    0x7D,
    0x02,
    0x46,
    0x60,
    0x80,
    0x2F,
    0x74,
    0x73,
    0x13,
    0x6F,
    0xE5,
    0x6E,
    0x90,
    0x36,
    0x40,
    0x66,
    0x7F,
    0x0E,
    0x54,
    0x81,
    0x71,
    0xEC,
    0x0F,
    0xC4,
    0xA1,
    0x3A,
    0x63,
    0x80,
    0x61,
    0x81,
    0xBB,
    0x29,
    0x51,
    0xED,
    0xA4,
    0x8F,
    0xC3,
    0x34,
    0xF5,
    0xFD,
    0xA9,
    0xF2,
    0xB3,
    0x78,
    0x61,
    0x8F,
    0xBB,
    0x09,
    0x78,
    0x96,
    0xD5,
    0x35,
    0x04,
    0x1D,
    0x0D,
    0x65,
    0x75,
    0xF7,
    0x83,
    0x8E,
    0xFB,
    0x65,
    0xB0,
    0xF9,
    0x67,
    0x9E,
    0xB1,
    0x16,
    0x08,
    0x0D,
    0x1A,
    0x93,
    0xB5,
    0x63,
    0x81,
    0x2C,
    0x90,
    0x18,
    0xE8,
    0x89,
    0xC8,
    0x05,
    0x01,
    0xE8,
    0xE6,
    0xC2,
    0xDE,
    0xE3,
    0xF5,
    0xA3,
    0x20,
    0x71,
    0xF4,
    0x39,
    0x90,
    0x91,
    0x04,
    0x29,
    0x3B,
    0x0F,
    0x0A,
    0xFA,
    0xED,
    0x92,
    0xB5,
    0xBD,
    0x19,
    0x00,
    0x57,
    0x48,
    0x08,
    0x8C,
    0x20,
    0x2F,
    0x05,
    0x4F,
    0xEC,
    0x82,
    0x99,
    0xB6,
    0x54,
    0xE5,
    0x39,
    0xF5,
    0x56,
    0x91,
    0xE4,
    0x89,
    0x74,
    0x47,
    0xB3,
    0x41,
    0x75,
    0x1C,
    0x0A,
    0xA5,
    0x7F,
    0x60,
    0x8D,
    0x70,
    0xFF,
    0xD9,
    0xE5,
    0x62,
    0xAC,
    0x2D,
    0xD6,
    0xF3,
    0xD6,
    0x46,
    0x6B,
    0xB8,
    0x57,
    0x92,
    0x0B,
    0x11,
    0xE6,
    0x5C,
    0x58,
    0x4E,
    0x85,
    0x02,
    0x86,
    0xE8,
    0x23,
    0x3E,
    0x10,
    0xE1,
    0x01,
    0x79,
    0xE1,
    0x2F,
    0x10,
    0xB5,
    0xE8,
    0x32,
    0x8A,
    0x01,
    0x9E,
    0x75,
    0xB5,
    0xAD,
    0x5B,
    0x4E,
    0x82,
    0xA7,
    0x8E,
    0xAF,
    0xB4,
    0x22,
    0xDE,
    0x9E,
    0x3C,
    0xF4,
    0xF7,
    0x6F,
    0x0F,
    0x1F,
    0xB1,
    0x7F,
    0xE3,
    0x8E,
    0xDF,
    0xED,
    0xDC,
    0xB5,
    0x7B,
    0xCF,
    0x5E,
    0xB1,
    0xC6,
    0x24,
    0x7A,
    0x4D,
    0x62,
    0xAD,
    0x49,
    0x3C,
    0x62,
    0x72,
    0x29,
    0x81,
    0x48,
    0x40,
    0xBA,
    0x2C,
    0xA2,
    0x6E,
    0xB8,
    0x83,
    0x51,
    0x83,
    0x2F,
    0xC3,
    0xFF,
    0xF9,
    0x2D,
    0xA1,
    0xE6,
    0x58,
    0x1B,
    0x0E,
    0xF6,
    0x18,
    0x6F,
    0xC1,
    0x95,
    0x75,
    0x31,
    0xC7,
    0x5A,
    0x73,
    0x37,
    0xD4,
    0xB7,
    0xE9,
    0x91,
    0xDA,
    0x3E,
    0xDD,
    0x51,
    0x03,
    0xBD,
    0x2B,
    0x7E,
    0xAE,
    0x2D,
    0x15,
    0x3E,
    0x17,
    0x5B,
    0xF5,
    0x1F,
    0xC5,
    0x22,
    0x77,
    0x44,
    0x44,
    0x36,
    0xD8,
    0x98,
    0x76,
    0xAF,
    0xA6,
    0x5D,
    0xFA,
    0x22,
    0x2D,
    0x31,
    0xCD,
    0xA1,
    0x7D,
    0x9C,
    0x4C,
    0x3C,
    0x44,
    0xBF,
    0x2C,
    0xA3,
    0x4F,
    0x2D,
    0x01,
    0x4F,
    0xC4,
    0x2C,
    0x1C,
    0x31,
    0x0B,
    0xB5,
    0x66,
    0xC1,
    0x6B,
    0x16,
    0x6A,
    0x80,
    0xA2,
    0xAA,
    0xEF,
    0x8B,
    0x4A,
    0x11,
    0x45,
    0x0D,
    0x72,
    0xA6,
    0x3D,
    0x03,
    0xF4,
    0xD6,
    0x8A,
    0x28,
    0x14,
    0x88,
    0x70,
    0x6D,
    0xB1,
    0xBF,
    0xA2,
    0x95,
    0x1D,
    0x02,
    0x3E,
    0xB7,
    0x44,
    0x64,
    0xCF,
    0x94,
    0x33,
    0xFC,
    0x77,
    0x55,
    0x4A,
    0x2D,
    0x2A,
    0xA5,
    0x1E,
    0x95,
    0xD2,
    0x20,
    0x15,
    0xCF,
    0x3A,
    0xD6,
    0x06,
    0xBC,
    0x5A,
    0x13,
    0xBC,
    0xDA,
    0x42,
    0xF4,
    0x16,
    0x84,
    0xC1,
    0x6D,
    0x31,
    0xAE,
    0xA4,
    0x27,
    0x97,
    0xD5,
    0x95,
    0x21,
    0xC1,
    0x8B,
    0xA3,
    0x3D,
    0xCE,
    0x17,
    0xEA,
    0x6A,
    0x1C,
    0x0D,
    0x75,
    0xB5,
    0x50,
    0x45,
    0xFD,
    0x43,
    0x1E,
    0x0A,
    0x2B,
    0xD6,
    0x08,
    0xB5,
    0xA2,
    0x37,
    0x97,
    0xD5,
    0xB6,
    0x08,
    0x47,
    0x78,
    0xBF,
    0x7C,
    0x4B,
    0x68,
    0x00,
    0x71,
    0x09,
    0xF3,
    0x4F,
    0x78,
    0xE7,
    0x0B,
    0x70,
    0x20,
    0xD8,
    0xE0,
    0xB6,
    0x4E,
    0xB5,
    0xDC,
    0x65,
    0xFA,
    0x47,
    0x7C,
    0xBC,
    0x8A,
    0x20,
    0xE1,
    0x12,
    0x60,
    0xBB,
    0xDC,
    0x61,
    0x47,
    0x43,
    0x39,
    0xBC,
    0xA1,
    0x62,
    0x83,
    0x70,
    0x5F,
    0xFC,
    0x24,
    0x11,
    0xE0,
    0x01,
    0x8F,
    0xE3,
    0x8A,
    0x78,
    0x91,
    0xE6,
    0x06,
    0x2E,
    0xD1,
    0xE7,
    0xC8,
    0x00,
    0xE1,
    0x92,
    0x3B,
    0x5C,
    0x74,
    0x05,
    0x9E,
    0x99,
    0xBA,
    0x06,
    0x5F,
    0xDF,
    0xD8,
    0x15,
    0xE1,
    0x32,
    0xB8,
    0xD5,
    0xDD,
    0xB7,
    0x6D,
    0x46,
    0x70,
    0x6F,
    0xC5,
    0xCB,
    0xC5,
    0xA5,
    0x78,
    0x68,
    0x5C,
    0xD4,
    0xE4,
    0xEA,
    0xE5,
    0x74,
    0x68,
    0xE0,
    0xF7,
    0x45,
    0x8D,
    0x16,
    0xC3,
    0x45,
    0x7F,
    0x9F,
    0xA6,
    0x00,
    0x36,
    0x42,
    0xD0,
    0xE0,
    0xCD,
    0x71,
    0xAF,
    0xAE,
    0xDD,
    0x5C,
    0x05,
    0x57,
    0x7A,
    0x2C,
    0xBC,
    0xB5,
    0x36,
    0xE4,
    0x1C,
    0x6B,
    0xB9,
    0x00,
    0xA9,
    0xAC,
    0xD5,
    0xB4,
    0x47,
    0x15,
    0x79,
    0x6E,
    0x15,
    0xD9,
    0x28,
    0x3C,
    0x24,
    0x37,
    0x85,
    0x47,
    0xE4,
    0x6B,
    0x00,
    0x5D,
    0x69,
    0x26,
    0xC8,
    0xF6,
    0x25,
    0x4D,
    0x3B,
    0x7D,
    0x32,
    0xD9,
    0x37,
    0xD0,
    0x54,
    0xF7,
    0x50,
    0x4A,
    0xF6,
    0x65,
    0x48,
    0xAC,
    0x9C,
    0xA6,
    0xB3,
    0x21,
    0x70,
    0xD5,
    0x23,
    0xCB,
    0x05,
    0x73,
    0xF0,
    0xFD,
    0x71,
    0xBE,
    0x3E,
    0x21,
    0x7A,
    0xAD,
    0xD0,
    0x3C,
    0x0A,
    0x3C,
    0xE8,
    0x45,
    0xF3,
    0x7F,
    0xFE,
    0x16,
    0xB9,
    0x2F,
    0xD4,
    0x3D,
    0xAC,
    0xFE,
    0xF8,
    0x9C,
    0x9C,
    0xEA,
    0xD4,
    0x57,
    0x54,
    0x20,
    0xFA,
    0x5A,
    0xEE,
    0x86,
    0xFC,
    0x72,
    0x64,
    0x1C,
    0xB8,
    0xB4,
    0x58,
    0xDA,
    0x20,
    0xE7,
    0x4A,
    0x03,
    0xE4,
    0x0C,
    0x09,
    0xCB,
    0x69,
    0xD2,
    0x30,
    0x39,
    0x09,
    0xDA,
    0xFA,
    0xD5,
    0x9A,
    0x0A,
    0x24,
    0x27,
    0x5F,
    0x06,
    0xB3,
    0xBC,
    0xCB,
    0x5B,
    0x10,
    0x7D,
    0xDE,
    0x99,
    0x54,
    0xB1,
    0x05,
    0xB9,
    0x03,
    0x65,
    0xFA,
    0xA1,
    0xEE,
    0x0B,
    0x96,
    0x73,
    0x35,
    0x36,
    0x54,
    0x51,
    0x89,
    0x80,
    0x59,
    0x66,
    0xB9,
    0x27,
    0x70,
    0xEB,
    0x2D,
    0xE4,
    0x4B,
    0x51,
    0xDF,
    0x5F,
    0x6B,
    0x20,
    0x51,
    0x7E,
    0xDE,
    0x6B,
    0x43,
    0x12,
    0x4B,
    0xBF,
    0x71,
    0xA9,
    0x12,
    0xC9,
    0x77,
    0xBC,
    0x0E,
    0x64,
    0x1C,
    0xE6,
    0x35,
    0xD0,
    0x62,
    0x64,
    0x33,
    0x50,
    0xEE,
    0xD7,
    0x37,
    0xD3,
    0xA5,
    0x34,
    0x3B,
    0x22,
    0x29,
    0xD5,
    0xFE,
    0x55,
    0x37,
    0x85,
    0x87,
    0xFE,
    0xD5,
    0x20,
    0x89,
    0x8B,
    0xC6,
    0x37,
    0xDE,
    0xFC,
    0xF0,
    0x26,
    0x7B,
    0x6B,
    0xE5,
    0xAD,
    0x01,
    0xBA,
    0x61,
    0x65,
    0x82,
    0x75,
    0x73,
    0xDF,
    0x97,
    0xE7,
    0x8C,
    0x7B,
    0xE5,
    0xC7,
    0x33,
    0x26,
    0x69,
    0x5E,
    0x9D,
    0x3E,
    0xF1,
    0xB5,
    0x09,
    0x45,
    0x33,
    0x4B,
    0x0A,
    0xDE,
    0x7C,
    0xBD,
    0x70,
    0xBC,
    0x69,
    0xD6,
    0xEC,
    0x6F,
    0x17,
    0x4F,
    0x36,
    0x4E,
    0xF9,
    0xE1,
    0xD4,
    0x37,
    0xF2,
    0xA6,
    0x71,
    0xA3,
    0x5F,
    0xFC,
    0xCE,
    0xD8,
    0xFC,
    0x9F,
    0x8E,
    0xFA,
    0x6E,
    0x7F,
    0xCD,
    0xF7,
    0x7F,
    0xFE,
    0xCD,
    0x6F,
    0xFD,
    0x22,
    0xE9,
    0x25,
    0x8C,
    0x7E,
    0xC4,
    0x2A,
    0xDB,
    0x6D,
    0xFB,
    0x2A,
    0xDE,
    0x2A,
    0xDF,
    0xE2,
    0x78,
    0xA6,
    0xF9,
    0x83,
    0x67,
    0xDB,
    0xC2,
    0xE7,
    0x34,
    0xCF,
    0x7D,
    0xF2,
    0xF1,
    0xF3,
    0x1F,
    0x05,
    0x1A,
    0x5B,
    0x5E,
    0x68,
    0x3D,
    0x9F,
    0xF3,
    0x61,
    0xB0,
    0x29,
    0x92,
    0x2B,
    0x85,
    0x8C,
    0x97,
    0x2E,
    0x5C,
    0xB9,
    0xFC,
    0xBF,
    0xF4,
    0xE2,
    0xD5,
    0x1F,
    0xFC,
    0xE4,
    0xF4,
    0xF7,
    0xC8,
    0xFB,
    0x3F,
    0xDB,
    0xA6,
    0xF9,
    0xFE,
    0xA7,
    0x63,
    0xFE,
    0xED,
    0x17,
    0x49,
    0x23,
    0x9D,
    0xA8,
    0x61,
    0xD3,
    0x9F,
    0x02,
    0xFC,
    0xEE,
    0xA8,
    0x6F,
    0xA7,
    0xB7,
    0xB6,
    0x4E,
    0xB3,
    0xEB,
    0x9D,
    0x9A,
    0xDD,
    0x67,
    0xCF,
    0x1C,
    0xF4,
    0xEF,
    0x39,
    0x21,
    0xEE,
    0x7D,
    0xAF,
    0xFE,
    0xE4,
    0xBB,
    0xB9,
    0xAE,
    0x63,
    0xC6,
    0x43,
    0x07,
    0x4E,
    0x55,
    0x7B,
    0x8E,
    0x1C,
    0x6F,
    0xFF,
    0xC1,
    0x4F,
    0xDC,
    0xDF,
    0xAB,
    0xBA,
    0xD6,
    0x1B,
    0x60,
    0xE3,
    0x2F,
    0x92,
    0x76,
    0x38,
    0xD1,
    0xE1,
    0x4D,
    0x8A,
    0x94,
    0x49,
    0x53,
    0x41,
    0xAE,
    0x16,
    0x51,
    0xAD,
    0x54,
    0x49,
    0xFB,
    0xC0,
    0x34,
    0xF7,
    0x2B,
    0x50,
    0xAE,
    0xDE,
    0xF9,
    0x34,
    0x9A,
    0x4A,
    0x4A,
    0x75,
    0x90,
    0x0F,
    0xAD,
    0xA4,
    0xA5,
    0x77,
    0xC8,
    0x86,
    0xD1,
    0x0E,
    0x64,
    0x73,
    0x20,
    0xD8,
    0x51,
    0xFE,
    0xF0,
    0x0D,
    0x1D,
    0xA4,
    0xF9,
    0xFD,
    0xE0,
    0xFE,
    0xC9,
    0xCA,
    0xDB,
    0xFD,
    0x92,
    0x86,
    0xB3,
    0x29,
    0x03,
    0x0C,
    0x23,
    0xB2,
    0x53,
    0xF7,
    0xA3,
    0xB4,
    0x6F,
    0x0C,
    0xEC,
    0xA7,
    0x4D,
    0xDF,
    0xAA,
    0xEB,
    0x93,
    0x95,
    0xA9,
    0xE8,
    0x33,
    0x14,
    0x65,
    0x08,
    0xA3,
    0x0C,
    0x55,
    0x86,
    0x29,
    0xCA,
    0x88,
    0xE1,
    0x8A,
    0x3D,
    0x45,
    0x49,
    0x56,
    0x94,
    0xBE,
    0x8A,
    0xC2,
    0xFC,
    0xED,
    0xB7,
    0xB7,
    0x00,
    0x86,
    0x34,
    0x66,
    0x40,
    0x56,
    0xEF,
    0x32,
    0x33,
    0x45,
    0x83,
    0xD8,
    0x24,
    0x46,
    0x1D,
    0x92,
    0x53,
    0x52,
    0xB9,
    0xA9,
    0xE3,
    0x26,
    0xEA,
    0xA6,
    0xCF,
    0x7A,
    0x43,
    0xC7,
    0xE4,
    0x4C,
    0x5B,
    0xE9,
    0x0C,
    0xCA,
    0x86,
    0xB1,
    0x66,
    0xFB,
    0xC9,
    0x68,
    0x9A,
    0xB1,
    0xA4,
    0xB4,
    0x0A,
    0xDE,
    0x4E,
    0xD3,
    0xD2,
    0x5D,
    0xEF,
    0x6B,
    0xC7,
    0xE6,
    0x6B,
    0x31,
    0x76,
    0xCB,
    0xA6,
    0x67,
    0xB7,
    0xEB,
    0xD3,
    0x0B,
    0x17,
    0x1D,
    0xC4,
    0x63,
    0x6F,
    0x17,
    0x6D,
    0x2F,
    0x3A,
    0x68,
    0x16,
    0x76,
    0xAC,
    0xC4,
    0x3B,
    0xEF,
    0x48,
    0x63,
    0x28,
    0x8B,
    0x85,
    0x3B,
    0x5E,
    0xE6,
    0xAA,
    0x36,
    0xCD,
    0xC6,
    0x5C,
    0x5D,
    0x89,
    0x2B,
    0xEF,
    0x00,
    0x8A,
    0x8F,
    0xDE,
    0x51,
    0x37,
    0x31,
    0xB9,
    0xE3,
    0x5B,
    0x00,
    0xBB,
    0xCF,
    0x1E,
    0xD4,
    0xA7,
    0xC3,
    0xDC,
    0xFE,
    0x85,
    0x0A,
    0xC0,
    0x00,
    0x33,
    0xB0,
    0x23,
    0x69,
    0x82,
    0x08,
    0x7E,
    0x97,
    0x98,
    0xAB,
    0xF4,
    0x49,
    0xBA,
    0x7D,
    0xCC,
    0xA2,
    0x1D,
    0xBE,
    0x64,
    0xEC,
    0xBC,
    0x43,
    0x59,
    0x70,
    0x04,
    0x9F,
    0x38,
    0x7C,
    0xB0,
    0x9B,
    0x7B,
    0x4D,
    0xAB,
    0x87,
    0xB8,
    0x01,
    0x91,
    0x36,
    0x92,
    0x24,
    0x5B,
    0x6A,
    0x3B,
    0xC9,
    0xB6,
    0x65,
    0xB5,
    0x7B,
    0x75,
    0xEA,
    0x2F,
    0x17,
    0x51,
    0x35,
    0xAA,
    0xEF,
    0x41,
    0x09,
    0x4F,
    0x4A,
    0xF3,
    0x09,
    0x8B,
    0x87,
    0x7E,
    0x5D,
    0x62,
    0xB0,
    0x4F,
    0x16,
    0x76,
    0xE0,
    0xD7,
    0x6E,
    0xAB,
    0x9E,
    0x8F,
    0xC1,
    0x8F,
    0x4F,
    0xAC,
    0x56,
    0x42,
    0x5C,
    0xFC,
    0xEE,
    0x6D,
    0x88,
    0x0A,
    0xF6,
    0xC6,
    0x2F,
    0x4A,
    0xF8,
    0x67,
    0xB7,
    0x1B,
    0x93,
    0x0B,
    0x17,
    0x6D,
    0xA7,
    0xC9,
    0x26,
    0x50,
    0x03,
    0x4A,
    0x0C,
    0x3C,
    0x10,
    0xFF,
    0x93,
    0x08,
    0xF2,
    0x8B,
    0x7C,
    0x32,
    0x06,
    0xF7,
    0xBD,
    0x0D,
    0x2E,
    0xE5,
    0xAE,
    0xF4,
    0x10,
    0xFD,
    0xAA,
    0x1A,
    0x3C,
    0xB9,
    0x9D,
    0x77,
    0x56,
    0x12,
    0x46,
    0x10,
    0x7B,
    0x83,
    0xE8,
    0xBF,
    0x20,
    0x9A,
    0x3F,
    0xB2,
    0x01,
    0xBE,
    0x7F,
    0x22,
    0x94,
    0xAA,
    0x1E,
    0xF0,
    0x40,
    0x6F,
    0x10,
    0x45,
    0x39,
    0x99,
    0x20,
    0xEE,
    0x9D,
    0xDB,
    0x70,
    0x32,
    0x78,
    0x90,
    0xB1,
    0x40,
    0x2E,
    0x26,
    0xD2,
    0xD0,
    0x5F,
    0x69,
    0xD6,
    0x3F,
    0x5C,
    0xE4,
    0xFE,
    0xD3,
    0xC2,
    0xF8,
    0x10,
    0x34,
    0x76,
    0x82,
    0xF9,
    0xB3,
    0xF6,
    0x69,
    0x2C,
    0x4E,
    0xB9,
    0x23,
    0xBA,
    0x4B,
    0x0C,
    0xBC,
    0x2A,
    0xE3,
    0x2E,
    0x30,
    0x52,
    0x27,
    0x89,
    0x3C,
    0x24,
    0x90,
    0x6B,
    0x7F,
    0x44,
    0xB0,
    0x3A,
    0xF9,
    0x53,
    0x04,
    0xBE,
    0xDC,
    0x75,
    0x52,
    0x25,
    0xCB,
    0x8B,
    0xF5,
    0xF2,
    0xEB,
    0xC6,
    0x31,
    0x45,
    0xC7,
    0x5D,
    0x8C,
    0x5D,
    0x13,
    0x2A,
    0x9F,
    0xE4,
    0x17,
    0xFC,
    0x81,
    0x7A,
    0x39,
    0xA5,
    0x3C,
    0x37,
    0x49,
    0x38,
    0xC6,
    0x13,
    0x9C,
    0xBF,
    0x1D,
    0xC9,
    0x03,
    0x8C,
    0x43,
    0x97,
    0x0E,
    0x10,
    0xFD,
    0x54,
    0x13,
    0x0A,
    0xD4,
    0x7B,
    0xB7,
    0x21,
    0xFD,
    0xF5,
    0x40,
    0x3D,
    0xBD,
    0x16,
    0xAB,
    0xA7,
    0xB2,
    0xE8,
    0x97,
    0x6F,
    0xD3,
    0xF8,
    0xD2,
    0x9E,
    0x69,
    0xB1,
    0x58,
    0xBD,
    0x4B,
    0xA1,
    0x17,
    0xBC,
    0xDB,
    0xD1,
    0x64,
    0x7B,
    0x6A,
    0xBE,
    0xE6,
    0xAA,
    0x7E,
    0xD8,
    0xB4,
    0xE1,
    0x20,
    0xDF,
    0x2F,
    0x1C,
    0xAB,
    0xDD,
    0x85,
    0xAC,
    0xF5,
    0x35,
    0xFB,
    0xD0,
    0xC5,
    0xFD,
    0x48,
    0xEE,
    0x5F,
    0xF7,
    0x54,
    0x38,
    0x7E,
    0x67,
    0x1F,
    0xE2,
    0xC9,
    0xD8,
    0x7C,
    0xA2,
    0xD7,
    0xED,
    0x44,
    0xB6,
    0x9D,
    0x08,
    0x12,
    0xA3,
    0xBB,
    0x3A,
    0xE5,
    0x00,
    0x3C,
    0xC4,
    0xB6,
    0x03,
    0x08,
    0xA6,
    0xBC,
    0x4B,
    0x0B,
    0x87,
    0x6E,
    0x43,
    0xB2,
    0x26,
    0x34,
    0x0D,
    0x85,
    0x42,
    0xBC,
    0x87,
    0xD0,
    0x97,
    0x03,
    0x55,
    0xF2,
    0x6B,
    0x46,
    0x9D,
    0x78,
    0x68,
    0xD2,
    0x61,
    0xE1,
    0x70,
    0xD1,
    0xA1,
    0x45,
    0xD5,
    0x7C,
    0xA0,
    0x4A,
    0x3C,
    0x24,
    0x2F,
    0x06,
    0x2A,
    0xE9,
    0x44,
    0xB3,
    0xB0,
    0x8A,
    0xDE,
    0x25,
    0x76,
    0xF0,
    0xD4,
    0x81,
    0xFF,
    0x01,
    0x24,
    0x10,
    0x48,
    0xE4,
    0x77,
    0xEF,
    0x8A,
    0x87,
    0x68,
    0x9C,
    0x0B,
    0x77,
    0xD6,
    0xEC,
    0x45,
    0xC1,
    0x8B,
    0x6F,
    0x23,
    0x79,
    0x54,
    0xC5,
    0x5E,
    0x54,
    0x26,
    0x56,
    0x03,
    0x99,
    0xA8,
    0x56,
    0x4E,
    0x9A,
    0xDC,
    0x4D,
    0x9F,
    0x38,
    0x1E,
    0xF7,
    0x6A,
    0xAB,
    0x37,
    0xD6,
    0xA9,
    0x81,
    0x65,
    0x76,
    0x29,
    0xC9,
    0x9F,
    0xE6,
    0x21,
    0x9A,
    0x7C,
    0x92,
    0x94,
    0xAF,
    0xF6,
    0x07,
    0xEA,
    0xA5,
    0xC7,
    0xAD,
    0xBF,
    0x2B,
    0x56,
    0xD3,
    0xCF,
    0xE1,
    0xB5,
    0xDF,
    0x93,
    0x10,
    0x97,
    0x4B,
    0x50,
    0x42,
    0x1E,
    0xC5,
    0xDE,
    0x03,
    0x08,
    0x2A,
    0xF8,
    0x14,
    0x56,
    0x4B,
    0xBF,
    0xF0,
    0x6A,
    0xFE,
    0x1F,
};

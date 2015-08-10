/* C code produced by gperf version 3.0.4 */
/* Command-line: gperf --output-file=keywords.h -7 -c -C -E -t -K _name -H KeywordHash -N KeywordLookup keywords.gperf  */
/* Computed positions: -k'1-2' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "keywords.gperf"
struct KeywordInfo { const char* _name; enum ScannerToken _token; };
/* maximum key range = 47, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
KeywordHash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49,  0,
      49, 30, 49, 49, 15, 49,  5, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49,  0,  0,  0,
      15,  0, 10, 49, 49,  5, 49, 49, 25, 49,
      15,  3, 49, 49,  0, 25, 20,  0, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]];
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct KeywordInfo *
KeywordLookup (str, len)
     register const char *str;
     register unsigned int len;
{
  enum
    {
      TOTAL_KEYWORDS = 27,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 8,
      MIN_HASH_VALUE = 2,
      MAX_HASH_VALUE = 48
    };

  static const struct KeywordInfo wordlist[] =
    {
      {""}, {""},
#line 9 "keywords.gperf"
      {"u1",         TUInt},
#line 11 "keywords.gperf"
      {"u16",        TUInt},
#line 14 "keywords.gperf"
      {"u128",       TUInt},
#line 35 "keywords.gperf"
      {"break",      TKW_Break},
#line 33 "keywords.gperf"
      {"return",     TKW_Return},
#line 10 "keywords.gperf"
      {"u8",         TUInt},
#line 5 "keywords.gperf"
      {"i16",        TInt},
#line 8 "keywords.gperf"
      {"i128",       TInt},
      {""},
#line 28 "keywords.gperf"
      {"constant",   TKW_Constant},
#line 4 "keywords.gperf"
      {"i8",         TInt},
#line 15 "keywords.gperf"
      {"f16",        TFloat},
#line 18 "keywords.gperf"
      {"f128",       TFloat},
#line 37 "keywords.gperf"
      {"false",      TKW_False},
#line 32 "keywords.gperf"
      {"for",        TKW_For},
#line 30 "keywords.gperf"
      {"if",         TKW_If},
#line 13 "keywords.gperf"
      {"u64",        TUInt},
#line 34 "keywords.gperf"
      {"null",       TKW_Null},
      {""}, {""},
#line 39 "keywords.gperf"
      {"depends",    TKW_Depends},
#line 7 "keywords.gperf"
      {"i64",        TInt},
#line 36 "keywords.gperf"
      {"true",       TKW_True},
      {""}, {""},
#line 29 "keywords.gperf"
      {"fn",         TKW_Fn},
#line 17 "keywords.gperf"
      {"f64",        TFloat},
#line 31 "keywords.gperf"
      {"else",       TKW_Else},
      {""}, {""}, {""},
#line 12 "keywords.gperf"
      {"u32",        TUInt},
      {""}, {""}, {""}, {""},
#line 6 "keywords.gperf"
      {"i32",        TInt},
      {""}, {""}, {""}, {""},
#line 16 "keywords.gperf"
      {"f32",        TFloat},
      {""}, {""}, {""}, {""},
#line 19 "keywords.gperf"
      {"str",        TString}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = KeywordHash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key]._name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}

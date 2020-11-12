#include <assert.h>
#include "mem_ops.h"
#include "utils.h"
#include "match_algorithms.h"

int NextMachineState ( char *pat, int M, int state, int x )
{
  // TEST: state and M cannot be negative.
  assert( state >= 0 && M >= 0 );

  // If the character N is same as next character in pattern, then simply increment state
  if ( state < M && x == pat[state] )
    return state + 1;

  int fix = state, i = 0;

  while ( fix > 0 )
  {
    if ( pat[fix - 1] == x )
    {
      for ( i = 0; i < fix - 1; i++ )
        if ( pat[i] != pat[state - fix + 1 + i] )
          break;

      if ( i == fix - 1 )
        return fix;
    }

    fix--;
  }

  return 0;
}

// create automata table
void write_tf ( char *pat, int M, int TF[][256] )
{
  // TEST: M cannot be negative.
  assert( M >= 0 );

  int state, x;

  for ( state = 0; state <= M; ++state )
    for ( x = 0; x < 256; ++x )
      TF[state][x] = NextMachineState ( pat, M,  state, x );
}

/* Prints all occurrences of pat in txt */
bool DFA_Search ( char *pat, int patsize, char *txt, int txtsize )
{
  // TEST: patsize cannot be negative.
  assert( patsize >= 0 );

  // FIX: Changed to heap allocation to avoid putting
  //      too much pressure to the stack.
  int (*TF)[256];

  TF = xmalloc( (patsize + 1) * sizeof *TF );

  write_tf ( pat, patsize, TF );

  int i = 0, state = 0;

  while ( i < txtsize )
  {
    state = TF[state][ ( unsigned int ) txt[i]];

    if ( state == patsize )
    {
      free( TF );
      return true;
    }

    i++;
  }

  free( TF );

  return false;
}

/*** utility function to return max of two ints ***/
#define max_horspool(a,b) ((a)>(b)?(a):(b))

/*
 * simple Boyer Moore Horspool  http://en.wikipedia.org/wiki/Boyer%E2%80%93Moore%E2%80%93Horspool_algorithm
 *
 * txt_buffer = any text buffer
 * tctLen = length of text buffer
 * match = match string to find
 * matchLen = len of string match
 *
 * */
bool horspool_search ( char *txt, int txtLen, char *match, int matchLen )
{
  assert( txtLen >= 0 && matchLen >= 0 );

  int i;
  int badCharHtable[256];

  // FIX: Fill the array a little bit faster
  for ( i = 0; i < matchLen; i++ )
    badCharHtable[ ( unsigned int ) match[i]] = i;
  for ( ; i < 256; i++ )
    badCharHtable[i] =  -1;

  int shift = 0;

  while ( shift <= txtLen - matchLen )
  {
    int nInd = matchLen - 1;

    while ( ( nInd >= 0 ) && ( match[nInd] == txt[shift + nInd] ) )
      nInd--;

    if ( nInd < 0 )
    {
      return true;

      // FIXME: Unreachable code.
      #if 0
      if ( shift + matchLen < txtLen )
        shift += matchLen - badCharHtable[ ( int ) txt[shift + matchLen]];
      else
        shift += 1;
      #endif
    }
    else
      shift += max_horspool ( 1, nInd - badCharHtable[ ( int ) txt[shift + nInd]] );
  }

  return false;
}

/* Rabin–Karp algorithm - https://en.wikipedia.org/wiki/Rabin%E2%80%93Karp_algorithm
 * input = any text buffer
 * input_len = length of text buffer
 * match = match string to find
 * match_len = len of string match
 *
*/
bool Rabin_Karp_search ( char *input, int input_len, char *match, int match_len )
{
  int var1 = 0, var2 = 0, i, z = 1, sub;

  if ( match_len > 1 )
    z <<= match_len - 1;

  i = 0;
  while ( i < match_len )
  {
    var1 = ( ( var1 << 1 ) + match[i] );
    var2 = ( ( var2 << 1 ) + input[i] );
    i++;
  }

  sub = input_len - match_len;

  for ( i = 0; i <= sub; i++ )
  {
    if ( var1 == var2 && memcmp ( match, input + i, match_len ) == 0 )
      return true;

    var2 = ( ( var2 - input[i] * z ) << 1 ) + input[i + match_len];
  }

  return false;
}

bool pcre_regex_search ( const char *string, int string_len, const char *expression )
{
  const char *err;
  int errofs = 0, offset = 0;
  int ovector[100];

  pcre *re = pcre_compile ( expression, 0, &err, &errofs, NULL );

  if ( re == NULL )
  {
    DEBUG ( " regex compilation failed : %s\n", expression );
    exit ( 0 );
  }

  int rc = pcre_exec ( re, NULL, string, string_len, offset, 0, ovector, 100 );
  pcre_free ( re );

  return rc > 0;
}

/*
  Match with GPU something with NVIDIA's CUDA or OpenCL ? relax at the future i write this... :-D
*/


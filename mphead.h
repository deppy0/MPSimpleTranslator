// For Visual Studio IDE warnings
// disable unsafe warning to be able to use scanf
#define _CRT_SECURE_NO_WARNINGS
// warning for possible data truncation
// ex: size_t to int
#pragma warning(disable:4267)

#include "stdio.h"
#include "string.h"
#include "ctype.h"

//-- Macros for code readability--

// _STR expands the macro once first before STR turns it into a string
#define _STR(nonstr) #nonstr
#define STR(nonstr) _STR(nonstr)

// scanf format for strshrt_t
#define SHORT_SCANF " %"STR(STRING_SHORT)"[^\n]"
#define SHORT_LPRINTF "%-"STR(STRING_SHORT)"s"
#define SHORT_RPRINTF "%"STR(STRING_SHORT)"s"

// scanf format for strlng_t
#define LONG_SCANF " %"STR(STRING_LONG)"[^\n]"
#define LONG_LPRINTF "%-"STR(STRING_SHORT)"s"
#define LONG_RPRINTF "%"STR(STRING_SHORT)"s"

// Color console text by RGB formatting
#define FOREGROUND_RGB(text, r, g, b) "\x1b[38;2;"STR(r)";"STR(g)";"STR(b)"m"text"\x1b[0m"
// Color console text by available ANSI color presets
#define FOREGROUND_COLORSET(text, color_code) "\x1b[38;5;"STR(color_code)"m"text"\x1b[0m"

//-- Limitations --

// Maximum entries
#define LANGENTRY_MAX 150
// Maximum languages per entry
#define LANGPAIR_MAX 10
// Maximum translations per language
#define LANGTRANS_MAX 5

// For entry contents
#define STRING_SHORT 20
// For user input
#define STRING_LONG	100

typedef char strshrt_t[STRING_SHORT + 1];
typedef char strlng_t[STRING_LONG + 1];
typedef struct
{
	strshrt_t language_name, word_list[LANGTRANS_MAX];
	int words;
} langpair_t, *langpair_p;
typedef struct
{
	langpair_t pair_list[LANGPAIR_MAX];
	int pairs, english_index;
} langentry_t, *langentry_p;
typedef struct
{
	langentry_t entry_list[LANGENTRY_MAX];
	int entries;
} langentry_l, *langentry_l_p;
typedef struct
{
	langentry_p results[LANGENTRY_MAX];
	int count;
} langentry_p_l, langentry_p_l_p;
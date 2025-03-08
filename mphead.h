// For Visual Studio IDE warnings
// disable unsafe warning to be able to use scanf
#define _CRT_SECURE_NO_WARNINGS

#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

//-- Type definitions --

// Short string
typedef char strshrt_t[STRING_SHORT + 1];
// Long string
typedef char strlng_t[STRING_LONG + 1];
// Translation pair
typedef struct
{
	strshrt_t language_name, translation_list[LANGTRANS_MAX];
	int translations;
} langpair_t, *langpair_p;
// Translation entry
typedef struct
{
	langpair_t pair_list[LANGPAIR_MAX];
	int pairs, english_index;
} langentry_t, *langentry_p;
// List of translation entries
typedef struct
{
	langentry_t entry_list[LANGENTRY_MAX];
	int entries;
} langentry_l, *langentry_l_p;
// List of pointer addresses to translation entries
// Can be used to take portions of the list
typedef struct
{
	langentry_p ptr_list[LANGENTRY_MAX];
	int count;
} langentry_p_l, *langentry_p_l_p;

// Utilities

void nosymbols(char *, char *);
void nosymbols_o(char *);
void nocapital(char *, char *);
void nocapital_o(char *);

// Program displays

void display_pair(langpair_p);
void display_entry(langentry_l_p, int);
void display_entry_p(langentry_p_l_p, int);
int search_prompt(langentry_l_p, langentry_p_l_p, langpair_p);

// Program methods

void sort_pairs(langentry_p, int);
int search_once(langentry_l_p, strshrt_t, strshrt_t, langpair_p *, int, int);
void search_all(langentry_l_p, strshrt_t, strshrt_t, langpair_p_l_p, int, int);
void configure_entry(langentry_l_p, int *, langpair_p);
void add_entry(langentry_l_p, int *);

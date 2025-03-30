/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying
the concepts learned. I have constructed the functions and their respective algorithms and corresponding  
code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons.           
                                                                                                          
																				LAGUERTA, KURT ANJO A.    
																					 DLSU ID# 12412325    
*********************************************************************************************************/

// For Visual Studio IDE warnings
// disable unsafe warning to be able to use scanf
#define _CRT_SECURE_NO_WARNINGS

#include "assert.h"
#include "conio.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//-- Macros for code readability--

#define FILE_DS_NRL ".ds-nrl"
#define FILENAME_FORBIDDEN "\\/:*?\"<>|"
#define FILENAME_FORBIDDEN_SPACED "\\ / : * ? \" < > |"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// _STR expands the macro once first before STR turns it into a string
#define _STR(nonstr) #nonstr
#define STR(nonstr) _STR(nonstr)

// Color console text by RGB formatting
#define FOREGROUND_RGB(text, r, g, b) "\x1b[38;2;"STR(r)";"STR(g)";"STR(b)"m"text"\x1b[0m"
#define BACKGROUND_RGB(text, r, g, b) "\x1b[48;5;"STR(r)";"STR(g)";"STR(b)"m"text"\x1b[0m"
// Color console text by available ANSI color presets
#define FOREGROUND_COLORSET(text, color_code) "\x1b[38;5;"STR(color_code)"m"text"\x1b[0m"
#define BACKGROUND_COLORSET(text, color_code) "\x1b[48;5;"STR(color_code)"m"text"\x1b[0m"

// Prepared colors to reduce repetitive clutter
#define HIGHLIGHT1 "\x1b[38;5;21m"
#define HIGHLIGHT2 "\x1b[38;5;190m"
#define HIGHLIGHT3 "\x1b[38;5;202m"
#define HIGHLIGHT4 "\x1b[38;5;124m"
#define RESET "\x1b[0m"

// Easy repeats
#define SHOW_CANCEL_MSG printf("\nThe process has been cancelled.\n" \
	"Press any key to go back..."), \
	system("pause>nul"), \
	printf("\n")

#define SHOW_RESTRICTED_INPUT_WARNING \
	printf("\nYou are not allowed to type inputs with the following symbols: | . , ? !" \
		"\nTyping one of these in your input makes the program ask you again for another input.")

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
#define STRING_LONG	150
// For filenames
#define STRING_FILENAME 30

// scanf format for strshrt_t
#define SHORT_SCANF " %"STR(STRING_SHORT)"[^\n]"
#define SHORT_LPRINTF "%-"STR(STRING_SHORT)"s"
#define SHORT_RPRINTF "%"STR(STRING_SHORT)"s"

// scanf format for strlng_t
#define LONG_SCANF " %"STR(STRING_LONG)"[^\n]"
#define LONG_LPRINTF "%-"STR(STRING_LONG)"s"
#define LONG_RPRINTF "%"STR(STRING_LONG)"s"

// scanf format for filename_t
#define FILENAME_SCANF " %"STR(STRING_FILENAME)"[^\n]"
#define FILENAME_LPRINTF "%-"STR(STRING_FILENAME)"s"
#define FILENAME_RPRINTF "%-"STR(STRING_FILENAME)"s"

//-- Type definitions --

// Short string
typedef char strshrt_t[STRING_SHORT + 1];
// Long string
typedef char strlng_t[STRING_LONG + 1];
// Filename string
typedef char filename_t[STRING_FILENAME + 1];
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
	int entries, exported;
} langentry_l, *langentry_l_p;
// Pointer to an entry as a component of langentry_p_l
typedef struct {
	langentry_p ptr;
	int original_index;
} _HPTR_INFO;
// List of pointer addresses to translation entries
// Can be used to take portions of the list
typedef struct
{
	_HPTR_INFO _ptr_info[LANGENTRY_MAX];
	int count, exported;
} langentry_p_l, *langentry_p_l_p;

// Utilities

int upper(char c);
void remove_int(int int_array[], int dead_value, int target_index, int *count);
int valid_filename(char *filename);
char *get_token(char *str, char *delim, int offset);

// Program displays

void display_pair_p(langpair_p pair, int *lines);
void display_one_p(langentry_p_l_p ptr, int index, int *lines);
int display_all_p(langentry_p_l_p ptr);

// Helper functions

void atoz_s(strshrt_t a[], int nElems);
int paircmp(langpair_p pair1, langpair_p pair2);
int validate_s(strshrt_t input);
void restrict_input_s(strshrt_t input);
langentry_p_l get_entry_p_l(langentry_l_p list);
void remove_dead_entries(langentry_l_p list, langentry_p_l_p sorted_contents);
void remove_dead_pairs(langentry_l_p list, langentry_p_l_p sorted_contents);

// Program methods

void insert_data(char *source, langentry_p data, int count);
void refresh_data(char *source, langentry_l_p overwriting_list);
langentry_l read_data(char *source, langentry_p_l_p sorted);
void delete_translation(langpair_p pair, int index);
void add_entry(langentry_l_p list, langpair_t immediate_pair);
int add_pair(langentry_p_l_p filtered_contents, int index);
int add_translation(langpair_p pair);
int nearest_punctuation(char **words, char *punctuations, char **punctuation_found, int start_index, int word_count);
langpair_p get_first_pair_p(langentry_p_l_p pointered_contents, int from_index, strshrt_t language);
void translate(strlng_t text, strshrt_t text_language, strshrt_t language_target, langentry_p_l_p sorted_contents, char **translation);

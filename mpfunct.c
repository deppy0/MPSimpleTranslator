#include "mphead.h"

int find_first(char *str, char *key, int bLastChar)
{
	int i, j, m, ls, lk, r;
	ls = strlen(str);
	lk = strlen(key);
	i = j = m = 0;
	while (m != lk && i < ls)
	{
		if (str[i] == key[j])
			m++;
		else
			m = 0, j = -1;
		if (m != lk)
		{
			i++;
			if (j >= lk)
				j = 0;
			else
				j++;
		}
	}
	if (bLastChar)
		r = i;
	else
		r = i - lk + 1;
	return r;
}

int find_last(char *str, char *key, int bLastChar)
{
	int i, j, m, ls, lk, r;
	ls = strlen(str);
	lk = strlen(key);
	i = ls - 1;
	j = lk - 1;
	m = 0;
	while (m != lk && i > 0)
	{
		if (str[i] == key[j])
			m++;
		else
		{
			m = 0;
			j = lk;
		}
		if (m != lk)
		{
			i--;
			if (j <= 0)
				j = lk - 1;
			else
				j--;
		}
	}
	if (bLastChar)
		r = i + lk - 1;
	else
		r = i;
	return r;
}

void nosymbols(strshrt_t str, strshrt_t result)
{
	int i, c, l;
	for (i = 0, c = 0, l = strlen(str); i < l; i++)
		if ((str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= '0' && str[i] <= '9') || str[i] == ' ')
			result[c++] = str[i];
	result[c] = 0;
}

void nocapital(char *str, char *result)
{
	int i, l;
	for (i = 0, l = strlen(str); i < l; i++)
		result[i] = tolower(str[i]);
	result[i] = 0;
}

void display_pair(langpair_p pair)
{
	int i;
	printf(FOREGROUND_COLORSET("\t" SHORT_LPRINTF, 220), pair->language_name);
	for (i = 0; i < pair->words; i++)
	{
		printf(FOREGROUND_COLORSET("%s", 81), pair->word_list[i]);
		if (i + 1 < pair->words)
			printf(FOREGROUND_COLORSET(", ", 81));
	}
	printf("\n");
}

void display_entry(langentry_l_p list, int index)
{
	int i;
	langentry_p e;
	if (index < list->entries)
	{
		e = &list->entry_list[index];
		printf(FOREGROUND_COLORSET("\n#%d", 255), index + 1);
		for (i = 0; i < e->pairs; i++)
			display_pair(&e->pair_list[i]);
	}
}

void sort_pairs(langentry_p list, int isZtoA)
{
	int i, j, c;
	langpair_t t;
	langpair_p m;
	m = &list->pair_list[0];
	if (!isZtoA)
		c = strcmp(m->language_name, list->pair_list[list->pairs - 1].language_name) > 0;
	else
		c = strcmp(m->language_name, list->pair_list[list->pairs - 1].language_name) < 0;
	if (c)
		m = &list->pair_list[list->pairs - 1];
	t = list->pair_list[0];
	list->pair_list[0] = *m;
	*m = t;
	for (i = 0; i < list->pairs; i++)
	{
		m = &list->pair_list[i];
		for (j = i + 1; j < list->pairs; j++)
		{
			if (!isZtoA)
				c = strcmp(m->language_name, list->pair_list[j].language_name) > 0;
			else
				c = strcmp(m->language_name, list->pair_list[j].language_name) < 0;
			if (c)
				m = &list->pair_list[j];
		}
		t = list->pair_list[i];
		list->pair_list[i] = *m;
		*m = t;
	}
	printf("wow");
}

/* Searches the list for an existing language pair or word.
	@param list - The list of data to search in.
	@param language -
		Optional. The language of the target language pair.
		Can be set to NULL if search_word is 1.
	@param word -
		Required. One of the words of the target language pair.
	@param result_ptr -
		Required. The address of the language pair that's first to be found will be stored here.
	@param start_index -
		Required. The zero-based index of the dataset where the search would start.
		The function immediately ends on invalid values, i.e. < 0 or >= no. of entries.
	@param search_word -
		Boolean. This function searches for a language pair by default.
		If this variable is set to 1, searches for the string contained in
		variable 'word' regardless of the language that corresponds to it.
	@returns
		An integer representing the zero-based index of the entry where the occurrence was found.
		Meanwhile, returns -1 if no match was found, and -2 if there is an error on one of the parameters.
*/
int search_once(langentry_l_p list, strshrt_t language, strshrt_t word, langpair_p result_ptr, int start_index, int search_word)
{
	int i, j, k, f;
	strshrt_t _language, _word;
	langentry_p e;
	langpair_p p;
	if ((list == NULL || list->entries == 0) || (start_index < 0 || start_index >= list->entries))
		f = -2;
	// If all is valid, start the process
	else
	{
		f = 0;
		// Force lowercase
		// Only perform nocapital on language input if search_word is 0.
		if (!search_word)
			nocapital(language, _language);
		nocapital(word, _word);
		// Iterate through entries
		for (i = 0, f = -1; f == -1 && i < list->entries; i++)
		{
			e = &list->entry_list[i];
			// Iterate through pairs
			for (j = 0; f == -1 && j < e->pairs; j++)
			{
				p = &e->pair_list[j];
				// Iterate through words
				for (k = 0; f == -1 && k < p->words; k++)
				{
					// If word matches, perform according to value of 'search_word'
					if (!strcmp(p->word_list[k], _word))
					{
						// If search_word is 1, 
						// then it is already found regardless of language name
						if (search_word)
						{
							result_ptr = p;
							f = i;
						}
						else
						{
							// Otherwise, the language name has to be checked too
							if (!strcmp(p->language_name, _language))
							{
								result_ptr = p;
								f = i;
							}
						}
					}
				}
			}
		}
	}
	if (f == -1) // Not found
		result_ptr = NULL;
	return f;
}

langentry_p_l search_all(langentry_l_p list, strshrt_t language, strshrt_t word, int *result, int start_index, int search_word)
{

}

//void search_language_pair(language_p pair, translation_l_p list)
//{
//	int i, j, k;
//	strshrt_t t1, t2;
//	language_t l;
//	for (i = 0; i < list->count; i++)
//	{
//		for (j = 0; j < list->entries[i].pairs.count; j++)
//		{
//			l = list->entries[i].pairs.languages[j];
//			if (!strcmp(l.name, pair->name))
//				
//		}
//	}
//}
//
//void new_language_pair(language_p pair)
//{
//	strshrt_t t = { 0 };
//	int i = 0, s = 0;
//
//	printf("Enter the language name, or "FOREGROUND_COLORSET("!!", 196)" to cancel.\n");
//	printf("\nAny special characters entered will be skipped.");
//	printf("\nFor example, 'w^i&nd*ow' will be treated as 'window'.\n\n");
//	printf("Language name: ");
//	scanf(SHORT_SCANF, t);
//	if (!strcmp(t, "!!"))
//	{
//		nosymbols(t, pair->name);
//		pair->count = 0;
//		printf("\nYou can enter up to 5 translations for the entry.");
//		printf("\nEnter "FOREGROUND_COLORSET("!!", 196)" to finish input below 5.");
//		printf("\nAny special characters entered will be skipped.\n\n");
//		do
//		{
//			printf("Enter translation %d: ", pair->count + 1);
//			scanf(SHORT_SCANF, t);
//			if (!strcmp(t, "!!"))
//				s = 1;
//			else
//				nosymbols(t, pair->equivalents[pair->count++]);
//		} while (!s && pair->count < LANGPAIR_MAX);
//	}
//
//	/*
//	printf("\n%-20s", pair->name);
//	for (i = 0; i < pair->count; i++)
//	{
//		printf("%s", pair->equivalents[i]);
//		if (i + 1 < pair->count)
//			printf(", ");
//	}
//	printf("\n");
//	*/
//}


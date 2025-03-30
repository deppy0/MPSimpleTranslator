/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying
the concepts learned. I have constructed the functions and their respective algorithms and corresponding  
code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons.           
                                                                                                          
																				LAGUERTA, KURT ANJO A.    
																					 DLSU ID# 12412325    
*********************************************************************************************************/

#include "mphead.h"

/// <summary>
/// Returns the upper case form of that letter of the alphabet, or the same character
/// if that character does not support capitalization.
/// </summary>
/// <param name="c"> - The character to get the uppercase form.</param>
/// <returns>ASCII value of the uppercase form of the character</returns>
int upper(char c) { if (c >= 97 && c <= 122) c -= 32; return c; }

/// <summary>
/// "Deletes" an integer from the array, i.e. moving it to the last part and replacing it with a dead value.
/// </summary>
/// <param name="int_array"> - Array to modify.</param>
/// <param name="dead_value"> - Value that represents deletion.</param>
/// <param name="target_index"> - Index to "delete".</param>
/// <param name="count"> - Number of elements in the array to also be updated post-deletion</param>
void remove_int(int int_array[], int dead_value, int target_index, int *count)
{
	int i, temp;
	int_array[target_index] = dead_value;
	for (i = target_index; i < *count - 1; i++)
		temp = int_array[i], int_array[i] = int_array[i + 1], int_array[i + 1] = temp;
	(*count)--;
}

/// <summary>
/// Validates a filename.
/// </summary>
/// <param name="filename">Filename to check if valid.</param>
/// <returns>A boolean: 1 if valid, 0 if not.</returns>
int valid_filename(char *filename)
{
	int i, j, l1, l2, good;
	for (i = 0, l1 = strlen(filename), good = 1; good && i < l1; i++)
		for (j = 0, l2 = strlen(FILENAME_FORBIDDEN); good && j < l2; j++)
			good = filename[i] != FILENAME_FORBIDDEN[j];
	return good;
}

/// <summary>
/// Sorts an array of strings according to the ASCII codes of their characters.
/// </summary>
/// <param name="a"> - Array containing the strings to arrange.</param>
/// <param name="nElems"> - Number of elements to sort inside the array starting from the first element.</param>
void atoz_s(strshrt_t a[], int nElems)
{
	int i, j;
	strshrt_t *min, *next, *index, temp;
	
	// Throw an error if one of these conditions are not met
	assert(a != NULL);
	assert(nElems > 0);

	min = &a[0], next = &a[nElems - 1];
	if (strcmp(*min, *next) > 0)
		strcpy(temp, *min), strcpy(*min, *next), strcpy(*next, temp);

	for (i = 0; i < nElems; i++)
	{
		min = index = &a[i];
		for (j = i + 1; j < nElems; j++)
		{
			next = &a[j];
			if (strcmp(*min, *next) > 0)
				min = next;
		}
		if (min != index)
			strcpy(temp, *index), strcpy(*index, *min), strcpy(*min, *next), strcpy(*next, temp);
	}
	
}

/// <summary>
/// Finds a short string in an array of short strings.
/// </summary>
/// <param name="a"> - Array to check.</param>
/// <param name="nElems"> - Number of elements within the array.</param>
/// <param name="key"> - The short string to search.</param>
/// <returns>The index of the matching short string.</returns>
int lookup_s(strshrt_t a[], int nElems, strshrt_t key)
{
	int i, f;
	for (i = 0, f = -1; f == -1 && i < nElems; i++)
		if (strcmp(a[i], key) == 0)
			f = i;
	return f;
}

/// <summary>
/// Displays a translation pair inside an entry.
/// </summary>
/// <param name="pair"> - Pointer to the translation pair to display.</param>
void display_pair_p(langpair_p pair, int *lines)
{
	int i, line_dump = 0;
	if (lines == NULL)
		lines = &line_dump;
	printf(FOREGROUND_COLORSET("\t" SHORT_LPRINTF, 220), pair->language_name);
	for (i = 0; i < pair->translations; i++)
	{
		printf(FOREGROUND_COLORSET("%s", 81), pair->translation_list[i]);
		if (i + 1 < pair->translations)
			printf(FOREGROUND_COLORSET(", ", 81));
	}
	printf("\n"), (*lines)++;
}

/// <summary>
/// Displays a whole entry with all of its translation pairs.
/// </summary>
/// <param name="ptr"> - The list structure containing all the pointers of each entry in the dataset.</param>
/// <param name="index"> - The index of the entry inside the list structure for displaying.</param>
void display_one_p(langentry_p_l_p ptr, int index, int *lines)
{
	int i, line_dump = 0;
	langentry_p e;
	if (lines == NULL)
		lines = &line_dump;
	if (index >= 0 && index < ptr->count)
	{
		e = ptr->_ptr_info[index].ptr;
		printf(FOREGROUND_COLORSET("\nEntry #%d\n", 255), ptr->_ptr_info[index].original_index + 1), *lines += 2;
		for (i = 0; i < e->pairs; i++)
			display_pair_p(&e->pair_list[i], lines);
		for (i = 0; i < LANGPAIR_MAX - e->pairs; i++)
			printf("\n"), (*lines)++;
		(*lines)++;
	}
}

/// <summary>
/// Displays all entries one by one within the dataset.
/// </summary>
/// <param name="ptr"> - The list structure containing all the pointers of each entry in the dataset.</param>
/// <returns>The zero-based index where the user pressed '/' to end the display.</returns>
int display_all_p(langentry_p_l_p ptr)
{
	int i, lines;
	char ch;
	i = 0, lines = 0, ch = 0;
	printf("\n");
	printf("The order of entries shown through this display is sorted in alphabetical order by language.\n");
	printf("Therefore, the entry number only indicates which entry is added first, and may not necessarily be in ascending order.\n");
	do
	{
		for (; lines-- > 0;)
			printf("\x1b[2K\x1b[1A");
		display_one_p(ptr, i, &lines);
		if (i > 0)
			printf("(,) %8s ", "Previous");
		else
			printf("(,) %8s ", "Last");
		printf("| %d of %d |", i + 1, ptr->count);
		if (i + 1 < ptr->count)
			printf(" (.) %-8s ", "Next");
		else
			printf(" (.) %-8s ", "First");
		printf("\n\nPress any of the options or press '/' to end the display... "), lines += 2;
		if ((ch = _getch()) != '\n')
			printf("%c\n", ch), lines++;
		if (ch == ',')
			if (i > 0)
				i--;
			else
				i = ptr->count - 1;
		else if (ch == '.')
			if (i + 1 < ptr->count)
				i++;
			else
				i = 0;
		else;
	} while (i < ptr->count && ch != '/');
	printf("\n");
	return i;
}

/// <summary>
/// Compares two translation pairs by their language name.
/// In the case that the two pairs contain the same set of languages, the translations contained inside the
/// languages would also be compared.
/// </summary>
/// <param name="pair1"> - The translation pair to the left side of the supposed inequality.</param>
/// <param name="pair2"> - The translation pair to the right side of the supposed inequality.</param>
/// <returns></returns>
int paircmp(langpair_p pair1, langpair_p pair2)
{
	int i, j, comparison = 0;
	comparison = strcmp(pair1->language_name, pair2->language_name);
	for (i = 0; comparison == 0 && i < pair1->translations; i++)
		for (j = 0; comparison == 0 && j < pair2->translations; j++)
			comparison = strcmp(pair1->translation_list[i], pair2->translation_list[j]);
	return comparison;
}

/// <summary>
/// Sorts all the entries within the dataset along with their pairs and translations.
/// </summary>
/// <param name="list"> - The unsorted list structure.</param>
/// <param name="sorted_list">
/// - Output variable to store the sorted version of the list.
/// Note that the sorted list now contains the pointers to the unsorted list, except the arrangement of the
/// pointers is now sorted accordingly.
/// </param>
void list_sort(langentry_l_p list, langentry_p_l_p sorted_list)
{
	// Declarations
	int i, j, k, eng_cnt, _paircmp;
	langentry_p entry;
	langpair_t *pair, *pr_min, *pr_next, pr_temp;
	_HPTR_INFO *hpi, *hpi_min, *hpi_next, hpi_temp;
	// Pair sort
	for (i = 0; i < list->entries; i++)
	{
		entry = &list->entry_list[i];
		// Sort translations
		for (j = 0; j < entry->pairs; j++)
			pair = &entry->pair_list[j],
			atoz_s(pair->translation_list, pair->translations);
		// Sort pairs, skip English if any
		pr_min = &entry->pair_list[0];
		pair = &entry->pair_list[entry->pairs - 1];
		if (paircmp(pr_min, pair) > 0)
			pr_temp = *pr_min, *pr_min = *pair, *pair = pr_temp;
		for (j = 0; j < entry->pairs; j++)
		{
			pair = pr_min = &entry->pair_list[j];
			for (k = j + 1; k < entry->pairs; k++)
			{
				pr_next = &entry->pair_list[k];
				if (paircmp(pr_min, pr_next) > 0)
					pr_min = pr_next;
			}
			if (pr_min != pair)
				pr_temp = *pair, *pair = *pr_min, *pr_min = *pr_next, *pr_next = pr_temp;
		}
		// Update the English index
		entry->english_index = -1;
		for (j = 0; entry->english_index == -1 && j < entry->pairs; j++)
			if (strcmp(entry->pair_list[j].language_name, "English") == 0)
				entry->english_index = j;
	}
	// Assign original indices prior to sorting
	// Also updates count for output struct
	for (sorted_list->count = 0; sorted_list->count < list->entries; sorted_list->count++)
		sorted_list->_ptr_info[sorted_list->count].original_index = sorted_list->count,
		sorted_list->_ptr_info[sorted_list->count].ptr = &list->entry_list[sorted_list->count];
	// Put English entries on top
	eng_cnt = 0;
	for (i = 0; i < sorted_list->count; i++)
		if ((hpi = &sorted_list->_ptr_info[i], hpi->ptr->english_index) != -1)
			hpi_temp = sorted_list->_ptr_info[eng_cnt], sorted_list->_ptr_info[eng_cnt++] = *hpi, *hpi = hpi_temp;
	// Sort the entries from here
	// English entries are sorted depending on the first translation from the sorted translation set.
	// In the least common case that the translations are the same, then they will be sorted by their unsorted index.
	if (eng_cnt > 0 && sorted_list->count > 1)
	{
		hpi_min = &sorted_list->_ptr_info[0], hpi_next = &sorted_list->_ptr_info[eng_cnt - 1];
		pr_min = &hpi_min->ptr->pair_list[hpi_min->ptr->english_index], pr_next = &hpi_next->ptr->pair_list[hpi_next->ptr->english_index];
		if ((_paircmp = paircmp(pr_min, pr_next)) > 0)
			hpi_temp = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp;
		else if (_paircmp == 0)
			if (hpi_min->original_index > hpi_next->original_index)
				hpi_temp = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp; else; else;
		for (i = 0; i < eng_cnt; i++)
		{
			hpi = hpi_min = &sorted_list->_ptr_info[i];
			pr_min = &hpi_min->ptr->pair_list[hpi_min->ptr->english_index];
			for (j = i + 1; j < eng_cnt; j++)
			{
				hpi_next = &sorted_list->_ptr_info[j];
				pr_next = &hpi_next->ptr->pair_list[hpi_next->ptr->english_index];
				if ((_paircmp = paircmp(pr_min, pr_next)) > 0)
					hpi_min = hpi_next, pr_min = pr_next;
				else if (_paircmp == 0)
					if (hpi_min->original_index > hpi_next->original_index)
						hpi_min = hpi_next, pr_min = pr_next; else; else;
			}
			if (hpi_min != hpi)
				hpi_temp = *hpi, *hpi = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp; else;
		}
	}
	// Non-English entries are sorted depending on the first translation of the first language, since the pairs are sorted already.
	// Again, in the least common case that both of these are the same word, then they will be sorted by their unsorted index.
	if (eng_cnt != sorted_list->count && sorted_list->count > 1)
	{
		hpi_min = &sorted_list->_ptr_info[eng_cnt], hpi_next = &sorted_list->_ptr_info[sorted_list->count - 1];
		pr_min = &hpi_min->ptr->pair_list[0], pr_next = &hpi_next->ptr->pair_list[0];
		if ((_paircmp = paircmp(pr_min, pr_next)) > 0)
			hpi_temp = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp;
		else if (_paircmp == 0)
			if (hpi_min->original_index > hpi_next->original_index)
				hpi_temp = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp; else; else;
		for (i = eng_cnt; i < sorted_list->count; i++)
		{
			hpi = hpi_min = &sorted_list->_ptr_info[i];
			pr_min = &hpi_min->ptr->pair_list[0];
			for (j = i + 1; j < eng_cnt; j++)
			{
				hpi_next = &sorted_list->_ptr_info[j];
				pr_next = &hpi_next->ptr->pair_list[0];
				if ((_paircmp = paircmp(pr_min, pr_next)) > 0)
					hpi_min = hpi_next, pr_min = pr_next;
				else if ((_paircmp == 0))
					if (hpi_min->original_index > hpi_next->original_index)
						hpi_min = hpi_next, pr_min = pr_next; else; else;
			}
			if (hpi_min != hpi)
				hpi_temp = *hpi, *hpi = *hpi_min, *hpi_min = *hpi_next, *hpi_next = hpi_temp; else;
		}
	}
}

/// <summary>
/// Searches along the dataset strictly through a language and a translation.
/// </summary>
/// <param name="sorted_list"> - Pointer to the sorted list structure.</param>
/// <param name="key_language"> - The language of the translation pair to search.</param>
/// <param name="key_translation"> - One of the translations of the translation pair to search.</param>
/// <returns>A filtered list containing those pointers from the sorted list structure that matched the key language and translation.</returns>
langentry_p_l _strict_search(langentry_p_l_p sorted_list, strshrt_t key_language, strshrt_t key_translation)
{
	int i, j, k, eng, next;
	langentry_p_l filtered = { 0 };
	_HPTR_INFO *hpi;
	eng = strcmp(key_language, "English") == 0;
	for (i = 0; i < sorted_list->count; i++)
	{
		hpi = &sorted_list->_ptr_info[i];
		if (eng && hpi->ptr->english_index != -1)
			j = hpi->ptr->english_index; else j = 0;
		for (next = 0; !next && j < hpi->ptr->pairs; j++)
			if (strcmp(hpi->ptr->pair_list[j].language_name, key_language) == 0)
				for (k = 0; !next && k < hpi->ptr->pair_list[j].translations; k++)
					if (strcmp(hpi->ptr->pair_list[j].translation_list[k], key_translation) == 0)
						filtered._ptr_info[filtered.count++] = *hpi, next = 1; else; else;
	}
	return filtered;
}

/// <summary>
/// Searches along the dataset with only a translation from any corresponding language.
/// </summary>
/// <param name="sorted_list"> - Pointer to the sorted list structure.</param>
/// <param name="key_translation"> - The translation to search.</param>
/// <returns>A filtered list containing those pointers from the sorted list structure that matched the key translation.</returns>
langentry_p_l _light_search(langentry_p_l_p sorted_list, strshrt_t key_translation)
{
	int i, j, k, next;
	langentry_p_l filtered = { 0 };
	_HPTR_INFO *hpi;
	for (i = 0; i < sorted_list->count; i++)
		for (j = 0, next = 0, hpi = &sorted_list->_ptr_info[i]; !next && j < hpi->ptr->pairs; j++)
			for (k = 0; !next && k < hpi->ptr->pair_list[j].translations; k++)
				if (strcmp(hpi->ptr->pair_list[j].translation_list[k], key_translation) == 0)
					filtered._ptr_info[filtered.count++] = *hpi, next = 1; else;
	return filtered;
}

/// <summary>
/// Filters the sorted list to leave only entries that contain the given language.
/// </summary>
/// <param name="sorted_list"> - The list to search the language within.</param>
/// <param name="key_language"> - The language to search for.</param>
/// <returns>A new list containing pointers to the entries containing the language.</returns>
langentry_p_l _light_search_by_language(langentry_p_l_p sorted_list, strshrt_t key_language)
{
	int i, j, next, english;
	langentry_p_l filtered = { 0 };
	_HPTR_INFO *hpi;
	english = strcmp(key_language, "English") == 0;
	for (i = 0; i < sorted_list->count; i++)
		for (j = 0, next = 0, hpi = &sorted_list->_ptr_info[i]; !next && j < hpi->ptr->pairs; j++)
			if (english && hpi->ptr->english_index != -1)
				filtered._ptr_info[filtered.count++] = *hpi, next = 1;
			else if (!english)
				if (strcmp(hpi->ptr->pair_list[j].language_name, key_language) == 0)
					filtered._ptr_info[filtered.count++] = *hpi, next = 1; else;
			else;
	return filtered;
}

/// <summary>
/// Filters the sorted list to leave only entries in which the word occurs 
/// in between translations of a language.
/// </summary>
/// <param name="sorted_list"> - The list to search the word within.</param>
/// <param name="language"> - The language to search the corresponding word within.</param>
/// <param name="word"> - String representing the translation or a part of the translation.</param>
/// <returns>
/// A new list of pointers to the entries where the word occurred along the corresponding language.
/// Note that the word should be found at the start and not in between.
/// Searching "man" will not detect "wise man", but "wise" would.
/// </returns>
langentry_p_l _strict_search_by_occurrence(langentry_p_l_p sorted_list, strshrt_t language, strshrt_t word)
{
	int i, j, k, next;
	langentry_p_l filtered = { 0 };
	_HPTR_INFO *hpi;
	for (i = 0; i < sorted_list->count; i++)
		for (j = 0, next = 0, hpi = &sorted_list->_ptr_info[i]; !next && j < hpi->ptr->pairs; j++)
			if (strcmp(hpi->ptr->pair_list[j].language_name, language) == 0)
				for (k = 0; !next && k < hpi->ptr->pair_list[j].translations; k++)
					if (strstr(hpi->ptr->pair_list[j].translation_list[k], word) == &hpi->ptr->pair_list[j].translation_list[k][0])
						filtered._ptr_info[filtered.count++] = *hpi, next = 1; else; else;
	return filtered;
}

/// <summary>
/// Validates the user input to not contain the following symbols: | . , ? !
/// </summary>
/// <param name="input"> - The user's input.</param>
/// <returns>A boolean: 1 if valid, 0 if not.</returns>
int validate_s(strshrt_t input)
{
	int i, l, v;
	for (i = 0, l = strlen(input), v = 1; v && i < l; i++)
		v = input[i] != '|' && input[i] != '.' && input[i] != ',' && input[i] != '?' && input[i] != '!';
	return v;
}

/// <summary>
/// Repeatedly asks the user for input if their input contains the symbols | . , ? or !
/// </summary>
/// <param name="input"> - String where to store the user's input in case valid.</param>
void restrict_input_s(strshrt_t input)
{
	assert(scanf(SHORT_SCANF, input) == 1);
	if (strcmp(input, "!!") != 0)
		while (!validate_s(input))
			strcpy(input, ""),
			assert(scanf(SHORT_SCANF, input) == 1);
}

/// <summary>
/// Gathers inputs from the user regarding their desired search.
/// </summary>
/// <param name="sorted_entries"> - Pointer to the sorted list structure.</param>
/// <param name="filtered_output"> - Output variable to store the filtered version of the sorted list.</param>
/// <param name="immediate_pair">
/// - In case this function is used for the Add Entry option, the immediate pair would be the first pair of the new entry.
/// Beforehand, the user is warned of duplicates in case there are matches from the search.
/// </param>
/// <param name="force_strict">
/// - Setting this to 1 forces a strict search with the immediate pair.
/// Setting this to 0, only the translation is required for the search and the language would be optional.</param>
/// <returns>
/// Returns -1 if the input was cancelled. Otherwise, returns the number of matches from the requested search.
/// </returns>
int search_input(langentry_p_l_p sorted_entries, langentry_p_l_p filtered_output, langpair_p immediate_pair, int force_strict)
{
	// Declarations
	int ret = 0;
	strshrt_t language = { 0 }, translation = { 0 };
	// Process
	printf("Search along the existing entries by typing requested information below.");
	SHOW_RESTRICTED_INPUT_WARNING;
	printf("\nNote that searching accepts only 20 characters and is case-insensitive.\n\n");
	// Search language
	printf("Enter a translation from any language: ");
	restrict_input_s(translation);
	if (strcmp(translation, "!!") == 0)
		ret = -1;
	//lowerall_o(translation);
	if (ret != -1)
	{
		if (force_strict)
		{
			printf("\nEnter a language: ");
			restrict_input_s(language);
			if (strcmp(language, "!!") == 0)
				ret = -1;
			//upperfirst_o(language);
			if (ret != -1)
				*filtered_output = _strict_search(sorted_entries, language, translation);
		}
		else
		{
			printf("Press 'Y' if you want to make the search more particular by searching a language as well... ");
			if (upper(_getch()) == 'Y')
			{
				printf("\nEnter a language: ");
				restrict_input_s(language);
				if (strcmp(language, "!!") == 0)
					ret = -1;
				//upperfirst_o(language);
				if (ret != -1)
					*filtered_output = _strict_search(sorted_entries, language, translation);
			}
			else
				*filtered_output = _light_search(sorted_entries, translation);
		}
	}
	if (ret != -1)
	{
		ret = filtered_output->count;
		immediate_pair->translations = 1;
		strcpy(immediate_pair->language_name, language);
		strcpy(immediate_pair->translation_list[0], translation);
	}
	return ret;
}

/// <summary>
/// Gets the word from a string delimited by a substring.
/// </summary>
/// <param name="str"> - The string to look for tokens in.</param>
/// <param name="delim"> - The substring that divides the string.</param>
/// <param name="offset"> - Distance from the first token, assuming the string starts with the delimiter.</param>
/// <returns>The token in the given offset.</returns>
char *get_token(char *str, char *delim, int offset)
{
	// Declarations
	int i;
	long long l;
	char *result, *token1, *token2;
	ptrdiff_t distance;
	// Process
	assert(str != NULL);
	token1 = str;
	l = strlen(delim);
	for (i = 0; token1 != NULL && i <= offset; i++)
		if ((token1 = strstr(token1, delim)) != NULL)
			token1 += l;
	if (token1 != NULL)
	{
		if (strlen(token1) > 0)
		{
			token2 = strstr(token1, delim);
			if (token2 != NULL)
				distance = token2 - token1;
			else
				distance = strlen(token1);
			result = malloc(distance + 1);
			assert(result != NULL);
			strncpy(result, token1, distance);
			result[distance] = 0;
		}
		else
		{
			result = malloc(1);
			assert(result != NULL);
			result[0] = 0;
		}
	}
	else
	{
		result = malloc(1);
		assert(result != NULL);
		result[0] = 0;
	}
	return result;
}

/// <summary>
/// Gets the pointer list form of an entry list.
/// </summary>
/// <param name="list"> - List containing the entries.</param>
/// <returns>A list of pointers to the same order of entries.</returns>
langentry_p_l get_entry_p_l(langentry_l_p list)
{
	langentry_p_l p = { 0 };
	for (p.count = 0; p.count < list->entries; p.count++)
		p._ptr_info[p.count].ptr = &list->entry_list[p.count],
		p._ptr_info[p.count].original_index = p.count;
	return p;
}

/// <summary>
/// Inserts data into the source file.
/// </summary>
/// <param name="source"> - Source file to write the binary data to.</param>
/// <param name="data"> - Entry to store in the source file.</param>
/// <param name="count"> - Index of the entry</param>
void insert_data(char *source, langentry_p data, int count)
{
	// Declarations
	FILE *f;
	fpos_t pos;
	langpair_p p;
	char delim[5] = { 0 };
	int i, j, c, iter1, iter2;
	for (i = 1, j = 1, c = count; c-- > 0;)
		if (j == 0x06) j = 1, i++; else j++;
	delim[0] = 0x01;
	delim[1] = i + 0x20;
	delim[2] = j;
	delim[3] = 0x04;
	delim[4] = 0x00;
	// Open file for append
	f = fopen(source, "ab");
	assert(f != NULL);
	// If first entry, add delimiter
	if ((fgetpos(f, &pos), pos) == 0)
		fprintf(f, "%s", delim);
	fprintf(f, "\x05%d%s", data->english_index, delim);
	fprintf(f, "\x10%d%s", data->pairs, delim);
	for (iter1 = 0; iter1 < data->pairs; iter1++)
	{
		p = &data->pair_list[iter1];
		fprintf(f, "\x0C%d%s%s", iter1, p->language_name, delim);
		fprintf(f, "\x14%d-%d%s", iter1, p->translations, delim);
		for (iter2 = 0; iter2 < p->translations; iter2++)
			fprintf(f, "\x14%d.%d%s%s", iter1, iter2, p->translation_list[iter2], delim);
	}
	fprintf(f, "\x17");
	fclose(f);
}

/// <summary>
/// Reloads the contents of the source file.
/// </summary>
/// <param name="source"> - Source file to put the data in.</param>
/// <param name="overwriting_list"> - List to get the data to put in the source file.</param>
void refresh_data(char *source, langentry_l_p overwriting_list)
{
	// Declarations
	FILE *f;
	int i;
	// Overwrite source
	// Open the source for writing then check if NULL
	// Safer way than deleting, in case 
	f = fopen(source, "wb");
	if (f != NULL)
	{
		fclose(f);
		for (i = 0; i < overwriting_list->entries; i++)
			insert_data(source, &overwriting_list->entry_list[i], i);
	}
}

/// <summary>
/// Reads the contents from the source file.
/// </summary>
/// <param name="source"> - Source file to read the data from.</param>
/// <param name="sorted"> - List to put the sorted version of the data.</param>
/// <returns>The list of entries found within the source file, ordered accordingly to how it is written.</returns>
langentry_l read_data(char *source, langentry_p_l_p sorted)
{
	// Declarations
	langentry_p n;
	langentry_l out = { 0 };
	FILE *f;
	fpos_t fs;
	int i, j, k, stop_read, stop_entry;
	char *content, *token, delim[5] = { 0 };
	// Open file for binary reading
	f = fopen(source, "rb");
	if (f != NULL)
	{
		// Set cursor to EOF and get cursor position to get file size
		fseek(f, 0, SEEK_END);
		fgetpos(f, &fs);
		// Move cursor back to beginning
		rewind(f);
		// Allocate enough memory to store file contents
		content = malloc(fs + 1);
		assert(content != NULL);
		// Read the contents and add NUL byte
		fread(content, 1, fs, f);
		content[fs] = 0;
		// Read binary file
		for (out.entries = 0, stop_read = 0, i = 1, j = 1, k = 0;
			!stop_read && out.entries < LANGENTRY_MAX; out.entries++)
		{
			n = &out.entry_list[out.entries];
			// Delimiter is different for each entry
			delim[0] = 0x01;
			delim[1] = i + 0x20;
			delim[2] = j;
			delim[3] = 0x04;
			delim[4] = 0x00;
			// Reset variables for entry
			stop_entry = 0;
			k = 0;
			// Get current token
			token = get_token(content, delim, k);
			while (!stop_entry)
			{
				// ASCII code is based on the nth letter of the alphabet
				switch (token[0])
				{
					case 5: // E
						n->english_index = atoi(token + 1);
						break;
					case 16: // P
						n->pairs = atoi(token + 1);
						break;
					case 12: // L
						strcpy(n->pair_list[token[1] - 48].language_name, token + 2);
						break;
					case 20: // T
						switch (token[2])
						{
							case '-':
								n->pair_list[token[1] - 48].translations = atoi(token + 3);
								break;
							case '.':
								strcpy(n->pair_list[token[1] - 48].translation_list[token[3] - 48], token + 4);
								break;
						}
						break;
					case 23: // control character ETB serves as end of entry
						stop_entry = 1;
						break;
				}
				// Continue tokenizing while there is one
				// ...or if ETB is encountered
				if (strlen(token) > 0)
					if (!stop_entry)
						free(token), token = get_token(content, delim, k++);
					else
						free(token);
				else
					stop_entry = 1, stop_read = 1, out.entries--;
			}
			// Only increment if reading continues
			if (!stop_read)
			{
				assert(j <= 0x06);
				if (j == 0x06) j = 1, i++; else j++;
			}
		}
		fclose(f);
		free(content);
		list_sort(&out, sorted);
	}
	else
	{
		// If data file doesn't exist yet, simply create it blank
		f = fopen(source, "wb");
		fclose(f);
		out.entries = 0;
	}
	return out;
}

/// <summary>
/// Deletes a translation from a pair within an entry.
/// </summary>
/// <param name="pair"> - Pointer to the target pair containing translations.</param>
/// <param name="index"> - Zero-based index of the translation to delete.</param>
void delete_translation(langpair_p pair, int index)
{
	int i;
	strshrt_t temp;
	memset(pair->translation_list[index], 0, sizeof(strshrt_t));
	for (i = index; i < pair->translations - 1; i++)
		strcpy(temp, pair->translation_list[i]),
		strcpy(pair->translation_list[i], pair->translation_list[i + 1]),
		strcpy(pair->translation_list[i + 1], temp);
	pair->translations--;
}

/// <summary>
/// Shifts empty or dead entries to the last of the entry list to maintain its integrity.
/// </summary>
/// <param name="list"> - The list containing dead entries.</param>
/// <param name="sorted_contents"> - List where to store the sorted form of the cleaned list.</param>
void remove_dead_entries(langentry_l_p list, langentry_p_l_p sorted_contents)
{
	int i, j;
	langentry_t *dead, *exist, temp;
	for (i = 0; i < list->entries; i++)
	{
		if (list->entry_list[i].pairs == 0)
		{
			for (j = i; j < list->entries - 1; j++)
				dead = &list->entry_list[j], exist = &list->entry_list[j + 1],
				temp = *dead, *dead = *exist, *exist = temp;
			list->entries--;
		}
	}
	list_sort(list, sorted_contents);
}

/// <summary>
/// Shifts empty or dead pairs within an entry to maintain its integrity.
/// </summary>
/// <param name="list"> - The list containing dead pairs.</param>
/// <param name="sorted_contents"> - List where to store the sorted form of the cleaned list.</param>
void remove_dead_pairs(langentry_l_p list, langentry_p_l_p sorted_contents)
{
	int i, j, k;
	langpair_t *dead, *exist, temp;
	for (i = 0; i < list->entries; i++)
	{
		for (j = 0; j < list->entry_list[i].pairs; j++)
		{
			if (list->entry_list[i].pair_list[j].translations == 0)
			{
				for (k = j; k < list->entry_list[i].pairs - 1; k++)
					dead = &list->entry_list[i].pair_list[k], exist = &list->entry_list[i].pair_list[k + 1],
					temp = *dead, *dead = *exist, *exist = temp;
				list->entry_list[i].pairs--;
			}
		}
	}
	remove_dead_entries(list, sorted_contents);
	list_sort(list, sorted_contents);
}

/// <summary>
/// Prompts and adds entries to the given list.
/// </summary>
/// <param name="list"> - The list where to put the contents.</param>
/// <param name="immediate_pair"> - The first pair to add to the entry provided by search_input()</param>
void add_entry(langentry_l_p list, langpair_t immediate_pair)
{
	// Declarations
	int prompt_status, iterator, translation_counter;
	langentry_p entry;
	strshrt_t language = { 0 }, translations[LANGTRANS_MAX] = { {0} };
	// Introduction
	printf("\nThe data to be entered are for entry #%d.", list->entries++ + 1);
	printf(FOREGROUND_COLORSET("\nPlease enter a language with a corresponding translation below. Note that the input " FOREGROUND_COLORSET("is case-sensitive", 196) ".", 51));
	printf(FOREGROUND_COLORSET("\nOtherwise, type", 51) FOREGROUND_COLORSET(" !! ", 196) FOREGROUND_COLORSET("to cancel the process.\n\n", 51));
	SHOW_RESTRICTED_INPUT_WARNING;
	// Add immediate pair
	assert(list->entries < LANGENTRY_MAX);
	entry = &list->entry_list[list->entries - 1];
	entry->pairs = 1;
	entry->pair_list[0] = immediate_pair;
	if (strcmp(entry->pair_list[0].language_name, "English") == 0)
		entry->english_index = 0;
	else
		entry->english_index = -1;
	// Prompt pairs
	do
	{
		// Reset placeholders
		memset(language, 0, sizeof(strshrt_t));
		for (iterator = 0; iterator < LANGTRANS_MAX; iterator++)
			memset(translations[iterator], 0, sizeof(strshrt_t));
		// Prompt for language
		printf("Enter name for language #%d: ", entry->pairs + 1);
		restrict_input_s(language);
		// Check if input was not aborted
		prompt_status = strcmp(language, "!!") != 0;
		if (prompt_status)
		{
			// Prompt for translations
			for (translation_counter = 0
				; prompt_status && translation_counter < LANGTRANS_MAX
				; translation_counter++)
			{
				printf("Enter translation #%d: ", translation_counter + 1);
				restrict_input_s(translations[translation_counter]);
				if (!(prompt_status = strcmp(translations[translation_counter], "!!") != 0))
					translation_counter--; else;
			}
			// Only add pair if not aborted at translation 1
			if (translation_counter > 0)
			{
				strcpy(entry->pair_list[entry->pairs].language_name, language);
				for (iterator = 0; iterator < translation_counter; iterator++)
					strcpy(entry->pair_list[entry->pairs].translation_list[iterator], translations[iterator]);
				entry->pair_list[entry->pairs++].translations = translation_counter;
				prompt_status = 1;
			}
		}
	} while (prompt_status && entry->pairs < LANGPAIR_MAX);
}

/// <summary>
/// Prompts and adds pairs to an entry within a filtered list.
/// </summary>
/// <param name="filtered_contents"> - List to pick an index where to add the pair(s).</param>
/// <param name="index"> - Index of the entry to put pairs into.</param>
/// <returns>Returns 0 (process aborted) if pair count remains unchanged after the loop</returns>
int add_pair(langentry_p_l_p filtered_contents, int index)
{
	int prompt_status, iterator, translation_counter, pr_count;
	langentry_p entry;
	strshrt_t language = { 0 }, translations[LANGTRANS_MAX] = { {0} };
	// Prompt pairs
	// Mostly same as the iterating prompt on add_entry()
	entry = filtered_contents->_ptr_info[index].ptr;
	pr_count = entry->pairs;
	do
	{
		// Reset placeholders
		memset(language, 0, sizeof(strshrt_t));
		for (iterator = 0; iterator < LANGTRANS_MAX; iterator++)
			memset(translations[iterator], 0, sizeof(strshrt_t));
		// Prompt for language
		printf("Enter name for language #%d: ", entry->pairs + 1);
		restrict_input_s(language);
		// Check if input was not aborted
		prompt_status = strcmp(language, "!!") != 0;
		if (prompt_status)
		{
			// Prompt for translations
			for (translation_counter = 0
				; prompt_status && translation_counter < LANGTRANS_MAX
				; translation_counter++)
			{
				printf("Enter translation #%d: ", translation_counter + 1);
				restrict_input_s(translations[translation_counter]);
				if (!(prompt_status = strcmp(translations[translation_counter], "!!") != 0))
					translation_counter--; else;
			}
			// Only add pair if not aborted at translation 1
			if (translation_counter > 0)
			{
				strcpy(entry->pair_list[entry->pairs].language_name, language);
				for (iterator = 0; iterator < translation_counter; iterator++)
					strcpy(entry->pair_list[entry->pairs].translation_list[iterator], translations[iterator]);
				entry->pair_list[entry->pairs++].translations = translation_counter;
				prompt_status = 1;
			}
		}
	} while (prompt_status && entry->pairs < LANGPAIR_MAX);
	// Returns 0 (process aborted) if pair count remains unchanged after the loop
	return pr_count != entry->pairs;
}

/// <summary>
/// Prompts and adds translations to a pair.
/// </summary>
/// <param name="pair"> - Pointer to the pair to add translations to.</param>
/// <returns>0 if the pair was already full, otherwise 1 to indicate successful addition.</returns>
int add_translation(langpair_p pair)
{
	int translation_count, prompt_status, status;
	strshrt_t translation = { 0 };
	if ((status = pair->translations < LANGTRANS_MAX))
	{
		translation_count = pair->translations, prompt_status = 1;
		do
		{
			printf("Enter translation #%d: ", pair->translations + 1);
			restrict_input_s(translation);
			prompt_status = strcmp(translation, "!!") != 0;
			if (prompt_status)
				strcpy(pair->translation_list[pair->translations++], translation);
		} while (prompt_status && pair->translations < LANGTRANS_MAX);
		status = pair->translations != translation_count;
	}
	else
		printf("This pair has already reached the maximum capacity of translations (max=%d).\n", LANGTRANS_MAX);
	return status;
}

/// <summary>
/// For translate()'s tokenization, gives the index of the next token with a punctuation.
/// </summary>
/// <param name="words">Words to check punctuations in.</param>
/// <param name="punctuations">Sequence of possible punctuation characters to find within each string.</param>
/// <param name="punctuation_found">Variable where to store the index of the first occurence of one of the punctuations within that string.</param>
/// <param name="start_index">Offset from the first token where to start searching.</param>
/// <param name="word_count">Number of words within the array.</param>
/// <returns>The index of the next token or word with a punctuation.</returns>
int nearest_punctuation(char **words, char *punctuations, char **punctuation_found, int start_index, int word_count)
{	
	int i, min;
	for (i = start_index, min = -1; min == -1 && i < word_count; i++)
	{
		*punctuation_found = strpbrk(words[i], punctuations);
		if (*punctuation_found != NULL)
			min = i;
	}
	return min;
}

/// <summary>
/// In case there are duplicate matches of translations, 
/// this function gets the first occurence of the entry with that language within the list.
/// </summary>
/// <param name="pointered_contents"> - Matches containing the list of translations.</param>
/// <param name="from_index"> - Index from where to start searching.</param>
/// <param name="language"> - Language to search along the entries.</param>
/// <returns>The pointer to the found pair.</returns>
langpair_p get_first_pair_p(langentry_p_l_p pointered_contents, int from_index, strshrt_t language)
{
	int i, j, f;
	langentry_p entry;
	langpair_p pair = NULL;
	for (i = from_index, f = 0; !f && i < pointered_contents->count; i++)
	{
		entry = pointered_contents->_ptr_info[i].ptr;
		for (j = 0; !f && j < entry->pairs; j++)
		{
			pair = &entry->pair_list[j];
			f = strcmp(pair->language_name, language) == 0;
		}
	}
	return pair;
}

/// <summary>
/// Translates the given text from a language to another according to the given dataset.
/// </summary>
/// <param name="text"> - The text to translate.</param>
/// <param name="text_language"> - The language of the given text.</param>
/// <param name="language_target"> - The language where to translate the given text.</param>
/// <param name="sorted_contents"> - The dataset where to get translations from.</param>
/// <param name="translation"> - The variable to dynamically allocate and store the resulting translation into.</param>
void translate(strlng_t text, strshrt_t text_language, strshrt_t language_target, langentry_p_l_p sorted_contents, char **translation)
{
	void *realloc_address;
	char punctuations[] = ".,?!";
	char *space_token, **words, **merged, *_translation, *punctuation_found;
	int words_found, iterator, first_merge, merge_finished, merged_count, has_punctuation, translated_length;
	strlng_t keyword = { 0 }, previous = { 0 };
	strlng_t tokenized_text = { 0 };
	langentry_p_l filtered_dictionary = { 0 }, matched_entries = { 0 };
	langpair_p pr_langtxt, pr_langtrg;
	// Narrow down dataset by the two given languages
	filtered_dictionary = _light_search_by_language(sorted_contents, text_language);
	filtered_dictionary = _light_search_by_language(&filtered_dictionary, language_target);
	// Translation process will only proceed if there is text and also there are contents on the filtered dictionary
	if (strlen(text) > 0 && filtered_dictionary.count > 0)
	{
		/* Phase 1: Splitting by punctuations and spaces */
		// Make a copy to tokenize
		strcpy(tokenized_text, text);
		// Split the translation by spaces first
		words_found = 1;
		space_token = strtok(tokenized_text, " ");
		words = malloc(sizeof(char *));
		assert(words != NULL);
		words[0] = malloc(strlen(space_token) + 1);
		assert(words[0] != NULL);
		strcpy(words[0], space_token);
		// Count raw words
		while (space_token != NULL)
		{
			space_token = strtok(NULL, " ");
			if (space_token != NULL)
			{
				words_found++;
				// Resize the array accordingly to words found
				realloc_address = realloc(words, words_found * sizeof(char *));
				// Assume there is enough memory for reallocation
				assert(realloc_address != NULL);
				words = realloc_address;
				words[words_found - 1] = malloc(strlen(space_token) + 1);
				assert(words[words_found - 1] != NULL);
				strcpy(words[words_found - 1], space_token);
			}
		}
		/* Phase 2: Walk through the string to check for words to merge */
		merged = NULL;
		merged_count = 0;
		for (iterator = 0, first_merge = 1, has_punctuation = 0; iterator < words_found; iterator++)
		{
			if (first_merge != 1)
				strcpy(previous, keyword); else;
			merge_finished = has_punctuation || 
				strlen(keyword) + strlen(words[iterator]) + 1 - (first_merge == 1) > STRING_SHORT + strlen(words[iterator]) + 1;
			// If merging is not finished, combine to the next word
			if (!merge_finished)
				if (first_merge == 1)
					strcat(keyword, words[iterator]), first_merge = 0;
				else
					strcat(keyword, " "), strcat(keyword, words[iterator]);
			else;
			// Check for punctuations before and after adding a word
			merge_finished |= (has_punctuation = (iterator == nearest_punctuation(words, punctuations, &punctuation_found, iterator, words_found)));
			if (has_punctuation)
				keyword[strlen(keyword) - strlen(punctuation_found)] = 0; else;
			// Check for a matching translation from the filtered dictionary
			matched_entries = _strict_search_by_occurrence(&filtered_dictionary, text_language, keyword);
			// Merging finishes when there are no entries matched or if this iteration is the last one
			merge_finished |= matched_entries.count == 0 || iterator + 1 >= words_found;
			// If the merging is finished resize the array and append the merged word
			if (merge_finished)
			{
				// Resize
				realloc_address = realloc(merged, ++merged_count * sizeof(char *));
				assert(realloc_address != NULL);
				merged = realloc_address;
				merged[merged_count - 1] = malloc(STRING_SHORT + 1);
				// If there no matches and there's a previous form of the keyword, add the previous and revert the iterator
				if (matched_entries.count == 0 && (first_merge != 0 || previous[0] != 0))
					strcpy(merged[merged_count - 1], previous), iterator--;
				// Otherwise, add the current one to the array
				else
					strcpy(merged[merged_count - 1], keyword);
				// Reset values to default for more possible merges
				memset(previous, 0, sizeof(strshrt_t));
				memset(keyword, 0, sizeof(strshrt_t));
				first_merge = 1, merge_finished = 0, has_punctuation = 0;
			}
		}
		/* Phase 3: Translate each of the merged words */
		_translation = NULL;
		translated_length = 0, first_merge = 1;
		for (iterator = 0; iterator < merged_count; iterator++)
		{
			if (merged[iterator][0] != 0)
			{
				matched_entries = _strict_search_by_occurrence(&filtered_dictionary, text_language, merged[iterator]);
				if (matched_entries.count > 0)
				{
					pr_langtxt = get_first_pair_p(&matched_entries, 0, text_language);
					pr_langtrg = get_first_pair_p(&matched_entries, 0, language_target);
					if (lookup_s(pr_langtxt->translation_list, pr_langtxt->translations, merged[iterator]) != -1)
					{
						translated_length += strlen(pr_langtrg->translation_list[0]) + 1 - first_merge;
						realloc_address = realloc(_translation, translated_length + 1);
						assert(realloc_address != NULL);
						_translation = realloc_address;
						if (first_merge)
							strcpy(_translation, pr_langtrg->translation_list[0]), first_merge = 0;
						else
							strcat(_translation, " "), strcat(_translation, pr_langtrg->translation_list[0]);
					}
					else
					{
						translated_length += strlen(merged[iterator]) + 1 - first_merge;
						realloc_address = realloc(_translation, translated_length + 1);
						assert(realloc_address != NULL);
						_translation = realloc_address;
						if (first_merge)
							strcpy(_translation, merged[iterator]), first_merge = 0;
						else
							strcat(_translation, " "), strcat(_translation, merged[iterator]);
					}
				}
				else
				{
					translated_length += strlen(merged[iterator]) + 1 - first_merge;
					realloc_address = realloc(_translation, translated_length + 1);
					assert(realloc_address != NULL);
					_translation = realloc_address;
					if (first_merge)
						strcpy(_translation, merged[iterator]), first_merge = 0;
					else
						strcat(_translation, " "), strcat(_translation, merged[iterator]);
				}
			}
		}
		/* Phase 4: Clean up and return */
		for (iterator = 0; iterator < words_found; iterator++)
			free(words[iterator]);
		free(words);
		for (iterator = 0; iterator < merged_count; iterator++)
			free(merged[iterator]);
		free(merged);
		*translation = malloc(translated_length + 1);
		assert(*translation != NULL);
		strcpy(*translation, _translation);
		free(_translation);
	}
	else
	{
		realloc_address = realloc(*translation, strlen(text) + 1);
		assert(realloc_address != NULL);
		*translation = realloc_address;
		strcpy(*translation, text);
	}
}
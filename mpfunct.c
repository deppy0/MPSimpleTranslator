#include "mphead.h"

void nosymbols(char *str, char *result)
{
	int i, c, l;
	for (i = 0, c = 0, l = strlen(str); i < l; i++)
		if ((str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= '0' && str[i] <= '9') || str[i] == ' ')
			result[c++] = str[i];
	result[c] = 0;
}

void nosymbols_o(char *str)
{
	int i, c, l;
	strshrt_t t = { 0 };
	for (i = 0, c = 0, l = strlen(str); i < l; i++)
		if ((str[i] >= 'A' && str[i] <= 'Z') ||
			(str[i] >= 'a' && str[i] <= 'z') ||
			(str[i] >= '0' && str[i] <= '9'))
			t[c++] = str[i];
	strcpy(str, t);
}

void nocapital(char *str, char *result)
{
	int i, l;
	for (i = 0, l = strlen(str); i < l; i++)
		result[i] = tolower(str[i]);
	result[i] = 0;
}

void nocapital_o(char *str)
{
	int i, l;
	for (i = 0, l = strlen(str); i < l; i++)
		str[i] = tolower(str[i]);
}

void display_pair(langpair_p pair)
{
	int i;
	printf(FOREGROUND_COLORSET("\t" SHORT_LPRINTF, 220), pair->language_name);
	for (i = 0; i < pair->translations; i++)
	{
		printf(FOREGROUND_COLORSET("%s", 81), pair->translation_list[i]);
		if (i + 1 < pair->translations)
			printf(FOREGROUND_COLORSET(", ", 81));
	}
	printf("\n");
}

void display_entry(langentry_l_p list, int index)
{
	int i;
	langentry_p e;
	if (index >= 0 && index < list->entries)
	{
		e = &list->entry_list[index];
		printf(FOREGROUND_COLORSET("\n#%d", 255), index + 1);
		for (i = 0; i < e->pairs; i++)
			display_pair(&e->pair_list[i]);
	}
}

void display_entry_p(langentry_p_l_p list, int index)
{
	int i;
	langentry_p e;
	if (index >= 0 && index < list->count)
	{
		e = list->ptr_list[index];
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
}

static int _check_entry(langentry_p list, strshrt_t language, strshrt_t translation, int search_translation)
{
	int i, j, n, q, f = 0;
	langpair_p p;
	if (!strcmp(language, "eng") && !search_translation)
	{
		p = &list->pair_list[list->english_index];
		q = list->english_index;
		for (i = 0; !f && i < p->translations; i++)
			f = strcmp(p->translation_list[i], translation) == 0;
	}
	else
	{
		for (i = 0; !f && i < list->pairs; i++)
		{
			p = &list->pair_list[i];
			q = i;
			for (j = 0, n = 1; n && !f && j < p->translations; j++)
			{
				if (!search_translation)
					n = strcmp(p->language_name, language) == 0;
				if (n)
					f = strcmp(p->translation_list[j], translation) == 0;
			}
		}
	}
	if (f)
		f = q;
	else
		f = -1;
	return f;
}

int search_once(langentry_l_p list, strshrt_t language, strshrt_t word, langpair_p *result_ptr, int start_index, int search_word)
{
	int i, f = -1, p;
	for (i = start_index; f == -1 && i < list->entries; i++)
		if ((p = _check_entry(&list->entry_list[i], language, word, search_word)) != -1)
			f = i;
	if (f == -1)
		*result_ptr = NULL;
	else
		*result_ptr = &list->entry_list[f].pair_list[p];
	return f;
}

void search_all(langentry_l_p list, strshrt_t language, strshrt_t word, langentry_p_l_p result_ptr, int start_index, int search_word)
{
	int i;
	langpair_p p;
	i = start_index;
	result_ptr->count = 0;
	while (i >= 0 && i < list->entries)
		if ((i = search_once(list, language, word, &p, i, search_word)) != -1)
			result_ptr->ptr_list[result_ptr->count++] = &list->entry_list[i++];
}

int search_prompt(langentry_l_p list, langentry_p_l_p search_results, langpair_p immediate_pair)
{
	int stop, result;
	strshrt_t language, translation;
	// Prompt language code
	printf("Enter language code: ");
	scanf(SHORT_SCANF, language);
	// !! terminates input
	stop = strcmp(language, "!!") == 0;
	if (!stop)
	{
		printf("Enter translation: ");
		scanf(SHORT_SCANF, translation);
		// !! terminates input
		stop = strcmp(translation, "!!") == 0;
		if (!stop)
		{
			immediate_pair->translations = 1;
			nosymbols_o(language);
			nocapital(language, immediate_pair->language_name);
			nocapital(translation, immediate_pair->translation_list[0]);
			search_all(list, immediate_pair->language_name, immediate_pair->translation_list[0], search_results, 0, 0),
			result = search_results->count > 0;
		}
		else result = -1;
	}
	else result = -1;
	return result;
}

void configure_entry(langentry_l_p list, int *current_entries, langpair_p immediate_pair)
{
	int stop_entry, stop_translation, english_found;
	strshrt_t temp;
	langentry_p entry;
	langpair_p pair;
	// This function is useless when data is NULL.
	if (current_entries != NULL)
	{
		if (*current_entries > 0)
			list->entries = *current_entries;
		else
			list->entries = *current_entries = 0;
		// Prompt for this entry
		printf("\nThis is entry" FOREGROUND_COLORSET(" #%d ", 255) "of your dataset.", list->entries + 1);
		entry = &list->entry_list[list->entries];
		english_found = 0;
		// Prompt for languages within the entry
		// Add the immediate pair beforehand
		entry->pair_list[0] = *immediate_pair;
		for (stop_entry = 0, entry->pairs = 1; !stop_entry && entry->pairs < LANGPAIR_MAX; entry->pairs++)
		{
			pair = &entry->pair_list[entry->pairs];
			if (entry->pairs == 0)
				printf(FOREGROUND_COLORSET("\nYou can add up to 10 languages to your entry.", 51));\
			printf("\nPlease type in a 3-character language code for language " FOREGROUND_COLORSET("#%d", 255)
				", or type " FOREGROUND_COLORSET("'!!'", 196) " to terminate input.", entry->pairs + 1);
			printf(FOREGROUND_COLORSET("\nIt is best that you follow the", 160) FOREGROUND_COLORSET(" "
				"ISO 639-2 standards ", 255) FOREGROUND_COLORSET("to avoid unexpected results in your dataset.", 160));
			printf(FOREGROUND_COLORSET("\nYou may refer to this link:", 226));
			printf(FOREGROUND_COLORSET("\nhttps://en.wikipedia.org/wiki/List_of_ISO_639-2_codes\n\n", 255));
			printf("Language code: ");
			scanf(SHORT_SCANF, temp);
			// !! terminates input
			stop_entry = strcmp(temp, "!!") == 0;
			if (!stop_entry)
			{
				nosymbols_o(temp);
				nocapital(temp, pair->language_name);
				if (strcmp(pair->language_name, "eng") == 0)
					entry->english_index = entry->pairs, english_found = 1;
				else
					if (!english_found)
						entry->english_index = -1;
				// Enter corresponding translations for this language
				for (pair->translations = 0, stop_translation = 0; !stop_translation && pair->translations < LANGTRANS_MAX; pair->translations++)
				{
					if (pair->translations == 0)
						printf(
							"\nNow, enter a translation from that language to add to your entry."
							"\nAfter this word, you can type 4 more words connected to your first word."
							"\nType in '!!' to terminate input if you only plan to put less than 5 words.\n\n"
						);
					printf("Translation #%d: ", pair->translations + 1);
					scanf(SHORT_SCANF, temp);
					// !! terminates input
					stop_translation = strcmp(temp, "!!") == 0;
					if (!stop_translation)
						nocapital(temp, pair->translation_list[pair->translations]);
					else
						if (--pair->translations == -1)
							stop_translation = 0;
				}
			}
			else entry->pairs--;
		}
		if (entry->pairs > 0)
			sort_pairs(&list->entry_list[list->entries++], 0);
		*current_entries = list->entries;
	}
}

void add_entry(langentry_l_p list, int *current_entries)
{
	int i, search, stop_translation;
	strshrt_t user;
	langentry_p_l search_results;
	langpair_t immediate_pair;
	
	search = search_prompt(list, &search_results, &immediate_pair);
	if (search != -1)
	{
		if (search)
		{
			printf(FOREGROUND_COLORSET("\nExisting entries found.\n", 226));
			for (i = 0; i < search_results.count; i++)
				display_entry_p(&search_results, i);
			printf("\nIf you confirm adding a new entry regardless, please enter anything that starts with 'y'. ");
			scanf(SHORT_SCANF, user);
			nocapital_o(user);
			nosymbols_o(user);
			if (user[0] == 'y')
			{
				printf("\nDo you want to add more translations, please enter anything that starts with 'y'. ");
				scanf(SHORT_SCANF, user);
				nocapital_o(user);
				nosymbols_o(user);
				if (user[0] == 'y')
				{
					printf("\nTo terminate input, type '!!'.");
					for (stop_translation = 0, immediate_pair.translations = 1;
						!stop_translation && immediate_pair.translations < LANGTRANS_MAX;
						immediate_pair.translations++)
					{
						printf("\nTranslation #%d: ", immediate_pair.translations + 1);
						scanf(SHORT_SCANF, user);
						stop_translation = strcmp(user, "!!") == 0;
						if (!stop_translation)
							nocapital(user, immediate_pair.translation_list[immediate_pair.translations]);
						else
							immediate_pair.translations--;
					}
				}
				configure_entry(list, current_entries, &immediate_pair);
			}
		}
		else
		{
			printf(FOREGROUND_COLORSET("\nNo entries were found containing the above translation pair.", 51));
			printf(FOREGROUND_COLORSET("\nIt will serve as the first translation pair of this new entry.", 51));
			printf("\nDo you want to add more translations, please enter anything that starts with 'y'. ");
			scanf(SHORT_SCANF, user);
			nocapital_o(user);
			nosymbols_o(user);
			if (user[0] == 'y')
			{
				printf("\nTo terminate input, type '!!'.");
				for (stop_translation = 0, immediate_pair.translations = 1;
					!stop_translation && immediate_pair.translations < LANGTRANS_MAX;
					immediate_pair.translations++)
				{
					printf("\nTranslation #%d: ", immediate_pair.translations + 1);
					scanf(SHORT_SCANF, user);
					stop_translation = strcmp(user, "!!") == 0;
					if (!stop_translation)
						nocapital(user, immediate_pair.translation_list[immediate_pair.translations]);
					else
						immediate_pair.translations--;
				}
			}
			configure_entry(list, current_entries, &immediate_pair);
		}
	}
}

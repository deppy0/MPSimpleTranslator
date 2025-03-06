#include "mpfunct.c"

void add_entry(langentry_l_p list, int *new_entries)
{
	int se, sw, ef, rscanf;
	strshrt_t tempns = { 0 }, tempnc = { 0 };
	langentry_p entry;
	langpair_p pair;
	// This function is useless when data is NULL.
	if (new_entries != NULL)
	{
		printf("%d\n", *new_entries);
		if (*new_entries > 0)
			list->entries = *new_entries;
		else
			list->entries = *new_entries = 0;
		// Ask for inputs
		for (se = 0; !se && list->entries < LANGENTRY_MAX; list->entries++)
		{
			printf("\nThis is entry #%d of your data.", list->entries + 1);
			entry = &list->entry_list[list->entries];
			// enter pairs
			for (entry->pairs = 0, ef = 0; !se && entry->pairs < LANGPAIR_MAX; entry->pairs++)
			{
				pair = &entry->pair_list[entry->pairs];
				if (entry->pairs == 0)
					printf("\nYou can add up to 10 languages to your entry.");
				printf("\nPlease type in a 3-character language code for language #%d, or type '!!' to terminate input.", entry->pairs + 1);
				printf("\nIt is recommended to follow the ISO 639-2 standards. You can refer to the link below:");
				printf("\nhttps://en.wikipedia.org/wiki/List_of_ISO_639-2_codes\n\n");
				printf("Language code: ");
				rscanf = scanf(SHORT_SCANF, tempns);
				se = !strcmp(tempns, "!!");
				if (!se)
				{
					nosymbols(tempns, tempnc);
					nocapital(tempnc, pair->language_name);
					if (!strcmp(pair->language_name, "eng"))
					{
						entry->english_index = entry->pairs;
						ef = 1;
					}
					else
						if (!ef)
							entry->english_index = -1;
					// enter translations
					for (pair->words = 0, sw = 0; !sw && pair->words < LANGTRANS_MAX; pair->words++)
					{
						if (pair->words == 0)
						{
							printf("\nNow, enter a word from that language to add to your entry.\n");
							printf("After this word, you can type 4 more words synonymous to your first word.\n");
							printf("Type in '!!' to terminate input if you only plan to put less than 5 words.\n\n");
						}
						printf("Word #%d: ", pair->words + 1);
						rscanf = scanf(SHORT_SCANF, tempnc);
						sw = !strcmp(tempnc, "!!");
						if (!sw)
							nocapital(tempnc, pair->word_list[pair->words]);
						else
						{
							pair->words--;
							if (pair->words == -1)
								sw = 0;
						}
					}
				}
				else entry->pairs--;
			}
			printf("\nIf you want to continue adding entries, please enter 'y'. ");
			rscanf = scanf(SHORT_SCANF, tempns);
			nosymbols(tempns, tempnc);
			nocapital(tempnc, tempns);
			if (tempns[0] == 'y')
				se = 0;
			if (entry->pairs <= 0)
				list->entries--;
		}
		// Sort entries
		for (*new_entries = 0; *new_entries < list->entries; (*new_entries)++)
			sort_pairs(&list->entry_list[*new_entries], 1);
	}
}

int main(void)
{
	int i, s, e = 0;
	langentry_l root;
	langpair_p ptr = NULL;
	
	// Make sure 'e' is initialized before use
	add_entry(&root, &e);

	if (root.entries > 0)
		for (i = 0; i < root.entries; i++)
			display_entry(&root, i);
			
	s = search_once(&root, "jpn", "utukusii", ptr, 0, 0);
	if (s >= 0)
		printf("\n%d\n", s + 1);
	else
		printf("\n%d\n", s);

	add_entry(&root, &e);

	if (root.entries > 0)
		for (i = 0; i < root.entries; i++)
			display_entry(&root, i);

	s = search_once(&root, "jpn", "tarinai", ptr, 0, 0);
	if (s >= 0)
		printf("\n%d\n", s + 1);
	else
		printf("\n%d\n", s);
}

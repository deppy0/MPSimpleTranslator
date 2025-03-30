/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying
the concepts learned. I have constructed the functions and their respective algorithms and corresponding  
code by myself. The program was run, tested, and debugged by my own efforts. I further certify that I have
not copied in part or whole or otherwise plagiarized the work of other students and/or persons.           
                                                                                                          
																				LAGUERTA, KURT ANJO A.    
																					 DLSU ID# 12412325    
*********************************************************************************************************/

#include "mpfunct.c"

// Prototypes from layer 18 with the same mechanism to be used in layer 2
static int layer18b(filename_t source);
static void layer18c(filename_t source, langentry_l_p imported_contents);

static void layer0(int *layer)
{
	char ch;
	int v, lines;
	// Select
	ch = 0, v = 0, lines = 0;
	while (!v)
	{
		for (; lines-- > 0;)
			printf("\x1b[2K\x1b[1A");
		printf("\nSimple Translator\n"), lines += 2;
		printf("1) Manage Data\n"), lines++;
		printf("2) Translate\n"), lines++;
		printf("3) Exit\n"), lines++;
		printf("Select key of desired option... "), lines++;
		if ((ch = _getch()) != '\n')
			printf("%c\n", ch), lines++;
		v = ch >= '1' && ch <= '3';
	}
	*layer = ch - '0';
}

static int layer1b(int *exported)
{
	char ch;
	int lines, prompt, response;
	ch = 0, lines = 0, response = 0;
	if (!*exported)
	{
		prompt = 1;
		do
		{
			for (; lines-- > 0;)
				printf("\x1b[2K\x1b[1A");
			lines = 0;
			printf("\n\nUnsaved data was found from your previous Manage Data session."), lines += 2;
			printf("\nDo you want to load these? Otherwise, it will be deleted. (Y/N): "), lines++;
			if ((ch = getch()) != '\n')
				printf("%c\n", ch), lines++;
			if (ch == 'N' || ch == 'n')
				response = 0;
			else if (ch == 'Y' || ch == 'y')
				response = 1;
			else;
			prompt = ch != 'Y' && ch != 'N' && ch != 'y' && ch != 'n';
		} while (prompt);
	}
	return response;
}

static void layer1(int *layer, int *exported, int *main_menu, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	FILE *f;
	char ch;
	int v, lines;
	// Prompt to load unsaved data
	if (*main_menu && *exported != -1)
		if (layer1b(exported))
			*contents = read_data(FILE_DS_NRL, sorted_contents);
		else
			f = fopen(FILE_DS_NRL, "w"), fclose(f);
	else
		*exported = 0,
		*contents = read_data(FILE_DS_NRL, sorted_contents);
	// Select
	ch = 0, v = 0, lines = 0;
	while (!v)
	{
		// Clear occupied lines
		for (; lines-- > 0;)
			printf("\x1b[2K\x1b[1A");
		printf("\n"), lines++;
		printf("0) Add Entry...\n"), lines++;
		printf("1) Add Translation...\n"), lines++;
		printf("2) Delete Entry...\n"), lines++;
		printf("3) Delete Translation...\n"), lines++;
		printf("4) Display All Entries\n"), lines++;
		printf("5) Search Word...\n"), lines++;
		printf("6) Search Translation...\n"), lines++;
		printf("7) Export Data...\n"), lines++;
		printf("8) Import From File...\n"), lines++;
		printf("9) Exit this menu\n"), lines++;
		printf("Select key of desired option... "), lines++;
		if ((ch = _getch()) != '\n')
			printf("%c\n", ch), lines++;
		v = ch >= '0' && ch <= '9';
	}
	*layer = (ch - '0') + 10;
	*main_menu = *layer <= 9 || *layer == 19;
}

static void layer2(int *layer, int *main_menu, int *has_imported, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	char ch;
	int v, lines, cancelled;
	filename_t source;
	// Ask for file
	cancelled = 0;
	if (!*has_imported)
	{
		printf("\n\nBefore continuing, you must import a basis text file containing the translations that will be used.\n");
		system("pause");
		cancelled = layer18b(source);
		if (!cancelled)
		{
			*has_imported = 1;
			layer18c(source, contents);
			list_sort(contents, sorted_contents);
			printf("%d entries loaded.\n", sorted_contents->count);
		}
	}
	// Select
	if (!cancelled)
	{

		ch = 0, v = 0, lines = 0;
		while (!v)
		{
			for (; lines-- > 0;)
				printf("\x1b[2K\x1b[1A");
			printf("\n"), lines++;
			printf("1) Translate from input...\n"), lines++;
			printf("2) Translate from file...\n"), lines++;
			printf("3) Exit\n"), lines++;
			printf("Select key of desired option... "), lines++;
			if ((ch = _getch()) != '\n')
				printf("%c\n", ch), lines++;
			v = ch >= '1' && ch <= '3';
		}
		*layer = (ch - '0') + 20;
		*main_menu = *layer == 23;
	}
	else SHOW_CANCEL_MSG, *layer = 0;
}

static void layer10(int *layer, langentry_l_p contents)
{
	// Declarations
	char ch;
	strshrt_t s = { 0 };
	int search_return;
	langentry_p_l sorted = { 0 }, filter = { 0 };
	langpair_t impair = { 0 };
	// Check if dataset is not full
	if (contents->entries < LANGENTRY_MAX)
	{
		// Prompt
		printf(FOREGROUND_COLORSET("\nThe first translation pair of your entry", 226) FOREGROUND_COLORSET(" #%d ", 255) FOREGROUND_COLORSET(
			"must be searched prior to addition.", 226), contents->entries + 1);
		printf(FOREGROUND_COLORSET("\nPlease enter a language with a corresponding translation below. Note that the input is" FOREGROUND_COLORSET("case-sensitive", 196) ".", 51));
		printf(FOREGROUND_COLORSET("\nOtherwise, type", 51) FOREGROUND_COLORSET(" !! ", 196) FOREGROUND_COLORSET("to cancel the process.\n\n", 51));
		list_sort(contents, &sorted);
		if ((search_return = search_input(&sorted, &filter, &impair, 1)) != -1)
		{
			if (search_return > 0)
			{
				// display pair
				display_all_p(&filter);
				// prompt if continue
				ch = 0;
				printf("Despite the existing entries, would you like to add this entry? (1=Y, 2=N): \x1b[s");
				while (ch != '1' && ch != '2')
					printf("\x1b[u%c", ch = _getch());
				// if continue, ask for more translations
				if (ch == '1')
				{
					ch = 0;
					printf("\nDo you want to add another translation for this pair? (1=Y, 2=N): \x1b[s");
					while (ch != '1' && ch != '2')
						printf("\x1b[u%c", ch = _getch());
					if (ch == '1')
					{
						printf("\nYou can enter 4 more translations. Maximum of 20 characters.");
						printf("\nSimply enter" FOREGROUND_COLORSET(" !! ", 196) "to end input with less than 5 translations.\n");
						SHOW_RESTRICTED_INPUT_WARNING;
						do
						{
							printf("Enter translation #%d: ", impair.translations + 1);
							restrict_input_s(s);
							if (strcmp(s, "!!") != 0)
								strcpy(impair.translation_list[impair.translations++], s);
						} while (impair.translations < LANGTRANS_MAX && strcmp(s, "!!") != 0);
					}
					// add entry
					add_entry(contents, impair);
					insert_data(FILE_DS_NRL, &contents->entry_list[contents->entries - 1], contents->entries - 1);
					printf("\nEntry #%d has been added to your dataset.", contents->entries);
					printf("\nPress any key to continue...%c\n", _getch());
				}
			}
			else
			{
				// Same old
				ch = 0;
				printf("\nDo you want to add another translation for this pair? (1=Y, 2=N): \x1b[s");
				while (ch != '1' && ch != '2')
					printf("\x1b[u%c", ch = _getch());
				if (ch == '1')
				{
					printf("\nYou can enter " FOREGROUND_COLORSET("4", 21) " more translations.");
					printf("\nSimply enter " FOREGROUND_COLORSET(" !! ", 196) "to end input with less than 5 translations.\n\n");
					do
					{
						printf("Enter translation #%d: ", impair.translations + 1);
						restrict_input_s(s);
						if (strcmp(s, "!!") != 0)
							strcpy(impair.translation_list[impair.translations++], s);
					} while (impair.translations < LANGTRANS_MAX && strcmp(s, "!!") != 0);
				}
				// add entry
				add_entry(contents, impair);
				insert_data(FILE_DS_NRL, &contents->entry_list[contents->entries - 1], contents->entries - 1);
				printf("\nEntry #%d has been added to your dataset.", contents->entries);
				printf("\nPress any key to continue...%c\n", _getch());
			}
		}
	}
	// Otherwise, simply display a message
	else
		printf("\nYou cannot add any more entries.%c\n", _getch());
	*layer = 1;
}

static int layer11b()
{
	int prompt;
	char ch;
	printf("\nDo you want to...?");
	printf("\n1) Add translation(s) to a pair within the list");
	printf("\n2) Add pair(s) to the existing entry");
	printf("\n\nPress an appropriate action accordingly... \x1b[s");
	prompt = 0, ch = 0;
	while (prompt == 0)
	{
		switch (printf("\x1b[u%c", ch = _getch()), ch)
		{
			default:
				printf("\x1b[u" FOREGROUND_COLORSET("%c", 196), ch);
				break;
			case '1':
			case '2':
				prompt = ch - 48;
				printf("\n");
				break;
		}
	}
	return prompt;
}

static void layer11(int *layer, langentry_l_p contents, langentry_p_l_p sorted_output)
{
	// Declarations
	char ch;
	int search_return, display_index, target_index, pair_index;
	langentry_p_l sorted_contents, filtered_output;
	langpair_p target_pair;
	langpair_t discarded_pair;
	if (contents->entries != 0)
	{
		// Prompt for existing entry
		printf("\nThe entry must be existent. The program will now request for a search.");
		printf(FOREGROUND_COLORSET("\nPlease enter a language with a corresponding translation below. Note that the input " FOREGROUND_COLORSET("is case-sensitive", 196) ".", 51));
		printf(FOREGROUND_COLORSET("\nOtherwise, type", 51) FOREGROUND_COLORSET(" !! ", 196) FOREGROUND_COLORSET("to cancel the process.\n\n", 51));
		if ((list_sort(contents, &sorted_contents), search_return = search_input(&sorted_contents, &filtered_output, &discarded_pair, 1)) != -1)
		{
			if (search_return > 0)
			{
				// Introduction
				printf(FOREGROUND_COLORSET("\nEntries are found.", 226));
				printf(FOREGROUND_COLORSET("\nThe entries are going to be shown one by one for you to navigate.", 51));
				printf("\nEnd display at the entry that you want to add a translation pair to by pressing " FOREGROUND_COLORSET("'/'", 154) ".\n");
				system("pause");
				// Display all existing entries
				display_index = display_all_p(&filtered_output);
				target_index = filtered_output._ptr_info[display_index].original_index;
				// Prompt desired option
				switch (layer11b())
				{
					default:
						break;
					case 1: // Option 1
					{
						printf("\nPress a digit starting from '1' to choose the pair you want to add translations to.");
						printf("\nIf it exists, pair #10 can be selected by pressing '0'.");
						printf("\nYou can press '-' to cancel and return to the Manage Data menu. \x1b[s");
						// Pair target prompt
						pair_index = -1;
						while (pair_index == -1)
						{
							printf("\x1b[u%c", ch = _getch());
							if (isdigit(ch) || ch == '-')
							{
								if (ch == '0' && 9 < filtered_output._ptr_info[display_index].ptr->pairs)
									pair_index = 9;
								else if (ch >= '1' && ch - '0' - 1 < filtered_output._ptr_info[display_index].ptr->pairs)
									pair_index = ch - '0' - 1;
								else if (ch == '-')
									pair_index = -2; // -2: cancel
								else
									pair_index = -1; // -1: invalid key
								if (pair_index >= 0)
									printf("\n\n" FOREGROUND_COLORSET("%s", 226) " has been selected from entry #%d.\n\n",
										filtered_output._ptr_info[display_index].ptr->pair_list[pair_index].language_name, target_index + 1);
							}
							else
								printf("\x1b[u" FOREGROUND_COLORSET("%c", 196), ch);
						}
						// Translation prompt
						if (pair_index != -2)
						{
							target_pair = &filtered_output._ptr_info[display_index].ptr->pair_list[pair_index];
							if (add_translation(target_pair))
								printf("\nTranslations has been added to %s at entry #%d.\n"
									"Press any key to go back to the Manage Data menu.", target_pair->language_name, target_index + 1),
								system("pause>nul");
							else SHOW_CANCEL_MSG;
						}
						else SHOW_CANCEL_MSG;
						break;
					}
					case 2: // Option 2
					{
						if (filtered_output._ptr_info[display_index].ptr->pairs == LANGPAIR_MAX)
							printf("\nThis entry is already full. Maximum pairs is %d.\n", LANGPAIR_MAX), SHOW_CANCEL_MSG;
						if (add_pair(&filtered_output, display_index))
							printf("\nThe requested pair has been added to entry #%d.\n"
								"Press any key to go back to the Manage Data menu.", target_index + 1),
							system("pause>nul");
						else SHOW_CANCEL_MSG;
						break;
					}
				}
			}
			else
			{
				// Entry does not exist
				printf("\nNo such entry was found on your current dataset.");
				printf("\nPlease be sure that the entry exists through the" FOREGROUND_COLORSET(" Add Entry ", 207)
					"option first before proceeding with adding translation pairs.");
				printf("\nPress any key to return to the Manage Data menu.");
				system("pause>nul");
			}
		}
		else SHOW_CANCEL_MSG;
		list_sort(contents, sorted_output);
		refresh_data(FILE_DS_NRL, contents);
	}
	else
	{
		// No entries
		printf("\nYour dataset is empty, so there's no entry to add pairs or translations to.");
		printf("\nPress any key to return to the Manage Data menu.");
		system("pause>nul");
	}
	*layer = 1;
}

static void layer12(int *layer, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	int display_index, target_index;
	langentry_p target, last;
	langentry_t blank = { 0 };
	strshrt_t last_prompt = { 0 };
	if (contents->entries != 0)
	{
		// Introduction
		printf(FOREGROUND_COLORSET("\nThe entries are going to be shown one by one for you to navigate.", 51));
		printf("\nEnd display at the entry that you want to delete by pressing " FOREGROUND_COLORSET("'/'", 154) ".\n");
		system("pause");
		// Display all entries and select the desired entry
		display_index = display_all_p(sorted_contents);
		target_index = sorted_contents->_ptr_info[display_index].original_index;
		printf("You have selected entry " FOREGROUND_COLORSET("#%d", 255) ".\n\n", target_index + 1);
		// One last prompt of deletion for the user
		printf("Are you sure you want to delete " FOREGROUND_COLORSET("entry #%d?", 14) "\n", target_index + 1);
		printf("Kindly type " FOREGROUND_COLORSET("\"yes\"", 161) " in lowercase. Any other input returns to the Manage Data menu.\n\n");
		assert(scanf(SHORT_SCANF, last_prompt) == 1);
		if (strcmp(last_prompt, "yes") == 0)
		{
			target = sorted_contents->_ptr_info[display_index].ptr;
			last = &contents->entry_list[--contents->entries];
			// Copy to target index and remove values stored on the last index
			if (contents->entries != 0)
				*target = *last;
			*last = blank;
			// Sort list and update data on .ds-nrl
			if (contents->entries != 0)
				list_sort(contents, sorted_contents);
			refresh_data(FILE_DS_NRL, contents);
			// Announce
			printf("\nEntry #%d has been deleted.", target_index + 1);
			printf("\nPress any key to return to the Manage Data menu.");
			system("pause>nul");
		}
		else SHOW_CANCEL_MSG;
	}
	else
	{
		// No entries
		printf("\nYour dataset is empty.");
		printf("\nPress any key to return to the Manage Data menu.");
		system("pause>nul");
	}
	*layer = 1;
}

static int layer13ba()
{
	int prompt;
	char ch;
	printf("\x1b[1A\rDo you want to...?");
	printf("\n1) Delete a whole pair");
	printf("\n2) Delete a translation from a pair");
	printf("\n\nPress an appropriate action accordingly... \x1b[s");
	prompt = 0, ch = 0;
	while (prompt == 0)
	{
		switch (printf("\x1b[u%c", ch = _getch()), ch)
		{
			default:
				printf("\x1b[u" FOREGROUND_COLORSET("%c", 196), ch);
				break;
			case '1':
			case '2':
				prompt = ch - 48;
				printf("\n");
				break;
		}
	}
	return prompt;
}

static int layer13b(langentry_p_l_p sorted_contents, _HPTR_INFO *target_ptr, int *action)
{
	int entry_index;
	printf(FOREGROUND_COLORSET("\nEntries will be shown one by one for you to select.\n", 51));
	printf("Navigate through the entries and end the display on the entry of your choice.\n");
	printf("Press any key to proceed... ");
	system("pause>nul");
	// Display all entries
	entry_index = display_all_p(sorted_contents);
	*target_ptr = sorted_contents->_ptr_info[entry_index];
	*action = layer13ba();
	return entry_index;
}

static int layer13c(langpair_p *target_pair, _HPTR_INFO target_ptr, int action, int pair_count)
{
	char ch;
	int pair_index, prompting;
	strshrt_t input = { 0 };
	printf("You have chosen " FOREGROUND_COLORSET("entry #%d", 190) ".\n", target_ptr.original_index + 1);
	printf("The contents of " FOREGROUND_COLORSET("entry #%d", 190) " are as shown above.\n\n", target_ptr.original_index + 1);
	if (action == 1)
		printf("Enter the index of the pair to delete.\n");
	else
		printf("Enter the index of the pair you want to delete a translation from.\n");
	printf("Your input should be only from \x1b[38;5;202m\x1b[4m1 to the number of pairs\x1b[0m shown above.\n");
	printf("Any other input will cancel the process and return you to the Manage Data menu.\n\n");
	prompting = 1;
	while (prompting)
	{
		printf("Pair index: ");
		assert(scanf(SHORT_SCANF, input) == 1);
		if ((prompting = sscanf(input, "%d", &pair_index)) != 1)
			pair_index = 0;
		prompting = pair_index < 1 || pair_index > pair_count;
		if (prompting)
		{
			printf("\nInvalid input.");
			printf("\nPress " FOREGROUND_COLORSET("'/'", 255) " to try again, or " 
				FOREGROUND_COLORSET("'\\'", 255) " to go back to the Manage Data menu... ");
			ch = 0;
			while (ch != '/' && ch != '\\')
				ch = _getch();
			if (ch == '/')
				printf("\n\n");
			else
				pair_index = 0, prompting = 0;
		}
	}
	if (pair_index != 0)
		*target_pair = &target_ptr.ptr->pair_list[pair_index - 1];
	else
		*target_pair = NULL;
	return pair_index - 1;
}

static int layer13d(langentry_l_p contents, langentry_p_l_p sorted_contents, int entry_index, int pair_index)
{
	int return_value;
	strshrt_t input = { 0 };
	langpair_t *pr_target, pr_blank = { 0 };
	langentry_t *en_target;
	printf("\nAre you sure you want to delete pair #%d of entry #%d?\n", pair_index + 1, entry_index + 1);
	printf("Type " FOREGROUND_COLORSET("\"yes\"", 160) " in lowercase to confirm your action.\n");
	printf("Any other input will return you to the Manage Data menu. ");
	assert(scanf(SHORT_SCANF, input) == 1);
	return_value = 1;
	if (strcmp(input, "yes") == 0)
	{
		en_target = &contents->entry_list[entry_index];
		if (en_target->pairs == 1)
			return_value = 2;
		pr_target = &en_target->pair_list[pair_index];
		*pr_target = pr_blank;
		// Cleanup
		remove_dead_pairs(contents, sorted_contents);
		remove_dead_entries(contents, sorted_contents);
	}
	else
		return_value = 0;
	return return_value;
}

static int layer13e(langpair_p target_pair, int pair_index, int translation_count)
{
	char ch;
	int translation_index, prompting;
	strshrt_t input = { 0 };
	display_pair_p(target_pair, NULL);
	printf("From \x1b[4mpair #%d\x1b[0m, select a translation to remove.\n", pair_index + 1);
	printf("Your input should be only from \x1b[38;5;202m\x1b[4m1 to the number of translations\x1b[0m shown above.\n");
	printf("Any other input will cancel the process and return you to the Manage Data menu.\n\n");
	prompting = 1;
	while (prompting)
	{
		printf("Translation input: ");
		assert(scanf(SHORT_SCANF, input) == 1);
		if ((prompting = sscanf(input, "%d", &translation_index)) != 1)
			translation_index = 0;
		prompting = translation_index < 1 || translation_index > translation_count;
		if (prompting)
		{
			printf("\nInvalid input.");
			printf("\nPress " FOREGROUND_COLORSET("'/'", 255) " to try again, or " 
				FOREGROUND_COLORSET("'\\'", 255) " to go back to the Manage Data menu... ");
			ch = 0;
			while (ch != '/' && ch != '\\')
				ch = _getch();
			if (ch == '/')
				printf("\n\n");
			else
				translation_index = 0, prompting = 0;
		}
	}
	return translation_index - 1;
}

static int layer13f(langentry_l_p contents, langentry_p_l_p sorted_contents, int entry_index, int pair_index, int translation_index)
{
	int return_value;
	strshrt_t input = { 0 };
	langpair_p pr_target;
	printf("\nAre you sure you want to delete translation #%d from pair #%d of entry #%d?\n",
		translation_index + 1, pair_index + 1, entry_index + 1);
	printf("Type " FOREGROUND_COLORSET("\"yes\"", 160) " in lowercase to confirm your action.\n");
	printf("Any other input will return you to the Manage Data menu. ");
	assert(scanf(SHORT_SCANF, input) == 1);
	return_value = 1;
	if (strcmp(input, "yes") == 0)
	{
		pr_target = &contents->entry_list[entry_index].pair_list[pair_index];
		if (pr_target->translations == 1)
			return_value = 2;
		if (return_value == 2 && contents->entry_list[entry_index].pairs == 1)
			return_value = 3;
		delete_translation(pr_target, translation_index);
		// Cleanup
		remove_dead_pairs(contents, sorted_contents);
		remove_dead_entries(contents, sorted_contents);
	}
	else
		return_value = 0;
	return return_value;
}

static void layer13(int *layer, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	// Declaration
	_HPTR_INFO target_ptr;
	langpair_p target_pair;
	int 
		action, entry_index, pair_index, translation_index,
		_13d, _13f;
	if (contents->entries != 0)
	{
		// Get target entry and show contents
		entry_index = layer13b(sorted_contents, &target_ptr, &action);
		display_one_p(sorted_contents, entry_index, NULL);
		// Get target pair
		pair_index = layer13c(&target_pair, target_ptr, action, target_ptr.ptr->pairs);
		if (pair_index != -1)
		{
			switch (action)
			{
				default:
					break;
				case 1: // Delete the pair as a whole
				{
					_13d = layer13d(contents, sorted_contents, target_ptr.original_index, pair_index);
					if (_13d > 0)
					{
						// Update .ds-nrl
						list_sort(contents, sorted_contents);
						refresh_data(FILE_DS_NRL, contents);
						// Announce
						printf(FOREGROUND_COLORSET("\nPair #%d has been deleted from entry #%d.", 124), pair_index + 1, target_ptr.original_index + 1);
						if (_13d == 2)
							printf(FOREGROUND_COLORSET("\nSince entry #%d ended up empty, it was also removed.", 255), target_ptr.original_index + 1);
						printf(FOREGROUND_COLORSET("\nPress any key to go back to the Manage Data menu.", 255));
						system("pause>nul");
					}
					else SHOW_CANCEL_MSG;
					break;
				}
				case 2: // Delete a translation from the pair
				{
					translation_index = layer13e(target_pair, pair_index, target_pair->translations);
					if (translation_index != -1)
					{
						_13f = layer13f(contents, sorted_contents, target_ptr.original_index, pair_index, translation_index);
						if (_13f > 0)
						{
							// Update .ds-nrl
							list_sort(contents, sorted_contents);
							refresh_data(FILE_DS_NRL, contents);
							// Announce
							printf("\nTranslation #%d has been deleted.", translation_index + 1);
							if (_13f == 2)
								printf("\nSince pair #%d ended up empty, it was also removed.", pair_index + 1);
							else if (_13f == 3)
								printf(
									FOREGROUND_COLORSET("\nSince pair #%d ended up empty, it was also removed.", 51)
									FOREGROUND_COLORSET("\nSince pair #%d is also the last pair for entry #%d, the entry is also deleted.", 200),
									pair_index + 1, pair_index + 1, target_ptr.original_index + 1); else;
							printf(FOREGROUND_COLORSET("\nPress any key to go back to the Manage Data menu.", 255));
							system("pause>nul");
						}
						else SHOW_CANCEL_MSG;
					}
					else SHOW_CANCEL_MSG;
					break;
				}
			}
		}
		else SHOW_CANCEL_MSG;
	}
	else
		// No entries
		printf("\nYour dataset is empty.\n"
			"Press any key to return to the Manage Data menu."),
		system("pause>nul");
	*layer = 1;
}

static void layer14(int *layer, langentry_p_l_p sorted_contents)
{
	if (sorted_contents->count > 0)
		display_all_p(sorted_contents);
	else
		printf("\nNo entries can be displayed.\n"), system("pause");
	*layer = 1;
}

static void layer15(int *layer, langentry_p_l_p sorted_contents)
{
	int search_result;
	langentry_p_l filtered_contents;
	langpair_t pr_discard;
	if ((search_result = search_input(sorted_contents, &filtered_contents, &pr_discard, 0)) != -1)
	{
		if (search_result > 0)
			display_all_p(&filtered_contents);
		else
			printf("\n\nNo entries matched your search.\n"
				"Press any key to go back to the Manage Data menu."), system("pause>nul");
	}
	else SHOW_CANCEL_MSG;
	*layer = 1;
}

static void layer16(int *layer, langentry_p_l_p sorted_contents)
{
	int search_result;
	langentry_p_l filtered_contents;
	langpair_t pr_discard;
	if ((search_result = search_input(sorted_contents, &filtered_contents, &pr_discard, 1)) != -1)
	{
		if (search_result > 0)
			display_all_p(&filtered_contents);
		else
			printf("No entries matched your search.\n"
				"Press any key to go back to the Manage Data menu."), system("pause>nul");
	}
	else SHOW_CANCEL_MSG;
	*layer = 1;
}

static void layer17(int *layer, int *exported, langentry_p_l_p sorted_contents)
{
	// Declarations
	char ch;
	int loop, good, i, j, k;
	FILE *target;
	filename_t filename = { 0 };
	langentry_p entry;
	langpair_p pair;
	if (sorted_contents->count > 0)
	{
		// Filename prompt
		good = 0, loop = 1;
		while (good == 0 && loop)
		{
			// Introduction
			printf("\n\nInput a filename of \x1b[38;5;226m\x1b[4mat most 30 characters\x1b[0m, including the extension.\n");
			printf("\nType " FOREGROUND_COLORSET("\"\?\?\"", 117) " to abort this process and go back to the Manage Data menu.\n");
			printf("Since \".txt\" is automatically appended to your filename, you can only input at most 26 characters.\n");
			printf("Make sure the file does not contain any of the following symbols: %s\n", FILENAME_FORBIDDEN_SPACED);
			printf(BACKGROUND_COLORSET(FOREGROUND_COLORSET("WARNING!", 255), 196)
				" Your file will be overridden if you enter a filename that does exist.\n\n");
			printf("Enter filename for export: ");
			assert(scanf(FILENAME_SCANF, filename) == 1);
			// Check if length is at most 26 and does not contain any forbidden characters
			// Also cancel operation if filename is ??
			if (strcmp(filename, "\?\?") == 0)
				good = -1;
			else
				good = valid_filename(filename) && strlen(filename) <= STRING_FILENAME - 4;
			if (!good)
			{
				printf("This filename is invalid.\n");
				printf("Please type a valid filename.\n\n");
				printf("\nPress " FOREGROUND_COLORSET("'/'", 255) " to try again, or "
					FOREGROUND_COLORSET("'\\'", 255) " to go back to the Manage Data menu... ");
				ch = 0;
				while (ch != '/' && ch != '\\')
					ch = _getch();
				loop = ch == '/';
			}
		}
		if (good == 1)
		{
			// Append ".txt"
			strcat(filename, ".txt");
			// Open for writing, overwrite existing file
			target = fopen(filename, "w");
			// Iterate through dataset as formatted
			for (i = 0; i < sorted_contents->count; i++)
			{
				entry = sorted_contents->_ptr_info[i].ptr;
				for (j = 0; j < entry->pairs; j++)
				{
					pair = &entry->pair_list[j];
					fprintf(target, "%s: ", pair->language_name);
					for (k = 0; k < pair->translations; k++)
					{
						fprintf(target, "%s", pair->translation_list[k]);
						if (k + 1 < pair->translations)
							fprintf(target, "|"); else;
					}
					// Do not put new lines on last entry
					if (j + 1 < entry->pairs)
						fprintf(target, "\n"); else;
				}
				// ... same here
				if (i + 1 < sorted_contents->count)
					fprintf(target, "\n\n"); else;
			}
			fclose(target);
			*exported = 1;
			// Success message
			printf(FOREGROUND_COLORSET("\nYour entries are saved into ", 51) FOREGROUND_COLORSET("\"%s\"\n", 200), filename);
			printf("Press any key to go back to the Manage Data menu.");
			system("pause>nul");
		}
		else SHOW_CANCEL_MSG;
	}
	else
	{
		// No entries
		printf("\nYour dataset is empty, so there's no entry to add pairs or translations to.");
		printf("\nPress any key to return to the Manage Data menu.");
		system("pause>nul");
	}
	*layer = 1;
}

static int layer18b(filename_t source)
{
	FILE *test_open;
	int filename_valid, file_existing, cancelled;
	do
	{
		printf("\n\nEnter the name of the file to import entries from. ");
		printf("\n\".txt\" is already assumed as the extension, therefore simply input the name of the file.");
		printf("\nMake sure you refer to a \".txt\" file with this name, and the filename is limited to %d characters.", STRING_FILENAME - 4);
		printf("\nOtherwise, Type \"\?\?\" to cancel this process and go back to the Manage Data menu.");
		printf("\n\nImport file: ");
		filename_valid = 0, file_existing = 0, cancelled = 0;
		assert(scanf(FILENAME_SCANF, source) == 1);
		filename_valid = valid_filename(source);
		cancelled = strcmp(source, "\?\?") == 0;
		if (!cancelled)
		{
			if (filename_valid)
			{
				filename_valid = strlen(source) <= STRING_FILENAME - 4;
				if (!filename_valid)
				{
					printf("\nThis filename is too long.");
					printf("\nPlease regard of the instructions above.");
					printf("\nPress any key to try again...");
					system("pause>nul");
				}
			}
			else
			{
				printf("\nYour filename contains forbidden characters.");
				printf("\nMake sure you have entered none of the following special characters in your file.");
				printf("\nFiles cannot exist with these characters: %s", FILENAME_FORBIDDEN_SPACED);
				printf("\n\nPress any key to try again...");
				system("pause>nul");
			}
			if (filename_valid)
			{
				strcat(source, ".txt");
				file_existing = (test_open = fopen(source, "r")) != NULL;
				if (!file_existing)
				{
					printf("\nThere is no existing " FOREGROUND_COLORSET("\"%s\"", 200) " within the same directory as this program.", source);
					printf("\nMake sure the file and this program are in the same folder.");
					printf("\nPress any key to try again...");
					system("pause>nul");
				}
			}
		}
	} while (!file_existing && !cancelled);
	return cancelled;
}

static void layer18c(filename_t source, langentry_l_p imported_contents)
{
	FILE *target;
	strshrt_t language = { 0 };
	strlng_t translations = { 0 };
	langentry_p entry;
	langpair_p pair;
	char *tk, chBin[5] = { 0 };
	target = fopen(source, "rb");
	// This is CRLF-based
	while (!feof(target))
	{
		if (fscanf(target, "%"STR(STRING_SHORT)"[^:]: %"STR(STRING_LONG)"[^\r\n]", language, translations) == 2)
		{
			entry = &imported_contents->entry_list[imported_contents->entries];
			pair = &entry->pair_list[entry->pairs];
			strcpy(pair->language_name, language);
			if (strcmp(pair->language_name, "English") == 0)
				entry->english_index = entry->pairs;
			tk = strtok(translations, "|");
			while (tk != NULL)
			{
				strcpy(pair->translation_list[pair->translations++], tk);
				tk = strtok(NULL, "|");
			}
			if (fscanf(target, "%c%c%c%c", &chBin[0], &chBin[1], &chBin[2], &chBin[3]) == 4)
			{
				if (strcmp(chBin, "\r\n\r\n") == 0)
					imported_contents->entries++;
				else
					fseek(target, -2, SEEK_CUR);
			}
			entry->pairs++;
		}
	}
	imported_contents->entries++;
	fclose(target);
}

static int layer18d(int current_entries, langentry_l_p imported_contents, langentry_p_l_p selected_contents)
{
	char ch;
	int iterator, index, lines, display, proceed, selected, unselected;
	int *unselected_indices;
	_HPTR_INFO ptrinfo = { 0 };
	langentry_p_l ptrlist = get_entry_p_l(imported_contents);
	// Introduction
	printf(FOREGROUND_COLORSET("Since you already have entries contained in your dataset,\n"
		"you'd have to select which entries you get to include in your file.\n", 51));
	printf("Each entry will be shown below. Press the appropriate action to decide for which entries.\n");
	system("pause");
	// Prompting process
	unselected_indices = malloc(ptrlist.count * sizeof(int));
	assert(unselected_indices != NULL);
	for (iterator = 0; iterator < ptrlist.count; iterator++)
		unselected_indices[iterator] = iterator;
	ch = 0, lines = 0, display = 0, index = 0, selected = 0, unselected = ptrlist.count, proceed = 1;
	do
	{
		if (display)
			for (; lines-- > 0;)
				printf("\x1b[2K\x1b[1A");
		display_one_p(&ptrlist, unselected_indices[index], &lines);
		if (index > 0)
			printf("(,) %8s ", "Previous");
		else
			printf("(,) %8s ", "Last");
		printf("| %d of %d |", index + 1, unselected);
		if (index + 1 < unselected)
			printf(" (.) %-8s ", "Next");
		else
			printf(" (.) %-8s ", "First");
		printf("\n\nPress " FOREGROUND_COLORSET("'.'", 121) " or " FOREGROUND_COLORSET("','", 121) " as mentioned above to navigate through the imported entries."); lines += 2;
		printf("\nPress " FOREGROUND_COLORSET("';'", 190) " to add this entry to your selections."), lines++;
		printf("\nPress " FOREGROUND_COLORSET("'{'", 202) " to cancel all selections."), lines++;
		printf("\nPress " FOREGROUND_COLORSET("'}'", 135) " to select all entries up to how many can fit in the dataset."), lines++;
		printf("\nPress " FOREGROUND_COLORSET("'~'", 160) " to end your selection process."), lines++;
		if (current_entries + selected >= LANGENTRY_MAX - 5)
			printf("\nYou can only add \x1b[38;5;255m\x1b[4m%d more entries\x1b[0m before your entries before "
				"selection automatically stops.", LANGENTRY_MAX - current_entries - selected), lines++; else;
		printf("\n\nPress any of the options above: "), lines += 2;
		if ((ch = _getch()) != '\n')
			printf("%c\n", ch), lines++;
		switch (ch)
		{
			case ',':
				index = index > 0 ? index - 1 : unselected - 1;
				break;
			case '.':
				index = index + 1 < unselected ? index + 1 : 0;
				break;
			case ';':
				ptrinfo.ptr = &imported_contents->entry_list[unselected_indices[index]];
				ptrinfo.original_index = current_entries + ptrlist.count - unselected;
				selected_contents->_ptr_info[selected_contents->count++] = ptrinfo;
				remove_int(unselected_indices, -1, index, &unselected);
				selected++;
				if (current_entries + selected >= LANGENTRY_MAX)
					printf(FOREGROUND_COLORSET("\nYou can no longer add more entries.\n", 132)),
					proceed = 0; 
				else;
				break;
			case '{':
				memset(selected_contents, 0, sizeof(*selected_contents));
				selected = 0;
				proceed = 0;
				break;
			case '}':
				for (iterator = current_entries; iterator - current_entries < unselected && iterator < LANGENTRY_MAX; iterator++)
					ptrinfo.ptr = &imported_contents->entry_list[unselected_indices[iterator - current_entries]],
					ptrinfo.original_index = current_entries,
					selected_contents->_ptr_info[selected_contents->count++] = ptrinfo;
				selected = selected_contents->count;
				proceed = 0;
				break;
			case '~':
				proceed = 0;
				break;
			default:
				break;
		}
		display = 1;
		proceed = proceed && unselected != 0;
	} while (proceed);
	free(unselected_indices);
	return selected;
}

static void layer18e(langentry_l_p contents, langentry_p_l_p selected_contents, langentry_p_l_p sorted_contents)
{
	int i;
	for (i = contents->entries; i < contents->entries + selected_contents->count; i++)
		insert_data(FILE_DS_NRL, selected_contents->_ptr_info[i - contents->entries].ptr, i);
}

static void layer18(int *layer, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	// Declarations
	int selected, cancelled;
	langentry_l imported_contents = { 0 };
	langentry_p_l selected_contents = { 0 };
	filename_t source = { 0 };
	// 18B: Request file
	cancelled = layer18b(source);
	// 18C: Read entries from file
	if (!cancelled)
	{
		layer18c(source, &imported_contents);
		if (contents->entries == 0)
		{
			selected = imported_contents.entries;
			selected_contents = get_entry_p_l(&imported_contents);
			// Success message
		}
		else
			// 18D: Show file contents and prompt for selection
			selected = layer18d(contents->entries, &imported_contents, &selected_contents);
		// 18E: Combine and update .ds-nrl
		// No need to sort since read_data() sorts after reading
		layer18e(contents, &selected_contents, sorted_contents);
		printf(FOREGROUND_COLORSET("\n\n%d contents from ", 154) FOREGROUND_COLORSET("\"%s\"", 202) FOREGROUND_COLORSET(" have been loaded into the dataset.", 154), selected, source);
		printf("\nPress any key to go back to the Manage Data menu...");
		system("pause>nul");
	}
	else SHOW_CANCEL_MSG;
	*layer = 1;
}

static void layer19(int *layer, int *exported, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	char ch, lines, prompt, cancelled;
	ch = 0, lines = 0, cancelled = 0;
	if (!*exported)
	{
		prompt = 1;
		do
		{
			for (; lines-- > 0;)
				printf("\x1b[2K\x1b[1A");
			lines = 0;
			printf(FOREGROUND_COLORSET("\n\nYou seem to have not exported your data yet.", 226)), lines += 2;
			printf("\nAre you sure you want to exit? (Y/N): "), lines++;
			if ((ch = getch()) != '\n')
				printf("%c\n", ch), lines++;
			if (ch == 'N' || ch == 'n')
				cancelled = 1, *exported = -1, SHOW_CANCEL_MSG;
			else if (ch == 'Y' || ch == 'y')
				memset(contents, 0, sizeof(*contents)),
				memset(sorted_contents, 0, sizeof(*sorted_contents));
			else;
			prompt = ch != 'Y' && ch != 'N' && ch != 'y' && ch != 'n';
		} while (prompt);
	}
	else
		*exported = 0, 
		memset(contents, 0, sizeof(*contents)),
		memset(sorted_contents, 0, sizeof(*sorted_contents));
	*layer = cancelled;
}

static int layer21b(strlng_t text, strshrt_t text_language, strshrt_t language_target, int more_translating)
{
	int cancel;
	if (!more_translating)
	{
		printf("\n\nPlease enter the information requested below, or enter " 
			FOREGROUND_COLORSET("\"%%\?%%\"", 177) " to return to the main menu.");
		printf("\nNote that all inputs are \x1b[38;5;255m\x1b[4mcase-sensitive\x1b[0m!");
		printf("\n\nEnter the language of your text: ");
		assert(scanf(SHORT_SCANF, text_language) == 1);
	}
	cancel = strcmp(text_language, "%\?%") == 0;
	if (!cancel)
	{
		if(!more_translating)
		{
			printf("\nEnter the language you want the text to be translated to: ");
			assert(scanf(SHORT_SCANF, language_target) == 1);
		}
		cancel = strcmp(language_target, "%\?%") == 0;
		if (!cancel)
		{
			printf("\nEnter the text from %s that you want to translate to %s:\n", text_language, language_target);
			assert(scanf(LONG_SCANF, text) == 1);
			cancel = strcmp(text, "%\?%") == 0;
		}
	}
	return cancel;
}

static void layer21(int *layer, langentry_p_l_p sorted_contents)
{
	int cancelled, more_translating, lines;
	char ch, *translation = NULL;
	strlng_t text = { 0 };
	strshrt_t text_language = { 0 }, language_target = { 0 };
	more_translating = -1;
	cancelled = 0;
	// Loop 'til user desires
	while (more_translating && !cancelled)
	{
		// 21B: prompt for inputs
		cancelled = layer21b(text, text_language, language_target, more_translating == 1);
		if (!cancelled)
		{
			translate(text, text_language, language_target, sorted_contents, &translation);
			printf("\n\x1b[38;5;193mTranslated from\x1b[0m \x1b[4m%s\x1b[0m \x1b[38;5;193mto\x1b[0m \x1b[4m%s\x1b[0m:", 
				text_language, language_target);
			printf("\n%s\n", translation);
			lines = 0;
			do
			{
				for (; lines-- > 0;)
					printf("\x1b[2K\x1b[1A");
				lines = 0;
				printf("\nDo you want to translate more from \x1b[4m%s\x1b[0m to \x1b[4m%s\x1b[0m? (Y/N): ", 
					text_language, language_target), lines++;
				if ((ch = getch()) != '\n')
					printf("%c\n", ch), lines++;
				if (ch == 'Y' || ch == 'y')
					more_translating = 1;
				else if (ch == 'N' || ch == 'n')
					more_translating = 0;
				else;
			} while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n');
			free(translation);
		}
		else SHOW_CANCEL_MSG;
	}
	*layer = 2;
}

static int layer22b(filename_t target_file, filename_t output_file, strshrt_t text_language, strshrt_t language_target)
{
	FILE *test_open;
	char ch;
	int file_existing, length_ok, cancel, lines;
	// Introduction
	printf("\n\nPlease enter the information requested below.");
	printf("\nNote that all inputs are case-sensitive!");
	// Prompt for file
	cancel = 0, file_existing = 0, length_ok = 0;
	while (!file_existing && !length_ok && !cancel)
	{
		printf("\nEnter the name of the text file to translate below. Maximum of 30 characters including the extension.");
		printf("\nNote that \".txt\" will be appended to your input and that no forbidden characters can be used.");
		printf("\nHowever, you may enter " FOREGROUND_COLORSET("\"%%\?%%\"", 177) " to return to the main menu.");
		printf("\n\nRestricted inputs: \x1b[38;5;226m%s\x1b[0m", FILENAME_FORBIDDEN_SPACED);
		printf("\nInput filename to translate contents of: ");
		assert(scanf(FILENAME_SCANF, target_file) == 1);
		cancel = strcmp(target_file, "%\?%") == 0;
		if (!cancel)
		{
			if ((length_ok = strlen(target_file) <= STRING_FILENAME - 4))
			{
				strcat(target_file, ".txt");
				test_open = fopen(target_file, "r");
				file_existing = test_open != NULL;
				if (file_existing)
					fclose(test_open);
				else
					printf(FOREGROUND_COLORSET("\nThis file does not exist.", 196)),
					printf("\nPlease try again with an existing filename."),
					system("pause");
			}
			else
				printf(FOREGROUND_COLORSET("\nThis file name is too long.", 196)),
				printf("\nPlease try again with a shorter filename."),
				system("pause");
		}
	}
	if (!cancel)
	{
		cancel = 0, file_existing = 1, length_ok = 0;
		while (file_existing && !length_ok && !cancel)
		{
			printf("\nInput filename to write results into: ");
			assert(scanf(FILENAME_SCANF, output_file) == 1);
			cancel = strcmp(output_file, "%\?%") == 0;
			if (!cancel)
			{
				if ((length_ok = strlen(output_file) <= STRING_FILENAME - 4))
				{
					strcat(output_file, ".txt");
					test_open = fopen(output_file, "r");
					file_existing = test_open != NULL;
					if (file_existing)
					{
						printf("\nWARNING! File already exists.");
						lines = 0;
						do
						{
							for (; lines-- > 0;)
								printf("\x1b[2K\x1b[1A");
							lines = 0;
							printf("\nThis will be overridden if you proceed. Continue? (Y/N): "), lines++;
							if ((ch = getch()) != '\n')
								printf("%c\n", ch), lines++;
							if (ch == 'Y' || ch == 'y')
								file_existing = 0;
							else if (ch == 'N' || ch == 'n')
								file_existing = 1;
							else;
						} while (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n');
						if (file_existing)
							printf("\nThis file already exists."),
							printf("\nPlease try again with an available filename."),
							system("pause");
						else;
					}
				}
				else
					printf("\nThis file name is too long."),
					printf("\nPlease try again with a shorter filename."),
					system("pause");
			}
		}
	}
	if (!cancel)
	{
		printf("\nEnter further requested information about your text below.");
		printf("\nYou may enter " FOREGROUND_COLORSET("\"%%\?%%\"", 177) " to return to the main menu.");
		printf("\n\nEnter the language of your file: ");
		assert(scanf(SHORT_SCANF, text_language) == 1);
		cancel = strcmp(text_language, "%\?%") == 0;
	}
	if (!cancel)
	{
		printf("\nEnter the language you want the texts to be translated into: ");
		assert(scanf(SHORT_SCANF, language_target) == 1);
		cancel = strcmp(language_target, "%\?%") == 0;
	}
	return cancel;
}

static void layer22(int *layer, langentry_p_l_p sorted_contents)
{
	FILE *f;
	fpos_t file_size;
	filename_t target_file = { 0 }, output_file = { 0 };
	strshrt_t text_language = { 0 }, language_target = { 0 };
	int iterator, cancel, input_count, translation_count;
	void *realloc_address;
	char **input_set, **translation_set, *file_contents, *token, *newline;
	// 22B: prompt for inputs
	cancel = layer22b(target_file, output_file, text_language, language_target);
	if (!cancel)
	{
		// Phase 1: Read file
		f = fopen(target_file, "r");
		assert(f != NULL);
		fseek(f, 0, SEEK_END);
		fgetpos(f, &file_size);
		rewind(f);
		file_contents = malloc(file_size + 1);
		assert(file_contents != NULL);
		file_contents[fread(file_contents, 1, file_size, f)] = 0;
		fclose(f);
		// Phase 2a: Tokenize; delimited with punctuations
		realloc_address = NULL;
		input_set = NULL;
		input_count = 0;
		token = strtok(file_contents, ".?!");
		while (token != NULL)
		{
			realloc_address = realloc(input_set, ++input_count * sizeof(char *));
			assert(realloc_address != NULL);
			input_set = realloc_address;
			input_set[input_count - 1] = calloc(STRING_LONG + 1, 1);
			assert(input_set[input_count - 1] != NULL);
			strcpy(input_set[input_count - 1], token);
			token = strtok(NULL, ".?!");
		}
		// Phase 2b: Replace all '\n' with spaces
		for (iterator = 0; iterator < input_count; iterator++)
		{
			newline = input_set[iterator];
			while ((newline = strchr(newline, '\n')) != NULL)
				*newline++ = ' ';
		}
		// Phase 3: Translate all
		translation_set = NULL;
		translation_count = 0;
		for (iterator = 0; iterator < input_count; iterator++)
		{
			realloc_address = realloc(translation_set, ++translation_count * sizeof(char *));
			assert(realloc_address != NULL);
			translation_set = realloc_address;
			translate(input_set[iterator], text_language, language_target, sorted_contents, &translation_set[translation_count - 1]);
		}
		// Phase 4: Write to output file and print at the same time
		f = fopen(output_file, "w+");
		assert(f != NULL);
		printf("\n");
		for (iterator = 0; iterator < translation_count; iterator++)
		{
			fwrite(translation_set[iterator], 1, strlen(translation_set[iterator]), f);
			printf("%s\n", translation_set[iterator]);
			if (iterator + 1 < translation_count)
				fprintf(f, "\n");
		}
		fclose(f);
		// Closing message and cleanup
		printf(FOREGROUND_COLORSET("\n\nSuccessfully translated from ", 211) "\x1b[4m%s\x1b[0m" FOREGROUND_COLORSET(" to ", 211) "\x1b[4m%s\x1b[0m"
			FOREGROUND_COLORSET(" and written to ", 211) "\x1b[38;5;21m\x1b[4m\"%s\"\x1b[0m.\n", text_language, language_target, output_file);
		for (iterator = 0; iterator < input_count; iterator++)
			free(input_set[iterator]);
		free(input_set);
		for (iterator = 0; iterator < translation_count; iterator++)
			free(translation_set[iterator]);
		free(translation_set);
		free(file_contents);
	}
	else SHOW_CANCEL_MSG;
	*layer = 2;
}

static void interface_mode(int *layer, int *main_menu, int *exported, int *has_imported, langentry_l_p contents, langentry_p_l_p sorted_contents)
{
	switch (*layer)
	{
		case 0: // Main menu
			layer0(layer);
			break;
		case 1: // Manage Data
			layer1(layer, exported, main_menu, contents, sorted_contents);
			break;
		case 2: // Translate
			layer2(layer, main_menu, has_imported, contents, sorted_contents);
			break;
		case 3: // Exit program
			*layer = -1;
			break;
		case 10: // Add entry... 
			layer10(layer, contents);
			break;
		case 11: // Add translation...
			layer11(layer, contents, sorted_contents);
			break;
		case 12: // Delete entry...
			layer12(layer, contents, sorted_contents);
			break;
		case 13: // Delete translation...
			layer13(layer, contents, sorted_contents);
			break;
		case 14: // Display all entries
			layer14(layer, sorted_contents);
			break;	
		case 15: // Search word...
			layer15(layer, sorted_contents);
			break;
		case 16: // Search translation...
			layer16(layer, sorted_contents);
			break;
		case 17: // Export data
			layer17(layer, exported, sorted_contents);
			break;
		case 18: // Import data
			layer18(layer, contents, sorted_contents);
			break;
		case 19: // Exit Manage Data Menu
			layer19(layer, exported, contents, sorted_contents);
			break;
		case 21: // Translate input
			layer21(layer, sorted_contents);
			break;
		case 22: // Translate file
			layer22(layer, sorted_contents);
			break;
		case 23: // Exit Translate Menu
			memset(contents, 0, sizeof(*contents));
			memset(sorted_contents, 0, sizeof(*sorted_contents));
			*has_imported = 0;
			*layer = 0;
			break;
		default:
			*layer = 0;
			break;
	}
}

int main(void)
{
	FILE *f;
	fpos_t s;
	langentry_l contents = { 0 };
	langentry_p_l sorted = { 0 };
	// Main menu
	int state = 1, main_menu = 1, layer = 0;
	// Manage Data
	int exported = 0;
	// Translate
	int has_imported = 0;

	// A short Batch script code to change the code page
	// Also displays a reminder to not resize the window
	system(
		"chcp 65001>nul&"
		"echo.&"
		"echo To maintain the correctness of the display, please do not resize this console window.&"
		"pause&"
		"echo \x1b[2K\x1b[1A\x1b[2K\x1b[1A\x1b[2K\x1b[1A"
	);
	while (state)
	{
		f = fopen(FILE_DS_NRL, "r");
		if (f != NULL)
		{
			fseek(f, 0, SEEK_END);
			fgetpos(f, &s);
			exported = exported == -1 ? -1 : s <= 0;
			fclose(f);
		}
		else
		{
			f = fopen(FILE_DS_NRL, "w");
			fclose(f);
		}
		interface_mode(&layer, &main_menu, &exported, &has_imported, &contents, &sorted), state = layer != -1;
	}
	return 0;
}

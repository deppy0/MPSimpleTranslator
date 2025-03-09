#include "mpfunct.c"

int main(void)
{
	// Enable ANSI escape sequences by changing the code page.
	// '> nul' hides the output message from the system command.
	system("chcp 65001 > nul");
	
	// Where the entries will be stored throughout the lifetime of the program.
	langentry_l dataset;
	int current_entries = 0;
	
	// Test mode
	int i;
	for (i = 0; i < 10; i++)
		add_entry(&dataset, &current_entries);

	display_all_entries(&dataset);
	add_entry(&dataset, &current_entries);
	display_all_entries(&dataset);
}

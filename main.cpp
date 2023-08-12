#include "Flotation_optimisation.h"
#include <conio.h>  // required for _getch()
// other required libraries are in "Flotation_optimisation.h" and "functions_hub.cpp"

// function to ask users a yes-no question
void ask_yes_no(string question, char* ans)
{
	do
	{
		cout << question;
		*ans = tolower(_getch());  // recieve a character from users ('Y' = 'y' = yes, 'N' = 'n' = no)
		cout << endl;
		// let users know if they type a character that is not 'Y', 'y', 'N', 'n' and start a new loop to re-ask
		if (*ans != 'y' && *ans != 'n') cout << "Wrong character, try again." << endl;
	} while (*ans != 'y' && *ans != 'n');
}

int main()
{
	cout << "Welcome to our program for the financial optimisation of flotation.\n\n";  // welcoming message

	// declare char variables for storing 'y' or 'n' as pointers so that they can change dynamically when being passed through the "ask_yes_no" function
	char* new_data_file, * new_plant;
	// allocate memory for them
	new_data_file = new char;
	new_plant = new char;
	// set default values as 'y'
	*new_data_file = 'y';
	*new_plant = 'y';

	// loop if users want to add data of a new plant
	while (*new_plant == 'y')
	{
		// declare a Flotation_down_a_bank object 
		Flotation_optimisation Plant;
		// bool variable representing if a file can be opened or the data of a file are valid (true by default)
		bool is_file_valid = true;
		// ask users for new data
		do
		{
			// ask users if they want to import a data file
			string ask_data = "Would you like to import your data as a file, otherwise you will need to type your data on console?\n(y/n): ";
			ask_yes_no(ask_data, new_data_file);

			if (*new_data_file == 'y')  // if users choose to import a file, ask them for the filename
			{
				string name;
				cout << "What is your .txt data file name (don't include '.txt')?: ";
				getline(cin, name);
				is_file_valid = Plant.read_file(name);
			}
			else  // if users choose to type data on console
			{
				Plant.read_console();
				Plant.write_file_for_input();  // write a .txt file from the data inputted on the console for future input
				break;  // exit the loop as data have already been given via console
			}
		} while (!is_file_valid);  // loop to ask a user again for data if the file cannot be opened or is invalid

		Plant.show_input();  // show inputted data on console
		Plant.calculate();  // use inputted data to calculate
		Plant.write_calculated_file();  // write a .txt file containing the result from the calculation
		Plant.optimum();  // evaluate the optimal point of flotation

		// ask a user if users want to add the data of another flotation plant
		string ask_do_again = "Would you like to add the data of a new plant? (y/n): ";
		ask_yes_no(ask_do_again, new_plant);
	}

	cout << "Goodbye!" << endl;  // goodbye message

	// de-allocate memory of the pointers
	delete new_data_file;
	delete new_plant;

	return 0;  // exit the program
}
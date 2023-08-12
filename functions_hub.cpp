#include "Flotation_optimisation.h"  // <iostream>, <vector>, <string> libraries already included
#include <conio.h>  // required for _getch()
#include <fstream>  // required for std:: fstream
#include <sstream>  // required for std:: stringstream, getline
#include <algorithm>  // required for std:: sort
// #include <iomanip>  // required for std:: setprecision

using namespace std;

/*** Functions rquired for the implementation of the member functions of the Flotation_optimisation class ***/

// function to round a number to 1 decimal place
#define to_1_dec(d) (round(d * 10.) / 10.)

// function to divide a number by 1000 to make it in kilo unit
#define to_k(d) (d / 1000.)

// function to check if x is between two values 
bool is_between(double val1, double val2, double x) { return ((x >= val1 && x <= val2) || (x <= val1 && x >= val2)) ? 1 : 0; }

// function to find a factorial using a recursive algorithm
int factorial(int n) { return (n == 0) ? 1 : n * factorial(n - 1); }

// functions overloading vector operators
vector<double> operator-(vector<double> &v1, vector<double> &v2)  // subtract a vector with another vector
{
	vector<double> result;
	if (v1.size() != v2.size() || v1.size() == 0 || v2.size() == 0) return result;  // return an empty vector if the sizes of the two vectors are different or zero
	for (int i = 0; i < v1.size(); i++) result.push_back(v1[i] - v2[i]);  // otherwise, subtract a vector with another vector
	return result;
}
vector<double> operator-(double &s, vector<double> &v)  // subtract a scalar with a vector
{
	vector<double> result;
	for (int i = 0; i < v.size(); i++) result.push_back(s - v[i]);
	return result;
}
vector<double> operator*(vector<double> v1, vector<double> v2)  // multiply the elements at the same position of two vectors
{
	vector<double> result;
	if (v1.size() != v2.size() || v1.size() == 0 || v2.size() == 0) return result;  // return an empty vector if the sizes of the two vectors are different or zero
	for (int i = 0; i < v1.size(); i++) result.push_back(v1[i] * v2[i]);  // otherwise, multiply the elements at the same position of two vectors
	return result;
}
vector<double> operator*(vector<double> v, double s)  // multiply a vector by a scalar
{
	vector<double> result;
	for (int i = 0; i < v.size(); i++) result.push_back(v[i] * s);
	return result;
}
vector<double> operator/(vector<double> v1, vector<double> v2)  // divide the elements at the same position of two vectors
{
	vector<double> result;
	if (v1.size() != v2.size() || v1.size() == 0 || v2.size() == 0) return result;  // return an empty vector if the sizes of the two vectors are different or zero
	for (int i = 0; i < v1.size(); i++) result.push_back(v1[i] / v2[i]);  // otherwise, divide the elements at the same position of two vectors
	return result;
}
vector<double> operator/(vector<double> v, double s)  // divide a vector by a scalar
{
	vector<double> result;
	for (int i = 0; i < v.size(); i++) result.push_back(v[i] / s);
	return result;
}

// struct storing the value of an element (val) and its position (pos) in an array
struct val_pos_in_array 
{
	double val;
	int pos;
};

// function to return the value and position of the maximum element in an array
val_pos_in_array max_val_pos(double *arr, int size)
{
	val_pos_in_array max_in_array;  // initialise a "val_pos_in_array" struct to store the maxmimum element and its position in an array
	// initially assume that the first element is the maximum
	double max_val = arr[0];  // the value of the maximum
	int max_pos = 0;  // the position of the maximum

	// loop over the elements to find the maximum
	for (int i = 1; i < size; i++)
	{
		// if the next value is higher than the current maximum, it will be the new maximum
		if (arr[i] > max_val)
		{
			max_val = arr[i];
			max_pos = i;
		}
	}
	// store the value and position of the maximum in the struct
	max_in_array.val = max_val;
	max_in_array.pos = max_pos;
	return max_in_array;  // return the struct
}

// function to solve a square system of linear equations Ax=b (n variables and n equations)
vector<double> solve_square_linear_system(vector<vector<double>> A, vector<double> b) 
{
	int n = A.size();  // the number of equations
	vector<double> x(n);  // initialise a vector to store the solution
	if (A[0].size() != n || b.size() != n) return x;  // return a zero vector if the system is not square
	// loop over rows of A
	for (int i = 0; i < n - 1; i++)  
	{
		int pivot = i;  // set the pivot row to the current row
		for (int j = i + 1; j < n; j++)  // search for a row with a larger pivot
		{
			if (abs(A[j][i]) > abs(A[pivot][i])) pivot = j;
		}
		if (pivot != i)  // swap the pivot row with the current row
		{ 
			swap(A[pivot], A[i]);
			swap(b[pivot], b[i]);
		}
		for (int j = i + 1; j < n; j++)  // eliminate the i-th column in rows below the current row
		{ 
			double factor = A[j][i] / A[i][i];  // calculate the elimination factor to eliminate the i-th variable from the j-th equation
			for (int k = i; k < n; k++) A[j][k] -= factor * A[i][k];  // apply the elimination factor to the row
			b[j] -= factor * b[i];  // apply the elimination factor to b
		}
	}
	// perform back-substitution to find the solution
	for (int i = n - 1; i >= 0; i--) 
	{ 
		x[i] = b[i];
		for (int j = i + 1; j < n; j++) x[i] -= A[i][j] * x[j];
		x[i] /= A[i][i];
	}
	return x;  // return the solution
}

// function to convert string to double using stod() function and return an error if the convertion is unsuccessful
double str_to_double(string str)
{
	double d;  // a variable storing the converted double
	try  // convert str to double
	{
		d = stod(str);
	}
	catch (const invalid_argument&)  // error if the convertion is unsuccessful
	{
		cerr << "Error: non-numeric value(s) are found.\n";
		return -999999;
	}
	return d;  // return the double
}

// function to check if a file can be opened
bool is_opened(const fstream &file)
{
	if (file.fail())  // if it's fail to be opened
	{
		cout << "Error: the file cannot be opened.\n";
		return false;
	}
	return true;
}

// function to exit the program if "quit" is inputted
void goodbye(string str)
{
	if (str == "quit")
	{
		cout << "\nGoodbye!\n";
		exit(0);
	}
}

// function to check if there is at least one type of characters in "char_set" in a string
bool check_chars_in_str(string str, char* char_set)
{
	// loop to check if the string contains a character in "char_set"
	for (int i = 0; i < strlen(char_set); i++)
	{
		if (str.find(char_set[i]) != string::npos) return true;  // if the character is found, return true to exit the function
	}
	return false;  // if all the characters in "char_set" are not found, return false
}

/*** member functions of Flotation_optimisation ***/

// default constructor - set the initial values
Flotation_optimisation::Flotation_optimisation()
{
	clear_single_values();
}

// find_grade - function to calculate the grade of a stream from the flowrates of the stream and that of metal in the stream
// grade = (flowrate of metal in a stream / flowrate of the stream) * 100%
double Flotation_optimisation::find_grade(double m_stream, double stream) { return m_stream / stream * 100.; }
vector<double> Flotation_optimisation::find_grade(vector<double> m_stream, vector<double> stream) { return m_stream / stream * 100.; }

// find_metal_in_conc - function to calculate the flowrate of metal in concentrate from feed flowrate and recovery
// flowrate of metal in concentrate = (recovery / 100%) * feed flowrate
double Flotation_optimisation::find_metal_in_conc(double recov, double m_feed) { return recov / 100. * m_feed; }
vector<double> Flotation_optimisation::find_metal_in_conc(vector<double> recov, double m_feed) { return recov / 100. * m_feed; }

// find_metal_in_stream - function to calculate the flowrate of metal in a stream from the flowrate and grade of the stream
// flowrate of metal in a stream = flowrate of the stream * (grade of the stream / 100%)
double Flotation_optimisation::find_metal_from_stream(double stream, double grade) { return stream * grade / 100.; }
vector<double> Flotation_optimisation::find_metal_from_stream(vector<double> stream, vector<double> grade) { return stream * grade / 100.; }

// find_stream_from_metal - function to calculate the flowrate of a stream from the flowrate of metal in the stream and the grade of the stream 
// flowrate of a stream = flowrate of metal in a stream / (grade of the stream / 100%)
double Flotation_optimisation::find_stream_from_metal(double metal, double grade) { return metal * 100. / grade; }
vector<double> Flotation_optimisation::find_stream_from_metal(vector<double> metal, vector<double> grade) { return metal * 100. / grade; }

// find_gain - function to calculate the revenue from selling metal in concentrate
// revenue from selling metal = flowrate of metal in concentrate * price per ton of metal
double Flotation_optimisation::find_gain(double metal) { return metal * metal_price; }
vector<double> Flotation_optimisation::find_gain(vector<double> metal) { return metal * metal_price; }

// find_loss - function to calculate the treatment charge for selling concentrate
// treatment charge for selling concentrate = concentrate flowrate * treatment charge per ton of concentrate 
double Flotation_optimisation::find_loss(double conc) { return conc * treatment_charge; }
vector<double> Flotation_optimisation::find_loss(vector<double> conc) { return conc * treatment_charge; }

// function to clear the values of member variables that are single values (not vectors)
void Flotation_optimisation::clear_single_values()
{
	num_conditions = 0;
	metal_price = 0.;
	treatment_charge = 0.;
	flowrate_feed = 0.;
	grade_feed = 0.;
	flowrate_metal_in_feed = 0.;
	plant_name = "Unknown";
}

// function to clear the member variables that are vectors
void Flotation_optimisation::clear_vectors()
{
	flowrate_conc.clear(); 
	flowrate_metal_in_conc.clear();
	grade_conc.clear();
	recovery.clear();
	flowrate_tail.clear();
	flowrate_metal_in_tail.clear();
	grade_tail.clear();
	gain.clear(); 
	loss.clear(); 
	profit.clear();
	grade_recov_spline_coeff.clear();
}

// function to clear data
void Flotation_optimisation::clear_data()
{
	clear_single_values();
	clear_vectors();
}

// enum defining the stage numbers to be used in the "check_data" function
enum Flotation_optimisation::check_stage
{
	check_number,  // 0
	check_metal_price,  // 1
	check_treatment_charge,  // 2
	check_flowrate_feed,  // 3
	check_grade_feed,  // 4
	check_num_conditions,  // 5
	check_grade_conc_and_recovery,  // 6
};

// function to check if input data are valid
bool Flotation_optimisation::check_data(check_stage stage, string str, string str2)
{
	bool return_value = true;  // bool variable to be returned (true by default)
	
	// exit the program if "quit" is received
	goodbye(str);
	goodbye(str2);

	// check if input is numeric
	double temp = str_to_double(str);  // convert str to double
	if (temp == -999999) return false;  // it's non-numeric --> return false and stop further checking

	// if the second string was also sent to check
	double temp2 = 0;  // initialise another variable to store the converted double
	if (str2 != "")
	{
		temp2 = str_to_double(str2);  // convert str2 to double
		if (temp2 == -999999) return false;  // it's non-numeric --> return false and stop further checking
	}

	// if input is numeric, check if it is valid
	if (return_value)
	{
		switch (stage)
		{
			case 0:  // nothing else except for checking if input is numeric
				break;
			case 1:  // check metal_price
			{
				metal_price = temp;  // add to the member variable
				if (temp <= 0)  // return false if it's non-positive
				{
					cout << "The value of metal price must be positive.\n";
					return false;
				}
				break;
			}
			case 2:  // check treatment_charge
			{
				treatment_charge = temp;  // add to the member variable
				if (treatment_charge <= 0)  // return false if it's non-positive
				{
					cout << "The value of treatment charge must be positive.\n";
					return false;
				}
				break;
			}
			case 3:  // check flowrate_feed
			{
				flowrate_feed = temp;  // add to the member variable
				if (temp <= 0)  // return false if it's non-positive
				{
					cout << "The value of flowrate must be positive.\n";
					return false;
				}
				break;
			}
			case 4:  // check grade_feed
			{
				grade_feed = temp;  // add to the member variable
				if (temp <= 0 || temp >= 100)  // return false if it's outside 0-100 range
				{
					cout << "The value of grade must be between 0 and 100.\n";
					return false;
				}
				flowrate_metal_in_feed = find_metal_from_stream(flowrate_feed, grade_feed);  // otherwise, calculate the flowrate of metal in the feed
				break;
			}
			case 5:  // check num_conditions - must be at least 2 so that there are enough data points to create a curve
			{
				num_conditions = temp;  // convert to int automatically and add to the member variable
				if (num_conditions < 2)  // return false if it's less than 2 - interpolation cannot be done
				{
					cout << "The number of flotation conditions must be at least 2.\n";
					return false;
				}
				else if (num_conditions < 5)  // ask users if they still want to use their data whose numbers of grade and recovery values are less than 5
				{
					cout << "More various flotation conditions likely lead to a more accurate interpolation.\n";
					char ans;
					do
					{
						cout << "Are you sure to proceed (y/n)?: ";
						ans = tolower(_getch());  // recieve a character from users ('Y' = 'y' = yes, 'N' = 'n' = no)
						cout << endl;
						if (ans == 'n') return false;  // return false to exit the function if they don't want to do so
						else if (ans != 'y') cout << "Wrong character, try again." << endl;  // let users know if they type a character that is not 'Y', 'y', 'N', 'n'
					} while (ans != 'y');  // ask users again if they type a character that is not 'Y', 'y' (if 'N' or 'n' is inputted, exit the function)
				}
				break;
			}
			case 6:  // check grade_conc and recovery
			{
				// note that
				// temp = an inputted value of recovery
				// temp2 = an inputted value of grade_conc

				// first-order validity check
				if (temp <= 0 || temp >= 100)  // return false if recovery is outside 0-100 range
				{
					cout << "The value of recovery must be between 0 and 100.\n";
					return_value = false;
				}
				if (temp2 <= grade_feed || temp2 >= 100)  // return false if recovery is not more than feed grade and less than 100
				{
					cout << "The value of grade must be more than the grade of the feed but less than 100.\n";
					return_value = false;
				}

				// the new grade value must not be too low so the flowrate of the concentrate is higher than that of the feed
				double flowrate_metal_in_conc_temp = find_metal_in_conc(temp, flowrate_metal_in_feed);  // the flowrate of metal in the concentrate
				double flowrate_conc_temp = find_stream_from_metal(flowrate_metal_in_conc_temp, temp2);  // the flowrate of the concentrate
				if (flowrate_conc_temp >= flowrate_feed)  // return false if the flowrate of the concentrate is higher than that of the feed
				{
					cout << "The grade is too low - the flowrate of the concentrate must be less than that of the feed.\n";
					return_value = false;
				}

				// the new recovery or grade value must not already exist in the vectors 
				auto find_new_recovery = find(recovery.begin(), recovery.end(), temp);  // find the new recovery value in the "recovery" vector
				auto find_new_grade_conc = find(grade_conc.begin(), grade_conc.end(), temp2);  // find the new grade value in the "grade_conc" vector
				if (find_new_recovery != recovery.end())  // return false if the new reovery already exists
				{
					cout << "This recovery value must not be the same as the previous ones.\n";
					return_value = false;
				}
				if (find_new_grade_conc != grade_conc.end())  // return false if the new grade already exists
				{
					cout << "This grade value must not be the same as the previous ones.\n";
					return_value = false;
				}

				// if the new values are valid and have not yet existed in the relevant vectors
				// add them to those vectors and check if they agree with the grade-recovery inverse relationship
				// that concentrate grade decreases as recovery increases
				if (return_value)
				{
					recovery.push_back(temp);  // add the new recovery to the "recovery" vector
					grade_conc.push_back(temp2);  // add the new grade to the "grade_conc" vector

					// if this is not the first condition
					if (recovery.size() > 1)
					{
						// sort values in the "recovery" vector in ascending order
						sort(recovery.begin(), recovery.end());
						// sort values the "grade_conc" vector in descending order since concentrate grade decreases as recovery increases
						sort(grade_conc.begin(), grade_conc.end(), [](int a, int b) { return a > b; });

						// after being sorted, the position of the new value in the "recovery" vector 
						// must be the same as that of the new value in the "grade_conc" vector
						// find the indices of the new values in the "recovery" and "grade_conc" vectors 
						int new_recovery_index = distance(recovery.begin(), find(recovery.begin(), recovery.end(), temp));
						int new_grade_index = distance(grade_conc.begin(), find(grade_conc.begin(), grade_conc.end(), temp2));

						// pop the new values out of the vectors if they disagree with the grade-recovery inverse relationship
						if (new_recovery_index != new_grade_index)
						{
							cout << "Grade must decrease as recovery increases.\n";
							recovery.erase(recovery.begin() + new_recovery_index);
							grade_conc.erase(grade_conc.begin() + new_grade_index);
							return_value = false;
						}
					}
				}
				break;
			}
			default:  // the stage number is not 0-6, not existing in the "check_stage" enum
			{
				cout << "Internal error!\n";
				return false;
			}
		}
	}
	return return_value;  // return true if input can be used, otherwise return false
}

/*
Function to read the data in a .txt file.
The file is tab-delimited and has a specific format:

Your file name will be the name of your flotation plant
Line 1: The string "Price per ton of metal ($)" followed by a numeric value.
Line 2: The string "Treatment charge per ton of concentrate ($)" followed by a numeric value.
Line 3: The string "Feed flowrate (tph)" followed by a numeric value.
Line 4: The string "Feed grade (%)" followed by a numeric value.
Line 5: The string "Recovery (%)" followed by a series of numeric values (at least 2 values), each of which is the recovery achieved by the plant at each different condition.
Line 6: The string "Concentrate grade (%)" followed by a series of numeric values (at least 2 values) , each of which is the recovery achieved by the plant at each different condition.

File example (also see in demo.txt):

Price per ton of metal ($)	8900
Treatment charge per ton of concentrate ($)	1000
Feed flowrate (tph)	1000
Feed grade (%)	0.6
Recovery (%)	70	75	80	85	90	95
Concentrate grade (%)	27	26.5	26	25	21	2
*/
bool Flotation_optimisation::read_file(string filename)
{
	clear_data();  // clear previous data

	// bool variable used to mark file and data checks (true by default)
	bool is_valid = true;
	// temporary vectors storing the data of the recovery and concentrate grade from a file
	vector<string> str_recovery, str_grade;
	// declare an fstream object for a data file
	fstream input_file;
	// open the file to read (note that the filename must include ".txt")
	input_file.open(filename + ".txt", fstream::in);
	// return false to exit the function if the file cannot be opened
	if (!is_opened(input_file)) return false;
	// if the file can be opened, // the filename will be the name of a flotation plant
	// no need of checking if the filename is valid because the name must be initially valid when creating a file in the file explorer
	else plant_name = filename;

	// extract values from the file
	string line;  // declare a string variable storing a line of the file
	// loop to read each line in the file
	while (getline(input_file, line)) 
	{ 
		stringstream ss(line);  // create a stringstream from the line
		string substr;  // declare a string variable storing a substring between two '\t'

		// skip to the next line if '\t' is not found
		size_t tab_position = line.find('\t');
		if (tab_position == string::npos)
		{
			is_valid = false;
			continue;
		}

		// get the first substr (up to '\t') from the stringstream
		getline(ss, substr, '\t');
		// check which variable the current row corresponds to and extract the value accordingly
		if (substr == "Price per ton of metal ($)")
		{
			getline(ss, substr, '\t');  // extract value after "Price per ton of metal ($)"
			if (!check_data(check_metal_price, substr, ""))  // check the substr
			{
				is_valid = false;
				break;
			}
		}
		else if (substr == "Treatment charge per ton of concentrate ($)")
		{
			getline(ss, substr, '\t');  // extract value after "Treatment charge per ton of concentrate ($)"
			if (!check_data(check_treatment_charge, substr, ""))  // check the substr
			{
				is_valid = false;
				break;
			}
		}
		else if (substr == "Feed flowrate (tph)")
		{
			getline(ss, substr, '\t');  // extract value after "Feed flowrate (tph)"
			if (!check_data(check_flowrate_feed, substr, ""))  // check the substr
			{
				is_valid = false;
				break;
			}
		}
		else if (substr == "Feed grade (%)")
		{
			getline(ss, substr, '\t');  // extract value after "Feed grade (%)"
			if (!check_data(check_grade_feed, substr, ""))  // check the substr
			{
				is_valid = false;
				break;
			}
		}
		else if (substr == "Recovery (%)")
		{
			while (getline(ss, substr, '\t'))  // extract values after "Recovery (%)" and add to the "str_recovery" vector
			{
				str_recovery.push_back(substr);
			}
		}
		else if (substr == "Concentrate grade (%)")
		{
			while (getline(ss, substr, '\t'))  // extract values after "Concentrate grade (%)" and add to the "str_grade" vector
			{
				str_grade.push_back(substr);
			}
		}
		else is_valid = false;  // the file has an incorrect format - there are irrelevant lines
	}

	// close the file
	input_file.close();

	// further check
	if (is_valid)
	{
		int num_recovery = str_recovery.size();  // the number of recovery values
		int num_grade = str_grade.size();  // the number of values of concentrate grades 

		// check if the data of the flowrate and grade of concentrates are in pair to each other
		// if yes, the number of pairs will be the number of conditions
		if (num_recovery == num_grade)
		{
			num_conditions = num_recovery;
			// check num_conditions
			is_valid = check_data(check_num_conditions, to_string(num_conditions), "");
			// if the number of conditions is not too few, check the data of the flowrate and grade of concentrates
			if (is_valid)
			{
				for (int i = 0; i < num_conditions; i++)
				{
					is_valid = check_data(check_grade_conc_and_recovery, str_recovery[i], str_grade[i]);
					if (!is_valid) break;  // exit the loop if some values are invalid
				}
			}
		}
		// if not, data cannot be used
		else
		{
			cout << "The data of the flowrate and grade of concentrates are not in pair to each other.\n";
			is_valid = false;
		}
	}

	// let users know if some data in the file are invalid or incomplete
	if (!is_valid) cout << "Some data are invalid or incomplete, or the file does not follow the format.\n\n";
	return is_valid;  // return true if all the values are valid, otherwise return false
}

// function to receive data on console
void Flotation_optimisation::read_console()
{
	clear_data();  // clear previous data

	// ask users for the name of a flotation plant and check if the name of the plant is valid
	// it cannot be empty, start by a space, and contain any of the special characters (tab, /, \, :, *, ^, “, <, >, |).
	bool valid_name = false;  // bool variable storing the validity of the name - false by default
	while (!valid_name)  // loop to ask user for a name, check its validity, and re-ask if they gave an invalid name
	{
		cout << endl << "Plant name: ";  // ask
		getline(cin, plant_name);  // receive name from users (allow space between characters)
		char invalid_char[10] = { '\t', '/', '\\', ':', '*', '^', '\"', '<', '>', '|' };  // declare a set of the special characters
		if (plant_name == "") cout << "The name cannot be empty.\n";  // if the name is empty
		else if (plant_name.find(' ') == 0) cout << "The name cannot start by a space.\n";  // if the name starts by space
		else if (check_chars_in_str(plant_name, invalid_char))  // if the name has at least one of the special characters
			cout << "The name cannot contain any of the special characters (tab, /, \\, :, *, ^, \", <, >, |).\n";
		else valid_name = true;  // exit the loop if the name is valid
	}

	// message to tell users to input their data
	cout << "\nPlease input your data. If you would like to quit the program, you can type 'quit' and enter.\n\n";

	// ask users for data and re-ask if the input is invalid
	string temp, temp2;  // temporary variables storing input
	do  // receive and check metal_price
	{
		cout << "Price per ton of metal ($): ";
		cin >> temp;
		cin.ignore(INT_MAX, '\n');
	} while (!check_data(check_metal_price, temp, ""));
	do  // receive and check treatment_charge
	{
		cout << "Treatment charge per ton of concentrate ($): ";
		cin >> temp;
		cin.ignore(INT_MAX, '\n');
	} while (!check_data(check_treatment_charge, temp, ""));
	do  // receive and check flowrate_feed
	{
		cout << "Feed flowrate (tph) = ";
		cin >> temp;
		cin.ignore(INT_MAX, '\n');
	} while (!check_data(check_flowrate_feed, temp, ""));
	do  // receive and check grade_feed
	{
		cout << "Feed grade (%) = ";
		cin >> temp;
		cin.ignore(INT_MAX, '\n');
	} while (!check_data(check_grade_feed, temp, ""));
	do  // receive and check num_conditions
	{
		cout << "The number of conditions: ";
		cin >> temp;
		cin.ignore(INT_MAX, '\n');
	} while (!check_data(check_num_conditions, temp, ""));
	for (int i = 1; i <= num_conditions; i++)
	{
		cout << "Condition " << i << ":\n";
		do  // receive and check recovery and grade_conc
		{
			cout << "Recovery (%) = ";
			cin >> temp;
			cin.ignore(INT_MAX, '\n');
			goodbye(temp);
			cout << "Concentrate grade (%) = ";
			cin >> temp2;
			cin.ignore(INT_MAX, '\n');
		} while (!check_data(check_grade_conc_and_recovery, temp, temp2));
	}
}

// function to show inputted data on console
void Flotation_optimisation::show_input()
{
	cout << endl << "Your data:" << endl;
	cout << "Price per ton of metal ($)\t" << metal_price << endl;
	cout << "Treatment charge per ton of concentrate ($)\t" << treatment_charge << endl;
	cout << "Feed flowrate (tph)\t" << flowrate_feed << endl;
	cout << "Feed grade (%)\t" << grade_feed << endl;
	cout << "Recovery (%)";
	for (auto r : recovery) cout << '\t' << r;
	cout << endl << "Concentrate grade (%)";
	for (auto g : grade_conc) cout << '\t' << g;
	cout << endl;
}

// function to calculate data
void Flotation_optimisation::calculate()
{
	flowrate_metal_in_conc = find_metal_in_conc(recovery, flowrate_metal_in_feed);  // calculate the flowrate of metal in concentrate
	flowrate_metal_in_tail = flowrate_metal_in_feed - flowrate_metal_in_conc;  // calculate the flowrate of metal in tailings
	flowrate_conc = find_stream_from_metal(flowrate_metal_in_conc, grade_conc);  // calculate the flowrate of concentrate
	flowrate_tail = flowrate_feed - flowrate_conc;  // calculate the flowrate of tailings
	grade_tail = find_grade(flowrate_metal_in_tail, flowrate_tail);  // calculate the grade of tailings
	gain = find_gain(flowrate_metal_in_conc);  // calculate the revenue from selling metal in concentrate
	loss = find_loss(flowrate_conc);  // calculate the treatment charge for selling concentrate
	profit = gain - loss;  // calculate the profit
}

// function to write a file containing the result of the calculation
void Flotation_optimisation::write_calculated_file()
{
	// define an fstream object that will be the output file
	fstream output_file;
	// open the file to write (the filename is the name of the flotation plant appended by "_calculated.txt")
	output_file.open(plant_name + "_calculated.txt", fstream::out);
	// if the file can be opened, write it
	if (is_opened(output_file))
	{
		output_file << "Plant name\t" << plant_name << endl;
		output_file << "Price per ton of metal ($)\t" << metal_price << endl;
		output_file << "Treatment charge per ton of concentrate ($)\t" << treatment_charge << endl;
		output_file << "Feed flowrate (tph)\t" << flowrate_feed << endl;
		output_file << "Feed grade (%)\t" << grade_feed << endl;
		output_file << "Flowrate of metal in feed (tph)\t" << flowrate_metal_in_feed << "\n";
		output_file << endl << " ";
		for (int i = 0; i < num_conditions; i++) output_file << "\tCondition " << i + 1;
		output_file << endl << "Recovery (%)";
		for (auto r : recovery) output_file << '\t' << r;
		output_file << endl << "Concentrate flowrate (tph)";
		for (auto fc : flowrate_conc) output_file << '\t' << fc;
		output_file << endl << "Flowrate of metal in concentrate (tph)";
		for (auto fmic : flowrate_metal_in_conc) output_file << '\t' << fmic;
		output_file << endl << "Concentrate grade (%)";
		for (auto gc : grade_conc) output_file << '\t' << gc;
		output_file << endl << "Tailings flowrate (tph)";
		for (auto ft : flowrate_tail) output_file << '\t' << ft;
		output_file << endl << "Flowrate of metal in tailings (tph)";
		for (auto fmit : flowrate_metal_in_tail) output_file << '\t' << fmit;
		output_file << endl << "Tailings grade (%)";
		for (auto gt : grade_tail) output_file << '\t' << gt;
		output_file << endl << "Revenue from selling metal ($/hr)";
		for (auto g : gain) output_file << '\t' << g;
		output_file << endl << "Treatment charge ($/hr)";
		for (auto l : loss) output_file << '\t' << l;
		output_file << endl << "Profit ($/hr)";
		for (auto p : profit) output_file << '\t' << p;
	}

	// close the file
	output_file.close();  
}

void Flotation_optimisation::write_file_for_input()
{
	// define an fstream object that will be the output file
	fstream output_file;
	// open the file to write (the filename is the name of the flotation plant appended by "_for_input.txt")
	output_file.open(plant_name + "_for_input.txt", fstream::out);
	// if the file can be opened, write it
	if (is_opened(output_file))
	{
		output_file << "Price per ton of metal ($)\t" << metal_price << endl;
		output_file << "Treatment charge per ton of concentrate ($)\t" << treatment_charge << endl;
		output_file << "Feed flowrate (tph)\t" << flowrate_feed << endl;
		output_file << "Feed grade (%)\t" << grade_feed << endl;
		output_file << "Recovery (%)";
		for (auto r : recovery) output_file << '\t' << r;
		output_file << endl << "Concentrate grade (%)";
		for (auto gc : grade_conc) output_file << '\t' << gc;
	}

	// close the file
	output_file.close();
}

// function to calculate the coefficients of each segment of the spline interpolated from inputted data
void Flotation_optimisation::find_grade_recov_spline_coeff()
{
	/*
	n data points require n-1 spline functions.

	The coefficients of the spline functions can be found by solving the system of linear equation Ac = b where
		A = the coefficient matrix - acquired from substituting x at each data point to relevant equations - see below
		c = the vector of the coefficients of all the segments of the spline - 4(n-1) in length as there are 4 coefficients in a cubic equation
		b = the vector of the RHS of those relevant equations

	Relevant equations (i means ordinal positions; 0 to n-1 for data points, and 0 to n-2 for splines):
		row 0 to 2(n-1)-1 --> the values of cubic splines at the data points:
			a_i*x_i^3 + b_i*x_i^2 + c_i*x_i + d_i = y_i (at (x_i, y_i))
			a_i*x_(i+1)^3 + b_i*x_(i+1)^2 + c_i*x_(i+1) + d_i = y_(i+1) (at (x_(i+1), y_(i+1))
			i = 0, ..., n-2
		row 2(n-1) to 3(n-1)-2 --> the values of the 1st derivative of the splines at the interior data points are the same
			(3*a_(i-1)*x_i^2 + 2*b_(i-1)*x_i + c_(i-1)) - (3*a_i*x_i^2 + 2*b_i*x_i + c_i) = 0
			i = 1, ..., n-2; RHS = 0
		row 3(n-1)-1 to 4n-7: --> the values of the 2nd derivative of the splines at the interior data points are the same
			(6*a_(i-1)*x_i + 2*b_(i-1)) - (6*a_i*x_i + 2*b_i) = 0
			i = 1, ..., n-2; RHS = 0
		row 4n-6 to 4n-5 --> the boundary conditions of natural cubic spline - the values of the 2nd derivative of the splines at the end data points are 0
			6*a_0*x_0 + 2*b_0 = 0
			6*a_(n-2)*x_(n-1) + 2*b_(n-2) = 0
	
	The total number of the rows of A is 4(n-1), and the total number of the coefficients of all the segments of the spline is 4(n-1).
	So, A is a square matrix with a size of 4(n-1) x 4(n-1).
	The size of c is 4(n-1)
	The number of equations equals to the number of the rows of A, so the size of b is 4(n-1).
	Therefore, Ac=b is a square linear system, so it can be solved using the "solve_square_linear_system" function.
	*/

	// initialise A and b as zero matrix/vector
	int size_system = 4 * (num_conditions - 1);  // size of A, c, b
	vector<vector<double>> A(size_system, vector<double>(size_system, 0));
	vector<double> b(size_system, 0);

	// change values in A by the equations
	// row 0 to 2(n-1)-1
	for (int i = 0; i < num_conditions - 1; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			A[2 * i][4 * i + j] = pow(recovery[i], 3 - j);
			A[2 * i + 1][4 * i + j] = pow(recovery[i + 1], 3 - j);
		}
		b[2 * i] = grade_conc[i];
		b[2 * i + 1] = grade_conc[i + 1];
	}
	// row 2(n-1) to 3(n-1)-2
	int starting_row = 2 * (num_conditions - 1);
	for (int i = 1; i < num_conditions - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			A[starting_row + i - 1][4 * (i - 1) + j] = (3 - j) * pow(recovery[i], 2 - j);
			A[starting_row + i - 1][4 * i + j] = -A[starting_row + i - 1][4 * (i - 1) + j];
		}
	}
	// row 3(n-1)-1 to 4n-7
	starting_row = 3 * (num_conditions - 1) - 1;
	for (int i = 1; i < num_conditions - 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			A[starting_row + i - 1][4 * (i - 1) + j] = factorial(3 - j) * pow(recovery[i], 1 - j);
			A[starting_row + i - 1][4 * i + j] = -A[starting_row + i - 1][4 * (i - 1) + j];
		}
	}
	// row 4n-6 to 4n-5
	starting_row = 4 * num_conditions - 6;
	A[starting_row][0] = 6 * recovery[0];
	A[starting_row][1] = 2;
	A[starting_row + 1][4 * (num_conditions - 2)] = 6 * recovery[num_conditions - 1];
	A[starting_row + 1][4 * (num_conditions - 2) + 1] = 2;

	// solve for c
	vector<double> c = solve_square_linear_system(A, b);

	// reserve the size of vector to store the structs containing the coeffcients of each segment of the spline
	grade_recov_spline_coeff.reserve(num_conditions - 1);
	// collect the coeffcients of each segment of the spline from A and store as structs in the "grade_recov_spline_coeff" vector
	for (int i = 0; i < num_conditions - 1; i++)
	{
		spline_coeff new_segment;
		new_segment.a = c[4 * i];
		new_segment.b = c[4 * i + 1];
		new_segment.c = c[4 * i + 2];
		new_segment.d = c[4 * i + 3];
		grade_recov_spline_coeff.push_back(new_segment);
		// look up the coefficients
		// cout << '[' << fixed << setprecision(10) << grade_recov_spline_coeff[i].a << ',' << fixed << setprecision(10) << \
			grade_recov_spline_coeff[i].b << ',' << fixed << setprecision(10) << grade_recov_spline_coeff[i].c << ',' << fixed << setprecision(10) << \
			grade_recov_spline_coeff[i].d << "]," << endl;  
	}
}

// function to calculate grade at a recovery from the spline
double Flotation_optimisation::find_grade_from_spline(double recov)
{
	int i;  // i = the index of the spline segment that contains the given recovery value
	// loop to find i - exit the loop when the given recovery value is between two consecutive values in the "recovery" vector  
	for (i = 0; i < num_conditions - 1; i++) 
		if (is_between(recovery[i], recovery[i + 1], recov)) break;

	// return the value of grade calculated from the spline
	return grade_recov_spline_coeff[i].a * pow(recov, 3) + \
		grade_recov_spline_coeff[i].b * pow(recov, 2) + \
		grade_recov_spline_coeff[i].c * recov + \
		grade_recov_spline_coeff[i].d;
}

// function to find the maximum profit and the optimal recovery/grade
void Flotation_optimisation::optimum()
{
	// calculate the coefficients of the grade-recovery spline
	find_grade_recov_spline_coeff();

	// calculate the values of grade and profit at 1001 values of recovery within the recovery range of the inputted data
	// each of the 1001 recovery values is separated equally. The amount of the separation is called "step_length".
	int num_steps = 1000;
	double recovery_range_from_data = recovery[num_conditions - 1] - recovery[0];
	double step_length = recovery_range_from_data / num_steps;
	// declare arrays to store 1001 values of recovery, grade, and profit along the spline curve
	double* recovery_plot, * grade_plot, * profit_plot;
	// allocate memory for the arrays
	recovery_plot = new double[num_steps + 1];
	grade_plot = new double[num_steps + 1];
	profit_plot = new double[num_steps + 1];
	// calculate the value of grade and profit at each of the 1001 values of recovery
	for (int i = 0; i <= num_steps; i++)
	{
		recovery_plot[i] = recovery[0] + i * step_length;  // the 1001 recovery values
		grade_plot[i] = find_grade_from_spline(recovery_plot[i]);  // the value of grade at each of the recovery values from the spline
		double flowrate_metal_in_conc_i = 
			find_metal_in_conc(recovery_plot[i], flowrate_metal_in_feed);  // the flowrate of metal in concentrate at each of the recovery values
		double gain_i = find_gain(flowrate_metal_in_conc_i);  // the revenue from selling metal in concentrate at each of the recovery values
		double flowrate_conc_i = 
			find_stream_from_metal(flowrate_metal_in_conc_i, grade_plot[i]);  // the flowrate of concentrate at each of the recovery values
		double loss_i = find_loss(flowrate_conc_i);  // the treatment charge for selling concentrate at each of the recovery values
		profit_plot[i] = gain_i - loss_i;  // the profit at each of the recovery values
	}

	// pick up the maximum profit and the corresponding recovery and grade
	val_pos_in_array profit_max_val_pos = max_val_pos(profit_plot, num_steps + 1);
	double max_profit = profit_max_val_pos.val;
	int max_profit_pos = profit_max_val_pos.pos;
	double optimal_recovery = recovery_plot[max_profit_pos];
	double optimal_grade = grade_plot[max_profit_pos];

	// print the result of the calculation
	if (max_profit_pos == 0)  // the leftmost point of the profit-recovery curve is the maximum profit
	{
		cout << endl << "A lower recovery than " << optimal_recovery << "% could lead to a higher profit.\n";
		// if maximum profit from data is non-positive, an optimal recovery can lead to either a positive or negative profit
		if (max_profit <= 0) cout << "Otherwise, the mineral processing is highly likely non-profitable.\n\n";
		else cout << endl;
	}
	else if (max_profit_pos == num_steps)  // the rightmost point of the profit-recovery curve is the maximum profit
	{
		cout << endl << "A higher recovery than " << optimal_recovery << "% could lead to a higher profit.\n";
		// if maximum profit from data is non-positive, an optimal recovery can lead to either a positive or negative profit
		if (max_profit <= 0) cout << "Otherwise, the mineral processing is highly likely non-profitable.\n\n";
		else cout << endl;
	}
	else if (max_profit <= 0)  // the profit-recovery curve is under x-axis, confident to say that there is no profitable data point
	{
		cout << endl << "The mineral processing is highly likely non-profitable.\n\n";
	}
	else  // confident to say that the maximum profit is positive
	{
		cout << endl << "By a natural cubic spline interpolation, the highest profit is $" << to_1_dec(to_k(max_profit)) << "k per hour," << endl;
		cout << "achieved at a recovery of " << to_1_dec(optimal_recovery) << "% and a grade of " << \
			to_1_dec(optimal_grade) << "%." << endl << endl;
	}

	// write a .txt file containing 1001 sets of (recovery, grade, profit) values 
	// so that users can plot their grade-recovery and profit-recovery curves using Microsoft Excel for example
	// define an fstream object that will be the output file
	fstream output_file;
	// open the file to write (the filename is the name of the flotation plant appended by "_for_plot.txt")
	output_file.open(plant_name + "_for_plot.txt", fstream::out);
	// if the file can be opened, write it
	if (is_opened(output_file))
	{
		output_file << "Recovery (%)\tConcentrate grade (%)\tProfit ($/hr)";  // the head of each column
		// loop over the arrays write a row containing (recovery, grade, profit) values respectively 
		for (int i = 0; i <= num_steps; i++) output_file << endl << recovery_plot[i] << '\t' << grade_plot[i] << '\t' << profit_plot[i];  
	}

	// close the file
	output_file.close();

	// deallocate memory of the arrays
	delete[] recovery_plot, grade_plot, profit_plot;
}

// destructor - clear data
Flotation_optimisation::~Flotation_optimisation()
{
	clear_data();
}
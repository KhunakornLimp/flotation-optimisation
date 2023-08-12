#pragma once

#include <iostream>  // required for std:: cout, cin, cerr, endl
#include <vector>  // required for std:: vector
#include <string>  // required for std:: getline

using namespace std;

class Flotation_optimisation
{
public:
	/*
	member variables
		num_conditions = the number of conditions (at least 2 for an interpolation and at least 5 for a more accurate one)
		plant_name = the name of a flotation plant
		metal_price = the price of per ton of metal ($)
		treatment_charge = the treatment charge per ton of (final) concentrate ($)
		flowrate_feed = the flowrate of the feed (tph)
		grade_feed = the grade of the feed (%)
		flowrate_metal_in_feed = the flowrate of metal in the feed (tph)
		flowrate_conc = the vector storing the flowrate of the concentrate produced by the plant on each of the conditions (tph)
		flowrate_metal_in_conc = the vector storing the flowrate of metal in the concentrate produced by the plant on each of the conditions (tph)
		grade_conc = the vector storing the grade of the concentrate produced by the plant on each of the conditions (%)
		recovery = the vector storing the recovery of the plant on each of the conditions (%)
		flowrate_tail = the vector storing the flowrate of the (final) tailings produced by the plant on each of the conditions (tph)
		flowrate_metal_in_tail = the vector storing the flowrate of metal in the tailings produced by the plant on each of the conditions (tph)
		grade_tail = the vector storing the grade of the tailings produced by the plant on each of the conditions (%)
		gain = the vector storing the revenue from selling metal in the concentrate produced by the plant on each of the conditions ($/hr)
		loss = the vector storing the treatment charge for selling the concentrate produced by the plant on each of the conditions ($/hr)
		profit = the vector storing the profit made by selling the concentrate produced by the plant on each of the conditions ($/hr) = gain - loss
		check_stage = the enum defining the stage numbers to be used in the "check_data" function
		spline_coeff = the struct storing the coefficients of each segment of the spline (S_i(x) = a_i*x^3 + b_i*x^2 + c_i*x + d_i) interpolated from inputted data
		grade_recov_spline_coeff = the vector storing the struct that stores the coefficients of each segment of the spline
	*/
	int num_conditions;
	string plant_name;
	double metal_price, treatment_charge, flowrate_feed, grade_feed, flowrate_metal_in_feed;
	vector<double> flowrate_conc, flowrate_metal_in_conc, grade_conc, recovery, flowrate_tail, flowrate_metal_in_tail, grade_tail, gain, loss, profit;
	enum check_stage;
	struct spline_coeff { double a, b, c, d; };  // S_i(x) = a_i*x^3 + b_i*x^2 + c_i*x + d_i
	vector<spline_coeff> grade_recov_spline_coeff;

	// member functions

	// default constructor
	Flotation_optimisation();  // set the initial values

	// other member functions
	
	// find_grade - calculate the grade of a stream from the flowrates of the stream and that of metal in the stream
	double find_grade(double m_stream, double stream);
	vector<double> find_grade(vector<double> m_stream, vector<double> stream);
	// find_metal_in_conc - calculate the flowrate of metal in a concentrate from feed flowrate and recovery
	double find_metal_in_conc(double recov, double m_feed);
	vector<double> find_metal_in_conc(vector<double> recov, double m_feed);
	// find_metal_in_stream - calculate the flowrate of metal in a stream from the flowrate and grade of the stream 
	double find_metal_from_stream(double stream, double grade);
	vector<double> find_metal_from_stream(vector<double> stream, vector<double> grade);
	// find_stream_from_metal - calculate the flowrate of a stream from the flowrate of metal in the stream and the grade of the stream 
	double find_stream_from_metal(double metal, double grade);
	vector<double> find_stream_from_metal(vector<double> metal, vector<double> grade);
	// find_gain - calculate the revenue from selling metal in the concentrate
	double find_gain(double metal);
	vector<double> find_gain(vector<double> metal);
	// find_loss - calculate the treatment charge for selling the concentrate
	double find_loss(double conc);
	vector<double> find_loss(vector<double> conc);

	void clear_single_values();  // clear the values of member variables that are single values (not vectors)
	void clear_vectors();  // clear the member variables that are vectors
	void clear_data();  // clear data
	bool read_file(string filename);  // read a data file
	void read_console();  // read data inputted on the console
	bool check_data(check_stage stage, string str, string str2);  // check inputted data
	void show_input();  // show inputted data on console
	void calculate();  // calculate flowrate_metal_in_feed, flowrate_metal_in_conc, recovery, flowrate_tail, flowrate_metal_in_tail, grade_tail, gain, loss, profit
	void write_calculated_file();  // write a .txt file containing the result of the calculation
	void write_file_for_input();  // write a .txt file from the data inputted on the console for future input
	void find_grade_recov_spline_coeff();  // calculate the coefficients of each segment of the spline
	double find_grade_from_spline(double recov);  // calculate grade at a recovery from the spline
	void optimum();  // find the maximum profit and the optimal recovery/grade and write a .txt file containing 501 sets of (recovery, grade, profit) values 
					 // so that users can plot their grade-recovery and profit-recovery curves using Microsoft Excel for example

	// destructor
	~Flotation_optimisation();  // clear the data
};
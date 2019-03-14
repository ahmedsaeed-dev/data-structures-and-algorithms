/*
> AHMED SAEED
> DR JP BAUGH
> CIS 3501.02
> SUMMER 2018
> PROGRAM 04 - Security Breach
:
> DESCRIPTION:
		User  is attempting to steal  crates from a warehouse. The guy on the inside, named Frank for  all  future 
		purposes, is  feeding the user  information. The first two  things that Frank tells the user is the number 
		of rows and  columns that the crates are stacked in. After that, Frank begins telling the number of stacks 
		per [r][c]  location. Once all crates/locations of the given matrix are given by Frank, the program begins 
		to determine the  best way to steal  crates without the security system noticing. There are three  cameras 
		on the inside that  periodically check the  floor and  crate layout; a front, side and top camera all keep 
		track of the number of  boxes. The program  will determine  how many boxes the  user can steal without the 
		camera picking up on the image change. The way this program works is  that it first determines the highest 
		stack of crates there are per row and column. After this is determined, the program will subtract one from
		all other  [r][c] locations without  altering the highest element seen by that given camera. For  example, 
		in 2x3 matrix { 50, 20, 3, 20, 10, 3 }, 50 is the largest element of row 1, as well as the largest element 
		of column 1. So no crate will be subtracted  from that. The  second element of the matrix, 20, is NOT  the 
		largest element of row 1, but is the largest element of column 2, so no crate will be taken from that pile. 
		The third element, 3, is not the largest element in row 1 or column 3, so it will be depleted by 1. Skipping 
		to element 4, 20 is not the largest element of col 1, but it is the largest element of row 2, so nothing will 
		be taken. The next two elements are both not the largest element of their respective rows/columns, so they
		will both be reduced by 1. This process will continue until the largest number of crates is removed.  The 
		number of elements without affecting the highest [r][c] element is 9.  The user is able to steal 9 crates.
: 
> SAMPLE: 
		A. 2 3 50 20 3 20 10 3
			= 30
		B. 5 5 1 4 0 5 2 2 1 2 0 1 0 2 3 4 4 0 3 0 3 1 1 2 2 1 1
			= 9
*/

#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector> 

using namespace std;

// struct of data for row and col elements
typedef struct node {
	int crate_count;
	int max_crate_height;
	bool visited;
} Node;

class Heist {
private:

	// results could be at most 10e9 
	long long int total_crates;
	int r, c;

	// graph via vector of floor layout 
	vector<int> floor_layout;

	// pairs location [i][j] of crates that CAN be removed 
	vector<pair<int, int>> good_crates;

	// maps data for r/c as unordered map
	unordered_map<int, Node> col_data, row_data;


public:

	/*  Pre: Need object to be declared
	Post: will get row and col from input */
	Heist::Heist() {
		getRC();
		initialize();
	}

	/*  Pre: Constructor = trigger
	Post: Will input and verify r/c are within range ( 1 <= x <= 100 ) */
	void Heist::getRC() {
		cin >> r;
		if (r < 1 || r > 100) exit(1);
		cin >> c;
		if (c < 1 || c > 100) exit(1);
	}

	/*  Pre: Needs r and c to be initialized
	Post: Will resize matricies by r and c
	Brief: Set vector size for floor layout  	*/
	void Heist::initialize() {
		floor_layout.resize(r*c);
	}

	/*  Pre: Triggerd by main / needs object to be declared
	Post: Will update preHeist vector with [i][j] elements */
	void Heist::set_floor() {
		int height;
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				cin >> height; cin.ignore();

				//vector is created based on i * c + j formula  
				floor_layout[i * c + j] = height;

				// total num crates 
				total_crates += height;

				// set row data
				if (height > row_data[i].max_crate_height) {
					row_data[i].max_crate_height = height;
					row_data[i].crate_count = 1; // init with 1
				}
				else if (height == row_data[i].max_crate_height)
					row_data[i].crate_count++; // new row crate

											   // set col data
				if (height > col_data[j].max_crate_height) {
					col_data[j].max_crate_height = height;
					col_data[j].crate_count = 1; // init with 1
				}
				else if (height == col_data[j].max_crate_height)
					col_data[j].crate_count++; // new col crate

			}
		}
	}

	/*  Pre: Triggered via main(), scopes out floor plan to see if curr element cannot be moved
	Post: Increases count of crates per row/col if there is another crate blocking view		*/
	void Heist::scope_floor() {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {

				// set temp element 
				int element = floor_layout[i * c + j];

				// if element at curr location meets conditions, IT CANNOT BE REMOVED!!!
				if (element > 0 && element < row_data[i].max_crate_height && row_data[i].max_crate_height == col_data[j].max_crate_height) {
					floor_layout[i * c + j] = row_data[i].max_crate_height;

					// increase count per row/col since crate above is blocking view
					row_data[i].crate_count++;
					col_data[j].crate_count++;
				}
			}
		}
	}

	/*  Pre:	Triggered by start_heist(), needs element location, element value, num crates + highest num crates in r/c
	Post:	Camera sweep at all three angles. If conditions to steal crate met, steel crate + mark true, else fail		*/
	bool Heist::camera_check(const int i, const int j, int element, int num_crates_ROW, int num_crates_COL, int max_crate_ROW, int max_crate_COL) {

		// checks top 
		if (row_data[i].visited && col_data[j].visited || element < max_crate_ROW && element < max_crate_COL) {
			return true;
		}
		// checks side 
		else if ((row_data[i].visited || element < max_crate_ROW) && num_crates_COL > 1) {
			col_data[j].crate_count--;
			return true;
		}
		// checks front
		else if ((col_data[j].visited || element < max_crate_COL) && num_crates_ROW > 1) {
			row_data[i].crate_count--;
			return true;
		}

		return false;
	}

	/*  Pre: Triggerd by main(), moves camera throughout floorplan
	Post: will mark elements as it traverse through floorplan */
	void Heist::move_camera() {
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				int element = floor_layout[i * c + j];

				// traverses and marks element as for~loops on 
				start_heist(i, j, element, true);
			}
		}
	}

	/*  Pre: Triggered by camera pans. Need the current row/col, the location's element/value, and whether the angle is currently the top camera
	Post: Will decrease number of available crates within warehouse by one if crate is eligible to be stolen
	Brief:	Will check camera with given element data to assess whether crate at given location can be stolen								*/
	void Heist::start_heist(const int curr_row, const int curr_col, const int element, bool top_camera) {

		// if returns true, it is possible to steal crate at given location 
		bool clear = camera_check(curr_row, curr_col, element, row_data[curr_row].crate_count, col_data[curr_col].crate_count, row_data[curr_row].max_crate_height, col_data[curr_col].max_crate_height);

		if (clear) {
			// verify at least 1 crate @ element location 
			if (floor_layout[curr_row * c + curr_col] > 0)
				total_crates -= 1;
		}
		else {
			if (top_camera) { // set pair of eligible crates that can be stolen 
				good_crates.push_back(make_pair(curr_row, curr_col));
			}
			else {
				// this element will be stolen, so reduce from total crates in warehouse 
				total_crates -= element;

				// mark location of row and col as visited 
				row_data[curr_row].visited = true;
				col_data[curr_col].visited = true;
			}
		}
	}

	/*  Pre: Uses marked locations to steal crates (see line 94)
	Post: Removes crates from floor							*/
	void Heist::remove_crates() {

		while (!good_crates.empty()) {

			// calls pair from 94 and sets eligble crates to be removed 
			pair<int, int> element_location = good_crates.back();

			// sets coordinates for element
			int i = element_location.first;
			int j = element_location.second;

			// flags crates
			start_heist(i, j, floor_layout[i * c + j], false);

			// removes curr crate from stack 
			good_crates.pop_back();
		}
	}

	/*  Pre: Triggered by main / needs object to be declared and total to be updated during program
	Post: Prints total number of crates that are able to be stolen	*/
	void Heist::print() {
		cout << total_crates << endl;
	}
};


int main() {
	while (true) {

		// declare object within scope and get r/c length 
		Heist Floor;

		// fill in the graph before heist 
		Floor.set_floor();

		// scopes floor for immovable crates
		Floor.scope_floor();

		// moves between cameras
		Floor.move_camera();

		// removes eligble crates 
		Floor.remove_crates();

		// prints number of stolen crates 
		Floor.print();
	}
	return 0;
}

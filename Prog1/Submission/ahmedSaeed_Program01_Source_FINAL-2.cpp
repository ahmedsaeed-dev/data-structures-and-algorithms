/*
* AHMED L SAEED
* CIS 3501 002
* DR ELENBOGEN
* MAY 29 2018
* PROGRAM 01 - HAPPY LANDINGS
* COMPILED ON VISUAL STUDIO 2015
:
:
* DESCRIPTION:
     This program was created for users to determine in what order planes should land safely. 
     A user is typically given an interval (per airplane) based on when the airplane should land. 
     For example, two airplanes are in the sky. Airplane A can land between 0 and 10 (after the hour), 
     while Airplane B is recommended to land between 10 and 20 (after the hour). This program will 
     determine the the farthest each plane could land, given the minimum distance between the two 
     landings. This is determined by taking the change in the smallest start time, and furthest end 
     interval, and dividing the result by the numbeer of planes minus 1. In the example above, 20, 
     the furthest landing time, minus 0, the smallest starting time, divided by the number of planes 2, 
     minus 1, is 20 over 1, therefor the maximum, minimum distance would be 20.00 (each value is 
     rounded to the nearest hundreth). Then, the program will determine in which order these two 
     planes should land safetly by looping and obtaining the perfect order that would yield the result 
     above. In this case, it would make sense to have plane A land, followed by plane B. 
     NOTE: The current input sequence is number of test cases, then the number of planes per said 
     test case, followed by the suggested intervals to land each subsequent plane, each separated 
     by a [SPACE]. The program will then spit out the results (landing sequence, max, min difference 
     between landings), and then if number of test case is greater than 1, program will wait for next 
     number of planes, and landing intervals.
*/


/*-----------------------------------------------------------------------------
* Preprocessor Includes
*----------------------------------------------------------------------------*/
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

/*-----------------------------------------------------------------------------
* Type Definitions (Struct)
*----------------------------------------------------------------------------*/
struct landing_info {
     int start_interval; //** STARTING INTERVAL TIME
     int end_interval;   //** ENDING INTERVAL TIME
     int plane_position; //** PLANE <n>
};

/*-----------------------------------------------------------------------------
* @name   comparison
* @brief  Compares one <plane_position> to another.
* @pre    Will need plane positions to permutate through corresponding vector.
* @post 	Will return true if plane position <x> is less than plane position <y>.
*----------------------------------------------------------------------------*/
bool comparison(landing_info x, landing_info y) { 
     return x.plane_position < y.plane_position; 
}

class PlaneDetails {
private:
     int num_planes;
     double change_condition_check;
     vector<landing_info> landing_sequence;
     vector<double> landing_intervals;
     vector<landing_info> plane_depot;

public:
     /*.................................................................................
     * @name   PlaneDetails
     * @brief  Copy constructor, makes sure new test case begins with <-1>.
     * @pre    Will need a declared object, and pass through curr object's <num_planes>.
     * @post 	Will initialize values and resize vectors. 
     *.................................................................................*/
     PlaneDetails::PlaneDetails(int curr_num_planes) {
          change_condition_check = -1;
          num_planes = curr_num_planes;
          landing_sequence.resize(curr_num_planes);
          landing_intervals.resize(curr_num_planes);
     }

     /*.................................................................................
     * @name   insert_plane
     * @brief  Adds current plane details to plane depository.
     * @pre    Needs variable of data type <landing_info> to be declare/initialized.
     * @post 	Pushes plane into vector that contains list of planes.
     *.................................................................................*/
     void PlaneDetails::insert_plane(landing_info curr_plane_details) {
          plane_depot.push_back(curr_plane_details);
     }

     /*.................................................................................
     * @name   determine_sequence
     * @brief  Determines sequence of plane landings.
     * @pre    Needs <plane_depot> to be filled with n number planes. Needs bool for comparison.
     * @post 	Sequence of plane landings is determined. 
     *.................................................................................*/
     void PlaneDetails::determine_sequence() {
          do {
               //** <New_Landing_Time> IS WHAT THE AIR TRAFFIC CONTROLLER WILL USE TO DETERMINE LANDING ORDER
               vector<double> New_Landing_Time(num_planes);
               double minimum_change = 0;

               //** DETERMINED END LANDING TIME IS EQUAL TO THE FINAL PERMUTATION'S FIRST INTERVAL
               New_Landing_Time[0] = plane_depot[0].start_interval;

               //** DETERMINED START LANDING TIME IS EQUAL TO THE FINAL PERMUTATION'S END INTERVAL
               New_Landing_Time[num_planes - 1] = plane_depot[num_planes - 1].end_interval;

               //** DETERMINE IF SEQUENCE IS GOOD, EDITS PRIVATE MEMBERS
               determine_distance(New_Landing_Time, minimum_change);

               //** DO NOTHING, <change_condition_check> FAILED SINCE == <-1>
               if (minimum_change == -1) {}

               //** MINIMUM CHANGE PASSED, NOT EQUAL TO <-1>
               else {
                    double minimum_gap_intervals = DBL_MAX;
                    for (int i = 1; i < num_planes; i++) {
                         double curr_change = New_Landing_Time[i] - New_Landing_Time[i - 1];
                         //** NEW MINIMUM CHANGE FOUND
                         if (curr_change < minimum_gap_intervals)
                              minimum_gap_intervals = curr_change;
                    }

                    minimum_change = minimum_gap_intervals;

                    //** NEW MINIMUM CHANGE TOO LARGE
                    if (minimum_change > change_condition_check) {
                         change_condition_check = minimum_change;
                         for (int i = 0; i < num_planes; i++) {
                              landing_intervals[i] = New_Landing_Time[i];
                              landing_sequence[i] = plane_depot[i];
                         }
                    }
               }
               //** PERMUTATES THROUGH PLANE DEPOSITORY, COMPARING WITH EACH ORDER
          } while (next_permutation(plane_depot.begin(), plane_depot.end(), comparison));
     }

     /*.................................................................................
     * @name   determine_distance
     * @brief  Determine largest distance between closest intervals
     * @pre    Need recommended intervals to be submitted, need a value for comparison <minimum_interval_change>
     * @post 	Largest minimum difference between interval permutations will be determined. 
     *.................................................................................*/
     void PlaneDetails::determine_distance(vector <double> & Landingtime, double & minimum_interval_change) {
          for (int i = 1; i < num_planes - 1; i++) {

               //** FORMULA TO FIND CHANGE IN T == LAST - CURR / TOTAL NUM OF PLANES - <i>
               double change_in_T = (Landingtime[num_planes - 1] - Landingtime[i - 1]) / (num_planes - i);

               //** KEEP ADDING CHANGE IN <T> TO CURR CHANGE IN <T>
               change_in_T = change_in_T + Landingtime[i - 1];

               bool start_pass = plane_depot[i].start_interval < change_in_T;
               bool end_pass = plane_depot[i].end_interval > change_in_T;

               /* IF START TIME IS LESS THAN THE DELTA T, AND END TIME IS GREATER THAN DELTA T,
                  THIS IS THE MAX, MIN DIFF, STORE IT WITHIN THE <LandingTime> VECTOR */
               if (start_pass && end_pass)
                    Landingtime[i] = change_in_T;

               //** THE END TIME IS GREATER THAN DELTA T, SET CHANGE TO -1, BREAK
               else if (!start_pass && end_pass) {
                    Landingtime[i] = plane_depot[i].start_interval;
                    if (Landingtime[i] > Landingtime[num_planes - 1]) {
                         minimum_interval_change = -1;
                         break;
                    }
               }

               //** START TIME IS WITHIN BOUNDS, END TIME IS NOT, SET CHANGE TO -1, BREAK
               else if (start_pass && !end_pass) {
                    Landingtime[i] = plane_depot[i].end_interval;
                    if (Landingtime[i] < Landingtime[i - 1]) {
                         minimum_interval_change = -1;
                         break;
                    }
               }
          }
     }

     /*.................................................................................
     * @name   display_results
     * @brief  Prints results of current test case
     * @pre    Plane landing sequence needs to be set. Max min difference needs to be determined.
     * @post 	Results output to screen. Landing sequence separated by [SPACE], and then difference. 
     *.................................................................................*/
     void PlaneDetails::display_results() {
          for (int i = 0; i < num_planes; i++)
               cout << landing_sequence[i].plane_position << " ";

          //** ROUNDED TO NEAREST HUNDRETH
          cout << fixed << setprecision(2) << change_condition_check << endl;
     }
};

/*.................................................................................
* @name   main
* @brief  Suggested landing intervals, number of test cases, number of planes are inputted.
* @pre    User needs to be aware of input sequence. No prompts will be displayed. 
* @post 	Landing sequence and difference in landings will be determined. 
*.................................................................................*/
int main() {
     //** TEMP VALUES
     int temp_start = 0, temp_end = 0, num_test_cases = 0, num_planes = 0;

     //cout << "Enter number of test cases: ";
     cin >> num_test_cases;

     for (int i = 0; i < num_test_cases; i++) { //** PER TEST CASE
          //cout << "Enter number of planes: ";
          cin >> num_planes;
          PlaneDetails curr_plane(num_planes); //** OBJECT WITHIN TEST CASE SCOPE / COPY CONSTRUCTOR ALLOCATES SIZE OF VECTOR
          for (int i = 0; i < num_planes; i++) {
               //cout << "Enter suggested start and end intervals for plane " << i + 1 << " separated by a single [SPACE]: ";
               cin >> temp_start; cin >> temp_end;
               landing_info temp_plane; //** VARIABLE OF TYPE <landing_info> WITHIN SCOPE
               temp_plane.plane_position = i + 1;
               temp_plane.start_interval = temp_start;
               temp_plane.end_interval = temp_end;
               curr_plane.insert_plane(temp_plane);
          }

          //** DO MATHS <( O.O)/
          curr_plane.determine_sequence();

          //** SHOW ME THE MONEY \(O.O )>
          curr_plane.display_results();
     }

     //system("PAUSE");
     return 0;
}


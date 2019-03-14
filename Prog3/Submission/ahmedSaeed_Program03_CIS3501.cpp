/*
@ AHMED SAEED 
@ DR BAUGH
@ PROGRAM 03: THREES COMPANY
@ 25 JULY 2018
@ CIS 3501


@ DESCRIPTION: 

     This program was designed in order to determine what vehicle can get to the
	 destination the fastest, avoiding obstacals. In this particular program, 
	 the vehicles are the Twilight characters Edward, a vampire, and Jacob, a 
	 werewolf, while the destination is anywhere their potential partner Bella is
	 located. All three characters start off in a cave, with the cave layout
	 described by the user. The first number the user should input determines the
	 number of caves there are within the cave system. The second, third and fourth 
	 inputs each tell which caves Bella, Jacob, and Edward are in (respectively). 
	 The next sets of input are based on the number of caves. For every cave, the user
	 will enter one line. The first input of the cave line will read either 0, 1, or -1; 
	 each representing the type of allergen within that specified cave. For example,
	 line 2 of the entire input will represent cave one. A 0 means there are no allergens
	 within the cave. A -1 represents garlic, meaning that vampires (Edward) cannot enter. 
	 A 1 represents wolfsbane, telling that Jacob cannot enter these caves. If a character 
	 enters a cave that consists of his allergen, they will double back and return to the 
	 cave from which they came. The next input will consist of the number of caves
	 connected to that specific cave, and the next inputs after that are the connected 
	 caves in clockwise order. A typical input looks like: 
																4 1 3 2
																0 2 2 3
																0 3 1 3 4
																-1 3 1 2 4
																0 2 2 3
	In the example above, there are 4 caves, with Bella in 1, Jacob in 3, and Edward in 
	2. In the first cave, there are no allergens. Cave one is connected to two caves, 
	both cave 2 and 3. The next line shows cave 2 and cave 2 also does not contain an allergen.
	Cave 2 is connected to 3 caves, 1, 3, and 4, in a clockwise order. Cave 3 contains the 
	allergen garlic, which means Edward cannot enter this cave. Cave 3 is also connected 
	to 3 caves, caves 1, 2 and 4. Finally, cave 4 contains no allergens, and is connected to
	caves 2 and 3. In this scenario, Edward and Jacob start at caves 3 and 2 respectively. 
	For the first move (and all first moves), Edwards moves to the highest available cave, 
	while Jacob moves to the lowest. After the first move, Edward moves in a counterclockwise
	order, while Jacob moves clockwise. If they find their specific allergen within a cave,
	the character double backs. If after 99 turns, neither one can reach Bella, all three
	characters die of starvation. In this specific scenario, Edward reaches Bella in 5 turns. 
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define GARLIC -1
#define WOLFSBANE 1
#define BELLA 0
#define START 1

class Cave_Layout {
private:
	vector<vector<int>> map;
	int n_caves;

public:

     /*
     @ PRE: main() must create class object of type Cave_Layout and push graph and number of cave parameters.
     @ POST: Will initialize object with number of caves and current map of caves. 
     */
	Cave_Layout(vector<vector<int>> graph, int n) {
		n_caves = n;
		map = graph;
	}

     /*
     @ PRE: User must understand how program input functions.
     @ POST: Will push curr cave layout to class functions to compute results.
     */
     void trigger_movement() {
          for (int i = START; i < 100; i++) {
               moveEdward(i);
               moveJacob(i);
               bool Jacob = map[n_caves][2] == map[n_caves][BELLA];
               bool Edward = map[n_caves][4] == map[n_caves][BELLA];
               if (Jacob && Edward) { //Jacob and Edward reach Bella at the same time
                    cout << "Everyone dies in " << i << " moves" << endl;
                    break;
               }
               if (Jacob) { //If Jacob reaches Bella first
                    cout << "Bella marries Jacob in " << i << " moves" << endl;
                    break;
               }
               if (Edward) { //If Edward reaches Bella first 
                    cout << "Bella marries Edward in " << i << " moves" << endl;
                    break;
               }
               if (i == 99) { //No one ever reaches Bella
                    cout << "Everyone dies in 101 moves" << endl;
                    break;
               }
          }
     }

     /*
     @ PRE: Must be called from trigger_movement(), caves must be set/initialized
     @ POST: Will move Jacob clockwise unless wolfsbane is found.
     */
	void moveJacob(int i) {
		if (i == START) {
               //Jacob moves to 2 at start
			map[n_caves][2] = map[map[n_caves][2]][0];

               //if wolfsbane is at 2, Jacob moves back to 3
			if (map[0][map[n_caves][2] - 1] == WOLFSBANE) {
				map[n_caves][2] = map[n_caves][1];
				map[n_caves][1] = map[map[n_caves][2]][0];
			}
		}

          //for all turns except start
		else {
			for (int j = 0; j < map[map[n_caves][2]].size(); j++) {
				if (map[n_caves][1] == map[map[n_caves][2]][j]) {

					if (j == map[map[n_caves][2]].size() - 1) {
						map[n_caves][1] = map[n_caves][2];
						map[n_caves][2] = map[map[n_caves][2]][0];

                              //if wolfsbane located at cave, circle back
						if (map[0][map[n_caves][2] - 1] == WOLFSBANE) {
							map[n_caves][2] = map[n_caves][1];
							map[n_caves][1] = map[map[n_caves][2]][0];
						}

					}
					else {
						map[n_caves][1] = map[n_caves][2];
						map[n_caves][2] = map[map[n_caves][2]][j + 1];

                              //if wolfsbane located at cave, circle back
						if (map[0][map[n_caves][2] - 1] == WOLFSBANE) {
							map[n_caves][2] = map[n_caves][1];
							map[n_caves][1] = map[map[n_caves][2]][j + 1];
						}
					}
					return;
				}
			}
		}
	}

     /*
     @ PRE: Must be called from trigger_movement(), caves must be set/initialized
     @ POST: Will move Edward counter-clockwise unless garlic is found.
     */
	void moveEdward(int i) {
		if (i == START) {
               //Edward moves to 4 at start 
			map[n_caves][4] = map[map[n_caves][4]][map[map[n_caves][4]].size() - 1];

               //if garlic is at 4, edward moves to 3
			if (map[0][map[n_caves][4] - 1] == GARLIC) {
				map[n_caves][4] = map[n_caves][3];
				map[n_caves][3] = map[map[n_caves][4]][map[map[n_caves][4]].size() - 1];
			}

		}
		else {
			for (int j = 0; j < map[map[n_caves][4]].size(); j++) {
				if (map[n_caves][3] == map[map[n_caves][4]][j]) {
					if (j == 0) {
						map[n_caves][3] = map[n_caves][4];
						map[n_caves][4] = map[map[n_caves][4]][map[map[n_caves][4]].size() - 1];

                              //if garlic located at cave, circle back
						if (map[0][map[n_caves][4] - 1] == GARLIC) {
							map[n_caves][4] = map[n_caves][3];
							map[n_caves][3] = map[map[n_caves][4]][map[map[n_caves][4]].size() - 1];
						}

					}
					else {
						map[n_caves][3] = map[n_caves][4];
						map[n_caves][4] = map[map[n_caves][4]][j - 1];

                              //if garlic located at cave, circle back
						if (map[0][map[n_caves][4] - 1] == GARLIC) {
							map[n_caves][4] = map[n_caves][3];
							map[n_caves][3] = map[map[n_caves][4]][j - 1];
						}
					}
					return;
				}
			}
		}
	}
};


/* 
 @ PRE: User must understand how program input functions. 
 @ POST: Will push curr cave layout to class functions to compute results. 
*/
int main() {
     int  n_caves = 0, bella = 0, jacob = 0, edward = 0, 
          allergen = 0, n_degrees = 0, adjacent = 0;
     
	while (true) {

          //create cave directory 
		vector<vector<int>> total_caves;

          //create curr cave layout  
		vector<int> graph;
		total_caves.push_back(graph);

          //input n caves, and character positons 
		cin >> n_caves >> bella >> jacob >> edward;

          //exit condition 
		if (n_caves == 0 && bella == 0 && jacob == 0 && edward == 0)
			exit(1);

		else {
			//for every n cave, input/push back cave info 
               for (int i = 0; i < n_caves; i++) {

                    //create curr cave layout
				vector<int> curr_graph;
				total_caves.push_back(curr_graph);

                    //input allergens and n degrees
				cin >> allergen >> n_degrees;
				total_caves[0].push_back(allergen);

                    //find all connected caves
				for (int j = 0; j < n_degrees; j++) {
					cin >> adjacent;
					total_caves[i + 1].push_back(adjacent);
				}
			}
               //push layout into double vector  
			total_caves.push_back({ bella,jacob,jacob,edward,edward });
			
               //push entire cave to class 
               Cave_Layout Curr_Cave(total_caves, n_caves + 1);
			Curr_Cave.trigger_movement();
		}
	}
	return 0;
}

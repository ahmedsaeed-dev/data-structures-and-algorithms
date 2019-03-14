/*
* Ahmed Saeed
* CIS 3501 - Program 02
* Dr Elenbogen
* June 12 2018
 
     Program 02 creates a n-ary tree and is focused on the seniority of white house staff. 
     In this function, a user is given four options. Have an existing member hire a new member,
     fire an existing member, print the current tree, or quit the program. To use this program,
     the user must first enter the first instance of hire. For example, if a user first enters
     " A hires B " (without quotes), A is initiated as the root, and B is then set as a child
     of A. If a user were to then enter " A hires C " (without quotes), then A's two children
     are B and C in that particular order. In any n-ary tree, seniority matters. In order to 
     terminate children, the user must enter " Fire < name of child > ". For example, to
     terminate child B, the user would enter " Fire B " and C would move up the seniority list.
     If a user wanted to print this tree, they would simply enter, "Print". Once a user enters 
     print, the program will print beginning at the root, with every level of seniority showing 
     a +. For example, if a user enters "A hires B" "B hires C" "C hires D " the program will 
     display the following when "Print" is entered:    A
                                                       +B
                                                       ++C
                                                       +++D
     This program will continue to run until the user enters "Quit". If print shows that there 
     are no nodes/children, the user can initiate a new n-ary tree by simply typing a new hire
     command. For example, if there are no seen nodes and the user enters "X hires Z", then X 
     will be the new root, and Z will be the first child. 

*/

#include <iostream >
#include <vector >
#include <string >

using namespace std;

struct Node {
     Node * parent;
     string name;
     vector<Node *> children;
};

class WhiteHouse {
private:
     Node * root;

public:
   /*@brief:   constructor with root
     @pre:     needs to be initiated with string argument
     @post:    will create object of class <WhiteHouse>, will set root to given string parameter */
     WhiteHouse::WhiteHouse(string name) {
          root = new Node();
          root->name = name;
     }

   /*@brief:   destructor
     @pre:     needs object of class <WhiteHouse> to have been created
     @post:    will set root to nullptr */
     WhiteHouse::~WhiteHouse() {
          root = nullptr;
     }

   /*@brief:   returns root
     @pre:     needs object of class <WhiteHouse> to be initiated with root
     @post:    will return root */
     Node * WhiteHouse::getRoot() {
          return root;
     }


   /*@brief:   check if tree is empty
     @pre:     needs object of class <WhiteHouse> to be created
     @post:    will return true if root is truly equal to nullptr */
     bool WhiteHouse::emptyTree() {
          return root == nullptr;
     }

   /*@brief:   traverse tree to find node containing target name
     @pre:     object needs to be created, target name needs to be set. 
     @post:    will return node * depending on if a node containing target was found. */
     Node * WhiteHouse::find(Node * ptr_root, string target_name) {

          // if root is empty, return nullptr
          if (ptr_root == nullptr) return nullptr;

          // if root node contains target, return root
          else if (ptr_root->name == target_name) return ptr_root;

          // search tree for node containing target
          else {

               // traverse through tree up to size of tree
               for (int i = 0; i < ptr_root->children.size(); i++) {

                    // create node to store node containing target
                    Node * target_node = find(ptr_root->children[i], target_name);

                    // if target node target_node, return node containing target
                    if (target_node != nullptr) return target_node;
               }

               // if no node containing target target_node after traverse, return nullptr
               return nullptr;
          }
     }

   /*@brief:   sets child as subordinate of parent
     @pre:     <WhiteHouse> object needs to be created, parent name and child name need to be provided
     @post:    will assign a node containing child name to appropriate parent node within parent's children vector */
     void WhiteHouse::hire(string parent_name, string child_name) {

          // locate parent node given parent name
          Node * parent_node = find(root, parent_name);

          // if parent node not target_node, no parent, return. Illegal input
          if (parent_node == nullptr) return;

          // create new child node
          Node * child_node = new Node();

          // assign name to child node
          child_node->name = child_name;

          // assign child node to parent vector in appropriate seniority
          parent_node->children.push_back(child_node);

          // new child points to parent
          child_node->parent = parent_node;
     }

   /*@brief:   will terminate a node
     @pre:     a target name that needs to be removed must be sent
     @post:    will remove node containing target from tree */
     void WhiteHouse::fire(string target_name) {

          // assign node containing target to new node
          Node * target_node = find(root, target_name);

          // if target node has no parent and has no children, must be root. Delete
          if (target_node->parent == nullptr && target_node->children.size() == 0) {
               delete target_node;
               root = nullptr;
               return;
          }

          // if target node is not pointing to anything, no one to fire, return. 
          if (target_node == nullptr) return;

          // Since it passed prior tests, that means this node can be deleted. Delete
          reorder_tree(target_node);

          // if target node is pointing to nothing, assign root to target node children
          if (target_node->parent == nullptr) {
               root = target_node->children[0];
               target_node->children[0]->parent = nullptr;
               delete target_node;
               return;
          }

          // keep track of how many children there are
          int count;

          // traverse through target height, and reassign
          for (int i = 0; i < target_node->parent->children.size(); i++) {
               if (target_node->parent->children[i]->name == target_node->name)
                    count = i;
          }

          // if target node has no children, erase that node from parent's children seniority
          if (target_node->children.size() < 1)
               target_node->parent->children.erase(target_node->parent->children.begin() + count);

          // if target does have children, assign last seniority of target level up
          else {
               target_node->parent->children[count] = target_node->children[0];
               target_node->children[0]->parent = target_node->parent;
          }

          // delete target after all previous conditions passed 
          delete target_node;
     }

   /*@brief:   will reorder after termination of node containing target
     @pre:     target needs to be removed from tree
     @post:    will reorder children based on seniority following termination of a parent with children */
     void WhiteHouse::reorder_tree(Node * del_node) {

          // If root is empty or there are no children for root to fire, illegal move, return
          if (del_node == nullptr || del_node->children.size() == 0) return;

          // traverse to the target node's child with highest seniority
          reorder_tree(del_node->children[0]);

          // resize child node based on how many nodes parent has
          del_node->children[0]->children.resize(del_node->children.size());

          // correct all nodes based on child moving up to parent position
          for (int i = 1; i < del_node->children.size(); i++) {
               del_node->children[0]->children[i] = del_node->children[i];
               del_node->children[i]->parent = del_node->children[0];
          }

          // fire highest seniority of target node which should be target node
          if (del_node->children[0]->children[0] == nullptr)
               del_node->children[0]->children.erase(del_node->children[0]->children.begin());
     }

   /*@brief:   prints tree
     @pre:     object of class <WhiteHouse> needs to be initiated
     @post:    will print contents of tree. Subordinates will have '+' based on seniority. */
     void WhiteHouse::print(Node* root_ptr, int seniority) {

          // if no root, nothing to print, escape
          if (root_ptr == nullptr) return;

          // print seniority '+' 
          for (int i = 0; i < seniority; i++)
               cout << "+";

          // print current node name
          cout << root_ptr->name << endl;

          // traverse through tree
          for (int i = 0; i < root_ptr->children.size(); i++)
               print(root_ptr->children[i], seniority + 1);
     }
};

/*@brief:   main
  @pre:     User must understand how program functions/how inputs are formated
  @post:    will display contents of tree given by keyboard/user input, based on description on lines 7-26 above */
int main() {

     // initiate tree
     string first_word, buffer, second_word;
     cin >> first_word >> buffer >> second_word;

     // assign first name as root
     WhiteHouse Tree(first_word);
     Tree.hire(first_word, second_word);

     // initiate do while condition
     cin >> first_word;

     do {

          // if first word is quit, exit program
          if (first_word == "quit" || first_word == "Quit")
               exit(1);

          // if first word is print, print current tree
          else if (first_word == "print" || first_word == "Print")
               Tree.print(Tree.getRoot(), 0);

          // if first word is fire, input second word and fire second word
          else if (first_word == "fire" || first_word == "Fire") {
               cin >> second_word;
               Tree.fire(second_word);
          }

          // else, it's a hire. buffer == "hire" and then input hire. 
          else {

               // get root to check if tree is empty
               bool isEmpty = Tree.emptyTree();

               // if empty, empty current tree, restart with new(&Tree)
               if (isEmpty) {
                    Tree.~WhiteHouse();
                    new(&Tree) WhiteHouse(first_word);
                    cin >> buffer >> second_word;
                    Tree.hire(first_word, second_word);
               }

               // if not empty, add to curr tree
               else {
                    cin >> buffer >> second_word;
                    Tree.hire(first_word, second_word);
               }
          }

          // input new first word for loop 
          cin >> first_word;

     } while (true);

     //system("PAUSE")
     return 0;
}
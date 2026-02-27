//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Marek Lubecki
// Version     : 1.0
// Description : Project Two - CS 300 
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype> 

using namespace std;

// Define a structure to represent a course
struct Course {
    string courseNumber; //Will be the unique course number
    string courseName; //Will be the name of the course
    vector<string> prerequisites; //Will hold all, if any, prerequisites for the course
};

class BinarySearchTree { // Define a class to represent a binary search tree for storing courses

private:
    struct Node { // Define a structure to represent a node in the binary search tree
        Course course;
        Node* left;
        Node* right;

        Node() { // Default constructor
            left = nullptr;
            right = nullptr;
        }

        Node (Course c) { // Constructor that initializes the course and sets left and right pointers to nullptr
            course = c;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root; // Pointer to the root of the binary search tree

    void inOrder(Node* node); // Helper function for in-order traversal of the binary search tree

public:
    BinarySearchTree(); // Constructor that initializes the root pointer to nullptr
    void insertCourse(Course course); // Function to insert a course into the binary search tree
    void printCourseList(); // Function to print the course list in order
    void printCourseInfo(string courseNumber); // Function to print the information of a specific course

};

void BinarySearchTree::inOrder(Node* node) { // Function for in-order traversal and printing of the binary search tree
    if (node != nullptr) {
        inOrder(node->left);
        cout << "\nCourse Number: " << node->course.courseNumber << ", Course Name: " << node->course.courseName << endl;
        inOrder(node->right);
    }
}

BinarySearchTree::BinarySearchTree() { // Constructor that initializes the root pointer to nullptr
    root = nullptr;
}

void BinarySearchTree::insertCourse(Course course) { // Function to insert a course into the binary search tree
    Node* newNode = new Node(course); // Create a new node with the given course

    if (root == nullptr) { // If the tree is empty, set the new node as the root
        root = newNode;
        return;
    }

    Node* current = root; // Start from the root
    Node* parent = nullptr; // Initialize a pointer to keep track of the parent node

    while (current != nullptr) { // Traverse the tree to find the correct position for the new node
        parent = current; // Update the parent pointer

        if (course.courseNumber == current->course.courseNumber) {
            return; // If a course with the same course number already exists, do not insert the new node and return
        }

        else if (course.courseNumber < current->course.courseNumber) { // If the course number is less than the current node's course number, go left
            current = current->left;
        } 
        else { // If the course number is greater than or equal to the current node's course number, go right
            current = current->right;
        }
    }

    if (course.courseNumber < parent->course.courseNumber) { // Insert the new node as a left child of the parent node
        parent->left = newNode;
    } 
    else { // Insert the new node as a right child of the parent node
        parent->right = newNode;
    }
}

void BinarySearchTree::printCourseList() { // Function to print the course list alphanumerically by course number using in-order traversal of the binary search tree
    if (root == nullptr) { // If the tree is empty, print a message and return
        cout << "\n\nNo courses available to print." << endl;
        return;
    }
    else if (root != nullptr) { // If the tree is not empty, perform in-order traversal to print the course list
        inOrder(root);
    }
}

void BinarySearchTree::printCourseInfo(string courseNumber) { // Function to print the information of a specific course given its course number
    Node* current = root; // Start from the root of the binary search tree

    while (current != nullptr) { // While the tree is not empty
        // Compare current node to the input
        if (courseNumber == current->course.courseNumber) { // If a match is found, print the course information
            cout << "\n\nCourse Number: " << current->course.courseNumber << endl;
            cout << "Course Name: " << current->course.courseName << endl;
            cout << "Prerequisites: ";
            
            if (current->course.prerequisites.empty()) { // If there are no prerequisites, print "None"
                cout << "None";
            } else { // If there are prerequisites loop through the prerequisites vector and print each prerequisite
                for (size_t i = 0; i < current->course.prerequisites.size(); ++i) { 
                    cout << current->course.prerequisites[i];
                    if (i < current->course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
            }
            cout << endl;
            return; // Return after printing the course information
        }
        else if (courseNumber < current->course.courseNumber) { // If the input course number is less than the current node's course number, go left
            current = current->left;
        }
        else { // If the input course number is greater than the current node's course number, go right
            current = current->right;
        }
    }
    cout << "\nCourse " << courseNumber << " not found." << endl; // If the course is not found in the binary search tree, print a message indicating that the course was not found
}

void mainMenu() { // Function to display the main menu and handle user input
    bool fileUpload = false; // Flag to track whether a course file has been uploaded
    char choice = '0'; // Variable to store the user's menu choice
    bool menuRunning = true; // Flag to control the main menu loop
    BinarySearchTree bst; // Create an instance of the BinarySearchTree class to store the courses

    while (menuRunning) { // Main menu loop that continues until the user chooses to exit
        cout << "\n\nWelcome to the ABCU Computer Science Course Catalog." << endl;
        cout << "Please select an option from the menu below:" << endl;
        cout << "1. Upload Course File" << endl;
        cout << "2. Print All Courses" << endl;
        cout << "3. Search for Course Information" << endl;
        cout << "9. Exit Program" << endl;
        cout << "Enter your choice (1, 2, 3, or 9): ";
        cin >> choice;

        switch (choice) { // Handle the user's menu choice using a switch statement
            case '1': // Case for uploading a course file, validating courses, and inserting valid courses into the BST
            {
                auto trim = [](const string& str) -> string { // function to trim leading and trailing whitespace from a string
                size_t first = str.find_first_not_of(" \t\r\n");
                if (string::npos == first) return "";
                size_t last = str.find_last_not_of(" \t\r\n");
                return str.substr(first, (last - first + 1));
                };
                // function to convert string to lowecase
                auto toLower = [](string str) -> string { 
                    transform(str.begin(), str.end(), str.begin(), ::tolower);
                    return str;
                };

                if (!fileUpload) { //If a course file has not yet been uploaded, user input file name to attempt upload into BST
                    string fileName; // Variable to store the name of the course file to upload
                    cout << "\n\nEnter the name of the course file to upload (Case Sensitive): ";
                    cin >> fileName;

                    try {
                        ifstream inFile(fileName); // Attempt to open the specified course file for reading
                        if (!inFile) { //If file cannot be opened throw an error message
                            throw runtime_error("\nError opening file. Please try again.");
                        }

                        // First pass to collect all vourse IDs for validation
                        vector<string> courseIds;
                        string line;

                        while (getline(inFile, line)) {
                            
                            if (trim(line).empty()) //If the line is empty, skip it
                                continue;

                            stringstream ss(line);
                            string token;
                            if (getline(ss, token, ',')) { // Get first token (CourseID) and add to the courseID vector
                                courseIds.push_back(toLower(trim(token)));
                            }
                        }

                        inFile.clear(); // Clear the EOF flag after the first pass
                        inFile.seekg(0); // Reset the file stream to the beginning of the file for the second pass
                        int lineNumber = 0; // Variable to keep track of the current line number for error reporting

                        while (getline(inFile, line)) { // Second pass to validate prereqs and insert courses into BST
                            lineNumber++; // Increment line number for each line read
                            
                            if (trim(line).empty()) continue; //If the line is empty, skip it

                            stringstream ss(line); 
                            string token;
                            
                            vector<string> originalTokens;
                            vector<string> normalizedTokens;

                            while (getline(ss, token, ',')) { // Split the line into tokens using comma as a delimiter, trim whitespace, and store both original and normalized (lowercase) versions of the tokens for validation
                                string cleanToken = trim(token);
                                originalTokens.push_back(cleanToken);
                                normalizedTokens.push_back(toLower(cleanToken));
                            }

                            if (normalizedTokens.size() < 2) { // If there are fewer than 2 tokens (CourseID and CourseName), print error message and skip the line
                                cout << "\n\nError: Incorrect formatting at line " << lineNumber << ". Skipping." << endl;
                                continue;
                            }

                            bool allPrereqsValid = true; 
                            // Iterate through tokens starting at index 2
                            for (size_t i = 2; i < normalizedTokens.size(); ++i) {
                                if (normalizedTokens[i].empty()) continue; // If the prereq token is blank, skip validation

                                bool matchFound = false;
                                for (const auto& id : courseIds) { // Check if the normalized prereq token matches any of the normalized course IDs collected in the first pass
                                    if (id == normalizedTokens[i]) { // If a match is found, set matchFound to true and break
                                        matchFound = true;
                                        break;
                                    }
                                }
                                if (!matchFound) { // If no match found, print error message and  skip this course insertion
                                    cout << "\n\nError: Invalid prerequisite '" << originalTokens[i] << "' at line " << lineNumber << ". Skipping." << endl;
                                    allPrereqsValid = false;
                                    break; 
                                }
                            }

                            if (allPrereqsValid) { //If prereqs are validated, create course object and insert into BST
                                Course course;
                                course.courseNumber = originalTokens[0];
                                course.courseName = originalTokens[1];

                                for (size_t i = 2; i < originalTokens.size(); ++i) { // Add only non-empty tokens as prereqs
                                    if (!originalTokens[i].empty()) {
                                        course.prerequisites.push_back(originalTokens[i]);
                                    }
                                }

                                bst.insertCourse(course);
                            }
                        }

                        inFile.close(); // Close the course file after processing
                        fileUpload = true; // Set the file upload flag to true to indicate that a course file has been successfully uploaded
                        cout << "\n\nCourse file uploaded successfully." << endl;

                    } catch (const exception& e) { // Catch any exceptions thrown during file upload and print the error message
                        cout << e.what() << endl;
                    }
                } else {
                    cout << "\n\nCourse file has already been uploaded." << endl;
                }
                break;
            }

            case '2': // Case for printing the course list in order by course number
            {
                if (fileUpload) { //If file has been successfully uploaded
                    bst.printCourseList(); // Call the printCourseList function of the BinarySearchTree class to print the course list in order by course number
                } else {
                    cout << "\n\nPlease upload a course file before printing the course list." << endl; // Else iof no file has been uploaded, inform user they need to upload a file
                }
            
                break;
            }

            case '3': // Case for searching for a specific course by course number
            {
                if (fileUpload) { //If a file has been successfully uploaded
                    string searchCourseNumber;
                    cout << "\n\nEnter the course number to search for (Case Sensitive): ";
                    cin >> searchCourseNumber; //Prompt user for course number to search for and store in variable
                    bst.printCourseInfo(searchCourseNumber); // Call printCourseInfo function using the user's input
                } else {
                    cout << "\n\nPlease upload a course file before searching for course information." << endl; // Else if no file has been uploaded, inform user they need to upload a file
                }
                break;
            }

            case '9': // Case for exiting the program
                menuRunning = false; // Set the menuRunning flag to false to exit the main menu loop
                cout << "\n\nExiting program. Goodbye!" << endl;
                break;

            default: // Default case for handling invalid menu choices
                cout << "\n\nInvalid choice. Please enter 1, 2, 3, or 9." << endl;
        }
    }
}

int main() { // Main function that starts the program by calling the mainMenu function
    mainMenu();
    return 0;
}

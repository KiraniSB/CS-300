//============================================================================
// Name        : CS 300 Project 2.cpp
// Author      : Robert Parish
// Version     : 1.0
// Description : SNHU Course sorting and manipulation
//============================================================================
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector <string> prereqs;
};

/**
 * A search function to look for a specific course in the courses vector
 *
 * @param courses the vector being searched in, id the unique identifier being looked for
 * 
 * @return the index the searched term is at or -1 if not found
 */
int search(vector<Course> courses, string id) {

    //for loop to iterate over the whole vector
    for (int i = 0; i < courses.size(); i++) {

        //if the courseID at this index matches the one being searched for then return the index the searched term is at
        if (courses.at(i).courseId == id) {
            return i;
        }
    }

    //return -1 if id is not found
    return -1;
}

/**
 * A search function to look for a the amount of commas in a string
 *
 * @param s the string being searched through
 * 
 * @return the amount of commas found in the string
 */
int commaCount(string s) {

    //initialize count
    int count = 0;

    //Iterate through the string, if a comma is found increment count
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ',') {
            count++;
        }
    }

    //return amount of commas found
    return count;
}

/**
 * A function that makes a vector of Course objects from a file input
 *
 * @return the vector of Course objects made
 */
vector<Course> loadVector() {

    //initialize housekeeping and tracker variables
    vector<Course> courses;
    ifstream fin;
    string fileName;
    string tempString;
    string tempPrereqString;
    Course tempCourse;
    vector<Course> empty;
    int commas;
    int increment = 0;
    int pos = 0;

    //prompt user for a file name and use it to open an input file
    cout << "What is the name of the input file?" << endl;
    cin >> fileName;
    fin.open(fileName);

    //Check to see if file opened properly if so, continue with loading the vector
    if (fin.is_open()) {

        //get first line of data
        getline(fin, tempString);

        //while the end of file hasn't been reached
        while (!fin.eof()) {

            //be sure the line has an appropriate amount of commas and reset temporary/tracking variables
            commas = commaCount(tempString);
            increment = 0;
            tempCourse.courseId = "";
            tempCourse.title = "";
            int temp = tempCourse.prereqs.size();
            for (int i = 0; i < temp; i++) {
                tempCourse.prereqs.pop_back();
            }

            //if an appropriate amount of commas are in the line of data, continue
            if (commas > 0) {

                //do while loop that executes until the end of the line has been reached
                do {

                    //find next comma or the end of the line
                    pos = tempString.find(',');

                    //switch case to see which Course element needs to be filled
                    switch (increment) {

                    //the Course ID is added to tempCourse, then that substring is erased from the data string
                    case 0:
                        tempCourse.courseId.append(tempString, 0, pos);
                        tempString.erase(0, pos + 1);
                        break;

                     //the title is added to tempCourse, then that substring is erased from the data string
                    case 1:
                        if (pos == -1) {
                            tempCourse.title.append(tempString, 0, tempString.size());
                            tempString.erase(0, tempString.size());
                        }
                        else {
                            tempCourse.title.append(tempString, 0, pos);
                            tempString.erase(0, pos + 1);
                        }
                        break;

                     //the prerequisite(s) is added to tempCourse, then that substring is erased from the data string
                    default:
                        if (pos == -1) {
                            tempPrereqString.append(tempString, 0, tempString.size());
                            tempCourse.prereqs.push_back(tempPrereqString);
                            tempString.erase(0, tempString.size());
                            tempPrereqString = "";
                        }
                        else {
                            tempPrereqString.append(tempString, 0, pos);
                            tempCourse.prereqs.push_back(tempPrereqString);
                            tempString.erase(0, pos + 1);
                            tempPrereqString = "";
                        }
                        break;
                    }

                    //increment housekeeping variable
                    increment++;
                } while (pos != -1);
            }

            //add tempCourse to the courses vector and get a new line of data to start the loop over
            courses.push_back(tempCourse);
            getline(fin, tempString);
        }

        //close the opened file
        fin.close();
    }

    //error opening file
    else {
        cout << "Error opening file" << endl;
    }

    //return the vector of Courses
    return courses;
}

/**
 * A function prints the menu of possible choices for the program to the user
 */
void printMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "4. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

/**
 * A function to print a course and it's prerequisites
 *
 * @param courses the vector in which the course to print is in
 * id the Course ID for the course wanting to be printed
 */
void printCourse(vector<Course> courses, string id) {

    //initialize a variable to hold the index that the Course being searched for is in
    int index = 0;

    //search for the Course to be printed and store the index in index (-1 is stored if not found)
    index = search(courses, id);

    //if the Course being searched for wasn't found then print an error message and return
    if (index == -1) {
        cout << "The course you are trying to print does not exist" << endl;
        return;
    }

    //print the Course's data
    cout << courses.at(index).courseId << ", " << courses.at(index).title << endl;
    if (courses.at(index).prereqs.size() != 0) {
        cout << "Prerequisites: ";
        for (int i = 0; i < courses.at(index).prereqs.size(); i++) {
            cout << courses.at(index).prereqs.at(i);
            if (i != courses.at(index).prereqs.size() - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl << endl;
    return;
}

/**
 * A function to sort a set of courses
 *
 * @param courses the vector in which the course to print is in
 * lowIndex the low end of the sub vector being sorted
 * highIndex the high end of the sub vector being sorted
 * 
 * @return the highIndex
 */
int Partition(vector<Course>& courses, int lowIndex, int highIndex) {
    //find a pivot value from the middle of the vector being sorted
    int midPoint = lowIndex + (highIndex - lowIndex) / 2;
    string pivot = courses.at(midPoint).courseId;

    //initialize a temporary Course object for swapping purposes
    Course tempCourse;

    //initialize a housekeeping variable and a while loop that runs while until said variable is true
    bool done = false;
    while (!done) {

        //if the course ID is lower than the pivot value in alphanumeric order then lowIndex is incremented
        while (courses.at(lowIndex).courseId < pivot) {
            lowIndex += 1;
        }

        //if the course ID is higher than the pivot value in alphanumeric order then highIndex is decremented
        while (pivot < courses.at(highIndex).courseId) {
            highIndex -= 1;
        }

        //if the vector is sorted into values higher and lower than the pivot point, then see done to true
        if (lowIndex >= highIndex) {
            done = true;
        }

        //if it isn't then swap the values that need to be on the other side of the pivot value and increment lowIndex and decrement highIndex
        else {
            tempCourse = courses.at(lowIndex);
            courses.at(lowIndex) = courses.at(highIndex);
            courses.at(highIndex) = tempCourse;

            lowIndex += 1;
            highIndex -= 1;
        }
    }

    //return highIndex
    return highIndex;
}

/**
 * A function to sort a set of courses
 *
 * @param courses the vector in which the course to print is in
 * lowIndex the low end of the sub vector being sorted
 * highIndex the high end of the sub vector being sorted
 */
void quickSort(vector<Course>& courses, int lowIndex, int highIndex) {

    //if the vector is sorted then return
    if (lowIndex >= highIndex) {
        return;
    }
    
    // Partition the data within the array. Value lowEndIndex 
    // returned from partitioning is the index of the low partition's last element.
    int lowEndIndex = Partition(courses, lowIndex, highIndex);

    // Recursively sort low partition (lowIndex to lowEndIndex) 
    // and high partition (lowEndIndex + 1 to highIndex)
    quickSort(courses, lowIndex, lowEndIndex);
    quickSort(courses, lowEndIndex + 1, highIndex);
}

/**
 * The one and only main() method
 */
int main() {

    //initialize vector and user choice variables
    vector<Course> courses;
    string tempString;
    int userChoice;

    //print menu of choices and prompt user for what they want to do
    cout << "Welcome to the course planner." << endl << endl;
    printMenu();
    cin >> userChoice;

    //while the user hasn't chosen to exit the program
    while (userChoice != 4) {

        //run code based on user's choice
        switch (userChoice) {

            //load the courses vector based on an input file of the user's choosing
            case 1:
                courses = loadVector();
                break;

            //sort the courses vector into alphanumeric order and print them all
            case 2:
                quickSort(courses, 0, courses.size() - 1);
                for (int i = 0; i < courses.size(); i++) {
                    cout << courses.at(i).courseId << ", " << courses.at(i).title << endl;
                }
                break;

            //prompt user for a course id that represents the course they want information for and print it
            case 3:
                cout << "What is the Course ID for the course you want to print?" << endl;
                cin >> tempString;
                printCourse(courses, tempString);
                break;

            //error catching if the user enters an innapropriate choice
            default:
                cout << userChoice << " is not a valid option." << endl << endl;
        }

        //re-print the menu and prompt the user for a choice
        printMenu();
        cin >> userChoice;
    }

    //print a goodbye message to the user and exit the program
    cout << "Thank you for using the course planner!";
    return 0;
}
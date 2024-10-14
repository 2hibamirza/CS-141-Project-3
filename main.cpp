/**------------------------------------------
  Program 3: Typing Tutor
  Course: CS 141, Spring 2023, UIC
  Author: Hiba Mirza, Starter code provided by George Maratos and David Hayes 
 ---------------------------------------------**/

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void display_header(const int ndict, const int nmiss) {
  // Prints out the header
  cout << endl;
  cout << "*** Typing Tutor" << endl;
  cout << "There are " << ndict << " words in the dictionary." << endl;
  cout << "There are " << nmiss << " commonly misspelled words." << endl;
}

void display_menu_options() {
  /* This function prints out all the menu options, it is
  given to you as part of the starter code. If you change
  it,  you run the  risk of failing  an early test of the
  autograder.*/

  cout << endl
       << "*** Select a menu option:" << endl
       << "1. Spell commonly misspelled words" << endl
       << "2. Type random words from the full dictionary" << endl
       << "3. Display some words from one of the lists" << endl
       << "4. Use binary search to lookup a word in the dictionary" << endl
       << "5. Exit the program." << endl
       << "Your choice: ";
}

vector<int> 
randn(const int n, const int max) 
{
  // Create and return a vector that has n random numbers drawn from  between 0 and the value of the parameter max
  vector<int> temp;
  for (int i = 0; i < n; ++i) {
    temp.push_back(rand() % (max + 1));
  }
  return temp;
}

ifstream open(const string &src_file) {
  ifstream src{src_file};
  if (!src.is_open()) {
    cout << "Could not find file " << src_file << "." << endl;
    exit(1);
  }
  return src;
}

void extract_dict(vector<string> &dst, const string &src_file) 
{
  // Reads the data from the file, the file name is src_file
  ifstream src = open(src_file);
  string line;
  while (getline(src, line)) {
    // stores each line in the vector dst
    dst.push_back(line);
  }
  src.close();
}

void extract_misspelled(vector<string> &misspelled, vector<string> &correct,
                        const string &src_file) 
{
  ifstream infile(src_file);
  string line;
  // Extracts the word pairs (misspelled, correct) from the file and store them in their respective vectors
  while (getline(infile, line)) {
      int emptySpace = line.find(" ");
      string misspelledWord = line.substr(0, emptySpace);
      string correctWord = line.substr(emptySpace + 1);

      misspelled.push_back(misspelledWord);
      correct.push_back(correctWord);
  }
}

void display_elems(const vector<string> &src, const vector<int> &positions) {
  // Prints out the elements in the vector src only at those indices,  with a space between each of the elements
  // Doesn't print a space after the last element
  for (int i = 0; i < positions.size(); i++) {
    cout << src[positions[i]];
    if (i < positions.size() - 1) {
      cout << " ";
    }
  }
}

void extract_spelling_answers(vector<string> &dst,
                              const vector<string> &correct,
                              const vector<int> positions) 
{
  // Copies the elements at the indices, in the positions vector, from the correct vector, into the vector dst
  for (int i = 0; i < positions.size(); ++i) {
    dst.push_back(correct[positions[i]]);
  }
}

void extract_spelling_mistakes(vector<string> &dst, const string &to_grade,
                               const vector<string> &correct) 
{
  // Populates the vector dst with all the spelling errors in the user's answer (to_grade)
  vector<string> words;
      string word;
      for (char c : to_grade) {
          if (isspace(c)) {
              if (!word.empty()) {
                  words.push_back(word);
                  word.clear();
              }
          } else {
              word += c;
          }
      }
      if (!word.empty()) {
          words.push_back(word);
      }
  
      for (int i = 0; i < words.size(); ++i) {
          if (words[i] != correct[i]) {
              dst.push_back(words[i] + " -> " + correct[i]);
          }
      }
}

void report_misspell_mistakes(const vector<string> &mistakes,
                              const int numWords) 
{
  // Print out the spelling mistakes to the user
  int numberCorrect = numWords - mistakes.size();
    int total_score = numberCorrect * 3 - mistakes.size() * 6;
    if (mistakes.empty()) {
        cout << "No misspelled words!" << endl;
    } else {
        cout << "Misspelled words: " << endl;
        for (const string& mistake : mistakes) {
            cout << "    " << mistake << endl;
        }
    }
    cout << numberCorrect * 3 << " points: " << numberCorrect << " spelled correctly x 3 points for each." << endl;
    cout << mistakes.size() * 6 << " point penalty: " << mistakes.size() << " spelled incorrectly x -6 points for each." << endl;
    cout << "Score: " << total_score << endl;
}

void evaluate_user_misspells(const vector<string> &src,
                             const vector<int> &positions) {
  // Reads the users input for the typing test, and reports back their performance
  string input;
  getline(cin, input);
  vector<string> mistakes;
  vector<string> answers;
  extract_spelling_answers(answers, src, positions);
  extract_spelling_mistakes(mistakes, input, answers);
  report_misspell_mistakes(mistakes, positions.size());
}

void misspelled(const vector<string> &mspelled, const vector<string> &correct) {
  // Prints out the starting message for option 1, displays 5 random words from the dictionary, and evaluates the user's input
  vector<int> numbers = randn(NMISP, mspelled.size() - 1);
  cout << endl;
  cout << "*** Using commonly misspelled words." << endl;
  display_elems(mspelled, numbers);
  cout << endl;
  cout << "Type the words above with the correct spelling:" << endl;
  evaluate_user_misspells(correct, numbers);
}

void full_dict(const vector<string> &dict) {
  // Prints out starting message for menu option 2, displays five random words from the full dictionary, and then evaluates the user's input
  int wordCount = 5;
  vector<int> indicesCount =
      randn(wordCount,
            dict.size() - 1);
  cout << endl;
  cout << "*** Using full dictionary." << endl;
  display_elems(dict, indicesCount);
  cout << endl;
  cout << "Correctly type the words above:" << endl;
  evaluate_user_misspells(dict, indicesCount);
}

void display_words_prompt() {
  // This displays the data set choices for menu option 3
  cout << endl
       << "*** Displaying word list entries." << endl
       << "Which words do you want to display?" << endl
       << "A. Dictionary of all words" << endl
       << "B. Commonly misspelled words (wrongly spelled)" << endl
       << "C. Commonly misspelled words (correctly spelled)" << endl
       << "Your choice: ";
}

void print_range(const vector<string> &data) {
  // Displays the starting  message for menu option 3, reads the users chosen range, verify that the indices are in range, then prints out the  words in the index range
  cout << "*** Enter the start and end indices between 0 and "
       << data.size() - 1 << ": ";
  int start, end;
  cin >> start >> end;

  if (start < 0 || start > end || end >= data.size()) {
    cout << "Invalid range!" << endl;
  }
  for (int i = start; i <= end; i++) {
    cout << i << ". " << data[i] << endl;
  }
}

void display_in_range(const vector<string> &dict,
                      const vector<string> &mspelled,
                      const vector<string> &correct) {
  // Displays the initial messages for menu option 3, reads in the user's choice of dataset, and displays the data in range
  display_words_prompt();
  char choice;
  cin >> choice;

  switch (choice) {
  case 'A':
  case 'a':
    print_range(dict);
    break;
  case 'B':
  case 'b':
    print_range(mspelled);
    break;
  case 'C':
  case 'c':
    print_range(correct);
    break;
  default:
    cout << "Invalid choice!" << endl;
    break;
  }
}

void bsearch(const vector<string> &data) {
  // Reads in the word to search for, from the user, and runs the Binary Search Algorithm to search for the word. 
  // Prints out the current word, that is being compared with the target, at every step of the algorithm
  cout << "*** Enter word to search: ";
  string wordToSearch;
  cin >> wordToSearch;
  int start = 0, end = data.size(), mid, i = 1;
  do {
    mid = (start + end) / 2;
    cout << setw(5) << i << ". Comparing to: " << data[mid] << endl;
    if (data[mid] < wordToSearch) {
      start = mid + 1;
    } 
    else if (data[mid] > wordToSearch) {
      end = mid;
    } 
    else {
      cout << wordToSearch << " was found." << endl;
      return;
    }
    i++;
  } 
  while (start < end);
  cout << wordToSearch << " was NOT found." << endl;
}

void extract_all_data(vector<string> &dict, vector<string> &mspelled,
                      vector<string> &correct) {
  extract_dict(dict, "dictionary.txt");
  extract_misspelled(mspelled, correct, "misspelled.txt");
}

int main() {
  srand(1);
  vector<string> dict, mspelled, correct;
  extract_all_data(dict, mspelled, correct);
  display_header(dict.size(), correct.size());
  unsigned int choice;
  do {
    display_menu_options();
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice) {
    case 1:
      misspelled(mspelled, correct);
      break;
    case 2:
      full_dict(dict);
      break;
    case 3:
      display_in_range(dict, mspelled, correct);
      break;
    case 4:
      bsearch(dict);
      break;
    case 5:
      cout << "Exiting." << endl;
      break;
    default:
      cout << "No menu option " << choice << ". "
           << "Please try again." << endl;
    }
  } while ((choice != 5) && !cin.eof());

  return 0;
}

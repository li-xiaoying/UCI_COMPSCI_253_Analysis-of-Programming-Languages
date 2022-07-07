#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// function to convert capital letters to lowercase letters
string toLowerCase(string s) {
	for (char& c : s) {
    if (c >= 'A' && c <= 'Z') {
      c = c - 'A' + 'a';
    }
  }
	return s;
}

// function to check if a given character is a letter
bool isLetter(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

int main(int argc, char* argv[]) {
  // the global vector of [frequency, word] pairs
  vector< pair<int, string> > wordFreqs;
  // create a file instance for stop_words
	ifstream inputFile("../stop_words.txt");
	string line;
  // the vector of stop words
  vector<string> stopWords;

  // load the vector of stop words
	while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			int j = i;
			while (j < line.size() && line[j] != ',') {
        j++;
      }
			string word = line.substr(i, j - i);
			word = toLowerCase(word);
			stopWords.push_back(word);
			i = j;
		}
	}

  inputFile.close();
  // create a file instance for the input file
	inputFile = ifstream(argv[1]);

  // iterate through the input file one line at a time
	while (getline(inputFile, line)) {
    // iterate through characters of the current line
		for (int i = 0; i < line.size(); i++) {
			if (!(isLetter(line[i]))) {
        continue;
      }
      // the start of a word found
			int j = i;
			while (j < line.size() && isLetter(line[j])) {
        j++;
      }
      // the end of a word found
			string word = line.substr(i, j - i);
			word = toLowerCase(word);
			i = j;
      // ignore stop words
      bool isStopWords = false;
      for (const auto &item : stopWords) {
        if (item == word) {
            isStopWords = true;
            break;
        }
      }
      // see if the word already exists
      bool wordFound = false;
			if (word.size() >= 2 && !isStopWords) {
        for (auto &item : wordFreqs) {
          if (item.second == word) {
            item.first++;
            wordFound = true;
            break;
          }
        }
        if (!wordFound) {
          wordFreqs.push_back(make_pair(1, word));
        }
			}
		}
	}

  inputFile.close();
  // sort and find the 25 most frequently occurring words
	sort(wordFreqs.begin(), wordFreqs.end());
	reverse(wordFreqs.begin(), wordFreqs.end());

  for (int i = 0; i < min((int)wordFreqs.size(), 25); i++) {
		cout << wordFreqs[i].second << "  -  " << wordFreqs[i].first << endl;
	}
  
  return 0;
}
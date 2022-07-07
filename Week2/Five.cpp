#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// the shared mutable data
map<string, bool> stopWords;
map<string, int> wordsFreqs;
vector<pair<int, string>> freqsPairs;

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

// precedures
void readStopWords(string pathToFile) {
  // create a file instance for stop_words
	ifstream inputFile(pathToFile);
	string line;

  // load the list of stop words
	while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			int j = i;
			while (j < line.size() && line[j] != ',') {
        j++;
      }
			string word = line.substr(i, j - i);
			word = toLowerCase(word);
			stopWords[word] = true;
			i = j;
		}
	}
  
	inputFile.close();
}

void scanInputFile(string pathToFile) {
  // create a file instance for the input file
	ifstream inputFile(pathToFile);
	string line;

  // iterate through the input file one line at a time
	while (getline(inputFile, line)) {
    // iterate through characters of the current line
		for (int i = 0; i < line.size(); i++) {
			char c = line[i];
			if (!(isLetter(c))) {
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
      // see if the word already exists
			if (word.size() >= 2 && !stopWords[word]) {
				wordsFreqs[word]++;
			}
		}
	}
  
	inputFile.close();
}

void frequencies() {
	for (auto& item : wordsFreqs) {
		freqsPairs.emplace_back(item.second, item.first);
	}
}

void sort() {
  sort(freqsPairs.begin(), freqsPairs.end());
	reverse(freqsPairs.begin(), freqsPairs.end());
}

int main(int argc, char* argv[]) {
  readStopWords("../stop_words.txt");
  scanInputFile(argv[1]);
  frequencies();
  sort();
  
  for (int i = 0; i < min((int)freqsPairs.size(), 25); i++) {
		cout << freqsPairs[i].second << "  -  " << freqsPairs[i].first << endl;
	}
  
	return 0;
}
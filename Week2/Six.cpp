#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
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

// funcional adstractions
map<string, bool> readStopWords(string pathToFile) {
  map<string, bool> stopWords;
  // create a file instance for stop_words
	ifstream inputFile(pathToFile);
	string line;

  // load the vector of stop words
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
  return stopWords;
}

map<string, int> scanInputFile(string pathToFile, map<string, bool> stopWords) {
  map<string, int> wordsFreqs;
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
  return wordsFreqs;
}

vector<pair<int, string>> frequencies(map<string, int> wordsFreqs) {
  vector<pair<int, string>> freqsPairs;
	for (auto& item : wordsFreqs) {
		freqsPairs.emplace_back(item.second, item.first);
	}

  return freqsPairs;
}

vector<pair<int, string>> sort(vector<pair<int, string>> freqsPairs) {
  sort(freqsPairs.begin(), freqsPairs.end());
	reverse(freqsPairs.begin(), freqsPairs.end());
  return freqsPairs;
}

void print(vector<pair<int, string>> freqsPairs) {
  for (int i = 0; i < min((int)freqsPairs.size(), 25); i++) {
		cout << freqsPairs[i].second << "  -  " << freqsPairs[i].first << endl;
	}
}

int main(int argc, char* argv[]) {
  print(sort(frequencies(scanInputFile(argv[1], readStopWords("../stop_words.txt")))));
	return 0;
}
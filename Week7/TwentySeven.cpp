#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// the columns
// each column is a data element and a formula
unordered_set<string> stopWords;
vector<string> allWords;
vector<string> nonStopWords;
vector<string> uniqueWords;
vector<int> counts;
vector<pair<int, string>> sortedData;

void loadStopWords() {
  stopWords.clear();
	ifstream inputFile("../stop_words.txt");
	string line;
  
	while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			int j = i;
			while (j < line.size() && line[j] != ',') {
        j++;
      }
      string word = line.substr(i, j - i);
      transform(word.begin(), word.end(), word.begin(), ::tolower);
			stopWords.insert(word);
			i = j;
		}
	}
  
	inputFile.close();
}

void loadAllWords(string pathToFile) {
	ifstream inputFile(pathToFile);
	string line;

  while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			char c = line[i];
			if (!(isalpha(c))) {
        continue;
      }
			int j = i;
			while (j < line.size() && isalpha(line[j])) {
        j++;
      }
			string word = line.substr(i, j - i);
      transform(word.begin(), word.end(), word.begin(), ::tolower);
			i = j;
      if (word.size() >= 2) {
        allWords.push_back(word);
      }
		}
	}
  
	inputFile.close();
}

void updateNonStopWords() {
  nonStopWords.clear();
  nonStopWords.shrink_to_fit();
  
  for (auto word : allWords) {
    if (!stopWords.count(word)) {
      nonStopWords.push_back(word);
    }
  }
}

void updateUniqueWords() {
  uniqueWords.clear();
  uniqueWords.shrink_to_fit();
  unordered_set<string> tempWords(nonStopWords.begin(), nonStopWords.end());
  
  for (auto word : tempWords) {
    uniqueWords.push_back(word);
  }
}

void updateCounts() {
  counts.clear();
  counts.shrink_to_fit();
  unordered_map<string, int> wordsFreqs;
  
  for (auto word : nonStopWords) {
    wordsFreqs[word] += 1;
  }
  
  for (auto word : uniqueWords) {
    counts.push_back(wordsFreqs[word]);
  }
}

void updateSortedData() {
  sortedData.clear();
  sortedData.shrink_to_fit();
  
  for (int i = 0; i < uniqueWords.size(); i++) {
    sortedData.push_back({counts[i], uniqueWords[i]});
  }
  
  sort(sortedData.begin(), sortedData.end());
  reverse(sortedData.begin(), sortedData.end());
}

// the entire spreadsheet
// the active procedure over the columns of data
// call this everytime the input data changes, or periodically
void update(string pathToFile) {
  // load the fixed data into the first 2 columns
  loadStopWords();
  loadAllWords(pathToFile);
  // update the columns with formulas
  updateNonStopWords();
  updateUniqueWords();
  updateCounts();
  updateSortedData();
}

int main(int argc, char* argv[]) {
  // exercise 27.1
  cout << "Result for pride-and-prejudice.txt: " << endl;
  update(argv[1]);
  
  for (int i = 0; i < min((int)sortedData.size(), 25); i++) {
    cout << sortedData[i].second << "  -  " << sortedData[i].first << endl;
  }

  // exercise 27.2 interactive
  string pathToFile;
  while (true) {
    cout << "Please enter path to another file to test exercise 27.2 or enter 'exit' to exit the program: " << endl;
    cin >> pathToFile;
      if (pathToFile == "exit") {
        break;
      }
      update(pathToFile);
      for (int i = 0; i < min((int)sortedData.size(), 25); i++) {
        cout << sortedData[i].second << "  -  " << sortedData[i].first << endl;
      }
    }
  
  return 0;
}
#include <iostream>
#include <algorithm>
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int noOperation() {
  return 0;
}

void print(vector<pair<int, string>> freqs, function<int()> func) {
  for (int i = 0; i < min((int)freqs.size(), 25); i++) {
    cout << freqs[i].second << "  -  " << freqs[i].first << endl;
  }
  func();
}

void sortFreqs(vector<pair<int, string>> freqs, function<void(vector<pair<int, string>>, function<int()>)> func) {
  sort(freqs.begin(), freqs.end());
	reverse(freqs.begin(), freqs.end());
  func(freqs, &noOperation);
}

void removeStopWords(map<string, int> freqs, function<void(vector<pair<int, string>>, function<void(vector<pair<int, string>>, function<int()>)>)> func) {
  map<string, bool> stopWords;
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
			stopWords[word] = true;
			i = j;
		}
	}
  
  inputFile.close();
  
  for (auto& item : freqs) {
    if (stopWords[item.first] == true) {
      item.second = 0;
    }
  }

  vector<pair<int, string>> freqsPairs;
  
  for (auto& item : freqs) {
    freqsPairs.emplace_back(item.second, item.first);
  }

  func(freqsPairs, &print);
}

void readFile(string pathToFile, function<void(map<string, int>, function<void(vector<pair<int, string>>, function<void(vector<pair<int, string>>, function<int()>)>)>)> func) {
	ifstream inputFile(pathToFile);
  string line;
  map<string, int> wordsFreqs;
  
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
			if (word.size() >= 2) wordsFreqs[word]++;
		}
	}
  
	inputFile.close();
  func(wordsFreqs, &sortFreqs);
}

int main(int argc, char* argv[]) {
  readFile(argv[1], &removeStopWords);
}
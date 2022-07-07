#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

#define NUMPARTITION 200

// functions for map reduce
vector<vector<string>> partition(vector<string> dataStrings) {
  vector<vector<string>> lines(NUMPARTITION);
  
  for (int i = 0; i < dataStrings.size(); i++) {
    lines[i % NUMPARTITION].push_back(dataStrings[i]);
  }
  
  return lines;
}

vector<pair<string, int>> splitWords(const vector<string> lines) {
  map<string, bool> stopWords;
  vector<pair<string, int>> wordsFreqs;
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
  
  for (auto line : lines) {
    stringstream currentLine(line);
    string word;
    while (getline(currentLine, word, ' ')) {
      if (word.size() >= 2 && !stopWords[word]) {
        wordsFreqs.push_back({word, 1});
      }
    }
  }
  
  return wordsFreqs;
}

unordered_map<string, vector<pair<string, int>>> regroup(vector<vector<pair<string, int>>> pairsList) {
  unordered_map<string, vector<pair<string, int>>> mapping;
  for (auto pairs : pairsList) {
    for (auto p : pairs) {
      string word = p.first;
      if (word[0] >= 'a' && word[0] <= 'e') {
        mapping["abcde"].push_back(p);
      }
      else if (word[0] >= 'f' && word[0] <= 'j') {
        mapping["fghij"].push_back(p);
      }
      else if (word[0] >= 'k' && word[0] <= 'o') {
        mapping["klmno"].push_back(p);
      }
      else if (word[0] >= 'p' && word[0] <= 't') {
        mapping["pqrst"].push_back(p);
      }
      else {
        mapping["uvwxyz"].push_back(p);
      }
    }
  }
  
  return mapping;
}

vector<pair<int, string>> countWords(unordered_map<string, vector<pair<string, int>>> mapping) {
  unordered_map<string, int> freqs;
  vector<pair<int, string>> wordsFreqs;
  
  for (auto [_, pairs] : mapping) {
    for(auto p: pairs) {
      freqs[p.first] += p.second;
    }
  }
  
  for (auto [word, count] : freqs) {
    wordsFreqs.push_back({count, word});
  }
  
  return wordsFreqs;
}

// auxiliary function
vector<string> readInputFile(string pathToFile) {
  vector<string> dataStrings;
  ifstream inputFile(pathToFile);
  string line;
  
  while (getline(inputFile, line)) {
    transform(line.begin(), line.end(), line.begin(), [](auto c) {
      return isalpha(c) ? tolower(c) : ' ';
    });
    dataStrings.push_back(line);
  }
  
  inputFile.close();
  return dataStrings;
}

// the main function
int main(int argc, char *argv[]) {
  vector<vector<string>> parts = partition(readInputFile(argv[1]));
  vector<vector<pair<string, int>>> splits;
  
  for_each(parts.begin(), parts.end(), [&](auto part){
    splits.push_back(splitWords(part));
  });
  
  vector<pair<int, string>> wordsFreqs = countWords(regroup(splits));
  sort(wordsFreqs.rbegin(), wordsFreqs.rend());
  
  for(int i = 0; i < 25; i++) {
    cout << wordsFreqs[i].second << "  -  " << wordsFreqs[i].first << endl;
  }

  return 0;
}
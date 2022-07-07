#include <iostream>
#include <algorithm>
#include <any>
#include <queue>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <thread>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

#define NUMTHREAD 3
map<string, bool> stopWords;

// two data spaces
vector<vector<string>> wordSpace(NUMTHREAD);
vector<unordered_map<string, int>> freqSpace(NUMTHREAD);

// worker function that consumes words from the word space
// and sends partial results to the frequency space
void processWords(int index) {
  for (auto line : wordSpace[index]) {
    stringstream currentLine(line);
    string word;
    
    while (getline(currentLine, word, ' ')) {
      if (word.size() >= 2 && !stopWords[word]) {
        freqSpace[index][word] += 1;
      }
    }
  }
}

int main(int argc, char* argv[]) {
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
  inputFile = ifstream(argv[1]);
  vector<string> lines;
  
  while (getline(inputFile, line)) {
    transform (line.begin(), line.end(), line.begin(), [](auto c) {
      return isalpha(c) ? tolower(c) : ' ';
    });
    lines.push_back(line);
  }
  
  for (int i = 0; i < lines.size(); i++) {
    wordSpace[i % NUMTHREAD].push_back(lines[i]);
  }
  
  inputFile.close();
  thread t[NUMTHREAD];
  
  // create the workers and launch them at their jobs
  // have this thread populate the word space
  for (int i = 0; i < NUMTHREAD; i++) {
    t[i] = thread(processWords, i);
  }

  // wait for the workers to finish
  for (int i = 0; i < NUMTHREAD; i++) {
    t[i].join();
  }
  
  // merge the partial frequency results
  // by consuming frequency data from the frequency space
  unordered_map<string, int> freqs;
  for (auto item : freqSpace) {
    for (auto [word, count]: item) {
      freqs[word] += count;
    }
  }
  
  vector<pair<int, string>> wordFreqs;
  for (auto [word, count] : freqs) {
    wordFreqs.push_back({count, word});
  }
  
  sort(wordFreqs.rbegin(), wordFreqs.rend());
  for (int i = 0; i < min((int)wordFreqs.size(), 25); i++) {
    cout << wordFreqs[i].second << "  -  " << wordFreqs[i].first << endl;
  }
  
  return 0;
}
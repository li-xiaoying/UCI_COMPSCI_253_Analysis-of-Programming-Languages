#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

// recursive sort function to sort the word frequency dictionary
void recursiveSort(pair<string, int> freqs[], int n) {
  // base case
  if (n == 1) {
    return;
  }

  // one pass of recursive sort
  // after this pass, the largest element is moved to end
  for (int i = 0; i < n-1; i++ ) {
    if (freqs[i].second < freqs[i+1].second) {
      swap (freqs[i], freqs[i+1]);
    }
  }
  
  //largest element is fixed, recur for remaining array
  recursiveSort(freqs, n-1);
}

int main(int argc, char* argv[]) {
  map<string, bool> stopWords;
  map<string, int> wordsFreqs;
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
			if (word.size() >= 2 && !stopWords[word]) wordsFreqs[word]++;
		}
	}
  
	inputFile.close();
  vector<pair<string, int>> freqsPairs;
  
  for (auto& item : wordsFreqs) {
    freqsPairs.emplace_back(item.first, item.second);
  }

  int size = freqsPairs.size();
  pair<string, int> freqsArr[size];
  
  for (int i = 0; i < size; i++) {
    freqsArr[i] = freqsPairs[i];
  }
  
  recursiveSort(freqsArr, size);

  for (int i = 0; i < min(size, 25); i++) {
    cout << freqsArr[i].first << "  -  " << freqsArr[i].second << endl;
  }
  
	return 0;
}
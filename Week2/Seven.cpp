#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
  map<string, bool> stopWords;
  map<string, int> wordsFreqs;
	ifstream inputFile("../stop_words.txt");
	string line;
	while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			int j = i;
			while (j < line.size() && line[j] != ',') j++;
      transform(line.substr(i, j - i).begin(), line.substr(i, j - i).end(), line.substr(i, j - i).begin(), ::tolower);
			stopWords[line.substr(i, j - i)] = true;
			i = j;
		}
	} 
	inputFile.close();
	inputFile = ifstream(argv[1]);
	while (getline(inputFile, line)) {
		for (int i = 0; i < line.size(); i++) {
			char c = line[i];
			if (!(isalpha(c))) continue;
			int j = i;
			while (j < line.size() && isalpha(line[j])) j++;
			string word = line.substr(i, j - i);
      transform(word.begin(), word.end(), word.begin(), ::tolower);
			i = j;
			if (word.size() >= 2 && !stopWords[word]) wordsFreqs[word]++;
		}
	}
	inputFile.close();
  vector<pair<int, string>> freqsPairs;
  for (auto& item : wordsFreqs) freqsPairs.emplace_back(item.second, item.first);
  sort(freqsPairs.begin(), freqsPairs.end());
	reverse(freqsPairs.begin(), freqsPairs.end());  
  for (int i = 0; i < min((int)freqsPairs.size(), 25); i++) cout << freqsPairs[i].second << "  -  " << freqsPairs[i].first << endl;
	return 0;
}
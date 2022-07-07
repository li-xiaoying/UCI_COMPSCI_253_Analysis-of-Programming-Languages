#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

// models the stop word filter
class StopWordsManager {
  private:
    map<string, bool> stopWords;

    bool setStopWords() {
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
      return 0;
    }

    bool getStopWords(string word) {
       return word.size() >= 2 && !stopWords[word];
    }

  public:
    bool dispatch(string message[]) {
      if (message[0] == "setStopWords") {
        return setStopWords();
      }
      else if (message[0] == "getStopWords") {
        return getStopWords(message[1]);
      }
      else {
        throw "Message not understood!";
      }
    }  
};

// models the contents of the file
class DataStorageManager {
  private:
    vector<string> data;

    vector<string> setWords(string pathTofile) {
      ifstream inputFile(pathTofile);
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
          data.push_back(word);
    			i = j;
    		}
      }
    	inputFile.close();
      return data;
    }

    vector<string> getWords() {
      return data;
    }

  public:
    vector<string> dispatch(string message[]) {
      if (message[0] == "setWords") {
        return setWords(message[1]);
      }
      else if (message[0] == "getWords") {
        return getWords();
      }
      else {
        throw "Message not understood!";
      }
    }
};

// keeps the word frequency data
class WordFrequencyManager {
  private:
    map<string, int> wordsFreqs;
    vector<pair<int, string>> freqsPairs;

    vector<pair<int, string>> incrementFreqs(string word) {
      wordsFreqs[word]++;
      return freqsPairs;
    }

    vector<pair<int, string>> sortFreqs() {
      for (auto& item : wordsFreqs) {
        freqsPairs.emplace_back(item.second, item.first);
      }
      sort(freqsPairs.begin(), freqsPairs.end());
    	reverse(freqsPairs.begin(), freqsPairs.end());
      return freqsPairs;
    }

  public:
    vector<pair<int, string>> dispatch(string message[]) {
      if (message[0] == "incrementFreqs") {
        return incrementFreqs(message[1]);
      }
      else if (message[0] == "sortFreqs") {
        return sortFreqs();
      }
      else {
        throw "Message not understood!";
      }
    }
};

class WordFrequencyController {
  private:
    StopWordsManager swManager;
    DataStorageManager dsManager;
    WordFrequencyManager wfManager;

    void init(string pathToFile) {
      string swMessage[] = {"setStopWords"};
      swManager.dispatch(swMessage);
      string dsMessage[] = {"setWords", pathToFile};
      dsManager.dispatch(dsMessage);
    }

    void run() {
      string dsMessage[] = {"getWords"};
      for (auto& item : dsManager.dispatch(dsMessage)) {
        string swMessage[] = {"getStopWords", item};
        if (swManager.dispatch(swMessage)) {
          string wfMessage[] = {"incrementFreqs", item};
          wfManager.dispatch(wfMessage);
        }
      }

      vector<pair<int, string>> freqs;
      string wfMessage[] = {"sortFreqs"};
      freqs = wfManager.dispatch(wfMessage);
      for (int i = 0; i < min((int)freqs.size(), 25); i++) {
        cout << freqs[i].second << "  -  " << freqs[i].first << endl;
      }
    }

  public:  
    void dispatch(string message[]) {
      if (message[0] == "init") {
        init(message[1]);
      }
      else if (message[0] == "run") {
        run();
      }
      else {
        throw "Message not understood!";
      }
    }
};

// the main function
int main(int argc, char* argv[]) {
  WordFrequencyController wfController;
  string wfMessage1[] = {"init", argv[1]};
  wfController.dispatch(wfMessage1);
  string wfMessage2[] = {"run"};
  wfController.dispatch(wfMessage2);
  return 0;
}
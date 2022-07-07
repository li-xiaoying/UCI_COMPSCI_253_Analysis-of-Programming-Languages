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

// the "I'll call you back" word frequency framework
class WordFrequencyFramework {
  private:
    vector<function<void(string)>> loadEventHandlers;
    vector<function<void()>> doworkEventHandlers;
    vector<function<void()>> endEventHandlers;

  public:
    void registerForLoadEvent(function<void(string)> handler) {
      loadEventHandlers.push_back(handler);
    }

    void registerForDoworkEvent (function<void()> handler) {
      doworkEventHandlers.push_back(handler);
    }

    void registerForEndEvent (function<void()> handler) {
      endEventHandlers.push_back(handler);
    }

    void run(string pathToFile) {
      for (auto& item : loadEventHandlers) {
        item(pathToFile);
      }
      for (auto& item : doworkEventHandlers) {
        item();
      }
      for (auto& item : endEventHandlers) {
        item();
      }
    }
};

// the entities of the application
class StopWordFilter {
  private:
    map<string, bool> stopWords;

    void load() {
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
    }

  public:
    StopWordFilter(WordFrequencyFramework& wffapp) {
      wffapp.registerForLoadEvent(bind(&StopWordFilter::load, this));
    }

    bool isStopWord(string word) {
      return word.size() >= 2 && !stopWords[word];
    }
};

class DataStorage {
  private:
    vector<string> data;
    vector<function<void(string)>> wordEventHandlers;
    StopWordFilter& swf;

    void load(string pathTofile) {
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
    }

    void produceWords() {
      for (auto& item : data) {
        if (swf.isStopWord(item)) {
          for (auto handler : wordEventHandlers ){
            handler(item);
          }
        }
      }
    }

  public:
    DataStorage(WordFrequencyFramework& wffapp, StopWordFilter& swfapp): swf {swfapp} {
      wffapp.registerForLoadEvent(bind(&DataStorage::load, this, placeholders::_1));
      wffapp.registerForDoworkEvent(bind(&DataStorage::produceWords, this));
    }
    
    void registerForWordEvent(function<void(string)> handler){
      wordEventHandlers.push_back(handler);
    }
};

class WordFrequencyCounter {
  private:
    map<string, int> wordsFreqs;
    vector<pair<int, string>> freqsPairs;

    void incrementFreqs(string word) {
      wordsFreqs[word]++;
    }

    void printSortedFreqs() {
      for (auto& item : wordsFreqs) {
        freqsPairs.emplace_back(item.second, item.first);
      }
      sort(freqsPairs.begin(), freqsPairs.end());
    	reverse(freqsPairs.begin(), freqsPairs.end());
      for (int i = 0; i < min((int)freqsPairs.size(), 25); i++) {
        cout << freqsPairs[i].second << "  -  " << freqsPairs[i].first << endl;
      }
    }

  public:
    WordFrequencyCounter(WordFrequencyFramework& wffapp, DataStorage& dsapp){
      dsapp.registerForWordEvent(bind(&WordFrequencyCounter::incrementFreqs, this, placeholders::_1));
      wffapp.registerForEndEvent(bind(&WordFrequencyCounter::printSortedFreqs, this));
    }
};

// exercise 15.2
class WordsWithZ{
private:
    int counter = 0;    
    void count(string word) {
      if (word.find('z') != string::npos){
        counter ++;
      }
    }

    void printCounter() {
      cout << "exercise 15.2: words with z  -  " << counter << endl;
    }

public:
    WordsWithZ(WordFrequencyFramework& wffapp, DataStorage& dsapp){
      dsapp.registerForWordEvent(bind(&WordsWithZ::count, this, placeholders::_1));
      wffapp.registerForEndEvent(bind(&WordsWithZ::printCounter, this));
    }
};

// the main function
int main(int argc,char* argv[]) {
  WordFrequencyFramework wffapp;
  StopWordFilter swfapp{wffapp};
  DataStorage dsapp{wffapp,swfapp};
  WordFrequencyCounter wfcapp{wffapp,dsapp};
  WordsWithZ wwzapp{wffapp,dsapp};  // exercise 15.2
  wffapp.run(argv[1]);
  return 0;
}
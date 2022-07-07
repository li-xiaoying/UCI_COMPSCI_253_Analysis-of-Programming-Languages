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

// exercises 28.2 lines vs. characters
string lines(string pathToFile) {
  static ifstream inputFile(pathToFile);
	string line;
  
  if (inputFile.eof()) {
    inputFile.close();
    return "$EOF$";
  }
  
  getline(inputFile, line);
  return line;
}

string allWords(string pathToFile) {
  static bool startFlag = true;
  static string firstLine = lines(pathToFile);
  static stringstream stringStream;
  
  if (startFlag) {
    transform(firstLine.begin(), firstLine.end(), firstLine.begin(), [](auto character){
      return isalpha(character) ? tolower(character) : ' ';
    });
    firstLine.erase(firstLine.find_last_not_of(" ") + 1);
    stringStream = stringstream(firstLine);
    startFlag = false;
  }
  
  string word;
  
  while (getline(stringStream, word, ' ')) {
    if (stringStream.eof()) {
      string line;
      while (line.size() == 0) {
        line = lines(pathToFile);
        if (line == "$EOF$") {
          break;
        }
        transform(line.begin(), line.end(), line.begin(), [](auto character){
          return isalpha(character) ? tolower(character) : ' ';
        });
        line.erase(line.find_last_not_of(" ")+1);
      }
      if (line != "$EOF$") {
        stringStream = stringstream(line);
      }
    }    
    return word;
  }
  
  return "$EOF$";
}

string nonStopWords(string pathToFile) {
  static bool startFlag = true;
  static ifstream stopWordsFile("../stop_words.txt");
  static unordered_set<string> stopWords;
  static string stopWord;
  
  if (startFlag) {
    while (getline(stopWordsFile, stopWord, ','))
      stopWords.insert(stopWord);
      startFlag = false;
  }
  
  string word;
  
  while (word = allWords(pathToFile), word != "$EOF$") {
    if (word.size() >= 2 && !stopWords.count(word)) {
      return word;
    }
  }
    
  return "$EOF$";
}

vector<pair<int, string>> countAndSort(string pathToFile) {
  static bool finishFlag = false;
  static unordered_map<string, int> wordsFreqs;
  static int count = 0;
  string word;
  
  if (finishFlag == true) {
    return {};
  }
  
  while(word = nonStopWords(pathToFile), word != "$EOF$") {
    wordsFreqs[word] += 1;
    count += 1;
    if (count % 5000 == 0) {
      vector<pair<int, string>> sortedFreqs;
      for (auto [w, c] : wordsFreqs) {
        sortedFreqs.push_back({c, w});
      }
      sort(sortedFreqs.begin(), sortedFreqs.end());
      reverse(sortedFreqs.begin(), sortedFreqs.end());
      return sortedFreqs;
    }
  }
  
  vector<pair<int, string>> sortedFreqs;
  
  for (auto [w, c] : wordsFreqs) {
    sortedFreqs.push_back({c, w});
  }
  
  sort(sortedFreqs.begin(), sortedFreqs.end());
  reverse(sortedFreqs.begin(), sortedFreqs.end());
  finishFlag = true;
  return sortedFreqs;
}

int main(int argc, char* argv[]) {
  vector<pair<int, string>> wordsFreqs;
  
  while (wordsFreqs = countAndSort(argv[1]), !wordsFreqs.empty()) {
    cout << "--------------------" << endl;
    for (int i = 0; i < min((int)wordsFreqs.size(), 25); i++) {
      cout << wordsFreqs[i].second << "  -  " << wordsFreqs[i].first << endl;
    }
  }
  
  return 0;
}
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

class TFTheOne {
  public:
    map<string, bool> readStopWords();
    map<string, int> scanInputFile(string, map<string, bool>);
    vector<pair<int, string>> sortFreqs(map<string, int>);
    void print(vector<pair<int, string>>);
};

map<string, bool> TFTheOne::readStopWords() {
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
  return stopWords;
}

map<string, int> TFTheOne::scanInputFile(string pathToFile, map<string, bool> stopWords) {
  map<string, int> wordsFreqs;
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
			if (word.size() >= 2 && !stopWords[word]) wordsFreqs[word]++;
		}
	}
  
	inputFile.close();
  return wordsFreqs;
}

vector<pair<int, string>> TFTheOne::sortFreqs(map<string, int> freqs) {
  vector<pair<int, string>> freqsPairs;
  
  for (auto& item : freqs) {
    freqsPairs.emplace_back(item.second, item.first);
  }
  
  sort(freqsPairs.begin(), freqsPairs.end());
	reverse(freqsPairs.begin(), freqsPairs.end());
  return freqsPairs;
}

void TFTheOne::print(vector<pair<int, string>> freqs) {
  for (int i = 0; i < min((int)freqs.size(), 25); i++) {
    cout << freqs[i].second << "  -  " << freqs[i].first << endl;
  }
}

int main(int argc, char* argv[]) {
  TFTheOne obj;
  obj.print(obj.sortFreqs(obj.scanInputFile(argv[1], obj.readStopWords())));
  return 0;
}

/*
template <class T>
class TFTheOne {
  public:
    TFTheOne(function<T()> func) : _act(func) {}
    T run() {
      return _act();
    }
  private:
    function<T()> _act;
};
*/

/*
template <typename T, typename U>
class TFTheOne {
  private:
    T value;
    TFTheOne (T v) {
      this -> value = v;
    }
  public :
    TFTheOne bind(function<U()> func) {
      this -> value = func.apply(this -> value);
      return this;
    }
    void printme() {
    }
};
*/
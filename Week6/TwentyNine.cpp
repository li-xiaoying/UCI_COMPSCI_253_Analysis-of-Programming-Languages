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
#include <unistd.h>
#include <utility>
#include <bits/stdc++.h>

using namespace std;

class ActiveWFObject {
  public:
    thread t;
    queue<vector<any>> q;
    virtual void dispatch(const vector<any> &message) {};
    void run() {
      while (true) {
        if (!q.empty()) {
          vector<any> message = q.front();
          q.pop();
          if (any_cast<string> (message[0]) == "die") {
            break;
          }
          else {
            dispatch(message);
          }
        }
      }
    }
};

template <typename T> void send(T &receiver, const vector<any> &message) {
  receiver.q.push(message);
}

// models the contents of the file
class DataStorageManager : public ActiveWFObject {
  public:
    string data;
    bool initFinished;

    DataStorageManager() {
      t = thread(&DataStorageManager::run, this);
      initFinished = false;
    }

    void dispatch(const vector<any> &message) {
      if (any_cast<string> (message[0]) == string("init")) {
        init(vector<any> (message.begin() + 1, message.end()));
      }
      else if (any_cast<string> (message[0]) == string("processWords")) {
        processWords();
      }
      else {
        throw invalid_argument("DataStorageManager: Message not understood!");
      }
    }
    
    void init(const vector<any> &message) {
      string pathToFile = any_cast<string> (message[0]);
      string line;
      ifstream inputFile(pathToFile);
      while (getline(inputFile, line)) {
        transform(line.begin(), line.end(), line.begin(), [](auto c) {
          return isalpha(c) ? tolower(c) : ' ';
        });
        data = (data + " " + line);
      }
      inputFile.close();
      initFinished = true;
    }

    void processWords();
};

// models the stop word filter
class StopWordManager : public ActiveWFObject {
  public:
    unordered_set<string> stopWords;
    bool initFinished;

    StopWordManager() {
      t = thread(&StopWordManager::run, this);
      initFinished = false;
    }

    void dispatch(const vector<any> &message) {
      if (any_cast<string>(message[0]) == string("init")) {
        init();
      }
      else if (any_cast<string>(message[0]) == string("filter")) {
        filter(vector<any> (message.begin() + 1, message.end()));
      }
      else {
        throw invalid_argument("StopWordManager: Message not understood!");
      }
    }

    void init() {
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
      initFinished = true;
    }

    void filter(const vector<any> &message);
};

// keeps the word frequency data
class WordFrequencyManager : public ActiveWFObject {
  public:
    unordered_map<string, int> freqs;
    vector<pair<int, string>> wordsFreqs;
    bool incrementFinished;

    WordFrequencyManager() {
      t = thread(&WordFrequencyManager::run, this);
      incrementFinished = false;
    }

    void dispatch(const vector<any> &message) {
      if (any_cast<string>(message[0]) == string("incrementCount")) {
        incrementCount(vector<any> (message.begin() + 1, message.end()));
      }
      else if (any_cast<string>(message[0]) == string("top25")) {
        top25();
      }
      else {
        throw invalid_argument("WordFrequencyManager: Message not understood!");
      }
    }

    void incrementCount(const vector<any> &message) {
      vector<std::string> words = any_cast<vector<string>>(message[0]);
      for (auto word : words) {
        freqs[word] += 1;
      }
      incrementFinished = true;
    }

    void top25();
};

class WordFrequencyController : public ActiveWFObject {
  public:
    WordFrequencyController() {
      t = thread(&WordFrequencyController::run, this);
    }

    void dispatch(const vector<any> &message) {
      if (any_cast<string>(message[0]) == string("start")) {
        start();
      }
      else if (any_cast<string>(message[0]) == string("display")) {
        display(vector<any> (message.begin() + 1, message.end()));
      }
      else {
        throw invalid_argument("WordFrequencyController: Message not understood!");
      }
    }

    void start();
    void display(const vector<any> &message);
};

DataStorageManager dsManager;
StopWordManager swManager;
WordFrequencyManager wfManager;
WordFrequencyController wfController;

void DataStorageManager::processWords() {
  send<StopWordManager>(swManager, {string("filter"), data});
  sleep(1);
  send<WordFrequencyManager>(wfManager, {string("top25")});
}

void StopWordManager::filter(const vector<any> &message) {
  string data = any_cast<string>(message[0]);
  stringstream dataStream(data);
  string word;
  vector<string> filteredWords;
  while (getline(dataStream, word, ' ')) {
    if (word.size() >= 2 && !stopWords.count(word)) {
      filteredWords.push_back(word);
    }
  }
  send(wfManager, {string("incrementCount"), filteredWords});
}

void WordFrequencyManager::top25() {
  for(auto [word, count]: freqs) {
    wordsFreqs.push_back({count, word});
  }
  sort(wordsFreqs.rbegin(), wordsFreqs.rend());
  send<WordFrequencyController>(wfController, {string("display"), wordsFreqs});
}

void WordFrequencyController::start() {
  send<DataStorageManager>(dsManager, {string("processWords")});
}

void WordFrequencyController::display(const vector<any> &message) {
  vector<pair<int, string>> wordsFreqs = any_cast<vector<pair<int, string>>>(message[0]);
  for (int i = 0; i < 25; i++) {
    cout << wordsFreqs[i].second << "  -  " << wordsFreqs[i].first << endl;
  }
  vector<any> dieMessage = {string("die")};
  send<DataStorageManager>(dsManager, dieMessage);
  send<StopWordManager>(swManager, dieMessage);
  send<WordFrequencyManager>(wfManager, dieMessage);
  this -> q.push(dieMessage);
}

// the main function
int main(int argc, char *argv[]) {
  send<StopWordManager>(swManager, {string("init")});
  send<DataStorageManager>(dsManager, {string("init"), string(argv[1])});
  send<WordFrequencyController>(wfController, {string("start")});
  // wait for the active objects to finish
  wfManager.t.join();
  swManager.t.join();
  dsManager.t.join();
  wfController.t.join();
  return 0;
}
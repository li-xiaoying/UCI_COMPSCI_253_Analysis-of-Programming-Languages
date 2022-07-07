const fs = require('fs')

function extractWords(obj, pathToFile) {
  var inputFile = fs.readFileSync(pathToFile, 'utf8');
  obj['data'] = inputFile.match(/[a-zA-Z]{2,}/g).map(word => word.toLowerCase());
}

function loadStopWords(obj) {
  obj['stopWords'] = new Set(
    fs.readFileSync('../stop_words.txt', 'utf8')
      .split(',')
      .map(stopWord => stopWord.replace('\n', ''))
  );
}

function incrementCount(obj, word) {
  if (!obj['freqs'][word]) {
    obj['freqs'][word] = 1;
  }
  else {
    obj['freqs'][word] ++;
  }
}

dataStorageObj = {
  'data': [],
  // comment out for exercise 13.3 this
  // 'init': (pathToFile) => extractWords(this, pathToFile),
  'init': function (pathToFile) { extractWords(this, pathToFile) },
  // comment out for exercise 13.3 this
  // 'words': () => dataStorageObj['data']
  'words': function () { return this['data'] }
};

stopWordsObj = {
  'stopWords': [],
  // comment out for exercise 13.3 this
  // 'init': () loadStopWords(this),
  'init': function () { loadStopWords(this) },
  // comment out for exercise 13.3 this
  // 'isStopWord': (word) => stopWordsObj['stopWords'].has(word)
  'isStopWord': function (word) { return this['stopWords'].has(word) }
};

wordFreqsObj = {
  'freqs': {},
  // comment out for exercise 13.3 this
  // 'incrementCount': (word) => incrementCount(wordFreqsObj, word),
  'incrementCount': function (word) { incrementCount(this, word) },
  /* comment out for exercise 13.3 this
  'sorted': () => {
    let counts = []
    for (let [word, frequency] of Object.entries(wordFreqsObj['freqs'])) {
      counts.push({word, frequency})
    }
    return counts.sort((a, b) => b.frequency - a.frequency)
  }*/
  'sorted': function () {
    let counts = []
    for (let [word, frequency] of Object.entries(this['freqs'])) {
      counts.push({word, frequency})
    }
    return counts.sort((a, b) => b.frequency - a.frequency)
  }
};

stopWordsObj['init']();
dataStorageObj['init'](process.argv[2]);

const words = dataStorageObj['words']();
for (let word of words) {
  if (!stopWordsObj['isStopWord'](word)) {
    wordFreqsObj['incrementCount'](word);
  }
}

/* comment out for exercise 13.2
const wordFreqs = wordFreqsObj['sorted']()
for (let i = 0; i < 25; i++) {
  console.log(wordFreqs[i].word, ' - ', wordFreqs[i].frequency)
}
*/

// exercise 13.2 add method
wordFreqsObj['top25'] = function() {
  // comment out for exercise 13.3 this
  // const wordFreqs = wordFreqsObj['sorted']()
  const wordFreqs = this['sorted']();
  for (let i = 0; i < 25; i++) {
    console.log(wordFreqs[i].word, ' - ', wordFreqs[i].frequency);
  }
}

wordFreqsObj['top25']();
#include "InvertedIndex.h"
#include <algorithm>
#include <sstream>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    freq_dictionary.clear();

    for (size_t docId = 0; docId < docs.size(); ++docId) {
        std::map<std::string, size_t> wordCount;
        std::istringstream stream(docs[docId]);
        std::string word;

        while (stream >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            wordCount[word]++;
        }

        for (const auto& [word, count] : wordCount) {
            freq_dictionary[word].push_back({docId, count});
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    std::string lowerWord = word;
    std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);
    if (freq_dictionary.count(lowerWord)) {
        return freq_dictionary[lowerWord];
    }
    return {};
}
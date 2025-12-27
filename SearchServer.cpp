#include "SearchServer.h"
#include <algorithm>
#include <sstream>
#include <set>

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::istringstream stream(query);
        std::vector<std::string> words;
        std::string word;

        while (stream >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            words.push_back(word);
        }

        std::sort(words.begin(), words.end(), [&](const std::string& a, const std::string& b) {
            return _index.GetWordCount(a).size() < _index.GetWordCount(b).size();
        });

        std::set<size_t> docsSet;
        bool first = true;

        for (const auto& w : words) {
            auto entries = _index.GetWordCount(w);
            std::set<size_t> currentDocs;
            for (const auto& e : entries) {
                currentDocs.insert(e.doc_id);
            }

            if (first) {
                docsSet = currentDocs;
                first = false;
            } else {
                std::set<size_t> intersect;
                std::set_intersection(docsSet.begin(), docsSet.end(),
                                      currentDocs.begin(), currentDocs.end(),
                                      std::inserter(intersect, intersect.begin()));
                docsSet = intersect;
            }

            if (docsSet.empty()) break;
        }

        if (docsSet.empty()) {
            result.push_back({});
            continue;
        }

        std::vector<RelativeIndex> relIndex;
        float maxRank = 0;

        for (size_t docId : docsSet) {
            float rank = 0;
            for (const auto& w : words) {
                auto entries = _index.GetWordCount(w);
                for (const auto& e : entries) {
                    if (e.doc_id == docId) {
                        rank += e.count;
                        break;
                    }
                }
            }
            relIndex.push_back({docId, rank});
            maxRank = std::max(maxRank, rank);
        }

        for (auto& item : relIndex) {
            item.rank /= maxRank;
        }

        std::sort(relIndex.begin(), relIndex.end(),
                  [](const RelativeIndex& a, const RelativeIndex& b) {
                      return a.rank > b.rank;
                  });

        result.push_back(relIndex);
    }

    return result;
}
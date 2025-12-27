#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converter;
        InvertedIndex index;
        SearchServer server(index);

        auto docs = converter.GetTextDocuments();
        index.UpdateDocumentBase(docs);

        auto requests = converter.GetRequests();
        int maxResponses = converter.GetResponsesLimit();

        std::vector<std::vector<std::pair<int, float>>> answers;

        for (const auto& request : requests) {
            std::vector<std::pair<int, float>> singleAnswer;

            auto results = server.search({request});
            if (!results[0].empty()) {
                for (size_t i = 0; i < results[0].size() && i < maxResponses; ++i) {
                    singleAnswer.emplace_back(static_cast<int>(results[0][i].doc_id), results[0][i].rank);
                }
            }
            answers.push_back(singleAnswer);
        }

        converter.putAnswers(answers);
        std::cout << "Search completed. Results saved to answers.json." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
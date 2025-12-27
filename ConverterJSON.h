#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <string>
#include <vector>

class ConverterJSON {
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
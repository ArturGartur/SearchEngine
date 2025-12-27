#include <gtest/gtest.h>
#include "InvertedIndex.h"

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
        {{0, 1}},
        {{0, 1}, {1, 3}}
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    std::vector<std::vector<Entry>> result;
    for (auto& request : requests) {
        result.push_back(idx.GetWordCount(request));
    }
    ASSERT_EQ(result, expected);
}
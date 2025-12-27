#include <gtest/gtest.h>
#include "InvertedIndex.h"
#include "SearchServer.h"

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> requests = {"milk water", "sugar"};
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            {2, 1},
            {0, 0.7f},
            {1, 0.3f}
        },
        {}
    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    auto result = srv.search(requests);
    ASSERT_EQ(result, expected);
}
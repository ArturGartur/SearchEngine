#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream configFile("config.json");
    if (!configFile.is_open()) {
        throw std::runtime_error("config file is missing");
    }

    json config;
    configFile >> config;
    configFile.close();

    if (config.find("config") == config.end()) {
        throw std::runtime_error("config file is empty");
    }

    std::string version = config["config"]["version"];
    if (version != "0.1") {
        throw std::runtime_error("config.json has incorrect file version");
    }

    std::cout << "Starting " << config["config"]["name"] << std::endl;

    std::vector<std::string> documents;
    auto files = config["files"];

    for (const auto& file : files) {
        std::ifstream docFile(file);
        if (!docFile.is_open()) {
            std::cerr << "File not found: " << file << std::endl;
            continue;
        }

        std::stringstream buffer;
        buffer << docFile.rdbuf();
        documents.push_back(buffer.str());
        docFile.close();
    }

    return documents;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream configFile("config.json");
    json config;
    configFile >> config;
    configFile.close();

    if (config.contains("config") && config["config"].contains("max_responses")) {
        return config["config"]["max_responses"].get<int>();
    }
    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream requestsFile("requests.json");
    if (!requestsFile.is_open()) {
        throw std::runtime_error("requests.json file is missing");
    }

    json requestsJson;
    requestsFile >> requestsJson;
    requestsFile.close();

    std::vector<std::string> requests;
    for (const auto& req : requestsJson["requests"]) {
        requests.push_back(req.get<std::string>());
    }

    return requests;
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json result;
    result["answers"] = json::object();

    for (size_t i = 0; i < answers.size(); ++i) {
        std::string requestId = "request" + std::to_string(i + 1);
        if (answers[i].empty()) {
            result["answers"][requestId]["result"] = false;
        } else {
            result["answers"][requestId]["result"] = true;
            result["answers"][requestId]["relevance"] = json::array();
            for (const auto& [docId, rank] : answers[i]) {
                json rel;
                rel["docid"] = docId;
                rel["rank"] = rank;
                result["answers"][requestId]["relevance"].push_back(rel);
            }
        }
    }

    std::ofstream outFile("answers.json");
    outFile << result.dump(2);
    outFile.close();
}
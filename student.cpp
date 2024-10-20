#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>
#include "customSerializer.cpp"


struct Student {
    int id;
    float average_score;
    std::string name;
    std::vector<std::string> lessons;
    std::unordered_map<std::string, std::string> estimation;

    std::unordered_map<std::string, SerializeType> toSerilizeType(){    
        std::unordered_map<std::string, SerializeType> studentData;
        studentData["id"] = this->id;
        studentData["average_score"] = this->average_score;
        studentData["name"] = this->name;
        studentData["lessons"] = this->lessons;
        studentData["estimation"] = std::unordered_map<std::string, std::string>{};

        for (const auto& [subject, grade] : this->estimation) {
            std::unordered_map<std::string, std::string>& estimationMap = std::get<std::unordered_map<std::string, std::string>>(studentData["estimation"]);
            estimationMap[subject] = grade;
        }

        return studentData;
    }

    void fromSerilizeType(std::unordered_map<std::string, SerializeType> studentData){
        this->id = std::get<int>(studentData.at("id"));
        this->name = std::get<std::string>(studentData.at("name"));
        this->average_score = std::get<float>(studentData.at("average_score"));
        this->lessons = std::get<std::vector<std::string>>(studentData.at("lessons"));
        this->estimation = std::get<std::unordered_map<std::string, std::string>>(studentData.at("estimation"));
    }
};
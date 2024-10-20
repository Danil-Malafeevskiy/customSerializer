#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <unordered_map>
#include "student.cpp"


int main() {
    CustomSerializer serializer;

    std::vector<Student> students;

    for (int i = 0; i < 1; i++){
        Student student;
        student.id = i;
        student.name = "Danil";
        student.average_score = 4.66;
        student.lessons = std::vector<std::string>{"maths", "physics", "english"};
        student.estimation = std::unordered_map<std::string, std::string>{{"maths", "4"}, {"physics", "5"}, {"english", "4"}};

        students.push_back(student);
    }

    clock_t start = clock();

    std::string filename = "data.bin";
    serializer.serialize<Student>(filename, students);

    clock_t end = clock();
    double seconds = (double)(end - start);

    std::cout << "Custom serialize: " << seconds << std::endl;

    clock_t start2 = clock();

    std::vector<Student> newStudents = serializer.deserialize<Student>(filename);

    for (const auto& newStudent : newStudents) {
        // std::cout << "id: " << newStudent.id << std::endl;
        // std::cout << "name: " << newStudent.name << std::endl;
        // std::cout << "average_score: " << newStudent.average_score << std::endl;

        // std::cout << "lessons: [";
        // for (int i=0; i<newStudent.lessons.size(); i++) {
        //     std::cout << " " << newStudent.lessons[i];
        // }
        // std::cout << "]" << std::endl;

        // std::cout << "estimates: {";
        // for (auto x : newStudent.estimation) 
        //     std::cout << x.first << ": " << x.second << ", ";
        // std::cout << "}" << std::endl;
    }

    clock_t end2 = clock();
    double seconds2 = (double)(end2 - start2);

    std::cout << "Custom deserialize: " << seconds2 << std::endl;

    return 0;
}

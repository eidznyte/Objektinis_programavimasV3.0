#include "Students.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <chrono>

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;

void displayDuration(const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end,
    const std::string& operationName) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << operationName << " took " << duration << " milliseconds." << std::endl;
}

double calculateAverage(const std::vector<int>& grades) {
    double sum = 0;
    for (int grade : grades) {
        sum += grade;
    }
    return grades.empty() ? 0 : sum / grades.size();
}

double calculateMedian(std::vector<int> grades) {
    size_t size = grades.size();
    if (size == 0) return 0;

    sort(grades.begin(), grades.end());
    size_t mid = size / 2;

    return size % 2 == 0 ? (grades[mid - 1] + grades[mid]) / 2.0 : grades[mid];
}

void readFromFile(std::vector<Student>& students, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file '" << filename << "'\n";
        return;
    }

    std::string headerLine;
    std::getline(file, headerLine);

    Student student;
    while (file >> student) {
        students.push_back(student);
    }

    file.close();
}



void writeToFile(const std::vector<Student>& students, const std::string& filename) {
    std::cout << "Writing " << students.size() << " students to file: " << filename << std::endl;
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw std::runtime_error("Could not open file " + filename + " for writing.");
    }

    outFile << std::left << std::setw(15) << "Name"
        << std::setw(15) << "Surname"
        << std::setw(15) << "Final(Avg)"
        << std::setw(15) << "Final(Med)" << std::endl;
    outFile << std::string(60, '-') << std::endl;

    for (const Student& student : students) {
        outFile << std::left
            << std::setw(15) << student.getName()
            << std::setw(15) << student.getSurname()
            << std::setw(15) << std::fixed << std::setprecision(2) << student.getFinalScoreAvg()
            << std::setw(15) << std::fixed << std::setprecision(2) << student.getFinalScoreMed() << std::endl;
    }

    outFile.close();
}

void inputStudentsManually(std::vector<Student>& students) {
    std::cout << "Enter the number of students: ";
    int numStudents;
    while (!(std::cin >> numStudents) || numStudents <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input! Please enter a positive integer for the number of students: ";
    }
    std::cin.ignore();

    for (int i = 0; i < numStudents; ++i) {
        std::cout << "Enter details for student " << i + 1 << ":\n";

        std::string name, surname;
        std::cout << "Name: ";
        std::cin >> name;
        std::cout << "Surname: ";
        std::cin >> surname;
        Student student(name, surname);

        std::cout << "Enter homework scores (end with a non-numeric input, e.g., 'done'):\n";
        int score;
        std::vector<int> scores;
        while (std::cin >> score) {
            scores.push_back(score);
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter exam score: ";
        std::cin >> score;
        student.setExam(score);
        student.setHomeworks(scores);

        student.setFinalScoreAvg(calculateAverage(student.getHomeworks()));
        student.setFinalScoreMed(calculateMedian(student.getHomeworks()));

        students.push_back(student);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}



void generateRandomScores(Student& student) {
    const int numHomeworks = rand() % 20 + 1;
    std::vector<int> homeworks;
    for (int i = 0; i < numHomeworks; ++i) {
        homeworks.push_back(rand() % 10 + 1);
    }
    student.setHomeworks(homeworks);
    student.setExam(rand() % 10 + 1);

    student.setFinalScoreAvg(calculateAverage(student.getHomeworks()));
    student.setFinalScoreMed(calculateMedian(student.getHomeworks()));
}

void generateFile(const std::string& filename, int numStudents) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return;
    }

    outFile << std::setw(15) << "Vardas"
        << std::setw(15) << "Pavarde"
        << std::setw(25) << "ND scores"
        << std::setw(30) << "Egz." << std::endl;

    for (int i = 1; i <= numStudents; i++) {
        outFile << std::setw(15) << ("Vardas" + std::to_string(i))
            << std::setw(15) << ("Pavarde" + std::to_string(i));

        for (int j = 0; j < 14; j++) {
            outFile << std::setw(3) << (rand() % 10 + 1);
        }

        outFile << std::setw(10) << (rand() % 10 + 1) << std::endl;
    }

    outFile.close();
}

void categorizeStudents(std::vector<Student>& students, std::vector<Student>& dummies) {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Starting categorization. Initial student count: " << students.size() << std::endl;

    for (auto it = students.begin(); it != students.end();) {
        if (it->getFinalScoreAvg() < 5.0) {
            std::cout << "Moving " << it->getName() << " to dummies." << std::endl;
            dummies.push_back(*it);
            it = students.erase(it);
        }
        else {
            ++it;
        }
    }

    std::cout << "Categorization complete. Smart count: " << students.size() << ", Dummies count: " << dummies.size() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    displayDuration(start, end, "Categorizing students with vectors");
}

void displayStudents(const std::vector<Student>& students) {
    for (const Student& student : students) {
        std::cout << student << std::endl;
    }
}


std::istream& operator>>(std::istream& in, Student& student) {
    in >> std::ws;

    in >> student.name >> student.surname;

    student.homeworks.clear();
    int score;
    while (in.peek() != '\n' && in >> score) {
        student.homeworks.push_back(score);
    }

    if (!student.homeworks.empty()) {
        student.exam = student.homeworks.back();
        student.homeworks.pop_back();
    }
    else {
        student.exam = 0;
    }

    student.setFinalScoreAvg(calculateAverage(student.homeworks));
    student.setFinalScoreMed(calculateMedian(student.homeworks));


    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return in;
}



std::ostream& operator<<(std::ostream& out, const Student& student) {
    out << "Name: " << student.getName() << ", Surname: " << student.getSurname()
        << ", Final Avg Score: " << student.getFinalScoreAvg()
        << ", Final Med Score: " << student.getFinalScoreMed();
    return out;
}




void Student::displayInfo() const {
    std::cout << "Name: " << name
        << ", Surname: " << surname
        << ", Exam Score: " << exam
        << ", Final Average Score: " << finalScoreAvg
        << ", Final Median Score: " << finalScoreMed
        << std::endl;

    std::cout << "Homework Scores: ";
    for (const auto& score : homeworks) {
        std::cout << score << " ";
    }
    std::cout << std::endl;
}


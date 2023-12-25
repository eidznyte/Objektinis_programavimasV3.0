#ifndef STUDENTS_H
#define STUDENTS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

class Person {
protected:
    std::string name;
    std::string surname;

public:
    Person() = default;
    Person(const std::string& name, const std::string& surname)
        : name(name), surname(surname) {}
    virtual ~Person() = default;

    const std::string& getName() const { return name; }
    void setName(const std::string& newName) { name = newName; }

    const std::string& getSurname() const { return surname; }
    void setSurname(const std::string& newSurname) { surname = newSurname; }

    virtual void displayInfo() const = 0;
};

class Student : public Person {
private:
    std::vector<int> homeworks;
    int exam = 0;
    double finalScoreAvg = 0;
    double finalScoreMed = 0;

public:
    Student() = default;
    Student(const std::string& name, const std::string& surname)
        : Person(name, surname) {}
    Student(const Student& other) = default;
    Student& operator=(const Student& other) = default;
    virtual ~Student() = default;

    void displayInfo() const override;

    const std::vector<int>& getHomeworks() const { return homeworks; }
    void setHomeworks(const std::vector<int>& newHomeworks) { homeworks = newHomeworks; }
    void addHomework(int score) { homeworks.push_back(score); }

    int getExam() const { return exam; }
    void setExam(int newExam) { exam = newExam; }

    double getFinalScoreAvg() const { return finalScoreAvg; }
    void setFinalScoreAvg(double newFinalScoreAvg) { finalScoreAvg = newFinalScoreAvg; }

    double getFinalScoreMed() const { return finalScoreMed; }
    void setFinalScoreMed(double newFinalScoreMed) { finalScoreMed = newFinalScoreMed; }

    friend std::istream& operator>>(std::istream& in, Student& student);
    friend std::ostream& operator<<(std::ostream& out, const Student& student);
};


std::istream& operator>>(std::istream& in, Student& student);
std::ostream& operator<<(std::ostream& out, const Student& student);

void displayDuration(const std::chrono::high_resolution_clock::time_point& start,
    const std::chrono::high_resolution_clock::time_point& end,
    const std::string& operationName);
double calculateAverage(const std::vector<int>& grades);
double calculateMedian(std::vector<int> grades);
void readFromFile(std::vector<Student>& students, const std::string& filename);
void writeToFile(const std::vector<Student>& students, const std::string& filename);
void displayStudents(const std::vector<Student>& students);
void inputStudentsManually(std::vector<Student>& students);
void generateRandomScores(Student& student);
void generateFile(const std::string& filename, int numStudents);
void categorizeStudents(std::vector<Student>& students, std::vector<Student>& dummies);

#endif // STUDENTS_H
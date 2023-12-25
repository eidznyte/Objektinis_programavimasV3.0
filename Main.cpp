#include "Students.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <stdexcept>

int main() {
    std::vector<Student> studentsVector, dummiesVector;

    int choice;
    while (true) {
        std::cout << "Would you like to: \n"
            << "1. Input data manually \n"
            << "2. Read data from a file \n"
            << "3. Generate a random student list file\n"
            << "Choose an option (1/2/3): ";
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 3) {
            std::cerr << "Invalid choice, please try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    std::string filename;
    try {
        switch (choice) {
        case 1:
            inputStudentsManually(studentsVector);
            break;
        case 2:
            std::cout << "Enter the filename to read from (including extension, e.g., 'data.txt'): ";
            std::cin >> filename;
            readFromFile(studentsVector, filename);
            break;
        case 3:
            std::cout << "How many records would you like to generate? \n"
                << "1. 1,000 \n"
                << "2. 10,000 \n"
                << "3. 100,000 \n"
                << "4. 1,000,000 \n"
                << "5. 10,000,000 \n"
                << "Choose an option (1/2/3/4/5): ";
            int subChoice;
            std::cin >> subChoice;
            size_t records;
            switch (subChoice) {
            case 1: records = 1'000; break;
            case 2: records = 10'000; break;
            case 3: records = 100'000; break;
            case 4: records = 1'000'000; break;
            case 5: records = 10'000'000; break;
            default:
                throw std::invalid_argument("Invalid option!");
            }
            filename = "random_students_" + std::to_string(records) + ".txt";
            generateFile(filename, records);
            readFromFile(studentsVector, filename);
            break;
        default:
            throw std::invalid_argument("Invalid choice!");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    try {
        auto start = std::chrono::high_resolution_clock::now();
        categorizeStudents(studentsVector, dummiesVector);
        writeToFile(dummiesVector, "dummies_vector.txt");
        writeToFile(studentsVector, "smart_vector.txt");
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::cout << "\nTime taken for file operations: " << duration.count() << " seconds." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during file operations: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

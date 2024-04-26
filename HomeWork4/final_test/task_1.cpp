#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <thread>


struct Student {
    int id;
    std::string name;
    int age;
};


class StudentDatabase {
private:
    std::unordered_map<int, std::shared_ptr<Student>> students;
    std::mutex mtx;

public:
    void AddStudent(int id, const std::string& name, int age) {
        std::lock_guard<std::mutex> lock(mtx);
        students[id] = std::make_shared<Student>(Student{id, name, age});
    }

    bool RemoveStudent(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        return students.erase(id) > 0;
    }

    std::shared_ptr<Student> GetStudent(int id) {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = students.find(id);
        if (it != students.end()) {
            return it->second;
        }
        return nullptr;
    }
};

void AddStudents(StudentDatabase& db) {
    for (int i = 0; i < 5; ++i) {
        db.AddStudent(i, "Student " + std::to_string(i), 18 + i);
    }
}

void PrintStudentInfo(StudentDatabase& db, int id) {
    auto student = db.GetStudent(id);
    if (student) {
        std::cout << "Student ID: " << student->id << ", Name: " << student->name << ", Age: " << student->age << std::endl;
    } else {
        std::cout << "Student with ID " << id << " not found." << std::endl;
    }
}

int main() {
    StudentDatabase db;

    // Add users to DB
    std::thread writer_thread(AddStudents, std::ref(db));


    // Read users from DB
    for (int i = 0; i < 5; ++i) {
        std::thread reader_thread(PrintStudentInfo, std::ref(db), i);
        reader_thread.join();
    }


    writer_thread.join();


    return 0;
}


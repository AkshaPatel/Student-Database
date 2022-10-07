#include "StudentDatabase.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

Student::Student() {

}

Student::Student(unsigned int IDnum, std::string& Name, unsigned short Age) {
    student_ID = IDnum;
    name = Name;
    age = Age;
}

unsigned int Student::get_ID() const {
    return student_ID;
}

std::string Student::get_name() const {
    return name;
}

unsigned short Student::get_age() const {
    return age;
}

unsigned int Student::convert_to_ui(const std::string& s) {
    unsigned int num = (unsigned int)std::stoul(s);
    return num;
}

unsigned short Student::convert_to_us(const std::string& s) {
    unsigned short num = (unsigned short)std::stoul(s);
    return num;
}

void Student::set_name(const std::string& s) {
    name = s;
}

Student::~Student() {

}


MyParser::MyParser() {

}

std::vector<std::string> MyParser::operator()(std::string& input) {
    std::vector<std::string> parsed_command;

    std::string token = "";
    int num_of_quotes = 0;

    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '"')
            num_of_quotes++;
    }

    int i = 0;

    if (num_of_quotes == 2 || num_of_quotes == 0) {
        while (i < input.length()) {
            if (input[i] == delimiters[0] || input[i] == delimiters[1]) {
                if (token == "") {
                }
                else {
                    parsed_command.push_back(token);
                    token = "";
                }
            }
            else if (input[i] == '<' || input[i] == '>' || input[i] == '=') {
                if (token != "") {
                    parsed_command.push_back(token);
                }

                token = input[i];
                parsed_command.push_back(token);
                token = "";
            }
            else if (input[i] == '"') {
                if (token != "") {
                    parsed_command.push_back(token);
                    token = "";
                }
                i++;

                while (input[i] != '"') {
                    token += input[i];
                    i++;
                }

                parsed_command.push_back(token);
                token = "";
            }
            else {
                token += input[i];
            }

            if (i == (input.length() - 1) && token != "")
                parsed_command.push_back(token);

            i++;
        }
    }
    else {
        parsed_command.push_back("BAD");        
    }
  
    check_input(parsed_command);

    
    return parsed_command;
}

void MyParser::check_input(std::vector<std::string>& parsed) {
    
    //Max size of a parsed input is 4
    if (parsed.size() > 4) {
        parsed.clear();
        parsed.push_back("BAD");        
    }
    //Tests for non-digit characters in numerical expressions
    else if (parsed.size() == 4) {
        if (parsed[1] == "age" || parsed[1] == "id") {
            std::string checker = parsed[3];
            //bool test = 0;
            for (int i = 0; i < checker.length(); i++) {
                if (!isdigit(checker[i])) {
                    parsed.clear();
                    parsed.push_back("BAD");
                    break;
                }
            }        
        }
        else if (parsed[0] == "ADD") {
            std::string checker1 = parsed[1];
            std::string checker2 = parsed[3];
                      
            for (int i = 0; i < checker1.length(); i++) {
                if (!isdigit(checker1[i])) {
                    parsed.clear();
                    parsed.push_back("BAD");
                    break;
                }
            }
            for (int i = 0; i < checker2.length(); i++) {
                if (!isdigit(checker2[i])) {
                    parsed.clear();
                    parsed.push_back("BAD");
                    break;
                }
            }
        }
    }
    //If the size of parsed is 1 the input can only be STOP
    else if (parsed.size() == 1 || parsed.size() == 0) {
        if (parsed.size() == 0) {
            parsed.push_back("BAD");
        }
        else if (parsed[0] != "STOP" && parsed[0] != "BAD") {
            parsed.clear();
            parsed.push_back("BAD");
        }
    }

}

MyParser::~MyParser() {

}

HardDiskManager::HardDiskManager() {

}

std::string HardDiskManager::get_filename() const {
    return file;
}

std::vector<Student*> HardDiskManager::take_in_file(std::string& filename) {
    std::fstream infile;
    infile.open(filename, std::ios::in);

    std::vector<Student*> read_file;

    if (infile.is_open()) {
        std::string s;
        while (getline(infile, s)) {
            std::vector<std::string> parsed_input;

            MyParser parser;
            parsed_input = parser(s);

            class Student* person = nullptr;

            unsigned int IDnum = person->convert_to_ui(parsed_input[0]);
            std::string Name = parsed_input[1];
            unsigned short Age = person->convert_to_us(parsed_input[2]);

            person = new class Student(IDnum, Name, Age);

            read_file.push_back(person);

        }
    }

    infile.close();
    return read_file;
}

void HardDiskManager::output_file(std::string& filename, std::vector<Student*>& students) {
    std::fstream outfile;
    outfile.open(filename, std::ios::out);

    for (int i = 0; i < students.size(); i++) {
        if ((students[i]->get_name()).std::string::find(' ') != std::string::npos || students[i]->get_name().std::string::find('\t') != std::string::npos) {
            std::string temp = students[i]->get_name();
            temp.push_back('"');
            std::string quote;
            quote.push_back('"');
            temp.insert(0, quote);
            students[i]->set_name(temp);
        }
        outfile << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
    }

    outfile.close();
}

HardDiskManager::~HardDiskManager() {

}



MyManager::MyManager() {
    std::string temp = File_manager.get_filename();
    students = File_manager.take_in_file(temp);
}

bool MyManager::process(std::vector<std::string>& parsed_userinput) {
    if (parsed_userinput[0] == "BAD") {
        std::cout << "bad command!!!" << '\n';
        return false;
    }
    else if (parsed_userinput[0] == "ADD") {
        return ADD(parsed_userinput);
    }
    else if (parsed_userinput[0] == "FIND") {
        return FIND(parsed_userinput);
    }
    else if (parsed_userinput[0] == "REMOVE") {
        return REMOVE(parsed_userinput);
    }
    else if (parsed_userinput[0] == "STOP") {
        return STOP();
    }

    std::cout << "bad command!!!" << '\n';
    return false;
}

bool MyManager::ADD(std::vector<std::string>& parsed_userinput) {
    class Student* person = nullptr;
    
    unsigned int IDnum = person->convert_to_ui(parsed_userinput[1]);
    unsigned short Age = person->convert_to_us(parsed_userinput[3]);

    person = new class Student(IDnum, parsed_userinput[2], Age);
    students.push_back(person);

    return false;
}

bool MyManager::FIND(std::vector<std::string>& parsed_userinput) {
    if (parsed_userinput[1] == "id") {
        if (parsed_userinput[2] == "<") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() < students[i]->convert_to_ui(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
        else if (parsed_userinput[2] == ">") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() > students[i]->convert_to_ui(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
        else if (parsed_userinput[2] == "=") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() == students[i]->convert_to_ui(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
    }
    else if (parsed_userinput[1] == "name") {
        if (parsed_userinput[2] == "=") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_name() == parsed_userinput[3]) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
    }
    else if (parsed_userinput[1] == "age") {
        if (parsed_userinput[2] == "<") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() < students[i]->convert_to_us(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
        else if (parsed_userinput[2] == ">") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() > students[i]->convert_to_us(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
        else if (parsed_userinput[2] == "=") {
            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() == students[i]->convert_to_us(parsed_userinput[3])) {
                    std::cout << students[i]->get_ID() << " " << students[i]->get_name() << " " << students[i]->get_age() << '\n';
                }
            }
        }
    }
    return false;
}

bool MyManager::REMOVE(std::vector<std::string>& parsed_userinput) {
    if (parsed_userinput[1] == "id") {
        if (parsed_userinput[2] == "<") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() < students[i]->convert_to_ui(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
        else if (parsed_userinput[2] == ">") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() > students[i]->convert_to_ui(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
        else if (parsed_userinput[2] == "=") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_ID() == students[i]->convert_to_ui(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
    }
    else if (parsed_userinput[1] == "name") {
        if (parsed_userinput[2] == "=") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_name() == parsed_userinput[3]) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
    }
    else if (parsed_userinput[1] == "age") {
        if (parsed_userinput[2] == "<") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() < students[i]->convert_to_us(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
        else if (parsed_userinput[2] == ">") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() > students[i]->convert_to_us(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
        else if (parsed_userinput[2] == "=") {
            std::vector<int> del_students;

            for (int i = 0; i < students.size(); i++) {
                if (students[i]->get_age() == students[i]->convert_to_us(parsed_userinput[3])) {
                    del_students.insert(del_students.begin(), i);
                }
            }
            if (del_students.size() != 0) {
                int j = 0;
                for (int i = (int)(students.size() - 1); i >= 0; i--) {
                    if (i == del_students[j]) {
                        students.erase(students.begin() + i);
                        j++;
                    }

                }
            }
        }
    }
    return false;
}

bool MyManager::STOP() {
    std::string temp = File_manager.get_filename();
    File_manager.output_file(temp, students);
    return true;  
}

MyManager::~MyManager() {
    
    for (unsigned int i = 0; i < students.size(); i++) {
        delete students[i];
        students[i] = nullptr;
    }
}



int main()
{
    MyManager engine;

    MyParser parser;
    std::string  userInput;
    bool  shouldStop{ false };
    while (!shouldStop)
    {
        std::getline(std::cin, userInput); // pay attention, pure cin doesn't allow spaces or tabs in userInput
        std::vector<std::string> temp = parser(userInput);
        shouldStop = engine.process(temp);
    }

    return 0;
}
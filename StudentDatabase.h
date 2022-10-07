#ifndef _STUDENTDATABASE_H
#define _STUDENTDATABASE_H

#include <string>
#include <vector>

class Student {
public:
	Student();
	Student(unsigned int IDnum, std::string& Name, unsigned short Age);
	~Student();

	unsigned int get_ID() const;
	std::string get_name() const;
	unsigned short get_age() const;

	void set_name(const std::string& s);

	unsigned int convert_to_ui(const std::string& s);
	unsigned short convert_to_us(const std::string& s);

private:
	unsigned int student_ID;
	std::string name;
	unsigned short age;

	//void check_input(std::string)
};

class MyParser {
public:
	MyParser();
	~MyParser();

	std::vector<std::string> operator()(std::string& input);
	void check_input(std::vector<std::string>& parsed);

private:
	std::vector<char> delimiters{ ' ', '\t' };
};

class HardDiskManager {
public:
	HardDiskManager();
	~HardDiskManager();

	std::string get_filename() const;

	std::vector<Student*> take_in_file(std::string& filename);
	void output_file(std::string& filename, std::vector<Student*>& students);

private:
	std::string   file = "StudentDatabase.txt";
};


class MyManager {
public:
	MyManager();
	~MyManager();
	
	bool process(std::vector<std::string>& parsed_userinput);
	bool ADD(std::vector<std::string>& parsed_userinput);
	bool FIND(std::vector<std::string>& parsed_userinput);
	bool REMOVE(std::vector<std::string>& parsed_userinput);
	bool STOP();

private:
	std::vector<Student*> students;
	HardDiskManager File_manager;
};




#endif
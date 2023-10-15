#ifndef PARSING_HPP
# define PARSING_HPP

#include <fstream>

class Parsing {
private:
	std::string config_file;
	int status;
protected:
public:
	Parsing();
	Parsing(char *argv);
	int config_file_exist(std::string& config_file);
	int config_file_exist();
	int get_status() const { return (this->status); };
	~Parsing();
};

#endif
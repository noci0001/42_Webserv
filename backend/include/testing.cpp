#include <iostream>
#include <fstream>

char *return_content(std::string path){
	std::string prefix = "../../root/";
	path = prefix + path;
	std::cout << path << std::endl;
	char *buffer = new char[3000];
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Error opening file!" << path << std::endl;
		return NULL;
	}
	if (!file.read(buffer, 3000)) {
		std::cerr << "Error hadnling file!" << path << std::endl;
		delete[] buffer;
		return (NULL);
	}
	file.close();
	return (buffer);
}

int main(void) {
	
	char *response = return_content("index.html");
	std::cout << response << std::endl; 
	return (0);
}

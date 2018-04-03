#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


struct vertex {
    std::vector<double>location;
    std::vector<double>color;
    double average;
};

template <typename U>
void readToOne(U &allMok,std::string filename) {
    std::string str{};
    std::vector<std::string> tokens;
    std::ifstream f1(filename);
    while(std::getline(f1, str)) { //read file until the end line by line
        std::istringstream ss (str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        allMok.push_back({tokens[0],tokens[1],{std::stod(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]),std::stoi(tokens[5]),std::stoi(tokens[6])}});
    }
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
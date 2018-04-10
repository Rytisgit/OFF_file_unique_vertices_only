#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


struct vertex {
    std::vector<double>location;
    std::vector<double>color;
    double average;
};
struct comments {
    std::string text;
    int line;
};
bool int_check(const std::string s){
    return s.find_first_not_of( "0123456789" ) == std::string::npos;
}
int nvert{},nface{},nedge{};
template <typename U,typename T>
void readToOne(U &allMok,T &others,std::string filename) {
    int linecount{0};
    std::string str{};
    std::vector<std::string> tokens;
    std::ifstream f1(filename);
    while(std::getline(f1, str)) { //read file until the end line by line

        std::istringstream ss (str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        if(tokens[0]=="#"||tokens[0]=="OFF"){
            others.push_back(str,linecount);
            linecount++;
            continue;
        }
        if (int_check(tokens[0])&&int_check(tokens[1])&&int_check(tokens[2])) {
            nvert=std::stoi( tokens[0]);
            nface=std::stoi( tokens[1]);
            nedge=std::stoi( tokens[2]);
            linecount++;
            others.push_back(str,linecount);
            for (int i = 0; i <nvert ; ++i) {

            }
            break;
        }
        allMok.push_back({tokens[0],tokens[1],{std::stod(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]),std::stoi(tokens[5]),std::stoi(tokens[6])}});

    }
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
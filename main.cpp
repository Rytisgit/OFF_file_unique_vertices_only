#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>
#include <map>
#include <algorithm>
#include <iomanip>

struct vertex {
    std::vector<float> numbers;
    double average{};
    int count{};
    int replacer{};
};
struct comments {
    std::string text;
    int line;
};
struct faces {
    std::vector<float> numbers;
};

bool int_check(const std::string &s) {
    return s.find_first_not_of("0123456789") == std::string::npos;
}

int nvert{}, nface{}, nedge{};
int linecount{0};

template<typename U, typename T>
std::ifstream &readTillData(U &allMok, T &others, std::ifstream &f1) {
    std::string str{};
    std::vector<std::string> tokens;
    while (std::getline(f1, str)) { //read file line by line
        std::string a = str;
        std::istringstream ss(str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        if (tokens[0] == "#" || tokens[0] == "OFF") { //save comments and the OFF line
            others.push_back({a, linecount});
            linecount++;
        } else if (int_check(tokens[0]) && int_check(tokens[1]) &&
                   int_check(tokens[2])) { //find the vertex and face counts and save them
            nvert = std::stoi(tokens[0]);
            nface = std::stoi(tokens[1]);
            nedge = std::stoi(tokens[2]);
            linecount++;
            others.push_back({a, linecount});
            break;
        }
    }
    return f1; //return the ifstream position where the vertices data begins
}

template<typename U, typename T, typename map>
std::ifstream &readTheVertices(U &allMok, T &others, map &replace, std::ifstream &f1) {
    vertex temp;
    std::string str{};
    std::vector<std::string> tokens;
    std::vector<float> numbers;
    double average{};
    std::string a{};
    int security{0};
    for (int i = 0; i < nvert; ++i) {
        std::getline(f1, str); //read file line by line
        a = str;
        std::istringstream ss(str);
        tokens.clear();
        numbers.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        for (int j = 0; j < 3; ++j) {
            numbers.push_back(std::stof(tokens[j]));
        }
        linecount++;
        average = std::accumulate(numbers.begin(), numbers.end(), 0.0) / 3;
        temp.average = average;
        temp.numbers = {numbers};
        temp.count = security++ - replace.size();
        temp.replacer = i;
        if (allMok.empty()) {
            allMok.push_back(temp);
        } else {
            auto it = std::next(allMok.begin(), allMok.size() / 2);
            bool repeated = {
                    numbers[0] == it->numbers[0] && numbers[1] == it->numbers[1] && numbers[2] == it->numbers[2]};
            while (it->average >= average && it != allMok.begin() &&
                   !repeated) { //find equal averages and compare the coordiantes by going down from the middle
                it--;
                if (it->average == average)
                    repeated = {numbers[0] == it->numbers[0] && numbers[1] == it->numbers[1] &&
                                numbers[2] == it->numbers[2]};
            }
            if (!repeated) {//if not found in previous step  find equal averages and compare the coordiantes by going up from the middle
                it = std::next(allMok.begin(), allMok.size() / 2);
                while (it->average <= average && it != allMok.end() && !repeated) {
                    it++;
                    if (it->average == average)
                        repeated = {numbers[0] == it->numbers[0] && numbers[1] == it->numbers[1] &&
                                    numbers[2] == it->numbers[2]};
                }
            }
            if (repeated) { //if the comparison finds a repeat, insert into a map the current position of the coordinates and which position to replace it as
                replace.insert(std::pair<int, int>(i, it->count));
            } else { // otherwise it is unique so insert in a place sorted by average
                it = allMok.begin();
                while (it->average <= average && it + 1 != allMok.end()) {
                    it++;
                }
                allMok.insert(it, temp);
            }
        }
    }
    return f1;//return ifstream with location pointed to after the end of the vertices data
}

bool compareInterval(vertex i1, vertex i2) {
    return (i1.count < i2.count);
}

template<typename U, typename T, typename map>
void readFaces(U &end, const T &vertexes, const map &replace, std::ifstream &f1) {
    std::string str{};
    std::vector<std::string> tokens;
    std::vector<float> numbers;
    bool repeat{};
    while (std::getline(f1, str)) {//read until the end of the file line by line
        std::string a = str;
        std::istringstream ss(str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        numbers.clear();
        int temph{};
        for (int j = 1; j <= std::stoi(tokens[0]); ++j) {
            temph = std::stoi(tokens.at(j));
            repeat = false;
            if (replace.find(temph) == replace.end()) {//check if vertex index is unique

                for (vertex b: vertexes) {
                    if (b.replacer == temph) { //find new index to replace if it is unique
                        temph = b.count;
                        break;
                    }
                }
                for (float num : numbers) {//check if index which is to be inserted doesnt repeat in the same face
                    if (num == temph) { repeat = true; }
                }
                if (!repeat) { //if all good insert as a new vertex for the face
                    numbers.push_back(temph);
                }
            } else { //not unique - find with which index to replace it with
                int check = replace.at(temph);
                for (int num : numbers) { //check if index which is to be inserted doesnt repeat in the same face
                    if (num == check) { repeat = true; }
                }
                if (!repeat) { // if doesnt repeat insert the new index
                    numbers.push_back(check);
                }
            }
        }
        numbers.insert(numbers.begin(), numbers.size());//add vertex count to the start of the vector
        for (size_t i = tokens.size() - std::stoi(tokens[0]) + 1; i < tokens.size(); ++i) {//add colour information
            numbers.push_back(std::stof(tokens[i]));
        }
        faces temp = {numbers};
        end.push_back(temp);
    }
}

template<typename cmp>
void
generateNewFile(const std::vector<vertex> &replace, const std::vector<comments> &others, const std::vector<faces> &end,
                cmp &compare,
                const std::string &output) {
    std::sort(replace.begin(), replace.end(), compare);
    std::ofstream f2(output);
    for (int i = 0; i < others.size() - 1; i++) {//print OFF and comments at the top
        f2 << others[i].text << "\n";
    }
    int facesh{0};
    for (faces arr:end) { //count legitimate faces(more than 2 vertices)
        if (arr.numbers[0] > 2) {
            facesh++;
        }
    }
    f2 << replace.size() << " " << facesh << " " << nedge << std::endl;//print nvert,nfaces,nedges
    for (vertex arr:replace) { // print vertices
        f2 << std::fixed << std::showpoint;
        f2 << std::setprecision(6);
        for (float a : arr.numbers) {

            f2 << a << " ";
        }
        f2 << "\n";
    }
    for (faces arr:end) {//print faces which are atleast a triangle
        f2 << std::fixed << std::noshowpoint;
        f2 << std::setprecision(0);
        if (arr.numbers[0] > 2) {
            for (float a : arr.numbers) {

                f2 << a << " ";
            }
            f2 << "\n";
        }

    }
}

int main() {
    std::ifstream f1("pasalinti_dubliuotas1.off");
    std::string outputname = "fix.off";
    std::vector<vertex> v;
    std::vector<comments> c;
    std::map<int, int> replace;
    std::vector<faces> end;
    readFaces(end, v, replace, readTheVertices(v, c, replace, readTillData(v, c, f1)));
    std::cout << "old vertices = " << nvert << " new vertices = " << v.size() << std::endl;
    generateNewFile(v, c, end, compareInterval, outputname);
    return 0;
}
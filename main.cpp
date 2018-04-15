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
    double average;
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
bool int_check(const std::string s) {
    return s.find_first_not_of("0123456789") == std::string::npos;
}

int nvert{}, nface{}, nedge{};
int linecount{0};

template<typename U, typename T>
std::ifstream &readTillData(U &allMok, T &others, std::ifstream &f1) {

    std::string str{};
    std::vector<std::string> tokens;
    //  std::ifstream f1(filename);
    while (std::getline(f1, str)) { //read file until the end line by line
        std::string a = str;
        std::istringstream ss(str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        if (tokens[0] == "#" || tokens[0] == "OFF") {
            others.push_back({a, linecount});
            linecount++;
        } else if (int_check(tokens[0]) && int_check(tokens[1]) && int_check(tokens[2])) {
            nvert = std::stoi(tokens[0]);
            nface = std::stoi(tokens[1]);
            nedge = std::stoi(tokens[2]);
            linecount++;
            others.push_back({a, linecount});
            break;
        }
    }    //allMok.push_back({tokens[0],tokens[1],{std::stod(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]),std::stoi(tokens[5]),std::stoi(tokens[6])}});
    return f1;

}

template<typename U, typename T,typename map>
std::ifstream &readTherest(U &allMok, T &others,map &replace, std::ifstream &f1) {
    vertex temp;
    std::string str{};
    std::vector<std::string> tokens;
    std::vector<float> numbers;
    double average{};
    std::string a{};
    int security{0};
//  std::ifstream f1(filename);
    for(int i = 0;i<nvert;++i){
        std::getline(f1, str); //read file until the end line by line
        a = str;
        std::istringstream ss(str);
        tokens.clear();
        numbers.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        for (int j = 0; j <3 ; ++j) {
            numbers.push_back(std::stof(tokens[j]));
        }
        linecount++;
        average =std::accumulate(numbers.begin(),numbers.end(),0.0)/3;

        temp.average = average;
        temp.numbers={numbers};
        temp.count = security++-replace.size();
        temp.replacer=i;
        if (allMok.empty()){
            allMok.push_back(temp);
        }
        else {

            auto it = std::next(allMok.begin(), allMok.size()/2);
            bool repeated = {numbers[0]==it->numbers[0]&&numbers[1]==it->numbers[1]&&numbers[2]==it->numbers[2]};
            while(it->average>=average&&it!=allMok.begin()&&!repeated){
                it--;
                if(it->average==average)repeated = {numbers[0]==it->numbers[0]&&numbers[1]==it->numbers[1]&&numbers[2]==it->numbers[2]};
            }
            if(!repeated){
                it = std::next(allMok.begin(), allMok.size()/2);
                while(it->average<=average&&it!=allMok.end()&&!repeated){
                    it++;
                    if(it->average==average)repeated = {numbers[0]==it->numbers[0]&&numbers[1]==it->numbers[1]&&numbers[2]==it->numbers[2]};
                }
            }
            if(repeated){
                replace.insert(std::pair<int,int>(i,it->count));
                //security++;
            }
            else {
                it = allMok.begin();
                while(it->average<=average&&it+1!=allMok.end()){
                    it++;
               }
//if(it==allMok.end())it--;
               allMok.insert(it,temp);
              //  security++;
            }
        }
       }
    return f1;
}
template<typename U>
bool my_cmp(const U& a, const U& b)
{
    // smallest comes first
    return a->count < b->count;
}
bool compareInterval(vertex i1, vertex i2)
{
    return (i1.count < i2.count);
}
template<typename U, typename T,typename map>
void keepReading(U &end, T &vertexes,map &replace, std::ifstream &f1) {

    std::string str{};
    std::vector<std::string> tokens;
    std::vector<float> numbers;
    std::vector<float> norepeats;
    bool repeat = false;
    //  std::ifstream f1(filename);
    while (std::getline(f1, str)) { //read file until the end line by line
        std::string a = str;
        std::istringstream ss(str);
        tokens.clear();
        while (ss >> str) {
            tokens.push_back(str);
        }
        numbers.clear();
        norepeats.clear();
        int temph{};
        for (int j = 1; j <=std::stoi (tokens[0]) ; ++j) {
            temph = std::stoi(tokens.at(j));
            repeat = false;
            if(replace.find(temph)==replace.end()){

                for(vertex b: vertexes){
                    if(b.replacer==temph){temph=b.count;break;}

                }
                    for (float num : numbers) {
                        if(num==temph){repeat = true;}
                    }
                    if(!repeat){
                    //std::cout<<tokens.at(j)<<" replaced with "<<temph<<std::endl;
                numbers.push_back(temph);norepeats.push_back(temph);}



            }
            else{
//                for (int num : norepeats) {
//                    if(num==temp){repeat = true; break;}
//                }
                int check = replace.at(temph);
//                for(vertex a: vertexes){
//                    if(a.replacer==check){
//                        temph=a.count;
//
//                    if(temph>350){
//                        temph = 1000;
//                    }
//
//                    break;
//                    }
//                }
                  for (int num : numbers) {
                        if(num==check){repeat = true; }
                    }
                    if(!repeat){
                      numbers.push_back(check);
                    norepeats.push_back(check);}

//                numbers.push_back(replace.at(temp));
//                norepeats.push_back(replace.at(temp));
            }


//repeat=false;
        }
        numbers.insert(numbers.begin(),numbers.size());
        for (size_t i = tokens.size()-std::stoi (tokens[0])+1; i < tokens.size(); ++i) {
            numbers.push_back(std::stof(tokens[i]));
        }

        linecount++;
        faces temp = {numbers};
        end.push_back(temp);

    }    //allMok.push_back({tokens[0],tokens[1],{std::stod(tokens[2]),std::stoi(tokens[3]),std::stoi(tokens[4]),std::stoi(tokens[5]),std::stoi(tokens[6])}});

}
template <typename cmp>
void generateNewFile(std::vector<vertex> &replace,  std::vector<comments> &others,std::vector<faces>  &end, cmp &compare,std::string output) {
    std::sort(replace.begin(),replace.end(),compare);
    std::ofstream f2(output);
    for(int i = 0;i<others.size()-1;i++){
        f2<<others[i].text<<"\n";
    }
     int facesh{0};
    for(faces arr:end){
        if(arr.numbers[0]>2){
            facesh++;
    }}
    f2<<replace.size()<<" "<<facesh<<" "<<nedge<<std::endl;
    for(vertex arr:replace){
        f2 << std::fixed << std::showpoint;
        f2<< std::setprecision(6);
        for (float a : arr.numbers) {

            f2<<a<<" ";
        }
        f2<<"\n";
    }
    for(faces arr:end){
        f2 << std::fixed << std::noshowpoint;
        f2<< std::setprecision(0);
        if(arr.numbers[0]>2){
            for (float a : arr.numbers) {

                f2<<a<<" ";
            }
            f2<<"\n";}

    }
}
int main() {
    std::ifstream f1("pasalinti_dubliuotas1.off");
    std::string outputname = "fix.off";
    std::vector<vertex> v;
    std::vector<comments> c;
    std::map<int,int> replace;
    std::vector<faces> end;
    keepReading(end,v,replace, readTherest(v, c,replace,readTillData(v, c, f1)));
    std::cout << "old vertices = " <<nvert<<" new vertices = "<< v.size() << std::endl;
    generateNewFile(v,c,end,compareInterval,outputname);

    return 0;
}
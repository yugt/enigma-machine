#include<iostream>
#include<iterator>
#include<sstream>
#include<string>
#include<vector>

class Parser
{
    std::vector<std::string> input;

    bool wheel_comb(){
        char c = input.at(1).c_str()[0];
        comb = c - '0';
        return (comb>=0 && comb<6);
    }

public:
    std::string key;
    std::string msg;
    char mode;
    int comb;

    Parser(std::string& line) {
        std::istringstream iss(line);
        std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(input));
    }

    bool parse(){
        if(input.size()<2 || input.size()>4) return false;
        if(input.at(0).length()!=1) return false;
        mode = input.at(0).c_str()[0];
        msg = input.back();
        key = input.at(input.size()-2);
        switch (mode)
        {
        case 'a': // no hint, hardest
            key.clear();
            return input.size()==2;
        case 'b': // wheel pos given, hard
            return input.size()==3 
            && input.at(1).length()==1
            && wheel_comb();
        case 'c': // key given, easy
            return input.size()==3
            && key.size()==3;
        case 'd':
        case 'e':
            return input.size()==4
            && input.at(1).length()==1
            && wheel_comb()
            && key.size()==3;
        default:
            return false;
        }
    }


};
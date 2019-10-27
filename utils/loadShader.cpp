#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "loadShader.hpp"

string LoadShader(string file_path){
    std::string ShaderSource;
    std::ifstream ShaderStream(file_path.c_str(), std::ios::in);
    if(ShaderStream.is_open()){
        std::stringstream sstr;
        sstr << ShaderStream.rdbuf();
        ShaderSource = sstr.str();
        ShaderStream.close();
    } else {
        cout <<"File I/O Error. " << file_path << " Not Found";
        throw "File I/O Error. ";
    }
    return ShaderSource;
}
//
// Created by chorm on 2020-06-01.
//

#include "ModuleFormat.hpp"
#include <FrameworkConfig.h>

#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std::string_literals;

const static std::string output{"out.mia"};

int main(int argc,char** argv){
    if(argc<1)
        throw std::runtime_error("Usage: modc [-o <output-file>] <input-file>");
    std::vector<std::string> args(argv,argv+argc);
    std::reference_wrapper<const std::string> output_name{output};
    const std::string& input{args.back()};
    if(auto _output = std::find(begin(args), end(args), "-o"s);_output != end(args)){
        _output++;
        if(_output == end(args))
            throw std::runtime_error("Argument Parsing Error, found -o as the last argument");
        output_name = std::ref(*_output);
    }else{

    }

}
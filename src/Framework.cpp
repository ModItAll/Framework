//
// Created by chorm on 2020-05-28.
//

#include <Framework.h>
#include <FrameworkConfig.h>
#include <iostream>

int main(int argc,char** argv){
    std::cout << "Project MIA v" MIA_VERSION << std::endl;
    std::cout << "Program name: " << argv[0] << std::endl;
    std::cout << "Number of parameters: " << argc << std::endl;
}

/*This header file contains simple functions for representing software information to the user*/
#ifndef __software_info_h
#define __software_info_h

#include <iostream>



void startup_message() {
    std::cout << "Welcome to Tenstat MCV Number-Crunching Software" << std::endl;
    std::cout << "Made by David Medlock in Visual Studio Code" << std::endl;
    std::cout << "Tenstat is open-source. Repository at https://github.com/Metalsofa/Tenstat_MCV" << std::endl;
}

#endif
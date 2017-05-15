#include <string>
#include <iostream>
#include <stdlib.h>

void die(std::string message){
   std::cerr << message << std::endl;
   exit(1);
}
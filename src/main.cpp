#include <iostream>

int main(int argc, char** argv){
    if(argc != 1)
        for(int i = 0; i != argc; ++i)
            std::cout << argv[i] << "\n";
}

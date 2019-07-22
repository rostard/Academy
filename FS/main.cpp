#include <iostream>

#include "FS.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    FS fs(1024 * 1024 * 1024);
    auto fd = fs.OpenFolder("/");
    for(auto& i: fd.GetSubItems())
    {
        std::cout<<i->GetFullPath()<<'\n';
    }
    fs.CreateEmptyFile("/test.txt");
    return 0;
}
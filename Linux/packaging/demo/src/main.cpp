#include <iostream>
#include <string>

#include "Demo.hpp"

int main(int argc, char *argv[])
{
    std::string action(argv[1]);

    Demo d;
    std::string result;
    if (action == "--name")
    {
        result = d.name();
    }
    else if (action == "--version")
    {
        result = d.version().to_str();
    }
    std::cout << result << std::endl;

    return 0;
}

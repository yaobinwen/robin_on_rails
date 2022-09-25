#include <iostream>
#include <stdexcept>

int
main(int argc, char *argv[])
{
    int *p = new int(10);
    delete p;
    try
    {
        delete p; // Expect bad things to happen.
        std::cout << "Nothing happens." << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Catch an unexpected exception." << std::endl;
    }

    return 0;
}

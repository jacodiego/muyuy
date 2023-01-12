#include "engine/core.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{

    muyuy::Core core{};

    try
    {
        core.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#include <CppUTest/CommandLineTestRunner.h>

#include "event.hpp"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

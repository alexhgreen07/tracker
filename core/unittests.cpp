#include <CppUTest/CommandLineTestRunner.h>

#include "scheduler.hpp"

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

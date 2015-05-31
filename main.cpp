#include <string>
#include <iostream>

#include <app.hpp>

using namespace Tracker::Application;

int main(int ac, char** av)
{
    string input;
    
    TrackerApp app;
    app.run();
    
    getline (cin, input);
    
    return 0;
}

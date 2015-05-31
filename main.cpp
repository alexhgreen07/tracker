#include <string>
#include <iostream>

#include <app.hpp>

int main(int ac, char** av)
{
    string input;
    
    TrackerApp app;
    app.run();
    
    getline (cin, input);
    
    return 0;
}

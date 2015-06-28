#include <string>
#include <iostream>
#include <vector>

#include <app.hpp>

using namespace Tracker::Application;

int main(int argc, char* argv[])
{
    string input;
    string dbName = "bin/db.sqllite3";
    vector<string> args(argv + 1, argv + argc);

    if(args.size() > 0)
    {
    	dbName = args[0];
    }

    TrackerApp app(dbName);
    app.start();
    
    getline (cin, input);
    
    return 0;
}

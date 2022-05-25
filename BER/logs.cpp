#include "includes.h"
using namespace std;

fstream logFile; //handler for log file

void openLog(string fileName)
{
    logFile.open(fileName.c_str(), ios_base::app);
    if (!logFile.good() == true)
    {
        cout << "Cannot open log file " << "log.txt" << endl;
    }
    saveLog("Log file opened");
}

void closeLog(void)
{
    saveLog("Log file closed");
    logFile.close();
}

void saveLog(string msg)
{
    string ss;
    time_t currentTime = time(NULL);
    string txttime = (string)asctime(localtime(&currentTime));
    txttime = txttime.substr(0, txttime.length() - 1);
    ss = (string)"T: " + txttime + " M: " + msg + "\n";
    logFile << ss.c_str();
    logFile.flush();
    cout << ss.c_str();
    cout.flush();
}
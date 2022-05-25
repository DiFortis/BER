#include "includes.h"
using namespace std;
struct CompareResults
{
    double tot; //total number of bits
    double err; //error bits (different bits)
    float ber;  //ber calc result
    clock_t t1; //calc start time
    clock_t t2; //calc stop time
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2);
void CreateTestFile(const string name, const int count, const char value);
CompareResults calculateBer(string fpath1, string fpath2);
void printResult(CompareResults results);
int main(int argc, char* argv[])
{
    string fpath1; //path to file 1
    string fpath2; //path to file 2
    CompareResults results;

    openLog("logs.logs"); //logs in file
    if (argc != 3) //check the number of arguments
    {
        saveLog("User did not provide path to the files.");
        saveLog("Creating the test files for user...");
        //test 1
        CreateTestFile("test1_file1.bin", 100, 0xFF); 
        CreateTestFile("test1_file2.bin", 100, 0xFF); 
        //test 2
        CreateTestFile("test2_file1.bin", 100, 0x55);
        CreateTestFile("test2_file2.bin", 100, 0x45);
        //test 3
        CreateTestFile("test3_file1.bin",400000000,0x55); 
        CreateTestFile("test3_file2.bin",400000000,0x50);

        saveLog("Test files are being prepared");
        saveLog("Re-run with correct arguments: For test 1: G:/Visual_Studio_Repos/BER/BER.exe test1_file1.bin test1_file2.bin For test 2: G:/Visual_Studio_Repos/BER/BER.exe test2_file1.bin test2_file2.bin For test 3 G:/Visual_Studio_Repos/BER/BER.exe test3_file1.bin test3_file2.bin   Paste it in CMD");
    }
    else 
    {
        fpath1 = argv[1];
        fpath2 = argv[2];

        saveLog("Processing test files");
        results = calculateBer(fpath1, fpath2);
        printResult(results);
    }

    closeLog();
    return 0;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2; 
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

void CreateTestFile(const string name, const int count, const char value)
{
    fstream f;
    f.open(name.c_str(), ios::binary | ios::out);
    for (int i = 0; i < count; i++)
    {
        f.write((char*)&value, 1);
    }
    f.close();
}



CompareResults calculateBer(string fpath1, string fpath2)
{
    fstream f1, f2; 
    CompareResults results;
    results.t1 = 0;
    results.t2 = 0;
    results.ber = 0;
    results.err = 0;
    results.tot = 0;

    saveLog("Calculating BER...");
    f1.open(fpath1.c_str(), ios::binary | ios::in);
    f2.open(fpath2.c_str(), ios::binary | ios::in);
    char a = 0x00;
    char b = 0x00;
    results.t1 = clock();

    while (!f1.eof())
    {
        f1 >> a; 
        f2 >> b; 
        if (!f1.eof()) 
        {
            results.err += hammingDistance(a, b); 
            results.tot += 8; 
        }
    }

    results.ber = (float)results.err / results.tot; // calculate ber
    results.t2 = clock();
    saveLog("BER calculations are done");
    return results; //return structure with all results
}

void printResult(CompareResults results)
{
    stringstream message;
    message << "Results are: " << endl;
    message << "BER: " << results.ber << endl;
    message << "Tot: " << results.tot << endl;
    message << "Err: " << results.err << endl;
    message << "Calc time: " << ((float)results.t2 - results.t1) / CLOCKS_PER_SEC << " sec " << endl;
    saveLog(message.str());
}
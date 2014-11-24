/********************************************************************
 * Program to implement Eclat algorithm to perform itemset mining ***
 * Jun Dan, 10/26/2014                                            ***
 * Usage:                                                         ***
 * ./main input_file minsup                                       ***
 */

#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <string>
#include "DbReader.hpp"
#include "PatternStore.hpp"

using namespace std;

int main(int argc, char* argv[]){

    time_t startT, endT;
    time(&startT); // record start time

    char* filename; // input file name
    int minsup; // min sup
    bool prnt = false; // print or not

    // use getopt to get options and arguments
    int c;
    while((c = getopt(argc, argv, "f:s:p")) != -1){
        switch(c){
            case 'f': // input file
                filename = optarg;
                break;
            case 's': // min sup
                minsup = (int)atof(optarg);
                break;
            case 'p': // print or not
                prnt = true;
                break;
            default: // print usage
                cout << "Usage: \n" << "./main -f input -s minsup [-p]\n"
                     << "(-f input) should be together\n"
                     << "(-s minsup) should be together\n"
                     << "the three argument pairs can be in any order\n";
        }
    }


    DbReader reader; // reader of input file

    // the DbReader class can detect the type of input file, int, char, string
    switch( reader.template_type(filename) ){
        case 1: // int
          {
            cout << "The items are integer.\n";
            PatternStore<int> pS1; // pattern store
            pS1.set_minsup(minsup); // set up min sup
            reader.read_txt(pS1, filename, minsup); // read input and populate single item tid list

            pS1.eclat(pS1.root()); // eclat algorithm
            pS1.display(); // show how many frequent patterns

            if (prnt)
                  pS1.print_patterns(); // if -p is specified, print patterns
            break;
          }
        case 2: // char
          {
            cout << "The items are char.\n";
            PatternStore<char> pS2;
            pS2.set_minsup(minsup);
            reader.read_txt(pS2, filename, minsup);

            pS2.eclat(pS2.root());
            pS2.display();

            if (prnt)
                  pS2.print_patterns();
            break;
          }
        case 3: // string
          {
            cout << "The items are string.\n";
            PatternStore<string> pS3;
            pS3.set_minsup(minsup);
            reader.read_txt(pS3, filename, minsup);

            pS3.eclat(pS3.root());
            pS3.display();

            if (prnt)
                  pS3.print_patterns();
            break;
          }
        default:
            cout << "The format of item in input file cannot be recognized.\n";

    }

    time(&endT);
    cout << "The program takes " << difftime(endT, startT) << " seconds to finish.\n\n"; // output time

    return 0;
}

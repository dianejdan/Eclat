/*********************************************************
 * DbReader is a class for reading input file. And it  ***
 * also populates the single-item frequent patterns    ***
 * with their tid-list                                 ***
 */

#ifndef DBREADER_HPP // guard for multiple inclusion
#define DBREADER_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <typeinfo>
#include "PatternStore.hpp"
#include "Pattern.hpp"

using namespace std;

class DbReader{

public:

    // the function for reading input file.
    // the variables of input file, to store patterns, and minsup should be provided
    // filename - input file
    // PatternStore - store patterns
    template <class Typ>
    void read_txt(PatternStore<Typ>& pS, char* filename, int minsup){

        ifstream in_(filename); // open input file
        char buffer[4096]; // store each line/transaction
        string line; // change char array to a string
        stringstream ss; // used for parse each item in a line/transaction

        int tid = 1; // transaction id, start from 1
        map<Typ, vector<int> > tidList; // use a map to store item and its transactions

        // parse the input file, add item and tid into the tid list
        while(in_.getline(buffer, 4096)){
            line = buffer; // change char array to string
            ss << line; // transform string to a stringstream so we can parse items
            Typ item;

            while(ss >> item){ // parse each item in the transaction

                if (tidList.find(item) == tidList.end()){ // if not find a item, create a new
                                                          // record in tidList for this item
                    vector<int> tids; // transaction list for the current item
                    tids.push_back(tid); // add this current transaction to transaction list
                    tidList.insert(std::pair<Typ, vector<int> >(item, tids)); // add <item, tids> to the map of tidList

                }else{

                    tidList[item].push_back(tid); // if the item is already in map of tidList, find the <item, tidList>
                                                  // add the current transaction id to the tid list

                }
            }

            ss.clear(); // clear state of stringstream for next transaction
            ++tid; // add one to the transaction id for next transaction
        }

        //cout << "Number of unique items in input file is  " << tidList.size() << "." << endl; // check how many items in the input file
        cout << endl;

        // now populate level-1 (single-item) frequency patterns
        int count = 0; // count level-1 (single-item) frequency patterns
        typename map<Typ, vector<int> >::iterator iter;

        for (iter = tidList.begin(); iter != tidList.end(); ++iter){ // iterate through tid list
            if (iter->second.size() >= minsup){ // store level-1 pattern only when |t(i)| > minsup
                pS.add_pattern(pS.root(), iter->first, iter->second); // add the frequent pattern to pattern store
                count++;
            }
        }

        in_.close(); // finish reading, close input file
    }


    int template_type(char* filename){

        ifstream in_(filename); // open input file
        char buffer[4096]; // store each line/transaction
        string line;

        int typ = 3;
        // parse the input file, add item and tid into the tid list
        in_.getline(buffer, 4096);
        line = buffer;
        size_t pos1 = line.find_first_not_of(" ");
        if (buffer[pos1+1] == ' '){
            if (buffer[pos1] == '0' || buffer[pos1] == '1' || buffer[pos1] == '2' ||
                buffer[pos1] == '3' || buffer[pos1] == '4' || buffer[pos1] == '5' ||
                buffer[pos1] == '6' || buffer[pos1] == '7' || buffer[pos1] == '8' ||
                buffer[pos1] == '9')
                typ = 1;
            else
                typ = 2;
        }

        in_.close();
        return typ;
    }

};

#endif


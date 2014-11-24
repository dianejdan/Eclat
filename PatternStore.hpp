/*****************************************************************************
 * PatternStore class store patterns. Also, it utilizes the data structure ***
 * of Pattern to find siblings. PatternStore also performs Eclat algorithm ***
 * to perform itemset mining. The Eclat algorithm can be a separate class. ***
 * But it is more convenient to put Eclat in PatternStore since it stores  ***
 * patterns.                                                               ***
 *                                                                         ***
 * PatternStore is a tree structure. The joined itemset between X and Y    ***
 * (X < Y) is the child node of X. Here, in order to save memory, I uti-   ***
 * lized the observation that if X and Y join to form Z, then, only the    ***
 * last item in Z is the last item in Y and all other items in Z are the   ***
 * same as all other items in X. Therefore, I only need to store one item  ***
 * (the last item) in each Pattern.                                        ***
 *                                                                         ***
 * Graph illustrationg:                                                    ***
 *             A      B      C                                             ***
 *            / \     |                                                    ***
 *           B   C    C                                                    ***
 *           |                                                             ***
 *           C                                                             ***
 * To obtain the frequent pattern, I just need to DFS the tree: A, AB, ABC ***
 * AC, B, BC, C                                                            ***
 *                                                                         ***
 * On the other hand, the whole structure of PatternStore can be imple-    ***
 * mented using map<vector<int>, vector<int> >. But it is kind of slow and ***
 * memory-unefficient.                                                     ***
 */

#ifndef PATTERNSTORE_HPP
#define PATTERNSTORE_HPP

#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "Pattern.hpp"

using namespace std;

template <class Typ>
class PatternStore{
public:
    PatternStore(){ numPatterns = 0; 
                    freqPatterns = new Pattern<Typ>();}

    // show how many frequent patterns
    void display(){
        cout << "number of frequent tid_list " << numPatterns << endl;
    }

    // set the minsup
    void set_minsup(int minsup_){
        minsup = minsup_;
    }

    // add a new pattern to the pattern store
    // since the pattern store is a tree structure, I created a new Pattern
    // then add this new Pattern to its parent, i.e. one item fewer Pattern
    // and share all other items.
    void add_pattern(Pattern<Typ> *parent, Typ item, vector<int> newTidList){
        Pattern<Typ> *new_pat = new Pattern<Typ>(); // create a Pattern node
        new_pat->set_item(item);
        new_pat->set_tid_list(newTidList);
        parent->add_child(new_pat); // add to the children list of its parent
                                    // i.e. I link its parent and its child
        ++numPatterns; // once add a frequent pattern, increase number of frequent
                       // pattern by 1
    }

    // return the root node, which is empty. to start the Eclat item mining
    Pattern<Typ> * root(){
        return freqPatterns;
    }

    // Eclat algorithm, the parent node is the parameter
    // I will loop through all children of this parent, and calculate the
    // join of its children, if the intersection of tid lists is larger than
    // minsup, put the Pattern into pattern store, the item of this new node is
    // the last item of the "larger" child. i.e. "larger" child has an index
    // larger than the "smaller" child
    void eclat(Pattern<Typ> *parent){
        vector<Pattern<Typ> * > myChildren = parent->get_children(); // get children list

        for (int i = 0; i < myChildren.size(); ++i){ // loop through child in children list
            int newPats = 0; // a count to see if there are new Patterns

            for (int j = (i+1); j < myChildren.size(); ++j){ // find the "larger" child
                // perform the join of tid lists of two children
                vector<int> joinTidList = myChildren[i]->join_tid_list(myChildren[j]);
                if (joinTidList.size() >= minsup){ // check if the join tid list is larger than minsup
                    // add this new pattern as a new child of the smaller child for joining
                    add_pattern(myChildren[i], myChildren[j]->get_item(), joinTidList);
                    ++newPats; // new pattern count increases by 1
                }
            }
            if (newPats > 0)
                eclat(myChildren[i]); // if more than one new patterns, eclat the smaller child for joining
                                      // it is like a DFS of tree structure
        }
    }

    // print the patterns, I use DFS. Because it is a recursive call
    // and I want to hide information, I use a print_work function to
    // print. print_patterns is a "wrapper" for print_work.
    void print_patterns(){
        printCount = 1; // the number of printed pattern
        print_work(freqPatterns, "itemset"); // "itemset" just a header
    }

    // real print pattern function
    void print_work(Pattern<Typ> *parent, string head){
        vector<Pattern<Typ> *> myChildren = parent->get_children(); // get children list

        stringstream ss; // for forming new header

        for (int i = 0; i < myChildren.size(); ++i){ // loop through child in children list
            cout << "[" << printCount << "]" << head << "-" << myChildren[i]->get_item() << "\n\n"; // print the current pattern
            ++printCount; // add printed pattern number by 1
            string newHead; 
            ss << head << "-" << myChildren[i]->get_item(); // form the new header
            ss >> newHead;
            ss.clear();
            print_work(myChildren[i], newHead); // recursive call
        }
    }

private:
    Pattern<Typ> *freqPatterns; // the root node of pattern store
    int numPatterns; // number of patterns
    int minsup; // minsup
    int printCount;
};

#endif

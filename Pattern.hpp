/********************************************************************
 * Pattern class represents a pattern. It contains the additional ***
 * (new) item different from its parent, like explained in Patt-  ***
 * ernStore class. Also, Pattern class stores the tid list, which ***
 * I used vector to store since vector is much easy to use. Since ***
 * every Pattern can be a parent pattern for some other pattern,  ***
 * I have a children list component in the Pattern class. The     ***
 * Pattern class also defined some standard set-get methods and   ***
 * the join (extension). List siblings can be obtained through    ***
 * children list. The support counting can be obtained using      ***
 * the tidList.size()                                             ***
 */

#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <class Typ>
class Pattern; // forward declaration of class, since I used Pattern*
               // in Pattern class, as the linkage of parent and child

template <class Typ>
class Pattern{
public:
    Pattern(){}

    // set the item
    void set_item(Typ itm){
        item = itm;
    }

    // set the tid_list
    void set_tid_list(vector<int> tidList_){
        tidList = tidList_;
        // for the ease of joining two tid lists, I sort the tid list
        sort(tidList.begin(), tidList.end());
    }

    // get item
    Typ get_item(){
        return item;
    }

    // add a new child to children list
    void add_child(Pattern<Typ> *child){
        children.push_back(child);
    }

    // get children list
    vector<Pattern<Typ> *> get_children(){
        return children;
    }

    // join (extend) tid list
    vector<int> join_tid_list( Pattern<Typ> *x){
        vector<int> ret; // joined tid list
        vector<int> xTidList = x->tidList; // the "larger" child tid list

        int i = 0; // iterator for one tid list
        int j = 0; // iterator for another tid list

        while (i < xTidList.size()){ // go through one tid list
            while (j < tidList.size()){ // go through second tid list

                if (xTidList[i] == tidList[j]){ // if the tid of two tid lists
                                                // are the same, add this tid to
                                                // joined tid list. The first tid list
                                                // catches up the second
                    ret.push_back(xTidList[i]);
                    ++j; // move iterator of second tid list by one
                    break; // get out of the while loop since I need to compare
                           // this new tid with the next tid in the first tid list
                }

                if (xTidList[i] < tidList[j]){ // if the tid in first tid list is 
                                               // smaller than the second one tid list
                                               // break, the second one needs to catch
                                               // up the first tid list
                    break;
                }
                ++j; // if two tids are unequal, or they are racing, move the iterator
                     // of second tid list by one
            }
            ++i; // move the iterator of the first tid list by one
        }
        return ret;
    }

    // display the item and tid list for this current Pattern
    void display(){
        cout << "Item " << item << " " << tidList.size() << endl;
        for (int i = 0; i < tidList.size(); ++i)
            cout << tidList[i] << " ";
        cout << endl;
    }

private:
    Typ item; // additional (new) item
    vector<int> tidList; // tid list
    vector<Pattern<Typ> *> children; // its children list
};

#endif

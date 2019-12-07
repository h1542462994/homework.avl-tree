//
// Created by cht on 2019/5/16.
//
#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<initializer_list>
#include <iomanip>
#include"consolecolor.h"

using namespace std;

class Menu
{
public:
    Menu()
    {
    }
    Menu(initializer_list<string> menu)
    {
        container.clear();
        for (auto p = menu.begin();p != menu.end();++p)
        {
            container.push_back(*p);
        }


    }

    string operator[] (int index)
    {
        return container[index];
    }

    friend ostream& operator<< (ostream& out, Menu& menu)
    {
        //out << "===================menu===================" << endl;
        out << consoleforecolor::green <<  "***************************************************" << endl;
        int index = 0;
        for (int i = 0; i < menu.size(); i++)
        {
            out << setw(18) << setiosflags(ios::left) << to_string(i+1)+"."+menu[i] ;
            if((i + 1) % 3 == 0){
                out << endl;
            }
            //out << (++index) << "." << menu[i] << endl;
        }
        if((menu.size()) % 3 != 0){
            out << endl;
        }
        out << "***************************************************" << endl << consoleforecolor::normal;
        //out << "==========================================" << endl;
        return out;
    }

    auto begin()
    {
        return container.begin();
    }

    auto end()
    {
        return container.end();
    }

    int size() {
        return container.size();
    }


private:
    vector<string> container;
};

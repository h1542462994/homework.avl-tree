//
// Created by cht on 2019/5/16.
//
#include"consolecolor.h"
using namespace std;
ostream& operator<< (ostream& out, consoleforecolor color)
{
    setConsoleColor(consolecolorhelper::getColorId(color));

    return out;
}


ostream& operator<< (ostream& out, consolebackcolor color)
{
    setConsoleColor(consolecolorhelper::getColorId(color));

    return out;
}

consolecolorjoin operator + (consoleforecolor fore, consolebackcolor back)
{
    return consolecolorjoin(fore, back);
}

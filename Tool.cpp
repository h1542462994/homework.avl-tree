//
// Created by cht on 2019/11/27.
//
#include "Tool.h"
void tryGetInt(const string &s, int &v) {
    istringstream iStringStream(s);
    iStringStream >> v;
}

int checkPassword(const string &password) {
    if(password.length() < 6 || password.length() > 64){
        return 1;
    }
    for(auto v: password) {
        string s = R"(_,./\[];:{}`~!@#$%^&*()'")";
        if((v >= '0' && v <= '9') || (v>='A' && v<='Z') || (v>='a' && v<='z') || s.find(v) != string::npos ){

        } else {
            return 2;
        }
    }
    return 0;
}

int checkUsername(const string &name) {
    if(name.length() < 2 || name.length() > 20){
        return 1;
    }
    for(auto v: name) {

        if((v >= '0' && v <= '9') || (v>='A' && v<='Z') || (v>='a' && v<='z') || v=='_'){
        }  else{
            return 2;
        }
    }
    return 0;
}



int checkFileName(const string &fileName) {
    if (fileName.empty())
    {
        return 1;
    }
    else if (fileName.size() > 40)
    {
        return 2;
    }
    else
    {
        string except = "/\\:*\"<>|?";
        for (int i = 0; i < except.length(); i++)
        {
            if (fileName.find(except[i]) != string::npos)
            {
                return 3;
            }
        }
    }
    return 0;
}
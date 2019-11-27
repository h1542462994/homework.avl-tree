//
// Created by cht on 2019/11/27.
//
#include <string>
#include <iostream>
#include <utility>
#include "Tool.h"

using namespace std;
class LoginData{
public:
    LoginData()= default;
    explicit LoginData(string username):username(std::move(username)){}
    LoginData(string username, string password):username(std::move(username)), password(std::move(password)){}

    string username;
    string password;

    static int tryParse(const string& value, LoginData& data){
        istringstream iStringStream(value);
        string u,p;
        iStringStream >> u >> p;
        int check1 = checkUsername(u);
        int check2 = checkPassword(p);
        if(check1 == 0 && check2 == 0){
            data.username = u;
            data.password = p;
            return 0;
        } else if(check1 == 1){
            return 1;
        } else if(check1 == 2){
            return 2;
        } else if(check2 == 1){
            return 3;
        } else {
            return 4;
        }

    }
    static int tryParseSec(const string& value, LoginData& data, string& pw){
        istringstream iStringStream(value);
        string u,p;
        iStringStream >> u >> p >> pw;
        int check1 = checkUsername(u);
        int check2 = checkPassword(p);
        int check3 = checkPassword(pw);
        if(check1 == 0 && check2 == 0 && check3 == 0){
            data.username = u;
            data.password = p;
            return 0;
        } else if(check1 == 1){
            return 1;
        } else if(check1 == 2){
            return 2;
        } else if(check2 == 1 || check3 == 1){
            return 3;
        } else {
            return 4;
        }
    }

    friend bool operator == (const LoginData& left, const LoginData& right){
        return left.username == right.username;
    }
    friend bool operator != (const LoginData& left, const LoginData& right){
        return left.username != right.username;
    }
    friend bool operator < (const LoginData& left, const LoginData& right){
        return left.username < right.username;
    }
    friend bool operator > (const LoginData& left, const LoginData& right){
        return left.username > right.username;
    }
    friend bool operator <= (const LoginData& left, const LoginData& right){
        return left.username <= right.username;
    }
    friend bool operator >= (const LoginData& left, const LoginData& right){
        return left.username >= right.username;
    }
    friend ostream& operator << (ostream& oStream, const LoginData& value){
        oStream << value.username << " " << value.password;
        return oStream;
    }
    friend istream& operator >> (istream& iStream, LoginData& value){
        iStream >> value.username >> value.password;
        return iStream;
    }
};


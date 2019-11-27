//
// Created by cht on 2019/11/27.
//
#pragma once
#include <string>
#include <sstream>
using namespace std;

void tryGetInt(const string& s,int& v);

int checkUsername(const string& name);

int checkPassword(const string& password);

//用于检查文件名是否合法。
int checkFileName(const string& fileName);


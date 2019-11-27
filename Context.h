//
// Created by cht on 2019/5/16.
//
#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include "Menu.h"
#include "listener.h"
#include <map>
#include <algorithm>
#include <fstream>
#include "Log.h"
#include "AVLTree.h"
#include "LoginData.h"
//#include"AppBase.h"

using namespace std;

class Context
{
public:
    Context()
    {
        init();
    }

    Menu& getMenu(string key) {
        return menus[key];
    }
    Menu& getCurrentMenu()
    {
        return menus[page];
    }

    void setOnPageChangedListener(IOnPageChangedListener* listener)
    {
        pageChangedListener = listener;
    }

    string getPage()
    {
        return page;
    }
    void setPage(string page)
    {
        this->page = page;
        if (pageChangedListener != NULL)
        {
            pageChangedListener->onPageChanged(page);
        }
    }

    bool isMenuIndexOf(const string& option, const string& index){
        auto menu = menus[page];
        int i = 1;
        for(const auto& v: menu){
            if(option == v){
                return to_string(i) == index;
            }
            i++;
        }

        return false;
    }

    AVLTree<LoginData> loginData;

    string subpage;
    //��ϵ�˵Ĵ洢

private:
    inline void init()
    {
        page = "home";
        menus["home"] = { "����","����","ע���˺�","��¼�˺�","ɾ���˺�","�޸�����","��ӡ","���л�","��ջ���","�˳�����"};
        subpage = "";
    }
    map<string, Menu> menus;
    string page;
    IOnPageChangedListener* pageChangedListener = nullptr;

};
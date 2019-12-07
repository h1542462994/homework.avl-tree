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

    string current;
private:
    inline void init()
    {
        page = "main";
        menus["main"] = { "导入","导出","转到普通用户界面","转到管理员界面","退出程序"};
        menus["client"] = {"退回主界面","注册","登录"};
        menus["logged"] = {"退出登录","欢迎","修改密码","删除账号"};
        menus["admin"] = {"退回主界面", "添加账号", "查看账号信息", "修改密码", "删除账号", "清除缓存","调试/打印","调试/显示所有信息"};
        subpage = "";
    }
    map<string, Menu> menus;
    string page;
    IOnPageChangedListener* pageChangedListener = nullptr;

};
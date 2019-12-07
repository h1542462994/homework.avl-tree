//
// Created by cht on 2019/11/26.
//
#include "App.h"
#include "consolecolor.h"
#include <sstream>
#include <string>
#include "Tool.h"
#include "Log.h"
#include <iostream>

using namespace std;

void App::onLoad() {
    cout << consoleforecolor::violet << "欢迎来到用户登录系统(AVL实验) VERSION:beta 5" << consoleforecolor::normal << endl;
}

string App::onTip() {
    string page = getContext().getPage();
    string subpage = getContext().subpage;
    if(subpage.empty()){
        cout << consoleforecolor::indigo << "page = " << page << endl;
        cout << getContext().getCurrentMenu();
    }

    string tip;

    if (subpage.empty()){
        tip = "请选择菜单中的一项";
    } else if(page == "main"){
        if(subpage == "import")
            tip = "请输入你想要导入的文件名(quit取消)";
        else if(subpage == "export")
            tip = "请输入你想要导出的文件名(quit取消)";
    } else if(page == "admin"){
        if(subpage == "register")
            tip = "请输入你想添加账号的用户名和密码(quit取消)";
        else if(subpage == "login")
            tip = "请输入你想查看账号的用户名(quit取消)";
        else if(subpage == "changepw")
            tip = "请输入你想修改账号的用户名和新密码(quit取消)";
        else if(subpage == "delete")
            tip = "请输入你想删除账号的用户名(quit取消)";
        else if(subpage == "clear")
            tip = "请确认你想删除所有缓存吗吗(y/n)";
    } else if(page == "client"){
        if (subpage == "register")
            tip = "请输入你想注册账号的用户名和密码(quit取消)";
        else if (subpage == "login")
            tip = "请输入用户名和密码(quit取消)";
    } else if(page == "logged"){
        if (subpage == "changepw"){
            tip = "请输入新密码(quit取消)";
        } else if (subpage == "delete"){
            tip = "请确认你想删除该账号吗(y/n)";
        }
    }

    return tip;
}

bool App::onHandleInput(string command) {
    string page = getContext().getPage();
    string subpage = getContext().subpage;
    if(getContext().isMenuIndexOf("调试/打印", command)){
        getContext().loginData.lPrintSlicing(toSimple,6);
    } else if(getContext().isMenuIndexOf("调试/显示所有信息", command)){
        getContext().loginData.print();
    } else if(page == "main"){
        if(subpage.empty()){
            if(getContext().isMenuIndexOf("退出程序", command)) {
                return false;
            } else if(getContext().isMenuIndexOf("导入", command)) {
                getContext().subpage = "import";
            } else if(getContext().isMenuIndexOf("导出", command)) {
                getContext().subpage = "export";
            } else if(getContext().isMenuIndexOf("转到普通用户界面", command)){
                getContext().setPage("client");
            } else if(getContext().isMenuIndexOf("转到管理员界面", command)){
                getContext().setPage("admin");
            } else {
                Log::e("输入错误，请重新输入");
            }
        } else if(subpage == "import" || subpage == "export"){
            int check = checkFileName(command);
            if(check == 1){
                Log::e("文件名不能为空");
            } else if(check == 2){
                Log::e("文件名太长，不应该超过40个字符");
            } else if(check == 3){
                Log::e("文件名不应该含有[/\\:*\"<>|?]这些非法字符");
            } else {
                if(subpage == "import"){
                    if(command == "quit"){
                        getContext().subpage = "";
                    } else {
                        if(getContext().loginData.loadFile(command)){
                            Log::d("导入成功");
                        } else {
                            Log::e("文件不存在");
                        }
                        getContext().subpage = "";
                    }

                } else if(subpage == "export"){
                    if(command == "quit"){
                        getContext().subpage = "";
                    } else {
                        getContext().loginData.saveFile(command);
                        Log::d("导出成功");
                        getContext().subpage = "";
                    }
                }
            }
        }
    } else if(page == "client"){
        if (subpage.empty()){
            if (getContext().isMenuIndexOf("退回主界面", command)){
                getContext().setPage("main");
            } else if (getContext().isMenuIndexOf("注册", command)){
                getContext().subpage = "register";
            } else if(getContext().isMenuIndexOf("登录", command)){
                getContext().subpage = "login";
            }
        } else if(subpage == "register" || subpage == "login") { //创建账号或登录账号
            LoginData data;
            if(command == "quit"){
                getContext().subpage = "";
            } else {

                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("用户名长度不合法，应在2-20位");
                    } else if(result == 2){
                        Log::e("用户名出现非法字符");
                    } else if(result == 3){
                        Log::e("密码长度不合法，应在6-64位");
                    } else if(result == 4) {
                        Log::e("密码出现非法字符");
                    }
                } else {
                    if(subpage == "register"){
                        if(getContext().loginData.find(data)!= nullptr){
                            Log::e("已存在该用户，无法创建用户");
                            getContext().subpage = "";
                        } else {
                            getContext().loginData.update(data);
                            Log::d("创建用户成功");
                            getContext().subpage = "";
                        }
                    } else {
                        bnode<LoginData>* u = getContext().loginData.find(data);
                        if(u == nullptr){
                            Log::e("用户不存在");
                            getContext().subpage = "";
                        } else if(u->value.password != data.password){
                            Log::e("密码错误");
                        } else {
                            Log::d(data.username + "登录成功");
                            getContext().current = data.username;
                            getContext().setPage("logged");
                            getContext().subpage = "";
                        }
                    }
                }
            }
        }
    } else if(page == "logged"){
        if( subpage.empty()){
            if (getContext().isMenuIndexOf("退出登录", command)){
                getContext().setPage("client");
            } else if (getContext().isMenuIndexOf("欢迎", command)){
                Log::w("Welcome!" + getContext().current);
            } else if(getContext().isMenuIndexOf("修改密码",command)) {
                getContext().subpage = "changepw";
            } else if(getContext().isMenuIndexOf("删除账号", command)) {
                getContext().subpage = "delete";
            }

        } else if(subpage == "delete"){
            if(command == "quit" || command == "n"){
                getContext().subpage = "";
            } else if (command == "y"){
                LoginData data(getContext().current, "");
                int result = getContext().loginData.remove(data);
                Log::d("删除用户成功");
                getContext().setPage("client");
                getContext().subpage = "";
            } else {
                Log::e("输入错误，请重新输入");
            }
        } else if(subpage == "changepw"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                LoginData data(getContext().current, command);
                bnode<LoginData>* u = getContext().loginData.find(data);
                if(u->value.password == data.password) {
                    Log::e("密码重复");
                } else {
                    getContext().loginData.update(data);
                    Log::d("修改密码" + data.username + "成功");
                    getContext().subpage = "";
                }
            }
        }
    } else if(page == "admin"){
        if( subpage.empty()){
            if (getContext().isMenuIndexOf("退回主界面", command)){
                getContext().setPage("main");
            } else if(getContext().isMenuIndexOf("添加账号", command)){
                getContext().subpage = "register";
            } else if(getContext().isMenuIndexOf("查看账号信息", command)){
                getContext().subpage = "login";
            } else if(getContext().isMenuIndexOf("修改密码", command)){
                getContext().subpage = "changepw";
            } else if(getContext().isMenuIndexOf("删除账号", command)){
                getContext().subpage = "delete";
            } else if(getContext().isMenuIndexOf("清除缓存", command)){
                getContext().subpage = "clear";
            }
        } else if(subpage == "register" || subpage == "login") { //创建账号或登录账号
            LoginData data;
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                if(subpage == "login"){
                    command += " 123456";
                }

                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("用户名长度不合法，应在2-20位");
                    } else if(result == 2){
                        Log::e("用户名出现非法字符");
                    } else if(result == 3){
                        Log::e("密码长度不合法，应在6-64位");
                    } else if(result == 4) {
                        Log::e("密码出现非法字符");
                    }
                } else {
                    if(subpage == "register"){
                        if(getContext().loginData.find(data)!= nullptr){
                            Log::e("已存在该用户，无法创建用户");
                            getContext().subpage = "";
                        } else {
                            getContext().loginData.update(data);
                            Log::d("创建用户" + data.username + "成功");
                            getContext().subpage = "";
                        }
                    } else {
                        bnode<LoginData>* u = getContext().loginData.find(data);
                        if(u == nullptr){
                            Log::e("用户不存在");
                            getContext().subpage = "";
                        } else {
                            cout << u->value << endl;
                            getContext().subpage = "";
                        }
                    }
                }
            }
        } else if(subpage == "delete"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                int check = checkUsername(command);
                if(check == 1){
                    Log::e("用户名长度不合法，应在2-20位");
                } else if(check == 2){
                    Log::e("用户名出现非法字符");
                } else {
                    LoginData data(command, "");
                    int result = getContext().loginData.remove(data);
                    if(!result){
                        Log::e("用户不存在");
                    } else {
                        Log::d("删除用户成功");
                    }
                    getContext().subpage = "";
                }

            }
        } else if(subpage == "changepw"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                LoginData data;
                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("用户名长度不合法，应在2-20位");
                    } else if(result == 2){
                        Log::e("用户名出现非法字符");
                    } else if(result == 3){
                        Log::e("密码长度不合法，应在6-64位");
                    } else if(result == 4) {
                        Log::e("密码出现非法字符");
                    }
                } else {
                    bnode<LoginData>* u = getContext().loginData.find(data);
                    if(u == nullptr){
                        Log::e("用户不存在");
                        getContext().subpage = "";
                    } else {
                        getContext().loginData.update(data);
                        Log::d("修改密码" + data.username + "成功");
                        getContext().subpage = "";
                    }
                }
            }
        } else if(subpage == "clear"){
            if(command == "quit" || command == "n"){
                getContext().subpage = "";
            } else if (command == "y"){
                getContext().loginData.clear();
                Log::d("已清除所有缓存");
                getContext().subpage = "";
            } else {
                Log::e("输入错误，请重新输入");
            }
        }
    }
    return true;
}
//
// Created by cht on 2019/11/26.
//
#pragma once

#include"AppBase.h"
#include"Context.h"

class App :public AppBase
{
public:
    App(Context* context) :AppBase(context) {}

protected:
    virtual void onLoad() override;

    virtual string onTip() override;

    virtual bool onHandleInput(string command) override;
private:
};
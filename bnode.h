//
// Created by cht on 2019/11/27.
//
template <typename T>
class bnode {
public:
    explicit bnode(T value):value(value),left(nullptr),right(nullptr),height(1),flag(false){

    }

    T value;
    bnode<T>* left;
    bnode<T>* right;
    int height = 0;
    /**
     * 用于记录该节点的最近状态 1:LL 2:RR 3:LR 4:RR
     */
    int flag = 0;
};
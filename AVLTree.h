//
// Created by cht on 2019/11/27.
//
#include <ntdef.h>
#include "bnode.h"
#include <iostream>
#include <iomanip>
#include "consolecolor.h"
#include <fstream>

using namespace std;

template <typename T>
class AVLTree{


public:
    /**
     * 从文件加载缓存
     * @param fileName
     * @return
     */
    bool loadFile(const string& fileName){
        fstream _file;
        _file.open(fileName, ios::in);

        if(!_file){
            return false;
        } else {
            deSerialize(_file);
            return true;
        }
    }

    /**
     * 保存缓存到文件
     * @param fileName
     */
    void saveFile(const string& fileName){
        fstream _file;
        _file.open(fileName, ios::out);

        serialize(_file);
    }

    /**
     * 更新节点，相等比较由==来决定。
     * @param value
     */
    void update(T value){
        //清除Flag标签
        _clearFlag(root);
        _update(root, value);
    }

    /**
     * 查找某一个节点
     * @param value
     * @return
     */
    bnode<T>* find(T value){
        return _find(root,value);
    }

    /**
     * 移除某一个节点
     * @param value
     * @return
     */
    bool remove(T value){
        _clearFlag(root);
        return _remove(root,value);
    }

    /**
     * 序列化函数
     * @param oStream
     */
    void serialize(ostream& oStream){
        _serialize(oStream,root);
    }

    /**
     * 打印函数
     */
    void print(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
        } else {
            _serialize(cout,root);
        }
    }

    /**
     * 反序列化函数
     * @param iStream
     * @return
     */
    bool deSerialize(istream& iStream){
        clear();
        string line;
        while(!iStream.eof()){
            getline(iStream,line);
            if(!line.empty()){
                istringstream iStringStream(line);
                T value;
                iStringStream >> value;
                update(value);
            }
        }

        return true;
    }

    /**
     * 清空整棵AVL树
     */
    void clear(){
        _clear(root);
        root = nullptr;
    }

    /**
     * 横向打印输出
     */
    void lPrint(int spacing = 4){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
        }
        _lPrint(cout, root, 0, spacing);
    }
private:
    bnode<T>* root = nullptr;
    /**
     * 右旋某一节点
     * @param node 要操作的节点 &reference type
     */
    void _r(bnode<T>*& node){
        bnode<T>* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;

        //标记Flag为右旋
        if(newRoot->flag == 0){
            newRoot->flag = 1;
        } else {
            newRoot->flag = 3;
        }

        _updateHeight(node);
        _updateHeight(newRoot);

        node = newRoot;
    }
    /**
     * 左旋某一节点
     * @param node 要操作的节点 &reference type
     */
    void _l(bnode<T>*& node){
        bnode<T>* newRoot = node->right;
        node->right = newRoot->left;

        newRoot->left = node;

        if(newRoot->flag == 0){
            newRoot->flag = 2;
        } else {
            newRoot->flag = 4;
        }


        _updateHeight(node);
        _updateHeight(newRoot);

        node = newRoot;
    }
    /**
     * 对当前节点进行调整平衡的操作 &reference type
     * @param node 要操作的节点
     */
    void _balance(bnode<T>*& node){
        int bf = _bf(node);

        if(bf > 1){ // 节点左边更高，为LL型或者LR型
            int inBf = _bf(node->left);
            if(inBf < 0){
                // 考虑LR型需要旋转左分支节点
                _l(node->left);
            }
            _r(node);
        } else if(bf < -1){ // 节点右边更高，为RR型或者RL型
            int inBf = _bf(node->right);
            if(inBf > 0){
                // 考虑RL型需要旋转右分支节点
                _r(node->right);
            }
            _l(node);
        }
    }

    /**
     * 对某个节点的高度重新计算
     * @param node
     */
    void _updateHeight(bnode<T>* node){
        node -> height = 1 + max(_getHeight(node->left), _getHeight(node->right));
    }

    /**
     * 获取某个节点的高度
     * @param node
     * @return
     */
    int _getHeight(bnode<T>* node){
        if(node == nullptr){
            return 0;
        } else {
            return node->height;
        }
    }

    /**
     * 获取一个节点的BF值
     * @param node
     * @return
     */
    int _bf(bnode<T>* node){
        if(node == nullptr){
            return 0;
        } else {
            return _getHeight(node->left) - _getHeight(node->right);
        }
    }

    /**
     * 辅助函数，用于横向输出整个树
     * @param oStream 输出流
     * @param node
     * @param start
     * @param spacing
     */
    void _lPrint(ostream& oStream, bnode<T>* node, int start = 0, int spacing = 4){
        if(node == nullptr){
            return;
        }

        _lPrint(oStream, node->right, start + spacing, spacing);

        oStream << setw(start) << " ";
        if(node->flag > 0){
            oStream << consoleforecolor::ochre;
            if(node->flag == 1){
                cout << "(LL)";
            } else if(node->flag == 2){
                cout << "(RR)";
            } else if(node->flag == 3) {
                cout << "(LR)";
            } else if(node->flag == 4){
                cout << "(RL)";
            }
        }
        cout << node->value << endl;
        oStream << consoleforecolor::normal;
        _lPrint(oStream, node->left,start + spacing, spacing);
    }

    /**
     * 查找某一个节点
     * @param node
     * @param value
     * @return
     */
    bnode<T>* _find(bnode<T>* node, T value){
        if(node == nullptr){
            return nullptr;
        } else if(node->value == value){
            return node;
        } else {
            if(value < node->value){
                return _find(node->left,value);
            } else {
                return _find(node->right,value);
            }
        }
    }

    /**
     * 序列化
     * @param oStream
     * @param node
     */
    void _serialize(ostream& oStream, bnode<T>* node){
        if(node == nullptr){
            return;
        }
        oStream << node->value << endl ;
        _serialize(oStream,node->left);
        _serialize(oStream,node->right);
    }

    /**
     * 更新一个值，或替换或插入。
     * @param node
     * @param value
     * @return
     */
    void _update(bnode<T>*& node, T value){
        if(node == nullptr){
            node = new bnode<T>(value);
        }

        //递归查找插入点
        if(value < node->value){
            _update(node->left, value);
        } else if(value > node->value){
            _update(node->right, value);
        } else {
            node->value = value;
        }

        //更新节点的高度
        _updateHeight(node);

        _balance(node);

    }

    void _clearFlag(bnode<T>* node){
        if(node == nullptr){
            return;
        } else {
            node->flag = 0;
        }

        _clearFlag(node->left);
        _clearFlag(node->right);
    }

    /**
     * 内部函数，找到某个子树的最大节点
     * @param node
     * @return
     */
    bnode<T>* _findMax(bnode<T>* node){
        if(node == nullptr){
            return nullptr;
        } else if(node->right == nullptr){
            return node;
        }
        return _findMax(node->right);
    }

    /**
     * 内部函数，找到某个子树的最小节点
     * @param node
     * @return
     */
    bnode<T>* _findMin(bnode<T>* node){
        if(node == nullptr){
            return nullptr;
        } else if(node->left == nullptr){
            return node;
        }
        return _findMin(node->left);
    }

    /**
     * 内部函数:移除某一个值
     * @param node
     * @param value
     * @return
     */
    bool _remove(bnode<T>* &node, T value){
        bool flag = false;
        if(node == nullptr){
            flag = false;
        } else if(node->value == value){ //要删除的节点刚好为根节点
            flag = true;
            if(node->left != nullptr && node->right!= nullptr){
                if(_getHeight(node->left) > _getHeight(node->right)){
                    //如果左子树更高，将左子树最大的元素赋给根节点，让后删除左子树最大的元素。
                    node->value = _findMax(node->left)->value;
                    _remove(node->left, node->value);
                    _balance(node->left);
                } else {
                    //如果右子树更高，将右子树最小的元素赋给根节点，然后删除右子树最小的元素。
                    node->value = _findMin(node->right)->value;
                    _remove(node->right, node->value);
                    _balance(node->right);
                }

            } else { //如果只有一边有子树，则直接用子节点代替根节点即可
                bnode<T>* old = node;
                node = node->left != nullptr ? node->left : node->right;
                delete old;
            }
            return flag;
        } else if(value < node->value) { //要删除的节点在左子树上
            flag = _remove(node->left, value);
            _balance(node);
        } else if(value > node->value){
            flag = _remove(node->right, value);
            _balance(node);
        }

        return flag;
    }

    /**
     * 内部函数:删除所有的节点
     * @param node
     */
    void _clear(bnode<T>* node){
        if(node == nullptr){
            return;
        }
        _clear(node->left);
        _clear(node->right);

        delete node;
    }
};
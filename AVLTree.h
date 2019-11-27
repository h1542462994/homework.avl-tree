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
        root = _update(root, value);
    }

    bnode<T>* find(T value){
        return _find(root,value);
    }

    bool contains(T value){
        return _contains(root, value);
    }

    bool remove(T value){
        _clearFlag(root);
        return _remove(root,value);
    }


    void serialize(ostream& oStream){
        _serialize(oStream,root);
    }

    void print(){
        if(root == nullptr){
            cout << "Empty Tree" << endl;
        } else {
            _serialize(cout,root);
        }
    }

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

    void clear(){
        _clear(root);
        root = nullptr;
    }

    /**
     * 打印输出
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
     * 对应节点的右旋操作
     * @param node 要操作的节点
     * @return 右旋操作后的根节点
     */
    bnode<T>* _r(bnode<T>* node){
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

        return newRoot;
    }
    /**
     * 对应节点的左旋操作
     * @param node 要操作的节点
     * @return 左旋操作后的根节点
     */
    bnode<T>* _l(bnode<T>* node){
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

        return newRoot;
    }

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

    void _serialize(ostream& oStream, bnode<T>* node){
        if(node == nullptr){
            return;
        }
        oStream << node->value << endl ;
        _serialize(oStream,node->left);
        _serialize(oStream,node->right);
    }

    void _updateHeight(bnode<T>* node){
        node -> height = 1 + max(_getHeight(node->left), _getHeight(node->right));
    }

    int _getHeight(bnode<T>* node){
        if(node == nullptr){
            return 0;
        } else {
            return node->height;
        }
    }

    int _getBalance(bnode<T>* node){
        if(node == nullptr){
            return 0;
        } else {
            return _getHeight(node->left) - _getHeight(node->right);
        }
    }

    bnode<T>* _toBalanced(bnode<T>*node, T value){
        int balance = _getBalance(node);

        // LL型
        if(balance > 1 && value < node->left->value){ // LL型
            return _r(node);
        } else if(balance < -1 && value > node->right->value) { // RR 型
            return _l(node);
        } else if(balance > 1 && value > node->left->value) { // LR型
            node->left = _l(node->left);
            return _r(node);
        } else if(balance < -1 && value < node->right->value) { // RL 型
            node->right = _r(node->right);
            return _l(node);
        }

        _updateHeight(node);

        return node;
    }

    bnode<T>* _update(bnode<T>* node, T value){
        if(node == nullptr){
            return new bnode<T>(value);
        }

        //递归查找插入点
        if(value < node->value){
            node->left = _update(node->left, value);
        } else if(value > node->value){
            node->right = _update(node->right, value);
        } else {
            node->value = value;
            return node;
        }

        //更新节点的高度
        //_updateHeight(node);

        return _toBalanced(node,value);

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

    bnode<T>* _findMax(bnode<T>* node){
        if(node == nullptr){
            return nullptr;
        } else if(node->right == nullptr){
            return node;
        }
        return _findMax(node->right);
    }

    bnode<T>* _findMin(bnode<T>* node){
        if(node == nullptr){
            return nullptr;
        } else if(node->left == nullptr){
            return node;
        }
        return _findMin(node->left);
    }

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
                } else {
                    //如果右子树更高，将右子树最小的元素赋给根节点，然后删除右子树最小的元素。
                    node->value = _findMin(node->right)->value;
                    _remove(node->right, node->value);
                }
            } else { //如果只有一边有子树，则直接用子节点代替根节点即可
                bnode<T>* old = node;
                node = node->left != nullptr ? node->left : node->right;
                delete old;
            }
            return flag;
        } else if(value < node->value) { //要删除的节点在左子树上
            flag = _remove(node->left, value);
            node = _toBalanced(node, value);
        } else if(value > node->value){
            flag = _remove(node->right, value);
            node = _toBalanced(node, value);
        }

        return flag;
    }

    void _clear(bnode<T>* node){
        if(node == nullptr){
            return;
        }
        _clear(node->left);
        _clear(node->right);

        delete node;
    }
};
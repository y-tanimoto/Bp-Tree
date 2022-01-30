#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

#define M   3

class Node {
public:
    // コンストラクタ
    Node(const bool is_leaf, Node* parent);

    // 現在のキー数を取得
    int count_keys();

    // 現在の子ノード数を取得
    int count_children();

    // キーを追加可能か
    bool is_able_to_add();

    // 葉ノードか否か
    bool is_leaf();

    // 親ノードの設定
    void set_parent(Node* parent_node);

    // 親ノードの取得
    Node* get_parent();

    // キーの追加
    bool add(const int key_to_add, Node* node_to_add);

    // キーの取得
    int get_key(const int num);

    // 子ノードの取り出し（取得して削除）
    Node* pull_child(const int num);

    // キーの削除
    void del(const int num);

    // 部分木の最小値を取得
    int get_min_key_recursive();

    // ノードの中身を表示
    void print_node();

private:
    bool m_is_leaf;         // このノードが葉ノードか

    Node* m_parent;         // 親ノードへのポインタ
    
    int* m_keys[M];           // キー用配列
    Node* m_children[M+1]; // 子ノード用配列
};

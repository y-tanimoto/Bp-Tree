#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

#define M   5

class Node {
public:
    // コンストラクタ
    Node(const int size, Node* parent, bool is_leaf);

    // 要素の追加
    bool add(const int key_to_add);     // 葉ノード用
    bool add(Node* child_node_to_add);  // 葉以外のノード用

    // 要素の削除
    bool del(const int num);
    bool del(Node* del_node);

    // ノードの初期化
    void clear();

    // 親ノードの設定
    void set_parent(Node* parent);

    // 隣接ノードの設定
    void set_right_node(Node* right_node);

    // 隣接ノードの取得
    Node* get_right_node();

    // 最小値の取得
    int get_min_key_recursive();

    // キー値の取得
    int get_key(const int num);

    // キー値の取り出し（取得してこのノードからは除去）
    int pull_key(const int num);

    // 一番右にあるキー値の取り出し
    int pull_last_key();

    // 子ノードの取得
    Node* get_child(const int num);

    // 子ノードを取り出し（取得してこのノードからは除去）
    Node* pull_child(const int num);

    // 一番右にある子ノードを取り出し
    Node* pull_last_child();

    // 親ノードを取得
    Node* get_parent();

    // ノードが保持するキーの数
    int count_keys();

    // ノードが保持する子ノードの数
    int count_children();

    // このノードが葉ノードか
    bool is_leaf_node();

    // このノードが根ノードか
    bool is_root_node();

    // ノードに要素を追加可能か否か
    bool is_able_to_add();
    bool is_able_to_add_child();

    // ノードが保持するキーの表示
    void print_keys(int height);

    // 子ノードの解放
    void clear_recursive();

private:
    // メンバ変数
    int m_size;                         // ノードのサイズ
    int m_total_keys;                   // ノードが持つキーの数
    bool m_is_leaf;                     // このノードが葉ノードか否か
    bool m_is_empty;                    // このノードが空か否か
    Node* m_parent_node;                // 親ノードへのポインタ
    int m_keys[M];            // キー保持用配列
    Node* m_children[M];      // 子ノードへのポインタ保持用配列

    // メンバ関数
    bool m_insert(const int key_to_insert, Node* child_node_to_insert); // キーを配列に挿入
    void m_slide_back(const int num);                                   // num番目から後ろの要素を1つずつ後ろへずらす
    void m_slide_front(const int num);                                  // num番目から後ろの要素を1つずつ前へずらす

};

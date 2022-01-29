#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include <iomanip>

class Node {
public:
    // コンストラクタ
    Node(const int size, Node* parent);                 // 葉ノード用
    Node(const int size, Node* parent);                 // 葉以外のノード用

    // 要素の追加
    bool add(const int key_to_add);     // 葉ノード用
    bool add(Node* child_node_to_add);  // 葉以外のノード用

    // 要素の削除
    bool del(const int num);

    // ノードの初期化
    void clear();
    void clear(Node* left_child, Node* right_child);    // 葉以外のノード用

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

    // 子ノードの取得
    Node* get_child(const int num);

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

private:
    // メンバ変数
    int m_size;                         // ノードのサイズ
    int m_total_keys;                   // ノードが持つキーの数
    bool m_is_leaf;                     // このノードが葉ノードか否か
    Node* m_parent_node;                // 親ノードへのポインタ
    std::vector<int> m_keys;            // キー保持用配列
    std::vector<Node*> m_children;      // 子ノードへのポインタ保持用配列

    // メンバ関数
    bool m_insert(const int key_to_insert, Node* child_node_to_insert); // キーを配列に挿入
    void m_slide_back(const int num);                                   // num番目から後ろの要素を1つずつ後ろへずらす
    void m_slide_front(const int num);                                  // num番目から後ろの要素を1つずつ前へずらす

};

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>

class Node {
public:
    // コンストラクタ
    Node(const int size);               // 葉ノード用
    Node(const int size, Node* left_child, Node* right_child, Node* parent);    // 葉以外のノード用

    // 要素の追加
    bool add(const int key_to_add);     // 葉ノード用
    bool add(Node* child_node_to_add);  // 葉以外のノード用

    // 要素の削除
    bool del(const int key_to_add);     // 葉ノード用
    bool del(Node* child_node_to_add);  // 葉以外のノード用   

    // 最小値の取得
    int get_min_key_recursive();

    // ノードが保持するキーの表示
    void print_keys();

private:
    // メンバ変数
    int m_size;                         // ノードのサイズ
    int m_total_keys;                   // ノードが持つキーの数
    bool m_is_leaf;                     // このノードが葉ノードか否か
    Node* m_parent_node;                // 親ノードへのポインタ
    std::vector<int> m_keys;            // キー保持用配列
    std::vector<Node*> m_children;      // 子ノードへのポインタ保持用配列

    // メンバ関数
    void m_clear_array(int size);                                       // 配列の初期化
    bool m_is_able_to_add();                                            // ノードに要素を追加可能か否か
    bool m_insert(const int key_to_insert, Node* child_node_to_insert); // キーを配列に挿入
    void m_slide_back(const int num);                                   // num番目から後ろの要素を1つずつ後ろへずらす

};

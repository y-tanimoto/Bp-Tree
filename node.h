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

private:
    // メンバ変数
    int m_size;                         // ノードのサイズ
    int m_total_children;               // ノードが持つ子ノードの数
    int m_total_keys;                   // ノードが持つキーの数
    bool m_is_leaf;                     // このノードが葉ノードか否か
    Node* m_parent_node;                // 親ノードへのポインタ
    std::vector<int> m_keys;            // キー保持用配列
    std::vector<Node*> m_children;      // 子ノードへのポインタ保持用配列

    // メンバ関数
    void m_clear_array(int size);                       // 配列の初期化
    bool m_add_key_to_leaf(const int key_to_add);       // キーを登録（葉ノードの場合）
    bool m_add_child(Node* child_node_to_add);          // 子ノードを登録（葉以外のノードの場合）

};

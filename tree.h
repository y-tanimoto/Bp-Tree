#include "node.h"

class Tree {
public:
    // コンストラクタ
    Tree(const int size);

    // 葉ノードへのキーの追加
    bool add(const int key_to_add);

    // キーの探索
    Node* search(const int key_to_search);

    // 各ノードの表示
    void print_tree();

private:
    // メンバ変数
    int m_node_size;                // 各ノードのサイズ
    Node* m_root_node;              // 根ノード

    // メンバ関数
    // 葉以外のノードに子ノードを追加
    bool m_add_child_node(Node* parent_node, Node* child_node);
};

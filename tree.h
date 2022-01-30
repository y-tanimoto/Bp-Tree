#include "node.h"

class Tree {
public:
    // コンストラクタ
    Tree();

    // 葉ノードへのキーの追加
    bool add(const int key_to_add, Node* node_to_add);
    bool add(Node* parent_node, const int key_to_add, Node* node_to_add);

    // キーの探索
    void search(const int key_to_search);

    // 各ノードの表示
    void print_tree();

    // メモリ解放
    void clear_tree();

private:
    // メンバ変数
    Node* m_root_node;              // 根ノード

    // ノードを分割
    // 戻り値は右側ノード（左側ノードは引数に与えられたノードを再利用）
    Node* m_div(Node* left_node, int key_to_add, Node* node_to_add);

    // キーが入る葉ノードを探索
    Node* m_search_leaf_node(const int key_to_search);
};

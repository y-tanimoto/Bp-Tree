#include "node.h"

class Tree {
public:
    // コンストラクタ
    Tree();

    // 葉ノードへのキーの追加
    bool add(const int key_to_add, Node* node_to_add);
    bool add(Node* parent_node, const int key_to_add, Node* node_to_add);

    // キーの削除
    bool del(const int key_to_delete);
    bool del(Node* parent_node, const int key_to_delete, Node* node_to_delete);

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

    // ノードのもつ要素を移動(source_nodeからdist_nodeへ)
    bool m_move_children(Node* source_node, Node* dist_node, bool dist_is_right_node);

    // ノード同士をマージ
    // source_nodeの各要素をdist_nodeへ
    void m_marge(Node* source_node, Node* dist_node);   

    // キーが入るノードを探索
    Node* m_search_leaf_node(const int key_to_search);
};

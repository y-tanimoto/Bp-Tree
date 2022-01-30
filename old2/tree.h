#include "node.h"

class Tree {
public:
    // コンストラクタ
    Tree();

    // キーをツリーに追加
    void add(const int key_to_add, Node* node_to_add);

private:
    Node* m_root_node;              // 根ノード

    Node* m_search_target_node(const int key_to_search);        // キーの追加先ノードを探索
    void m_div(Node* target_node);                              // ノードを分割
};

#include <node.h>

// 配列の初期化
void Node::m_clear_array(int size) {
    m_keys = std::vector<int>(size, NAN);
    m_children = std::vector<Node*>(size, nullptr);
}

// 葉ノード用コンストラクタ
Node::Node(const int size) {
    m_size = size;
    m_is_leaf = true;
}

// 葉以外のノード用コンストラクタ
Node::Node(const int size, Node* left_child, Node* right_child, Node* parent) {
    m_size = size;
    m_is_leaf = false;

    // 配列の初期化


    // 子ノードを登録する
    
}

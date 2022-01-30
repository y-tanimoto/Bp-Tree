#include "tree.h"

// コンストラクタ
Tree::Tree() {
    // 根ノードの生成
    m_root_node = new Node(true, nullptr);
}

// キーをツリーに追加
void Tree::add(const int key_to_add, Node* node_to_add) {
    // 追加先ノードの探索
    Node* target_node = m_search_target_node(key_to_add);

    // 追加先ノードに追加可能であれば、キーを追加して終了
    if (target_node->is_able_to_add()) {
        target_node->add(key_to_add, node_to_add);
        return;
    }

    // 追加できない場合は、追加先ノードを分割
}

// キーの追加先ノードを探索
Node* Tree::m_search_target_node(const int key_to_search) {
    Node* current_node = m_root_node;

    while (current_node->is_leaf()) {
        int left = std::numeric_limits<int>::min();   // 左側の値の最終値はintの最大値に
        int right;

        for (int i=0; i<=current_node->count_keys(); i++) {
            if (i == current_node->count_keys()) {
                right = std::numeric_limits<int>::max();
            }
            else {
                right = current_node->get_key(i);
            }

            if (left <= key_to_search && key_to_search < right) {
                return current_node;
            }
        }
    }

    return nullptr;
}

// ノードを分割
void Tree::m_div(Node* target_node) {
    // 左ノードの保持数
    int left_hold = ceil((M+1)/2);
    // 右ノードの保持数
    int right_hold = floor((M+1)/2);

    
}

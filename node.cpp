#include "node.h"
// コンストラクタ
Node::Node(const bool is_leaf, Node* parent) {
    m_is_leaf = is_leaf;
    m_parent = parent;

    // 配列の初期化
    for (int i=0; i<M; i++) {
        m_keys[i] = nullptr;
    }
    for (int i=0; i<M+1; i++) {
        m_children[i] = nullptr;
    }
}

// 現在のキー数を取得
int Node::count_keys() {
    for (int i=0; i<M; i++) {
        if (m_keys[i] == nullptr) {
            return i;
        }
    }
    return M;
}

// 現在の子ノード数を取得
int Node::count_children() {
    for (int i=0; i<M+1; i++) {
        if (m_children[i] == nullptr) {
            return i;
        }
    }
    return M+1;
}

// キーを追加可能か
bool Node::is_able_to_add() {
    if (count_keys() <= M) {
        return true;
    }
    return false;
}

// 葉ノードか否か
bool Node::is_leaf() {
    return m_is_leaf;
}

// 親ノードの設定
void Node::set_parent(Node* parent_node) {
    m_parent = parent_node;
}

// 親ノードの取得
Node* Node::get_parent() {
    return m_parent;
}

// キーの追加
bool Node::add(const int key_to_add, Node* node_to_add) {
    // 追加先の探索
    int num;
    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;

    for (int i=0; i<=count_keys(); i++) {
        if (i == count_keys()) {
            right = std::numeric_limits<int>::max();   // 左側の値の最終値はintの最大値に
        }
        else {
            right = *m_keys[i];
        }

        // 既にキー値が存在するなら、子ノードを置き換え終了
        if (right == key_to_add) {
            // 子ノードに対し自分を親ノードに設定
            if (node_to_add != nullptr) {
                node_to_add->set_parent(this);
            }

            if (m_is_leaf) {
                m_children[i] = node_to_add;
            }
            else {
                m_children[i+1] = node_to_add;
            }
            
            return true;
        }

        // 左の値 < 追加するキー値 < 右の値 の場所にキーを挿入
        if (left < key_to_add && key_to_add < right) {
            // i番目以降の要素を後ろにずらす
            for (int j=count_keys()-1; j>=i; j--) {
                if (m_is_leaf) {
                    m_keys[j+1] = m_keys[j];
                    m_children[j+1] = m_children[j];
                }
                else {
                    m_keys[j+1] = m_keys[j];
                    m_children[j+2] = m_children[j+1];
                }
            }

            // 子ノードに対し自分を親ノードに設定
            if (node_to_add != nullptr) {
                node_to_add->set_parent(this);
            }

            // i番目に追加
            if (m_is_leaf) {
                m_keys[i] = new int(key_to_add);
                m_children[i] = node_to_add;
            }
            else {
                m_keys[i] = new int(key_to_add);
                m_children[i+1] = node_to_add;
            }
            return true;
        }
    }

    return false;
}

// キーの取得
int Node::get_key(const int num) {
    return *m_keys[num];
}

// 子ノードの取り出し（取得して削除）
Node* Node::pull_child(const int num) {
    Node* node_to_return = m_children[num];

    // このノードから削除
    del(num);

    return node_to_return;
}

// キーの削除
void Node::del(const int num) {
    if (num >= count_keys()) {
        return;
    }
    
    // num番目以降の要素を前にずらす
    for (int j=num; j<count_keys(); j++) {
        if (m_is_leaf) {
            if (j+1 >= count_keys()) {
                m_keys[j] = nullptr;
                m_children[j] = nullptr;
            }
            else {
                m_keys[j] = m_keys[j+1];
                m_children[j] = m_children[j+1];
            }
        }
        else {
            if (j+1 >= count_keys()) {
                m_keys[j] = nullptr;
                m_children[j+1] = nullptr;
            }
            else {
                m_keys[j] = m_keys[j+1];
                m_children[j+1] = m_children[j+2];
            }
        }
    }
    // 一番最後の要素はnullptrに初期化
    if (m_is_leaf) {
        int last = count_keys()-1;
        m_keys[last] = nullptr;
        m_children[last] = nullptr;
    }
    else {
        int last = count_keys()-1;
        m_keys[last] = nullptr;
        m_children[last+1] = nullptr;
    }
}

// 部分木の最小値を取得
int Node::get_min_key_recursive() {
    if (m_is_leaf) {
        return *m_keys[0];
    }

    return m_children[0]->get_min_key_recursive();
}

// ノードの中身を表示
void Node::print_node() {
    for (int i=0; i<count_keys(); i++) {
        std::cout << "[" << *m_keys[i] << "]";
    }
    std::cout << std::endl;
}

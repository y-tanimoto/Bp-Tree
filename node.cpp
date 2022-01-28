#include "node.h"

// 配列の初期化
void Node::m_clear_array(int size) {
    m_keys = std::vector<int>(size, NAN);
    m_children = std::vector<Node*>(size, nullptr);
}

// ノードに要素を追加可能か否か
bool Node::m_is_able_to_add() {
    if (m_total_keys + 1 > m_size) {
        return false;
    }
    return true;
}

// キーの挿入
bool Node::m_insert(const int key_to_insert, Node* child_node_to_insert) {
    // 空のノードであれば0番目に追加
    if (m_total_keys == 0) {
        m_keys[0] = key_to_insert;
        m_children[0] = child_node_to_insert;
        m_total_keys = 1;
        return true;
    }

    // そうでなければ、前から順に探索
    int left = -INFINITY;   // 左側の値の初期値は-∞
    int right;
    for(int i=0; i<=m_total_keys; i++) {
        if (i == m_total_keys) {
            // 一番うしろの要素+1番目まで到達した場合はそこに追加
            m_keys[i] = key_to_insert;              // i番目にキーを登録
            m_children[i] = child_node_to_insert;   // i番目に子ノードを登録
            m_total_keys += 1;
            return true;
        }
        
        right = m_keys[i];

        // 左の値 == key_to_insertなら、既にキー値が存在するのでfalse
        if (left == key_to_insert) {
            return false;
        }

        // 左の値 < key_to_insert < 右の値なら、そこに挿入
        if (left < key_to_insert && key_to_insert < right) {
            m_slide_back(i);                        // i番目から後ろの要素を後ろにずらす
            m_keys[i] = key_to_insert;              // i番目にキーを登録
            m_children[i] = child_node_to_insert;   // i番目に子ノードを登録
            return true;
        }

        left = right;
    }

    return false;
}

// num番目から後ろの要素を1つずつ後ろへずらす
void Node::m_slide_back(const int num) {
    for (int j=m_total_keys; j>=num+1; j--) {
        m_keys[j] = m_keys[j-1];
        m_children[j] = m_children[j-1];
    }
    m_total_keys += 1;
}

// 葉ノード用コンストラクタ
Node::Node(const int size) {
    m_size = size;
    m_total_keys = 0;
    m_is_leaf = true;

    // 配列の初期化
    m_clear_array(size);
}

// 葉以外のノード用コンストラクタ
Node::Node(const int size, Node* left_child, Node* right_child, Node* parent) {
    m_size = size;
    m_total_keys = 0;
    m_is_leaf = false;

    // 配列の初期化
    m_clear_array(size);

    // 子ノードを登録する
    m_children[0] = left_child;
    m_children[1] = right_child;
}

// 葉ノードに要素を追加
bool Node::add(const int key_to_add) {
    // 葉ノードでなければfalse
    if (!m_is_leaf) {
        return false;
    }

    // 追加不可能ならfalse
    if (!m_is_able_to_add()) {
        return false;
    }

    // 以下、追加可能の場合
    return m_insert(key_to_add, nullptr);
}

// 葉以外のノードに要素を追加
bool Node::add(Node* child_node_to_add) {
    // 葉ノードならfalse
    if (m_is_leaf) {
        return false;
    }

    // 追加不可能ならfalse
    if (!m_is_able_to_add()) {
        return false;
    }

    // 以下、追加可能の場合
    // キー値を取得（追加するノードの最小値）
    int key = child_node_to_add->get_min_key_recursive();
    return m_insert(key, nullptr);
}

// 最小値の取得
int Node::get_min_key_recursive() {
    // 葉ノードなら、最小のキー値を返す
    if (m_is_leaf) {
        return m_keys[0];
    }

    // 葉ノードでなければ、一番左の子ノードで再帰的に呼び出し
    return m_children[0]->get_min_key_recursive();
}

// ノードが保持するキーの表示
void Node::print_keys() {
    for (int i=0; i<m_total_keys; i++) {
        std::cout << "[" << m_keys[i] << "]";
    }
    std::cout << std::endl;
}

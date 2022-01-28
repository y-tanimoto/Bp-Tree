#include "node.h"

// 配列の初期化
void Node::m_clear_array(int size) {
    m_keys = std::vector<int>(size, NAN);
    m_children = std::vector<Node*>(size, nullptr);
}

// キーの挿入
bool Node::m_insert(const int key_to_insert, Node* child_node_to_insert) {
    // そうでなければ、前から順に探索
    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;
    for(int i=0; i<=m_total_keys; i++) {
        right = m_keys[i];

        if (i == m_total_keys) {
            // 一番うしろの要素+1番目まで到達した場合はそこに追加
            m_keys[i] = key_to_insert;              // i番目にキーを登録
            m_children[i] = child_node_to_insert;   // i番目に子ノードを登録
            m_total_keys += 1;
            return true;
        }

        // 左の値 == key_to_insertなら、既にキー値が存在するのでfalse
        if (left == key_to_insert) {
            print_keys();
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

// num番目から後ろの要素を1つずつ前へずらす
void Node::m_slide_front(const int num) {
    for (int j=num; j<=m_total_keys-1; j++) {
        m_keys[j] = m_keys[j+1];
        m_children[j] = m_children[j+1];
    }
    m_total_keys -= 1;

    // 最後の要素は削除
    m_keys[m_total_keys] = NAN;
    m_children[m_total_keys] = nullptr;
}

// 葉ノード用コンストラクタ
Node::Node(const int size, Node* parent) {
    m_size = size;
    m_parent_node = parent;
    m_total_keys = 0;
    m_is_leaf = true;

    // 配列の初期化
    m_clear_array(size);
}

// 葉以外のノード用コンストラクタ
Node::Node(const int size, Node* left_child, Node* right_child, Node* parent) {
    m_size = size;
    m_parent_node = parent;
    m_total_keys = 2;
    m_is_leaf = false;

    // 配列の初期化
    m_clear_array(size);

    // 子ノードを登録する
    m_children[0] = left_child;
    m_children[1] = right_child;

    // キー値は右の子ノードの最小値に設定
    m_keys[0] = right_child->get_min_key_recursive();
}

// 葉ノードに要素を追加
bool Node::add(const int key_to_add) {
    // 葉ノードでなければfalse
    if (!m_is_leaf) {
        return false;
    }

    // 追加不可能ならfalse
    if (!is_able_to_add()) {
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
    if (!is_able_to_add()) {
        return false;
    }

    // 以下、追加可能の場合
    // キー値を取得（追加するノードの最小値）
    int key = child_node_to_add->get_min_key_recursive();
    return m_insert(key, child_node_to_add);
}

// 要素の削除
bool Node::del(const int num) {
    m_slide_front(num);
    return true;
}

// 親ノードの設定
void Node::set_parent(Node* parent) {
    m_parent_node = parent;
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

// キー値の取得
int Node::get_key(const int num) {
    return m_keys[num];
}

// 子ノードの取得
Node* Node::get_child(const int num) {
    return m_children[num];
}

// 親ノードを取得
Node* Node::get_parent() {
    return m_parent_node;
}

// ノードが保持するキーの数
int Node::count_keys() {
    return m_total_keys;
}

// このノードが葉ノードか
bool Node::is_leaf_node() {
    return m_is_leaf;
}

// このノードが根ノードか
bool Node::is_root_node() {
    // 親ノードが存在しなければ根ノード
    if (m_parent_node == nullptr) {
        return true;
    }
    return false;
}

// ノードに要素を追加可能か否か
bool Node::is_able_to_add() {
    if (m_total_keys + 1 > m_size) {
        return false;
    }
    return true;
}

// ノードが保持するキーの表示
void Node::print_keys() {
    for (int i=0; i<m_total_keys; i++) {
        std::cout << "[" << m_keys[i] << "]";
    }
    std::cout << std::endl;

    // 子ノードがあれば子ノードでも表示
    for (int i=0; i<m_total_keys; i++) {
        if (m_children[i] == nullptr) {
            break;
        }
        m_children[i]->print_keys();
    }
}

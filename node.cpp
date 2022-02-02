#include "node.h"

// 葉ノード用コンストラクタ
Node::Node(Node* parent, bool is_leaf) {
    m_parent_node = parent;
    m_is_leaf = is_leaf;
    m_is_empty = true;
    m_total_keys = 0;
}

// ノードに要素を追加
bool Node::add(int key_to_add, Node* child_node_to_add) {
    // 追加不可能ならfalse
    if (!is_able_to_add()) {
        return false;
    }

    // 以下、追加可能の場合
    // 子ノードに親ノードとして自分を設定
    if (child_node_to_add != nullptr) {
        child_node_to_add->set_parent(this);
    }

    if (m_is_empty && !m_is_leaf) {
        m_children[0] = child_node_to_add;
        m_is_empty = false;
        m_total_keys = 0;
        m_total_children = 1;
        return true;
    }
    m_is_empty = false;

    return m_insert(key_to_add, child_node_to_add);
}

// num番目の要素の削除
bool Node::del(const int num) {
    m_slide_front(num);
    update_keys();
    return true;
}

// 子ノードの削除
bool Node::del_child(Node* del_node) {
    for (int i=0; i<m_total_children; i++) {
        if (m_children[i] == del_node) {
            m_slide_front(i);
            update_keys();
            return true;
        }
    }
    return false;
}

// キーの削除
bool Node::del_key(const int key_to_delete) {
    for (int i=0; i<m_total_keys; i++) {
        if (m_keys[i] == key_to_delete) {
            m_slide_front(i);
            update_keys();
            return true;
        }
    }
    return false;
}

// 親ノードの設定
void Node::set_parent(Node* parent) {
    m_parent_node = parent;
}

// 隣接ノードの設定
void Node::set_right_node(Node* right_node) {
    m_children[M] = right_node;
}

// 隣接ノードの設定
Node* Node::get_right_node() {
    if (!is_leaf_node()) {
        return nullptr;
    }
    return m_children[M];
}

// 部分木の最小値の取得
int Node::get_min_key_recursive() {
    // 葉ノードなら、最小のキー値を返す
    if (m_is_leaf) {
        return m_keys[0];
    }

    // 葉ノードでなければ、一番左の子ノードで再帰的に呼び出し
    return m_children[0]->get_min_key_recursive();
}

// 部分木の最大値の取得
int Node::get_max_key_recursive() {
    // 葉ノードなら、最大のキー値を返す
    if (m_is_leaf) {
        return m_keys[m_total_keys-1];
    }

    // 葉ノードでなければ、一番右の子ノードで再帰的に呼び出し
    return m_children[m_total_children-1]->get_max_key_recursive();
}

// ノードの高さの取得
int Node::get_height() {
    if (is_root_node()) {
        return 0;
    }
    return m_parent_node->get_height() + 1;
}

// キー値の取得
int Node::get_key(const int num) {
    return m_keys[num];
}

// キー値の取り出し（取得してこのノードからは除去）
int Node::pull_key(const int num) {
    int ret = get_key(num);
    del(num);
    return ret;
}

// キー値が存在するか
bool Node::has_key(const int key_to_search) {
    for (int i=0; i<m_total_keys; i++) {
        if (m_keys[i] == key_to_search) {
            return true;
        }
    }
    return false;
}

// キー値の更新
// ボトムアップで再帰的に実行
void Node::update_keys() {
    // 葉ノードの場合は何もしない
    if (is_leaf_node()) {
        // 親ノードに対しても実行
        if (!is_root_node()) {
            m_parent_node->update_keys();
        }
        return;
    }

    for (int i=1; i<m_total_children; i++) {
        // (i-1)番目のキー値を子ノードが持つ部分木の最小値に
        m_keys[i-1] = m_children[i]->get_min_key_recursive();
    }
    
    // 親ノードに対しても実行
    if (!is_root_node()) {
        m_parent_node->update_keys();
    }
}

// 子ノードの取得
Node* Node::get_child(const int num) {
    return m_children[num];
}

// 子ノードを取り出し（取得してこのノードからは除去）
Node* Node::pull_child(const int num) {
    Node* ret = get_child(num);
    del(num);
    return ret;
}

// 親ノードを取得
Node* Node::get_parent() {
    return m_parent_node;
}

// ノードが保持するキーの数
int Node::count_keys() {
    return m_total_keys;
}

// ノードが保持する子ノードの数
int Node::count_children() {
    if (is_leaf_node()) {
        return m_total_keys;
    }
    return m_total_children;
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
    return is_ok(1);   // 1個だけ追加した場合の結果を返す
}

// ノードの要素数が条件に合致するか
bool Node::is_ok(const int additional_num) {
    // 根ノードかつ葉ノードなら上限値を超えなければOK
    if (is_root_node() && is_leaf_node()) {
        return (m_total_keys + additional_num <= M);
    }
    // 根ノードなら上限値を超えなければOK
    if (is_root_node()) {
        return (m_total_children + additional_num <= M + 1);
    }
    // 葉ノードの場合、キーがROUNDDOWN((M+1)/2)個以上ならOK
    if (is_leaf_node()) {
        return (m_total_keys + additional_num >= required() && m_total_keys + additional_num <= M);
    }
    // それ以外の場合、ROUNDDOWN((M+1)/2)個以上ならOK
    return (m_total_children + additional_num >= required() && m_total_children + additional_num <= M + 1);
}

// ノードに必要な要素数
int Node::required() {
    if (is_root_node()) {
        return 0;
    }
    if (is_leaf_node()) {
        return floor((M+1)/2);
    }
    return ceil((M+1)/2);
}

// ノードが保持するキーの表示
void Node::print_keys(int height) {
    std::cout << std::setw(height);
    for (int i=0; i<m_total_keys; i++) {
        std::cout << "[" << m_keys[i] << "]";
    }
    std::cout << std::endl;

    // 子ノードがあれば子ノードでも表示
    for (int i=0; i<=m_total_keys; i++) {
        if (m_children[i] == nullptr) {
            break;
        }
        m_children[i]->print_keys(height+1);
    }
}
void Node::print_keys_once(int height) {
    std::cout << std::setw(height);
    for (int i=0; i<m_total_keys; i++) {
        std::cout << "[" << m_keys[i] << "]";
    }
    std::cout << std::endl;
}

// 子ノードの解放
void Node::clear_recursive() {
    if (m_is_leaf) {
        return;         // 葉ノードなら何もしない
    }

    for (int i=0; i<count_children(); i++) {
        m_children[i]->clear_recursive();
        delete m_children[i];
    }
}

// キーの挿入
bool Node::m_insert(const int key_to_insert, Node* child_node_to_insert) {
    // 葉ノードでなければ、子ノードはm_childrenの(i+1)番目に格納する
    int offset = 1;
    if (is_leaf_node()) {
        offset = 0;
    }

    // 追加するノードのもつ部分木の最大値 < このノードの持つ部分木の最小値なら、一番前に挿入
    if (child_node_to_insert != nullptr) {
        if (child_node_to_insert->get_max_key_recursive() < get_min_key_recursive()) {
            m_slide_back(0);
            m_children[0] = child_node_to_insert;
            update_keys();
            return true;
        }
    }

    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;
    for(int i=0; i<=m_total_keys; i++) {
        if (i == m_total_keys) {
            if (!m_is_leaf && m_children[i] != nullptr) {
                if (key_to_insert < m_children[i]->get_min_key_recursive()) {
                    m_keys[i] = m_children[i]->get_min_key_recursive();
                    m_children[i+1] = m_children[i];
                    m_children[i] = child_node_to_insert;
                    m_total_keys += 1;
                    m_total_children += 1;
                    return true;
                }
            }

            // 一番うしろの要素+1番目まで到達した場合はそこに追加
            m_keys[i] = key_to_insert;                      // i番目にキーを登録
            m_children[i+offset] = child_node_to_insert;    // (i+offset)番目に子ノードを登録
            m_total_keys += 1;
            m_total_children += 1;
            return true;
        }
        right = m_keys[i];

        // 左の値 == key_to_insertなら、既にキー値が存在するのでfalse
        if (left == key_to_insert) {
            return false;
        }

        // 左の値 < key_to_insert < 右の値なら、そこに挿入
        if (left < key_to_insert && key_to_insert < right) {
            m_slide_back(i);                                // i番目から後ろの要素を後ろにずらす
            m_keys[i] = key_to_insert;                      // i番目にキーを登録
            m_children[i+offset] = child_node_to_insert;    // (i+offset)番目に子ノードを登録
            return true;
        }

        left = right;
    }
    return false;
}

// num番目から後ろの要素を1つずつ後ろへずらす
void Node::m_slide_back(const int num) {
    // 葉ノードの場合
    if (is_leaf_node()) {
        for (int j=M-1; j>=num+1; j--) {
            m_keys[j] = m_keys[j-1];
            m_children[j] = m_children[j-1];
        }
        m_total_keys += 1;
        m_total_children += 1;
    }
    // 葉以外のノードの場合
    else {
        for (int j=m_total_keys; j>=num; j--) {
            if (j > num) {
                m_keys[j] = m_keys[j-1];
            }
            m_children[j+1] = m_children[j];
        }
        m_total_keys += 1;
        m_total_children += 1;
    }
}

// num番目から後ろの要素を1つずつ前へずらす
void Node::m_slide_front(const int num) {
    // 葉のノードの場合
    if (is_leaf_node()) {
        for (int j=num; j<=m_total_keys-2; j++) {
            m_keys[j] = m_keys[j+1];
            m_children[j] = m_children[j+1];
        }

        // 最後の要素は削除
        m_keys[m_total_keys-1] = NAN;
        m_children[m_total_keys-1] = nullptr;

        m_total_keys -= 1;
        m_total_children -= 1;
    }
    // 葉以外のノードの場合
    else {
        for (int j=num; j<=m_total_keys-1; j++) {
            if (j >= 1) m_keys[j-1] = m_keys[j];
            m_children[j] = m_children[j+1];
        }
        if (m_total_keys > 0) {
            m_total_keys -= 1;
        }
        m_total_children -= 1;

        // 最後の要素は削除
        m_keys[m_total_keys] = NAN;
        m_children[m_total_children] = nullptr;
    }
}

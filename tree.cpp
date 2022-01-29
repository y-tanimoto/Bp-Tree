#include "tree.h"

// コンストラクタ
// ノードのサイズを設定し、最初の根ノードを生成
Tree::Tree(const int size) {
    m_node_size = size;

    // 最初のノード（根ノードかつ葉ノード）を生成
    m_root_node = new Node(size, nullptr);
}

// 葉ノードへのキーの追加
bool Tree::add(const int key_to_add) {
    // 追加先ノードの探索
    Node* target_node = search(key_to_add);

    // 追加先ノードに要素を追加可能なら、そのノードに追加
    if (target_node->is_able_to_add()) {
        target_node->add(key_to_add);
        return true;
    }

    // 以下、追加不可能の場合
    // 親ノードを分割
    
    // 分割後の左ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);

    // キーが入る場所を決定
    int key_index;
    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;
    for (int i=0; i<=target_node->count_keys(); i++) {
        if (i == target_node->count_keys()) {
            key_index = i;
            break;
        }
        right = target_node->get_key(i);

        if (left <= key_to_add && key_to_add < right) {
            key_index = i;
            break;
        }
        left = right;
    }
    // キーが左ノード、右ノードのどちらに入るか
    bool key_in_left = (key_index < left_hold);

    // 分割後のノードの用意
    Node *left_node, *right_node;
    // 左の葉ノードは現在のノードをそのまま利用
    left_node = target_node;
    // 右の葉ノードの用意
    right_node = new Node(m_node_size, nullptr);
    // 左ノードのleft_hold番目以降の要素を右ノードに移行
    for (int i=left_hold-key_in_left; i<left_node->count_keys(); i++) {
        right_node->add(left_node->get_key(i));
        std::cout << "move " << left_node->get_key(i) << std::endl;
    }
    // 右ノードに移行した子ノードは左ノードから削除
    for (int i=left_hold-key_in_left; i<left_node->count_keys(); i++) {
        left_node->del(i);
    }

    // 左ノードに右ノードへのポインタを設定
    left_node->set_right_node(right_node);

    // 新しいキーを登録
    // キーが入る場所(key_index)が左ノード内であれば、左ノードに挿入
    if (key_in_left) {
        left_node->add(key_to_add);
    }
    // キーが入る場所(key_index)が右ノード内であれば、右ノードに挿入
    else {
        right_node->add(key_to_add);
    }

    // 親ノードを取得
    Node* parent_node;
    // 左ノードが根ノードの場合は新しく根ノードを生成
    if (left_node->is_root_node()) {
        parent_node = new Node(m_node_size, left_node, right_node, nullptr);
        left_node->set_parent(parent_node);
        right_node->set_parent(parent_node);
        m_root_node = parent_node;
        return true;
    }
    parent_node = left_node->get_parent();

    // 親ノードの親ノードに右ノードを登録
    return m_add_child_node(parent_node, right_node);
}

// キーの探索
Node* Tree::search(const int key_to_search) {
    // ルートノードから順に探索
    Node* current_node = m_root_node;
    
    // 葉ノードにたどり着くまで探索
    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;
    while (!current_node->is_leaf_node()) {
        for (int i=0; i<=current_node->count_keys(); i++) {
            if (i == current_node->count_keys()) {
                // ノードの最後の要素にたどり着いたら、rightの値はintの最大値に
                right = std::numeric_limits<int>::max();
            }
            else {
                // そうでなければ、rightの値はi番目のキー値
                right = current_node->get_key(i);
            }

            if (left <= key_to_search && key_to_search < right) {
                current_node = current_node->get_child(i);
                break;
            }
        }
    }

    return current_node;
}

// 各ノードの表示
void Tree::print_tree() {
    m_root_node->print_keys(1);
}

// 葉以外のノードに子ノードを追加
bool Tree::m_add_child_node(Node* parent_node, Node* child_node) {
    // 親ノードに子ノードを追加可能であれば、追加して完了
    if (parent_node->is_able_to_add()) {
        parent_node->add(child_node);
        child_node->set_parent(parent_node);
        return true;
    }

    // 以下、追加不可能の場合
    // 親ノードを分割

    // 分割後の左ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);

    // キーが入る場所を決定
    int key_index;
    int key = child_node->get_min_key_recursive();
    int left = std::numeric_limits<int>::min();   // 左側の値の初期値はintの最小値に
    int right;
    for (int i=0; i<=parent_node->count_keys(); i++) {
        if (i == parent_node->count_keys()) {
            key_index = i;
            break;
        }
        right = parent_node->get_key(i);

        if (left <= key && key < right) {
            key_index = i;
            break;
        }
        left = right;
    }
    // 子ノードが左ノード、右ノードのどちらに入るか
    bool child_node_in_left = (key_index < left_hold);

    // 分割後のノードの用意
    Node *left_node, *right_node;
    // 左ノードは親ノードをそのまま利用
    left_node = parent_node;
    // 右ノードには生成と同時に先に子ノードを2つ登録
    right_node = new Node(m_node_size, left_node->get_child(left_hold), left_node->get_child(left_hold+1), nullptr);

    // 左ノードのleft_hold+2番目以降の要素を右ノードに移行
    for (int i=left_hold+2-child_node_in_left; i<left_node->count_keys(); i++) {
        left_node->get_child(i)->set_parent(right_node);
        right_node->add(left_node->get_child(i));
    }
    // 右ノードに移行した子ノードは左ノードから削除
    for (int i=left_node->count_keys(); i>=left_hold-child_node_in_left; i--) {
        left_node->del(i);
    }

    // 子ノードを登録
    // 子ノードが入る場所(key_index)が左ノード内であれば、左ノードに挿入
    if (child_node_in_left) {
        left_node->add(child_node);
        child_node->set_parent(left_node);
    }
    // 子ノードが入る場所(key_index)が右ノード内であれば、右ノードに挿入
    else {
        right_node->add(child_node);
        child_node->set_parent(right_node);
    }

    // 左ノードの親ノードを取得
    Node* left_parent_node;
    // 左ノードが根ノードの場合は新しく根ノードを生成
    if (left_node->is_root_node()) {
        left_parent_node = new Node(m_node_size, left_node, right_node, nullptr);
        left_node->set_parent(left_parent_node);
        right_node->set_parent(left_parent_node);
        m_root_node = left_parent_node;
        return true;
    }
    left_parent_node = left_node->get_parent();

    // 右ノードに親ノードを登録
    right_node->set_parent(left_parent_node);

    // 親ノードの親ノードに右ノードを登録
    return m_add_child_node(left_parent_node, right_node);
}

#include "tree.h"

// コンストラクタ
// ノードのサイズを設定し、最初の根ノードを生成
Tree::Tree() {
    // 最初のノード（根ノードかつ葉ノード）を生成
    m_root_node = new Node(M, nullptr, true);
}

// 葉ノードへのキーの追加
bool Tree::add(const int key_to_add, Node* node_to_add) {
    if (node_to_add == nullptr) {
        std::cout << "---------" << std::endl;
        std::cout << "add: " << key_to_add << std::endl;
    }

    // 追加先ノードを探索
    Node* target_node = search(key_to_add);

    return add(target_node, key_to_add, node_to_add);
}
bool Tree::add(Node* parent_node, const int key_to_add, Node* node_to_add) {
    // 追加先に追加可能であれば、キーを追加して終了
    if (parent_node->is_able_to_add()) {
        parent_node->add(key_to_add, node_to_add);
        return true;
    }

    // 追加不可能の場合、追加先ノードを分割
    Node* left_node = parent_node;
    Node* right_node = m_div(left_node, key_to_add, node_to_add);

    // 左ノードが根ノードであれば、新たに根ノードを生成
    if (left_node->is_root_node()) {
        Node* new_root_node = new Node(M, nullptr, false);
        new_root_node->add(0, left_node);
        new_root_node->add(right_node->get_min_key_recursive(), right_node);
        m_root_node = new_root_node;
        return true;
    }

    // 親ノードに右ノードを追加
    return add(left_node->get_parent(), right_node->get_min_key_recursive(), right_node);
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

// メモリ解放
void Tree::clear_tree() {
    m_root_node->clear_recursive();
}

// ノードを分割
// 戻り値は右側ノード（左側ノードは引数に与えられたノードを再利用）
Node* Tree::m_div(Node* left_node, int key_to_add, Node* node_to_add) {
    // 右ノードの生成
    Node* right_node = new Node(M, nullptr, left_node->is_leaf_node());

    // 左ノードの保持数
    int left_hold = ceil((double)(M+1)/2);
    // 右ノードの保持数
    int right_hold = floor((double)(M+1)/2);
    
    // 新しいキーがどちらに入るか
    int new_key_num;
    int left = std::numeric_limits<int>::min();
    int right;
    for (int i=0; i<=left_node->count_keys(); i++) {
        if (i == left_node->count_keys()) {
            right = std::numeric_limits<int>::max();
        }
        else {
            right = left_node->get_key(i);
        }

        if (left <= key_to_add && key_to_add < right) {
            new_key_num = i;
            break;
        }
    }
    int left_add = true;
    if (new_key_num >= left_hold) {
        left_add = false;
    }

    // 格納先は左側ノードを優先
    int left_count = left_node->count_keys();
    if (!left_add && left_count - left_hold > right_hold) {
        left_hold += 1;
    }

    // left_hold番目から先の要素を左ノードから取り出し、右ノードに移動
    for (int i=left_count-1; i>=left_hold-left_add; i--) {
        if (left_node->is_leaf_node()) {
            right_node->add(left_node->get_key(i), left_node->pull_child(i));
        }
        else {
            right_node->add(left_node->get_key(i), left_node->pull_child(i+1));
        }
    }

    // ノードの追加
    if (left_add) {
        left_node->add(key_to_add, node_to_add);
    }
    else {
        right_node->add(key_to_add, node_to_add);
    }

    // 隣接ノードの設定
    if (left_node->is_leaf_node()) {
        right_node->set_right_node(left_node->get_right_node());
        left_node->set_right_node(right_node);
    }

    return right_node;
}

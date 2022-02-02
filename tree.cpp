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
    Node* target_node = m_search_leaf_node(key_to_add);

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

// キーの削除
bool Tree::del(const int key_to_delete) {
    std::cout << "---------" << std::endl;
        std::cout << "delete: " << key_to_delete << std::endl;

    // 削除するキーを検索
    Node* target_node = m_search_leaf_node(key_to_delete);

    // キーがツリー上に存在しなければfalse
    if (!target_node->has_key(key_to_delete)) {
        return false;
    }

    return del(target_node, key_to_delete, nullptr);
}
bool Tree::del(Node* parent_node, int key_to_delete, Node* node_to_delete) {
    // ノードから除去
    if (parent_node->is_leaf_node()) {
        parent_node->del_key(key_to_delete);
    }
    else {
        parent_node->del_child(node_to_delete);
    }
    // 除去後、要素数が条件に合致するなら完了
    if (parent_node->is_ok(0)) {
        return true;
    }

    // 根ノードなら無条件に完了
    if (parent_node->is_root_node()) {
        return true;
    }

    // 要素数が条件に合致しないなら要素の移動やマージを行う
    std::cout << "A" << std::endl;
    // parent_nodeの隣接ノードを取得
    Node* next_node;
    bool next_is_right_node = true;
    // parent_nodeの部分木の最大値を持つ葉ノードから、右側の隣接ノードを取得
    Node* rightmost_leaf_node_next = m_search_leaf_node(parent_node->get_max_key_recursive())->get_right_node();

    // 右側の隣接ノードが取得できた場合、右側の隣接ノードと要素の移動やマージを行う
    if (rightmost_leaf_node_next != nullptr) {
        // 葉ノードの右側の隣接ノードから、parent_nodeと同じ高さの親ノードを取得
        next_node = rightmost_leaf_node_next;
        while (next_node->get_height() != parent_node->get_height()) {
            next_node = next_node->get_parent();
        }
    }
    // 右側の隣接ノードが取得できた場合、左側の隣接ノードと要素の移動やマージを行う
    else {
        // parent_nodeの部分木の最小値-1のキーが入り得るノードが、parent_nodeの左隣の部分木のうち最も右側の葉ノード
        Node* leftmost_leaf_node_prev = m_search_leaf_node(parent_node->get_min_key_recursive() - 1);
        next_node = leftmost_leaf_node_prev;
        next_is_right_node = false;
    }
    // 葉ノードの右側（あるいは左側）の隣接ノードから、parent_nodeと同じ高さの親ノードを取得
    while (next_node->get_height() != parent_node->get_height()) {
        next_node = next_node->get_parent();
    }

    // 移動する個数 = 足りない子ノードの個数
    int transfer_nodes;
    if (parent_node->is_leaf_node()) {
        transfer_nodes = parent_node->required() - parent_node->count_keys();
    }
    else {
        transfer_nodes = parent_node->required() - parent_node->count_children();
    }
std::cout << "B" << std::endl;
    // 隣接ノードからparent_nodeへ移動
    // 隣接ノードからtransfer_nodes個除去しても条件を満たすなら、隣接ノードから移動
    if (next_node->is_ok(-transfer_nodes)) {
        // 隣接ノードが右側の隣接ノードなら、前から順にtransfer_nodes個分取り出して移動
        if (next_is_right_node) {
            for (int i=0; i<transfer_nodes; i++) {
                if (parent_node->is_leaf_node()) {
                    int transfer_key = next_node->pull_key(0);
                    parent_node->add(transfer_key, nullptr);
                }
                else {
                    Node* transfer_node = next_node->pull_child(0);
                    parent_node->add(transfer_node->get_min_key_recursive(), transfer_node);
                }
            }
        }
        // 隣接ノードが左側の隣接ノードなら、うしろから順にtransfer_nodes個分取り出して移動
        else {
            if (parent_node->is_leaf_node()) {
                int init_next_node_keys = next_node->count_keys();
                for (int i=init_next_node_keys-1; i>init_next_node_keys-1-transfer_nodes; i--) {
                    int transfer_key = next_node->pull_key(i);
                    parent_node->add(transfer_key, nullptr);
                }
            }
            else {
                int init_next_node_keys = next_node->count_keys();
                for (int i=init_next_node_keys-1; i>=init_next_node_keys-1-transfer_nodes; i--) {
                    Node* transfer_node = next_node->pull_child(i);
                    parent_node->add(transfer_node->get_min_key_recursive(), transfer_node);
                }
            }
        }
        return true;
    }
std::cout << "C" << std::endl;
    // 隣接ノードから移動できない場合、マージする
    // 葉ノードの場合、parent_nodeの左隣のノードを右側のノードと隣接ノードとしてつなぐ
    if (parent_node->is_leaf_node() && !parent_node->is_root_node()) {
        Node* left_node = m_search_leaf_node(parent_node->get_min_key_recursive() - 1);
        Node* right_node = parent_node->get_right_node();
        left_node->set_right_node(right_node);
    }
std::cout << "D" << std::endl;
std::cout << "parent: " << parent_node << " next: " << next_node << std::endl;

    // マージ
    if (parent_node->is_leaf_node()) {
        for (int i=0; i<parent_node->count_keys(); i++) {
            next_node->add(parent_node->pull_key(0), nullptr);
        }
    }
    else {
        for (int i=0; i<parent_node->count_children(); i++) {
            std::cout << "append " << parent_node->get_key(0) << std::endl;
            std::cout << parent_node->get_child(0)->get_min_key_recursive() << std::endl;
            next_node->add(parent_node->get_key(0), parent_node->pull_child(0));
        }
    }
std::cout << "E" << std::endl;
    // parent_nodeの親ノードからparent_nodeを削除
    return del(parent_node->get_parent(), 0, parent_node);
}

// キーの検索と結果の表示
void Tree::search(const int key_to_search) {
    std::cout << std::endl << "----------------------" << std::endl << "search key " << key_to_search << std::endl;

    Node* current_node = m_root_node;
    int node_count = 1;

    while (current_node != nullptr) {
        int left = std::numeric_limits<int>::min();     // 最小値の初期値はintの最小値に
        int right;

        for (int i=0; i<=current_node->count_keys(); i++) {
            if (i == current_node->count_keys()) {
                right = std::numeric_limits<int>::max();
            }
            else {
                right = current_node->get_key(i);
            }
            if (left <= key_to_search && key_to_search < right) {
                std::cout << node_count << ") ";
                current_node->print_keys_once(0);

                if (i == 0) {
                    std::cout << std::setw(node_count) << "   " << key_to_search << " < " << right << std::endl;
                    if (!current_node->is_leaf_node()) {
                        std::cout << "    >> Go to 1st node" << std::endl;
                    }
                }
                else if (i == current_node->count_keys()) {
                    std::cout << std::setw(node_count) << "   " << left << " <= " << key_to_search << std::endl;
                    if (!current_node->is_leaf_node()) {
                        std::cout << "    >> Go to last node" << std::endl;
                    }
                }
                else {
                    std::cout << std::setw(node_count) << "   " << left << " <= " << key_to_search << " < " << right << std::endl;
                    if (!current_node->is_leaf_node()) {
                        std::cout << "    >> Go to " << (i+1) << "th node" << std::endl;
                    }
                }

                node_count += 1;
                current_node = current_node->get_child(i);

                if (current_node == nullptr) {
                    if (left == key_to_search) {
                        std::cout << "key " << key_to_search << " exists." << std::endl;
                    }
                    else {
                        std::cout << "key " << key_to_search << " not found." << std::endl;
                    }
                }

                break;
            }
            left = right;
        }
    }
    std::cout << "----------------------" << std::endl;

    return;
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

// キーが入る葉ノードを探索
Node* Tree::m_search_leaf_node(const int key_to_search) {
    // ルートノードから順に探索
    Node* current_node = m_root_node;
    Node* before_current_node;
    
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

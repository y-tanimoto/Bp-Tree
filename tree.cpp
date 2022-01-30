#include "tree.h"

// コンストラクタ
// ノードのサイズを設定し、最初の根ノードを生成
Tree::Tree(const int size) {
    m_node_size = size;

    // 最初のノード（根ノードかつ葉ノード）を生成
    m_root_node = new Node(size, nullptr, true);
}

// 葉ノードへのキーの追加
bool Tree::add(const int key_to_add) {
    std::cout << "----------------" << std::endl;
    std::cout << "add " << key_to_add << std::endl;
    // 追加先ノードの探索
    Node* target_node = search(key_to_add);

    // 追加先ノードに要素を追加可能なら、そのノードに追加
    if (target_node->is_able_to_add()) {
        target_node->add(key_to_add);
        return true;
    }

    // 以下、追加不可能の場合
    // 親ノードを分割

    // 左ノード、右ノードの用意（左ノードは追加先ノードを再利用）
    Node* left_node = target_node;
    Node* right_node = new Node(m_node_size, nullptr, true);

    // 左ノードの最後の要素を右ノードに移動
    right_node->add(left_node->pull_last_key());

    // 左ノードに新しいキーを追加
    left_node->add(key_to_add);

    // 分割後の左右ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);
    // 右：ROUNDDOWN((m+1)/2)個
    int right_hold = floor((m_node_size+1)/2);
    // 右ノードに多く追加される場合はleft_hold += 1
    if (left_node->count_keys() + 1 - left_hold > right_hold) {
        left_hold += 1;
    }

    // left_hold番目から先の要素は右ノードに移動
    for (int i=left_hold; i<left_node->count_keys(); i++) {
        std::cout << "add to right: " << i << std::endl;
        right_node->add(left_node->pull_key(i));
    }

    // 右ノードに隣接ノードへのポインタを設定
    right_node->set_right_node(left_node->get_right_node());

    // 左ノードに右ノードへのポインタを設定
    //left_node->set_right_node(right_node);

    // 親ノードを取得
    Node* parent_node;
    // 左ノードが根ノードの場合は新しく根ノードを生成
    if (left_node->is_root_node()) {
        parent_node = new Node(m_node_size, nullptr, false);
        parent_node->add(left_node);
        parent_node->add(right_node);
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
    if (parent_node->is_able_to_add_child()) {
        parent_node->add(child_node);
        child_node->set_parent(parent_node);
        return true;
    }

    // 以下、追加不可能の場合
    // 親ノードを分割

    // 左ノードの用意（左ノードは追加先ノードを再利用）
    Node* left_node = parent_node;
    // 右ノードの用意
    Node* right_node = new Node(m_node_size, nullptr, false);

    // 左ノードのもっと右側の子ノードは右ノードに移動
    right_node->add(left_node->pull_last_child());

    // 新しい子ノードを左ノードに追加
    left_node->add(child_node);

    // 分割後の左右ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);
    // 右：ROUNDDOWN((m+1)/2)個
    int right_hold = floor((m_node_size+1)/2);
    // 右ノードに多く追加される場合はleft_hold += 1
    if (left_hold == right_hold && left_node->count_children() + 1 - left_hold > right_hold) {
        left_hold += 1;
    }

    // left_hold番目以降の要素を左ノードから右ノードへ移動
    for (int i=left_hold; i<left_node->count_children(); i++) {
        left_node->get_child(i)->set_parent(right_node);
        right_node->add(left_node->pull_child(i));
    }

    std::cout << "fin" << std::endl;

    // 左ノードの親ノードを取得
    Node* left_parent_node;
    // 左ノードが根ノードの場合は新しく根ノードを生成
    if (left_node->is_root_node()) {
        left_parent_node = new Node(m_node_size, nullptr, false);
        left_parent_node->add(left_node);
        left_parent_node->add(right_node);
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

// メモリ解放
void Tree::clear_tree() {
    m_root_node->clear_recursive();
}

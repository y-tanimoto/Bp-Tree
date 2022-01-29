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

    // ソート用のノードを生成
    Node sort_node(m_node_size + 1, nullptr, true);
    // sort_nodeに左ノードのすべての要素をコピー
    for (int i=0; i<left_node->count_keys(); i++) {
        sort_node.add(left_node->get_key(i));
    }
    // 追加するキーをsort_nodeに追加
    sort_node.add(key_to_add);

    // 分割後の左右ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);
    // 右：ROUNDDOWN((m+1)/2)個
    int right_hold = floor((m_node_size+1)/2);
    // 右ノードに多く追加される場合はleft_hold += 1
    if (sort_node.count_keys() - left_hold > right_hold) {
        left_hold += 1;
    }

    // 左ノードを初期化
    left_node->clear();

    // ノードはキーの追加時にソートされるので、それを利用し前から順に取り出す
    for (int i=0; i<left_hold; i++) {
        std::cout << "add to left: " << i << std::endl;
        left_node->add(sort_node.get_key(i));
    }
    for (int i=left_hold; i<sort_node.count_keys(); i++) {
        std::cout << "add to right: " << i << std::endl;
        right_node->add(sort_node.get_key(i));
    }

    // 左ノードに右ノードへのポインタを設定
    left_node->set_right_node(right_node);

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
        std::cout << "changed root node" << std::endl;
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

    // 左ノードの用意（左ノードは追加先ノードを再利用）
    Node* left_node = parent_node;
    // 右ノードの用意
    Node* right_node = new Node(m_node_size, nullptr, false);

    // ソート用のノードを生成
    Node sort_node(m_node_size + 1, nullptr, false);
    // sort_nodeに左ノードのすべての要素をコピー(0,1番目は宣言時にコピー済み)
    for (int i=0; i<left_node->count_children(); i++) {
        sort_node.add(left_node->get_child(i));
        std::cout << "left child: " << left_node->get_child(i)->get_min_key_recursive() << std::endl;
    }
    // 追加するキーをsort_nodeに追加
    sort_node.add(child_node);

    // 分割後の左右ノードの保持数を計算
    // 左：ROUNDUP((m+1)/2)個
    int left_hold = ceil((m_node_size+1)/2);
    // 右：ROUNDDOWN((m+1)/2)個
    int right_hold = floor((m_node_size+1)/2);
    // 右ノードに多く追加される場合はleft_hold += 1
    std::cout << "m:" << sort_node.count_children() << std::endl;
    if (sort_node.count_children() - left_hold > right_hold) {
        left_hold += 1;
    }

    // 左ノードをクリア
    left_node->clear();

    // ノードはキーの追加時にソートされるので、それを利用し前から順に取り出す
    for (int i=0; i<left_hold; i++) {
        std::cout << "madd to left: " << i << " left-hold=" << left_hold << std::endl;
        std::cout << sort_node.get_child(i)->get_min_key_recursive() << std::endl;
        sort_node.get_child(i)->set_parent(left_node);
        left_node->add(sort_node.get_child(i));
    }
    for (int i=left_hold; i<sort_node.count_children(); i++) {
        std::cout << sort_node.get_child(i)->get_min_key_recursive() << std::endl;
        sort_node.get_child(i)->set_parent(right_node);
        right_node->add(sort_node.get_child(i));
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

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>

// ノードクラス
class Node {
public:
    // クラスの初期化
    Node(const int size, Node* parent_node_p) {
        m_size = size;

        // キー保持用領域の確保（キー値はNaNで初期化）
        m_keys_array = std::vector<int>(m_size, -1);

        // ポインタ保持用領域の確保（ポインタはnullptrで初期化）
        m_pointers_array = std::vector<Node*>(m_size+1, nullptr);

        // キー保持数を0にリセット
        m_total_keys = 0;
    }

    // 最小キー値を取得
    int get_min_key() {
        return m_keys_array[0];
    }

    // 最大キー値を取得
    int get_max_key() {
        return m_keys_array[m_total_keys-1];
    }

    // キー数を取得
    int get_total_keys() {
        return m_total_keys;
    }

    // キー数がtotal_keysに変化した場合に、B+-Treeの性質を満たすか否か
    bool isOk(const int total_keys) {
        // 根ノードの場合、total_keys <= m_sizeとなるか
        if (is_root()) {
            if (total_keys <= m_size) {
                return true;
            }
            else {
                return false;
            }
        }
        // 葉ノードの場合、total_keys >= ROUNDDOWN((m_size+1)/2)かつtotal_keys <= m_sizeとなるか
        else if (is_leaf()) {
            if (total_keys >= floor(m_size+1)/2 && total_keys <= m_size) {
                return true;
            }
            else {
                return false;
            }
        }
        // 根と葉以外のノードの場合、total_keys >= ROUNDUP((m_size+1)/2)かつtotal_keys <= m_sizeとなるか
        else {
            if (total_keys >= ceil(m_size+1)/2 && total_keys <= m_size) {
                return true;
            }
            else {
                return false;
            }
        }
    }

    // キー値の有無：存在すればtrue、存在しなければfalseを返す
    bool isExist(const int key_to_search) {
        // 最小値<=キー値<=最大値でなければそもそも存在しない
        if (get_min_key() > key_to_search || key_to_search > get_max_key()) {
            return false;
        }

        // 探索
        for (int i=0; i<m_total_keys; i++) {
            if (key_to_search == m_keys_array[i]) {
                std::cout << m_keys_array[i] << std::endl;
                return true;
            }
        }
        // キーが存在しない
        return false;
    }

    // キー値の検索：何番目に存在するかを返す
    // キーがノードに存在しなければNaNを返す
    int get_key_position(const int key_to_search) {
        // 探索
        for (int i=0; i<m_total_keys; i++) {
            if (key_to_search == m_keys_array[i]) {
                // キーがi番目に存在する
                return i;
            }
        }
        // キーが存在しない
        return NAN;
    }

    // 根ノードか否か
    bool is_root() {
        // 親ノードが存在しなければ根ノードと判断
        if (m_parent_node_p == nullptr) {
            return true;
        }
        return false;
    }

    // 葉ノードか否か
    bool is_leaf() {
        // 最初の要素のポインタがnullptrなら葉ノードと判断
        if (m_pointers_array[0] == nullptr) {
            return true;
        }
        return false;
    }

    // num番目のキー値の取得
    int get_key(const int num) {
        return m_keys_array[num];
    }

    // num番目のポインタの取得
    Node* get_node_p(const int num) {
        return m_pointers_array[num];
    }

    // 親ノードの取得
    Node* get_parent_node() {
        return m_parent_node_p;
    }

    // 親ノードの設定
    void set_parent_node(Node* parent_node_p) {
        m_parent_node_p = parent_node_p;
    }

    // ノードへのキーの追加
    // 追加可能であればtrue、追加不可能であればfalseを返す
    bool add_key(const int key_to_add, Node* node_p_to_add) {
        // 要素数を超えてしまう場合は追加不可能（=分割の必要あり）
        if (m_total_keys + 1 > m_size) {
            return false;
        }

        // 以下は追加可能の場合
        // 要素数0なら最初の要素に追加
        if (m_total_keys == 0) {
            m_keys_array[0] = key_to_add;
            m_pointers_array[0] = node_p_to_add;
            m_total_keys += 1;
            return true;
        }

        // 値が入る場所を探索
        for (int i=0; i<m_total_keys; i++) {
            // i番目のキー値==追加するキー値であれば
            // 追加するキー値が既存のキー値と重複するため、子ノードへのポインタのみ書き換え
            if (m_keys_array[i] == key_to_add) {
                m_pointers_array[i] = node_p_to_add;
                return true;
            }
            // i番目のキー値<追加するキー値であれば
            else if (m_keys_array[i] < key_to_add) {
                std::cout << "Add" << key_to_add << std::endl;
                // i番目がノードの末尾要素であれば最後の要素に追加
                if (i+1 == m_total_keys) {
                    m_set(i+1, key_to_add, node_p_to_add);
                    return true;
                }
                // 追加するキー値<=次のキー値であればi+1番目に挿入
                if (m_keys_array[i+1] >= key_to_add) {
                    m_slide_back(i+1);                          // ずらして
                    m_set(i+1, key_to_add, node_p_to_add);      // 挿入
                    return true;
                }
            }
            // i番目のキー値>追加するキー値であれば
            if (m_keys_array[i] > key_to_add) {
                // i==0なら最初の要素にキーを挿入
                if (i == 0) {
                    m_slide_back(0);                            // ずらして
                    m_set(0, key_to_add, node_p_to_add);        // 挿入
                    return true;
                }
            }
        }

        return false;
    }

    // ノードからのキー値の削除
    // 削除可能であればtrue、削除不可能であればfalseを返す
    bool delete_key(const int key_to_delete) {
        // キー値がノードに存在しなければfalse
        if (!isExist(key_to_delete)) {
            return false;
        }
        std::cout << key_to_delete << std::endl;

        // 以下、キーがノードに存在する場合
        // キー値の場所を検索
        int key_position = get_key_position(key_to_delete);

        // キーを削除後にB+-Treeの性質を満たさなくなる場合はfalse
        if (!isOk(m_total_keys-1)) {
            return false;
        }

        // 削除するキーから後ろのキーを1つずつ前へずらす
        m_slide_front(key_position);

        m_total_keys -= 1;

        return true;
    }

    // ノードの表示
    void print_node() {
        for (int i=0; i<m_total_keys; i++) {
            std::cout << "[" << m_keys_array[i] << "]";
        }
        std::cout << std::endl;
    }

private:
    int m_size;         // ノードが持つ最大キー数
    int m_total_keys;   // ノードが持つ現在のキー数

    Node* m_parent_node_p;      // 親ノードへのポインタ（根ノードならnullptr）

    std::vector<int> m_keys_array;          // キー保持用配列
    std::vector<Node*> m_pointers_array;    // ポインタ保持用配列

    // キー値のセット
    void m_set(const int num, const int key_to_add, Node* node_p_to_add) {
        // num番目を新たなキー値に置き換え
        m_keys_array[num] = key_to_add;
        m_pointers_array[num] = node_p_to_add;
        m_total_keys += 1;
    }

    // num番目から後ろのキーを1個後ろへずらす
    void m_slide_back(const int num) {
        // num+1番目〜最後の要素までをずらす
        for (int j=m_total_keys; j>=num; j--) {
            m_keys_array[j] = m_keys_array[j-1];
            m_pointers_array[j] = m_pointers_array[j-1];
        }
    }

     // num番目から後ろのキーを1個前へずらす
    void m_slide_front(const int num) {
        // num+1番目〜最後の要素までをずらす
        for (int j=num; j<=m_total_keys-1; j++) {
            m_keys_array[j] = m_keys_array[j+1];
            m_pointers_array[j] = m_pointers_array[j+1];
        }

        // 最後の要素は初期化
        m_keys_array[m_total_keys-1] = NAN;
        m_pointers_array[m_total_keys-1] = nullptr;
    }
};

// ツリークラス
class BpTree {
public:
    // 初期化：根ノードの生成
    BpTree(const int size) {
        m_node_size = size;
        m_root_node = new Node(m_node_size, nullptr);
    }

    // キーの追加
    bool add_key(const int key_to_add) {
        // キーが追加されうるノードを探索
        Node* current_node = m_search_node_recursive(key_to_add, m_root_node);
        if (current_node == nullptr) {
            return false;
        }

        // キー追加
        // ノードにキーを追加できたら完了
        if (current_node->add_key(key_to_add, nullptr)) {
            current_node->print_node();
            return true;
        }
        // 追加できなかった場合、追加先ノードが満杯
        // -> ノードを分割し親ノードに追加
        else {
            // 分割後の左ノードの要素数
            int left_node_keys = ceil((m_node_size + 1) / 2);
            // 分割後の右ノードの要素数
            int right_node_keys = floor((m_node_size + 1) / 2);

            // 現在のノードが根ノードであれば、新たに根ノードを生成
            if (current_node->is_root()) {
                Node new_root_node(m_node_size, nullptr);
                current_node->set_parent_node(&new_root_node);
            }

            // 分割後のノードの生成
            Node left_node(m_node_size, current_node->get_parent_node());
            Node right_node(m_node_size, current_node->get_parent_node());

            // 小さい値から順にleft_node_keys個分を左ノードに格納
            for (int i=0; i<left_node_keys; i++) {
                left_node.add_key(current_node->get_key(i), current_node->get_node_p(i));
            }
            // 残りのキーは右ノードに格納
            for (int i=left_node_keys; i<current_node->get_total_keys(); i++) {
                right_node.add_key(current_node->get_key(i), current_node->get_node_p(i));
            }

            // 親ノードから現在のノードを削除
            

            // 親ノードに分割後のノードを登録

        }

        return true;
    }

private:
    int m_node_size;        // 各ノードのサイズ

    Node* m_root_node;      // 根ノードのポインタ

    // ノード検索（再帰関数）
    Node* m_search_node_recursive(const int key_to_search, Node* current_node_p) {
        // 葉ノードならこのノードを返す
        if (current_node_p->is_leaf()) {
            return current_node_p;
        }

        // キーと合致する値が現在のノードに存在しない場合
        // キー値が値域に入るノードを子ノードから探索
        for (int i=0; i<current_node_p->get_total_keys(); i++) {
            if (current_node_p->get_key(i) <= key_to_search &&
                key_to_search <= current_node_p->get_key(i)) {
                
                // 再帰
                return m_search_node_recursive(key_to_search, current_node_p->get_node_p(i));
            }
        }

        return nullptr;
    }
};

int main() {
    BpTree bp_tree(3);
    std::cout << bp_tree.add_key(3) << std::endl;
    std::cout << bp_tree.add_key(3) << std::endl;
    std::cout << bp_tree.add_key(4) << std::endl;
    std::cout << bp_tree.add_key(1) << std::endl;

    printf("Hello\n");
}

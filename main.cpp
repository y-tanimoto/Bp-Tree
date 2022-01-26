#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <cmath>

// ノードクラス
class Node {
public:
    // クラスの初期化
    Node(const int size, const bool is_root, const bool is_leaf) {
        m_size = size;
        m_is_root = is_root;
        m_is_leaf = is_leaf;

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
        // 葉ノードの場合、total_keys >= ROUNDDOWN((m_size+1)/2)かつtotal_keys <= m_sizeとなるか
        if (m_is_leaf) {
            if (total_keys >= floor(m_size+1)/2 && total_keys <= m_size) {
                return true;
            }
            else {
                return false;
            }
        }
        // 根ノードの場合はtotal_keys <= m_sizeとなるか
        else if (m_is_root) {
            if (total_keys <= m_size) {
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
            // i番目のキー値≦追加するキー値であれば
            if (m_keys_array[i] <= key_to_add) {
                // i番目がノードのサイズ-1番目であれば最後の要素に追加
                if (i == m_size - 2) {
                    m_set(i+1, key_to_add, node_p_to_add);
                    return true;
                }
                // 追加するキー値≦次のキー値であればi+1番目に挿入
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

    bool m_is_root;     // このノードが根ノードか否か
    bool m_is_leaf;     // このノードが葉ノードか否か

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

int main() {
    Node node(3, false, false);

    std::cout << node.add_key(3, nullptr) << std::endl;
    std::cout << node.add_key(2, nullptr) << std::endl;
    std::cout << node.add_key(1, nullptr) << std::endl;
    
    node.print_node();

    std::cout << node.delete_key(4) << std::endl;
    node.print_node();

    std::cout << node.delete_key(2) << std::endl;
    node.print_node();

    std::cout << node.delete_key(1) << std::endl;
    node.print_node();

    std::cout << node.delete_key(3) << std::endl;
    node.print_node();

    printf("Hello\n");
}

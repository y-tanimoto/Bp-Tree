#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// ノードクラス
class Node {
public:
    // クラスの初期化
    Node(const int size) {
        m_size = size;

        // キー保持用領域の確保（キー値はNaNで初期化）
        m_keys_array = std::vector<int>(m_size, NAN);

        // ポインタ保持用領域の確保（ポインタはnullptrで初期化）
        m_pointers_array = std::vector<Node*>(m_size+1, nullptr);

        // キー保持数を0にリセット
        m_total_keys = 0;
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
                    m_insert(i+1, key_to_add, node_p_to_add);
                    return true;
                }
            }
            // i番目のキー値>追加するキー値であれば
            if (m_keys_array[i] > key_to_add) {
                // i==0なら最初の要素にキーを挿入
                if (i == 0) {
                    m_insert(0, key_to_add, node_p_to_add);
                    return true;
                }
            }
        }

        return false;
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

    std::vector<int> m_keys_array;          // キー保持用配列
    std::vector<Node*> m_pointers_array;    // ポインタ保持用配列

    // キー値のセット
    void m_set(const int num, const int key_to_add, Node* node_p_to_add) {
        // num番目を新たなキー値に置き換え
        m_keys_array[num] = key_to_add;
        m_pointers_array[num] = node_p_to_add;
        m_total_keys += 1;
    }

    // キーを特定の位置(num)に挿入
    // num : キーを挿入する位置
    void m_insert(const int num, const int key_to_add, Node* node_p_to_add) {
        // num+1番目〜最後の要素までをずらす
        for (int j=m_total_keys; j>=num+1; j--) {
            m_keys_array[j] = m_keys_array[j-1];
            m_pointers_array[j] = m_pointers_array[j-1];
        }

        m_set(num, key_to_add, node_p_to_add);
    }
};

int main() {
    Node node(3);

    std::cout << node.add_key(3, nullptr) << std::endl;
    std::cout << node.add_key(3, nullptr) << std::endl;
    std::cout << node.add_key(2, nullptr) << std::endl;
    
    node.print_node();

    printf("Hello\n");
}

#include "tree.h"

int main() {
    Tree bp_tree;
    bp_tree.add(2, nullptr);
    bp_tree.print_tree();
    bp_tree.add(3, nullptr);
    bp_tree.print_tree();
    bp_tree.add(7, nullptr);
    bp_tree.print_tree();
    bp_tree.add(11, nullptr);
    bp_tree.print_tree();
    bp_tree.add(5, nullptr);
    bp_tree.print_tree();
    bp_tree.add(19, nullptr);
    bp_tree.print_tree();
    bp_tree.add(13, nullptr);
    bp_tree.print_tree();
    bp_tree.add(17, nullptr);
    bp_tree.print_tree();
    bp_tree.add(23, nullptr);
    bp_tree.print_tree();
    bp_tree.add(29, nullptr);
    bp_tree.print_tree();
    bp_tree.add(31, nullptr);
    bp_tree.print_tree();
    bp_tree.add(37, nullptr);
    bp_tree.print_tree();
    bp_tree.add(43, nullptr);
    bp_tree.print_tree();
    bp_tree.add(47, nullptr);
    bp_tree.print_tree();
    bp_tree.add(41, nullptr);
    bp_tree.print_tree();
    bp_tree.add(10, nullptr);
    bp_tree.print_tree();
    bp_tree.add(40, nullptr);
    bp_tree.print_tree();

    bp_tree.del(29);
    bp_tree.print_tree();
    bp_tree.del(7);
    bp_tree.print_tree();
    bp_tree.del(10);
    bp_tree.print_tree();
    bp_tree.del(2);
    bp_tree.print_tree();
    bp_tree.del(3);
    bp_tree.print_tree();
    bp_tree.del(19);
    bp_tree.print_tree();
    bp_tree.del(23);
    bp_tree.print_tree();
    bp_tree.del(5);
    bp_tree.print_tree();

    bp_tree.del(11);
    bp_tree.print_tree();
    bp_tree.del(11);
    bp_tree.print_tree();
    bp_tree.del(13);
    bp_tree.print_tree();
    bp_tree.del(17);
    bp_tree.print_tree();
    bp_tree.del(37);
    bp_tree.print_tree();
    bp_tree.del(41);
    bp_tree.print_tree();
    bp_tree.del(40);
    bp_tree.print_tree();
    bp_tree.del(55);
    bp_tree.print_tree();
    bp_tree.del(43);
    bp_tree.print_tree();
    bp_tree.del(47);
    bp_tree.print_tree();
    bp_tree.del(31);
    bp_tree.print_tree();

    // キーの検索
    bp_tree.search(23);
    bp_tree.search(24);
    bp_tree.search(30);
    bp_tree.search(31);

    // メモリ解放
    bp_tree.clear_tree();

    return 0;
}

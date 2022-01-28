#include "tree.h"

int main() {
    Tree bp_tree(3);

    bp_tree.add(3);
    bp_tree.add(2);
    bp_tree.add(1);

    bp_tree.add(4);
    bp_tree.add(7);
    bp_tree.add(8);
    bp_tree.print_tree();

    return 0;
}

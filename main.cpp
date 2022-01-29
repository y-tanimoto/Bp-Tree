#include "tree.h"

int main() {
    Tree bp_tree(3);

    bp_tree.add(3);
    bp_tree.add(2);
    bp_tree.add(1);

    bp_tree.add(4);
    bp_tree.add(7);
    bp_tree.add(8);
    bp_tree.add(5);
    bp_tree.add(6);
    bp_tree.print_tree();
    
    bp_tree.add(10);
    bp_tree.add(11);
    bp_tree.print_tree();

    return 0;
}

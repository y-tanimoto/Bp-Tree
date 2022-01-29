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

    bp_tree.add(9);
    bp_tree.add(13);
    bp_tree.print_tree();
    bp_tree.add(12);
    bp_tree.add(0);
    bp_tree.add(14);
    bp_tree.add(15);
    bp_tree.add(16);
    bp_tree.add(17);
    bp_tree.add(18);
    bp_tree.add(19);
    bp_tree.add(20);
    bp_tree.add(21);
    bp_tree.add(22);
    bp_tree.add(23);
    bp_tree.add(24);
    bp_tree.print_tree();

    return 0;
}

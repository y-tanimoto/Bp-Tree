#include "tree.h"

int main() {
    Tree bp_tree(3);

    bp_tree.add(2);
    bp_tree.print_tree();
    bp_tree.add(3);
    bp_tree.print_tree();
    bp_tree.add(7);
    bp_tree.print_tree();
    bp_tree.add(11);
    bp_tree.print_tree();
    bp_tree.add(13);
    bp_tree.print_tree();
    bp_tree.add(17);
    bp_tree.print_tree();
    bp_tree.add(23);
    bp_tree.print_tree();
    bp_tree.add(29);
    bp_tree.print_tree();
    bp_tree.add(31);
    bp_tree.print_tree();
    bp_tree.add(37);
    bp_tree.print_tree();
    bp_tree.add(43);
    bp_tree.print_tree();
    bp_tree.add(47);
    bp_tree.print_tree();
    bp_tree.add(5);
    bp_tree.print_tree();
    bp_tree.add(19);
    bp_tree.print_tree();
    bp_tree.add(41);
    bp_tree.print_tree();
    bp_tree.add(43);
    bp_tree.print_tree();
    bp_tree.add(47);
    bp_tree.print_tree();

    bp_tree.add(10);
    bp_tree.print_tree();

    bp_tree.add(40);
    bp_tree.print_tree();

    return 0;
}

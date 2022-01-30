#include "node.h"

int main() {
    Node root_node(true, nullptr);

    root_node.add(2, nullptr);
    root_node.add(3, nullptr);
    root_node.add(1, nullptr);
    root_node.del(2);

    root_node.print_node();

    return 0;
}

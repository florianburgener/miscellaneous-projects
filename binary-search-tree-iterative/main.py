class Node:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

    def _search(self, value):
        parent = None
        node = self

        while node and node.value != value:
            parent = node
            node = node.left if value < node.value else node.right

        return parent, node

    def insert(self, value):
        parent, _ = self._search(value)

        if value < parent.value:
            parent.left = Node(value)
        else:
            parent.right = Node(value)

    def search(self, value):
        _, node = self._search(value)
        return node is not None

    def _find_largest_value(self):
        node = self

        while node.right:
            node = node.right

        return node.value

    def delete(self, value):
        parent, node = self._search(value)

        if not node:
            return self

        if node.left and node.right:
            largest_value = node.left._find_largest_value()
            replacement = node
            replacement.delete(largest_value)
            replacement.value = largest_value
        elif node.left:
            replacement = node.left
        elif node.right:
            replacement = node.right
        else:
            replacement = None

        if not parent:
            return replacement

        if value < parent.value:
            parent.left = replacement
        else:
            parent.right = replacement

        return self

    def __str__(self):
        result = ""
        nodes = [(self, 0)]

        while nodes:
            node, depth = nodes.pop()

            if not node:
                continue

            result += f"{depth * 2 * ' '}{node.value}\n"

            nodes.append((node.right, depth + 1))
            nodes.append((node.left, depth + 1))

        # Removes the extra newline character.
        result = result[:-1]
        return result


class BinarySearchTree:
    def __init__(self):
        self.root = None

    def insert(self, value):
        if not self.root:
            self.root = Node(value)
        else:
            self.root.insert(value)

    def search(self, value):
        if not self.root:
            return False
        return self.root.search(value)

    def delete(self, value):
        self.root = self.root.delete(value)

    def __str__(self):
        if not self.root:
            return "Empty"
        return str(self.root)


def main():
    tree = BinarySearchTree()
    tree.insert(1000)
    tree.insert(500)
    tree.insert(1500)
    tree.insert(250)
    tree.insert(750)
    tree.insert(1750)
    tree.insert(1250)
    tree.insert(1125)
    tree.insert(1375)
    tree.insert(875)
    tree.insert(1450)
    print(tree)
    print(tree.search(1000))
    print(tree.search(1375))
    print(tree.search(750))
    print(tree.search(125))
    print("---")
    print("root.delete(125)")
    tree.delete(125)
    print(tree)
    print("---")
    print("root.delete(1375)")
    tree.delete(1375)
    print(tree)
    print("---")
    print("root.delete(750)")
    tree.delete(750)
    print(tree)
    print("---")
    print("root.delete(1500)")
    tree.delete(1500)
    print(tree)
    print("---")
    print("root.delete(1000)")
    tree.delete(1000)
    print(tree)

    # Binary search tree with a single node.
    print("---")
    tree = BinarySearchTree()
    tree.insert(150)
    tree.delete(150)
    print(tree)


if __name__ == "__main__":
    main()

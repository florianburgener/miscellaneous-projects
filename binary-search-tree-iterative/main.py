class Node:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

    def insert(self, value):
        current = self

        while value != current.value:
            if value < current.value:
                if current.left is None:
                    current.left = Node(value)
                    return
                current = current.left
            else:
                if current.right is None:
                    current.right = Node(value)
                    return
                current = current.right

    def search(self, value):
        current = self

        while current is not None:
            if value == current.value:
                return True
            if value < current.value:
                current = current.left
            else:
                current = current.right

        return False

    def _find_largest_value(self):
        current = self

        while current.right is not None:
            current = current.right

        return current.value

    def delete(self, value):
        parent = None
        current = self

        while current is not None and value != current.value:
            parent = current

            if value < current.value:
                current = current.left
            else:
                current = current.right

        # The value to be deleted does not exist in the tree.
        if current is None:
            return self

        if current.left is not None and current.right is not None:
            largest_value = current.left._find_largest_value()
            replacement = current
            replacement.delete(largest_value)
            replacement.value = largest_value
        elif current.left is not None:
            replacement = current.left
        elif current.right is not None:
            replacement = current.right
        else:
            replacement = None

        if parent is None:
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
        if self.root is None:
            self.root = Node(value)
        else:
            self.root.insert(value)

    def search(self, value):
        if self.root is None:
            return False
        return self.root.search(value)

    def delete(self, value):
        self.root = self.root.delete(value)

    def __str__(self):
        if self.root is None:
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

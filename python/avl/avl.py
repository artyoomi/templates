from typing import List, Optional
import queue
import copy

class AVL:
    class Node:
        """Node for AVL tree class implementation"""
        def __init__(self, key: int, left: Optional['AVL.Node'] = None, right: Optional['AVL.Node'] = None):
            self.key = key
            self.left = left
            self.right = right
            self.height = 1

    def __init__(self):
        self._root   = None
        
        # Used to access size in O(1)
        self._size = 0

    #=========================#
    # CLASS INTERFACE METHODS #
    #=========================#
    def height(self) -> int:
        """Get height of tree"""
        return self._height(self._root)

    def insert(self, key: int) -> None:
        """Insert new element in tree"""
        if key < 0:
            raise ValueError("Element of tree must be natural number!")

        self._root = self._run_insert(self._root, key)

    def remove(self, key: int) -> None:
        """Remove specified element from tree"""
        self._root = self._run_remove(self._root, key)

    def remove_min(self) -> None:
        """Remove min element from tree"""
        self._root = self._run_remove_min(self._root)

    def remove_max(self) -> None:
        """Remove max element from tree"""
        self._root = self._run_remove_max(self._root)

    def min(self) -> int:
        """Get min element in tree"""
        min_node = self._min(self._root)
        if min_node is None:
            raise ValueError("AVL tree is empty!")
        
        return min_node.key

    def max(self) -> int:
        """Get max element in tree"""
        max_node = self._max(self._root)
        if max_node is None:
            raise ValueError("AVL tree is empty!")
        
        return max_node.key

    def data(self, order: str="in") -> List[int]:
        """Get elements of tree in specified order"""
        data = []
        if order == "in":
            self._get_in_order(self._root, data)
        elif order == "pre":
            self._get_pre_order(self._root, data)
        elif order == "post":
            self._get_post_order(self._root, data)
        elif order == "width":
            self._get_width_traversal(self._root, data)
        else:
            raise ValueError("Unknown traversal order!")
        return data

    def raw(self) -> Optional[Node]:
        """Get raw pointer to tree root"""
        return self._root

    def count(self, key: int) -> int:
        """Count amount elements with key `key` in tree"""
        return self._run_count(self._root, key)

    def size(self) -> int:
        """Return size of tree"""
        return self._size

    def split(self, key: int) -> (Optional['AVL'], Optional['AVL']):
        """Splits tree at given key"""
        avl_copy = copy.deepcopy(self)
        
        left_tree, right_tree = AVL(), AVL()
        left, right = self._run_split(avl_copy._root, key)

        # Update root pointers
        left_tree._root, right_tree._root = left, right

        return (left_tree, right_tree)

    def validate(self):
        """Validate tree structure"""
        return self._run_validate_AVL_BST(self._root) and \
               self._size == len(self.data())

    def clear(self) -> None:
        """Empty tree"""
        self._size = 0
        self._root = None
        self._run_clear(self._root)

    #=======================#
    # CLASS BACKEND METHODS #
    #=======================#
    def _height(self, node: Optional[Node]) -> int:
        """Backend function to get height of tree"""
        if node is None:
            return 0

        return node.height

    def _recalc_height(self, node: Node) -> None:
        """Function to recalculate height of specified node"""
        if node is None:
            return
        
        node.height = 1 + max(self._height(node.left),
                              self._height(node.right))

    def _calc_bfactor(self, node: Node) -> int:
        """Function to calculate balance factor of current node"""
        if node is None:
            return 0
        return self._height(node.right) - self._height(node.left)

    def _run_left_rotation(self, rotate_root: Node) -> Node:
        """
        Function to do left rotation in rotate_node.
 
        We have tree like this (balance factor in brackets):
        A(2)                B(-1)
          \                /    \
          B(0)     ->    A(1)  C(0) 
          /  \             \
        D(0)  C(0)        D(0)
        
        Logic of rotation:
        1. B - new root
        2. A - new left of B
        
        If B have left children, then this children become new A right subtree
        """
        
        new_root = rotate_root.right
        old_new_root_left = new_root.left

        new_root.left = rotate_root
        new_root.left.right = old_new_root_left

        self._recalc_height(rotate_root)
        self._recalc_height(new_root)

        return new_root

    def _run_right_rotation(self, rotate_root: Node) -> Node:
        """
        Do right rotation in rotate_node
        
        Same logic as for left rotation, but mirrored
        """
        
        new_root = rotate_root.left
        old_new_root_right = new_root.right

        new_root.right = rotate_root
        new_root.right.left = old_new_root_right

        self._recalc_height(rotate_root)
        self._recalc_height(new_root)

        return new_root

    def _run_balancing(self, rotate_node: Node) -> Node:
        """Function to do left rotation in `rotate_node`"""
        bfactor = self._calc_bfactor(rotate_node)
        
        # Update height for each node
        self._recalc_height(rotate_node)

        # AVL tree is balanced, we leave it as it is
        if -2 < bfactor < 2:
            return rotate_node

        if bfactor >= 2:
            # Left rotation
            if self._calc_bfactor(rotate_node.right) >= 0:
                return self._run_left_rotation(rotate_node)
            # Right-Left rotation
            else:
                rotate_node.right = self._run_right_rotation(rotate_node.right)
                return self._run_left_rotation(rotate_node)
        else:
            # Right rotation
            if self._calc_bfactor(rotate_node.left) <= 0:
                return self._run_right_rotation(rotate_node)
            # Left-Right rotation
            else:
                rotate_node.left = self._run_left_rotation(rotate_node.left)
                return self._run_right_rotation(rotate_node)

    def _run_insert(self, node: Optional[Node], key: int) -> Node:
        """Function to insert node in AVL tree with root in `node`"""
        if node is None:
            self._size += 1
            return self.Node(key)

        if key < node.key:
            node.left = self._run_insert(node.left, key)
        else:
            node.right = self._run_insert(node.right, key)

        return self._run_balancing(node)

    def _min(self, node: Optional[Node]) -> Optional[Node]:
        """Function to find min in AVL tree with root in `node`"""
        if node is None:
            return None

        current = node
        while current.left is not None:
            current = current.left

        return current

    def _max(self, node: Optional[Node]) -> Optional[Node]:
        """Function to find min in AVL tree with root in `node`"""
        if node is None:
            return None

        current = node
        while current.right is not None:
            current = current.right

        return current

    def _run_remove(self, node: Optional[Node], key: int) -> Optional[Node]:
        """Function to remove node in AVL tree with root in `node`"""
        if node is None:
            return None

        if key < node.key:
            node.left = self._run_remove(node.left, key)
        elif key > node.key:
            node.right = self._run_remove(node.right, key)
        else:
            self._size -= 1
            
            if node.left is None:
                return node.right
            elif node.right is None:
                return node.left
            else:
                tmp_key = self._min(node.right).key
                node.key = tmp_key
                node.right = self._run_remove(node.right, tmp_key)

        return self._run_balancing(node)

    def _run_remove_min(self, node: Optional[Node]) -> Optional[Node]:
        """Remove min node in AVL tree with root in `node`"""
        min_node = self._min(node)
        # if min_node is not None:
        #     return self._run_remove(node, min_node.key)

        # return node
        return self._run_remove(min_node, min_node.key)

    def _run_remove_max(self, node: Optional[Node]) -> Optional[Node]:
        """Remove max node in AVL tree with root in `node`"""
        max_node = self._max(node)
        # if max_node is not None:
        #     return self._run_remove(node, max_node.key)
        
        # return node
        return self._run_remove(max_node, max_node.key)

    def _run_search(self, node: Optional[Node], key: int) -> Optional[Node]:
        """Function to search node with `key` in AVL tree which root in `node`"""
        if node is None:
            return None

        found_node = None

        if key < node.key:
            found_node = self._run_search(node.left, key)
        elif key > node.key:
            found_node = self._run_search(node.right, key)
        else:
            return True

        return found_node
    
    def _run_count(self, node: Optional[Node], key: int) -> int:
        """Count amount of elements with key `key` in tree"""
        count = 0

        if node is not None:
            if node.key == key:
                count += 1

                """
                If key is equal to node.left.key, then this keys
                can't be placed in node.right subtree and node.left.left
                subtree, so all this values will be in node.left.right subtree
                """
                if node.left is not None and node.left.key == key:
                    count = count + 1 + \
                            self._run_count(node.left.left, key) + \
                            self._run_count_size(node.left.right)
                else:
                    count = count + self._run_count(node.left, key)

                """
                Same case as earlier but mirrored
                """
                if node.right is not None and node.right.key == key:
                    count = count + 1 + \
                            self._run_count(node.right.right, key) + \
                            self._run_count_size(node.right.left)
                else:
                    count = count + self._run_count(node.right, key)

            # Go to the right subtree if key > node.key
            elif node.key < key:
                count = self._run_count(node.right, key)
            # Go to the left subtree if key < node.key
            elif node.key > key:
                count = self._run_count(node.left, key)

        return count

    def _run_count_size(self, node: Node) -> int:
        """Recursively count amount of nodes in tree"""
        if node is None:
            return 0

        count = 1
        for child in [node.left, node.right]:
            count += self._run_count_size(child)

        return count

    def _run_deepcopy(self, node: Optional[Node]) -> Optional[Node]:
        """Helper method to copy each node recursively"""
        if node is None:
            return None

        # Copy current node and recursively copy it's subtrees
        new_node        = self.Node(key=node.key)
        new_node.height = node.height
        new_node.left   = self._run_deepcopy(node.left)
        new_node.right  = self._run_deepcopy(node.right)

        return new_node

    def _run_split(self, node: Optional[Node], key: int) -> ('AVL', 'AVL'):
        """Helper for split tree at given key"""
        # Base case
        if node is None:
            return None, None

        # Target key is in left subtree
        if key < node.key:
            left_tree, node.left = self._run_split(node.left, key)
            node = self._run_balancing(node)
            return left_tree, node
        # Target node is in right subtree
        elif key > node.key:
            node.right, right_tree = self._run_split(node.right, key)
            node = self._run_balancing(node)
            return node, right_tree
        # Target node was found
        else:
            left_tree = node.left
            right_tree = node.right
            node.left, node.right = None, None
            return left_tree, right_tree
            

    def _run_validate_AVL_BST(self, node: Optional[Node]) -> bool:
        """Validate BST property"""
        # Empty tree = correct BST 
        if node is None:
            return True
        # Left child has greater value than parent
        if node.left is not None and node.key < node.left.key:
            return False
        # Right child has less value than parent
        if node.right is not None and node.key > node.right.key:
            return False
        if abs(self._calc_bfactor(node)) >= 2:
            return False

        # Rescursively check BST structure
        return self._run_validate_AVL_BST(node.left) and \
               self._run_validate_AVL_BST(node.right)
    
    def _run_clear(self, node: Optional[Node]) -> None:
        """Clear all tree with root in `node`"""
        if node is None:
            return
        
        self._run_clear(node.left)
        self._run_clear(node.right)

        node.left  = None
        node.right = None
        node       = None

    #=================#
    # TREE TRAVERSALS #
    #=================#
    def _get_in_order(self, node: Optional[Node], keys: List[int]) -> None:
        """In order tree traversal"""
        if node is None:
            return

        self._get_in_order(node.left, keys)
        keys.append(node.key)
        self._get_in_order(node.right, keys)

    def _get_pre_order(self, node: Optional[Node], keys: List[int]) -> None:
        """Pre order tree traversal"""
        if node is None:
            return

        keys.append(node.key)
        self._get_pre_order(node.left, keys)
        self._get_pre_order(node.right, keys)

    def _get_post_order(self, node: Optional[Node], keys: List[int]) -> None:
        """Post order tree traversal"""
        if node is None:
            return

        self._get_post_order(node.left, keys)
        self._get_post_order(node.right, keys)
        keys.append(node.key)

    def _get_width_traversal(self, root: Optional[Node], keys: List[int]) -> None:
        """Breadth-first tree traversal"""
        if root is None:
            return

        q = queue.Queue()
        q.put(root)

        while not q.empty():
            current = q.get()
            keys.append(current.key)

            if current.left is not None:
                q.put(current.left)
            if current.right is not None:
                q.put(current.right)

    #===============#
    # MAGIC METHODS #
    #===============#
    def __len__(self):
        """Get amount of elements in tree"""
        return self._size

    def __contains__(self, key: int):
        """Find if tree contatins node with key equal to `key`"""
        found_node = self._run_search(self._root, key)

        return True if found_node is not None else False

    def __bool__(self) -> bool:
        """Check on True/False"""
        return False if self._root is None else True

    # def _run_merge(self, left: Optional[Node], right: Optional[Node]) -> Optional[Node]:
    #     if left is None:
    #         return right
    #     if right is None:
    #         return left

    #     left, right = self._run_split(left, right.key)

    #     merged_left  = self._run_merge(left, right.left)
    #     merged_right = self._run_merge(right, right.right)

    #     new_node = AVL.Node(right.key, merged_left, merged_right)
    #     self._recalc_height(new_node)
        
    #     return self._run_balancing(new_node)
    
    def __add__(self, other: Optional['AVL']) -> Optional['AVL']:
        """+ operator"""
        new_avl = copy.deepcopy(self)

        if other is None:
            return new_avl

        other_keys = other.data(order="in")

        for key in other_keys:
            new_avl.insert(key)

        return new_avl
    
    def __deepcopy__(self, memo={}) -> 'AVL':
        """Deepcopy of tree"""
        new_tree = AVL()
        new_tree._root = self._run_deepcopy(self._root)
        new_tree._size = self._size

        return new_tree

if __name__ == "__main__":
    avl = AVL()

    avl.insert(10)    
    avl.insert(20)    
    avl.insert(5)    
    avl.insert(15)
    avl.insert(123)
    avl.insert(0)
    avl.insert(1)
    avl.insert(545)
    avl.insert(9)


    left, right = avl.split(15)

    print(left.data())
    print(right.data())

    print(avl.data())

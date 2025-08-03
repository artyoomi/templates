import unittest
from avl import AVL

class TestAVLTree(unittest.TestCase):

    def setUp(self):
        self.avl = AVL()

    def test_insert_and_search(self):
        """Insert and search test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.assertTrue(10 in self.avl)
        self.assertTrue(20 in self.avl)
        self.assertTrue(5 in self.avl)
        self.assertFalse(15 in self.avl)

    def test_remove(self):
        """Remove element test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.avl.remove(20)
        self.assertFalse(20 in self.avl)
        self.assertTrue(10 in self.avl)
        self.assertTrue(5 in self.avl)

    def test_height(self):
        """Height calculation test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.assertEqual(self.avl.height(), 2)
        self.avl.insert(15)
        self.assertEqual(self.avl.height(), 3)

    def test_min_max(self):
        """Finding min and max key test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.assertEqual(self.avl.min(), 5)
        self.assertEqual(self.avl.max(), 20)

    def test_traversal(self):
        """Tree traversal test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.assertEqual(self.avl.data(order="in"), [5, 10, 20])
        self.assertEqual(self.avl.data(order="pre"), [10, 5, 20])
        self.assertEqual(self.avl.data(order="post"), [5, 20, 10])
        self.assertEqual(self.avl.data(order="width"), [10, 5, 20])

    def test_clear(self):
        """Clear tree test"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.clear()
        self.assertFalse(bool(self.avl))
        self.assertEqual(self.avl.height(), 0)

    def test_split(self):
        """Split tree test"""

        # Split in root
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.avl.insert(15)
        self.avl.insert(123)
        self.avl.insert(0)
        self.avl.insert(1)
        self.avl.insert(545)
        self.avl.insert(9)
        left, right = self.avl.split(10)
        self.assertEqual(left.data(order="in"), [0, 1, 5, 9])
        self.assertEqual(right.data(order="in"), [15, 20, 123, 545])

        self.assertEqual(self.avl.data(), [0, 1, 5, 9, 10, 15, 20, 123, 545])

        # Differenet split place
        left, right = self.avl.split(15)
        self.assertEqual(left.data(), [0, 1, 5, 9, 10])
        self.assertEqual(right.data(), [20, 123, 545])

    def test_validation(self):
        """Tree validation"""
        self.avl.insert(10)
        self.avl.insert(20)
        self.avl.insert(5)
        self.assertTrue(self.avl.validate())

    def test_add(self):
        """Merge two trees"""
        self.avl.insert(2311)
        self.avl.insert(233)
        self.avl.insert(2235)
        
        other = AVL()
        other.insert(5)
        other.insert(1023)
        other.insert(23)

        new_avl = self.avl + other
        self.assertTrue(new_avl.validate())
        self.assertEqual(new_avl.size(), 6)

if __name__ == '__main__':
    unittest.main()

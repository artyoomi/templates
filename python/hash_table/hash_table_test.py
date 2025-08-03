from hash_table import HashTable
import unittest

class TestHashTable(unittest.TestCase):
    def setUp(self):
        """Initialize hash table before each test"""
        self.ht = HashTable()

    def test_initialization(self):
        """Initialization test"""
        self.assertEqual(len(self.ht), 0)
        self.assertEqual(self.ht._capacity, 8)
        self.assertEqual(self.ht._size, 0)

        ht_diff_cap = HashTable(initial_capacity=19)
        self.assertEqual(ht_diff_cap.capacity(), 19)

    def test_setitem_and_getitem(self):
        """Get and set element test"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        # Test of getitem and setitem with two pairs
        self.assertEqual(self.ht["key1"], "value1")
        self.assertEqual(self.ht["key2"], "value2")
        self.assertEqual(len(self.ht), 2)

        # Change value by key
        self.ht["key1"] = "something"
        self.assertEqual(self.ht["key1"], "something")

        # Add more pairs
        for i in range(30):
            self.ht[f"value_{i}"] = i

        self.assertEqual(len(self.ht), 32)

    def test_setitem_update(self):
        """Update value by key"""
        self.ht["key1"] = "value1"
        self.ht["key1"] = "value2"

        self.assertEqual(self.ht["key1"], "value2")
        self.assertEqual(len(self.ht), 1)

    def test_getitem_key_error(self):
        """Test for obtaining a non-existent key"""
        with self.assertRaises(KeyError):
            _ = self.ht["nonexistent_key"]

    def test_contains(self):
        """Test of in operator"""
        self.ht["key1"] = "value1"

        self.assertTrue("key1" in self.ht)
        self.assertFalse("nonexistent_key" in self.ht)

    def test_get(self):
        """Test of get method"""
        self.ht["key1"] = "value1"

        self.assertEqual(self.ht.get("key1"), "value1")
        self.assertEqual(self.ht.get("nonexistent_key"), None)
        self.assertEqual(self.ht.get("nonexistent_key", "default"), "default")

    def test_pop(self):
        """Test of pop element by key"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        self.assertEqual(self.ht.pop("key1"), "key1")
        self.assertEqual(len(self.ht), 1)
        with self.assertRaises(KeyError):
            self.ht.pop("nonexistent_key")

    def test_items(self):
        """Get all key-value pairs"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        items = self.ht.items()
        self.assertIn(("key1", "value1"), items)
        self.assertIn(("key2", "value2"), items)
        self.assertEqual(len(items), 2)

    def test_keys(self):
        """Get alll keys test"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        keys = self.ht.keys()
        self.assertIn("key1", keys)
        self.assertIn("key2", keys)
        self.assertEqual(len(keys), 2)

        for i in range(100):
            self.ht[f"key_{i}"] = i

        keys = self.ht.keys()
        self.assertIn("key_50", keys)
        self.assertEqual(len(keys), 102)

    def test_clear(self):
        """Clear hash table test"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        self.ht.clear()
        self.assertEqual(len(self.ht), 0)
        self.assertEqual(self.ht._capacity, 8)
        self.assertEqual(self.ht._size, 0)

    def test_str(self):
        """Test of hash table string representation"""
        self.ht["key1"] = "value1"
        self.ht["key2"] = "value2"

        str_repr = str(self.ht)
        self.assertIn("key1: value1", str_repr)
        self.assertIn("key2: value2", str_repr)

if __name__ == "__main__":
    unittest.main()

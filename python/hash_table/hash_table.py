class Node:
    """Node implementation for custom Hash table"""
    def __init__(self, key, value):
        self.key   = key
        self.value = value
        self.next  = None

class HashTable:
    """Hash table implementation using Python built-in hash() function"""
    def __init__(self, initial_capacity=8):
        self._data             = [None] * initial_capacity
        self._size             = 0
        self._capacity         = initial_capacity
        self._resize_threshold = 0.75
        self._resize_ratio     = 1.5

    #===================#
    # INTERFACE METHODS #
    #===================#
    def pop(self, key):
        """Removes the element with specified key and returns it"""
        ht_index = self._key_hash(key)
        current = self._data[ht_index]

        while current:
            # When current is the last element of chain
            if current.next is None:
                if current.key == key:
                    saved_key = current.key
                    current = None
                    self._size -= 1
                    return saved_key

            # General case
            elif current.next.key == key:
                saved_key = current.next.key
                current.next = current.next.next
                self._size -= 1
                return saved_key

            current = current.next

        raise KeyError(f"{self.__class__.__name__}: pop: Unknown key: {key}")

    def get(self, key, default_value=None):
        """Returns the value of the specified key"""
        try:
            # Reuse __getitem__ logic
            value = self.__getitem__(key)
            return value
        except KeyError:
            return default_value

    def items(self):
        """Returns a list containing a tuple for each key value pair"""
        items_list = []
    
        for chain in self._data:
            current = chain
            while current:
                items_list.append((current.key, current.value))
                current = current.next

        return items_list

    def keys(self):
        """Returns a list contatining the dictionary's keys"""
        keys_list = []
    
        for chain in self._data:
            current = chain
            while current:
                keys_list.append(current.key)
                current = current.next

        return keys_list

    def capacity(self):
        """Get current capacity"""
        return self._capacity

    def clear(self) -> None:
        """Removes all elements from the array"""
        self._data.clear()

        self._size     = 0
        self._capacity = 8

    #=================#
    # BACKEND METHODS #
    #=================#
    def _key_hash(self, key) -> int:
        """Calculate hash value"""
        return hash(key) % self._capacity

    def _resize(self):
        """Resize hash table"""
        data = self._data

        # Update capacity and empty data
        self._size = 0
        self._capacity = int(self._capacity * self._resize_ratio)
        self._data = [None] * self._capacity

        for chain in data:
            current = chain
            while current:
                self[current.key] = current.value
                current = current.next

    def _calc_current_fullness(self):
        """Calculate current fullness of hash table 0..1"""
        return self._size / self._capacity

    #===============#
    # MAGIC METHODS #
    #===============#
    def __len__(self):
        """Get size of hash table"""
        return self._size

    def __str__(self) -> str:
        """Get string representation of hash table"""
        str_representations = []
        
        for chain in self._data:
            while chain:
                str_representations.append(f"{chain.key}: {chain.value}")
                chain = chain.next

        return '{' + ", ".join(str_representations) + '}'

    def __setitem__(self, key, value) -> None:
        """Set value by key, overload []"""
        ht_index = self._key_hash(key)

        # Get head of list which represents `ht_index` chain
        if self._data[ht_index] is None:
            self._data[ht_index] = Node(key, value)
            self._size += 1
        else:
            current = self._data[ht_index]
            while current:
                # Replace existing value, if key already exist
                if key == current.key:
                    current.value = value
                    return
                current = current.next

            # Create new node
            new_node = Node(key, value)
            new_node.next = self._data[ht_index]
            self._data[ht_index] = new_node
            self._size += 1

        if self._calc_current_fullness() > self._resize_threshold:
            self._resize()
        
    def __getitem__(self, key):
        """Get value by key, overload []"""
        ht_index = self._key_hash(key)
        current  = self._data[ht_index]
        
        # Search for given key value
        while current:
            if current.key == key:
                return current.value
            
            current = current.next

        # Raise error if key was not found
        raise KeyError(f"{self.__class__.__name__}: __getitem__: Unknown key: {key}")

    def __contains__(self, key) -> bool:
        """Determine if hash table contain pair with key `key`"""
        try:
            # Reuse logic from __getitem__ method
            self.__getitem__(key)
            return True
        except KeyError:
            return False

if __name__ == "__main__":
    ht = HashTable()

    ht[['key']] = "value0"
    ht["key1"] = "value1"

    for i in range(2, 30):
        ht[f"value{i}"] = i
        print(f"len of ht {len(ht)}")
        
    # print(len(ht))

    print(ht)

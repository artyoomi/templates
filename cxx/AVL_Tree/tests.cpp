#include <gtest/gtest.h>

#include "modules/AVL.h"

class AVLTest : public testing::Test {
protected:
    void SetUp() override
    {
        tree = new AVL<int>();
    }

    void TearDown() override
    {
        delete tree;
    }

    AVL<int> *tree; 
};

TEST_F(AVLTest, InsertSingleNode) {
    tree->insert(10);
    ASSERT_EQ(tree->height(), 1);
}

TEST_F(AVLTest, InsertMultipleNodes) {
    tree->insert(10);
    tree->insert(20);
    tree->insert(30);
    ASSERT_EQ(tree->height(), 2); // tree must be balanced
}

TEST_F(AVLTest, InOrderTraversal) {
    tree->insert(30);
    tree->insert(20);
    tree->insert(10);

    std::vector<int> result = tree->get_keys("in");
    
    std::vector<int> expected = {10, 20, 30};
    ASSERT_EQ(result, expected);
}

TEST_F(AVLTest, HeightOfEmptyTree) {
    ASSERT_EQ(tree->height(), 0);
}

TEST_F(AVLTest, RemoveMinFromTree) {
    tree->insert(30);
    tree->insert(20);
    tree->insert(10);
    
    tree->remove_min();
    std::vector<int> result = tree->get_keys("in");
    
    std::vector<int> expected = {20, 30};
    ASSERT_EQ(result, expected);
}

TEST_F(AVLTest, RemoveMaxFromTree) {
    tree->insert(10);
    tree->insert(20);
    tree->insert(30);
    
    tree->remove_max();
    std::vector<int> result = tree->get_keys("in");
    
    std::vector<int> expected = {10, 20};
    ASSERT_EQ(result, expected);
}

TEST_F(AVLTest, RemoveNonExistentNode) {
    tree->insert(10);
    tree->insert(20);
    tree->insert(30);
    
    tree->remove(40); // try to delete non-exist node
    std::vector<int> result = tree->get_keys("in");
    
    std::vector<int> expected = {10, 20, 30};
    ASSERT_EQ(result, expected);
}

TEST_F(AVLTest, InOrderTraversalAfterMultipleInsertions) {
    for (int i = 1; i <= 10; ++i) {
        tree->insert(i);
    }

    std::vector<int> result = tree->get_keys("in");
    
    std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQ(result, expected);
}

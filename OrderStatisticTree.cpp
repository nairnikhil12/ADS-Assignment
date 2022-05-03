#include <iostream>
#include <vector>
using namespace std;

struct Node {
    Node* left;
    Node* right;

    int key;
    int height;
    int subtree_size;

    Node(int key) {
        this->key = key;

        height = 1;
        subtree_size = 1;

        left = nullptr;
        right = nullptr;
    }
};

/**
 * Implements Order Statistic Tree by augmenting AVL Tree
 */
class OrderStatisticTree {
private:
    /**
     * Returns the height of the given node
     *
     * @param node the node whose height is needed
     * @return height of the given node
     */
    int _getHeight(Node* node) {
        if(node == nullptr)
            return 0;

        return node->height;
    }

    /**
     * Returns the balance factor of the given node
     *
     * @param node the node whose balance is needed
     * @return balance factor of given node
     */
    int _getBalance(Node* node) {
        if(node == nullptr)
            return 0;

        return _getHeight(node->left) - _getHeight(node->right);
    }

    int _getSize(Node* node) {
        if(node == nullptr)
            return 0;

        return node->subtree_size;
    }

    /**
     * Performs the left-rotate operation on the given node.
     *
     * @param node the node which needs to be rotated
     * @return the node that will replace the given node as root after rotation
     */
    Node* _leftRotate(Node* node) {
        Node* right_child = node->right;
        Node* left_child_of_right_child = right_child->left;

        right_child->left = node;
        node->right = left_child_of_right_child;

        // recompute the heights
        node->height = 1 + max(_getHeight(node->left), _getHeight(node->right));
        right_child->height = 1 + max(_getHeight(right_child->left), _getHeight(right_child->right));

        // recompute the subtree sizes
        node->subtree_size = 1 + _getSize(node->left) + _getSize(node->right);
        right_child->subtree_size = 1 + _getSize(right_child->left) + _getSize(right_child->right);

        if(left_child_of_right_child != nullptr)
            left_child_of_right_child->subtree_size = 1 + _getSize(left_child_of_right_child->left) + _getSize(left_child_of_right_child->right);

        return right_child;
    }

    /**
     * Performs the right-rotate operation on the given node.
     *
     * @param node the node which needs to be rotated
     * @return the node that will replace the given node as root after rotation
     */
    Node* _rightRotate(Node* node) {
        Node* left_child = node->left;
        Node* right_child_of_left_child = left_child->right;

        left_child->right = node;
        node->left = right_child_of_left_child;

        // recompute the heights
        node->height = 1 + max(_getHeight(node->left), _getHeight(node->right));
        left_child->height = 1 + max(_getHeight(left_child->left), _getHeight(left_child->right));

         // recompute the subtree sizes
        node->subtree_size = 1 + _getSize(node->left) + _getSize(node->right);
        left_child->subtree_size = 1 + _getSize(left_child->left) + _getSize(left_child->right);

        if(right_child_of_left_child != nullptr)
            right_child_of_left_child->subtree_size = 1 + _getSize(right_child_of_left_child->left) + _getSize(right_child_of_left_child->right);

        return left_child;
    }

    /**
     * Finds and returns the inorder predecessor of the given node
     *
     * @param node the node whose inorder predecessor is required
     * @return a pointer to the inorder predecessor
     */
    Node* _minValueNode(Node* node) {
        Node* current = node;
        while(current->left != nullptr)
            current = current->left;

        return current;
    }

    /**
     * Balances the given node after AVL insertion
     *
     * @param node the node which needs to be balanced
     * @return the root of the subtree after balancing is performed
     */
    Node* _BalanceTree(Node* node) {
         int balance = _getBalance(node);
         int key = node->key;

        // Left child has height greater than right child
        if(balance > 1) {
            // Case 1: Left-Left Case
            if(node->left != nullptr && key < node->left->key) {
                return _rightRotate(node);
            }
            // Case 2: Left-Right Case
            if(node->right != nullptr && key > node->right->key) {
                node->left = _leftRotate(node->left);
                return _rightRotate(node);
            }
        }
        // Right child has height greater than left child
        if(balance < -1) {
            // Case 3: Right-Left Case
            if(node->left != nullptr && key < node->left->key) {
                node->right = _rightRotate(node->right);
                return _leftRotate(node);
            }
            // Case 4: Right-Right Case
            if(node->right != nullptr && key > node->right->key) {
                return _leftRotate(node);
            }
        }

        return node;
    }

    /**
     * Inserts the given key in the AVL tree represented by node
     *
     * @param node the root of the tree
     * @param key the key to be inserted
     * @return the root after insertion is performed.
     */
    Node* _insert(Node* node, int key) {
        if(node == nullptr) {
            return new Node(key);
        }
        else if(key < node->key) {
            node->left = _insert(node->left, key);
        }
        else if(key > node->key) {
            node->right = _insert(node->right, key);
        }
        else {
            return node;
        }

        node->height = 1 + max(_getHeight(node->left), _getHeight(node->right));
        node->subtree_size = 1 + _getSize(node->left) + _getSize(node->right);

        return _BalanceTree(node);
    }

    /**
     * Deletes the key from the subtree denoted by node.
     *
     * @param node the root of the tree
     * @param key the key to be deleted
     * @return the root of the tree after deletion is performed.
     */
    Node* _deleteNode(Node* node, int key) {
        if(node == nullptr) {
            return nullptr;
        }

        if(key < node->key) {
            node->left = _deleteNode(node->left, key);
        }
        else if(key > node->key) {
            node->right = _deleteNode(node->right, key);
        }
        else {
            // Case 1: Less than two children
            if(node->left == nullptr || node->right == nullptr) {
                Node* current = node->left ? node->left : node->right;

                if(current == nullptr) {
                    current = node;
                    node = nullptr;
                }
                else {
                    *node = *current;
                }

                delete current;
                current = nullptr;
            }
            // Case 2: Two children
            else {
                Node* current = _minValueNode(node->right);

                node->key = current->key;
                node->right = _deleteNode(node->right, node->key);
            }
        }

        if(node == nullptr)
            return node;

        node->height = max(_getHeight(node->left), _getHeight(node->right)) + 1;
        node->subtree_size = 1 + _getSize(node->left) + _getSize(node->right);

        int balance = _getBalance(node);

        // Left child has height greater than right child
        if(balance > 1) {
            if(_getBalance(node->left) >= 0) {
                return _rightRotate(node);
            }
            if(_getBalance(node->left) < 0) {
                node->left = _leftRotate(node->left);
                return _rightRotate(node);
            }
        }
        // Right child has height greater than left child
        if(balance < -1) {
            if(_getBalance(node->right) <= 0) {
                return _leftRotate(node);
            }
            if(_getBalance(node->right) > 0) {
                node->right = _rightRotate(node->right);
                return _leftRotate(node);
            }
        }

        return node;
    }

    /**
     * Returns the node that would lie on the position 'rank' from the left
     * if all the nodes were sorted in ascending order according to the key.
     *
     * @param node the root of the tree
     * @param rank the rank
     * @return the pointer to the key with the given rank
     */
    Node* _findByRank(Node* node, int  rank) {
        if(node == nullptr)
            return nullptr;

        int left_size = _getSize(node->left) + 1;

        if(rank == left_size) {
            return node;
        }
        else if(rank < left_size) {
            return _findByRank(node->left, rank);
        }
        else {
            return _findByRank(node->right, rank - left_size);
        }
    }

    /**
     * Returns the rank of the key.
     *
     * @param node the root of the tree
     * @param key the key whose rank is needed
     * @return rank of the key
     */
    int _rankOfKey(Node* node, int key) {
        if(node == nullptr)
            return -1;

        if(key < node->key) {
            return _rankOfKey(node->left, key);
        }
        else if(key == node->key) {
            return _getSize(node->left) + 1;
        }
        else {
            return 1 + _getSize(node->left) + _rankOfKey(node->right, key);
        }
    }

public:
    Node* root;

    OrderStatisticTree() {
        root = nullptr;
    }

    /**
     * Performs a preorder traversal from a specific node.
     *
     * @param node the node from where you want to start the traversal.
     */
    void Preorder(Node* node) {
        if(node == nullptr)
            return ;

        cout<<node->key<<" ";
        Preorder(node->left);
        Preorder(node->right);
    }

    /**
     * Performs a preorder traversal from the root.
     */
    void Preorder() {
        Preorder(root);
    }

    /**
     * Performs a inorder traversal from a specific node.
     *
     * @param node the node from where you want to start the traversal.
     */
    void Inorder(Node* node, vector<int>& inorder) {
        if(node == nullptr)
            return ;

        Inorder(node->left, inorder);
        inorder.push_back(node->key);
        Inorder(node->right, inorder);
    }

    /**
     * Performs a preorder traversal from the root.
     */
    vector<int> Inorder() {
        vector<int> result;
        Inorder(root, result);
        return result;
    }

    /**
     * Performs the standard BST search operation.
     *
     * @param key the key to search for
     * @return the node corresponding to the key or nullptr if the key is not found
     */
    Node* Search(int key) {
        Node* tmp = root;

        while(tmp != nullptr) {
            if(key == tmp->key) {
                return tmp;
            }
            else if(key > tmp->key) {
                tmp = tmp->right;
            }
            else {
                tmp = tmp->left;
            }
        }

        return nullptr;
    }

    /**
     * Inserts the given key into the tree.
     *
     * @param key the key to be inserted
     */
    void Insert(int key) {
        root = _insert(root, key);
    }

    /**
     * Deletes the given key and its corresponding object from
     * the tree.
     *
     * @param key the key to be deleted
     */
    void Delete(int key) {
        root = _deleteNode(root, key);
    }

    /**
     * Returns the node that would lie on the position 'rank' from the left
     * if all the nodes were sorted in ascending order according to the key.
     *
     * @param rank 1-based rank
     * @return the pointer to the key with the given rank
     */
    Node* FindByRank(int rank) {
        return _findByRank(root, rank);
    }

    /**
     * Returns the 1-based rank of the key.
     *
     * @param key the key whose rank is needed
     * @return rank of the key
     */
    int RankOfKey(int key) {
        return _rankOfKey(root, key);
    }
};
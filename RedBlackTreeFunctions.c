//
// Created by Oikonomou Dimitrios on 25/2/2023.
//


#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "RedBlackTreeFunctions.h"

#define BUFFER_SIZE 100
typedef struct node *Node;

typedef enum Colour {
    Red,
    Black
} Colour;

struct rbTree {
    Node root;
    Node sentinel_node; //to avoid edge cases with nulls

    int (*compare_func)(const void *, const void *);

    void (*transform_key_to_string_func)(const void *, char *buffer, size_t buffer_size);

};

struct node {
    void *key;
    Colour colour;
    Node left;
    Node right;
    Node parent;
};

RedBlackTree RBTree;

const char *colours[] = {"red", "black"};


void rbt_insert_node_fixup(RedBlackTree *redBlackTree, Node aux_node);

void rbt_left_rotate(RedBlackTree *redBlackTree, Node aux_node);

void rbt_right_rotate(RedBlackTree *redBlackTree, Node aux_node);

void rbt_transplant(RedBlackTree *redBlackTree, Node old_node, Node new_node);

bool rbt_is_left_child(Node node);

bool rbt_is_right_child(Node node);

Node rbt_tree_minimum(Node aux_node, Node sentinelNode);

void rbt_delete_fixup(RedBlackTree *redBlackTree, Node aux_node);

Node rbt_initialize_new_node(Node sentinel_node, void *key, Colour colour);

Node rbt_initialize_sentinel();


Node rbt_find_node_internal(Node auxNode, RedBlackTree rbTree, const void *key);

void rbt_free_node(Node aux_node);

void rbt_print_tree_internal(RedBlackTree redBlackTree, Node aux_node, int *is_empty);

void rbt_insert_node(RedBlackTree *redBlackTree, void *value_from_user, int *error) {
    Node nullNode = (*redBlackTree)->sentinel_node;
    if (rbt_find_node_internal((*redBlackTree)->root, *redBlackTree, value_from_user)) {
        *error = 1;
        return;
    }
    Node new_node = rbt_initialize_new_node((*redBlackTree)->sentinel_node, value_from_user, Red);
    Node parent_node = nullNode;
    Node current_node = (*redBlackTree)->root;
    while (current_node != nullNode) {
        parent_node = current_node;
        if ((*redBlackTree)->compare_func((new_node->key), (current_node->key)) < 0) {
            current_node = current_node->left;
        } else {
            current_node = current_node->right;
        }
    }
    new_node->parent = parent_node;
    if (parent_node == nullNode) {
        (*redBlackTree)->root = new_node;
    } else if ((*redBlackTree)->compare_func((new_node->key), (parent_node->key)) < 0) {
        parent_node->left = new_node;
    } else {
        parent_node->right = new_node;
    }
    rbt_insert_node_fixup(redBlackTree, new_node);
    *error = 0;
}

void rbt_insert_node_fixup(RedBlackTree *redBlackTree, Node aux_node) {
    Node uncle;
    Node nullNode = (*redBlackTree)->sentinel_node;
    while (aux_node->parent && aux_node->parent != nullNode && aux_node->parent->colour == Red) {
        if (!aux_node->parent->parent || aux_node->parent->parent == nullNode) {
            break;
        }
        if (aux_node->parent == aux_node->parent->parent->left) {
            uncle = aux_node->parent->parent->right;
            if (uncle->colour == Red) {
                aux_node->parent->colour = Black;
                uncle->colour = Black;
                aux_node->parent->parent->colour = Red;
                aux_node = aux_node->parent->parent;
            } else {
                if (aux_node == aux_node->parent->right) {
                    aux_node = aux_node->parent;
                    rbt_left_rotate(redBlackTree, aux_node);
                }
                aux_node->parent->colour = Black;
                if (aux_node->parent->parent != nullNode) {
                    aux_node->parent->parent->colour = Red;
                    rbt_right_rotate(redBlackTree, aux_node->parent->parent);
                }
            }
        } else {
            uncle = aux_node->parent->parent->left;
            if (uncle->colour == Red) {
                aux_node->parent->colour = Black;
                uncle->colour = Black;
                aux_node->parent->parent->colour = Red;
                aux_node = aux_node->parent->parent;
            } else {
                if (aux_node == aux_node->parent->left) {
                    aux_node = aux_node->parent;
                    rbt_right_rotate(redBlackTree, aux_node);
                }
                aux_node->parent->colour = Black;
                if (aux_node->parent->parent != nullNode) {
                    aux_node->parent->parent->colour = Red;
                    rbt_left_rotate(redBlackTree, aux_node->parent->parent);
                }
            }
        }
    }
    (*redBlackTree)->root->colour = Black;
}

RedBlackTree
rbt_initialize_tree(int (*compare_func)(const void *, const void *),
                    void(*transform_key_to_string_func)(const void *, char *buffer, size_t buffer_size)) {
    RBTree = (RedBlackTree) malloc(sizeof(struct rbTree));
    assert(RBTree != NULL);
    Node sentinel = rbt_initialize_sentinel();
    RBTree->root = sentinel;
    RBTree->sentinel_node = sentinel;
    RBTree->compare_func = compare_func;
    RBTree->transform_key_to_string_func = transform_key_to_string_func;
    return RBTree;
}

void rbt_right_rotate(RedBlackTree *redBlackTree, Node aux_node) {
    Node nullNode = (*redBlackTree)->sentinel_node;
    Node new_parent = aux_node->left;
    aux_node->left = new_parent->right;
    if (new_parent->right != nullNode) {
        new_parent->right->parent = aux_node;
    }
    new_parent->parent = aux_node->parent;
    if (aux_node->parent == nullNode) {
        (*redBlackTree)->root = new_parent;
    } else if (rbt_is_left_child(aux_node)) {
        aux_node->parent->left = new_parent;
    } else {
        aux_node->parent->right = new_parent;
    }
    new_parent->right = aux_node;
    aux_node->parent = new_parent;
}

void rbt_left_rotate(RedBlackTree *redBlackTree, Node aux_node) {
    Node nullNode = (*redBlackTree)->sentinel_node;
    Node new_parent = aux_node->right;
    aux_node->right = new_parent->left;
    if (new_parent->left != nullNode) {
        new_parent->left->parent = aux_node;
    }
    new_parent->parent = aux_node->parent;
    if (aux_node->parent == nullNode) {
        (*redBlackTree)->root = new_parent;
    } else if (rbt_is_left_child(aux_node)) {
        aux_node->parent->left = new_parent;
    } else {
        aux_node->parent->right = new_parent;
    }
    new_parent->left = aux_node;
    aux_node->parent = new_parent;

}

void rbt_delete_node(RedBlackTree *redBlackTree, const void *value_from_user, int *error) {
    Node nullNode = (*redBlackTree)->sentinel_node;
    Node node_to_be_deleted = rbt_find_node_internal((*redBlackTree)->root, *redBlackTree, value_from_user);
    if (!node_to_be_deleted) {
        *error = 1;
        return;
    } else {
        Node y = node_to_be_deleted;
        Colour y_original_Colour = y->colour;
        Node replacement_node;
        if (node_to_be_deleted->left == nullNode) {
            replacement_node = node_to_be_deleted->right;
            rbt_transplant(redBlackTree, node_to_be_deleted, node_to_be_deleted->right);
        } else if (node_to_be_deleted->right == nullNode) {
            replacement_node = node_to_be_deleted->left;
            rbt_transplant(redBlackTree, node_to_be_deleted, node_to_be_deleted->left);

        } else {
            y = rbt_tree_minimum(node_to_be_deleted->right, (*redBlackTree)->sentinel_node);
            y_original_Colour = y->colour;
            replacement_node = y->right;
            if (y->parent == node_to_be_deleted) {
                replacement_node->parent = y;
            } else {
                rbt_transplant(redBlackTree, y, y->right);
                y->right = node_to_be_deleted->right;
                y->right->parent = y;
            }
            rbt_transplant(redBlackTree, node_to_be_deleted, y);
            y->left = node_to_be_deleted->left;
            y->left->parent = y;
            y->colour = node_to_be_deleted->colour;
        }
        rbt_free_node(node_to_be_deleted);
        if (y_original_Colour == Black) {
            rbt_delete_fixup(redBlackTree, replacement_node);
        }
    }
    *error = 0;
}

void rbt_delete_fixup(RedBlackTree *redBlackTree, Node aux_node) {
    while (aux_node != (*redBlackTree)->root && aux_node->colour == Black) {
        if (aux_node == aux_node->parent->left) {
            Node sibling_node = aux_node->parent->right;
            if (sibling_node->colour == Red) {
                sibling_node->colour = Black; // case 1
                aux_node->parent->colour = Red; // case 1
                rbt_left_rotate(redBlackTree, aux_node->parent); // case 1
                sibling_node = aux_node->parent->right; // case 1
            }
            if (sibling_node->left->colour == Black && sibling_node->right->colour == Black) {
                sibling_node->colour = Red; // case 2
                aux_node = aux_node->parent; // case 2
            } else {
                if (sibling_node->right->colour == Black) {
                    sibling_node->left->colour = Black; // case 3
                    sibling_node->colour = Red; // case 3
                    rbt_right_rotate(redBlackTree, sibling_node); // case 3
                    sibling_node = aux_node->parent->right; // case 3
                }

                sibling_node->colour = aux_node->parent->colour; // case 4
                aux_node->parent->colour = Black; // case 4
                sibling_node->right->colour = Black; // case 4
                rbt_left_rotate(redBlackTree, aux_node->parent); // case 4
                aux_node = (*redBlackTree)->root; // case 4
            }
        } else { // same as then clause with "right" and "left" exchanged
            Node w = aux_node->parent->left;
            if (w->colour == Red) {
                w->colour = Black; // case 1
                aux_node->parent->colour = Red; // case 1
                rbt_right_rotate(redBlackTree, aux_node->parent); // case 1
                w = aux_node->parent->left; // case 1
            }
            if (w->right->colour == Black && w->left->colour == Black) {
                w->colour = Red; // case 2
                aux_node = aux_node->parent; // case 2
            } else {
                if (w->left->colour == Black) {
                    w->right->colour = Black; // case 3
                    w->colour = Red; // case 3
                    rbt_left_rotate(redBlackTree, w); // case 3
                    w = aux_node->parent->left; // case 3
                }

                w->colour = aux_node->parent->colour; // case 4
                aux_node->parent->colour = Black; // case 4
                w->left->colour = Black; // case 4
                rbt_right_rotate(redBlackTree, aux_node->parent); // case 4
                aux_node = (*redBlackTree)->root; // case 4
            }
        }
    }
    aux_node->colour = Black;
}


Node rbt_tree_minimum(Node aux_node, Node sentinelNode) {
    while (aux_node->left != sentinelNode) {
        return rbt_tree_minimum(aux_node->left, sentinelNode);
    }
    return aux_node;
}

Node rbt_find_node_internal(Node auxNode, RedBlackTree rbTree, const void *key) {
    if (!auxNode || auxNode == (rbTree)->sentinel_node) return NULL;
    int result = rbTree->compare_func(key, (auxNode->key));
    if (result == 0) return auxNode;
    if (result < 0) return rbt_find_node_internal(auxNode->left, rbTree, key);
    else return rbt_find_node_internal(auxNode->right, rbTree, key);
}

void rbt_print_tree(RedBlackTree *redBlackTree, int *is_empty) {
    assert((*redBlackTree) && (*redBlackTree)->root);
    rbt_print_tree_internal((*redBlackTree), (*redBlackTree)->root, is_empty);
}

void rbt_print_tree_internal(RedBlackTree redBlackTree, Node aux_node, int *is_empty) {
    if (aux_node == redBlackTree->sentinel_node || aux_node == NULL) {
        *is_empty = 1;
        return;
    }
    if (aux_node->left != (redBlackTree)->sentinel_node) {
        rbt_print_tree_internal(redBlackTree, aux_node->left, is_empty);
    }
    char is_left_or_right_child[50];
    char keyValue[150];
    char buffer[BUFFER_SIZE];
    if (aux_node->parent != (redBlackTree)->sentinel_node && aux_node->parent &&
        aux_node->parent->key != NULL) {
        redBlackTree->transform_key_to_string_func(aux_node->parent->key, buffer, BUFFER_SIZE);
        sprintf(keyValue, "%s", buffer);
    }
    if (rbt_is_left_child(aux_node)) {
        strcpy(is_left_or_right_child, "left child of ");
        strcat(is_left_or_right_child, keyValue);
    } else if (rbt_is_right_child(aux_node)) {
        strcpy(is_left_or_right_child, "right child of ");
        strcat(is_left_or_right_child, keyValue);
    } else if (aux_node->parent == (redBlackTree)->sentinel_node) {
        strcpy(is_left_or_right_child, "root");
    }
    redBlackTree->transform_key_to_string_func(aux_node->key, buffer, BUFFER_SIZE);
    printf("\nkey : %s color : %s is %s ", buffer, colours[aux_node->colour], is_left_or_right_child);
    if (aux_node->right != (redBlackTree)->sentinel_node) {
        rbt_print_tree_internal(redBlackTree, aux_node->right, is_empty);
    }
    *is_empty = 0;

}

void rbt_transplant(RedBlackTree *redBlackTree, Node old_node, Node new_node) {
    if (old_node->parent == (*redBlackTree)->sentinel_node) {
        (*redBlackTree)->root = new_node;
    } else if (rbt_is_left_child(old_node)) {
        old_node->parent->left = new_node;
    } else {
        old_node->parent->right = new_node;
    }
    new_node->parent = old_node->parent;
}


bool rbt_is_left_child(Node node) {
    return node == node->parent->left;
}

bool rbt_is_right_child(Node node) {
    return node == node->parent->right;
}


Node rbt_initialize_new_node(Node sentinel_node, void *key, Colour colour) {
    Node newRecord = (Node) malloc(sizeof(struct node));
    assert(newRecord != NULL);
    (newRecord->key) = key;
    newRecord->colour = colour ? colour : Red;
    newRecord->left = sentinel_node;
    newRecord->right = sentinel_node;
    newRecord->parent = sentinel_node;
    return newRecord;
}

Node rbt_initialize_sentinel() {
    Node newRecord = (Node) malloc(sizeof(struct node));
    assert(newRecord != NULL);
    newRecord->colour = Black;
    newRecord->left = newRecord;
    newRecord->right = newRecord;
    newRecord->parent = newRecord;
    return newRecord;
}

void rbt_postorder_walk(RedBlackTree *redBlackTree, Node n, void (*callback)(Node)) {
    if (n == NULL || n == (*redBlackTree)->sentinel_node) {
        return;
    }
    rbt_postorder_walk(redBlackTree, n->left, callback);
    rbt_postorder_walk(redBlackTree, n->right, callback);
    callback(n);
}

void rbt_free(RedBlackTree *redBlackTree) {
    // Free the nodes in the tree using a post-order traversal.
    rbt_postorder_walk(redBlackTree, (*redBlackTree)->root, rbt_free_node);
    free((*redBlackTree)->sentinel_node);
    // Free the tree itself.
    free(*redBlackTree);
    *redBlackTree = NULL;
}

// Helper function to free a node.
void rbt_free_node(Node aux_node) {
    free(aux_node->key);
    free(aux_node);
}

void * rbt_find_node(RedBlackTree redBlackTree, const void *value_from_user) {
    void *val = NULL;
    Node result = rbt_find_node_internal(redBlackTree->root, redBlackTree, value_from_user);
    if (result != NULL) {
        return result->key;
    }
    return val;
}
//
// Created by DimitrisPhys_Desktop on 31/12/2022.
//

#ifndef REDBLACKTREE_REDBLACKTREEFUNCTIONS_H
#define REDBLACKTREE_REDBLACKTREEFUNCTIONS_H

#include <stdbool.h>
#include <string.h>


typedef struct rbTree *RedBlackTree;

/**
 * Inserts a node to a Red-Black Tree.
 *
 * Preconditions:
 * - `redBlackTree` points to a valid RedBlackTree struct.
 * - `value_from_user` is a pointer to the value to be inserted into the tree.
 * - `error` is a pointer to a variable that can be used to indicate any errors that occur during the insertion process.
 *
 * Postconditions:
 * - If the value was successfully inserted into the tree, the function sets *error to 0.
 * - If the value already exists in the Red-Black Tree, the function sets *error to a non-zero value that indicates that
 *   an error occurred during insertion, and returns without modifying the RedBlackTree struct.
 */
void rbt_insert_node(RedBlackTree *redBlackTree, void *value_from_user, int *error);

/**
 * Initializes a new Red-Black Tree.
 *
 * Preconditions:
 * - compare_func: a valid function pointer to a comparison function that takes two
 *                 const void pointers as arguments and returns an integer.
 *                 The function should return a negative integer if the first
 *                 argument is less than the second, a positive integer if the
 *                 first argument is greater than the second, and 0 if they
 *                 are equal.
 * - transform_key_to_string_func: a valid function pointer to a function that takes
 *                                  a const void pointer as the first argument, a
 *                                  char buffer as the second argument, and the size
 *                                  of the buffer as the third argument. The function
 *                                  should not modify the key and should transform it
 *                                  into a human-readable string representation that
 *                                  fits within the provided buffer in order for it to be
 *                                  user by rbt_print_tree function.
 * - buffer_size: the maximum size of the buffer passed to transform_key_to_string_func
 *                which must not exceed 100.
 *
 * Postconditions:
 * - Returns a pointer to the newly initialized Red-Black Tree.
 */
RedBlackTree
rbt_initialize_tree(int (*compare_func)(const void *, const void *),
                    void (*transform_key_to_string_func)(const void *, char *buffer, size_t buffer_size));

/**
 * Prints the Red-Black Tree in a depth-first order
 *
 * Preconditions:
 *  - redBlackTree: a pointer to a RedBlackTree struct that has been initialized.
 *  - is_empty: a pointer to an int variable that has been initialized.
 * Postconditions:
 * - The tree structure is printed to the console in a human-readable format
 * - The output includes the key value and color of each node in the tree, as well as its parent-child relationship
 * - The output includes a message indicating if the node is the root of the tree or a left/right child of its parent
 * - The value of is_empty is set to 1 if the tree is empty, or 0 if it is not.
 */
void rbt_print_tree(RedBlackTree *redBlackTree, int *is_empty);

/**
 * Deletes the node with the specified value from the given Red-Black Tree.
 *
 * Preconditions:
 * - redBlackTree: a valid pointer to a Red-Black Tree object.
 * - value_from_user: a valid pointer to the value to be deleted from the tree.
 * - error: a valid pointer to an integer that will be set to 1 if the node with the
 *          specified value is not found in the tree, or 0 otherwise. The pointer
 *          must not be NULL.
 *
 * Postconditions:
 * - If the node with the specified value is found in the tree, it is deleted and the
 *   tree is adjusted to maintain the Red-Black Tree properties.
 * - If the node is not found, the error variable is set to 1.
 * - If the deletion and adjustment of the tree succeeds, the error variable is set to 0.
 * - The memory allocated for the deleted node is freed.
 */
void rbt_delete_node(RedBlackTree *redBlackTree, const void *value_from_user, int *error);

/**
 * Frees the memory used by the given Red-Black Tree and all of its nodes.
 *
 * Preconditions:
 * - redBlackTree: a valid pointer to a Red-Black Tree object. The pointer must not be NULL.
 *
 * Postconditions:
 * - The memory used by the Red-Black Tree and all of its nodes is freed.
 * - The pointer to the Red-Black Tree object is set to NULL.
 */
void rbt_free(RedBlackTree *redBlackTree);

void * rbt_find_node(RedBlackTree redBlackTree, const void *value_from_user);

#endif //REDBLACKTREE_REDBLACKTREEFUNCTIONS_H

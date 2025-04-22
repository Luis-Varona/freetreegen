#ifndef LAYOUT_H
#define LAYOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    TreeNode **children;
    size_t num_children;
} TreeNode;

typedef struct {
    int order;
    int *layout;
} FreeTree;

FreeTree *build_tree_layout(TreeNode *node);
bool is_primary_rooted(FreeTree *tree);

#endif

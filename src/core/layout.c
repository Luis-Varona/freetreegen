#include "layout.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Here, and in the rest of the code, replace (failed) exits with errors
static void dfs_collect(TreeNode *node, FreeTree *tree, size_t *capacity,
                        size_t depth) {
  if (tree->order >= *capacity) {
    *capacity *= 2;
    tree->layout = realloc(tree->layout, sizeof(size_t) * (*capacity));

    if (!tree->layout) {
      fprintf(stderr, "Memory reallocation failed for `tree->layout`\n");
      exit(EXIT_FAILURE);
    }
  }

  tree->layout[tree->order++] = depth;

  for (size_t i = 0; i < node->num_children; i++) {
    dfs_collect(node->children[i], tree, capacity, depth + 1);
  }
}

// TODO: This just builds a layout; how do we the lexicographically largest one?
FreeTree *build_tree_layout(TreeNode *node) {
  FreeTree *tree = malloc(sizeof(FreeTree));

  if (!tree) {
    fprintf(stderr, "Memory allocation failed for `tree`\n");
    exit(EXIT_FAILURE);
  }

  size_t capacity = 32;
  tree->order = 0;
  tree->layout = malloc(sizeof(size_t) * capacity);

  if (!tree->layout) {
    fprintf(stderr, "Memory allocation failed for `tree->layout`\n");
    exit(EXIT_FAILURE);
  }

  dfs_collect(node, tree, &capacity, 0);
  tree->layout = realloc(tree->layout, sizeof(size_t) * tree->order);

  if (!tree->layout) {
    fprintf(stderr, "Memory reallocation failed for `tree->layout`\n");
    exit(EXIT_FAILURE);
  }

  return tree;
}

bool is_primary_rooted(FreeTree *tree) {
  size_t order = tree->order;

  // Small trees of order up to 2 are trivially primary-rooted
  if (order <= 2) {
    return true;
  }

  size_t *layout = tree->layout;
  size_t m = 0;
  size_t max1 = 0;
  size_t curr = layout[0];
  size_t one_count = curr == 1 ? 1 : 0;

  // There are at least 2 degree-1 nodes, since the order is greater than 2
  while (one_count < 2) {
    if (curr > max1) {
      max1 = curr;
    }

    curr = layout[++m];
    
    if (curr == 1) {
      one_count++;
    }
  }

  size_t max2 = layout[0];

  for (size_t i = m; i < order; i++) {
    if (max2 < layout[i]) {
      max2 = layout[i];
    }
  }

  if (max1 > max2) {
    return false;
  }

  if (max1 < max2) {
    return true;
  }

  size_t len1 = m - 1; // No underflow, since `m` is the second degree-1 index
  size_t len2 = order - m + 1;

  if (len1 > len2) {
    return false;
  }

  if (len1 < len2) {
    return true;
  }

  // TODO: Implement (also, remember to lexicographically maximize the layout)
}

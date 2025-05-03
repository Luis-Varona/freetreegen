#include "layout.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: This does not yet work; lexicographical sorting != subtree size sorting
// Lexicographical sorting involves first sorting by maximum subtree depth, then
// by subtree size. Use SA-IS, and maybe go back to one-pass DFS with no sizes?
static int treenode_cmp(const void *a, const void *b) {
  const TreeNode *node_a = *(const TreeNode **)a;
  const TreeNode *node_b = *(const TreeNode **)b;
  size_t subtree_size_a = node_a->subtree_size;
  size_t subtree_size_b = node_b->subtree_size;
  return (subtree_size_a > subtree_size_b) - (subtree_size_a < subtree_size_b);
}

static size_t sort_subtrees(TreeNode *node) {
  size_t subtree_size = 1;

  for (size_t i = 0; i < node->num_children; i++) {
    subtree_size += sort_subtrees(node->children[i]);
  }

  if (node->num_children > 1) {
    qsort(node->children, node->num_children, sizeof(TreeNode *), treenode_cmp);
  }

  node->subtree_size = subtree_size;
  return subtree_size;
}

// TODO: Here, and in the rest of the code, replace (failed) exits with errors
static void store_depths(TreeNode *node, FreeTree *tree, size_t *capacity,
                         size_t depth) {
  if (tree->order >= *capacity) {
    *capacity *= 2;
    tree->layout = realloc(tree->layout, sizeof(size_t) * (*capacity));

    if (!tree->layout) {
      perror("Memory reallocation failed for `tree->layout`");
      exit(EXIT_FAILURE);
    }
  }

  tree->layout[tree->order++] = depth;

  for (size_t i = node->num_children; i-- > 0;) { // Visit larger subtrees first
    store_depths(node->children[i], tree, capacity, depth + 1);
  }
}

FreeTree *build_tree_layout(TreeNode *node) {
  FreeTree *tree = malloc(sizeof(FreeTree));

  if (!tree) {
    perror("Memory allocation failed for `tree`");
    exit(EXIT_FAILURE);
  }

  size_t capacity = 32;
  tree->order = 0;
  tree->layout = malloc(sizeof(size_t) * capacity);

  if (!tree->layout) {
    perror("Memory allocation failed for `tree->layout`");
    exit(EXIT_FAILURE);
  }

  sort_subtrees(node);
  store_depths(node, tree, &capacity, 0);
  tree->layout = realloc(tree->layout, sizeof(size_t) * tree->order);

  if (!tree->layout) {
    perror("Memory reallocation failed for `tree->layout`");
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

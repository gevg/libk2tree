/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nlehmann@dcc.uchile.cl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Nicolás Lehmann
 * ----------------------------------------------------------------------------
 *
 */

#ifndef SRC_K2TREEBUILDER_H_
#define SRC_K2TREEBUILDER_H_

#include <cstddef>
#include <vector>
#include "utils/bitstring.h"

namespace k2tree_impl {

using utils::BitString;

/*
 * Implement the construction of section 3.3.3, building a regular tree (not compressed)
 * inserting one edge of the graph (1s in the matrix) at a time.
 */
class K2TreeBuilder {
 public:
  /* 
   * Create a builder for a tree with an hybrid aproach.
   *
   * @param nodes Number of nodes in the graph.
   * @param k1 arity of the first levels.
   * @param k2 arity of the second part.
   * @param kl arity of the level height-1.
   * @param k1_levels Number of levels with arity k1.
   */
  K2TreeBuilder(size_t nodes, int k1, int k2, int kl, int k1_levels);
  /* 
   * Insert an edge (1 on the matrix)
   *
   * @param row Starting node of the edge/row in the matrix
   * @param col Ending node of the edge/col in the matrix
   */
  void InsertEdge(size_t row, size_t col);

  void Build() const;

  inline int height() const {
    return height_;
  }

  inline int leafs() const {
    return leafs_;
  }

  inline int edges() const {
    return edges_;
  }

  inline int nodes_k1() const {
    return nodes_k1_;
  }
  
  inline int nodes_k2() const {
    return nodes_k2_;
  }

  inline int nodes_kl() const {
    return nodes_kl_;
  }


  ~K2TreeBuilder();

 private:
  // Number of nodes in the original graph
  size_t nodes_;
  // Number of rows (and cols) in the expanded matrix
  size_t size_;
  // Arity of the first part
  int k1_;
  // Arity of the second part
  int k2_;
  // Arity of the level height-1
  int kl_;
  // Last level with arity k1. Each node in this level has 0 or k1 children.
  int max_level1_;
  // Height of the tree, also the number of the leaf level.
  int height_;
  // Number of nodes on the last level.
  int leafs_;
  // Number of 1s on the matrix.
  int edges_;
  // Number of nodes on levels with arity k1
  int nodes_k1_;
  // Number of nodes on levels with arity k2
  int nodes_k2_;
  // Number of nodes on levels with arity kl, ie, level height_-1
  int nodes_kl_;

  // Struct to store the tree.
  struct Node {
    union {
      // Store the children of the level height_-1 (the leafs)
      BitString<char> *data_;
      // Pointers to children of internal nodes.
      Node **children_;
    };
  };
  // Create a node for the specified level using an appropiate k
  Node *CreateNode(int level);
  // Free memory allocated for n assuming it was built for the
  // specified level. Recursively delete children of internal nodes.
  void DeleteNode(Node *n, int level);

  // Root of the tree
  Node *root;
};

}  // namespace k2tree_impl

#endif  // SRC_K2TREEBUILDER_H_

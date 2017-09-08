#ifndef _TREENODE
#define _TREENODE

#include "huffman.h"

// Node in tree
typedef  struct  TreeNode{
  BYTE  value;
  int  freq;
  struct TreeNode *left, *right;
} TreeNode, *pTreeNode;

//-------------------   PROTOTYPE  ------------------------------------------------------------ 
pTreeNode newNode(BYTE , BYTE );
int isLeaf(pTreeNode );
void freeTree (pTreeNode node);

#endif

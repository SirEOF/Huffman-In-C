#include "tree.h"

//---------------------------------------------------------------------------------------- 
pTreeNode newNode(BYTE data, BYTE freq)
{
	  pTreeNode node = (pTreeNode) malloc(sizeof(TreeNode));
	  node->value = data;
	  node->freq = freq;
	  node->left = NULL;
	  node->right = NULL;  
	  return node;
}

//---------------------------------------------------------------------------------------- 
// Utility function to check if this node is leaf
int isLeaf(pTreeNode root)
{
    return !(root->left) && !(root->right) ;
}
//---------------------------------------------------------------------------------------- 
// 
void freeTree (pTreeNode node)
{
    //do nothing if passed a non-existent node
    if ( node )
	{
		//now onto the recursion
		freeTree(node->left );
		freeTree(node->right );
		free( node );
	}
}
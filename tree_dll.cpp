#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>

struct node
{
	int data;
	struct node *left,*right;
};

struct node * array_bst(int n, int *arr, int length) {
	struct node *newnode;
	if (n >= length)
		return NULL;
	newnode = (struct node *)malloc(sizeof(struct node));
	newnode->left = array_bst((2 * n) + 1, arr, length);
	newnode->data = arr[n];
	newnode->right = array_bst((2 * n) + 2, arr, length);
	return newnode;
}

void inOrder(struct node * myNode) {
	if (myNode) {
		inOrder(myNode->left);
		printf("%d ", myNode->data);
		inOrder(myNode->right);
	}
	return;
}

struct node * binarytree_dll(struct node *root){
	if (root == NULL)
		return root;
	if (root->left != NULL)
	{
		struct node *lchild = binarytree_dll(root->left);
		while (lchild->right != NULL)
			lchild = lchild->right;
		lchild->right = root;
		root->left = lchild;
	}
	if (root->right != NULL)
	{
		struct node *rchild = binarytree_dll(root->right);
		while (rchild->left!=NULL)
		{
			rchild = rchild->left;
		}
		rchild->left = root;
		root->right = rchild;
	}
	return root;
}

struct node * convertToDll(struct node *root)
{
	if (root == NULL)
		return root;
	root = binarytree_dll(root);
	while (root->left != NULL)
		root = root->left;
	return (root);
}

void printDll(struct node *root)
{
	while (root != NULL)
	{
		printf("%d ", root->data);
		root = root->right;
	}
}

int main()
{
	int a[] = {10,23,45,54,24,87};
	struct node *root = array_bst(0, a, 6);
	inOrder(root);
	printf("\nAfter converting the given tree::\n");
	struct node *head = convertToDll(root);
	printDll(head);
	getchar();
	return 0;
}
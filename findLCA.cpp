// problemSolving.cpp : Defines the entry point for the console application.
// finding LCA of two given nodes

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>

#define NODECOUNT 7

struct node {
	int data;
	struct node *left, *right;
};

struct node * array_bst(int n,int *arr,int length) {
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

struct node *find_LCA(struct node* root, int n1, int n2, int &v1, int &v2)
{
	if (root == NULL)
		return NULL;
	if (root->data == n1)
	{
		v1 = 1;
		return root;
	}
	if (root->data == n2)
	{
		v2 = 1;
		return root;
	}
	struct node *left_lca = find_LCA(root->left, n1, n2, v1, v2);
	struct node *right_lca = find_LCA(root->right, n1, n2, v1, v2);
	if (left_lca && right_lca)
		return root;
	// Otherwise check if left subtree or right subtree is LCA
	return (left_lca != NULL) ? left_lca : right_lca;
}


int key(struct node *root, int k)
{
	if (root == NULL)
		return 0;
	if (root->data == k || key(root->left, k) || key(root->right, k))
		return 1;
	return 0;
}

struct node * findLCA(struct node *root, int n1, int n2)
{
	int v1 = 0, v2 = 0;
	struct node *anc = find_LCA(root, n1, n2, v1, v2);
	if (v1 && v2 || v1 && key(anc, n2) || v2 && key(anc, n1))
		return anc;
	return NULL;
}

int main() {
	struct node *root = NULL;
	int i = 0;
	int arr[] = { 100, 23, 56, 3, 74, 13, 76, 2 };
	int length = 8;

	printf("Data in Array:\n");
	while (i < length) {
		printf("%d ", arr[i]);
		i++;
	}
	i = 0;
	root = array_bst(i,arr,length);
	printf("\nIn-Order:\n");
	inOrder(root);
	printf("\n");
	struct node *parent = findLCA(root, 2, 4);
	if (parent!=NULL)
		printf("LCA is %d", parent->data);
	else
	{
		printf("keys not present");
	}
	getchar();
	return 0;
}

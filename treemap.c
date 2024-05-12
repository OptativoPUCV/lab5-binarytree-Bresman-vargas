#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * new = createTreeNode(key, value);
    if (tree->root == NULL){
        tree->root = new;
        tree->current = new;
        return;
    }
    TreeNode * aux = tree->root;
    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)){
            return;
        }
        if(tree->lower_than(key, aux->pair->key)){
            if (aux->left == NULL){
                new->parent = aux;
                aux->left = new;
                tree->current = new;
                return;
            } else {
                aux = aux->left;
            }
        } else {
            if (aux->right == NULL){
                new->parent = aux;
                aux->right = new;
                tree->current = new;
                return;
            } else {
                aux = aux->right;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){

    TreeNode * aux = x;
    while(aux->left != NULL){
        aux = aux->left;
    }
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) {
        return; // Handle NULL pointers
    }

    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        tree->current = node->parent;
        free(node); // Deallocate memory
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode *child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child;
        } else if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        child->parent = node->parent;
        tree->current = child;
        free(node); // Deallocate memory
        return;
    }

    TreeNode *aux = minimum(node->right);
    node->pair->key = aux->pair->key;
    node->pair->value = aux->pair->value;
    removeNode(tree, aux);
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }if(tree->lower_than(key, aux->pair->key)){
            aux = aux->left;
        }else{
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    TreeNode * ub = NULL;
    while(aux != NULL){
        if(is_equal(tree, key, aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(key, aux->pair->key)){
            ub = aux;
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    if(ub != NULL){
        return ub->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * node = tree->root;
    while(node->left != NULL){
        node = node->left;
    }
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}

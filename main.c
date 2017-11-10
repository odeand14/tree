#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct TreeNode {
    char *name;
    void *data;
    unsigned int n;
    struct TreeNode **child;
    } Node ;
typedef Node NodeTree ;


NodeTree *createNode (int children, void *data, char *name)
{
    Node *node = (Node*) calloc(1, sizeof(Node));
    node->name = name;
    node->data = data;
    node->n = children;
    node->child = (Node**) calloc(children, sizeof(Node*)) ;
    return node ;
}

typedef void (*DataFreeFunc) (const void*) ;

void freeTree (NodeTree *tree, DataFreeFunc dFree) {
    unsigned i ;
    if( tree == NULL) return;
    for( i = 0 ; i < tree ->n; ++i) {
        freeTree(tree->child[i], dFree);
    }
    free(tree->child);
    if(dFree){
        dFree(tree->data);
    }
    free(tree);
}

void *createIntData(int data){
    int *ptr = (int*)calloc(1, sizeof(int));
    *ptr = data;
    return  ptr;
}

void *createDoubleData(double data) {
    double *ptr = (double*)calloc(1, sizeof(double));
    *ptr = data;
    return  ptr;
}

void *createStringData(const char *string){
    return strdup(string);
}



int appendChild(Node *root, void *data, char *name){
    root->n++;
    root->child = (Node**)realloc(root->child, (root->n) * sizeof(Node*));
    root->child[root->n - 1] = createNode(0, data, name);
    return root->n-1;
}

int insertChild(Node *root, int index, void *data, char *name) {
    unsigned i;
    root->n++;
    root->child = (Node**)realloc(root->child, (root->n) * sizeof(Node*));

    for (i = root->n-1; i > index; --i) {
        root->child[i] = root->child[i-1];
        root->child[i] = createNode(0, data, name);
    }
    return i;
}

void deleteChild(NodeTree *root, int index, DataFreeFunc dFree) {
    unsigned i;
    freeTree(root->child[index], dFree);

    for (i = index; i < root->n-1; ++i) {
        root->child[i] = root->child[i - 1];
    }

    root->n--;
    root->child = (Node**)realloc(root->child, (root->n) * sizeof(Node*));
}








int main() {
    NodeTree *tree = createNode(0, NULL, "root");




    printf("Hello, World!\n");
    return 0;
}
//
// Created by Andreas on 10.11.2017.
//

#ifndef TREE_TREEAPI_H
#define TREE_TREEAPI_H

#define OK      0
#define ERROR   1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory.h>

typedef struct Node {
    const char *name;
    const char *strData;
    int intData;
    unsigned int children;
    struct Node **child;
} Node ;
typedef Node NodeTree ;


typedef void (*DataFreeFunc) (const void*);

void forEach(Node *node, int level);
void freeTree (NodeTree *tree, DataFreeFunc dFree);
int createIntData(int data);
char *createStringData(const char *string);
int appendChild(Node *root, int intData, const char *strData, const char *name);
int appendChildNode(Node *root, Node *node);
int insertChild(Node *root, int index, int intData, char *strData, char *name);
void deleteChild(NodeTree *root, int index, DataFreeFunc dFree);
static long getLineLength(FILE *f);
int parseFile(Node *root, char *fileName);
Node *createNode (unsigned int children, int intData, const char *strData, const char *name);
int Insert(Node *pNode, const char *key, const char *strData, const int intData);




#endif //TREE_TREEAPI_H

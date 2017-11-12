//
// Created by Andreas on 10.11.2017.
//
#include "treeAPI.h"
#define TESTFIL "input.txt"

int main() {
    Node *tree = createNode(0, 0, NULL, "root");

//    Insert(tree, "strings.no.header", "Oppdatering", 0);
//    Insert(tree, "strings.no.text", "Oppdater programvaren", 0);


    parseFile(tree, TESTFIL);

    forEach(tree, 0);
//    freeTree(tree, freeStuff);

    deleteChild(tree, 1);
//    Insert(tree, "config.test", NULL, 10);

    forEach(tree, 0);

    printf("Hello, World!\n");
    printf("%i", tree->children);
    return 0;
}
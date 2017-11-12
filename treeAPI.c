#include "treeAPI.h"

Node *createNode (unsigned int children, int intData, const char *strData, const char *name) {
    Node *node = (Node*) calloc(1, sizeof(Node));
    node->name = name;
    node->strData = strData;
    node->intData = intData;
    node->children = children;
    node->child = (Node**) calloc(children, sizeof(Node*));
    return node;
}

int parseFile(NodeTree *root, char *fileName) {
    FILE *f;
    char *strData = NULL;
    char stringKey[32];
    int intData = 0;
    char *line = NULL, *character = NULL, *charKey = NULL;
    int irc = OK;

    f = fopen(fileName, "r");
    if (f == NULL) {
        printf("Cannot open %s.\n", fileName);
        return ERROR;
    }

    while(!feof(f) && irc == OK) {
        long length = getLineLength(f);
        line = malloc((size_t) length);
        if (line) {
            if (fscanf(f, "%[^\n]\n", line) == 1) {
                // We got the line, including the '\n'
                charKey = line;
                strData = NULL;
                intData = 0;
                while (isspace(*charKey)) ++charKey;      // Skip leading white space.
                character = strchr (charKey, '=');           // Look for a '=' ...
                if (character) {                         // We got it.
                    if (isspace(*(character - 1))) {
                        character--;
                        while (isspace(*character)) character--;  // Skip spaces before '='
                        character++;
                    }
                    int iLen = character - charKey;           // Length of keyword ...
                    if (iLen < sizeof(stringKey)) {    // Ok length?
                        strncpy (stringKey, charKey, iLen); // Yep. Copy keyword.
                        stringKey[iLen] = '\0';         // Add 0-termination.
                        character = strchr (character, '=');      // Find the '=' again...
                        ++character;                       // Skip it.
                        while (isspace(*character)) ++character;  // Skip spaces after '='
                        if (*character == '"') {           // Do we have a quote ?
                            charKey = ++character;              // Skip it and save pointer.
                            character = strchr(charKey, '"');   // Do we have another quote ?
                            if (character) {
                                *character = 0;              // Terminate string
                                strData = charKey;       // Save pointer to string value.
                            }
                            else {
                                printf ("Missing end quote in %s.\n", line);
                                irc = ERROR;
                            }
                        }
                        else {                      // We didn't have a quote.
                            if (sscanf(character, "%d", &intData) != 1) {
                                printf ("No numeric value as expected in %s.\n", line);
                                irc = ERROR;
                            }
                        }
                        if (irc == OK) {
                            // At this point we should have an szKey, pszValue or iValue:
                            printf ("Ready to insert %s = '%s' or %d\n", stringKey, strData, intData);

                            irc = Insert(root, stringKey, strData, intData);

                            if (irc != OK) {
                                printf ("Insert in list failed for %s.\n", line);
                            }
                        }
                    }
                    else {
                        printf ("Keyword too long in %s.\n", line);
                        irc = ERROR;
                    }
                }
                else {
                    printf ("Syntax error in %s.\n", line);
                    irc = ERROR;
                }
            }
            else {
                printf ("Could not read from file.\n");
                irc = ERROR;
            }
            free (line);
            line = NULL;
        } else {
            printf("malloc failed.\n");
            irc == ERROR;
        }

    }

}

int countChars(const char *s, const char c )
{
    return *s == '\0'
           ? 0
           : countChars( s + 1, c ) + (*s == c);
}

int compare(const Node *node, const char *token)
{
    return strcmp(node->name, token);
}

int Insert(Node *pNode, const char *key, const char *strData, const int intData) {

    const char delimiter[2] = ".";
    const char *token;
    int newNode = 0;
    int count = countChars(key, '.');
    Node *tmp;

    if (count != 0) {
        token = strtok((char *) key, delimiter);
        tmp = createNode(0, 0, NULL, createStringData(token));
    } else {
        token = key;
        tmp = createNode(0, createIntData(intData), createStringData(strData), createStringData(token));
    }

    if (count == 0) {
        return appendChildNode(pNode, tmp);
        return OK;
    } else {
        for (int i = 0; i < pNode->children; i++) {
            if (strcmp(pNode->child[i]->name, token) == 0) {
//                if (count > 1) {
//                    token = strtok(NULL, "");
//                    printf("token 1: %s\n", token);
//                }
                return Insert(pNode->child[i], token, strData, intData);
            }
        }
        newNode = appendChildNode(pNode, tmp);
        if (count > 0) {
            token = strtok(NULL, "");
            printf("token 2: %s\n", token);
        }
        return Insert(pNode->child[newNode], token, strData, intData);
    }
}

static long getLineLength(FILE *f) {
    char c;
    long l = 0;

    long linePosition = ftell(f);
    while (!feof(f)) {
        c = fgetc(f);
        ++l;
        if (c == '\n') break;
    }
    fseek(f, linePosition, SEEK_SET);
    return l;
}

void freeTree (NodeTree *tree, DataFreeFunc dFree) {
    unsigned i ;
    if( tree == NULL) return;
    for( i = 0 ; i < tree->children; ++i) {
        freeTree(tree->child[i], dFree);
    }
    free(tree->child);
    if(dFree){
        dFree(tree->strData);
    }
    free(tree);
}

int createIntData(int data){
    int *ptr = (int*)calloc(1, sizeof(int));
    *ptr = data;
    return  ptr;
}


char *createStringData(const char *string){
    return strdup(string);
}

void forEach(Node *node, int level)
{
    for (int i = 0; i < level; ++i) {
        printf("\t");
    }
    if(node->strData != NULL) {
        printf("%s = %s\n", node->name, node->strData);
    } else if(node->intData != NULL){
        printf("%s = %i\n", node->name, node->intData);
    } else {
        printf("(%s)\n", node->name);

        for (int i = 0; i < node->children; ++i) {
            forEach(node->child[i], level+1);
        }
    }
}

int appendChild(Node *root, int intData, const char *strData, const char *name){
    root->children++;
    root->child = (Node**)realloc(root->child, (root->children) * sizeof(Node*));
    root->child[root->children - 1] = createNode(0, intData, strData, name);
    return root->children-1;
}

int appendChildNode(Node *root, Node *node){
    root->children++;
    root->child = (Node**)realloc(root->child, (root->children) * sizeof(Node*));
    root->child[root->children - 1] = node;
    return root->children-1;
}

int insertChild(Node *root, int index, int intData, char *strData, char *name) {
    unsigned i;
    root->children++;
    root->child = (Node**)realloc(root->child, (root->children) * sizeof(Node*));

    for (i = root->children-1; i > index; --i) {
        root->child[i] = root->child[i-1];
        root->child[i] = createNode(0, intData, strData, name);
    }
    return i;
}

void deleteChild(NodeTree *root, int index, DataFreeFunc dFree) {
    unsigned i;
    freeTree(root->child[index], dFree);

    for (i = index; i < root->children-1; ++i) {
        root->child[i] = root->child[i - 1];
    }

    root->children--;
    root->child = (Node**)realloc(root->child, (root->children) * sizeof(Node*));
}


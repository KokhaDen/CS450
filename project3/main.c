#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define ALPHA 0.666666666666

typedef struct Node {
    char name[13];
    int banCount;
    int* servers;
    int serverCap;
    time_t lastBan;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct Tree {
    Node* root;
    int n; // current number of nodes in tree
    int q; // max number of nodes (not needed since no delete function?)
} Tree;

Node* createNode (char* name, int server, time_t banTime) {
    Node* node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Error: Failed to malloc for node when creating new node\n");
        return 0;
    }

    strcpy(node->name, name);
    node->banCount = 1;
    node->serverCap = 10;
    node->servers = malloc(node->serverCap * sizeof(int));
    if(node->servers == NULL) {
        fprintf(stderr, "Error: Failed to malloc for server array when creating new node\n");
        free(node);
        return 0;
    }
    node->servers[0] = server;
    node->lastBan = banTime;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int treeSize(Node *u) {
    if (u == NULL) {
        return 0;
    }

    return 1 + treeSize(u->left) + treeSize(u->right);
}

Node* flatten(Node* x, Node* y) {
    if(x == NULL) {
        return y;
    }

    x->right = flatten(x->right, y);
    return flatten(x->left, x);
}

Node* buildTree(int n, Node* x) {
    // builds a 1/2-weight-balanced tree of n nodes from a list of nodes headed by node x
    // assumed list of nodes has length at leasst n+1
    // returns the n+1st node of the list, s, modified so s.left (s left child) points to the root r of the n-node tree created
    if (n == 0) {
        x->left = NULL;
        return x;
    }

    Node* r = buildTree(n/2, x);
    Node* s = buildTree((n-1)/2, r->right);
    r->right = s->left;
    s->left = r;
    return s;
}

Node* rebuildTree(int n, Node* scapegoat) {
    // takes in input a pointer scapegoat to root of subtree to be rebuilt and size n of tree
    // returns root of subtree
    // rebuilt subtree is 1/2-weight-balanced
    Node w = {0};

    Node* z = flatten(scapegoat, &w);
    buildTree(n, z);

    return w.left;
}

Node* insert(Tree* tree, char* name, int server, time_t banTime) {
    Node* root = tree->root;
    if(root == NULL) {
        root = createNode(name, server, banTime);
        return root;
    }

    Node* pointer = root;
    for(;;) {
        if(pointer->left != NULL && strcmp(name, pointer->name) < 0) {
            pointer = pointer->left;
        } else if(pointer->left == NULL && strcmp(name, pointer->name) < 0) {
            pointer->left = createNode(name, server, banTime);
            tree->n++;
            tree->q++;
            return root;
        } else if(pointer->right != NULL && strcmp(name, pointer->name) > 0) {
            pointer = pointer->right;
        } else if(pointer->right == NULL && strcmp(name, pointer->name)> 0) {
            pointer->right = createNode(name, server, banTime);
            tree->n++;
            tree->q++;
            return root;
        } else if(strcmp(name, pointer->name) == 0) {
            if(banTime > pointer->lastBan) {
                pointer->lastBan = banTime;
            }

            for(int i = 0; i < pointer->banCount; i++) {
                if (pointer->servers[i] == server) {
                    return root;
                }
            }

            if (pointer->banCount == pointer->serverCap) {
                int newCap = pointer->serverCap * 2;
                int* temp = realloc(pointer->servers, newCap * sizeof(int));

                if (temp == NULL) {
                    fprintf(stderr, "Error: Failed to realloc for server array\n");
                    exit(EXIT_FAILURE);
                }

                pointer->servers = temp;
                pointer->serverCap = newCap;
            }

            pointer->servers[pointer->banCount] = server;
            pointer->banCount++;
        }
    }
}

void freeNode(Node* node) {
    if (node == NULL) {
        return;
    }

    freeNode(node->left);
    freeNode(node->right);

    free(node->servers);
    free(node);
}

void freeTree(Tree* tree) {
    if (tree == NULL) {
        return;
    }

    freeNode(tree->root);
    tree->root = NULL;
    tree->n = 0;
    tree->q = 0;
}

int main() {
    Node* root = createNode("Bob", 0, 0);
    Tree* tree = malloc(sizeof(Tree));

    if (tree == NULL) {
        fprintf(stderr, "Error: Failed to malloc for tree\n");
        return EXIT_FAILURE;
    }

    *tree = (Tree){root, 1, 1};

    insert(tree, "Alice", 812, 0);
    insert(tree, "Catchy", 890, 0);

    printf("Tree size: %d\n", tree->n);

    Node dummy = {0};
    Node* start = flatten(root, &dummy);
    Node* head = start;

    while (start->right != NULL) {
        printf("%s\n", start->name);
        start = start->right;
    }

    Node* dummy2 = buildTree(3, head);

    Node* newRoot = dummy2->left;

    printf("Root: %s\n", newRoot->name);
    printf("Left: %s\n", newRoot->left->name);
    printf("Right: %s\n", newRoot->right->name);

    freeTree(tree);

    return 0;
}
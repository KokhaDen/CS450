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

int maxDepth(int q) {
    if (q <= 1) {
        return 0;
    }
    
    return (int)floor(log(q) / log(1 / ALPHA) + 1);
}

Node* insert(Tree* tree, char* name, int server, time_t banTime) {
    Node* root = tree->root;
    if(root == NULL) {
        root = createNode(name, server, banTime);
        tree->root = root;
        tree->n++;
        tree->q++;
        return root;
    }

    if(root == NULL) {
        return NULL;
    }

    Node* history[1000];
    int historyCount = 0;

    Node* pointer = root;
    for(;;) {
        history[historyCount] = pointer;
        historyCount++;

        if(pointer->left != NULL && strcmp(name, pointer->name) < 0) {
            pointer = pointer->left;

        } else if(pointer->left == NULL && strcmp(name, pointer->name) < 0) {
            Node* newNode = createNode(name, server, banTime);
            if (newNode == NULL) {
                return tree->root;
            }

            pointer->left = newNode;
            tree->n++;
            tree->q++;

            if (historyCount > maxDepth(tree->q)) {
                Node* scapegoat = NULL;
                int j;

                for(j = historyCount - 2; j >= 0; j--) {

                    if(treeSize(history[j + 1]) > (ALPHA * treeSize(history[j]))) {
                        scapegoat = history[j];
                        break;
                    }
                }

                if (scapegoat != NULL) {
                    Node* rebuiltNode = rebuildTree(treeSize(scapegoat), scapegoat);

                    if(j == 0) {
                        tree->root = rebuiltNode;

                    } else {
                        if (history[j-1]->left == scapegoat) {
                            history[j-1]->left = rebuiltNode;

                        } else {
                            history[j-1]->right = rebuiltNode;
                        }
                    }
                }
            }
            return tree->root;

        } else if(pointer->right != NULL && strcmp(name, pointer->name) > 0) {
            pointer = pointer->right;

        } else if(pointer->right == NULL && strcmp(name, pointer->name)> 0) {
            Node* newNode = createNode(name, server, banTime);
            if (newNode == NULL) {
                return tree->root;
            }
            pointer->right = newNode;
            tree->n++;
            tree->q++;

            if (historyCount > maxDepth(tree->q)) {
                Node* scapegoat = NULL;
                int j;

                for(j = historyCount - 2; j >= 0; j--) {

                    if(treeSize(history[j + 1]) > (ALPHA * treeSize(history[j]))) {
                        scapegoat = history[j];
                        break;
                    }
                }

                if (scapegoat != NULL) {
                    Node* rebuiltNode = rebuildTree(treeSize(scapegoat), scapegoat);

                    if(j == 0) {
                        tree->root = rebuiltNode;

                    } else {
                        if (history[j-1]->left == scapegoat) {
                            history[j-1]->left = rebuiltNode;

                        } else {
                            history[j-1]->right = rebuiltNode;
                        }
                    }
                }
            }
            return tree->root;

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
            return tree->root;
        }
    }
}

Node* search(Tree* tree, char* name) {
    Node* root = tree->root;
    if(root == NULL) {
        return NULL;
    }

    Node* pointer = root;
    for(;;) {
        if(pointer->left != NULL && strcmp(name, pointer->name) < 0) {
            pointer = pointer->left;
        } else if(pointer->left == NULL && strcmp(name, pointer->name) < 0) {
            return NULL;
        } else if(pointer->right != NULL && strcmp(name, pointer->name) > 0) {
            pointer = pointer->right;
        } else if(pointer->right == NULL && strcmp(name, pointer->name)> 0) {
            return NULL;
        } else if(strcmp(name, pointer->name) == 0) {
            return pointer;
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Please provide file to build tree\n");
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open file\n");
        return EXIT_FAILURE;
    }

    Tree* tree = malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "Error: Failed to malloc for tree\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    *tree = (Tree){NULL, 0, 0};

    char name[13];
    int server;
    long long time;

    while (fscanf(fp, "%12s %d %lld", name, &server, &time) == 3) {
        time_t banTime = (time_t)time;
        insert(tree, name, server, banTime);
    }

    fclose(fp);

    while (scanf("%12s", name) == 1) {
        Node* node = search(tree, name);

        if (node != NULL) {
            printf("%s was banned from %d servers. most recently on: %lld\n",
               node->name, node->banCount, (long long)node->lastBan);
        } else {
            printf("%s is not currently banned from any servers.\n", name);
        }
    }

    freeTree(tree);
    free(tree);

    return 0;
}
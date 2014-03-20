/*
  tree.h 
  Written: Ray Brooks
*/
 
#ifndef tree_h
#define tree_h
 
#include "Energia.h"

enum Branch {
  branch_left, branch_right
};

struct Node {
 char name[10];
 void (*_callback)(void);
 Node *branches[2];
};

class Tree
{
  public:
    Tree();
    ~Tree();
    void initialize();
    static Node* createNode(char *text, void (*cb_func)(void));
    Node* getRoot(void);
    void pushNode(Node *root, Node *node, Branch branch);
    Node* getNext(Node *node, Branch branch);
    void getName(Node *node, char* name);
  private:
    Node* _root;
    void destroy(Node *node);
    static void callback(void (*cb_func)(void), void *thisPointer);
};
 
#endif

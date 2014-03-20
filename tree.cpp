#include "tree.h"

/******************************/
/* Tree
/******************************/

Tree::Tree() {

}

void Tree::initialize() {
  _root = createNode("Bsci", NULL);
}

Node* Tree::getRoot(void) {
  return _root;
}

void Tree::destroy(Node *node)
{
  if(node != 0)
  {
    Serial.print("destorying node: ");
    Serial.println(node->name);
    destroy(node->branches[branch_left]);
    destroy(node->branches[branch_right]);
    delete node;
  }
}

Node* Tree::createNode(char *text, void (*cb_func)(void)) {
  Node* node = new Node;
  strncpy(node->name, text, 10);
  node->_callback = cb_func;
  node->branches[branch_left] = 0;
  node->branches[branch_right] = 0;
  return node;
}

void Tree::pushNode(Node *root, Node *node, Branch branch) {

  while(root->branches[branch] != 0){
    root = root->branches[branch];
  }

  root->branches[branch] = node;

}


Node* Tree::getNext(Node *node, Branch branch) {
  if (node->branches[branch] != 0) {
    return node->branches[branch];
  }

  return 0;
}

void Tree::getName(Node *node, char *name) {
  strncpy(name, node->name, 5);
  return;
}


void Tree::callback(void (*cb_func)(void), void *thisPointer) {
/*  Menu* self = static_cast<Menu*>(thisPointer);
  self->goHome(); */
}


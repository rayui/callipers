#include "menu.h"
#include "utils.h"
#include "tree.h"

/******************************/
/* Menu controller
/******************************/

Menu::Menu() {

}

void Menu::initialize() {
  _tree = new Tree;
  _tree->initialize();
  _currentNode = _tree->getRoot();
}

Node* Menu::goHome() {
  char name[10];
  //navigate to root of tree
  _currentNode = _tree->getRoot();
  _tree->getName(_currentNode, name);
  Serial.print("current node: ");
  Serial.println(name);
  return _currentNode;
}

Node* Menu::moveToNextSibling() {
  //get next menu item in LEFT list and return text
  return moveToNextNode(branch_left);
}

Node* Menu::moveToNextChild() {
  //get next menu item in RIGHT list and return text
  return moveToNextNode(branch_right);
}

void Menu::activateCurrentSelection() {
  //execute callback on current node
  _currentNode->_callback();
}

void Menu::addNode(char *text, void (*cb_func)(void), Branch branch) {
  Node *siblingNode = Tree::createNode(text, cb_func);
  _tree->pushNode(_currentNode, siblingNode, branch);
}

void Menu::getCurrentNodeName(char *name) {
  strncpy(name, _currentNode->name, 10);
}

/** private */

Node* Menu::moveToNextNode(Branch branch) {
  char name[10];
  
  Node* temp = _tree->getNext(_currentNode, branch);
  if (temp != 0) {
    _currentNode = temp;
    _tree->getName(_currentNode, name);
    
    Serial.print("current node: ");
    Serial.println(name);
    return _currentNode;
  }
  return goHome();
}

void *Menu::callback(void) {
  
};

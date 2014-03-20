/*
  menu.h - library to create and control heirarchical menu
  (c) R Brooks 2014
*/
#ifndef menu_h
#define menu_h

#include <Energia.h>
#include "tree.h"

//define callback types for menu
//these will be passed in from the main program
//they will take no parameters
//they will return an int which describes the success state of the call

static void menuCallback(void *thisPointer);

class Menu
{
  public:
    Menu();
    void initialize();
    Node* goHome();
    Node* moveToNextSibling();
    Node* moveToNextChild();
    void activateCurrentSelection();
    void addNode( char *text, void (*cb_func)(void), Branch branch);
    void getCurrentNodeName(char *name);
  private:
    Tree* _tree;
    Node* _currentNode;
    Node* moveToNextNode(Branch branch);
    void *callback(void);
};

#endif

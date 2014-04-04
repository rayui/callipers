/*
  list.h 
  Written: Ray Brooks
*/
 
#ifndef list_h
#define list_h
 
#include "Energia.h"
#include "utils.h"

template <class Base> struct ListNode {
  public:
    ListNode(Base *node);
    ~ListNode();
    void setNext(ListNode<Base>* next);
    void setNode(Base* next);
    ListNode<Base>* getNext(void);
    Base* getNode(void);
  private:
    Base *_node;
    ListNode<Base> *_next;
};

template <class Base> class List
{
  public:
    List();
    ~List();
    void initialize();
    void pushNode(Base *node);
    Base* popNode();
    unsigned char hasNext();
    Base* moveToNext();
    Base* moveToHead();
    int getLength();
    Base* getNextNode();
    Base* getLastNode();
    Base* getCurrentNode();
    void empty();
    Base* getRoot(void);
    Base* spliceRoot();
    Base* spliceCurrent();
    void insertAtRoot(Base* node);
  private:
    ListNode<Base>* _root;
    ListNode<Base>* _lastNode;
    ListNode<Base>* _currentNode;
    ListNode<Base>* _getRoot(void);
    ListNode<Base>* _spliceNext(ListNode<Base>* node);
};

#include "list_impl.h"
 
#endif

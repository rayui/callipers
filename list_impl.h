/******************************/
/* ListNode
/******************************/

template <class Base> ListNode<Base>::ListNode(Base *node) {
  _node = node;
  _next = 0; 
}

template <class Base> ListNode<Base>::~ListNode() {
  delete _node;
}

template <class Base> ListNode<Base>* ListNode<Base>::getNext(void) {
  return _next;
}

template <class Base> void ListNode<Base>::setNext(ListNode* next) {
  _next = next;
}

template <class Base> Base* ListNode<Base>::getNode(void) {
  return _node;
}

template <class Base> void ListNode<Base>::setNode(Base* node) {
  _node = node;
}


/******************************/
/* List
/******************************/

template <class Base> List<Base>::List() {
  _root = 0;
  _lastNode = 0;
  _currentNode = _root;
}

template <class Base> List<Base>::~List() {
  while (_root != 0) {
    popNode();
  }
  delete _root;
}

template <class Base> Base* List<Base>::getRoot(void) {
  if (_root == 0) {
    return 0;
  }
  
  return _root->getNode();
}

template <class Base> Base* List<Base>::spliceRoot() {
  ListNode<Base>* temp = 0;
  
  if (_root == 0) {
    return 0;
  }
  
  if (_root->getNext() == 0) {
    delete _root;
    _root = 0;
    _currentNode = 0;
    _lastNode = 0;
  } else {
    temp = _root;
    _root = _root->getNext();
    delete temp;
  }
  
  return _root->getNode();
}

template <class Base> Base* List<Base>::spliceCurrent() {  
  ListNode<Base>* temp = 0;
  Base *root = 0;
  
  if (_root == 0) {
    return 0;
  }

  if (_currentNode == 0) {
    return 0;
  }
  
  if (_currentNode == _root) {
    root = spliceRoot();
    _currentNode = _root;
   return root; 
  }
  
  temp = _root;
  
  while(temp != 0) {
    if (temp->getNext() == _currentNode) {
      _currentNode = _spliceNext(temp);
      temp = 0;
    } else {
      temp = temp->getNext();
    }
  }
  
  return _currentNode->getNode();
}

template <class Base> ListNode<Base>* List<Base>::_spliceNext(ListNode<Base>* node) {  
  ListNode<Base>* next;
  ListNode<Base>* nextNext;
  
  if (node == 0) {
    return 0;
  }
 
  next = node->getNext();
  
  if (next != 0) {
    nextNext = next->getNext();
    if (nextNext != 0) {
      node->setNext(nextNext);
      delete next;
    } else {
      node->setNext(0);
      delete next;
    }
  }
  
  return node->getNext();
}


template <class Base> Base* List<Base>::getNextNode(void) {
  if (_currentNode == 0 || _currentNode->getNext() == 0) {
    return 0;
  }
  
  return _currentNode->getNext()->getNode();
}

template <class Base> ListNode<Base>* List<Base>::_getRoot(void) {
  return _root;
}

template <class Base> void List<Base>::pushNode(Base *node) {
  
  ListNode<Base>* newNode = new ListNode<Base>(node);

  if(_root == 0) {
    _root = newNode;
  } else {
    _lastNode->setNext(newNode);
  }
  
  _lastNode = newNode;

}

template <class Base> Base* List<Base>::popNode() {
  
  ListNode<Base>* temp = _root;
  ListNode<Base>* last = temp;
  Base* poppedNode = 0;
    
  if (_root == 0) {
    return 0; 
  }

  while(temp->getNext() != 0) {
    last = temp;
    temp = temp->getNext();
  }
  
  _lastNode = last;
  _lastNode->setNext(0);
  
  poppedNode = temp->getNode();  
  
  if (temp == _root) {
    delete _root;
    _root = 0;
  } else {
    delete temp;
  }

  return poppedNode;

}

template <class Base> void List<Base>::insertAtRoot(Base *node) {
  
  ListNode<Base>* newNode = new ListNode<Base>(node);

  newNode->setNext(_root);
  _root = newNode;

}

template <class Base> Base* List<Base>::moveToHead() {
  _currentNode = _root;
    
  if (_currentNode != 0) {
    return _currentNode->getNode();    
  }
  
  return 0;
}

template <class Base> Base* List<Base>::moveToNext() {
  ListNode<Base>* next;
  
  if (_currentNode == 0) {
    return 0;  
  }
  
  next = _currentNode->getNext();
    
  if (next != 0) {
    _currentNode = next;
    return _currentNode->getNode();
  }

  return 0;
}

template <class Base> Base* List<Base>::getCurrentNode() {   
  if (_currentNode != 0) {
    return _currentNode->getNode();    
  }
  
  return 0;
}

template <class Base> unsigned char List<Base>::hasNext() {
  if (_currentNode == 0) {
    return 0;  
  }
  if (_currentNode->getNext() != 0) {
   return 1;
  }
 return 0; 
}

template <class Base> void List<Base>::empty() {
  while(popNode() != 0) {}
  _lastNode = 0;
  _currentNode = 0;
}

template <class Base> int List<Base>::getLength() {
  int i = 0;
  ListNode<Base>* temp = _root;
  
  if(temp == 0) {
    return 0; 
  }
  
  temp = temp->getNext();
  while(temp != 0) {
    i++;
    temp = temp->getNext(); 
  }
  
  return i + 1;
}

template <class Base> Base* List<Base>::getLastNode() {
  if (_lastNode != 0) {
    return _lastNode->getNode();
  }
  
  return 0;
}

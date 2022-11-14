#ifndef RBBST_H
#define RBBST_H
 
#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"
 
struct KeyError { };
 
/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();
 
    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);
 
    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;
 
protected:
    int8_t balance_;    // effectively a signed char
};
 
/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/
 
/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first ed.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
 
}
 
/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
 
}
 
/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}
 
/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}
 
/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}
 
/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}
 
/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}
 
/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}
 
 
/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/
 
 
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
 
    // Add helper functions here
    virtual void insertHelper(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* child);
    virtual void removeHelper(AVLNode<Key, Value>* n, int diff);
    virtual void rotateToTheRight(AVLNode<Key,Value>* n);
    virtual void rotateToTheLeft(AVLNode<Key,Value>* n);
 
};
 
/*
 * Recall: If key is already in the tree, you should
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
 
  //initializing variables
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*> (this->internalFind(new_item.first));
  AVLNode<Key, Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
  AVLNode<Key, Value>* currNodeParent = static_cast<AVLNode<Key, Value>*>(this->root_);
 
  if(this->root_ == nullptr){ //tree is empty, root is the new node
    this->root_ = newNode;
    return;
  }
  if(temp != nullptr){ //node exists
    temp->setValue(new_item.second);
    return;
  }
  else{ //node does not exist, create one and make the connections
    while(currNodeParent != nullptr){ //walk down tree
      if(currNodeParent->getKey() > newNode->getKey()){ //traverse left
        if(currNodeParent->getLeft() == nullptr){ //if there is room, insert new node to the left
          currNodeParent->setLeft(newNode);
          newNode->setParent(currNodeParent);
          break;
        }
        else{
          currNodeParent = currNodeParent->getLeft();
        }
      }
      else{ //traverse right
        if(currNodeParent->getRight() == nullptr){ //if there is room, insert new node to the right
          currNodeParent->setRight(newNode);
          newNode->setParent(currNodeParent);
          break;
        }
        else{
          currNodeParent = currNodeParent->getRight();
        }
      }
    }
    if(currNodeParent->getBalance() == 1 || currNodeParent->getBalance() == -1){
      currNodeParent->setBalance(0);
    }
    else if(currNodeParent->getBalance() == 0){
      if(currNodeParent->getRight() == newNode){
        currNodeParent->updateBalance(+1);
      }
      else if(currNodeParent->getLeft() == newNode){
        currNodeParent->updateBalance(-1);
      }
      insertHelper(currNodeParent, newNode);
    }
  }
 
}
 
template<class Key, class Value>
void AVLTree<Key, Value>::insertHelper(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child){
    if(parent == this->root_ || parent == nullptr){ //basecase
      return;
    }

		AVLNode<Key, Value>* parentParent = parent -> getParent(); //using parent->getParent()->getLeft() causes errors
    if(parentParent->getLeft() == parent){ //the parent is the left child of its parent
        parentParent->updateBalance(-1);
 
        if(parentParent->getBalance() == 0){ //balanced
            return;
        }
        else if(parentParent->getBalance() == -1){ //recursive call to find more imbalance
            insertHelper(parentParent, parent);
        }
        else if(parentParent->getBalance() == -2){
            if(parent->getRight() == child){
                if(child->getBalance() == 0){
                    parentParent->setBalance(0);
                    parent->setBalance(0);
                }
                else if(child->getBalance() == -1){
                    child->setBalance(0);
                    parent->setBalance(0);
                    parentParent->setBalance(1);
                }
                else if(child->getBalance() == 1){
                    child->setBalance(0);
                    parent->setBalance(-1);
                    parentParent->setBalance(0);
                }
                rotateToTheLeft(parent); //rotate to fix zigzag
                rotateToTheRight(parentParent); //rotate to fix zigzag
            }
            else if(parent->getLeft() == child){
                parentParent->setBalance(0);
                parent->setBalance(0);
                rotateToTheRight(parentParent); //rotate to fix zigzag
            }
            return; //balanced
        }
    }
    else if(parentParent->getRight() == parent){ //parent is the right child of its parent
        parentParent->updateBalance(+1);
 
        if(parentParent->getBalance() == 0){ //balanced
            return;
        }
        else if(parentParent->getBalance() == 1){ //recursive call to find more imbalance
            insertHelper(parentParent, parent);
        }
        else if(parentParent->getBalance() == 2){
            if(parent->getLeft() == child){
                if(child->getBalance() == 0){
                    parentParent->setBalance(0);
                    parent->setBalance(0);
                }
                else if(child->getBalance() == -1){
                    child->setBalance(0);
                    parent->setBalance(1);
                    parentParent->setBalance(0);
                }
                else if(child->getBalance() == 1){
                    child->setBalance(0);
                    parent->setBalance(0);
                    parentParent->setBalance(-1);
                }
                rotateToTheRight(parent); //rotate to fix zigzag
                rotateToTheLeft(parentParent); //rotate to fix zigzag
            }
            else if(parent->getRight() == child){
                  parentParent->setBalance(0);
                  parent->setBalance(0);
                  rotateToTheLeft(parentParent); //rotate to fix zigzag
            }
            return; //balanced
        }
    }
 
}
 
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  // TODO
 
  //initializing variables
  AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>* > (this->internalFind(key));
  AVLNode<Key, Value>* temp2 = static_cast<AVLNode<Key, Value>* > (this->predecessor(temp));
  int8_t diff = 0;
 
 
  if(this->root_ == nullptr){
    return;
  }
  if(this->root_->getLeft() == nullptr && this->root_->getRight() == nullptr && this->root_->getKey() == key){ //only one node in the tree
    delete this->root_;
    this->root_ = nullptr;
    return;
  }
 
  if(temp == nullptr){ //node does not exist
    return;
  }
  else{
    if(temp->getRight() != nullptr && temp->getLeft() != nullptr){ //two children
      nodeSwap(temp, temp2);
    }
		AVLNode<Key, Value>* tempParent = temp->getParent();
    if(temp->getLeft() == nullptr && temp->getRight() == nullptr){ //no children
      if(tempParent != nullptr){
        if(tempParent->getLeft() == temp){
          tempParent->setLeft(nullptr);
          diff = 1;
        }
        else{
          tempParent->setRight(nullptr);
          diff = -1;
        }
      }
      else if(tempParent == nullptr){
        this->root_ = nullptr;
      }
    }
    else if(temp->getRight() == nullptr && temp->getLeft() != nullptr){ //left child exists, right does not
      AVLNode<Key, Value>* tempLeft = temp->getLeft(); //temp->getLeft()->setParent() causes errors
      if(tempParent == nullptr){ //node is the root
        tempLeft->setParent(nullptr);
        this->root_ = tempLeft;
      }
      else{
        if(tempParent->getLeft() == temp){ //deleting parent left node
          tempParent->setLeft(tempLeft);
          tempLeft->setParent(tempParent);
          diff  = 1;
        }
        else{
          tempParent->setRight(tempLeft); //deleting parent right node
          tempLeft->setParent(tempParent);
          diff = -1;
        }
      }
    }
    else if (temp->getRight() != nullptr && temp->getLeft() == nullptr){ //right child exists, left does not
      AVLNode<Key, Value>* tempRight = temp -> getRight(); //temp->getRight()->setParent() causes errors
      if(tempParent == nullptr){ //node is the root
        tempRight->setParent(nullptr);
        this->root_ = tempRight;
      }
      else{
        if(tempParent->getLeft() == temp){ //deleting parent left node
          tempParent->setLeft(tempRight);
          tempRight->setParent(tempParent);
          diff  = 1;
        }
        else{
          tempParent->setRight(tempRight); //deleting parent right node
          tempRight->setParent(tempParent);
          diff = -1;
        }
      }
    }
    delete temp;
    removeHelper(tempParent, diff);
  }
}


 
template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* n, int diff){
 
  int difference = -1;

  if(n == nullptr){
    return;
  }

	AVLNode<Key, Value>* nParent = n->getParent();
  if(nParent != nullptr){
    if(nParent->getLeft() == n){
      difference = 1;
    }
  }
 

  if(diff == 1){
    if(n->getBalance() + diff == 2){ //fix different forms of zigzags
      AVLNode<Key, Value>* nRight = n->getRight();
      if(nRight->getBalance() == 0){
        rotateToTheLeft(n);
        nRight->setBalance(-1);
        n->setBalance(1);
        return;
      }
      else if(nRight->getBalance() == -1){
        AVLNode<Key, Value>* nRightLeft = nRight->getLeft();
        rotateToTheRight(nRight);
        rotateToTheLeft(n);
        if(nRightLeft->getBalance() == 0){
          n->setBalance(0);
          nRight->setBalance(0);
        }
        else if(nRightLeft->getBalance() == -1){
          n->setBalance(0);
          nRight->setBalance(1);
          nRightLeft->setBalance(0);
        }
        else if(nRightLeft->getBalance() == 1){
          n->setBalance(-1);
          nRight->setBalance(0);
          nRightLeft->setBalance(0);
        }
        removeHelper(nParent, difference);
      }
      else if(nRight->getBalance() == 1){
        rotateToTheLeft(n);
        n->setBalance(0);
        nRight->setBalance(0);
        removeHelper(nParent, difference); //recurse
      }
    }
    else if(n->getBalance() + diff == 0){
      n->setBalance(0);
      removeHelper(nParent, difference); //recurse
    }
    else if(n->getBalance() + diff == 1){
      n->setBalance(1);
      return;
    }
  }
  else if(diff == -1){
    if(n->getBalance() + diff == -2){ //fix different forms of zigzags
      AVLNode<Key, Value>* nLeft = n->getLeft();
      if(nLeft->getBalance() == 0){
        rotateToTheRight(n);
        nLeft->setBalance(1);
        n->setBalance(-1);
        return;
      }
      else if(nLeft->getBalance() == 1){
        AVLNode<Key, Value>* nLeftRight = nLeft->getRight();
        rotateToTheLeft(nLeft);
        rotateToTheRight(n);
        if(nLeftRight->getBalance() == 0){
          n->setBalance(0);
          nLeft->setBalance(0);
          nLeftRight->setBalance(0);
        }
        else if(nLeftRight->getBalance() == -1){
          n->setBalance(1);
          nLeft->setBalance(0);
          nLeftRight->setBalance(0);
        }
        else if(nLeftRight->getBalance() == 1){
          n->setBalance(0);
          nLeft->setBalance(-1);
          nLeftRight->setBalance(0);
        }
        removeHelper(nParent, difference);
      }
      else if(nLeft->getBalance() == -1){
        rotateToTheRight(n);
        n->setBalance(0);
        nLeft->setBalance(0);
        removeHelper(nParent, difference); //recurse
      }
    }
    else if(n->getBalance() + diff == 0){
      n->setBalance(0);
      removeHelper(nParent, difference); //recurse
    }
    else if(n->getBalance() + diff == -1){
      n->setBalance(-1);
      return;
    }
  }
}


 
template<class Key, class Value>
void AVLTree<Key, Value>::rotateToTheRight (AVLNode<Key,Value>* n){
 
  AVLNode<Key,Value>* nLeft = n->getLeft(); //the left child of n
  AVLNode<Key, Value>* nLeftRight = n->getLeft()->getRight(); //the right child of the left child of n
 
  if(n->getParent() == NULL){ //if current node is the node that is the pivot
    this->root_ = nLeft; //the left child of n becomes the root of the subtree and its parent becomes the right child
    nLeft->setParent(nullptr);
  }
  else{
    //the parent of n is not the parent of n's left child
    AVLNode<Key,Value>* nParent = n->getParent();
    nLeft->setParent(nParent);
    if(nParent->getRight() == n){
      nParent->setRight(nLeft);
    }
    else if(nParent->getLeft() == n){
      nParent->setLeft(nLeft);
    }
  }
  //reassign n's parents and the new subtree node's right child,
  //and the right child of the new root becomes the left child of
  //n, completing the rotation.
  n->setParent(nLeft);
  nLeft->setRight(n);
  n->setLeft(nLeftRight);
  if(nLeftRight != nullptr){
    nLeftRight->setParent(n);
  }
}
 
template<class Key, class Value>
void AVLTree<Key, Value>::rotateToTheLeft(AVLNode<Key,Value>* n){
 
  AVLNode<Key,Value>* nRight = n->getRight(); //the right child of n
  AVLNode<Key, Value>* nRightLeft = n->getRight()->getLeft(); //the left child of the right child of n
 
  if(this->root_ == n){ //if current node is the node that is the pivot
    this->root_ = nRight; //the right child of n becomes the root of the subtree and its parent becomes the left child
    nRight->setParent(nullptr);
  }
  else{
    //the parent of n is not the parent of n's left child
    AVLNode<Key,Value>* nParent = n->getParent();
    nRight->setParent(nParent);
    if(nParent->getRight() == n){
      nParent->setRight(nRight);
    }
    else if(nParent->getLeft() == n){
      nParent->setLeft(nRight);
    }
  }
    //reassign n's parents and the new subtree node's left child,
    //and the left child of the new root becomes the right child of
    //n, completing the rotation.
  n->setParent(nRight);
  nRight->setLeft(n);
  n->setRight(nRightLeft);
  if(nRightLeft != nullptr){
    nRightLeft->setParent(n);
  }
}
 
 
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}
 
 
#endif
 


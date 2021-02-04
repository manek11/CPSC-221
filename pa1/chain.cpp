#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  Node *tmp;
  while(this->head_ != NULL){
    tmp = this->head_;
    this->head_ = this->head_->next;
    delete tmp;
  }
  this->length_ = 0;
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  
  Node *newn = new Node(ndata);
  if (p == NULL && this->head_ == NULL)
  {
    this->head_ = newn;
    //cout << "reacched if" << endl;
    this->length_++;
    return newn;
  }
  else if(p == NULL){ /*Newn is HEAD*/
    newn->next = this->head_;
    newn->prev = NULL;
    this->head_->prev = newn;
    this->head_ = newn; /*this has to be the last*/
    this->length_++;
    return newn;
  }
  else 
  { 
    Node *pnext = p->next;
    //cout << "reached else" << endl;
    Node *pnextprev;
    if(pnext != NULL){
    pnextprev = pnext->prev;
    }
    newn->next = pnext;
    pnextprev = newn;

    p->next = newn;
    newn->prev = p;

    this->length_++;
    return newn;
  }

}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if(p == NULL || q  == NULL || p == q){
    return;
  }
  Node *pprev = p->prev;
  Node *qprev = q->prev;
  const Block pdata = p->data;
  const Block qdata = q->data;
  if (p->prev != NULL) pprev->next = p->next;
  if (p->next != NULL) p->next->prev = pprev;
  delete p;
  if (q->prev != NULL) qprev->next = q->next;
  if (q->next != NULL) q->next->prev = qprev;
  delete q;
  
  Node *qcurr = insertAfter(pprev, qdata);
  Node *pcurr = insertAfter(qprev, pdata);

  return;

  
  
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node *tmp;
  while(this->head_ != NULL){
    tmp = this->head_;
    this->head_ = this->head_->next;
    delete tmp;
  }
  this->length_ = 0;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
    Chain *copied = new Chain();
    Node *tmp = other.head_;
    for( int i=0; i < other.size(); i++ ) {
    copied->head_ = insertAfter(copied->head_, tmp->data);
    tmp = tmp->next;
  }
}

/* Modifies the current chain: 
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block 
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
void Chain::unscramble() {
  /* your code here */
}

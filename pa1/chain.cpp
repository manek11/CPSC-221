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
  /* your code here */

  if( p==q || p==NULL || q==NULL){
    return;
  }
  if(p == head_ && q->next == NULL){
     Node *qprv = q->prev;
     Node *pnxt = p->next;

     head_ = q;
     q->prev = NULL;
     q->next = pnxt;
     pnxt->prev = q;

     p->next = NULL;
     p->prev = qprv;
     qprv->next = p;

     return;
  }

    if(q == head_ && p->next == NULL){
     Node *pprv = p->prev;
     Node *qnxt = q->next;

     head_ = p;
     p->prev = NULL;
     p->next = qnxt;
     qnxt->prev = p;

     q->next = NULL;
     q->prev = pprv;
     pprv->next = q;

     return;
  }
  
  if (p->next!=NULL && q->next!=NULL && p->prev!=NULL && q->prev!=NULL){
    //General case (No head or tail) also works for adjacent cases
    //Node *p_prev_next = p->prev->next;
    Node *p_prev = p->prev;
    Node *p_next = p->next;
    //Node *p_next_prev = p->next->prev;
    Node *old_p = p;

    //Node *q_prev_next = q->prev->next;
    Node *q_prev = q->prev;
    Node *q_next = q->next;
    //Node *q_next_prev = q->next->prev;
    Node *old_q = q;

    p->prev->next = q;
    q->prev = p_prev;
    q->next = p_next;
    p->next->prev = old_q;

    q->prev->next = old_p;
    p->prev = q_prev;
    p->next = q_next;
    q->next->prev = old_p;
    
    return;  
  }

  if(p == head_){
     Node *qprv = q->prev;
     Node *qnxt = q->next;
     //Node *pprv = p->prev;
     Node *pnxt = p->next;

     head_ = q;
     q->prev = NULL;
     q->next = pnxt;
     pnxt->prev = q;

     p->next = qnxt;
     p->prev = qprv;
     qprv->next = p;
     qnxt->prev = p;

     return;
  }
  if(q == head_){
    // Node *qprv = q->prev;
     Node *qnxt = q->next;
     Node *pprv = p->prev;
     Node *pnxt = p->next;

     head_ = p;
     p->prev = NULL;
     p->next = qnxt;
     qnxt->prev = p;

     q->next = pnxt;
     q->prev = pprv;
     pprv->next = q;
     pnxt->prev = q;
     return ;
  }
  if(p->next == NULL){
     Node *qprv = q->prev;
     Node *qnxt = q->next;
     Node *pprv = p->prev;
     Node *pnxt = p->next;

     q->next = NULL;
     q->prev = pprv;
     pprv->next = q;

     p->next = qnxt;
     p->prev = qprv;
     qprv->next = p;
     qnxt->prev = p;
     return;
  }
  if(q->next == NULL){
     Node *qprv = q->prev;
     Node *qnxt = q->next;
     Node *pprv = p->prev;
     Node *pnxt = p->next;

     p->next = NULL;
     p->prev = qprv;
     qprv->next = p;

     q->next = pnxt;
     q->prev = pprv;
     pprv->next = q;
     pnxt->prev = q;
     return;
  }

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

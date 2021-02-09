#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  /* your code here */
  clear();
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
  /* your code here */
  Node *newn = new Node(ndata);

  if (this->head_== NULL){
    this->head_ = newn;
    this->head_->prev = NULL;
    this->head_->next = NULL;
    this->length_++;
  }

  else if (p==NULL){
    newn->next = this->head_;
    newn->prev = NULL;
    this->head_->prev = newn;

    this->head_ = newn;
    this->length_++;
  }

  else{
    //tail
    if(p->next == NULL){
      p->next = newn;
      newn->next = NULL;
      newn->prev = p;

      this->length_++;
    }

    //general case
    else{
      Node *pnxt = p->next;
      p->next = newn;
      newn->prev = p;
      newn->next = pnxt;
      pnxt->prev = newn;

      this->length_++;
    }
  }

  return newn;
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

  else if (this->head_ == NULL || this->head_->next == NULL){
    return;
  }

  bool p_flag = false;
  bool q_flag = false;
  Node *temp = this->head_;

  while(temp!=NULL){
      if(temp == p){
          p_flag = true;
      }
      else if(temp == q){
          q_flag = true;
      }
      temp = temp->next;
  }  

  if(!p_flag || !q_flag){
      return;
  }

  if(p->next ==q){
    Node *pprev = p->prev;
    Node *qnext = q->next;
    
    q->prev = pprev;
    
    if(pprev)
        pprev->next = q;
    else 
        this->head_ = q;  

    q->next = p;
    p->prev = q;
    p->next = qnext;
    if(qnext)
        qnext->prev = p;
  }

  else if(q->next ==p){
    Node *qprev = q->prev;
    Node *pnext = p->next;
    
    p->prev = qprev;
    if(qprev)
        qprev->next = p;
    else    
        this->head_ = p;    
    p->next = q;
    q->prev = p;
    q->next = pnext;
    if(pnext)
        pnext->prev = q;
  }

  else {
    Node *pprev = p->prev;
    Node *pnext = p->next;
    Node *qprev = q->prev;
    Node *qnext = q->next;

    if(pprev)
        pprev->next = q;
    else
        this->head_ = q;

    q->prev = pprev;
    q->next = pnext;
    if(pnext)
        pnext->prev = q;

    if(qprev)
        qprev->next = p;
    else 
        this->head_ = p; 

    p->prev = qprev;
    p->next = qnext;
    if(qnext)
        qnext->prev = p;
  }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  /* your code here */
  Node *temp = this->head_;
  while(temp!=NULL){
    Node *temp_next = temp->next;
    delete temp;
    temp = temp_next;
    this->length_--;
  }
  this->head_ = NULL;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other) {
  /* your code here */
  clear();

  if(other.head_== NULL){
    return;
  }

  Node *temp = other.head_;
  Node *curr;
  
  //Copying data so that it is in different memory
  while(temp != NULL){
    if(temp == other.head_){
       this->head_ = insertAfter(this->head_, temp->data);
       curr = this->head_; 
       temp = temp->next;
    }
    else{
      curr = insertAfter(curr, temp->data);
      temp = temp->next;  
    }
  }
  curr->next = NULL; 
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
  std::vector<double> B_values;
  std::vector<double> saved_val;
  B_values = this->getB();

  int max_index = std::max_element(B_values.begin(), B_values.end()) - B_values.begin();
  Node *B = getNodeAt(max_index);
  swap(this->head_, B);
  swapvectorval(0, max_index, B_values);

  for (unsigned int i = 1; i < B_values.size(); i++){ 
    max_index = std::max_element(B_values.begin()+i, B_values.end()) - B_values.begin();
    //swapvectorval(i, max_index, B_values);
    Node *to_swap = getNodeAt(max_index);
    swap(B->next, to_swap);
    to_swap = B;
  }

}

void Chain::swapvectorval(int i, int j, std::vector<double> & A){
  double tmp = A[i];
  A[i] = A[j];
  A[j] = tmp;
}

std::vector<double> Chain::getB(){ /*First part of discription*/
  std::vector<double> B_values;
  /*Find min dist*/
  for (Node *curr = this->head_;  curr != NULL; curr = curr->next){
    std::vector<double> distances_to_p;
    Node *p = curr->prev; /*left of curr*/
    Node *n = curr->next; /*Right of curr*/
    while (p != NULL)
    {
      distances_to_p.push_back(p->data.distanceTo(curr->data));
      p = p->prev;
    }
    while (n != NULL)
    {
      distances_to_p.push_back(n->data.distanceTo(curr->data));
      n = n->next;
    }
    double min_dist = *min_element(distances_to_p.begin(), distances_to_p.end());
    B_values.push_back(min_dist);
  }
  return B_values;
}

Chain::Node * Chain::getNodeAt(int i){
  Node *p = this->head_;
  for( int a=0; a < i; ++a )
    p = p->next;
  return p;    
}

void Chain::setvector(std::vector<double> & B_values, ::vector<int> & indices){
  int max = 0;
  for(unsigned int i=0; i<B_values.size(); i++){
  bool contains = std::count(indices.begin(), indices.end(), max);
    if(!contains && B_values[i] > B_values[max]){
      max = i;
    }
  }
  indices.push_back(max);
}

int Chain::findMax(std::vector<double> & b_vector, int start_index){
  double max_val = b_vector[start_index];
  int max_index = 0;
  for (unsigned int i = start_index; i < b_vector.size(); i++)
  {
    if(b_vector[i] > max_val){
      max_val = b_vector[i];
      max_index = i;
    }
  }
  return max_index;
}
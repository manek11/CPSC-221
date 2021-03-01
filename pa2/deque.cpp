/**
 * @file deque.cpp
 * Implementation of the Deque class.
 *
 */

template <class T>
Deque<T>::Deque(){
/* YOUR CODE HERE! */
    this->n1 = -1; //entry => R 
    this->n2 = 0; //exit => L
    this->data.resize(0); //no elements yet
}

/**
 * Adds the parameter object to the right of the Deque.
 *
 * @param newItem object to be added to the Deque.
 */
template <class T>
void Deque<T>::pushR(T newItem)
{
    /**
     * @todo Your code here!
     */
    this->data.push_back(newItem); //add new item at the R
    this->n1++; //increment entry idx
}

/**
 * Removes the object at the left of the Deque, and returns it to the
 * caller.
 *
 * See .h file for implementation notes. 
 *
 * @return The item that used to be at the left of the Deque.
 */
template <class T>
T Deque<T>::popL()
{
    /**
     * @todo Your code here! 
     */

    /*
        Cases during pop:
        (1) Empty vector: no object to pop just return 
        (2) Resizing case: 
        (3) Regular
    */

    T ret_item = this->data[this->n2];

    if (this->isEmpty()){
        this->n1 = -1;
        this->n2 = 0;
        this->data.resize(0);
        return ret_item;
    }
    //if idx 0....n2 are avaialble
    else if (this->n2 + 1 >= this->n1 - this->n2){   
        vector <T> resizing_arr;
        for (int i=this->n2+1; i< this->n1+1; i++){
            resizing_arr.push_back(this->data[i]);
        }
        this->data.resize(resizing_arr.size());
        this->data.clear();
        this->data = resizing_arr;
        resizing_arr.clear();
        this->n1 = this->data.size()-1;
        this->n2 = 0;
        return ret_item;
    }
    else{
        this->n2++; //increment L idx to make the location before that irrelevant
        return ret_item; 
    }
}
/**
 * Removes the object at the right of the Deque, and returns it to the
 * caller.
 *
 * @return The item that used to be at the right of the Deque.
 */
template <class T>
T Deque<T>::popR()
{
    /**
     * @todo Your code here! You will need to replace the following line.
     */

    T ret_item = this->data[this->n1]; //grab rightmost val
    
    if (this->isEmpty()){
        this->n1 = -1;
        this->n2 = 0;
        this->data.resize(0);
        return ret_item;
    }
    //if 0.....n2-1 >= n2....n1
    else if (this->n2 >= this->n1 - this->n2){            
        vector <T> resizing_arr;
        for (int i=this->n2; i< this->n1; i++){
            resizing_arr.push_back(this->data[i]);
        }
        this->data.resize(resizing_arr.size());
        this->data.clear();
        this->data = resizing_arr;
        resizing_arr.clear();
        this->n1 = this->data.size()-1;
        this->n2 = 0;
        return ret_item;        
    }
    else{
        this->n1--; //decrement R idx 
        this->data.pop_back();
        return ret_item;
    }
}

/**
 * Finds the object at the left of the Deque, and returns it to the
 * caller. Unlike popL(), this operation does not alter the deque.
 *
 * @return The item at the front of the deque.
 */
template <class T>
T Deque<T>::peekL()
{
    /**
     * @todo Your code here! 
     */
    return this->data[this->n2]; //L idx
}

/**
 * Finds the object at the right of the Deque, and returns it to the
 * caller. Unlike popR(), this operation does not alter the deque.
 *
 * @return the value of The item at the right of the deque.
 */
template <class T>
T Deque<T>::peekR()
{
    /**
     * @todo Your code here! 
     */
    return this->data[this->n1]; //R idx
}

/**
 * Determines if the Deque is empty.
 *
 * @return bool which is true if the Deque is empty, false otherwise.
 */
template <class T>
bool Deque<T>::isEmpty() const
{
    /**
     * @todo Your code here! 
     */
    if (this->data.empty()){
        return true;
    }
    return false;
}

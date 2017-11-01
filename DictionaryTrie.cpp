#include "util.hpp"
#include "DictionaryTrie.hpp" 
#include <queue>
#include <stack>

using namespace std;

/* Create a new Dictionary that uses a Trie back end */
DictionaryTrie::DictionaryTrie(){

    root = new MTNode(0);     // create a Trie with one MTNode
    count = 0;                // and count as 0
}

/* Insert a word with its frequency into the dictionary.
 * Return true if the word was inserted, and false if it
 * was not (i.e. it was already in the dictionary or it was
 * invalid (empty string) */
bool DictionaryTrie::insert(std::string word, unsigned int freq)
{
    int wordSize = word.size(); // get the length of the word

    // if the input string is invalid, return false
    if(wordSize == 0) {
        return false;
    }

    MTNode* currNode = root;
    int i = 0;

    // loop from the root to the last internal node
    while(i<wordSize){

        // update all of parent node of current node

        char currChar = word[i]; // get the current char i in the word

        // check if the currChar is a space, then we set the currChar to the
        // ASCII char after 'z'
        if((int)currChar == 32){
            currChar = (char)123;
        }

        // if the target position already have a node, then we follow it
        // else we create a new node and update currNode
        if (currNode->arr[(int)currChar-97] != 0)
        {
            currNode = currNode->arr[(int)currChar-97];
        }
        else {
            currNode->arr[(int)currChar-97] = new MTNode(0);
            MTNode* temp = currNode; 
            currNode = currNode->arr[(int)currChar-97];
            currNode->parent = temp;
        }


        // update the maxCount and numMax while inserting the new node
        if(freq == currNode->parent->maxCount){
            currNode->parent->numMax++;
        }	
        else if (freq > currNode->parent->maxCount){
            currNode->parent->numMax = 1;
            currNode->parent->maxCount = freq;
        }

        // when inserting the last character node, we check if the node already
        // exist, then return false, else update all the fields.
        if(i == wordSize-1){
            if(currNode->isKey == true){  
                return false;
            }
            currNode->freq = freq;
            currNode->isKey = true;
            currNode->str = word;

            // if the maxCount of currNode is equal to freq, then we increment
            // numMax else, we set the maxCount to count
            if(currNode->maxCount == freq)
                currNode->numMax++;
            else if(currNode->maxCount < freq)
                currNode->maxCount = freq;
        }
        i++;
    }	
    count++;  // increment the count 
    return true; 
}

/* Return true if word is in the dictionary, and false otherwise */
bool DictionaryTrie::find(std::string word) const
{
    // initialize the root and get wordSize
    MTNode* currNode = root;
    int wordSize = word.size();

    // if the input string is invalid, return false
    if(wordSize == 0) {
        return false;
    }
    int i = 0;

    // use while loop to iterate from the root to the last word node, if exist
    while(i<wordSize){

        char currChar = word[i]; // get the current char i in the word

        // check if the currChar is a space, then we set the currChar to the
        // ASCII char after 'z'
        if((int)currChar == 32){
            currChar = (char)123;
        }

        // if there is a non-zero pointer to next node, then we go down to the 
        // next node, else return false
        if (currNode->arr[(int)currChar-97] != 0)
        {
            currNode = currNode->arr[(int)currChar-97];
        }
        else {
            return false;
        }
        i++;
    }

    // if the currNode is a key node, then return true, else return false
    if(currNode->isKey){
        return true;
    }
    else{
        return false;
    }
}

/* Return up to num_completions of the most frequent completions
 * of the prefix, such that the completions are words in the dictionary.
 * These completions should be listed from most frequent to least.
 * If there are fewer than num_completions legal completions, this
 * function returns a vector with as many completions as possible.
 * If no completions exist, then the function returns a vector of size 0.
 * The prefix itself might be included in the returned words if the prefix
 * is a word (and is among the num_completions most frequent completions
 * of the prefix)
 */
std::vector<std::string> DictionaryTrie::predictCompletions(std::string prefix, 
        unsigned int num_completions) {

    std::vector<std::string> words; // initialize the string vector
    std::vector<MTNode*> toDelete; // initialize the toDelete vector list

    // check for invalid num_completions input
    if(num_completions <= 0){
        return words;
    }

    int wordSize = prefix.size(); // get the length of the word

    // if the input string is invalid, return false
    if(wordSize == 0) {
        return words;
    }

    // traverse the trie to reach the start node which is the end node of 
    // prefix
    MTNode* start = root;
    int index = 0;

    for(int i = 0; i < wordSize; i++){

        // check for invalid prefix input
        if(!((prefix[i] >= 97 && prefix[i] <= 122)||(prefix[i] == 32))){
            return words;
        } 

        // reset the number of space correctly
        if (prefix[i] == (char)32){
            index = 26;
        }
        else {
            index = prefix[i]-97;
        }

        // update the current Node to iterate to go to the prefix root
        if (start->arr[index] != 0){
            start = start->arr[index];
        } 
        else {
            return words;
        }    
    }

    // create a dummy node for toSort LinkedList 
    LinkedListNode* toSort = new LinkedListNode((MTNode*)0, (LinkedListNode*)0,
            (LinkedListNode*)0);
    auto currLinkedListNode = toSort;

    // use a for loop to create LinkedListNode if numMax is greater than 1
    for(unsigned int i = 0; i < start->numMax; i++) {
        currLinkedListNode = new LinkedListNode(start, currLinkedListNode, 
                (LinkedListNode*)0);
    }

    // initialize the queue and currNode for traversing the trie
    std::queue<MTNode*> toVisit;
    MTNode* currNode = start;
    toVisit.push(currNode);   // push the currNode into the toVisit queue

    // if the current Node is a key, and the node is not hidden, create a new
    // node, update the maxCount and push the node to toVisit.
    if(currNode->isKey && currNode->freq != currNode->maxCount) {
        auto node = new MTNode(currNode);
        toDelete.push_back(node); ////////////////////////////////////////////////
        node->maxCount = currNode->freq;
        toVisit.push(node);
    }

    bool isSame; 

    // while the size of toVisit is greater or equal to 1, we keep looping
    // throught the queue
    while(toVisit.size() >= 1){

        isSame = false;
        currNode = toVisit.front();
        toVisit.pop();

        // if the currNode is the start root, then we add its children into 
        // the toVisit list
        if(currNode == start) {

            for(int i = 0; i < 27; i++){
                if(currNode->arr[i] != 0) {
                    toVisit.push(currNode->arr[i]);
                }
            }
            continue;
        }    

        // if the currNode does not have parent, then we insert the current 
        // Node into the linked list
        if(!(currNode->parent)) {
            topNChange(toSort, currNode, num_completions);
            continue;
        }

        // if the maxCount of currNode equal to that of its parent, 
        // then we set isSame to true.
        if(currNode->maxCount == currNode->parent->maxCount) {
            isSame = true;
        }

        // if the currNode is a key and the currNode is hidden
        if(currNode->isKey && (currNode->freq != currNode->maxCount)) {


            auto node = new MTNode(currNode);
            toDelete.push_back(node); 
            node->maxCount = currNode->freq;
            toVisit.push(node);

            // if the currNode is hidden, then we use a for loop to add all
            // its children into the toVisit
            if(isSame) {
                for(int i = 0; i < 27; i++){
                    if(currNode->arr[i] != 0) {
                        toVisit.push(currNode->arr[i]);
                    }
                }
            }
        }

        // if the currNode node is not hidden inside in its parent
        if(!isSame && topNChange(toSort, currNode, num_completions)){

            // initialze the unfinished bool to false 
            bool unfinished = true;

            // check the numMax of currNode and check if the currNode
            // will change the topN order inside the linked list
            for(unsigned int i = 1; i < currNode->numMax; i++) {
                unfinished = topNChange(toSort, currNode, num_completions);
            }

            // if the node does not change the order, then we skip the rest of 
            // the loop
            if(!unfinished)
                continue;

            // use a for loop to push all children the currNode into toVisit
            for(int i = 0; i < 27; i++){
                if(currNode->arr[i] != 0) {
                    toVisit.push(currNode->arr[i]);
                }
            }
        }

        // if the node is hidden and the key should be added into the topN
        // list
        if(isSame && (!(currNode->isKey) || ((currNode->isKey) 
                        && currNode->maxCount == currNode->freq))) {

            // use a for loop to add all of its children into the toVisit
            for(int i = 0; i < 27; i++){
                if(currNode->arr[i] != 0) {
                    toVisit.push(currNode->arr[i]);
                }
            }
        }    
    }

    std::string currString;
    std::vector<MTNode*> sorted;
    LinkedListNode* curr = toSort->next;  // set curr to the first real node

    // push the pointer to node to vector and handle the case when there is 
    // less actual word than num_completions
    for(unsigned int i = 0; i < num_completions; i++) {
        if(!curr)
            break;
        sorted.push_back(curr->data);
        curr = curr->next;
    }

    words = findString(sorted);             // find the topN words
    
    for(unsigned int i = 0; i < toDelete.size(); i++){
        MTNode* temp = toDelete[i];
        delete temp;
    }

    recursiveDeleteLinkedListNode(toSort);  // recursively delete the linked
    return words;                           // list node
}

/* Destructor to delete the DictionaryTrie */
DictionaryTrie::~DictionaryTrie(){

    deleteAll(this->root);  // call the delete method
}

/** helper method to recursive delete the MTNode */
void DictionaryTrie::deleteAll(MTNode* n) {

    // if the current node is a null pointer, then return
    if(!n){
        return;
    }

    // use a for loop to go throught each array element and call deleteAll
    // and then delete the current node after deleting all its children
    for(int i = 0; i < 27; i++){
        MTNode* curr = n->arr[i];
        deleteAll(curr);
    }
    delete n;
    n = 0;
}

/*
 * constructor to create a new MTNode
 * with arr initialize as all null pointers, isKey to false and freq
 * to zero
 */
MTNode::MTNode(int freq) {                                                         

    this->isKey = false; // by default the node is not a ket                         
    this->freq = freq; // set the freq to freq and initialize the pointer array            
    this->arr = vector<MTNode*>(27, (MTNode*) 0);                                 

    // initialize the fields of MTNode
    this->isVisited = false;  
    this->str = "";   
    this->numMax = 1;
    this->parent = 0;
    this->maxCount=0;
}   

/** 
 * copy constructor to make a copy of the MTNode that is passed into 
 * this method
 */
MTNode::MTNode(MTNode* copy) {

    // initialize the fields of copy of MTNode
    this->isKey = true; // only for orphane node
    this->freq = copy->freq;
    this->arr = copy->arr; // copy its children
    this->isVisited = false;
    this->parent = 0;
    this->str = copy->str;
    this->numMax = 1;
    this->maxCount=0;
}	

/** helper method to check if the order of topN word change while inserting 
 *  the new node into the LinkedList 
 */
bool DictionaryTrie::topNChange(LinkedListNode* head, MTNode* newNode, int n) {

    bool dup = false;
    LinkedListNode* prev = head;  // initilize the prev and curr pointer
    LinkedListNode* curr = prev->next;;

    // use a for loop to loop throught the linked list and create a new 
    // linked list node in the approproate position
    for(int i = 0; i < n; i++){

        // handle the case when there is no node inside the linked list 
        if(curr == 0){
            dup = true;
            new LinkedListNode(newNode, prev, curr);
            break;
        }

        // insert the new node into correct position in the queue
        if(curr->data->maxCount < newNode->maxCount){

            new LinkedListNode(newNode, prev, curr);
            dup = true;
            break;
        }

        // update the prev and curr pointer to nodes 
        prev = curr;
        curr = curr->next;
    }
    return dup;
}

/** helper method to find the keyNode start from the every node in the input
 *  vector and set the stirng stored inside the keyNode into the returned 
 *  string vector
 */
std::vector<std::string> DictionaryTrie::findString(
        const std::vector<MTNode*> & sorted) {

    // initialize the vector for string vector
    std::vector<std::string> result;
    std::stack<MTNode*> * stk = new std::stack<MTNode*>;
    bool same = false;
    MTNode * curr;
    std::vector<MTNode*> reset;

    // loop throught the linked list
    for(auto iter = sorted.begin(); iter != sorted.end(); iter++) {

        // is its not the same, then we delete the original one and create a
        // new stack
        if(!same) {
            delete stk;
            stk = new std::stack<MTNode*>;
            stk->push(*iter);
        }

        // use a while loop to find the keyNode with top N frequency by using
        // a DFS on the current trie
        while(true) {

            curr = stk->top();
            stk->pop();

            // if the current node does not has parent, then push string to
            // result
            if(curr->parent == 0) {
                result.push_back(curr->str);
                break;
            }
            // if the currNode is not hidden and is un visited, then we
            // set the node to be visited and push the word to the result
            if((curr->freq == curr->maxCount) && (curr->isVisited == false)) {
                curr->isVisited = true;
                reset.push_back(curr);
                result.push_back(curr->str);
                break;
            }	

            // then use a for loop to add all its children into stack if its
            // children is not 0
            for(int i = 0; i < 27; i++) {
                if((curr->arr[i] != 0) &&
                        (curr->arr[i]->maxCount >= curr->maxCount))
                    stk->push(curr->arr[i]);
            }
        }	

        // update stack and the boolean value same by checking iter 
        if(*iter == *(++iter)) {
            same = true;
            stk->push(curr);
        } else
            same = false;
        iter--;  // update iter by decrement it by 1
    }	

    // use a for loop to restore the original tree structure
    for(unsigned int i = 0; i < reset.size(); i++){
        reset[i]->isVisited = false;
    }

    delete stk;  // free the memory of the stack
    return result;
}

/** Constructor for a new LinkedList Node with the data as MTNode and 
 * insert the new Node between the prev node and next node.
 */
LinkedListNode::LinkedListNode(MTNode* data, LinkedListNode* prev, 
        LinkedListNode* next) {

    // check if this is the first node, if yes, then return
    if(!prev && !next) { 
        return;
    }

    // else we initialize the fields of LinkedListNode
    this->data = data;
    prev->next = this;
    this->next = next;
}


/** helper method to recursive delete the linkedlist node in the sorted
 *  linkedlist
 */
void DictionaryTrie::recursiveDeleteLinkedListNode(LinkedListNode* n) {

    // check the base case for null pointer 
    if(!n){   
        return;
    }
    recursiveDeleteLinkedListNode(n->next); // call itself 
    delete n;                               // delete the linkedlist node
}




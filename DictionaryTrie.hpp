/**
 *  Given starter code for UCSD CSE 100 programing assignment three.
 *  Authors: Jor-el Briones, Christine Alvarado
 */

#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <vector>
#include <string>

using namespace std;

class MTNode;     // forward declaration
class LinkedListNode;  // forward declaration

/**
 *  The class for a dictionary ADT, implemented as a trie
 *  You may implement this class as either a mulit-way trie
 *  or a ternary search trie, but you must use one or the other.
 *
 */
class DictionaryTrie
{
    public:

        /* Create a new Dictionary that uses a Trie back end */
        DictionaryTrie();

        /* Insert a word with its frequency into the dictionary.
         * Return true if the word was inserted, and false if it
         * was not (i.e. it was already in the dictionary or it was
         * invalid (empty string) */
        bool insert(std::string word, unsigned int freq);

        /* Return true if word is in the dictionary, and false otherwise */
        bool find(std::string word) const;

        /* Return up to num_completions of the most frequent completions
         * of the prefix, such that the completions are words in the dictionary
         * These completions should be listed from most frequent to least.
         * If there are fewer than num_completions legal completions, this
         * function returns a vector with as many completions as possible.
         * If no completions exist, then the function returns a vector of size 
         * 0.
         * The prefix itself might be included in the returned words if the
         * prefix is a word (and is among the num_completions most frequent
         * completions of the prefix)
         */
        std::vector<std::string>
        predictCompletions(std::string prefix, unsigned int num_completions);

        /* Destructor */
        ~DictionaryTrie();

    private:

        int count; // number of key in Mway tree
        MTNode* root; // root of MTNode

        /** helper method to recursive delete the MTNode */
        void deleteAll(MTNode* n);

        /** helper method to check if the order of topN word change while 
         * inserting the new node into the LinkedList 
         */
        bool topNChange(LinkedListNode * head, MTNode * newNode, int n);

        /** helper method to find the keyNode start from the every node in the 
         *  input vector and set the stirng stored inside the keyNode into the 
         *  returned string vector
         */
        std::vector<std::string> findString(
                const std::vector<MTNode*> & sorted); 

        /** helper method to recursive delete the linkedlist node in the sorted
         *  linkedlist
         */
        void recursiveDeleteLinkedListNode(LinkedListNode* n);

};


/** A class, instances of which are nodes in an Multi-way Tree.                 
*/                                                                                
class MTNode {                                                                     

    private:                                                                                   

        unsigned int freq;         // the frequency of the word                        
        std::vector<MTNode*> arr;  // the array of pointers to other MTNodes           
        bool isKey;                // check if the current node is a key    
        MTNode* parent;            // pointer to the parent
        unsigned int maxCount;     // the max freq of subtree
        unsigned int numMax;       // the number of node with max count           
        bool isVisited;            // check if the MTNode is visited                                             
        std::string str;           // to store the string of current node

        /* 
         * constructor to create a new MTNode             
         * with arr initialize as all null pointers and isKey to false                  
         * and freq to 0                                    
         */                                                                
        MTNode(int freq);

        /* copy constructor */
        MTNode(MTNode* copy);

        friend class DictionaryTrie;  // allow DictionaryTrie method access its        
        // private member                                
};  


/** A 'function class' for use as the LinkedList node for sorted LinkedList.
*/
class LinkedListNode {
    public:

        /** Constructor for a new LinkedList Node with the data as MTNode and 
         * insert the new Node between the prev node and next node.
         */
        LinkedListNode(MTNode* data, LinkedListNode* prev,
                LinkedListNode* next);

    private:

        LinkedListNode * next;        // pointer to next linkedlist node
        MTNode* data;                 // the data stored inside linkedlist node
        friend class DictionaryTrie;  // allow DictionaryTrie method access its        
        // private member           
};



#endif // DICTIONARY_TRIE_HPP

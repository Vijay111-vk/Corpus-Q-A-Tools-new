#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;


class info{
    public:
    int book_code;
    int pg;
    int para;
    vector<string> sentence;
    info* next = nullptr;
    info(int bc , int pno , int par , string s);
};

class linkedlist{
    public:
    info* head;
    info* tail;
    int size ;
    void add(int bc , int pno , int par , string s);
    linkedlist();
    ~linkedlist();

};

class node2{
    public:
    Node* node;
    float score;
    node2(Node* node , float score);
};

class maxheap{
    public:
    vector<node2*> heap;
    void insert(node2* node);
    void heapifyup(int it);
    Node* getmax();
    void deletemax();
    void heapifydown(int it);
    void swapnodes(int a , int b);
};


class QNA_tool {

private:

    // You are free to change the implementation of this function
    void query_llm(string filename, Node* root, int k, string API_KEY, string question);
    Dict* dct;
    vector<string> splitwords(string sentence);
    float scorecal(string word ,  int incorp);
    linkedlist* details;
    void parascore(vector<string> words , info* head ,  vector<pair<string , float>> wscore);
    float getscore(string word , vector<pair<string , float>> wscore );
    maxheap* pq;
    Node* head;
    Node* tail;

    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};
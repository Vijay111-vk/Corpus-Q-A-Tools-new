// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class node{

    public:

    string word;
    long long int cnt;
    node* left = NULL;
    node* right = NULL;

    node();

    node(string wd , long long int c);

    ~node();
};

class hashmap{
    private:

    vector<vector<node*>> htable1;
    vector<node*> htable2;

    int hash1(string word);

    int hash2(string word); 

    node* search(node* root,string w);

    int max(int x , int y);

    int height(node* root);

    node* rr(node* a);

    node* lr(node* a);

    node* inavl(node* root , string k);

    void inorder(node* root , vector<string>& vtr);

    public:

    hashmap();

    void insertword(string word);

    node* serachword(string word);

    vector<string> todump();

    ~hashmap();


};

class Dict {
private:
    // You can add attributes/helper functions here
    vector<string> seperatewords(string sentence);

    hashmap* hmap;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    long long int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};
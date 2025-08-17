// Do NOT add any other includes
#include "dict.h"

#define N 100000 //size for each char a-z starting words
#define M 200000 //size for non a-z starting words

    node::node(){
        this->left = nullptr;
        this->right = nullptr;
    }
    node::~node(){

    }

    node::node(string wd ,long long int c){
        this->word = wd;
        this->cnt = c;
        this->left = nullptr;
        this->right = nullptr;
    }

    int hashmap::hash1(string word){
        //for a-z starting words
        //djb2 hash function
        unsigned long hash = 5381;
        for (char c : word) {
            hash = ((hash << 5) + hash) + c; 
        }

        return hash% N;
    }

    int hashmap::hash2(string word){
        //djb2 hash function
        //non a-z starting words
        unsigned long hash = 5381;
        for (char c : word) {
            hash = ((hash << 5) + hash) + c; 
        }

        return hash% M;
    }   
    
    node* hashmap::search(node* root,string w){
        if(root == nullptr){return nullptr;}
        if(root->word < w){ return search(root->right , w); }
        else if(root->word > w){return search(root->left , w);}
        return root;
    }
    int hashmap::max(int x , int y){
        return (x > y)? x : y;
    }
    int hashmap::height(node* root){
        if(root == nullptr){return 0;}
        return max(height(root->right) ,height(root->left))+1;
    }
    node* hashmap::rr(node* a){
        node* b = a->left;
        node* t = b->right;
        b->right = a;
        a->left = t;
        return b;
    }
    node* hashmap::lr(node* a){
        node* b = a->right;
        node* t = b->left;
        b->left = a;
        a->right = t;
        return b;
    }

    node* hashmap::inavl(node* root , string k){
        if(root == nullptr){
            return new node(k , 1);
        }
        if(k < root->word ){ root->left = inavl(root->left , k);}
        else if(k > root->word){root->right = inavl(root->right , k);}
        else{return root;}
        int bf = (root == nullptr) ? 0 : (height(root->left) - height(root->right));
        if(bf > 1 and k < root->left->word){
            return rr(root);
        }
        if(bf < -1 and k > root->right->word){
            return lr(root);
        }
        if(bf > 1 and k > root->left->word){
            root->left = lr(root->left);
            return rr(root);
        }
        if(bf < -1 and k < root->right->word){
            root->right = rr(root->right);
            return lr(root);
        }
        return root;
    }

    hashmap::hashmap(){
        htable1 = vector<vector<node*>>(26, vector<node*>(N,nullptr));//for a-z starting words
        htable2 = vector<node*>(M, nullptr);//non a-z starting words
    }

    void hashmap::insertword(string word){

        //convert to lower case
        for(char &c : word){
            c = tolower(c);
        }

        //first char dependent
        int cs = ((word[0] - 'a') < 26 and (word[0] - 'a') >= 0 ) ? 1 : 2;
        if(cs == 1){
            //first char a-z
            int hshval = hash1(word);
            // cout << hshval << word << endl;
            node* root = htable1[word[0]-'a'][hshval] ;
            node* searched = search(root, word);
            if(searched == nullptr){
                //insert in avl tree
                node* newroot = inavl(root , word);
                htable1[word[0]-'a'][hshval] = newroot;

            }else{
                //inc count
                searched->cnt += 1;
            }
        }
        else if(cs ==2){
            //first char not a-z
            int hshval2 = hash2(word);
            // cout << hshval2 << word << endl;
            node* root2 = htable2[hshval2];
            node* searched2 = search(root2 , word);
            if(searched2 == nullptr){
                //insert in avl tree
                node* newroot2 = inavl(root2 , word);
                htable2[hshval2] = newroot2;

            }else{
                //inc count
                searched2->cnt += 1;
            }
        }

    }

    node* hashmap::serachword(string word){
        //convert to lower case
        for(char &c : word){
            c = tolower(c);
        }

        //first char dependent
        int cs = ((word[0] - 'a') < 26 and (word[0] - 'a') >= 0 ) ? 1 : 2;
        node* root;
        if(cs == 1){
            //first char a-z
            int hshval1 = hash1(word);
            // cout << hshval1 << word << endl;
            root = htable1[word[0]-'a'][hshval1] ;
            
        }
        else if(cs == 2){
            //first char not a-z
            int hshval2 = hash2(word);
            // cout << hshval2 << word << endl;
            root = htable2[hshval2];   
        }
         return search(root, word);
    }
    void hashmap::inorder(node* root , vector<string>& vtr){
        if(root == nullptr){return;}
        inorder(root->left , vtr);
        string curr = root->word + ", " + std::to_string(root->cnt);
        vtr.push_back(curr);
        curr.clear();
        inorder(root->right , vtr);
    }
    vector<string> hashmap::todump(){
        vector<string> ans;
        for(int i = 0 ; i < 26 ; i++){
            for(int j = 0 ; j < N ; j++){
                if(htable1[i][j] != nullptr){
                    inorder(htable1[i][j] , ans);
                }
            }
        }
        for(int i = 0  ; i < M ; i++){
            if(htable2[i] != nullptr){
                inorder(htable2[i] , ans);
            }
        }
        return ans;
    }
    hashmap::~hashmap(){
        for(auto a : htable1){
            for(auto b : a){
                delete b;
            }
        }
        for(auto a : htable2){
            delete a;
        }
    }

Dict::Dict(){
    // Implement your function here   
     hmap = new hashmap();
}

Dict::~Dict(){
    // Implement your function here
    delete hmap;    
}

vector<string> Dict::seperatewords(string sentence){
    vector<string> ans;
    string curr = "";
    for(char c : sentence){
        if(c == '.' or c == ',' or c == '-' or c == ':' or c == '!' or c == '\"' or c == '\'' or c == '(' or c == ')' or c == '?' or c == '[' or c == ']' or c == ';' or c == '@' or c == ' '  ){
            if(!curr.empty()){
                ans.push_back(curr);
                curr.clear();
            }
        }else{
            curr = curr + c;
        }
    }
    if(!curr.empty()){
        ans.push_back(curr);
    }
    return ans;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    vector<string> words = seperatewords(sentence);

    for(string str : words){

        hmap->insertword(str);
    }

    return;
}

long long int Dict::get_word_count(string word){
    // Implement your function here
    node* n = hmap->serachword(word);
    if(n == nullptr){
        return 0;
    }else{
        return n->cnt;
    }

    return -1;
}

void Dict::dump_dictionary(string filename){
    // Implement your function here  
    vector<string> info = hmap->todump();
    std::ofstream outputFile(filename , std::ios::app);
    for(string a : info){
        outputFile << a << endl;
    }
    outputFile.close();
    return;
}
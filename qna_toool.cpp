#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;


QNA_tool::QNA_tool(){
    // Implement your function here  
    dct = new Dict();
    details = new linkedlist();
    pq = new maxheap();
    head = tail = nullptr;
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
}


info::info(int bc , int pno , int par , string s){
    //a node to store all sentences according to paragraph 
    this->book_code = bc;
    this->pg = pno;
    this->para = par;
    this->sentence.push_back(s);
    this->next = nullptr;
}

linkedlist::linkedlist(){
    //used linkedlist to store information bcz easy to iterate
    head = nullptr;
    tail = nullptr;
    size = 0;
}
linkedlist::~linkedlist(){
    info* temp = head;
    info* next;
    while(temp){
        next = temp->next;
        delete temp;
        temp = next;
    }
}

void linkedlist::add(int bc , int pno , int par , string s){
    //if curr paragraph already exists then update its sentence , otherwise adding new node

    if(head == tail and head == nullptr){
        head = tail = new info(bc , pno , par , s);
        size++;
        return;
    }

    if(tail->book_code == bc and tail->pg == pno and tail->para == par){
        tail->sentence.push_back(s);
    }
    else{
        info* newnode = new info(bc , pno , par , s);
        tail->next = newnode;
        tail = tail->next;
        size++;
    }

}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    //inserting sentence in dictonary and details class

    for(char &c : sentence){
            c = tolower(c);
    }

    dct->insert_sentence(book_code , page , paragraph , sentence_no , sentence);
    details->add(book_code , page , paragraph  , sentence);
    return;
}


vector<string> QNA_tool::splitwords(string sentence){
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


float QNA_tool::scorecal(string word , int incorp){
    //to calculate score of each word 
    // cout << "meow";
    std::ifstream file("unigram_freq.csv");
    string line;
    long long int totcount = 0;
    while(getline(file , line)){
        std::stringstream it(line);
        string w , cnt;
        getline(it , w , ',');
        getline(it , cnt);
        if(w == word){
            totcount = std::stoll(cnt);
            // cout << "meow"  << totcount << endl;
            break;
        }
    }
    float score = ((float)incorp + 1)/((float)totcount + 1);
    return score;
}

float QNA_tool::getscore(string word , vector<pair<string , float>> wscore){
    //returns score of the given word

    for(auto it : wscore){
        if(it.first == word){
            return it.second;
        }
    }
}
void QNA_tool::parascore(vector<string> words , info* head ,  vector<pair<string , float>> wscore){
    // iterates over all paragraphs and calculates score of each paragraph and puts it into maxheap
    // O(k^2 * N) k = words in query(almost const) , N = total words in corpus

    info* curr = head;
    string w;
    float score = 0;
    while(curr){
        for(string str : curr->sentence){
            // cout << str<< endl;
            for(string word : words){
                for(size_t i = 0 ; i < str.length(); ++i ){
                    char c = str[i];
                    if(c == '.' or c == ',' or c == '-' or c == ':' or c == '!' or c == '\"' or c == '\'' or c == '(' or c == ')' or c == '?' or c == '[' or c == ']' or c == ';' or c == '@' or c == ' ' ){
                            // cout << w << endl;
                        if(w == word){
                            score += getscore(word , wscore);
                        }
                        w.clear();
                    }else{
                        w += c;
                    }

                }
                if(w == word){
                 score += getscore(word , wscore);
                }
                 w.clear();
            }
        }
        //insert in maxheap
        Node* data = new Node(curr->book_code , curr->pg , curr->para , 0,0);
        pq->insert(new node2(data , score));
        // cout << score << endl;
        score = 0;
        curr = curr->next;
    }

}

node2::node2(Node* node , float score){
    //node of maxheap

    this->node = node;
    this->score = score;
}

void maxheap::insert(node2* nod){
    // cout << " mew ";
    heap.push_back(nod);
    heapifyup(heap.size()-1);
}

void maxheap::swapnodes(int a , int b){
    node2* temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}
void maxheap::heapifyup(int it){
    int par = (it-1)/2;
    if(par >= 0 and heap[par]->score < heap[it]->score){
        swapnodes(par ,it);
        heapifyup(par);
    }
}
Node* maxheap::getmax(){
    if(heap.size() == 0) return nullptr;
    return heap[0]->node;
}
void maxheap::deletemax(){
    heap[0] = heap[heap.size()-1];
    heap.erase(heap.end()-1);
    heapifydown(0);
}

void maxheap::heapifydown( int it){
     int lc = 2* it + 1;
     int rc = 2*it + 2;
    int maxidx = it;
    if(lc >= heap.size()) return;

    if(lc < heap.size() and heap[lc]->score > heap[maxidx]->score){
        maxidx = lc;
    }
    if(rc < heap.size() and heap[rc]->score > heap[maxidx]->score){
        maxidx = rc;
    }

    if(maxidx != it){
        swapnodes(it , maxidx);
        heapifydown(maxidx);
    }

}

Node* QNA_tool::get_top_k_para(string question, int k) {
        for(char &c : question){
            c = tolower(c);
        }

    vector<string> words = splitwords(question);
    vector<pair<string , float>> wscore;
    
    for(string str : words){
        int incorp = dct->get_word_count(str);
        float score = scorecal(str , incorp);
        // cout <<str <<  score << endl;
        wscore.push_back({str , score});
    }

    parascore(words , details->head , wscore);
 
    // cout << details->size << endl; 
    while(k > 0){
        Node* curdata = pq->getmax();
        // cout << curdata->paragraph << endl;
        if(curdata == nullptr) break;

        if(tail == nullptr){
            head = tail = curdata;
        }else{
            tail->right = curdata;
            tail->right->left = tail;
            tail = tail->right;
        }

        pq->deletemax();
        k--;
    }
    //  cout << pq->heap.size() << endl;

    return head;
}






void QNA_tool::query(string question, string filename){
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}

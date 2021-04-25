#include <iostream>
#include <fstream>
#include <cstddef>
#include <list>
#include <vector>
#include <map>
#include <string>
#define NUM_OF_CHARS 256
using namespace std;

//our class that contains the symbol, the frequency of its occurrence, the left and right descendant
class Node
{
public:
    unsigned char token=0;
    int freq=0;
    Node* left=nullptr, * rigth=nullptr;

    Node()
    {

    }

    Node(unsigned char _token, int _freq)
    {
        token = _token;
        freq = _freq;
    }

    Node(Node* _left, Node* _rigth)
    {
        freq = _left->freq + _rigth->freq;
        left = _left;
        rigth = _rigth;
    }

    ~Node()
    {

    }
};

//the left - recursive function of traversing the tree and building to the leaves of their codes
void TreeGo(Node* head, vector<bool>code, map<char, vector<bool>> *list_code)
{
    if (head->left!=nullptr)
    {
        vector <bool> temp = code;
        temp.push_back(0);
        TreeGo(head->left, temp, list_code);
    }

    if (head->rigth != nullptr)
    {
        vector <bool> temp = code;
        temp.push_back(1);
        TreeGo(head->rigth, temp, list_code);
    }

    //we got to the sheet and write down the code
    if (head->left == nullptr && head->rigth == nullptr)
    {
        (*list_code)[head->token] = code;
    }
}

//decompression function of our file
void Decompression()
{
    ifstream fin("output.txt", ios::binary);
    int n, symbol_freq = 0;

    //reading the first number, how many bits are occupied in the last byte
    unsigned char count = fin.get();

    //reading the number how many characters will we have is in the map
    {
        string t;
        unsigned char c = fin.get();
        while (true)
        {
            c = fin.get();
            if (c==' ')
            {
                break;
            }
            t += c;
        }
        n = stoi(t);
    }

    //reading the characters themselves and the frequency of that character and writing them to a list
    unsigned char symbol, trash;
    list<Node*>freq_list;
    for (size_t i = 0; i < n; i++)
    {
        symbol = fin.get();
        trash = fin.get();
        {
            string t;
            unsigned char c;
            while (true)
            {
                c = fin.get();
                if (c == ' ')
                {
                    break;
                }
                t += c;
            }
            symbol_freq = stoi(t);
        }

        Node* temp = new Node(symbol, symbol_freq);
        freq_list.push_back(temp);
    }

    //building a tree from our list
    while (freq_list.size() != 1)
    {
        Node* l = freq_list.front();
        freq_list.pop_front();
        Node* r = freq_list.front();
        freq_list.pop_front();

        Node* new_node = new Node(0, l->freq + r->freq);
        new_node->left = l;
        new_node->rigth = r;

        auto iter = freq_list.begin();

        for (; iter != freq_list.end();)
        {
            if (new_node->freq > (*iter)->freq)
            {
                iter++;
                continue;
            }
            else
            {
                break;
            }
        }
        freq_list.insert(iter, new_node);
    }

    //building codes for leaves and writing them to map
    vector<bool> code;
    map<char, vector<bool>>* list_code = new map<char, vector<bool>>;
    TreeGo(*(freq_list.begin()), code, list_code);    

    //changing the key and value in map
    map<vector<bool>, char>* code_list = new map<vector<bool>, char>;
    for (auto it:*list_code)
    {
        (*code_list)[it.second] = it.first;
    }

    ofstream fout("output_output.txt");
    unsigned char c = 0;
    vector <bool> temp;

    //reading the next character from a compressed file and expanding them into text
    while (true)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;            
        }

        //it is necessary to check the last byte, whether the file has ended
        auto end_of_file = fin.peek();
        for (size_t i = 0; i < 8; i++)
        {
            //adding the next bit of the read character to the temporary vector
            if (bool((1 << (7-i)) & c))
            {
                temp.push_back(1);
            }
            else
            {
                temp.push_back(0);
            }

            //check if this sequence of bits is contained in map then output the text character
            auto iter = code_list->find(temp);
            if (iter==code_list->end())
            {
                continue;
            }
            temp.clear();
            fout << iter->second;

            //for the last byte, check that we have reached the end and written all the significant bits in the last byte
            if (end_of_file==EOF && i+1==count-'0')
            {
                break;
            }
        }
    }
    
    fout.close();
    fin.close();
}

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt", ios::binary);
    int frequency[NUM_OF_CHARS] = { 0 };
    unsigned char c=0;

    //reading a file and counting the character frequencies in an array
    while (1)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;
        }
        frequency[c]++;
    }

    //translating the pointer to the beginning of the file
    fin.clear();
    fin.seekg(0);

    //adding a symbol and its frequency to our list
    list<Node*>freq_list;
    for (size_t i = 0; i < NUM_OF_CHARS; i++)
    {
        if (frequency[i]==0)
        {
            continue;
        }

        Node* temp = new Node(i, frequency[i]);

        auto iter = freq_list.begin();

        for (; iter!=freq_list.end();)
        {
            if (temp->freq>(*iter)->freq)
            {
                iter++;
                continue;
            }
            else
            {
                break;
            }
        }
        freq_list.insert(iter, temp);
    }

    //pre-write two spaces to the beginning, one of which will be overwritten by the number of significant bits in the last byte
    fout << "  ";

    //we write down how many elements are stored in the map, and then the elements themselves and their frequencies are separated by spaces
    fout << freq_list.size() << " ";
    for (auto it : freq_list)
    {
        fout << it->token << " " << it->freq << " ";
    }
    
    //building a tree from our list
    while (freq_list.size()!=1)
    {
        Node *l = freq_list.front();
        freq_list.pop_front();
        Node *r = freq_list.front();
        freq_list.pop_front();

        Node *new_node = new Node(0, l->freq + r->freq);
        new_node->left = l;
        new_node->rigth = r;

        auto iter = freq_list.begin();

        for (; iter != freq_list.end();)
        {
            if (new_node->freq > (*iter)->freq)
            {
                iter++;
                continue;
            }
            else
            {
                break;
            }
        }
        freq_list.insert(iter, new_node);
    }

    //building codes for leaves and writing them to map
    vector<bool> code;
    map<char, vector<bool>>* list_code = new map<char, vector<bool>>;
    TreeGo(*(freq_list.begin()), code, list_code);

    //we read the next character and compress it
    unsigned char tx = 0;
    int count = 0;
    while (1)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;
        }

        vector <bool> x = list_code->at(c);
        for (size_t i = 0; i < x.size(); i++)
        {
            tx = tx | x[i] << (7 - count);
            count++;
            if (count==8)
            {
                count = 0;
                fout.write((char*)&tx, 1);
                tx = 0;
            }
        }
    }

    //write the last byte
    fout.write((char*)&tx, 1);

    //we translate the pointer to the beginning of the file and write the number of significant bits in the last byte
    fout.clear();
    fout.seekp(0);
    fout << count;
    fout.close();
    fin.close();    
    Decompression();

    return 0;
}

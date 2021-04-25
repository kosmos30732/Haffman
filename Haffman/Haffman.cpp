#include <iostream>
#include <fstream>
#include <cstddef>
#include <list>
#include <vector>
#include <map>
#include <string>
#define NUM_OF_CHARS 256

using namespace std;

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

    if (head->left == nullptr && head->rigth == nullptr)
    {
        (*list_code)[head->token] = code;
    }
}

void Decompression()
{
    ifstream fin("output.txt", ios::binary);
    int n, symbol_freq = 0;
    unsigned char count = fin.get();
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

    vector<bool> code;
    map<char, vector<bool>>* list_code = new map<char, vector<bool>>;
    TreeGo(*(freq_list.begin()), code, list_code);    

    map<vector<bool>, char>* code_list = new map<vector<bool>, char>;
    for (auto it:*list_code)
    {
        (*code_list)[it.second] = it.first;
    }

    ofstream fout("output_output.txt");
    unsigned char c = 0;
    vector <bool> temp;

    while (true)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;            
        }
        auto end_of_file = fin.peek();

        for (size_t i = 0; i < 8; i++)
        {
            if (bool((1 << (7-i)) & c))
            {
                temp.push_back(1);
            }
            else
            {
                temp.push_back(0);
            }

            auto iter = code_list->find(temp);
            if (iter==code_list->end())
            {
                continue;
            }
            temp.clear();
            fout << iter->second;
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
    while (1)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;
        }
        frequency[c]++;
    }
    fin.close();

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
    fout << "  ";
    fout << freq_list.size() << " ";

    for (auto it : freq_list)
    {
        fout << it->token << " " << it->freq << " ";
    }
    
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
    
    vector<bool> code;
    map<char, vector<bool>>* list_code = new map<char, vector<bool>>;
    TreeGo(*(freq_list.begin()), code, list_code);

    ifstream file_in("input.txt");
    unsigned char tx = 0;
    int count = 0;
    while (1)
    {
        c = file_in.get();
        if (file_in.eof())
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
    fout.write((char*)&tx, 1);
    fout.clear();
    fout.seekp(0);
    fout << count;
    fout.close();
    file_in.close();    
    Decompression();

    return 0;
}

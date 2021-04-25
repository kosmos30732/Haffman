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
    ifstream fin("output.txt");
    int n, symbol_freq = 0;
    fin >> n;
    unsigned char symbol;
    list<Node*>freq_list;

    for (int i = 0; i < n; i++)
    {
        fin >> symbol >> symbol_freq;
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
    
    ofstream fout("output_output.txt");
    unsigned char tx = 0, c = 0;
    int count = 0;

    while (1)
    {
        c = fin.get();
        if (fin.eof())
        {
            break;
        }
        vector <bool> x = list_code->at(c);
        for (int i = 0; i < x.size(); i++)
        {
            tx = tx | x[i] << (7 - count);
            count++;
            if (count == 8)
            {
                count = 0;
                fout << tx;
                tx = 0;
            }
        }
    }
    fout << tx;
    fout.close();
    
}

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");
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

    for (int i = 0; i < NUM_OF_CHARS; i++)
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
    
    fout << freq_list.size() << " ";

    for (auto it : freq_list)
    {
        fout << it->token << " " << it->freq << " ";
    }
    fout << endl;
    
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

    ifstream cin("input.txt");
    unsigned char tx = 0;
    int count = 0;
    while (1)
    {
        c = cin.get();
        if (cin.eof())
        {
            break;
        }
        vector <bool> x = list_code->at(c);
        for (int i = 0; i < x.size(); i++)
        {
            tx = tx | x[i] << (7 - count);
            count++;
            if (count==8)
            {
                count = 0;
                fout << tx;
                tx = 0;
            }
        }
    }
    fout << tx;
    fout.close();
    cin.close();
    
    return 0;
}

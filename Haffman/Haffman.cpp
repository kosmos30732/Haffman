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
    char token=0;
    int freq=0;
    Node* left=nullptr, * rigth=nullptr;

    Node()
    {

    }

    Node(char _token, int _freq)
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
        code.push_back(0);
        TreeGo(head->left, code, list_code);
    }

    if (head->rigth != nullptr)
    {
        code.push_back(1);
        TreeGo(head->rigth, code, list_code);
    }

    if (head->left == nullptr && head->rigth == nullptr)
    {
        (*list_code)[head->token] = code;
    }
}

string GetString(vector<bool>code)
{
    string s;
    for (auto c: code)
    {
        s += c;
    }

    return s;
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
    string res;

    while (1)
    {
        c = cin.get();
        if (cin.eof())
        {
            break;
        }
        res += GetString(list_code->at(c));
        
        if (res.length()>7)
        {
            while (res.length() > 7)
            {
                for (int i = 0; i < 7; i++)
                {
                    if (res[i]==0)
                    {
                        c &= ~(1 << (7 - i));
                    }
                    if (res[i] == 1)
                    {
                        c |= (1 << (7 - i));
                    }
                }

                fout << c;
                res.erase(0, 8);
            }
        }
    }


    return 0;
}

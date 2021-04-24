#include <iostream>
#include <fstream>
#include <cstddef>
#include <list>
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

int main()
{
    ifstream cin("input.txt");
    ofstream cout("output.txt");
    int frequency[NUM_OF_CHARS] = { 0 };
    unsigned char c=0;
    while (1)
    {
        c = cin.get();
        if (cin.eof())
        {
            break;
        }
        frequency[c]++;
    }

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
        cout << "New iteration" << endl;
        for (auto it : freq_list)
        {
            cout << it->token << " " << it->freq << endl;
        }
        cout << endl;
    }
    
    
    return 0;
}

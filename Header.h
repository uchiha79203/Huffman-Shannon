#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <msclr\marshal_cppstd.h>
#include <map>
#include <queue>
#include <bitset>
#include <chrono>
using namespace System;

struct Character {
    char c;
    unsigned frequency;
    Character* left, * right;
    Character(char a, unsigned f, Character* l, Character* r) {
        c = a;
        frequency = f;
        left = l;
        right = r;
    }
};

struct mycomp {
public:
    bool operator()(Character* a, Character* b) {
        return (a->frequency > b->frequency);
    }
};
class Huffman {
private:
    Character* root;

    void deleteTree(Character* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
public:
    void Huffman_encode(Character* head, std::string s, std::map<char, std::string>& huff) {
        if (head->c != '$') {
            huff[head->c] = s;
            return;
        }
        Huffman_encode(head->left, s + '0', huff);
        Huffman_encode(head->right, s + '1', huff);
    }

public:
    Huffman() : root(nullptr) {}

    ~Huffman() {
        deleteTree(root);
    }

    std::string Huffman_decode(Character* head, std::string en) {
        Character* curr = head;
        std::string ans;
        for (auto x : en) {
            if (x == '0') {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
            if (curr->left == NULL && curr->right == NULL) {
                if (curr->c == '_') {
                    ans += '\n';
                    curr = head;
                    continue;
                }
                else if (curr->c == '-') {
                    ans += ' ';
                    curr = head;
                    continue;
                }
                ans = ans + curr->c;
                curr = head;
            }
        }
        return ans;
    }

    Character* buildtree(std::map<char, unsigned>freq) {
        std::priority_queue<Character*, std::vector<Character*>, mycomp>pq;
        for (auto x : freq) {
            Character* temp = new Character(x.first, x.second, NULL, NULL);
            pq.push(temp);
        }
        Character* head = NULL;
        while (pq.size() > 1) {
            Character* l = pq.top();
            pq.pop();
            Character* r = pq.top();
            pq.pop();
            head = new Character('$', (l->frequency + r->frequency), l, r);
            pq.push(head);
        }
        root = pq.top();

        return pq.top();
    }
    void encode(std::string inputfile) {
        std::ifstream fin(inputfile);
        std::string inputs;
        std::map<char, unsigned>freq;
        char ch;
        while (fin.get(ch)) {
            if (ch == ' ') {
                ch = '-';
                inputs += ch;
                freq['-']++;
                continue;
            }
            else if (ch == '\n') {
                ch = '_';
                inputs += ch;
                freq['_']++;
                continue;
            }
            inputs += ch;
            freq[ch]++;
        }
        std::map<char, std::string>huff; // to map the character with its huffman code

        Character* head = buildtree(freq);
        std::string s = "";
        Huffman_encode(head, s, huff);
        fin.clear();
        fin.seekg(0);
        std::string en;

        for (auto c : inputs) {
            en += huff[c];
        }
        fin.close();
        unsigned char padding = 0;
        if (en.length() % 8 != 0) {
            padding = 8 - en.length() % 8;
            en.append(padding, '0');
        }

        std::ofstream output("encoded.xip");

        output << freq.size() << std::endl;
        for (auto x : freq) {
            output << x.first << " " << x.second << std::endl;
        }
        if (!output.is_open()) {
            return;
        }
        output.put(padding);
        for (size_t i = 0; i < en.length(); i += 8) {
            std::string byte = en.substr(i, 8);
            char byteValue = static_cast<char>(std::stoi(byte, nullptr, 2));
            output.put(byteValue);
        }

        output.close();
    }
    void decode(std::string outputfile) {
        std::ifstream input(outputfile, std::ios::binary);
        std::ofstream output("decoded.txt");

        if (!input.is_open()) {
            //cout << "Error opening input file" << endl;
            return;
        }
        int num;
        input >> num;
        char chi;
        unsigned f;
        std::map<char, unsigned>frequency;
        for (int i = 0; i < num; i++) {
            input >> chi >> f;
            frequency[chi] = f;
        }

        unsigned char padding;
        input >> reinterpret_cast<char&>(padding);


        std::string encodedData;
        char ch;
        while (input.get(ch)) {
            encodedData += std::bitset<8>(ch).to_string();
        }
        input.close();

        if (padding > 0) {
            encodedData = encodedData.substr(0, encodedData.length() - padding);
        }
        Character* head = buildtree(frequency);
        Character* currentNode = head;
        std::string ans = Huffman_decode(head, encodedData);
        output << ans;


        output.close();
    }
};

void Compress(System::String^ managedString,int choice)
{
    msclr::interop::marshal_context context;
    std::string standardString = context.marshal_as<std::string>(managedString);
    Huffman huffman;
    if (choice == 1) {
        huffman.encode(standardString);
        return;
    }
    else if (choice == 2) {
        huffman.decode(standardString);
        return;
    }
    return;
}
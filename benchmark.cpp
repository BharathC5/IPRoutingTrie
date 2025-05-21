#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
using namespace std;
using namespace chrono;

struct TrieNode {
	bool isEnd;
	TrieNode* child[2];
	TrieNode() {
		isEnd = false;
		child[0] = child[1] = NULL;
	}
};

class Trie {
	TrieNode* root;
public:
	Trie() {
		root = new TrieNode();
	}
	void insert(string ip_bin) {
		TrieNode* curr = root;
		for(char bit : ip_bin) {
			int idx = bit - '0';
			if(!curr->child[idx]) curr->child[idx] = new TrieNode();
			curr = curr->child[idx];
		}
		curr->isEnd = true;
	}

	bool search(string ip_bin) {
		TrieNode* curr = root;
		for(char bit : ip_bin) {
			int idx = bit - '0';
			if(!curr->child[idx]) return false;
			curr = curr->child[idx];
		}
		return curr->isEnd;
	}
};

// Convert IP string to binary
string ip_to_bin(string ip) {
	string bin;
	int num = 0;
	for(char ch : ip) {
		if(ch == '.') {
			for(int i = 7; i >= 0; i--) bin += ((num >> i) & 1) + '0';
			num = 0;
		} else {
			num = num * 10 + (ch - '0');
		}
	}
	for(int i = 7; i >= 0; i--) bin += ((num >> i) & 1) + '0';
	return bin;
}

int main() {
	ifstream fin("ips.txt");
	vector<string> ips;
	string ip;

	while(getline(fin, ip)) ips.push_back(ip);
	fin.close();

	cout << "Loaded " << ips.size() << " IPs\n";

	// ----------- Trie Insertion & Search -----------
	Trie trie;
	auto start = high_resolution_clock::now();
	for(string& ip : ips) {
		trie.insert(ip_to_bin(ip));
	}
	auto end = high_resolution_clock::now();
	cout << "[Trie] Insertion Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	start = high_resolution_clock::now();
	int found = 0;
	for(string& ip : ips) {
		found += trie.search(ip_to_bin(ip));
	}
	end = high_resolution_clock::now();
	cout << "[Trie] Search Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	// ----------- HashMap Insertion & Search -----------
	unordered_map<string, bool> hashmap;

	start = high_resolution_clock::now();
	for(string& ip : ips) {
		hashmap[ip] = true;
	}
	end = high_resolution_clock::now();
	cout << "[HashMap] Insertion Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	start = high_resolution_clock::now();
	found = 0;
	for(string& ip : ips) {
		if(hashmap[ip]) found++;
	}
	end = high_resolution_clock::now();
	cout << "[HashMap] Search Time: " << duration_cast<milliseconds>(end - start).count() << " ms\n";

	return 0;
}

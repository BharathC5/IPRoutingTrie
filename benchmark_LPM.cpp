#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
using namespace std;
using namespace chrono;

// --- Convert IP to 32-bit unsigned integer
uint32_t ip_to_int(const string& ip) {
    uint32_t res = 0;
    int octet = 0;
    istringstream ss(ip);
    string token;
    for (int i=0; i<4; i++) {
        getline(ss, token, '.');
        octet = stoi(token);
        res = (res << 8) + octet;
    }
    return res;
}

// --- Convert uint32_t IP to string (for printing)
string int_to_ip(uint32_t ip) {
    return to_string((ip >> 24) & 0xFF) + "." +
           to_string((ip >> 16) & 0xFF) + "." +
           to_string((ip >> 8) & 0xFF) + "." +
           to_string(ip & 0xFF);
}

// --- Trie Node for prefix matching
struct TrieNode {
    TrieNode* child[2] = {nullptr, nullptr};
    bool isEnd = false;
    string prefix = "";   // Store matched prefix for demonstration
};

class Trie {
    TrieNode* root;
public:
    Trie() { root = new TrieNode(); }

    void insert(uint32_t ip, int prefix_len, const string& prefix_str) {
        TrieNode* curr = root;
        for (int i=31; i >= 32 - prefix_len; i--) {
            int bit = (ip >> i) & 1;
            if (!curr->child[bit]) curr->child[bit] = new TrieNode();
            curr = curr->child[bit];
        }
        curr->isEnd = true;
        curr->prefix = prefix_str;
    }

    // Longest Prefix Match search
    string search(uint32_t ip) {
        TrieNode* curr = root;
        string last_match = "";
        for (int i=31; i >= 0; i--) {
            int bit = (ip >> i) & 1;
            if (!curr->child[bit]) break;
            curr = curr->child[bit];
            if (curr->isEnd) last_match = curr->prefix;
        }
        return last_match;
    }
};

// --- HashMap LPM simulation by checking all prefixes
string hashmap_lpm_search(const unordered_map<string, bool>& map, uint32_t ip) {
    string best_match = "";
    // Check prefixes from longest (32) to shortest (0)
    for (int prefix_len = 32; prefix_len >= 0; prefix_len--) {
        uint32_t mask = prefix_len == 0 ? 0 : (~0u << (32 - prefix_len));
        uint32_t masked_ip = ip & mask;
        string key = int_to_ip(masked_ip) + "/" + to_string(prefix_len);
        if (map.find(key) != map.end()) {
            return key;  // Found the longest prefix match
        }
    }
    return ""; // no match found
}

int main() {
    // Sample subnets to insert (prefix string, prefix length)
    vector<pair<string,int>> prefixes = {
        {"192.168.0.0", 16},
        {"192.168.1.0", 24},
        {"10.0.0.0", 8},
        {"10.0.0.0", 16},
        {"172.16.0.0", 12},
        {"0.0.0.0", 0} // default route
    };

    Trie trie;
    unordered_map<string,bool> hashmap;

    // Insert prefixes into Trie and HashMap
    for (auto& p : prefixes) {
        uint32_t ip_num = ip_to_int(p.first);
        string prefix_str = p.first + "/" + to_string(p.second);
        trie.insert(ip_num, p.second, prefix_str);
        hashmap[prefix_str] = true;
    }

    // Queries for LPM
    vector<string> queries = {
        "192.168.1.123",
        "192.168.2.5",
        "10.0.5.6",
        "172.16.5.4",
        "8.8.8.8"
    };

    // --- Trie search timing ---
    auto start = high_resolution_clock::now();
    for (auto& q : queries) {
        uint32_t ip_num = ip_to_int(q);
        string res = trie.search(ip_num);
        cout << "[Trie] IP: " << q << " -> Matched prefix: " << (res.empty() ? "NONE" : res) << "\n";
    }
    auto end = high_resolution_clock::now();
    cout << "Trie LPM search took: " << duration_cast<microseconds>(end - start).count() << " us\n\n";

    // --- HashMap search timing ---
    start = high_resolution_clock::now();
    for (auto& q : queries) {
        uint32_t ip_num = ip_to_int(q);
        string res = hashmap_lpm_search(hashmap, ip_num);
        cout << "[HashMap] IP: " << q << " -> Matched prefix: " << (res.empty() ? "NONE" : res) << "\n";
    }
    end = high_resolution_clock::now();
    cout << "HashMap LPM search took: " << duration_cast<microseconds>(end - start).count() << " us\n";

    return 0;
}

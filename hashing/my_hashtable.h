#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <set>
#include <unistd.h>

#include "linked_list.h"

#define ll long long
#define WORD_SIZE 7

const ll c1 = 41;
const ll c2 = 39;

enum COLLISION_HANDLING
{
    SEPARATE_CHAIN,
    LINEAR,
    QUAD,
    DOUBLE_HASH
};

struct _entry
{
    std::string key;
    ll val;

    _entry() : key(""), val(-1) {}

    _entry(std::string k, ll v) : key(k), val(v) {}

    _entry(_entry *e) : key(e->key), val(e->val) {}

    void print()
    {
        std::cout << "Entry key: " << key << ", value: " << val << "\n";
    }
};

std::vector<std::string> word_gen(ll n);

unsigned ll hashcode(std::string k);

unsigned ll jenkins_hashcode(std::string k);

unsigned ll hash_fun1(ll k, ll mod);

unsigned ll hash_fun2(ll k, ll mod);

class my_hashtable
{
private:
    ll n;
    _entry *table;
    COLLISION_HANDLING mt;
    ll probes;
    linked_list **llTable;

    my_hashtable(const my_hashtable &t) {}

public:
    my_hashtable() : n(2), table(NULL), mt(DOUBLE_HASH), probes(0), llTable(nullptr) {}

    my_hashtable(ll n, COLLISION_HANDLING mt) : n(n), table(new _entry[n]), mt(mt), probes(0), llTable(nullptr)
    {
        for (int i = 0; i < n; i++)
            table[i] = new _entry("", -1); // empty slot
        if (mt == SEPARATE_CHAIN)
        {
            llTable = new linked_list*[n];
            for (int i = 0; i < n; i++)
                llTable[i] = new linked_list();
        }
    }

    ~my_hashtable() {}

    unsigned ll insert(std::string key, ll val)
    {
        unsigned ll hash = jenkins_hashcode(key);
        unsigned ll i = 0, j = 0;
        probes = 0;

        if (mt == LINEAR)
        {
            for (; i < n; i++, probes++)
            {
                j = (hash_fun1(hash, n) + i) % n;
                if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                    break;
            }
            if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                hash = j;
            else
                std::cout << "Overflow in linear probe\n";
        }
        else if (mt == QUAD)
        {
            // unsigned ll c1 = n / 2 - 1, c2 = n / 2 + 1;
            for (; i < n; i++, probes++)
            {
                // j = (hash_fun1(hash, n) + c1 * i + c2 * i * i) % n;
                j = (hash_fun1(hash, n) + i * i) % n;
                if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                    break;
            }
            if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                hash = j;
            else
                std::cout << "Overflow in Quad probe\n";
        }
        else if (mt == DOUBLE_HASH)
        {
            for (; i < n; i++, probes++)
            {
                j = (hash_fun1(hash, n) + i * hash_fun2(hash, n - 1)) % n;
                if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                    break;
            }
            if (table[j].key == "" && (table[j].val == -1 || table[j].val == -2))
                hash = j;
            else
                std::cout << "Overflow in double hashing\n";
        }
        else
        {
            hash = hash_fun1(hash, n);

            if (llTable[hash]->get_curr() == "")
            {
                llTable[hash] = new linked_list();
            }
            llTable[hash]->insert(key, val);
        }

        if (mt != SEPARATE_CHAIN)
            table[hash] = new _entry(key, val);

        return hash;
    }

    void remove(std::string key)
    {
        if (mt == SEPARATE_CHAIN)
        {
            unsigned ll hash = hash_fun1(jenkins_hashcode(key), n);

            if (llTable[hash]->get_curr() == "")
                return;
            
            llTable[hash]->remove(key);
        }
        else
        {
            // is it in table
            ll idx = search(key);
            if (idx == -1 || idx >= n)
                return;
            table[idx].key = "";
            table[idx].val = -2;
        }
    }

    ll search(std::string key)
    {
        unsigned ll hash = (jenkins_hashcode(key));
        ll res = -1;
        unsigned ll i = 0, j = 0;
        probes = 0;

        if (mt == LINEAR)
        {
            for (; i < n; i++, probes++)
            {
                j = (hash_fun1(hash, n) + i) % n;
                if (table[j].key == "" && table[j].val == -1)
                    break;
                if (table[j].key == key)
                    break;
            }
        }
        else if (mt == QUAD)
        {
            // unsigned ll c1 = n / 2 - 1, c2 = n / 2 + 1;

            for (; i < n; i++, probes++)
            {
                // j = (hash_fun1(hash, n) + c1 * i + c2 * i * i) % n;
                j = (hash_fun1(hash, n) + i * i) % n;
                if (table[j].key == "" && table[j].val == -1)
                    break;
                if (table[j].key == key)
                    break;
            }
        }
        else if (mt == DOUBLE_HASH)
        {
            for (; i < n; i++, probes++)
            {
                j = (hash_fun1(hash, n) + i * hash_fun2(hash, n - 1)) % n;
                if (table[j].key == "" && table[j].val == -1)
                    break;
                if (table[j].key == key)
                    break;
            }
        }
        else
        {
            j = hash_fun1(hash, n);
            if (llTable[j]->search(key))
                res = j;
        }
        if (mt != SEPARATE_CHAIN && table[j].key == key)
        {
            res = j;
        }

        return res;
    }

    ll get_probes() { return probes; }

    ll get_capacity() { return n; }

    void print()
    {
        std::cout << "Capacity: " << n << "\n";
        for (int i = 0; i < n; i++)
        {
            if (table[i].key != "" && table[i].val != -1)
            {
                std::cout << "bucket " << i << ": ";
                table[i].print();
            }
        }
    }
};

std::string gen_random(const int len)
{
    static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
    {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

std::vector<std::string> word_gen(ll n)
{
    std::vector<std::string> res;
    std::set<std::string> temp;
    for (int i = 0; i < n; i++)
    {
        std::string t;
        int c = 0;

        while (true)
        {
            t = gen_random(WORD_SIZE);

            if (temp.find(t) == temp.end())
                break;
        }

        temp.insert(t);
        res.push_back(t);
    }
    return res;
}

unsigned ll hashcode(std::string k)
{
    ll l = k.size();
    ll h = 0;
    for (int i = 1; i <= l; i++)
    {
        h += ((int)k[i - 1]) * (41 ^ (l - i));
    }

    return h;
}

unsigned ll jenkins_hashcode(std::string k)
{
    int l = k.length();
    ll hash = 0;

    for (int i = 0; i < l; i++)
    {
        hash += ((int)k[i]);
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash >= 0 ? hash : -hash ;
}

unsigned ll hash_fun1(ll k, ll mod)
{
    unsigned long long a = std::floor(mod / 2 + 1), b = std::floor(mod / 2 - 1);
    return (a * k + b) % mod;
}

unsigned ll hash_fun2(ll k, ll mod)
{
    return 1 + (k % mod);
}
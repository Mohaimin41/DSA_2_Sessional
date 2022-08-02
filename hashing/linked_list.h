#include <iostream>
#include <algorithm>
#include <string>

#define ll long long

class linked_list
{
public:
    struct _node
    {
        std::string key;
        ll val;
        _node *next;

        _node() : key(""), val(-1), next(NULL) {}
        _node(std::string key, ll val, _node *n = NULL) : key(key), val(val), next(n) {}
    };

private:
    _node *_head;
    _node *_curr;
    _node *_tail;
    ll _items;

public:
    linked_list() : _head(new _node()), _curr(_head), _tail(_head), _items(0)
    {
        // _head = new _node();
        // _tail = _curr = _head;
    }
    _node *insert(std::string k, ll v)
    {
        _curr->next = new _node(k, v, _curr->next);
        if (_tail == _curr)
            _tail = _curr->next;
        _items++;
        return _curr->next;
    }
    std::string remove(std::string k)
    {
        _node *tmp = _curr->next;
        if (_tail == _curr->next)
            _tail = _curr;
        _curr->next = _curr->next->next;
        std::string t = tmp->key;
        delete tmp;
        _items--;
        return t;
    }
    bool search(std::string k)
    {
        bool res = false;
        _node *tmp = _head;
        while (tmp != NULL)
        {
            tmp = tmp->next;
            if (tmp->key == k)
            {
                res = true;
                break;
            }
        }
        return res;
    }
    std::string get_curr() 
    {
        return _curr->next->key;
    }
    void print()
    {
        _node *tmp = _head;
        while (tmp != NULL)
        {
            tmp = tmp->next;
            std::cout << "Key: " << tmp->key << ", value: " << tmp->val << "\n";
        }
    }
};
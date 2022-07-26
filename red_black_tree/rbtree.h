#include <iostream>

#define left 0
#define right 1
#define red 0
#define black 1
#define noKey -1

class rbtree
{
public:
    struct Node;

private:
    Node *rt;
    Node *sentinel;

public:
    struct Node
    {
        int key;
        int color;
        int subTreeSize;
        Node *p;
        Node *child[2];

        Node(int k, int col = red, int sub = 1, Node *p = NULL, Node *l = NULL, Node *r = NULL) : key(k), color(col), subTreeSize(sub), p(p)
        {
            child[left] = l;
            child[right] = r;
        }
    };

private:
    /**
     * @brief           finds node with minimum key in subtree rooted with x
     *
     * @param   x       subtree root
     * @return Node*    minimum key node
     */
    Node *tree_minimum(Node *x)
    {
        while (x->child[left] != sentinel)
        {
            x = x->child[left];
        }
        return x;
    }

    /**
     * @brief   rotates node by direction from dir
     *
     * @param   node    pointer to node to be rotated
     * @param   dir     true/1 for right, false/0 for left rotation
     * @return  Node*   new root node of the subtree after rotation
     */
    Node *rotate(Node *node, bool dir)
    {
        Node *y = node->child[!dir];
        node->child[!dir] = y->child[dir];
        //
        if (y->child[dir] != sentinel)
            y->child[dir]->p = node;

        y->p = node->p;

        if (node->p == sentinel)
            rt = y;
        else if (node == node->p->child[left])
            node->p->child[left] = y;
        else
            node->p->child[right] = y;

        y->child[dir] = node;
        node->p = y;

        y->subTreeSize = ((y->child[left] == sentinel) ? 0
                                                       : y->child[left]->subTreeSize) +
                         ((y->child[right] == sentinel) ? 0
                                                        : y->child[right]->subTreeSize) +
                         1;
        node->subTreeSize = ((node->child[left] == sentinel) ? 0
                                                             : node->child[left]->subTreeSize) +
                            ((node->child[right] == sentinel) ? 0
                                                              : node->child[right]->subTreeSize) +
                            1;

        return y;
    }

    /**
     * @brief   restores balance after insertion as
     *          insertion may break properties 2, 4
     *
     * @param   z   Node just inserted
     */
    void p_insertFixUp(Node *z)
    {
        while (z->p->color == red)
        {
            int dir = (z->p == z->p->p->child[right]); // 1 means z.parent was right child of z.grandparnet, 0 is for left
            // uncle of z is in opposite direction
            Node *y = z->p->p->child[!dir];

            if (y->color == red)
            {
                // z parent and uncle black
                z->p->color = y->color = black;
                // z grandparent red, has two black children
                z->p->p->color = red;
                // z is now grandparent
                z = z->p->p;
            }
            else
            {
                if (z == z->p->child[!dir])
                {
                    z = z->p;
                    rotate(z, dir);
                }
                z->p->color = black;
                z->p->p->color = red;
                rotate(z->p->p, !dir);
            }
        }
        rt->color = black;
    }

    /**
     * @brief   handles all the three rule violations by deletion (2,4,5),
     *          four possible cases are handled
     *
     * @param   x   the node that moves into y's i.e. the removed node's OR the successor node's place
     */
    void p_deleteFixUp(Node *x)
    {
        while (x != sentinel && x->color == black)
        {
            int dir = (x == x->p->child[right]); // 1 to right, 0 to left
            Node *w = x->p->child[!dir];

            if (w->color == red)
            {
                w->color = black;
                x->p->color = red;
                rotate(x, dir);
                w = x->p->child[!dir];
            }
            if (w->child[left]->color == black && w->child[right]->color == black)
            {
                w->color = red;
                x = x->p;
            }
            else
            {
                if (w->child[!dir]->color == black)
                {
                    w->child[dir]->color = black;
                    w->color = red;
                    rotate(w, !dir);
                    w = x->p->child[!dir];
                }
                w->color = x->p->color;
                x->p->color = black;
                w->child[!dir]->color = black;
                rotate(x->p, dir);
                x = rt;
            }
        }

        x->color = black;
    }

    /**
     * @brief   transplant node v in place of u
     *
     * @param   u   node to be transplanted by v
     * @param   v   node to transplant u
     */
    void p_transplant(Node *u, Node *v)
    {
        if (u->p == sentinel)
            rt = v;
        else if (u == u->p->child[left])
            u->p->child[left] = v;
        else
            u->p->child[right] = v;
        v->p = u->p;
        // successor update: u's parent lost u, gained v
        u->p->subTreeSize -= (u->subTreeSize - v->subTreeSize);
    }

    /**
     * @brief   recursively searches appropiate sub-subtree of the given node r for k
     *
     * @param   r       the given subtree root node
     * @param   k       key to be searched
     * @return  Node*   the node with the key equal k or NULL
     */
    Node *p_search(Node *r, int k)
    {
        // std::cout << k << " ";
        if (r == sentinel)
        {
            return NULL;
        }
        else if (k == r->key)
        {
            return r;
        }
        else
        {
            int dir = k > r->key; // 1 sends to right, 0 to left
            Node *toBeSearched = r->child[dir];
            Node *res = p_search(toBeSearched, k);
            if (res != NULL)
            {
                return res;
            }
            else
            {
                return NULL;
            }
        }
    }

    /**
     * @brief       we update subTreeSize for each ancestor of y
     *
     * @param   y   the node getting deleted
     */
    void p_subTreeSizeUpdate(Node *y)
    {
        Node *curr = y->p;

        while (curr != sentinel)
        {
            curr->subTreeSize--;
            curr = curr->p;
        }
    }

public:
    rbtree() : sentinel(new Node(noKey, black))
    {
        rt = sentinel;
        rt->child[left] = rt->child[right] = rt->p = sentinel;
    }
    ~rbtree() {}

    /**
     * @brief   returns true if value inserted successfully
     *
     * @param   k       value to insert
     * @return  true    value inserted
     * @return  false   value already in tree
     */
    bool insert(int k)
    {
        if (search(k))
            return false;

        Node *z = new Node(k, red);
        z->p = z->child[right] = z->child[left] = sentinel;
        Node *y = sentinel, *x = rt;

        int dir;
        while (x != sentinel)
        {
            y = x;
            y->subTreeSize++;
            dir = z->key > x->key; // 1 to right, 0 to left;
            x = x->child[dir];
        }

        z->p = y;

        if (y == sentinel)
        {
            rt = z;
        }
        else
        {
            int dir = z->key > y->key; // 1 to right, 0 to left;
            y->child[dir] = z;
        }
        z->child[left] = z->child[right] = sentinel;
        z->color = red;

        p_insertFixUp(z);
        return true;
    }

    /**
     * @brief   Returns true if value was removed from tree.
     *          Notice the node y is the one actually getting axed.
     *          So after fixing y properly, we loop back till root to
     *          decrease subTreeSize.
     *
     * @param   k       value to delete
     * @return  true    value was in tree, deleted now
     * @return  false   value was not in tree
     */
    bool remove(int k)
    {
        if (!search(k))
            return false;

        // deletion
        Node *x, *y, *z;

        y = z = p_search(rt, k);
        int y_og_color = y->color;

        if (z->child[left] == sentinel)
        {
            p_subTreeSizeUpdate(y);
            x = z->child[right];
            p_transplant(z, z->child[right]);
        }
        else if ((z->child[right] == sentinel))
        {
            p_subTreeSizeUpdate(y);
            x = z->child[left];
            p_transplant(z, z->child[left]);
        }
        else
        {
            y = tree_minimum(z->child[right]);
            y_og_color = y->color;
            p_subTreeSizeUpdate(y);

            x = y->child[right];

            if (y->p == z)
            {
                x->p = y;
            }
            else
            {
                p_transplant(y, y->child[right]);
                y->child[right] = z->child[right];
                y->child[right]->p = y;
            }

            p_transplant(z, y);
            y->child[left] = z->child[left];
            y->child[left]->p = y;
            y->color = z->color;
        }

        if (y_og_color == black)
            p_deleteFixUp(x);

        return true;
    }
    /**
     * @brief   returns true if value is in tree
     *
     * @param   k   the value to look for
     * @return  true    value in tree
     * @return  false   value not in tree
     */
    bool search(int k)
    {
        // if (p_search(rt, k) != NULL)
        //     return true;
        // return false;
        Node *curr = rt;

        while (curr != sentinel)
        {
            if (k == curr->key)
                return true;
            else if (k < curr->key)
                curr = curr->child[left];
            else
                curr = curr->child[right];
        }
        return false;
    }

    /**
     * @brief   returns how many keys are less than k by checking
     *          the possible subtree of k
     *
     * @param   k   the value to look for
     * @return  how many keys are less than k
     */
    int lesserKeyCount(int k)
    {
        Node *curr = rt;
        int r = 0;

        while (curr != sentinel)
        {
            if (k < curr->key)
            {
                curr = curr->child[left];
            }
            else if (k == curr->key)
            {
                if (curr->child[left] != sentinel)
                {
                    r += curr->child[left]->subTreeSize;
                }

                return r;
            }
            else if (k > curr->key)
            {
                if (curr->child[left] != sentinel)
                {
                    r += curr->child[left]->subTreeSize;
                }
                r++; // for this node

                curr = curr->child[right];
            }
        }
        return r;
    }

    /**
     * @brief   Get the Root object
     *
     * @return  Node*
     */
    Node *getRoot()
    {
        if (rt == sentinel || rt->key == noKey)
            return NULL;
        return rt;
    }
    /**
     * @brief   inOrder printing of tree
     *
     * @param   root    root of tree
     */
    void printInOrder(Node *root)
    {
        if (root != sentinel)
        {
            printInOrder(root->child[left]);
            std::cout << root->key << " ";
            printInOrder(root->child[right]);
        }
    }
};
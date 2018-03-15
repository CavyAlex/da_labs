#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <ctime>
using namespace std;
struct TNode {
    int l, r;
    TNode* par;
    TNode* link;
    map<char, TNode*> next;
    TNode(int l, int r, TNode* par)
        : l(l), r(r), par(par), link(NULL) {}
    int len() { 
        return r - l + 1; 
    }
};

string s;
int leaf;
TNode* blank;
TNode* root;

struct TVirtualNode {
    TNode* n;
    int c;
    bool IsNext(int i) {
        if (c == n->r || n == root) {
            return ((n->next).count(s[i]) > 0);
        }
        else {
            return (s[c + 1] == s[i]);
        }
    } 
};

void Split(int i, TVirtualNode &sp) {
    if (sp.n != root && sp.c != sp.n->r) {
        TNode* parent = sp.n->par;
        TNode* tmp1 = new TNode(sp.n->l, sp.c, parent);
        sp.n->par = tmp1;
        parent->next[s[sp.n->l]] = tmp1;
        sp.n->l = sp.c + 1;
        tmp1->next[s[sp.n->l]] = sp.n;
        TNode* tmp2 = new TNode(i, leaf, tmp1);
        tmp1->next[s[i]] = tmp2;
        sp.n = tmp1;
        sp.c = 0;
    }
    else {
        TNode* tmp2 = new TNode(i, leaf, sp.n);
        tmp2->par = sp.n;
        (sp.n->next)[s[i]] = tmp2;
    }
}

void NextLetter(TVirtualNode &v, int i) {
    if (v.c == v.n->r || v.n == root) {
        v.n = v.n->next[s[i]];
        v.c = v.n->l;
    }
    else {
        v.c++;
    }
}



void Go(TVirtualNode &v) {
    TNode* u = v.n;
    if (u != root)
        u = u->par;
    else{
        u = u->link;
        v.n = u;
        v.c = 0;
        return;
    }
    u = u->link;
    int b = v.n->l, e = v.n->r;
    u = u->next[s[b]];
    while ((e - b + 1) > 0) {
        if ((e - b + 1) == u->len()) {
            v.n = u;
            v.c = u->r;
            return;
        }
        else if ((e - b + 1) < u->len()) {
            v.n = u;
            v.c = u->l + e - b;
            return;
        }
        else {
            b += u->len();
            u = u->next[s[b]];
        }
    }
    v.n = u;
    v.c = v.n->l;
}

int main() {
    root = new TNode(0, 0, NULL);
    blank = new TNode(0, 0, NULL);
    root->link = blank;
    for (char i = 'a'; i <= 'z'; i++)
        blank->next[i] =  root;
    cin >> s;
    string min1 = s;
    int l = s.length();
    s += s;
    leaf = s.length() - 1;
    TVirtualNode sp;
    sp.n = root;
    sp.c = 0;
    TNode* tmp;
    for (int i = 0; i < s.length(); i++) {
        if (i == l - 1)
            i = i;
        tmp = NULL;
        while (!sp.IsNext(i)) {
            Split(i, sp);
            if (tmp != NULL) {
                tmp->link = sp.n;
            }
            tmp = sp.n;
            Go(sp);
            if(sp.c == sp.n->l || sp.c == sp.n->r)
                tmp->link = sp.n;
        }
        NextLetter(sp, i);
    }
    string ans = "";
    sp.n = root;
    sp.c = 0;
    int l1 = l;
    while (l > 0) {
        TNode* tmp = sp.n->next[(sp.n->next).begin()->first];
        if (tmp->len() <= l) {
            l -= tmp->len();
            for (int i = tmp->l; i <= tmp->r; i++) {
                ans += s[i];
                sp.n = tmp;
            }
        }
        else {
            for (int i = tmp->l; i < (tmp->l + l); i++) {
                ans += s[i];
            }
            l = 0;
        }
    }
    cout << ans << endl;
    return 0;
}

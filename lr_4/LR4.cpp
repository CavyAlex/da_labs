#include <iostream>
#include <string>
#include <queue>
#include <map>

using namespace std;
class TItem {
public:
    map<unsigned  int, TItem*> next;
    vector<int> leaf;
    TItem* parent;
    TItem* s1;
    TItem* s2;
    unsigned int v;
    TItem(char v1) {
        s1 = NULL;
        s2 = NULL;
        v = v1;
    }
};

void Bor(TItem* root, vector<unsigned int>& n, unsigned int leaf) {
    TItem* tmp = root;
    for (unsigned int i = 0; i < n.size(); i++) {
        if (tmp->next.find(n[i]) != tmp->next.end())
            tmp = tmp->next[n[i]];
        else {
            tmp->next[n[i]] = new TItem(n[i]);
            tmp->next[n[i]]->parent = tmp;
            tmp = tmp->next[n[i]];
        }
        if (i == (n.size() - 1)) {
            tmp->leaf.push_back(leaf);
        }
    }
}

void MakeLinks(TItem* root) {
    queue <TItem*> q;
    TItem* tmp;
    q.push(root);
    while (!q.empty()) {
        tmp = q.front();
        q.pop();
        if (tmp == root) {
            tmp->s1 = root;
        }
        else if (tmp->parent == root) {
            tmp->s1 = root;
        }
        else {
            TItem* p = tmp->parent;
            p = p->s1;
            unsigned int index = tmp->v;
            while (p != root && (p->next.find(index) == p->next.end()))
                p = p->s1;
            if (p->next.find(index) != p->next.end()) {
                tmp->s1 = p->next[index];
                if (!p->next[index]->leaf.empty())
                    tmp->s2 = p->next[index];
                else if(p->next[index]->s2 != NULL)
                    tmp->s2 = p->next[index]->s2;
            }
            else
                tmp->s1 = root;
        }
        for (map<unsigned int, TItem*>::iterator i = tmp->next.begin(); i != tmp->next.end(); i++) {
            q.push(i->second);
        }
    }
}

int main() {
    TItem* root = new TItem(0);
    unsigned int pattern1 = 0;
    unsigned int count_word = 0, count_all = 0;
    vector <unsigned int> pattern;
    char c;
    bool f = 0;
    while ((c = getchar()) != '\n') {
        if (c >= '0' && c <= '9') {
            pattern1 = pattern1 * 10 + (c - '0');
            f = 1;
        }
        else if (c == '?') {
            if (pattern.size() > 0) {
                Bor(root, pattern, count_all - 1);
                pattern.clear();
                count_word++;
                pattern1 = 0;
            }
            count_all++;
        }

        else if (f) {
            pattern.push_back(pattern1);
            count_all++;
            pattern1 = 0;
            f = 0;
        }
    }
    if (f) {
        pattern.push_back(pattern1);
        count_all++;
    }
    pattern1 = 0;
    if (pattern.size() > 0) {
        Bor(root, pattern, count_all - 1);
        pattern.clear();
        count_word++;
    }
    pattern.clear();
    MakeLinks(root);
    vector<unsigned int> text;
    unsigned int indrow = 1, indcol = 1;
    vector <pair<unsigned int, unsigned int> > index;
    f = 0;
    while ((c = getchar()) != EOF) {

        if (c == 'a')
            break;

        if (c >= '0' && c <= '9') {
            pattern1 = pattern1 * 10 + (c - '0');
            f = 1;
        }
        else {
            if (f) {
                text.push_back(pattern1);
                index.push_back(make_pair(indrow, indcol));
                pattern1 = 0;
                f = 0;
                indcol++;
            }
            if (c == '\n') {
                indcol = 1;
                indrow++;
            }
        }
    }
    int text_size = text.size();
    TItem* cur = root;
    vector<unsigned int> count(text_size, 0);
    for (int i = 0; i < text_size; i++) {
        while (cur->next.find(text[i]) == cur->next.end() && cur != root) {
            cur = cur->s1;
        }
        if (cur->next.find(text[i]) != cur->next.end()) {
            cur = cur->next[text[i]];
            if (cur->leaf.size() > 0) {
                for (int j = 0; j < cur->leaf.size(); j++)
                    if ((i - cur->leaf[j]) >= 0)
                        count[i - cur->leaf[j]]++;
            }
            TItem* exit = cur->s2;
            while (exit != NULL) {
                for (int j = 0; j < exit->leaf.size(); j++)
                    if (i - exit->leaf[j] >= 0)
                        count[i - exit->leaf[j]]++;
                exit = exit->s2;
            }
        }
    }
    int n = (text_size - count_all);
    for (int i = 0; i <= n; i++) {
        if (count[i] == count_word)
            cout << index[i].first << ", " << index[i].second << endl;
    }
    return 0;
}


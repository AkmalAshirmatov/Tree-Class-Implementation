#include <iostream>
#include "Mem.h"
#include "Tree.h"
#include <vector>
#include <string>
#include <numeric>
using namespace std;

int testFailed(string test_name) {
    cout << test_name + " failed.\n\n";
    return 0;
}

int testPassed(string test_name) {
    cout << test_name + " passed.\n\n";
    return 1;
}

int test_12_gotonext_remove_1_2_11() {
    string test_name = "test_12_gotonext_remove_1_2_11";
    Mem mem(100);
    Tree tree(mem);
    Tree::Iterator* iterator = tree.begin();
    vector<int> vt = {0, 1, 2};
    for (auto v : vt) {
        if (tree.insert(iterator, 0, NULL, 3 - v) == 1) {
            cout << "insert " << v << " failed\n"; return testFailed(test_name);
        }
    }
    int counter = 4;
    for (int i = 0; i < 3; i++) {
        if (iterator->goToChild(i) == 0) {
            cout << "cannot go to " << i << "\n"; return testFailed(test_name);
        }
        for (auto v : vt) {
            if (tree.insert(iterator, v, NULL, counter++) == 1) {
                cout << "insert " << v << " failed\n"; return testFailed(test_name);
            }
        }
        if (iterator->goToParent() == 0) {
            cout << "cannot go back from" << i << "\n"; return testFailed(test_name);
        }
    }
    vt = {1, 4, 5, 6, 2, 7, 8, 9, 3, 10, 11, 12};
    counter = 0;
    while (iterator->hasNext()) {
        iterator->goToNext();
        size_t size;
        iterator->getElement(size);
        if (vt[counter] != size) {
            cout << "error in traversing with " << counter << "\n"; return testFailed(test_name);
        }
        counter++;
    }
    iterator = tree.begin();
    if (iterator->goToChild(0) == 0) {
        cout << "cannot go to 1\n"; return testFailed(test_name);
    }
    for (int i = 0; i < 2; i++) {
        if (tree.remove(iterator, 1) == 1) {
            cout << "deleted " << i + 1 << "with error leaf_only\n"; return testFailed(test_name);
        }
        if (tree.remove(iterator, 0) == 0) {
            cout << "cannot delete " << i + 1 << "\n"; return testFailed(test_name);
        }
    }
    if (iterator->goToChild(1) == 0) {
        cout << "cannot go to 11\n"; return testFailed(test_name);
    }
    if (tree.remove(iterator, 1) == 0) {
        cout << "cannot delete 11\n"; return testFailed(test_name);
    }
    vt = {3, 10, 12};
    counter = 0;
    if (tree.size() != 3) {
        cout << "wrong tree size\n"; return testFailed(test_name);
    }
    iterator = tree.begin();
    while (iterator->hasNext()) {
        iterator->goToNext();
        size_t size;
        iterator->getElement(size);
        if (vt[counter] != size) {
            cout << "error in traversing with " << counter << "\n"; return testFailed(test_name);
        }
        counter++;
    }
    for (auto x : vt) {
        iterator = tree.find(NULL, x);
        if (iterator->vertex() == NULL) {
            cout << "not found " << x << "\n"; return testFailed(test_name);
        }
    }
    vt = {1, 4, 5, 6, 2, 7, 8, 9};
    for (auto x : vt) {
        iterator = tree.find(NULL, x);
        if (iterator->vertex() != NULL) {
            cout << "found " << x << "\n"; return testFailed(test_name);
        }
    }
    return testPassed(test_name);
}

int test_address_equals() {
    string test_name = "test_tree_10_address_equals";
    Mem mem(100);
    Tree tree(mem);
    Tree::Iterator *iterator = tree.begin();
    vector<int> vt(10000);
    iota(vt.begin(), vt.end(), 0);
    for (auto &v : vt) {
        if (tree.insert(iterator, v, &v, sizeof(int)) == 1) {
            cout << "insert " << v << " failed\n"; return testFailed(test_name);
        }
    }
    iterator = tree.begin();
    int counter = 0;
    while (iterator->hasNext()) {
        iterator->goToNext();
        size_t size;
        int v = *static_cast<int*>(iterator->getElement(size));
        if (vt[counter++] != v) {
            cout << "error in comparing values with " << counter << "\n"; return testFailed(test_name);
        }
    }
    iterator = tree.begin();
    if (iterator->goToChild(1) == 0) {
        cout << "cannot go to 1\n"; return testFailed(test_name);
    }
    Tree::Iterator *niterator = tree.begin();
    if (niterator->goToChild(0) == 0) {
        cout << "cannot go to 0\n"; return testFailed(test_name);
    }
    if (niterator->hasNext() == 0) {
        cout << "cannot go to 1\n"; return testFailed(test_name);
    }
    niterator->goToNext();
    if (iterator->equals(niterator) == 0) {
        cout << "error with equal\n"; return testFailed(test_name);
    }
    return testPassed(test_name);
}

int test_empty_iterator_get_element() {
    string test_name = "test_empty_iterator_get_element";
    Mem mem(100);
    Tree tree(mem);
    int flag = 0;
    try {
        Tree::Iterator *iterator = tree.newIterator();
        size_t size;
        iterator->getElement(size);
    }
    catch(Container::Error error) {
        string s_error = "Called getElement from empty Iterator";
        if (s_error != error.msg) {
            cout << "Called with other error\n"; return testFailed(test_name);
        }
        flag = 1;
    }
    if (!flag) {
        cout << "Didn't have throw\n"; return testFailed(test_name);
    }
    return testPassed(test_name);
}

int test_1e6_vertex() {
    string test_name = "test_1e6_vertex";
    Mem mem(100);
    Tree tree(mem);
    Tree::Iterator *iterator = tree.begin();

    const int N = 1000;
    vector<int> vt(N);
    iota(vt.begin(), vt.end(), 0);
    for (auto &v : vt) {
        if (tree.insert(iterator, 0, NULL, N * (N - 1) - v * N) == 1) {
            cout << "insert " << v << " failed\n"; return testFailed(test_name);
        }
    }
    //0,1000,2000,3000,4000...
    vt.pop_back(); // 999 childs
    for (int i = 0; i < N; i++) {
        if (iterator->goToChild(i) == 0) {
            cout << "cannot go to " << i << "\n"; return testFailed(test_name);
        }
        for (auto v : vt) {
            int last = (i + 1) * N - 1;
            if (tree.insert(iterator, 0, NULL, last - v) == 1) {
                cout << "insert " << v << " failed\n"; return testFailed(test_name);
            }
        }
        if (iterator->goToParent() == 0) {
            cout << "cannot go back from" << i << "\n"; return testFailed(test_name);
        }
    }
    vt.resize(N * N);
    iota(vt.begin(), vt.end(), 0);
    int counter = 0;
    if (tree.size() != N * N) {
        cout << "wrong tree size\n"; return testFailed(test_name);
    }
    iterator = tree.begin();
    while (iterator->hasNext()) {
        iterator->goToNext();
        size_t size;
        iterator->getElement(size);
        if (counter >= vt.size()) {
            cout << "out of range\n"; return testFailed(test_name);
        }
        if (vt[counter] != size) {
            cout << "error in traversing with " << counter << "\n"; cout << size << endl; return testFailed(test_name);
        }
        counter++;
    }
    if (counter != vt.size()) {
        return testFailed(test_name);
    }
    return testPassed(test_name);
}

int test_1e4_delete_odd() {
    string test_name = "test_1e4_delete_odd";
    Mem mem(100);
    Tree tree(mem);
    Tree::Iterator *iterator = tree.begin();
    int N = 1e4;
    vector<int> vt(N);
    iota(vt.begin(), vt.end(), 0);
    for (auto &v : vt) {
        if (tree.insert(iterator, 0, &v, 0) == 1) {
            cout << "insert " << v << " failed\n"; return testFailed(test_name);
        }
    }
    iterator = tree.begin();
    int index = N - 1;
    while (iterator->hasNext()) {
        iterator->goToNext();
        tree.remove(iterator);
        index--;
        size_t size;
        int v = *static_cast<int*>(iterator->getElement(size));
        if (index < 0) {
            cout << "error in deleting, indexing vector\n"; return testFailed(test_name);
        }
        if (vt[index] != v) {
            cout << "error in comparing values with " << vt[index] << " " << v << "\n"; return testFailed(test_name);
        }
        index--;
    }
    if (index != -1) {
        cout << "didn't go through elements\n"; return testFailed(test_name);
    }
    return testPassed(test_name);
}

//1.0 * clock() / CLOCKS_PER_SEC

Mem mem(100);
Tree tree(mem);

const int SIZETREE = 16;

void dfs(Tree::Iterator* it, int v) {
    if (v * 2 <= SIZETREE) {
        tree.insert(it, 0, NULL, v * 2);
        it->goToChild(0);
        dfs(it, v * 2);
        it->goToParent();
    }
    if (v * 2 + 1 <= SIZETREE) {
        tree.insert(it, 1, NULL, v * 2 + 1);
        it->goToChild(1);
        dfs(it, v * 2 + 1);
        it->goToParent();
    }
}

void test() {
    Tree::Iterator* it = tree.begin();
    dfs(it, 1);
    it = tree.begin();
    while (it->hasNext()) {
        it->goToNext();
        size_t size;
        it->getElement(size);
        cout << "vertex = " << size << " ";
        Tree::Vertex* v = it->parent();
        cout << "parent = " << v->element_size() << endl;
    }
}

int main() {
//    if (!test_empty_iterator_get_element())
//        return 0;
//    if (!test_12_gotonext_remove_1_2_11())
//        return 0;
//    if (!test_address_equals())
//        return 0;
//    if (!test_1e6_vertex())
//        return 0;
//    if (!test_1e4_delete_odd())
//        return 0;
    test();
}
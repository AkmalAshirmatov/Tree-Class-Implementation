#include "Container.h"
#include "TreeAbstract.h"
#include "Tree.h"

void Tree::removeDFS(Tree::Vertex* v, bool remove_v) {
    Vertex *u = v->first_child();
    while (u != NULL) {
        Vertex *nu = u->right_brother();
        removeDFS(u, 1);
        u = nu;
    }
    v->set_first_child(NULL);
    if (remove_v) {
        change_size(-1);
        free_mem(v);
    }
}

Tree::Vertex* Tree::findLeft(Vertex* p, Vertex* v) {
    Vertex* u = p->first_child();
    while (u != NULL) {
        if (u->right_brother() == v) {
            return u;
        }
        u = u->right_brother();
    }
    return NULL;
}

Tree::Tree(MemoryManager &mem) : AbstractTree(mem) {
    this->_memory = mem;
    _root = new (alloc_mem(sizeof(Vertex))) Vertex(NULL, 0, NULL);
    _size = 0;
}

Tree::~Tree() {
    Tree::clear();
}

Tree::Vertex* Tree::Iterator::goNext(int flag) {
    Vertex* v = this->vertex();
    stack<Vertex*> st = this->parents;
    if (v->first_child() != NULL) {
        if (flag)
            parents.push(v);
        return v->first_child();
    }
    while (v != NULL) {
        if (v->right_brother() != NULL) {
            return v->right_brother();
        }
        if (!flag) {
            if (st.empty()) v = NULL;
            else v = st.top(), st.pop();
        }
        else {
            if (parents.empty()) v = NULL;
            else v = parents.top(), parents.pop();
        }
    }
    return NULL;
}

void* Tree::Iterator::getElement(size_t &size) {
    if (_vertex == NULL) {
        throw Container::Error("Called getElement from empty Iterator");
    }
    size = _vertex->element_size();
    return _vertex->element();
}

bool Tree::Iterator::hasNext() {
    if (_vertex == NULL)
        return false;
    if (goNext(0) == NULL)
        return false;
    return true;
}

void Tree::Iterator::goToNext() {
    if (!hasNext()) {
        throw Container::Error("Wanted to go next, but it doesn't exist");
    }
    _vertex = goNext(1);
}

bool Tree::Iterator::equals(Container::Iterator *rgt) {
    if (rgt == NULL) // be careful
        return false;
    Tree::Iterator *right = dynamic_cast<Tree::Iterator*>(rgt);
    if (this->vertex() == NULL || right->vertex() == NULL)
        return false;
    return this->vertex() == right->vertex();
}

Tree::Vertex* Tree::Iterator::parent() {
    if (parents.empty())
        return NULL;
    else
        return parents.top();
}

bool Tree::Iterator::goToParent() {
    if (_vertex == NULL)
        return false;
    if (parents.empty())
        return false;
    _vertex = parents.top();
    parents.pop();
    return true;
}

bool Tree::Iterator::goToChild(int child_index) {
    Vertex* child = _vertex->first_child();
    for (int i = 0; i < child_index; i++) {
        if (child == NULL)
            return false;
        child = child->right_brother();
    }
    if (child == NULL)
        return false;
    parents.push(_vertex);
    _vertex = child;
    return true;
}

Tree::Iterator* Tree::find(void *elem, size_t size) {
    Tree::Iterator *iterator = new (alloc_mem(sizeof(Iterator))) Iterator(_root);
    while (iterator->hasNext()) {
        iterator->goToNext();
        size_t nsize;
        void *nelem = iterator->getElement(nsize);
        if (elem == nelem && size == nsize) {
            return iterator;
        }
    }
    return newIterator();
}

void Tree::remove(Container::Iterator *it) {
    Tree::Iterator *iter = dynamic_cast<Tree::Iterator*>(it);
    remove(iter, 1);
}

void Tree::clear() {
    Tree:Iterator* iterator = Tree::begin();
    remove(iterator, 0);
    _root = new (alloc_mem(sizeof(Vertex))) Vertex(NULL, 0, NULL);
    _size = 0;
}

int Tree::insert(AbstractTree::Iterator *it, int child_index, void *elem, size_t size) {
    Tree::Iterator *iter = dynamic_cast<Tree::Iterator*>(it);
    if (iter->vertex() == NULL || child_index < 0) {
        return 1;
    }
    Vertex* vertex = iter->vertex();
    Vertex* nvertex = new (alloc_mem(sizeof(Vertex))) Vertex(elem, size, vertex);
    Vertex* first_child = vertex->first_child();
    if (child_index == 0) {
        vertex->set_first_child(nvertex);
        nvertex->set_right_brother(first_child);
    }
    else {
        for (int i = 0; i < child_index - 1; i++) {
            first_child = first_child->right_brother();
            if (first_child == NULL) {
                return 1;
            }
        }
        nvertex->set_right_brother(first_child->right_brother());
        first_child->set_right_brother(nvertex);
    }
    change_size(1);
    return 0;
}

bool Tree::remove(AbstractTree::Iterator *it, int leaf_only) {
    Tree::Iterator *iter = dynamic_cast<Tree::Iterator*>(it);
	if (iter->vertex() == NULL) {
        return 1;
    }
    Vertex* v = iter->vertex();
    if (leaf_only == 1 && v->first_child() != NULL) {
        return false;
    }
    Vertex* p = iter->parent();
    if (p != NULL && p->first_child() == v) {
        p->set_first_child(v->right_brother());
    }
    else if (p != NULL && p->first_child() != v) {
        Vertex* left_brother = findLeft(p, v);
        left_brother->set_right_brother(v->right_brother());
    }
    removeDFS(v, 0);
    if (iter->hasNext())
        iter->goToNext();
    else
        iter->set_vertex(NULL);
    removeDFS(v, 1);
    return true;
}

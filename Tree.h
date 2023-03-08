#pragma once
#include "TreeAbstract.h"
#include <iostream>
#include <stack>

class Tree : public AbstractTree {

public:
    int _size;

    class Vertex {

    private:
        void *_element;
        size_t _element_size;
        Vertex *_right_brother;
        Vertex* _first_child;

    public:
        Vertex(void *_element, size_t _size_element, Vertex* _parent) {
            this->_element = _element;
            this->_element_size = _size_element;
            this->_right_brother = NULL;
            this->_first_child = NULL;
        }

        void* element() { return this->_element; }
        size_t element_size() { return this->_element_size; }

        Vertex* first_child() { return _first_child; }
        void set_first_child(Vertex* _first_child) { this->_first_child = _first_child; }

        Vertex* right_brother() { return _right_brother; }
        void set_right_brother(Vertex* _right_brother) { this->_right_brother = _right_brother; }

    };

    Vertex* _root;

    void removeDFS(Vertex* v, bool remove_v);

    Vertex* findLeft(Vertex* p, Vertex* v);

    Tree(MemoryManager &mem);

    ~Tree();

    void *alloc_mem(size_t size) { return _memory.allocMem(size); }

    void free_mem(void* ptr) { _memory.freeMem(ptr); }

    class Iterator: public AbstractTree::Iterator {

    private:
        Vertex* _vertex;
        stack<Vertex*> parents;

        Vertex* goNext(int flag);

    public:
        Iterator() { _vertex = NULL; }

        Iterator(Vertex* vertex) { this->_vertex = vertex; }

        Vertex* vertex() { return _vertex; }
        void set_vertex(Vertex* _vertex) { this->_vertex = _vertex; }

        Vertex* parent();

        void* getElement(size_t &size);

        bool hasNext();

        void goToNext();

        bool equals(Container::Iterator *rgt);

        bool goToParent();

        bool goToChild(int child_index);

    };

    Vertex* root() { return _root; }

    int size() { return _size; }

    void change_size(int delta) { _size += delta; }

    size_t max_bytes() { return this->_memory.maxBytes(); } 

    Tree::Iterator* find(void *elem, size_t size);

    Tree::Iterator* newIterator() { return new (alloc_mem(sizeof(Iterator))) Iterator(); }

    Tree::Iterator* begin() { return new (alloc_mem(sizeof(Iterator))) Iterator(_root); }

    Tree::Iterator* end() { return newIterator(); }

    void remove(Container::Iterator *it);

    void clear();

    bool empty() { return this->size() ? false : true; }

    int insert(AbstractTree::Iterator *it, int child_index, void *elem, size_t size);

    bool remove(AbstractTree::Iterator *it, int leaf_only);

};

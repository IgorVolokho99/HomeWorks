#ifndef ONELINKEDLIST_H_INCLUDED
#define ONELINKEDLIST_H_INCLUDED


template <typename T>
class Node {
public:
    T data;
    Node<T>* next;


    Node (T data) : data(data), next(nullptr) {}
};


template <typename T>
class Iterator {
public:
    Node<T>* current;

    Iterator(Node<T>* node) : current(node) {}

    T& operator*() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to dereference(*) end() iterator");}
        return current->data;
    }

    Iterator& operator++() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to increase(++) end() iterator"); }
        current = current->next;
        return *this;
    }

    Iterator operator++(int) {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to increase(++) end() iterator"); }
        Iterator temp = *this;
        current = current->next;
        return temp;
    }

    Node<T>* operator->() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to (->) end() iterator"); }
        return current;
    }

    bool operator!=(const Iterator& other) {
        return current != other.current;
    }
    bool operator==(const Iterator& other) {
        return current == other.current;
    }
};


template <typename T>
class OneLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    OneLinkedList() { this->head = this->tail = nullptr; }
    ~OneLinkedList() { while (head != nullptr) pop_front(); }

    bool empty() { return head == nullptr; }

    void pop_front()
    {
        if (empty()) return;
        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
            return;
        }

        Node<T>* node = head;
        head = node->next;
        delete node;
        return;
    }

    void push_back(T data)
    {
        Node<T>* node = new Node<T>(data);
        if (empty()){head = node;}
        if (tail != nullptr){ tail->next = node; }
        tail = node;
    }

    void push_front(T data)
    {
        Node<T>* node = new Node<T>(data);
        node->next = head;
        head = node;
        if (tail == nullptr) { tail = node; }
    }

    void pop_back()
    {
        if (tail == nullptr) { return; }
        if (head == tail)
        {
            delete head;
            head = tail = nullptr;
            return;
        }
        if (tail != nullptr)
        {
            Node<T>* node = head;
            while (node->next != tail)
            {
                node = node->next;
            }
            node->next = nullptr;
            delete tail;
            tail = node;
            return;
        }
    }

    void insert_after(Iterator<T>& pos, const T data) {
        if (pos.current == nullptr) { throw std::runtime_error("OperationDoesNotSupportError: Attempt to dereference end() iterator");}

        Node<T>* new_node = new Node<T>(data);
        Node<T>* right = pos->next;
        new_node->next = right;
        pos->next = new_node;
        if (pos.current == tail)
        {
            tail = new_node;
        }
    }

    void erase_after(Iterator<T>& pos) {
        if (pos.current == nullptr) { throw std::runtime_error("OperationDoesNotSupportError: Attempt to dereference end() iterator");}

        if (pos.current == tail) { return; }

        Node<T>* right = pos->next;
        if (right == tail)
        {
            tail = pos.current;
            delete right;
            tail->next = nullptr;
            return;
        }
        Node<T>* new_right = right->next;
        pos->next = new_right;
    };

    T& front() {
        if (empty()) { throw std::runtime_error("EmptyListError: Attempt to access front on empty list");}
        return head->data;
    }

    void remove(const T& data)
    {
        Node<T>* node = head;
        while (node && node->data == data) {
            pop_front();
            node = head;
        }
        Node<T>* next = head ? head->next : nullptr;
        while (next != nullptr)
        {
            if (next->data == data)
            {
                node->next = next->next;
                delete next;
                next = node->next;
            } else {
                node = next;
                next = node->next;
            }
        }

        tail = node;
    }

    Iterator<T> begin()
    {
        return Iterator<T>(head);
    }

    Iterator<T> end()
    {
        return Iterator<T>(nullptr);
    }

    Iterator<T> find(T data)
    {
        Iterator<T> it = begin();
        while (it != end() && *it != data) {
            ++it;
        }
        return it;
    }
};

#endif

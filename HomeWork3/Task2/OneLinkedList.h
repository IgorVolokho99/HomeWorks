#ifndef ONELINKEDLIST_H_INCLUDED
#define ONELINKEDLIST_H_INCLUDED

// Определение узла для односвязного списка.
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;


    Node (T data) : data(data), next(nullptr) {}
};

// Итератор для навигации по односвязному списку.
template <typename T>
class Iterator {
public:
    Node<T>* current; // Текущий узел итератора.

    // Конструктор, инициализирующий итератор с указанным узлом.
    Iterator(Node<T>* node) : current(node) {}

    // Оператор разыменования возвращает ссылку на данные текущего узла.
    T& operator*() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to dereference(*) end() iterator");}
        return current->data;
    }

    // Префиксный инкремент переводит итератор на следующий узел.
    Iterator& operator++() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to increase(++) end() iterator"); }
        current = current->next;
        return *this;
    }

    // Постфиксный инкремент сначала возвращает текущее состояние итератора, затем переводит его на следующий узел.
    Iterator operator++(T) {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to increase(++) end() iterator"); }
        Iterator temp = *this;
        current = current->next;
        return temp;
    }

    // Оператор доступа к члену класса через указатель.
    Node<T>* operator->() {
        if (current == nullptr) { throw std::runtime_error("DoesNotSupportError: Attempt to (->) end() iterator"); }
        return current;
    }

    // Операторы сравнения итераторов.
    bool operator!=(const Iterator& other) {
        return current != other.current;
    }
    bool operator==(const Iterator& other) {
        return current == other.current;
    }
};

// Основной класс односвязного списка.
template <typename T>
class OneLinkedList {
private:
    Node<T>* head; // Указатель на начало списка.
    Node<T>* tail; // Указатель на конец списка.

public:
    // Конструктор, инициализирующий пустой список.
    OneLinkedList() { this->head = this->tail = nullptr; }

    // Деструктор, удаляющий все узлы списка.
    ~OneLinkedList() { while (head != nullptr) pop_front(); }

    // Проверка списка на пустоту.
    bool empty() { return head == nullptr; }

    // Удаление первого элемента списка.
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

    // Добавление элемента в конец списка.
    void push_back(T data)
    {
        Node<T>* node = new Node<T>(data);
        if (empty()){head = node;}
        if (tail != nullptr){ tail->next = node; }
        tail = node;
    }

    // Добавление элемента в начало списка.
    void push_front(T data)
    {
        Node<T>* node = new Node<T>(data);
        node->next = head;
        head = node;
        if (tail == nullptr) { tail = node; }
    }

    // Удаление последнего элемента списка.
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

    // Вставка элемента после указанного итератора.
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

    // Удаление элемента после указанного итератора.
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

    // Получение ссылки на первый элемент списка.
    T& front() {
        if (empty()) { throw std::runtime_error("EmptyListError: Attempt to access front on empty list");}
        return head->data;
    }

    // Удаление всех элементов, равных заданному значению.
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

        tail = node; // Обновление указателя на последний узел
    }

    // Возвращает итератор, указывающий на начало списка.
    Iterator<T> begin()
    {
        return Iterator<T>(head);
    }

    // Возвращает итератор, указывающий на конец списка.
    Iterator<T> end()
    {
        return Iterator<T>(nullptr);
    }

    // Поиск элемента в списке и возвращение итератора на него.
    Iterator<T> find(T data)
    {
        Iterator<T> it = begin();
        while (it != end() && *it != data) {
            ++it;
        }
        return it;
    }
};

#endif // ONELINKEDLIST_H_INCLUDED

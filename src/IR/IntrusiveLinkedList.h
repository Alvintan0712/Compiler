//
// Created by tjan2 on 18/11/2021.
//

#ifndef SRC_INTRUSIVELINKEDLIST_H
#define SRC_INTRUSIVELINKEDLIST_H

template<class T>
class IntrusiveLinkedList {
public:
    T *head, *tail;

    IntrusiveLinkedList();
    bool isEmpty();
    void push_back(T *t);
    void push_front(T *t);
    void insertFront(T *t, T *front);
    void insertBack(T *t, T *back);
    void remove(T *t);
};

template<class T>
IntrusiveLinkedList<T>::IntrusiveLinkedList() {
    head = tail = nullptr;
}

template<class T>
bool IntrusiveLinkedList<T>::isEmpty() {
    return head == nullptr;
}

template<class T>
void IntrusiveLinkedList<T>::push_back(T *t) {
    if (tail) {
        tail->next = t;
        t->prev = tail;
        tail = t;
    } else {
        head = tail = t;
    }
}

template<class T>
void IntrusiveLinkedList<T>::push_front(T *t) {
    if (head) {
        head->prev = t;
        t->next = head;
        head = t;
    } else {
        head = tail = t;
    }
}

template<class T>
void IntrusiveLinkedList<T>::insertFront(T *t, T *front) {
    t->next = front;
    t->prev = front->prev;
    front->prev = t;
    if (front == head) head = t;
}

template<class T>
void IntrusiveLinkedList<T>::insertBack(T *t, T *back) {
    t->next = back->next;
    t->prev = back;
    back->next = t;
    if (back == tail) tail = t;
}

template<class T>
void IntrusiveLinkedList<T>::remove(T *t) {
    if (t->next) t->next->prev = t->prev;
    if (t->prev) t->prev->next = t->next;
    delete t;
}

#endif //SRC_INTRUSIVELINKEDLIST_H

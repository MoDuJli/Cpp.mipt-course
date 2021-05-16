#include <iostream>

using namespace std;


struct ListPair {
    int num;
    ListPair* tail;
};

struct List {
    ListPair* head;
};

List new_list() {
    List* x = new List;
    x->head = nullptr;
    return *x;
}

List new_list(List const &lst) {
    List copy;

    if (lst.head != nullptr) {
        copy.head = new ListPair;
        ListPair *a = lst.head;
        ListPair *x = copy.head;
        x->num = a->num;

        while(a->tail != nullptr) {
            x->tail = new ListPair;
            x = x->tail;
            a = a->tail;
            x->num = a->num;
        }

        x->tail = nullptr;
    } else {
        copy.head = nullptr;
    }

    return copy;
}

List push_list(List &lst, int num) {
    ListPair *x = new ListPair;
    x->num = num;
    x->tail = lst.head;
    lst.head = x;
    return lst;
}

int pop_list(List &lst) {
    ListPair* x = lst.head;
    lst.head = (lst.head)-> tail;
    return x-> num;
}

int peak_list(List const &lst) {
    return lst.head -> num;
}

void delete_list(List &lst) {
    ListPair *x;
    while(lst.head != nullptr) {
        x = lst.head;
        lst.head = lst.head->tail;
        delete x;
    }
    delete &lst;
}

bool is_empty(List const &lst) {
    if (lst.head == nullptr) return true;
    return false;
}

List reverse_list(List &lst) {
    ListPair *x,*t;
    if ((lst.head == nullptr) || (lst.head->tail == nullptr)) return lst;
    else {
        x = lst.head->tail;
        while(true) {
            if(x->tail == nullptr) {
                x->tail = lst.head;
                lst.head = x;
                return lst;
            } else {
                t = x->tail;
                x->tail = lst.head;
                lst.head = x;
                x = t;
            }
        }
    }
}

int main() {
    List* testList = new List;
    ListPair* testHead = new ListPair;
    ListPair* testSecond = new ListPair;
    testList->head = testHead;
    testHead->tail = testSecond;
    testHead->num = 3;
    (testHead->tail)->num = 7;

    testSecond->tail = nullptr;

    List proverka = reverse_list(*testList);
    int a = (proverka.head)->num;
    int b = ((proverka.head)->tail)->num;

    cout << a << " "  << b;

/*
List* testList = new List;
    ListPair* testHead = new ListPair;
    ListPair* testSecond = new ListPair;
    testList->head = testHead;
    testHead->tail = testSecond;
    testHead->num = 3;
    (testHead->tail)->num = 7;

    List proverka = new_list(*testList);
    int a = (proverka.head)->num;
    int b = ((proverka.head)->tail)->num;

    cout << a << " " << b;
*/
    return 0;
}

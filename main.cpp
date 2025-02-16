#include<iostream>
#include<list>
#include <chrono>

using namespace std;

template <class T1, class T2>
class Node
{
private:
    T1 key{};
    T2 value{};
    int degree{};
    Node* child, * sibling, * parent;
public:
    Node()
    {
        child = sibling = parent = NULL;
        degree = 0;
    }

    Node(T1 k, T2 v)
    {
        child = sibling = parent = NULL;
        degree = 0;
        key = k;
        value = v;
    }

    Node<T1, T2>* getChild();

    void setChild(Node<T1, T2>* t);

    Node<T1, T2>* getSibling();

    void setSibling(Node<T1, T2>* t);

    Node<T1, T2>* getParent();

    void setParent(Node<T1, T2>* t);

    T1 getKey();

    void setKey(T1 t);

    T2 getValue();

    void setValue(T2 t);

    int getDegree();

    void setDegree(int t);
};

template <class T1, class T2>
inline Node<T1, T2>* Node<T1, T2>::getChild()
{
    return child;
}
template <class T1, class T2>
inline void Node<T1, T2>::setChild(Node<T1, T2>* t)
{
    child = t;
}
template <class T1, class T2>
inline Node<T1, T2>* Node<T1, T2>::getSibling()
{
    return sibling;
}
template <class T1, class T2>
inline void Node<T1, T2>::setSibling(Node<T1, T2>* t)
{
    sibling = t;
}
template <class T1, class T2>
inline Node<T1, T2>* Node<T1, T2>::getParent()
{
    return parent;
}
template <class T1, class T2>
inline void Node<T1, T2>::setParent(Node<T1, T2>* t)
{
    parent = t;
}
template <class T1, class T2>
inline T1 Node<T1, T2>::getKey()
{
    return key;
}
template <class T1, class T2>
inline void Node<T1, T2>::setKey(T1 t)
{
    key = t;
}
template <class T1, class T2>
inline T2 Node<T1, T2>::getValue()
{
    return value;
}
template <class T1, class T2>
inline void Node<T1, T2>::setValue(T2 t)
{
    value = t;
}
template <class T1, class T2>
inline int Node<T1, T2>::getDegree()
{
    return degree;
}
template <class T1, class T2>
inline void Node<T1, T2>::setDegree(int t)
{
    degree = t;
}


template <class T1, class T2>
class Bin_Heap
{
private:
    Node<T1, T2>* head;

public:
    Bin_Heap()
    {
        head = NULL;
    }

    Bin_Heap(Node<T1, T2>* x)
    {
        this->head = x;
    }

    ~Bin_Heap()
    {
        while (head != NULL)
        {
            this->Remove_Maximum();
        }
    }

    Node<T1, T2>* Begin();

    //оъбединение 2 одинаковых по размеру деревьев
    Node<T1, T2>* Merge_Trees(Node<T1, T2>* n1, Node<T1, T2>* n2)
    {
        //корень 2 дерева n2 >= корня 1 дерева n1
        n1->setParent(n2);
        n1->setSibling(n2->getChild());
        n2->setChild(n1);
        n2->setDegree(n2->getDegree() + 1);

        return n2;
    }

    // слияние двух биномиальных куч
    void Merge(Bin_Heap* b)
    {
        Bin_Heap* H = new Bin_Heap();

        Node<T1, T2>* res = new Node<T1, T2>();
        Node<T1, T2>* t3 = res;

        Node<T1, T2>* t1 = this->head;
        Node<T1, T2>* t2 = b->head;

        if (t1 == NULL)
            H->head = NULL;
        if (t2 == NULL)
            H->head = NULL;
        while (t1 != NULL || t2 != NULL)
        {
            int b1 = t1 == NULL ? 1 : 0;
            int b2 = t2 == NULL ? 1 : 0;
            int b3 = 0;
            if (b1 + b2 == 0)
                b3 = (t1->getDegree() < t2->getDegree()) ? 1 : 0;
            int s = b1 * 100 + b2 * 10 + b3;
            switch (s)
            {
            case 0:
                t3->setSibling(t2);
                t3 = t3->getSibling();
                t2 = t2->getSibling();
                break;
            case 1:
                t3->setSibling(t1);
                t3 = t3->getSibling();
                t1 = t1->getSibling();
                break;
            case 10:
                t3->setSibling(t1);
                t3 = t3->getSibling();
                t1 = t1->getSibling();
                break;
            case 100:
                t3->setSibling(t2);
                t3 = t3->getSibling();
                t2 = t2->getSibling();
                break;
            }
        }

        H->head = res->getSibling();

        if (H->head == NULL)
        {
            this->head = NULL;
            return;
        }

        Node<T1, T2>* prevX = NULL;
        Node<T1, T2>* x = H->head;
        Node<T1, T2>* nextX = x->getSibling();

        while (nextX != NULL)
        {
            if (x->getDegree() != nextX->getDegree() || (nextX->getSibling() != NULL && nextX->getSibling()->getDegree() == x->getDegree()))
            {
                prevX = x;
                x = nextX;
            }
            else if (x->getKey() >= nextX->getKey())
            {
                x->setSibling(nextX->getSibling());
                Merge_Trees(nextX, x);
            }
            else
            {
                if (prevX == NULL)
                    H->head = nextX;
                else
                    prevX->setSibling(nextX);
                Merge_Trees(x, nextX);
                x = nextX;
            }
            nextX = x->getSibling();
        }
        b->head = NULL;
        this->head = H->head;
    }

    //добавление элемента в кучу
    void insert(T1 k, T2 v)
    {
        Node<T1, T2>* node = new Node<T1, T2>;
        node->setKey(k);
        node->setValue(v);

        this->Merge(new Bin_Heap(node));
    }

    //вывод кучи(вывод элементов дерева слева направо)
    void printHeap()
    {
        Node<T1, T2>* cur = head;
        while (cur != NULL)
        {
            cout << "Tree order " << cur->getDegree() << endl;
            cout << "There are " << pow(2, cur->getDegree()) << " nodes in this tree" << endl;
            cout << "Tree nodes:" << endl;

            list<Node<T1, T2>*> heap;
            heap.push_back(cur);

            while (!heap.empty())
            {
                Node<T1, T2>* n = heap.front();
                heap.pop_front();
                cout << "Key: " << n->getKey() << ", Value: " << n->getValue() << " | ";

                if (n->getChild() != NULL)
                {
                    Node<T1, T2>* t = n->getChild();
                    while (t != NULL)
                    {
                        heap.push_back(t);
                        t = t->getSibling();
                    }
                }
            }
            cur = cur->getSibling();
            cout << "\n\n";
        }
    }

    //поиск максимального элемента
    Node<T1, T2>* Find_Maximum()
    {
        Node<T1, T2>* max = head;
        for (Node<T1, T2>* n = head; n != NULL; n = n->getSibling())
        {
            if (n->getKey() > max->getKey())
            {
                max = n;
            }
        }
        return max;
    }

    //удаление максимума
    Node<T1, T2>* Remove_Maximum()
    {
        Node<T1, T2>* max;
        max = Find_Maximum();

        Node<T1, T2>* x = head;
        Node<T1, T2>* prevX = NULL;

        while (x != max)
        {
            prevX = x;
            x = x->getSibling();
        }
        if (prevX == NULL)
            head = x->getSibling();
        else
            prevX->setSibling(x->getSibling());

        Node<T1, T2>* ch = NULL;
        Node<T1, T2>* cur = max->getChild();
        while (cur != NULL)
        {
            Node<T1, T2>* next = cur->getSibling();
            cur->setSibling(ch);
            ch = cur;
            cur = next;
        }

        Bin_Heap* H = new Bin_Heap();
        H->head = ch;
        this->Merge(H);
        return max;
    }

    //изменение приоритета некоторого элемента
    void Change_Key(T1 key, T1 n_key)
    {
        Node<T1, T2>* res = NULL;

        Node<T1, T2>* cur = head;
        while (cur != NULL)
        {
            list<Node<T1, T2>*> heap;
            heap.push_back(cur);

            while (!heap.empty())
            {
                Node<T1, T2>* n = heap.front();
                heap.pop_front();

                if (n->getKey() == key)
                {
                    res = n;
                    break;
                }

                if (n->getChild() != NULL)
                {
                    Node<T1, T2>* t = n->getChild();
                    while (t != NULL)
                    {
                        heap.push_back(t);
                        t = t->getSibling();
                    }
                }
            }
            cur = cur->getSibling();
        }

        if (res == NULL)
            cout << "The item with this key was not found on the heap!\n\n";

        else
        {
            if (n_key > res->getKey())
            {
                res->setKey(n_key);
                while (res->getParent() != NULL && res->getKey() > res->getParent()->getKey())
                {
                    T1 tk = res->getKey();
                    T2 tv = res->getValue();

                    res->setKey(res->getParent()->getKey());
                    res->setValue(res->getParent()->getValue());

                    res->getParent()->setKey(tk);
                    res->getParent()->setValue(tv);

                    res = res->getParent();
                }
            }

            else if (n_key < res->getKey())
            {
                res->setKey(n_key);
                while (res->getChild() != NULL)
                {
                    T1 max = res->getChild()->getKey();
                    Node<T1, T2>* max_n = res->getChild();
                    Node<T1, T2>* s = res->getChild()->getSibling();
                    while (s != NULL)
                    {
                        if (s->getKey() > max)
                        {
                            max = s->getKey();
                            max_n = s;
                        }
                        s = s->getSibling();
                    }
                    T1 tk = res->getKey();;
                    T2 tv = res->getValue();

                    res->setKey(max_n->getKey());
                    res->setValue(max_n->getValue());

                    res = max_n;
                    res->setKey(tk);
                    res->setValue(tv);
                }
            }
        }
    }

    //поиск квантиля Quantile
    double Quantile(T1 key)
    {
        Node<T1, T2>* cur = head;

        double count_m = 0;
        double count_all = 0;


        while (cur != NULL)
        {
            count_all += pow(2, cur->getDegree());
            if (cur->getKey() >= key)
            {
                list<Node<T1, T2>*> heap;
                heap.push_back(cur);

                while (!heap.empty())
                {
                    Node<T1, T2>* n = heap.front();
                    heap.pop_front();

                    count_m++;

                    if (n->getChild() != NULL)
                    {
                        Node<T1, T2>* t = n->getChild();
                        while (t != NULL)
                        {
                            if (t->getKey() >= key)
                                heap.push_back(t);
                            t = t->getSibling();
                        }
                    }
                }
            }
            cur = cur->getSibling();
        }
        return (count_m / count_all);
    }
};
template <class T1, class T2>
inline Node<T1, T2>* Bin_Heap<T1, T2>::Begin()
{
    return head;
}



int main()
{
    Bin_Heap<int, string> A;

    //пример работы операции добавления элемента
    A.insert(5, "t1");
    A.insert(17, "t2");
    A.insert(27, "t3");
    A.insert(12, "t4");
    A.insert(18, "t5");

    //вывод кучи
    A.printHeap();

    cout << "------------\n\n";

    //пример работы функции поиска максимума
    Node<int, string> a2 = *A.Find_Maximum();
    cout << "The maximum heap A is: " << a2.getKey();

    cout << "\n\n------------\n\n";

    //пример работы функции слияния куч
    Bin_Heap<int, string> B;
    B.insert(30, "t6");
    B.insert(12, "t7");
    B.insert(23, "t8");

    A.Merge(&B);
    A.printHeap();
    cout << "------------\n\n";

    //пример работы функции удаления максимума
    A.Remove_Maximum();
    A.printHeap();

    cout << "------------\n\n";

    ////пример работы функции изменения приоритета элемента
    Bin_Heap<int, string> C;
    C.insert(15, "t1");
    C.insert(25, "t2");
    C.insert(13, "t3");
    C.insert(7, "t4");
    C.insert(5, "t5");

    C.Change_Key(25, 6);
    C.Change_Key(7, 30);
    C.printHeap();

    cout << "------------\n\n";

    //пример работы функции поиска квантиля
    cout << "The quantile for key 14 in heap C is: " << C.Quantile(14);
    cout << "\n\n------------\n\n";

    //вывод элементов по убыванию приоритета
    while (C.Begin() != NULL)
    {
        cout << "Heap element C with key: " << C.Remove_Maximum()->getKey() << endl;
    }
    cout << "\n------------\n\n";
}

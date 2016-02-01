#include <initializer_list>
#include <iostream>


struct Node {

    Node(int value, Node* next = nullptr): value(value), next(next) {}

    ~Node()
    {
        if (next) {
            delete next;
        }
    }

    int value;
    Node* next;
};


Node* create(std::initializer_list<int> values)
{
    Node* node = nullptr;

    const auto rend = values.begin() - 1;
    for (auto it = values.end() - 1; it != rend; --it) {
        node = new Node{*it, node};
    }

    return node;
}

void remove(const Node* head)
{
    if (head) {
        delete head;
    }
}

void print(const Node* head)
{
    std::cout << "[ ";
    for (auto node = head; node; node = node->next) {
        std::cout << node->value << " ";
    }
    std::cout << "]" << std::endl;
}

namespace reverse {

    Node* recursive(Node* head)
    {
        if (not head or not head->next) {
            return head;
        }
        Node* node = recursive(head->next);
        head->next->next = head;
        head->next = nullptr;
        return node;
    }

    Node* iterative(Node *head)
    {
        if (head) {
            Node* previous = nullptr;
            for (Node* next = nullptr; head->next; ) {
                next = head->next;
                head->next = previous;
                previous = head;
                head = next;
            }
            head->next = previous;
        }

        return head;
    }

}; // end of namespace reverse


int main() {

    Node* head = create({1, 2 , 3, 4, 5});
    print(head);

    head = reverse::iterative(head);
    print(head);

    head = reverse::recursive(head);
    print(head);

    remove(head);

    return 0;
}

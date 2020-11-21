/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

using namespace std;

class Node{
    public:
        int data;
        Node *next;
};

void print_list(Node * list)
{
    Node *node = list;

    if (list == NULL)
    {
        cout << "Empty!\n";
        return;
    }

    do{
        cout << node->data << " ";
        node = node->next;
    }
    while(node != NULL);
}

void push_back(Node** list, int value)
{
    Node* tmp = new Node();
    Node* n = *list;

    tmp->data = value;
    tmp->next = NULL;

    if (n == NULL)
    {
        *list = tmp;
        return;
    }

    while(n->next != NULL)
    {
        n = n->next;
    }

    n->next = tmp;
}

void push_front(Node **list, int value)
{
    Node *tmp = new Node();
    Node *n = *list;

    tmp->data = value;
    tmp->next = NULL;

    if (n == NULL)
    {
        *list = tmp;
        return;
    }

    tmp->next = *list;
    *list = tmp;
}


Node * add_pos(Node *list, int value, int pos)
{
    Node *node = new Node(); //Dung pointer(not de-allocate) -> vung` nho van con` sau khi out of this function.
    Node *n = list;

    node->data = value;
    node->next = NULL;

    if (n == NULL){
        list = node;
        return list;
    }

    if (pos == 0)
    {
        node->next = list;
        return node;
    }

    for(int i =1; i <pos ; i++){
        n = n->next;
        if(n==NULL){
            cout << "pos > size of list\n";
            return list;
        }
    }

    node->next = n->next; //dung` node de go through list -> at pos, dung` node->next -> truc tiep change address ma` next point to.
    n->next=node;

    return list;
}

Node * del_pos(Node *list, int pos)
{
    Node *n = list;

    if (n == NULL){
        cout<<"Empty\n";
        return NULL;
    }

    if (pos == 0)
    {
        return list->next;
    }

    for(int i =1; i <pos ; i++){
        n = n->next;
        if(n==NULL){
            cout << "pos > size of list\n";
            return list;
        }
    }
    n->next = (n->next)->next;

    return list;
}

int main()
{
    Node *list = NULL;
    push_back(&list,1);
    push_back(&list,2);
    push_back(&list,3);
    push_back(&list,4);

    push_front(&list,-1);
    push_front(&list,-2);

    print_list(list);

    //list = add_pos(list,0,2);
    list = del_pos(list,1);

    cout <<endl;
    print_list(list);


    return 0;
}

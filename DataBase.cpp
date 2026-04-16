#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include "httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

class Stack
{
    int top;
    int size;
    int* arr = nullptr;

    public: 
    Stack(int a)
    {
        cout << "Stack created";
        size = a;
        top = -1;
        arr = new int[size];
    }

    void push(int val)
    {
        if (top == size - 1)
        {
            cout << "Stack Overflow" << endl;
            return;
        }
        cout << "pushed" << std::endl;
        arr[top + 1] = val;
        top++;
    }

    int pop()
    {
        if (top == -1)
        {
            cout << "Stack Underflow" << endl;
            return -1;
        }
        cout << "popped";
        int temp = arr[top];
        top--;
        return temp;
    }

    int peek()
    {
        if (top == -1)
        {
            cout << "Stack is Empty" << endl;
            return std::pow(2, 31) * -2;
        }
        return arr[top];
    }

    bool isEmpty()
    {
        cout << "check empty";
        if (top == -1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool isFull()
    {
        cout << "check full";
        if (top == size - 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    vector<int> display()
    {
        cout << "display";
        vector<int> data;
        for (int i = top; i >= 0; i--)
        {
            cout << arr[i] << " ";
            data.push_back(arr[i]);
        }
        cout << endl;
        return data;
    }
};

class Queue
{
    int front;
    int rear;
    int count;
    int size;
    int* arr = nullptr;

    public:
    Queue(int a)
    {
        cout << "Queue created" << endl;
        size = a;
        front = 0;
        rear = -1;
        count = 0;
        arr = new int[size];
    }

    ~Queue()
    {
        delete[] arr;
    }

    void push(int val)
    {
        if (count == size)
        {
            cout << "Queue Overflow" << endl;
            return;
        }
        rear = (rear + 1) % size;
        arr[rear] = val;
        count++;
        cout << "pushed" << endl;
    }

    int pop()
    {
        if (count == 0)
        {
            cout << "Queue Underflow" << endl;
            return -1;
        }
        int temp = arr[front];
        front = (front + 1) % size;
        count--;
        cout << "popped" << endl;
        return temp;
    }

    int peek()
    {
        if (count == 0)
        {
            cout << "Queue is Empty" << endl;
            return std::pow(2, 31) * -2;
        }
        return arr[front];
    }

    bool isEmpty()
    {
        cout << "check empty" << endl;
        return count == 0;
    }

    bool isFull()
    {
        cout << "check full" << endl;
        return count == size;
    }

    vector<int> display()
    {
        cout << "display" << endl;
        vector<int> data;
        for (int i = 0; i < count; i++)
        {
            int idx = (front + i) % size;
            cout << arr[idx] << " ";
            data.push_back(arr[idx]);
        }
        cout << endl;
        return data;
    }
};

class Node
{
    public:
    int val;
    Node* pre;
    Node* next;

    Node(int a, Node* b, Node* c)
    {
        val = a;
        pre = b;
        next = c;
    }
};

class Linked
{
    Node* dummyHead = nullptr;
    Node* dummyTail = nullptr;
    int maxSize = 0;
    int count = 0;

    public:
    Linked(int a)
    {
        cout << "Linked List created" << endl;
        maxSize = a;
        dummyHead = new Node(0, nullptr, nullptr);
        dummyTail = new Node(0, dummyHead, nullptr);
        dummyHead->next = dummyTail;
    }

    ~Linked()
    {
        Node* current = dummyHead;
        while (current)
        {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    void push(int val)
    {
        if (count == maxSize)
        {
            cout << "Linked List Overflow" << endl;
            return;
        }
        Node* node = new Node(val, dummyTail->pre, dummyTail);
        dummyTail->pre->next = node;
        dummyTail->pre = node;
        count++;
        cout << "pushed" << endl;
    }

    int pop()
    {
        if (count == 0)
        {
            cout << "Linked List Underflow" << endl;
            return -1;
        }
        Node* node = dummyTail->pre;
        int temp = node->val;
        node->pre->next = dummyTail;
        dummyTail->pre = node->pre;
        delete node;
        count--;
        cout << "popped" << endl;
        return temp;
    }

    int peek()
    {
        if (count == 0)
        {
            cout << "Linked List is Empty" << endl;
            return std::pow(2, 31) * -2;
        }
        return dummyTail->pre->val;
    }

    bool isEmpty()
    {
        cout << "check empty" << endl;
        return count == 0;
    }

    bool isFull()
    {
        cout << "check full" << endl;
        return count == maxSize;
    }

    vector<int> display()
    {
        cout << "display" << endl;
        vector<int> data;
        Node* current = dummyHead->next;
        while (current != dummyTail)
        {
            cout << current->val << " ";
            data.push_back(current->val);
            current = current->next;
        }
        cout << endl;
        return data;
    }
};

int currentDS = 0;
int maxSize = 0;
Stack* stackDS = nullptr;
Queue* queueDS = nullptr;
Linked* linkedDS = nullptr;


int main()
{
    //Queue queue;
    //Linked linked;
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Post("/:opr/:val/:size", [](const httplib::Request& req, httplib::Response& res){

        std::string opr = req.path_params.at("opr");
        int val = std::stoi(req.path_params.at("val"));
        int size = std::stoi(req.path_params.at("size"));

        std::cout << "Operation: " << opr << " Value: " << val << " Size: " << size << std::endl;

        json response;

        if (opr == "New")
        {
            currentDS = val;
            maxSize = size;

            // Clear all structures before creating a new one.
            if (stackDS)
            {
                delete stackDS;
                stackDS = nullptr;
            }
            if (queueDS)
            {
                delete queueDS;
                queueDS = nullptr;
            }
            if (linkedDS)
            {
                delete linkedDS;
                linkedDS = nullptr;
            }

            if (val == 1)
            {
                std::cout << "New Stack Created" << std::endl;
                response = {{"message", "New Stack Created"}};
                currentDS = 1;
                stackDS = new Stack(size);
            }
            else if (val == 2)
            {
                std::cout << "New Queue Created" << std::endl;
                response = {{"message", "New Queue Created"}};
                currentDS = 2;
                queueDS = new Queue(size);
            }
            else if (val == 3)
            {
                std::cout << "New Linked List Created" << std::endl;
                response = {{"message", "New Linked List Created"}};
                currentDS = 3;
                linkedDS = new Linked(size);
            }
            else
            {
                response = {{"message", "Invalid Value"}};
            }
        }
        else if (opr == "Push")
        {
            if (currentDS == 1 && stackDS)
            {
                std::cout << "pushing in stack " << val << std::endl;
                stackDS->push(val);
            }
            else if (currentDS == 2 && queueDS)
            {
                std::cout << "pushing in queue " << val << std::endl;
                queueDS->push(val);
            }
            else if (currentDS == 3 && linkedDS)
            {
                std::cout << "pushing in linked list " << val << std::endl;
                linkedDS->push(val);
            }
            else
            {
                response = {{"message", "No data structure selected"}};
                res.set_content(response.dump(), "application/json");
                return;
            }
            response = {{"message", "Pushed Successfully"}};
        }
        else if (opr == "Pop")
        {
            if (currentDS == 1 && stackDS)
            {
                std::cout << "popping from stack" << std::endl;
                stackDS->pop();
            }
            else if (currentDS == 2 && queueDS)
            {
                std::cout << "popping from queue" << std::endl;
                queueDS->pop();
            }
            else if (currentDS == 3 && linkedDS)
            {
                std::cout << "popping from linked list" << std::endl;
                linkedDS->pop();
            }
            else
            {
                response = {{"message", "No data structure selected"}};
                res.set_content(response.dump(), "application/json");
                return;
            }
            response = {{"message", "last element popped"}};
        }
        else if (opr == "Peek")
        {
            int a = 0;
            if (currentDS == 1 && stackDS)
            {
                if (!stackDS->isEmpty())
                {
                    a = stackDS->peek();
                    response = {{"message", a}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else if (currentDS == 2 && queueDS)
            {
                if (!queueDS->isEmpty())
                {
                    a = queueDS->peek();
                    response = {{"message", a}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else if (currentDS == 3 && linkedDS)
            {
                if (!linkedDS->isEmpty())
                {
                    a = linkedDS->peek();
                    response = {{"message", a}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else
            {
                response = {{"message", "No data structure selected"}};
            }
        }
        else if (opr == "Display")
        {
            std::vector<int> data;

            if (currentDS == 1 && stackDS)
            {
                if (!stackDS->isEmpty())
                {
                    data = stackDS->display();
                    response = {{"message", data}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else if (currentDS == 2 && queueDS)
            {
                if (!queueDS->isEmpty())
                {
                    data = queueDS->display();
                    response = {{"message", data}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else if (currentDS == 3 && linkedDS)
            {
                if (!linkedDS->isEmpty())
                {
                    data = linkedDS->display();
                    response = {{"message", data}};
                }
                else
                {
                    response = {{"message", "empty DS"}};
                }
            }
            else
            {
                response = {{"message", "No data structure selected"}};
            }
        }
        else if (opr == "Full")
        {
            bool state = false;

            if (currentDS == 1 && stackDS)
            {
                state = stackDS->isFull();
            }
            else if (currentDS == 2 && queueDS)
            {
                state = queueDS->isFull();
            }
            else if (currentDS == 3 && linkedDS)
            {
                state = linkedDS->isFull();
            }
            else
            {
                response = {{"message", "No data structure selected"}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            response = {{"message", state}};
        }
        else if (opr == "Empty")
        {
            bool state = false;

            if (currentDS == 1 && stackDS)
            {
                state = stackDS->isEmpty();
            }
            else if (currentDS == 2 && queueDS)
            {
                state = queueDS->isEmpty();
            }
            else if (currentDS == 3 && linkedDS)
            {
                state = linkedDS->isEmpty();
            }
            else
            {
                response = {{"message", "No data structure selected"}};
                res.set_content(response.dump(), "application/json");
                return;
            }

            response = {{"message", state}};
        }
        else
        {
            response = {{"message", "Invalid Operation"}};
        }

        res.set_content(response.dump(), "application/json");
    });

    std::cout << "Server listening on port 8080" << std::endl;
    server.listen("0.0.0.0", 8080);

    return 0;
}

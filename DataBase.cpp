#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include "httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// ==========================================
// DATA STRUCTURES (Using C++ STL Built-ins)
// ==========================================

class Stack {
    vector<int> data;
    int maxSize;
public:
    Stack(int size) : maxSize(size) { cout << "Stack created\n"; }

    string push(int val) {
        if (isFull()) return "Stack Overflow limit reached.";
        data.push_back(val);
        return "Successfully pushed " + std::to_string(val) + " onto Stack.";
    }
    
    string pop() {
        if (isEmpty()) return "Stack Underflow. Nothing to pop.";
        int v = data.back();
        data.pop_back();
        return "Successfully popped " + std::to_string(v) + " from Stack.";
    }
    
    int peek() { return isEmpty() ? -1 : data.back(); }
    bool isEmpty() { return data.empty(); }
    bool isFull() { return data.size() >= maxSize; }
    
    vector<int> display() {
        vector<int> res;
        // Stack displays from top to bottom
        for (auto it = data.rbegin(); it != data.rend(); ++it) {
            res.push_back(*it);
        }
        return res;
    }
};

class Queue {
    deque<int> data;
    int maxSize;
public:
    Queue(int size) : maxSize(size) { cout << "Queue created\n"; }

    string push(int val) {
        if (isFull()) return "Queue Overflow limit reached.";
        data.push_back(val);
        return "Successfully enqueued " + std::to_string(val) + " to Queue.";
    }
    
    string pop() {
        if (isEmpty()) return "Queue Underflow. Nothing to dequeue.";
        int v = data.front();
        data.pop_front();
        return "Successfully dequeued " + std::to_string(v) + " from Queue.";
    }
    
    int peek() { return isEmpty() ? -1 : data.front(); }
    bool isEmpty() { return data.empty(); }
    bool isFull() { return data.size() >= maxSize; }
    
    vector<int> display() { 
        return vector<int>(data.begin(), data.end()); 
    }
};

class Linked {
    list<int> data;
    int maxSize;
public:
    Linked(int size) : maxSize(size) { cout << "Linked List created\n"; }

    string push(int val) { 
        if (isFull()) return "List Overflow limit reached.";
        data.push_back(val); 
        return "Successfully inserted " + std::to_string(val) + " at End.";
    }
    string pushBegin(int val) { 
        if (isFull()) return "List Overflow limit reached.";
        data.push_front(val); 
        return "Successfully inserted " + std::to_string(val) + " at Beginning.";
    }
    
    string pop() { 
        if (isEmpty()) return "List Underflow. Nothing to delete.";
        int v = data.back();
        data.pop_back(); 
        return "Successfully deleted " + std::to_string(v) + " from End.";
    }
    string popBegin() { 
        if (isEmpty()) return "List Underflow. Nothing to delete.";
        int v = data.front();
        data.pop_front(); 
        return "Successfully deleted " + std::to_string(v) + " from Beginning.";
    }
    
    int peek() { return isEmpty() ? -1 : data.back(); }
    bool isEmpty() { return data.empty(); }
    bool isFull() { return data.size() >= maxSize; }
    
    vector<int> display() { 
        return vector<int>(data.begin(), data.end()); 
    }
};

// ==========================================
// SERVER STATE & APP LOGIC
// ==========================================

int currentDS = 0;
Stack* stackDS = nullptr;
Queue* queueDS = nullptr;
Linked* linkedDS = nullptr;

int main()
{
    httplib::Server server;

    // Default configuration for Web interaction
    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Handle Pre-flight fetch requests naturally sent by browsers
    server.Options("/api", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_content("", "text/plain");
    });

    // Main POST Route to handle all JSON incoming data
    server.Post("/api", [](const httplib::Request& req, httplib::Response& res) {
        json response;
        std::string opr;
        int val = 0, size = 0;
        std::string pos = "end";

        // Parse JSON sent by JS `fetch`
        try {
            json req_body = json::parse(req.body);
            if(req_body.contains("opr")) opr = req_body["opr"];
            if(req_body.contains("val")) val = req_body["val"];
            if(req_body.contains("size")) size = req_body["size"];
            if(req_body.contains("pos")) pos = req_body["pos"];
        } catch(...) {
            response = {{"message", "Invalid JSON body"}};
            res.set_content(response.dump(), "application/json");
            return;
        }

        std::cout << "Operation: " << opr << " | Value: " << val << " | Size: " << size << " | Pos: " << pos << std::endl;

        if (opr == "New") {
            currentDS = val;

            if (stackDS) { delete stackDS; stackDS = nullptr; }
            if (queueDS) { delete queueDS; queueDS = nullptr; }
            if (linkedDS) { delete linkedDS; linkedDS = nullptr; }

            if (val == 1) {
                stackDS = new Stack(size);
                response = {{"message", "New Stack Created"}};
            } else if (val == 2) {
                queueDS = new Queue(size);
                response = {{"message", "New Queue Created"}};
            } else if (val == 3) {
                linkedDS = new Linked(size);
                response = {{"message", "New Linked List Created"}};
            } else {
                response = {{"message", "Invalid Structure ID"}};
            }
        }
        else if (opr == "Push") {
            std::string msg = "Error";
            if (currentDS == 1 && stackDS) msg = stackDS->push(val);
            else if (currentDS == 2 && queueDS) msg = queueDS->push(val);
            else if (currentDS == 3 && linkedDS) {
                if (pos == "beginning") msg = linkedDS->pushBegin(val);
                else msg = linkedDS->push(val);
            }
            response = {{"message", msg}};
        }
        else if (opr == "Pop") {
            std::string msg = "Error";
            if (currentDS == 1 && stackDS) msg = stackDS->pop();
            else if (currentDS == 2 && queueDS) msg = queueDS->pop();
            else if (currentDS == 3 && linkedDS) {
                if (pos == "beginning") msg = linkedDS->popBegin();
                else msg = linkedDS->pop();
            }
            response = {{"message", msg}};
        }
        else if (opr == "Peek") {
            int a = 0;
            if (currentDS == 1 && stackDS && !stackDS->isEmpty()) a = stackDS->peek();
            else if (currentDS == 2 && queueDS && !queueDS->isEmpty()) a = queueDS->peek();
            else if (currentDS == 3 && linkedDS && !linkedDS->isEmpty()) a = linkedDS->peek();
            response = {{"message", a}};
        }
        else if (opr == "Display") {
            std::vector<int> data;
            if (currentDS == 1 && stackDS && !stackDS->isEmpty()) data = stackDS->display();
            else if (currentDS == 2 && queueDS && !queueDS->isEmpty()) data = queueDS->display();
            else if (currentDS == 3 && linkedDS && !linkedDS->isEmpty()) data = linkedDS->display();
            
            if (data.empty()) response = {{"message", "empty DS"}};
            else response = {{"message", data}};
        }
        else if (opr == "Full") {
            bool state = false;
            if (currentDS == 1 && stackDS) state = stackDS->isFull();
            else if (currentDS == 2 && queueDS) state = queueDS->isFull();
            else if (currentDS == 3 && linkedDS) state = linkedDS->isFull();
            response = {{"message", state}};
        }
        else if (opr == "Empty") {
            bool state = false;
            if (currentDS == 1 && stackDS) state = stackDS->isEmpty();
            else if (currentDS == 2 && queueDS) state = queueDS->isEmpty();
            else if (currentDS == 3 && linkedDS) state = linkedDS->isEmpty();
            response = {{"message", state}};
        }
        else {
            response = {{"message", "Invalid Operation"}};
        }

        res.set_content(response.dump(), "application/json");
    });

    std::cout << "Server listening on port 8080..." << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}

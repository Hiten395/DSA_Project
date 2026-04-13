#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include "httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Global data structures
std::stack<int> st;
std::queue<int> qu;
std::vector<int> ll;

int currentDS = 0; // 1 = Stack, 2 = Queue, 3 = Linked List
int maxSize = 0;

// Helper functions
bool isFull() {
    if (currentDS == 1) return st.size() >= maxSize;
    if (currentDS == 2) return qu.size() >= maxSize;
    if (currentDS == 3) return ll.size() >= maxSize;
    return false;
}

bool isEmpty() {
    if (currentDS == 1) return st.empty();
    if (currentDS == 2) return qu.empty();
    if (currentDS == 3) return ll.empty();
    return true;
}

int main()
{
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

            // Clear all structures
            while (!st.empty()) st.pop();
            while (!qu.empty()) qu.pop();
            ll.clear();

            if (val == 1)
                response = {{"message", "New Stack Created"}};
            else if (val == 2)
                response = {{"message", "New Queue Created"}};
            else if (val == 3)
                response = {{"message", "New Linked List Created"}};
            else
                response = {{"message", "Invalid Value"}};
        }
        else if (opr == "Push")
        {
            if (isFull()) {
                response = {{"message", "Structure is Full"}};
            } else {
                if (currentDS == 1) {
                    st.push(val);
                } else if (currentDS == 2) {
                    qu.push(val);
                } else if (currentDS == 3) {
                    ll.push_back(val);
                }
                response = {{"message", "Pushed Successfully"}};
            }
        }
        else if (opr == "Pop")
        {
            if (isEmpty()) {
                response = {{"message", "Structure is Empty"}};
            } else {
                int a;
                if (currentDS == 1) {
                    a = st.top();
                    st.pop();
                } else if (currentDS == 2) {
                    a = qu.front();
                    qu.pop();
                } else if (currentDS == 3) {
                    a = ll.back();
                    ll.pop_back();
                }
                response = {{"message", a}};
            }
        }
        else if (opr == "Peek")
        {
            if (isEmpty()) {
                response = {{"message", "Structure is Empty"}};
            } else {
                int a;
                if (currentDS == 1) {
                    a = st.top();
                } else if (currentDS == 2) {
                    a = qu.front();
                } else if (currentDS == 3) {
                    a = ll.back();
                }
                response = {{"message", a}};
            }
        }
        else if (opr == "Display")
        {
            std::vector<int> data;

            if (currentDS == 1) {
                std::stack<int> temp = st;
                while (!temp.empty()) {
                    data.push_back(temp.top());
                    temp.pop();
                }
            } 
            else if (currentDS == 2) {
                std::queue<int> temp = qu;
                while (!temp.empty()) {
                    data.push_back(temp.front());
                    temp.pop();
                }
            } 
            else if (currentDS == 3) {
                data = ll;
            }

            response = {{"message", data}};
        }
        else if (opr == "Full")
        {
            response = {{"message", isFull()}};
        }
        else if (opr == "Empty")
        {
            response = {{"message", isEmpty()}};
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

#include <iostream>
#include <string>
#include "httplib.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// all user functions


int main()
{
    // DO NOT CHANGE
    httplib::Server server;

    // Add CORS headers to ALL responses DO NOT CHANGE
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
        // values from the client are stored in the above variables and printed to console

        json response;
        if (opr == "New")
        {
            // user has to choose the type of data structure of size 'size'
            if (val == 1)
            {
                // user has chosen to make a new Stack
                response = {{"message", "To New Stack"}};
            }
            else if (val == 2)
            {
                // user has chosen to make a new Queue
                response = {{"message", "To New Queue"}};
            }
            else if (val == 3)
            {
                // user has chosen to make a new Linked List
                response = {{"message", "To New Linked List"}};
            }
            else
            {
                response = {{"message", "Invalid Value"}};
            }
        }
        else if (opr == "Push")
        {
            // to push to whatever datastructure exists
            response = {{"message", "To Push"}};

            int a;
            // store the value in a returned by push of whatever datastructure exists and uncomment next line
            // response = {{"message", a}};
        }
        else if (opr == "Pop")
        {
            // to pop from whatever datastructure exists
            response = {{"message", "To Pop"}};

            int a;
            // store the value in a returned by pop of whatever datastructure exists and uncomment next line
            // response = {{"message", a}};
        }
        else if (opr == "Peek")
        {
            // to peek from whatever datastructure exists
            response = {{"message", "To Peek"}};

            int a;
            // store the value in a returned by peek of whatever datastructure exists and uncomment next line
            // response = {{"message", a}};
        }
        else if (opr == "Display")
        {
            // to display whatever datastructure exists
            response = {{"message", "To Display"}};

            std::vector<int> data;
            // store the value of whateber database in data and uncomment next line
            // response = {{"message", data}};
        }
        else if (opr == "Full")
        {
            // to check if the datastructure is full
            response = {{"message", "To Full"}};
            
            bool isFull;
            // store the value of full in isFull and uncomment next line
            //response = {{"message", isFull}}
        }
        else if (opr == "Empty")
        {
            // to check if the datastructure is empty
            response = {{"message", "To Empty"}};

            bool isEmpty;
            // store the value of empty in isEmpty and uncomment next line
            //response = {{"message", isEmpty}}
        }
        else
        {
            response = {{"message", "Invalid Operation"}};
        }

        res.set_content(response.dump(), "application/json");

    });


    // Do not change anything below this line
    
    std::cout << "Server listening on port 8080" << std::endl;
    server.listen("0.0.0.0", 8080);

    return 0;
}
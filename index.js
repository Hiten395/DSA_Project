async function New(val, size) {
    var response = await fetch("http://localhost:8080/New/" + val + "/" + size, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function Push(val) {
    var response = await fetch("http://localhost:8080/Push/" + val + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function Pop() {
    var response = await fetch("http://localhost:8080/Pop/" + 0 + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function Peek() {
    var response = await fetch("http://localhost:8080/Peek/" + 0 + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function IsEmpty() {
    var response = await fetch("http://localhost:8080/Empty/" + 0 + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function IsFull() {
    var response = await fetch("http://localhost:8080/Full/" + 0 + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}

async function Display() {
    var response = await fetch("http://localhost:8080/Display/" + 0 + "/" + 0, {
        method: "POST"
    });
    var data = await response.json();
    return data.message;
}


// The Above function return everything required for communication between frontend and backend 

// Use the test function to get familiar with the functions



async function Test() {
    console.log("Test");
    a = await New(1, 5);
    console.log(a);
    a = await Push(1);
    console.log(a);
    a = await Pop();
    console.log(a);
    a = await IsEmpty();
    console.log(a);
    a = await IsFull();
    console.log(a);
    a = await Display();
    console.log(a);
}
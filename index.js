// ─── API Communication Functions ───────────────────────────────────────────
// These send requests to the C++ backend on port 8080.

const BASE_URL = "http://localhost:8080";

async function requestBackend(opr, val = 0, size = 0) {
    const response = await fetch(`${BASE_URL}/${opr}/${val}/${size}`, {
        method: "POST"
    });
    const data = await response.json();
    return data.message;
}

async function New(val, size) {
    return requestBackend("New", val, size);
}

async function Push(val) {
    return requestBackend("Push", val, 0);
}

async function Pop() {
    return requestBackend("Pop", 0, 0);
}

async function Peek() {
    return requestBackend("Peek", 0, 0);
}

async function IsEmpty() {
    return requestBackend("Empty", 0, 0);
}

async function IsFull() {
    return requestBackend("Full", 0, 0);
}

async function Display() {
    return requestBackend("Display", 0, 0);
}


// ─── DOM Helpers ───────────────────────────────────────────────────────────

/** Shortcut to write text into the #output div */
function setOutput(text) {
    document.getElementById("output").textContent = text;
}

/** Returns the CSS class for the current DS type (stack / queue / linked) */
function getVisualClass(dsType) {
    if (dsType === "1") return "stack";
    if (dsType === "2") return "queue";
    if (dsType === "3") return "linked";
    return "stack";
}

/** Fetches Display data from the backend and renders boxes in #visual */
async function refreshVisual() {
    var visual = document.getElementById("visual");
    var data = await Display();

    // Clear old boxes
    visual.innerHTML = "";

    // data can be an array of ints, a string, or a single value
    if (Array.isArray(data)) {
        data.forEach(function (item) {
            var box = document.createElement("div");
            box.className = "box";
            box.textContent = item;
            visual.appendChild(box);
        });
    }
}


// ─── Button Handler Functions (called from index.html onclick) ─────────────

// Track current DS type so we can style the visual area correctly
var currentDSType = "1";

/** Called by the "Create" button */
async function createDS() {
    var dsSelect = document.getElementById("ds");
    var sizeInput = document.getElementById("size");

    var dsValue = dsSelect.value;       // 1, 2, or 3
    var sizeValue = sizeInput.value;

    if (!sizeValue || sizeValue <= 0) {
        setOutput("Please enter a valid size.");
        return;
    }

    currentDSType = dsValue;

    try {
        var result = await New(parseInt(dsValue), parseInt(sizeValue));
        setOutput(result);

        // Update visualization class to match DS type
        var visual = document.getElementById("visual");
        visual.className = "visual " + getVisualClass(dsValue);
        visual.innerHTML = ""; // clear preview boxes
        await refreshVisual();
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Push" button */
async function handlePush() {
    var valueInput = document.getElementById("value");
    var val = valueInput.value;

    if (val === "" || val === null) {
        setOutput("Please enter a value to push.");
        return;
    }

    try {
        var result = await Push(parseInt(val));
        setOutput(result);
        await refreshVisual();
        valueInput.value = ""; // clear input after successful push
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Pop" button */
async function handlePop() {
    try {
        var result = await Pop();
        setOutput("Popped: " + result);
        await refreshVisual();
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Peek" button */
async function handlePeek() {
    try {
        var result = await Peek();
        setOutput("Top/Front: " + result);
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Display" button */
async function handleDisplay() {
    try {
        var result = await Display();

        if (Array.isArray(result)) {
            setOutput("Elements: " + result.join(", "));
        } else {
            setOutput(result);
        }

        await refreshVisual();
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Is Empty" button */
async function handleEmpty() {
    try {
        var result = await IsEmpty();
        if (typeof result === "boolean") {
            setOutput(result ? "Yes, it is empty." : "No, it is not empty.");
        } else {
            setOutput(result);
        }
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}

/** Called by the "Is Full" button */
async function handleFull() {
    try {
        var result = await IsFull();
        if (typeof result === "boolean") {
            setOutput(result ? "Yes, it is full." : "No, it is not full.");
        } else {
            setOutput(result);
        }
    } catch (err) {
        setOutput("Error: Could not connect to server.");
        console.error(err);
    }
}
const CppSnippets = {
    LinkedList: {
        base: `class LinkedList {\n  ListNode* head;\n  int maxSize;\n  int currentSize;\npublic:\n  LinkedList(int size) {\n    head = nullptr;\n    maxSize = size;\n    currentSize = 0;\n  }\n};`,
        insert: `void insertAtBeginning(int val) {\n  if (isFull()) return;\n  ListNode* newNode = new ListNode(val);\n  newNode->next = head;\n  head = newNode;\n  currentSize++;\n}\n\nvoid insertAtEnd(int val) {\n  if (isFull()) return;\n  ListNode* newNode = new ListNode(val);\n  currentSize++;\n  if (!head) { head = newNode; return; }\n  ListNode* temp = head;\n  while (temp->next) temp = temp->next;\n  temp->next = newNode;\n}`,
        delete: `void deleteFromBeginning() {\n  if (!head) return;\n  ListNode* temp = head;\n  head = head->next;\n  delete temp;\n  currentSize--;\n}\n\nvoid deleteFromEnd() {\n  if (!head) return;\n  currentSize--;\n  if (!head->next) { delete head; head = nullptr; return; }\n  ListNode* temp = head;\n  while (temp->next->next) temp = temp->next;\n  delete temp->next;\n  temp->next = nullptr;\n}`,
        display: `void display() {\n  ListNode* temp = head;\n  while (temp) {\n    cout << temp->val << " -> ";\n    temp = temp->next;\n  }\n  cout << "NULL" << endl;\n}`,
        isempty: `bool isEmpty() {\n  return head == nullptr;\n}`,
        isfull: `bool isFull() {\n  return currentSize >= maxSize;\n}`
    },
    Stack: {
        base: `class Stack {\n  int* arr;\n  int topIndex;\n  int maxSize;\npublic:\n  Stack(int size) {\n    maxSize = size;\n    arr = new int[maxSize];\n    topIndex = -1;\n  }\n  ~Stack() {\n    delete[] arr;\n  }\n};`,
        push: `void push(int val) {\n  if (isFull()) {\n    cout << "Stack Overflow\\n";\n    return;\n  }\n  arr[++topIndex] = val;\n}`,
        pop: `void pop() {\n  if (isEmpty()) {\n    cout << "Stack Underflow\\n";\n    return;\n  }\n  topIndex--;\n}`,
        peek: `int peek() {\n  if (isEmpty()) {\n    cout << "Stack is empty\\n";\n    return -1;\n  }\n  return arr[topIndex];\n}`,
        display: `void display() {\n  for (int i = topIndex; i >= 0; i--) {\n    cout << arr[i] << "\\n";\n  }\n}`,
        isempty: `bool isEmpty() {\n  return topIndex == -1;\n}`,
        isfull: `bool isFull() {\n  return topIndex == maxSize - 1;\n}`
    },
    Queue: {
        base: `class Queue {\n  int* arr;\n  int front, rear, maxSize;\npublic:\n  Queue(int size) {\n    maxSize = size;\n    arr = new int[maxSize];\n    front = -1;\n    rear = -1;\n  }\n  ~Queue() {\n    delete[] arr;\n  }\n};`,
        enqueue: `void enqueue(int val) {\n  if (isFull()) {\n    cout << "Queue is Full\\n";\n    return;\n  }\n  if (front == -1) front = 0;\n  arr[++rear] = val;\n}`,
        dequeue: `void dequeue() {\n  if (isEmpty()) {\n    cout << "Queue is Empty\\n";\n    return;\n  }\n  for (int i = 0; i < rear; i++) {\n    arr[i] = arr[i+1];\n  }\n  rear--;\n  if (rear == -1) front = -1;\n}`,
        peek: `int peek() {\n  if (isEmpty()) {\n    cout << "Queue is empty\\n";\n    return -1;\n  }\n  return arr[front];\n}`,
        display: `void display() {\n  if (isEmpty()) return;\n  for (int i = front; i <= rear; i++) {\n    cout << arr[i] << " ";\n  }\n  cout << "\\n";\n}`,
        isempty: `bool isEmpty() {\n  return front == -1 || front > rear;\n}`,
        isfull: `bool isFull() {\n  return rear == maxSize - 1;\n}`
    }
};

const app = {
    currentDS: null,
    data: [],
    maxSize: null,

    sendApiRequest: async function(opr, val = 0, size = 0, pos = "end") {
        try {
            const resp = await fetch('http://localhost:8080/api', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ opr, val, size, pos })
            });
            return await resp.json();
        } catch (e) {
            console.error(e);
            return { message: "Backend Error" };
        }
    },

    loadDS: function(dsName) {
        this.currentDS = dsName;
        this.data = [];
        this.maxSize = null;
        
        document.getElementById('home-view').classList.remove('active');
        document.getElementById('dashboard-view').classList.add('active');
        
        document.getElementById('ds-title').innerText = `${dsName} Visualization`;
        document.getElementById('ds-subtitle').innerText = `Visualize operations on a ${dsName.toLowerCase()} data structure`;
        
        this.setCodeSnippet('base');
        this.renderProperties();
        this.renderSizeSetup();
        this.renderVisualization();
        document.getElementById('log-message').innerText = "";
    },

    renderSizeSetup: function() {
        document.getElementById('ops-tabs').innerHTML = '';
        const opContent = document.getElementById('ops-content');
        opContent.innerHTML = `
            <div style="margin-bottom: 15px; font-size: 0.95rem; color: var(--text-secondary);">
                Please specify the maximum capacity for this data structure.
            </div>
            <div class="form-group">
                <label>Max Size Capacity</label>
                <input type="number" id="inp-size-setup" class="form-control" placeholder="E.g. 5, 10, 100...">
            </div>
            <button class="btn-submit" onclick="app.submitSizeSetup()">Confirm Size</button>
        `;
    },

    submitSizeSetup: async function() {
        const val = parseInt(document.getElementById('inp-size-setup').value);
        if (isNaN(val) || val <= 0) {
            this.log('Invalid size. Enter a positive number.', true);
            return;
        }
        this.maxSize = val;
        
        let dsCode = 1;
        if(this.currentDS === 'Queue') dsCode = 2;
        if(this.currentDS === 'LinkedList') dsCode = 3;
        
        await this.sendApiRequest("New", dsCode, val);

        this.log(`Size set to ${val}. You can now perform operations.`);
        this.renderTabs();
    },

    goHome: function() {
        document.getElementById('dashboard-view').classList.remove('active');
        document.getElementById('home-view').classList.add('active');
    },

    setCodeSnippet: function(op) {
        const codeBox = document.getElementById('code-snippet-box');
        if(CppSnippets[this.currentDS] && CppSnippets[this.currentDS][op]) {
            codeBox.innerText = CppSnippets[this.currentDS][op];
        } else {
            codeBox.innerText = `// Implementation available soon...`;
        }
    },

    log: function(msg, isError=false) {
        const el = document.getElementById('log-message');
        el.innerText = `> ${msg}`;
        el.className = 'ops-log ' + (isError ? 'color-error' : 'color-success');
    },

    renderProperties: function() {
        const propContainer = document.getElementById('ds-properties');
        let html = '';
        if(this.currentDS === 'LinkedList') {
            html = `<div class="props-group"><div class="props-title">Structure</div><div class="props-desc">Nodes containing data and a memory reference to the next node.</div></div>
                    <div class="props-group"><div class="props-title">Time Complexity</div><div class="props-desc">Search: O(n)<br>Insertion/Deletion: O(1) (at head/tail if tracked)</div></div>`;
        } else if(this.currentDS === 'Stack') {
            html = `<div class="props-group"><div class="props-title">Structure</div><div class="props-desc">A sequential collection mapped to a Last-In-First-Out (LIFO) model.</div></div>
                    <div class="props-group"><div class="props-title">Time Complexity</div><div class="props-desc">Push: O(1)<br>Pop: O(1)<br>Peek: O(1)</div></div>`;
        } else if(this.currentDS === 'Queue') {
            html = `<div class="props-group"><div class="props-title">Structure</div><div class="props-desc">A linear arrangement following a First-In-First-Out (FIFO) methodology.</div></div>
                    <div class="props-group"><div class="props-title">Time Complexity</div><div class="props-desc">Enqueue: O(1)<br>Dequeue: O(1)<br>Front: O(1)</div></div>`;
        }
        propContainer.innerHTML = html;
    },

    renderTabs: function() {
        const tabContainer = document.getElementById('ops-tabs');
        let tabs = [];
        if(this.currentDS === 'LinkedList') tabs = ['Insert', 'Delete', 'Display', 'IsEmpty', 'IsFull'];
        else if(this.currentDS === 'Stack') tabs = ['Push', 'Pop', 'Peek', 'Display', 'IsEmpty', 'IsFull'];
        else if(this.currentDS === 'Queue') tabs = ['Enqueue', 'Dequeue', 'Peek', 'Display', 'IsEmpty', 'IsFull'];

        let html = '';
        tabs.forEach((t, i) => {
            html += `<button class="tab-btn ${i===0 ? 'active' : ''}" onclick="app.selectTab('${t}', this)">${t}</button>`;
        });
        tabContainer.innerHTML = html;
        if(tabs.length > 0) this.selectTab(tabs[0], tabContainer.firstChild);
    },

    selectTab: function(tabName, btnEl) {
        const btns = document.getElementById('ops-tabs').querySelectorAll('.tab-btn');
        btns.forEach(b => b.classList.remove('active'));
        if(btnEl) btnEl.classList.add('active');

        this.setCodeSnippet(tabName.toLowerCase());

        const opContent = document.getElementById('ops-content');
        let html = '';

        if(tabName === 'Insert') {
            html = `
                <div class="form-group"><label>Value</label><input type="number" id="inp-val" class="form-control" placeholder="Enter a number"></div>
                <div class="form-group"><label>Insert at</label><select id="inp-pos" class="form-control"><option value="end">End</option><option value="beginning">Beginning</option></select></div>
                <button class="btn-submit" onclick="app.executeOp('insert')">+ Insert Node</button>
            `;
        } else if(tabName === 'Delete') {
            html = `
                <div class="form-group"><label>Delete from</label><select id="inp-pos" class="form-control"><option value="beginning">Beginning</option><option value="end">End</option></select></div>
                <button class="btn-submit" onclick="app.executeOp('delete')">- Delete Node</button>
            `;
        } else if(tabName === 'Push' || tabName === 'Enqueue') {
            html = `<div class="form-group"><label>Value</label><input type="number" id="inp-val" class="form-control" placeholder="Enter a number"></div><button class="btn-submit" onclick="app.executeOp('add')">+ Add Value</button>`;
        } else if(tabName === 'Pop' || tabName === 'Dequeue') {
            html = `<button class="btn-submit" style="margin-top:20px;" onclick="app.executeOp('remove')">- Remove Element</button>`;
        } else if(tabName === 'Peek') {
            html = `<button class="btn-submit" style="margin-top:20px;" onclick="app.executeOp('peek')">Peek Top Element</button>`;
        } else if(tabName === 'Display') {
            html = `<button class="btn-submit" style="margin-top:20px;" onclick="app.executeOp('display')">Trigger Display</button>`;
        } else if(tabName === 'IsEmpty') {
            html = `<button class="btn-submit" style="margin-top:20px;" onclick="app.executeOp('isempty')">Verify If Empty</button>`;
        } else if(tabName === 'IsFull') {
            html = `<button class="btn-submit" style="margin-top:20px;" onclick="app.executeOp('isfull')">Verify If Full</button>`;
        }

        opContent.innerHTML = html;
        document.getElementById('log-message').innerText = "";
    },

    executeOp: async function(action) {
        if(action === 'display') {
            const res = await this.sendApiRequest("Display");
            if (Array.isArray(res.message)) {
                this.data = res.message;
                this.log(this.data.length > 0 ? `System displayed ${this.data.length} element(s) linearly in memory.` : `System is empty. Nothing to display.`);
            } else {
                this.log(`System is empty. Nothing to display.`);
            }
            this.renderVisualization();
            return;
        } else if(action === 'isempty') {
            const res = await this.sendApiRequest("Empty");
            if(res.message === true) this.log(`True, the data structure is fully empty.`);
            else this.log(`False, the data structure contains items.`, true);
            return;
        } else if(action === 'isfull') {
            const res = await this.sendApiRequest("Full");
            if (res.message === true) {
                this.log(`True, the data structure is full. (Max Size: ${this.maxSize})`);
            } else {
                this.log(`False, current space used: ${this.data.length}/${this.maxSize}.`, false);
            }
            return;
        }

        if(this.currentDS === 'LinkedList') await this.llOps(action);
        else if(this.currentDS === 'Stack') await this.stackOps(action);
        else if(this.currentDS === 'Queue') await this.queueOps(action);
        
        const displayRes = await this.sendApiRequest("Display");
        if(Array.isArray(displayRes.message)) this.data = displayRes.message;
        else this.data = [];
        this.renderVisualization();
    },

    llOps: async function(action) {
        if(action === 'insert') {
            const val = parseInt(document.getElementById('inp-val').value);
            if(isNaN(val)) return this.log("Value cannot be blank", true);
            const pos = document.getElementById('inp-pos').value;
            
            const res = await this.sendApiRequest("Push", val, 0, pos);
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        } else if(action === 'delete') {
            const pos = document.getElementById('inp-pos').value;
            
            const res = await this.sendApiRequest("Pop", 0, 0, pos);
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        }
    },

    stackOps: async function(action) {
        if(action === 'add') {
            const val = parseInt(document.getElementById('inp-val').value);
            if(isNaN(val)) return this.log("Value cannot be blank", true);
            const res = await this.sendApiRequest("Push", val);
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        } else if(action === 'remove') {
            const res = await this.sendApiRequest("Pop");
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        } else if(action === 'peek') {
            const res = await this.sendApiRequest("Peek");
            if (res.message === -1) this.log("Stack is currently empty.", true);
            else this.log(`The top element is ${res.message}.`);
        }
    },

    queueOps: async function(action) {
        if(action === 'add') {
            const val = parseInt(document.getElementById('inp-val').value);
            if(isNaN(val)) return this.log("Value cannot be blank", true);
            const res = await this.sendApiRequest("Push", val);
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        } else if(action === 'remove') {
            const res = await this.sendApiRequest("Pop");
            this.log(res.message, res.message.includes("limit") || res.message.includes("Nothing"));
        } else if(action === 'peek') {
            const res = await this.sendApiRequest("Peek");
            if (res.message === -1) this.log("Queue is currently empty.", true);
            else this.log(`The front element is ${res.message}.`);
        }
    },

    renderVisualization: function() {
        const vis = document.getElementById('vis-canvas');
        vis.innerHTML = '';
        if(this.maxSize === null) {
            vis.innerHTML = '<span style="color:var(--text-muted)">Please confirm the size capacity first.</span>';
            return;
        }
        if(this.data.length === 0) {
            vis.innerHTML = '<span style="color:var(--text-muted)">Begin by inserting a value.</span>';
            return;
        }

        if(this.currentDS === 'LinkedList' || this.currentDS === 'Queue') {
            vis.style.flexDirection = 'row';
            this.data.forEach((val, i) => {
                const node = document.createElement('div');
                node.className = 'ds-node';
                node.innerText = val;
                vis.appendChild(node);

                if(i < this.data.length - 1) {
                    const arrow = document.createElement('div');
                    arrow.className = 'll-arrow';
                    arrow.innerHTML = '&#8250;'; 
                    vis.appendChild(arrow);
                }
            });
        } else if (this.currentDS === 'Stack') {
            vis.style.flexDirection = 'column-reverse'; 
            
            for(let i = this.data.length - 1; i >= 0; i--) {
                const node = document.createElement('div');
                node.className = 'ds-node stack-node';
                if(i === this.data.length - 1) node.style.borderColor = '#d8b4fe';
                node.innerText = this.data[i];
                vis.appendChild(node);
            }
        }
    }
};
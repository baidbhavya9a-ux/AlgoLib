#ifndef ALGOLIB_DATA_STRUCTURES_HPP
#define ALGOLIB_DATA_STRUCTURES_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <cstddef>

namespace algolib {

// ==================================================
// 1. DYNAMIC ARRAY
// ==================================================
// Custom dynamically resizable array implemented without STL containers.
// Uses manual memory allocation (new[]/delete[]) with 2x amortized growth.

class DynamicArray {
private:
    int* data;
    std::size_t len;
    std::size_t cap;

    void grow() {
        std::size_t newCap = (cap == 0) ? 2 : cap * 2;
        int* temp = new int[newCap];

        for (std::size_t i = 0; i < len; ++i) {
            temp[i] = data[i];
        }

        delete[] data;
        data = temp;
        cap = newCap;
    }

public:
    DynamicArray() : data(nullptr), len(0), cap(0) {}

    // Rule of Three
    DynamicArray(const DynamicArray& other) : data(nullptr), len(other.len), cap(other.cap) {
        if (cap > 0) {
            data = new int[cap];
            for (std::size_t i = 0; i < len; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            cap = other.cap;
            data = nullptr;
            if (cap > 0) {
                data = new int[cap];
                for (std::size_t i = 0; i < len; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data;
        data = nullptr;
        len = 0;
        cap = 0;
    }

    std::size_t size() const {
        return len;
    }

    std::size_t capacity() const {
        return cap;
    }

    bool empty() const {
        return len == 0;
    }

    int at(std::size_t index) const {
        if (index >= len)
            throw std::out_of_range("Invalid array index: out of bounds");
        return data[index];
    }

    void insert(int value) {
        if (len == cap)
            grow();
        data[len++] = value;
    }

    void insertAt(std::size_t index, int value) {
        if (index > len)
            throw std::out_of_range("Invalid insert index: out of bounds");
        if (len == cap)
            grow();
        for (std::size_t i = len; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value;
        ++len;
    }

    void remove() {
        if (len == 0)
            throw std::runtime_error("Dynamic array is empty: cannot remove");
        --len;
    }

    void removeAt(std::size_t index) {
        if (index >= len)
            throw std::out_of_range("Invalid remove index: out of bounds");
        for (std::size_t i = index; i + 1 < len; ++i) {
            data[i] = data[i + 1];
        }
        --len;
    }

    bool removeValue(int value) {
        for (std::size_t i = 0; i < len; ++i) {
            if (data[i] == value) {
                removeAt(i);
                return true;
            }
        }
        return false;
    }

    bool search(int value) const {
        for (std::size_t i = 0; i < len; ++i) {
            if (data[i] == value)
                return true;
        }
        return false;
    }

    int indexOf(int value) const {
        for (std::size_t i = 0; i < len; ++i) {
            if (data[i] == value)
                return static_cast<int>(i);
        }
        return -1;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        len = 0;
        cap = 0;
    }

    std::string display() const {
        std::ostringstream out;
        out << "[ ";
        for (std::size_t i = 0; i < len; ++i)
            out << data[i] << " ";
        out << "]\nSize: " << len << " | Capacity: " << cap;
        return out.str();
    }
};


// ==================================================
// 2. SINGLY LINKED LIST
// ==================================================
// Custom singly linked list using dynamically allocated nodes.
// Supports insertion, deletion by value or position, search, and iteration.

class LinkedList {
private:
    struct Node {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };

    Node* head = nullptr;

public:
    LinkedList() = default;

    LinkedList(const LinkedList& other) {
        Node* curr = other.head;
        Node** tail = &head;
        while (curr) {
            *tail = new Node(curr->data);
            tail = &((*tail)->next);
            curr = curr->next;
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* curr = other.head;
            Node** tail = &head;
            while (curr) {
                *tail = new Node(curr->data);
                tail = &((*tail)->next);
                curr = curr->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        clear();
    }

    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool empty() const {
        return head == nullptr;
    }

    std::size_t size() const {
        std::size_t count = 0;
        Node* current = head;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }

    int at(std::size_t index) const {
        Node* current = head;
        while (current && index > 0) {
            current = current->next;
            --index;
        }
        if (!current)
            throw std::out_of_range("Invalid linked list index: out of bounds");
        return current->data;
    }

    void insert(int value) {
        Node* node = new Node(value);
        if (!head) {
            head = node;
            return;
        }
        Node* current = head;
        while (current->next)
            current = current->next;
        current->next = node;
    }

    void insertHead(int value) {
        Node* node = new Node(value);
        node->next = head;
        head = node;
    }

    bool remove(int value) {
        Node* current = head;
        Node* previous = nullptr;

        while (current && current->data != value) {
            previous = current;
            current = current->next;
        }

        if (!current)
            return false;

        if (previous)
            previous->next = current->next;
        else
            head = current->next;

        delete current;
        return true;
    }

    bool search(int value) const {
        Node* current = head;
        while (current) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    std::string display() const {
        std::ostringstream out;
        out << "HEAD -> ";
        Node* current = head;
        while (current) {
            out << current->data << " -> ";
            current = current->next;
        }
        out << "NULL\nTotal Nodes: " << size();
        return out.str();
    }
};


// ==================================================
// 3. STACK (LIFO)
// ==================================================
// Custom LIFO Stack implemented as a singly linked list with O(1) push and pop.

class Stack {
private:
    struct Node {
        int data;
        Node* next;
        Node(int value, Node* pointer) : data(value), next(pointer) {}
    };

    Node* topNode = nullptr;

public:
    Stack() = default;

    Stack(const Stack& other) {
        // Deep copy: reverse then copy to preserve order
        if (!other.topNode) return;
        // Count elements
        int count = other.size();
        int* tempArr = new int[count];
        Node* cur = other.topNode;
        for (int i = 0; i < count; ++i) {
            tempArr[i] = cur->data;
            cur = cur->next;
        }
        for (int i = count - 1; i >= 0; --i) {
            push(tempArr[i]);
        }
        delete[] tempArr;
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            if (other.topNode) {
                int count = other.size();
                int* tempArr = new int[count];
                Node* cur = other.topNode;
                for (int i = 0; i < count; ++i) {
                    tempArr[i] = cur->data;
                    cur = cur->next;
                }
                for (int i = count - 1; i >= 0; --i) {
                    push(tempArr[i]);
                }
                delete[] tempArr;
            }
        }
        return *this;
    }

    ~Stack() {
        clear();
    }

    void clear() {
        while (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    int size() const {
        int count = 0;
        Node* current = topNode;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }

    int at(int index) const {
        Node* current = topNode;
        while (current && index > 0) {
            current = current->next;
            --index;
        }
        if (!current)
            throw std::out_of_range("Invalid stack index: out of bounds");
        return current->data;
    }

    void push(int value) {
        topNode = new Node(value, topNode);
    }

    int pop() {
        if (!topNode)
            throw std::runtime_error("Stack underflow: stack is empty");
        Node* temp = topNode;
        int value = temp->data;
        topNode = topNode->next;
        delete temp;
        return value;
    }

    int peek() const {
        if (!topNode)
            throw std::runtime_error("Stack is empty: nothing to peek");
        return topNode->data;
    }

    bool empty() const {
        return topNode == nullptr;
    }

    bool search(int value) const {
        Node* current = topNode;
        while (current) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    std::string display() const {
        std::ostringstream out;
        out << "TOP\n";
        Node* current = topNode;
        while (current) {
            out << "|  " << current->data << "  |\n";
            current = current->next;
        }
        out << "====== (BOTTOM)\nSize: " << size();
        return out.str();
    }
};


// ==================================================
// 4. QUEUE (FIFO)
// ==================================================
// Custom FIFO Queue implemented with front and rear pointers for O(1) operations.

class Queue {
private:
    struct Node {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };

    Node* frontNode = nullptr;
    Node* rearNode = nullptr;

public:
    Queue() = default;

    Queue(const Queue& other) {
        Node* cur = other.frontNode;
        while (cur) {
            enqueue(cur->data);
            cur = cur->next;
        }
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            Node* cur = other.frontNode;
            while (cur) {
                enqueue(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    ~Queue() {
        clear();
    }

    void clear() {
        while (frontNode) {
            Node* temp = frontNode;
            frontNode = frontNode->next;
            delete temp;
        }
        rearNode = nullptr;
    }

    int size() const {
        int count = 0;
        Node* current = frontNode;
        while (current) {
            ++count;
            current = current->next;
        }
        return count;
    }

    int at(int index) const {
        Node* current = frontNode;
        while (current && index > 0) {
            current = current->next;
            --index;
        }
        if (!current)
            throw std::out_of_range("Invalid queue index: out of bounds");
        return current->data;
    }

    void enqueue(int value) {
        Node* node = new Node(value);
        if (!rearNode) {
            frontNode = rearNode = node;
        } else {
            rearNode->next = node;
            rearNode = node;
        }
    }

    int dequeue() {
        if (!frontNode)
            throw std::runtime_error("Queue underflow: queue is empty");
        Node* temp = frontNode;
        int value = temp->data;
        frontNode = frontNode->next;
        if (!frontNode)
            rearNode = nullptr;
        delete temp;
        return value;
    }

    int front() const {
        if (!frontNode)
            throw std::runtime_error("Queue is empty: no front element");
        return frontNode->data;
    }

    bool empty() const {
        return frontNode == nullptr;
    }

    bool search(int value) const {
        Node* current = frontNode;
        while (current) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    std::string display() const {
        std::ostringstream out;
        out << "FRONT -> ";
        Node* current = frontNode;
        while (current) {
            out << current->data << " -> ";
            current = current->next;
        }
        out << "REAR\nTotal Elements: " << size();
        return out.str();
    }
};


// ==================================================
// 5. BINARY SEARCH TREE
// ==================================================
// Custom BST with duplicate value handling (frequency count),
// traversals (inorder, preorder, postorder), 0/1/2 child deletion,
// and flat snapshot export for live graphical tree rendering.

class BST {
public:
    struct VisualNode {
        int value;
        int left;
        int right;
        int count;
    };

private:
    struct Node {
        int data;
        int count;
        Node* left;
        Node* right;

        Node(int value)
            : data(value), count(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    Node* insertNode(Node* node, int value) {
        if (!node)
            return new Node(value);

        if (value < node->data)
            node->left = insertNode(node->left, value);
        else if (value > node->data)
            node->right = insertNode(node->right, value);
        else
            ++node->count;

        return node;
    }

    bool searchNode(Node* node, int value) const {
        if (!node)
            return false;
        if (node->data == value)
            return true;
        if (value < node->data)
            return searchNode(node->left, value);
        return searchNode(node->right, value);
    }

    void inorderNode(Node* node, std::ostringstream& out) const {
        if (!node) return;
        inorderNode(node->left, out);
        for (int i = 0; i < node->count; ++i)
            out << node->data << " ";
        inorderNode(node->right, out);
    }

    void preorderNode(Node* node, std::ostringstream& out) const {
        if (!node) return;
        for (int i = 0; i < node->count; ++i)
            out << node->data << " ";
        preorderNode(node->left, out);
        preorderNode(node->right, out);
    }

    void postorderNode(Node* node, std::ostringstream& out) const {
        if (!node) return;
        postorderNode(node->left, out);
        postorderNode(node->right, out);
        for (int i = 0; i < node->count; ++i)
            out << node->data << " ";
    }

    int countDistinctNodes(Node* node) const {
        if (!node) return 0;
        return 1 + countDistinctNodes(node->left) + countDistinctNodes(node->right);
    }

    int countTotalElements(Node* node) const {
        if (!node) return 0;
        return node->count + countTotalElements(node->left) + countTotalElements(node->right);
    }

    int buildSnapshot(Node* node, VisualNode* output, int& nextIndex) const {
        if (!node)
            return -1;

        int index = nextIndex++;
        output[index] = { node->data, -1, -1, node->count };

        output[index].left = buildSnapshot(node->left, output, nextIndex);
        output[index].right = buildSnapshot(node->right, output, nextIndex);

        return index;
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    Node* removeWholeNode(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->data) {
            node->left = removeWholeNode(node->left, value);
        } else if (value > node->data) {
            node->right = removeWholeNode(node->right, value);
        } else {
            // Case 0 or 1 child
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // Case 2 children: inorder successor
            Node* successor = node->right;
            while (successor->left)
                successor = successor->left;

            node->data = successor->data;
            node->count = successor->count;
            node->right = removeWholeNode(node->right, successor->data);
        }
        return node;
    }

    Node* eraseNode(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->data) {
            node->left = eraseNode(node->left, value);
        } else if (value > node->data) {
            node->right = eraseNode(node->right, value);
        } else {
            // If duplicate count > 1, decrement count
            if (node->count > 1) {
                --node->count;
                return node;
            }

            // Case 0 or 1 child
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Case 2 children: replace with inorder successor
            Node* successor = node->right;
            while (successor->left)
                successor = successor->left;

            node->data = successor->data;
            node->count = successor->count;
            node->right = removeWholeNode(node->right, successor->data);
        }
        return node;
    }

    Node* copyTree(Node* node) {
        if (!node) return nullptr;
        Node* copy = new Node(node->data);
        copy->count = node->count;
        copy->left = copyTree(node->left);
        copy->right = copyTree(node->right);
        return copy;
    }

public:
    BST() = default;

    BST(const BST& other) {
        root = copyTree(other.root);
    }

    BST& operator=(const BST& other) {
        if (this != &other) {
            destroy(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    ~BST() {
        destroy(root);
        root = nullptr;
    }

    void clear() {
        destroy(root);
        root = nullptr;
    }

    void insert(int value) {
        root = insertNode(root, value);
    }

    bool search(int value) const {
        return searchNode(root, value);
    }

    bool remove(int value) {
        if (!search(value))
            return false;
        root = eraseNode(root, value);
        return true;
    }

    std::string inorder() const {
        std::ostringstream out;
        inorderNode(root, out);
        return out.str();
    }

    std::string preorder() const {
        std::ostringstream out;
        preorderNode(root, out);
        return out.str();
    }

    std::string postorder() const {
        std::ostringstream out;
        postorderNode(root, out);
        return out.str();
    }

    int size() const {
        return countDistinctNodes(root);
    }

    int totalCount() const {
        return countTotalElements(root);
    }

    bool empty() const {
        return root == nullptr;
    }

    int exportVisualNodes(VisualNode* output, int capacity) const {
        int required = size();
        if (!output || capacity < required)
            return required;

        int nextIndex = 0;
        buildSnapshot(root, output, nextIndex);
        return required;
    }
};


// ==================================================
// 6. MIN HEAP
// ==================================================
// Custom Min-Binary-Heap using manual dynamic array allocation.
// Supports insertion, extract-min, searching, and heap array inspection.

class MinHeap {
private:
    int* data;
    int len;
    int cap;

    void grow() {
        int newCap = (cap == 0) ? 4 : cap * 2;
        int* temp = new int[newCap];

        for (int i = 0; i < len; ++i)
            temp[i] = data[i];

        delete[] data;
        data = temp;
        cap = newCap;
    }

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (data[parent] <= data[index])
                break;

            int temp = data[parent];
            data[parent] = data[index];
            data[index] = temp;

            index = parent;
        }
    }

    void siftDown(int index) {
        while (true) {
            int smallest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            if (left < len && data[left] < data[smallest])
                smallest = left;

            if (right < len && data[right] < data[smallest])
                smallest = right;

            if (smallest == index)
                break;

            int temp = data[index];
            data[index] = data[smallest];
            data[smallest] = temp;

            index = smallest;
        }
    }

public:
    MinHeap() : data(nullptr), len(0), cap(0) {}

    MinHeap(const MinHeap& other) : data(nullptr), len(other.len), cap(other.cap) {
        if (cap > 0) {
            data = new int[cap];
            for (int i = 0; i < len; ++i)
                data[i] = other.data[i];
        }
    }

    MinHeap& operator=(const MinHeap& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            cap = other.cap;
            data = nullptr;
            if (cap > 0) {
                data = new int[cap];
                for (int i = 0; i < len; ++i)
                    data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~MinHeap() {
        delete[] data;
        data = nullptr;
        len = 0;
        cap = 0;
    }

    void clear() {
        delete[] data;
        data = nullptr;
        len = 0;
        cap = 0;
    }

    int size() const {
        return len;
    }

    int capacity() const {
        return cap;
    }

    bool empty() const {
        return len == 0;
    }

    int at(int index) const {
        if (index < 0 || index >= len)
            throw std::out_of_range("Invalid heap index: out of bounds");
        return data[index];
    }

    int peekMin() const {
        if (len == 0)
            throw std::runtime_error("Heap is empty: no minimum element");
        return data[0];
    }

    void insert(int value) {
        if (len == cap)
            grow();

        data[len] = value;
        siftUp(len);
        ++len;
    }

    int extractMin() {
        if (len == 0)
            throw std::runtime_error("Heap underflow: cannot extract from empty heap");

        int result = data[0];
        --len;

        if (len > 0) {
            data[0] = data[len];
            siftDown(0);
        }

        return result;
    }

    bool search(int value) const {
        for (int i = 0; i < len; ++i) {
            if (data[i] == value)
                return true;
        }
        return false;
    }

    std::string display() const {
        std::ostringstream out;
        out << "HEAP ARRAY: [ ";
        for (int i = 0; i < len; ++i)
            out << data[i] << " ";
        out << "]";
        if (len > 0)
            out << "\nMinimum Element: " << data[0];
        out << "\nTotal Elements: " << len;
        return out.str();
    }
};

} // namespace algolib

#endif // ALGOLIB_DATA_STRUCTURES_HPP

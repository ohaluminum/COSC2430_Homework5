#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "ArgumentManager.h"

struct node
{
    //Degree: Determine maximum value storage (n - 1) and maximum child nodes (n).
    int degree;

    //Array to store key value
    int* key;

    //Array to store child node pointer
    node** child;

    //Number of key value
    int keyNum;

    //Number of child node
    int childNum;

    //The node is leaf or not 
    bool isLeaf;

    node(int degree = 2, bool isLeaf = true)
    {
        //Initialize the degree of node and identify the new node
        this->degree = degree;
        this->isLeaf = isLeaf;

        //Initialize the value array
        key = new int[degree];

        for (int i = 0; i < degree; i++)
        {
            key[i] = 0;
        }

        //Initialize the child array
        child = new node * [degree + 1];

        for (int i = 0; i < degree + 1; i++)
        {
            child[i] = nullptr;
        }

        //Initialize the number of key
        keyNum = 0;
        childNum = 0;
    }

    ~node()
    {
        //Delete value array
        delete[] key;
        key = nullptr;

        //Delete child array
        for (int i = 0; i < degree + 1; i++)
        {
            delete child[i];
            child[i] = nullptr;
        }

        delete[] child;
        child = nullptr;
    }

    bool isKeyFull()
    {
        if (keyNum >= degree - 1)
        {
            return true;
        }

        return false;
    }

    void splitChild(node* left, node* right, int& median)
    {
        //The last (degree / 2) keys will belong to right node after splitting
        for (int i = 0; i < degree / 2; i++)
        {
            right->key[i] = left->key[i + degree - degree / 2];
            left->key[i + degree - degree / 2] = 0;
        }

        //Store the median key
        median = left->key[(degree - 1) / 2];
        left->key[(degree - 1) / 2] = 0;

        //Update the number of key of left child (key = degree - degree / 2 - 1)
        left->keyNum = degree - degree / 2 - 1;

        //Update the number of key of right child (key = degree / 2)
        right->keyNum = degree / 2;

        //If the right node has child node, need to splid child also
        if (!left->isLeaf)
        {
            //The last (degree / 2 + 1) children will belong to right node after splitting
            for (int i = 0; i < degree / 2 + 1; i++)
            {
                right->child[i] = left->child[i + degree - degree / 2];
                left->child[i + degree - degree / 2] = nullptr;
            }

            //Update the number of child
            left->childNum = left->keyNum + 1;
            right->childNum = right->keyNum + 1;
        }
    }

    void directInsert(int value)
    {
        int pos = 0;

        // --------- Using insertion sort -----------

        //Start from the last position
        pos = this->keyNum - 1;

        while (pos >= 0 && this->key[pos] > value)
        {
            //Push back one position
            this->key[pos + 1] = this->key[pos];

            pos--;
        }

        //Insert the new value at the correct position
        this->key[pos + 1] = value;

        //Update the number of key
        this->keyNum++;
    }

    //Search if the value already exist 
    bool searchHelper(int value)
    {
        int pos = 0;

        //Find the correct psotion 
        pos = this->keyNum - 1;

        while (pos >= 0 && this->key[pos] > value)
        {
            pos--;
        }

        //At this time, key[pos] is less than or equal to target value
        if (this->key[pos] == value)
        {
            //If equal return true
            return true;
        }

        //Before go to deeper level, check if this node is already leaf node
        if (this->isLeaf)
        {
            return false;
        }

        //Continue search at the deeper level
        return this->child[pos + 1]->searchHelper(value);
    }

    int heightHelper()
    {
        int pos = 0;

        if (this->isLeaf)
        {
            return 1;
        }

        for (int i = 0; i < this->childNum; i++)
        {
            if (this->child[i] != nullptr)
            {
                pos = i;
                break;
            }
        }

        return 1 + this->child[pos]->heightHelper();
    }
};

class BT
{
private:

    node* root;

    int degree;

public:

    BT(int degree = 2)
    {
        root = nullptr;
        this->degree = degree;
    }

    node* getRoot()
    {
        return root;
    }

    void insert(int value, node* currRoot, node* parentNode)
    {
        int pos = 0;

        //The median key need to be pushed to the parent
        int median = 0;

        //If the tree is empty
        if (root == nullptr)
        {
            //Create a temp leaf node
            node* temp = new node(degree, true);

            //Insert the value
            temp->key[temp->keyNum] = value;

            //Update the number of key
            temp->keyNum++;

            root = temp;
        }

        //If the tree is not empty
        else
        {
            //The current root is not leaf - go deeper level
            if (!currRoot->isLeaf)
            {
                //Find the proper child as a new root
                pos = currRoot->keyNum - 1;

                while (pos >= 0 && currRoot->key[pos] > value)
                {
                    pos--;
                }

                //Recursion: go to the deeper level root
                insert(value, currRoot->child[pos + 1], currRoot);

                //Check if the current root is more than maximum limit after inserting
                if (currRoot->keyNum >= degree)
                {
                    //The current root is the root of the tree
                    if (currRoot == root)
                    {
                        // -------------------- SPLIT ---------------------

                        //Create a new node to store the second half of key
                        node* newChild = new node(currRoot->degree, currRoot->isLeaf);

                        //Create a new root node
                        node* newRoot = new node(currRoot->degree, false);

                        //Split the old root
                        newRoot->splitChild(currRoot, newChild, median);

                        // --------------------- CONNECT -------------------
                        newRoot->key[0] = median;
                        newRoot->child[0] = currRoot;
                        newRoot->child[1] = newChild;

                        newRoot->keyNum += 1;
                        newRoot->childNum += 2;

                        //Change root
                        root = newRoot;
                    }

                    //The current root is not the root of the tree
                    else
                    {
                        // -------------------- SPLIT ---------------------

                        //Create a new node to store the second half of key
                        node* newChild = new node(currRoot->degree, currRoot->isLeaf);

                        //Split the old root
                        parentNode->splitChild(currRoot, newChild, median);

                        // --------------------- CONNECT -------------------

                        //Find the proper position to push the median to the parent node
                        pos = parentNode->keyNum - 1;

                        while (pos >= 0 && parentNode->key[pos] > median)
                        {
                            //Push back one position
                            parentNode->key[pos + 1] = parentNode->key[pos];
                            parentNode->child[pos + 2] = parentNode->child[pos + 1];

                            pos--;
                        }

                        parentNode->key[pos + 1] = median;
                        parentNode->child[pos + 1] = currRoot;
                        parentNode->child[pos + 2] = newChild;

                        parentNode->keyNum += 1;
                        parentNode->childNum += 1;
                    }
                }
            }

            //The current root is leaf - no need to go deeper level
            else
            {
                //The current root is not full
                if (!currRoot->isKeyFull())
                {
                    //Call insert helper function
                    currRoot->directInsert(value);
                }

                //The current root is full
                else
                {
                    //The current root is the root of the tree - create a new new root
                    if (currRoot == root)
                    {
                        // ------------------- INSERT ---------------------

                        //Call insert helper function
                        currRoot->directInsert(value);

                        // -------------------- SPLIT ---------------------

                        //Create a new node to store the second half of key
                        node* newChild = new node(currRoot->degree, currRoot->isLeaf);

                        //Create a new root node
                        node* newRoot = new node(currRoot->degree, false);

                        //Split the old root
                        newRoot->splitChild(currRoot, newChild, median);

                        // --------------------- CONNECT -------------------
                        newRoot->key[0] = median;
                        newRoot->child[0] = currRoot;
                        newRoot->child[1] = newChild;

                        newRoot->keyNum += 1;
                        newRoot->childNum += 2;

                        //Change root
                        root = newRoot;
                    }

                    //The current root is not the root of the tree
                    else
                    {
                        // ------------------- INSERT ---------------------

                        //Call insert helper fnction
                        currRoot->directInsert(value);

                        // -------------------- SPLIT ---------------------

                        //Create a new node to store the second half of key
                        node* newChild = new node(currRoot->degree, currRoot->isLeaf);

                        //Split the old root
                        parentNode->splitChild(currRoot, newChild, median);

                        // --------------------- CONNECT -------------------

                        //Find the proper position to push the median to the parent node
                        pos = parentNode->keyNum - 1;

                        while (pos >= 0 && parentNode->key[pos] > median)
                        {
                            //Push back one position
                            parentNode->key[pos + 1] = parentNode->key[pos];
                            parentNode->child[pos + 2] = parentNode->child[pos + 1];

                            pos--;
                        }

                        parentNode->key[pos + 1] = median;
                        parentNode->child[pos + 1] = currRoot;
                        parentNode->child[pos + 2] = newChild;

                        parentNode->keyNum += 1;
                        parentNode->childNum += 1;
                    }
                }
            }
        }
    }

    //Search if the value already exist 
    bool search(int value)
    {
        //If the tree is empty
        if (root == nullptr)
        {
            return false;
        }

        //Call helper function
        return root->searchHelper(value);
    }

    int height()
    {
        //If the tree is empty
        if (root == nullptr)
        {
            return 0;
        }

        //If the tree is not empty
        return root->heightHelper();
    }

    void printByLevel(int level, ofstream& outFS)
    {
        //If the tree is empty
        if (root == nullptr)
        {
            return;
        }

        if (level > this->height())
        {
            outFS << "Empty" << endl;
            return;
        }

        queue<node*> q;

        //Record current layer
        int count = 0;

        //Push the root of the tree to the queue
        q.push(root);

        //Push the null pointer to the queue as delimiter
        q.push(nullptr);

        //Increase counter after push an entire level
        count++;

        while (!q.empty())
        {
            //Delimiter - One level ends
            if (q.front() == nullptr)
            {
                //Have other node except for the nullptr
                if (q.size() > 1)
                {
                    //Put another delimiter to the queue
                    q.push(nullptr);
                    count++;
                }
            }
            else
            {
                if (count > level)
                {
                    break;
                }

                if (level == count)
                {
                    //Print all the key of the node in specific level 
                    for (int i = 0; i < q.front()->keyNum; i++)
                    {
                        outFS << q.front()->key[i] << " ";
                    }
                }

                //Push all the child node of the node
                for (int j = 0; j < q.front()->childNum; j++)
                {
                    if (q.front()->child[j] != nullptr)
                    {
                        q.push(q.front()->child[j]);
                    }
                }
            }

            q.pop();
        }

        outFS << endl;
    }
};

// ---------------------------------------- MAIN FUNCTION --------------------------------------------

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    //Get the filename of argument name "input" and "output" and "command"
    string input = am.get("input");
    string output = am.get("output");
    string command = am.get("command");

    //Test
    //string input = "input53.txt";
    //string output = "output53.txt";
    //string command = "command53.txt";

    ifstream inFS;
    ifstream comFS;
    ofstream outFS;
    istringstream inSS;

    //Open input file and output file and command file 
    inFS.open(input);
    comFS.open(command);
    outFS.open(output);

    string line = "";
    string bin = "";
    string degree_str = "";
    string level_str = "";
    int degree = 0;
    int level = 0;

    int newNum = 0;

    getline(comFS, line);

    //Clear input string stream
    inSS.clear();

    //Using input string stream to read problem line
    inSS.str(line);

    //Read useless information
    getline(inSS, bin, '=');

    //Read degree information
    getline(inSS, degree_str);
    degree = stoi(degree_str);

    //Create a new B-Tree with degree information
    BT tree(degree);

    //Read input file
    while (inFS >> newNum)
    {
        //Check if the new number already exist in tree
        if (!tree.search(newNum))
        {
            //Three parameter for insert function: 
            //1. insert value 
            //2. root node 
            //3. parent node
            tree.insert(newNum, tree.getRoot(), nullptr);
        }
    }

    //Output the height of the tree
    outFS << "Height=" << tree.height() << endl;

    while (getline(comFS, line))
    {
        //Check if the line is empty (if so continue to read next line)
        if (line.empty())
        {
            continue;
        }

        //Clear input string stream
        inSS.clear();

        //Using input string stream to read problem line
        inSS.str(line);

        //Read useless information
        getline(inSS, bin, ' ');

        //Read level information
        getline(inSS, level_str);
        level = stoi(level_str);

        tree.printByLevel(level, outFS);
    }

    //Close files
    inFS.close();
    comFS.close();
    outFS.close();

    return 0;
}

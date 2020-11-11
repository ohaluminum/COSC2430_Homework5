#include <iostream>
#include <fstream>
#include <sstream>
#include "ArgumentManager.h"

void bubbleSort(int* keyArr, int size)
{
    bool isSwapped;
    int temp;

    for (int i = 0; i < size - 1; i++)
    {
        isSwapped = false;

        for (int j = 0; j < size - 1 - i; j++)
        {
            if (keyArr[j] > keyArr[j + 1])
            {
                temp = keyArr[j];
                keyArr[j] = keyArr[j + 1];
                keyArr[j + 1] = temp;

                isSwapped = true;
            }
        }



        //Early exit
        if (isSwapped == false)
        {
            break;
        }
    }
}

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

    node(int degree = 2)
    {
        //Initialize the degree of node
        this->degree = degree;

        //Initialize the value array
        key = new int[degree - 1];

        for (int i = 0; i < degree - 1; i++)
        {
            key[i] = 0;
        }

        //Initialize the child array
        child = new node*[degree];

        for (int i = 0; i < degree; i++)
        {
            child[i] = nullptr;
        }

        //Initialize the number of key and child
        keyNum = 0;
        childNum = 0;
    }

    ~node()
    {
        //Delete value array
        delete[] key;
        key = nullptr;

        //Delete child array
        for (int i = 0; i < degree; i++)
        {
            delete child[i];
            child[i] = nullptr;
        }

        delete[] child;
        child = nullptr;
    }

    bool isKeyFull()
    {
        if (keyNum == degree - 1)
        {
            return true;
        }

        return false;
    }

    bool isChildFull()
    {
        if (childNum == degree)
        {
            return true;
        }

        return false;
    }

    bool isChildEmpty()
    {
        if (childNum == 0)
        {
            return true;
        }

        return false;
    }
};

class BT
{
private:

    node* root;

public:

    BT()
    {
        root = nullptr;
    }

    node* getRoot()
    {
        return root;
    }

    void insert(int value, node* root, int degree)
    {
        int temp = 0;

        //If the tree is empty
        if (this->root == nullptr)
        {
            //Create a temp node
            node* temp = new node(degree);

            //Insert the value
            temp->key[temp->keyNum] = value;

            //Update the number of key
            temp->keyNum++;

            this->root = temp;
        }

        //If the tree is not empty
        else
        {
            //If the given root has not child - no need to go deep
            if (root->isChildEmpty())
            {
                //If the given root is not full
                if (!root->isKeyFull())
                {
                    //Insert into a last position
                    root->key[root->keyNum] = value;

                    //Update the number of key
                    root->keyNum++;

                    //Sort the key ascending
                    bubbleSort(root->key, root->keyNum);
                }

                //If the given root is full
                else
                {
                    //Split the node


                }


            }

        }
    }



};

// ---------------------------------------- MAIN FUNCTION --------------------------------------------

int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);

    //Get the filename of argument name "input" and "output" and "command"
    //string input = am.get("input");
    //string output = am.get("output");
    //string command = am.get("command");

    //Test
    string input = "input51.txt";
    string output = "output51.txt";
    string command = "command51.txt";

    ifstream inFS;
    ifstream comFS;
    ofstream outFS;
    istringstream inSS;

    //Open input file and output file and command file 
    inFS.open(input);
    comFS.open(command);
    outFS.open(output);

    string line ="";
    string bin = "";
    string degree_str = "";
    int degree = 0;

    int newNum = 0;
    BT tree;

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

    cout << "Degree: " << degree << endl;

    //Read input file
    while (inFS >> newNum)
    {
        tree.insert(newNum, tree.getRoot(), degree);

        cout << "New: " << newNum << endl;



    }








    //Close files
    inFS.close();
    outFS.close();

    return 0;
}

/**
 * Issues:-
 * - define subtree_max attribute
 *  
 * Completed:-
 * - changed insert fn
 */


#include <iostream>
#include <algorithm>
#define RED 1
#define BLACK 0
using namespace std;

struct node
{
    int interval_low, interval_high;
    int subtree_max;
    int color;
    node* left;
    node* right;

    node() {
        left = 0;
        right = 0;
    }
};

class interval_tree {
private: 
    node* root;
    void swap(int& a, int& b) 
    {
        int temp = a;
        a = b;
        b = temp;
    }
    node* insert(node* tree, int interval_low, int interval_high)//changed
    {
        if(tree == NULL)
        {
            tree = new node;
            tree->interval_low = interval_low;
            tree->interval_high = interval_high;
            tree->subtree_max = interval_high;
            tree->color = RED;
        }
        else if(tree->interval_low <= interval_low)
        {
            tree->right = insert(tree->right, interval_low, interval_high);
        }
        else
        {
            tree->left = insert(tree->left, interval_low, interval_high); 
        }

        if(isRed(tree->right) && !isRed(tree->left)) tree = rotateLeft(tree);
        if(isRed(tree->left) && tree->left!= NULL && isRed(tree->left->left)) tree = rotateRight(tree);
        if(isRed(tree->left) && isRed(tree->right)) tree = flipColors(tree);
        

        if(tree->subtree_max < interval_high)
            tree->subtree_max = interval_high;

        return tree;
    }
    node* minValueNode(node* x) {
        node* current = x;
        while(current->left != 0) 
            current = current->left;
        return current;
    }
    node* deletefn(node* tree, int interval_low, int interval_high)//changed
    {
        if(tree == NULL)
        {
            cout<<"Node not found"<<endl;
            return 0;
        }
        
        else if(tree->interval_low < interval_low)
        {
            tree->right = deletefn(tree->right, interval_low, interval_high);
        }
        else if(tree->interval_low > interval_low)
        {
            tree->left = deletefn(tree->left, interval_low, interval_high); 
        }
        else {
            if(tree->left == 0) {
                node *temp = tree->right;
                tree->interval_low = 0;
                tree->interval_high = 0;
                return temp;
            }
            else if(tree->right == 0) {
                node *temp = tree->left;
                tree->interval_low = 0;
                tree->interval_high = 0;
                return temp;
            }
            else {
                node *temp=minValueNode(root->right);
                root->interval_low = temp->interval_low;
                root->interval_high = temp->interval_high;
                root->right = deletefn(root->right, temp->interval_low, temp->interval_high);
            }
        }

        if(isRed(tree->right) && !isRed(tree->left)) tree = rotateLeft(tree);
        if(isRed(tree->left) && tree->left!= NULL && isRed(tree->left->left)) tree = rotateRight(tree);
        if(isRed(tree->left) && isRed(tree->right)) tree = flipColors(tree);
        

        if(tree->subtree_max < interval_high)
            tree->subtree_max = interval_high;

        return tree;
    }
    node* rotateLeft(node* tree)
    {
        node* temp = tree->right;
        tree->right = temp->left;
        temp->left = tree;
        swap(tree->color,temp->color);
        return temp;
    }
    node* rotateRight(node* tree)
    {
        node* temp = tree->left;
        tree->left = temp->right;
        temp->right = tree;
        swap(tree->color, temp->color);
        return temp;
    }
    node* flipColors(node* tree)
    {
        tree->left->color = BLACK;
        tree->right->color = BLACK;
        tree->color = RED;
        return tree;
    }
    bool isRed(node* tree)
    {
        if(tree == NULL) return false;  
        return tree->color == RED;
    }
    void printLevel(node* tree, int currentLevel, int level)
    {
        if(tree!=NULL)
        {
            printLevel(tree->left, currentLevel+1, level);
            if(currentLevel == level)
                cout<<tree->interval_low<<"-"<<tree->interval_high<<" ";
            else
                cout<<"  ";
            printLevel(tree->right, currentLevel+1, level);
        }
    }
    int depth(node* tree)
    {
        if(tree == NULL) return 0;
        else
        {
            return 1 + max(depth(tree->right), depth(tree->left));
        }
    }
    node* search(node* tree, node* query) {
        if(tree == 0)
            return 0;
        //cout<<"tree->low="<<tree->interval_low<<endl;
        if(isOverlapping(tree, query)) {
            return tree;
        }

        if(tree->left != 0 && tree->left->subtree_max >= query->interval_low) {
            return search(tree->left, query);
        }
        else return search(tree->right, query);
    }
    bool isOverlapping(node* a, node* b) {
        //cout<<"a"<<a->interval_low<<"-"<<a->interval_high<<endl;
        //cout<<"b"<<b->interval_low<<"-"<<b->interval_high<<endl;
        if(a->interval_low <= b->interval_high && b->interval_low <= a->interval_high)
        {   
            //cout<<"true"<<endl;
            return true;
        }
        else return false;
    }
public:
    interval_tree()
    {
        root=NULL;
    }
    void insert(int low, int high)
    {
        root = insert(root, low, high);
    }
    void deletefn(int low, int high) {
        root = deletefn(root, low, high);
    }
    void printTree()
    {
        int dep = depth(root);
        cout<<"Depth : "<<dep<<endl;
        for(int i=0;i<dep;i++)
        {
            printLevel(i);
            cout<<endl;
        }
    }
    void printLevel(int level)
    {
        printLevel(root,0,level);
    }
    int depth()
    {
        return depth(root);
    }
    node* search(int low, int high) {
        node* query = new node;
        query->interval_low = low;
        query->interval_high = high;
        return search(root, query);
    }
};

int main()
{
    interval_tree interval_tree;
    int low, high;
    // Adding integers till a zero in provided
    do
    {
        cin>>low>>high;
        if(low) interval_tree.insert(low, high);
    }
    while(low!=0);
    interval_tree.printTree();

    cout<<"enter lower limit and upper limit of nos to search"<<endl;
    
        cin>>low>>high;
        if(low){
            node* ans=interval_tree.search(low, high);
                if(ans!=0) {
                    cout<<"Overlapping interval found"<<endl;
                    cout<<ans->interval_low<<"-"<<ans->interval_high<<endl;
                }
                else cout<<"Overlapping interval not found"<<endl;
        }
    
    
    
    cout<<"enter 2 nos to delete"<<endl;
        cin>>low>>high;
        interval_tree.deletefn(low, high);
    
    interval_tree.printTree();
    
    return 0;
}

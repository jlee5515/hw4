#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int helper(Node* root){
    if(root == nullptr){ //base case; checks to see if the tree is empty
        return 0;
    }


    //recurse until base case is hit, finding the number of nodes on both the right and left side
    int x = 0;
    if(helper(root->right) >= helper(root->left)){
        x = helper(root->right); //if the right side > left side, x = height of right side
    }
    else{
        x = helper(root->left); //if the left side > right side, x = height of right side
    }
    return x + 1; //increment by one because we are counting nodes from start to end inclusive
    
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr){ //base case; checks to see if the tree is empty
        return 1;
    }
    
    //cases in which only the right or left side has nodes
    else if(root->right != nullptr && root->left == nullptr){
        return equalPaths(root->right);
    }
    else if(root->left != nullptr && root->right == nullptr){
        return equalPaths(root->left);
    }

    //cases in which there are nodes on both sides
    else{
        int leftSide = helper(root->left);
        int rightSide = helper(root->right);
        if(leftSide != rightSide){
            return 0;
        }
    }
    return 1;
}


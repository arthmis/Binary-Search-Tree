#include <algorithm>
#include <iostream>

template <typename object>
class BinaryTree{
  public:
   BinaryTree();
   BinaryTree(BinaryTree &tree_to_copy);
   BinaryTree(BinaryTree &&tree_to_move);
   ~BinaryTree();

   const object & findMax() const;
   const object & findMin() const;
   bool isEmpty();
   void printTreeSorted() const;
   void makeEmpty();
   void insert(const object & item);
   void insert(const object && item);
   void remove(const object & item);
   bool contains(const object & item) const;

  private:
   struct TreeNode{
     object data;
     TreeNode *left_child_;
     TreeNode *right_child_;

     TreeNode(TreeNode *left, TreeNode *right): left_child_(left),
                                                right_child_(right) {}

     TreeNode(const object & item, TreeNode *left, TreeNode *right):
        data(item), left_child_(left), right_child_(right) {}

     TreeNode(const object && item, TreeNode *left, TreeNode *right):
       data(std::move(item)), left_child_(left), right_child_(right) {}
   };

   TreeNode *root;

   void insert(const object & item, TreeNode * & subtree_root);
   void insert(const object && item, TreeNode * & subtree_root);
   void printTreeSorted(TreeNode *subtree_root) const;
   const object & findMax(TreeNode *subtree_root) const;
   const object & findMin(TreeNode *subtree_root) const;
   void remove(const object & item_for_deletion, TreeNode * & subtree_root);
   void makeEmpty(TreeNode * & subtree_root);
   void copyInsert(const TreeNode * subtree_root);
   bool contains(const object & item, TreeNode * subtree_root) const;

};

template <typename object>
BinaryTree<object>::BinaryTree(){
  root = nullptr;
}

template <typename object>
BinaryTree<object>::~BinaryTree(){
  makeEmpty(root);
}

// deletes all nodes in the tree;
template <typename object>
void BinaryTree<object>::makeEmpty(){
  makeEmpty(root);
}

// inner function that recursively deletes all nodes starting from leaf nodes
// without children
template <typename object>
void BinaryTree<object>::makeEmpty(TreeNode * & subtree_root){
  if(subtree_root != nullptr){
    makeEmpty(subtree_root->left_child_);
    makeEmpty(subtree_root->right_child_);
  }

  TreeNode* node_to_delete = subtree_root;
  subtree_root = nullptr;
  delete node_to_delete;
}

template <typename object>
BinaryTree<object>::BinaryTree(BinaryTree<object> &tree_to_copy){
  root = nullptr;
  copyInsert(tree_to_copy.root);
}

// helper function that inserts data in the correct order when using the
// copy constructor to create a perfect BST copy in depth and child parent
// relationship
template <typename object>
void BinaryTree<object>::copyInsert(const TreeNode * copy_tree_root){
  if(copy_tree_root != nullptr){
    insert(copy_tree_root->data);
    copyInsert(copy_tree_root->left_child_);
    copyInsert(copy_tree_root->right_child_);
  }
}

template <typename object>
BinaryTree<object>::BinaryTree(BinaryTree<object> &&tree_to_move){
  root = tree_to_move.root;
  tree_to_move.root = nullptr;
}

// public insert for item by reference
template <typename object>
void BinaryTree<object>::insert(const object & item){
  insert(item, root);
}

// public insert by rvalue
template <typename object>
void BinaryTree<object>::insert(const object && item){
  insert(std::move(item), root);
}

// inner insert for item by reference
template <typename object>
void BinaryTree<object>::insert(const object & item, TreeNode * & subtree_root){
  if(subtree_root == nullptr){
    subtree_root = new TreeNode(item, nullptr, nullptr);
  }
  else if(item < subtree_root->data){
    insert(item, subtree_root->left_child_);
  }
  else if(item > subtree_root->data){
    insert(item, subtree_root->right_child_);
  }
  else{
    // a duplicate
  }
}

// inner insert for item by rvalue
template <typename object>
void BinaryTree<object>::insert(const object && item, TreeNode * & subtree_root){
  if(subtree_root == nullptr){
    subtree_root = new TreeNode(std::move(item), nullptr, nullptr);
  }
  else if(item < subtree_root->left_child_->data){
    insert(std::move(item), subtree_root->left_child_);
  }
  else if(item > subtree_root->right_child_->data){
    insert(std::move(item), subtree_root->right_child_);
  }
  else{
    // a duplicate
  }
}

// remove function that calls private remove function
template <typename object>
void BinaryTree<object>::remove(const object & item_for_deletion){
  if(isEmpty())
      return;
  remove(item_for_deletion, root);
}

// deletes item for deletion provided by outer remove function
template <typename object>
void BinaryTree<object>::remove(const object & item_for_deletion,
                                TreeNode * & subtree_root){
  if(subtree_root == nullptr)
    return;

  if(item_for_deletion < subtree_root->data){
    remove(item_for_deletion, subtree_root->left_child_);
  }
  else if(item_for_deletion > subtree_root->data){
    remove(item_for_deletion, subtree_root->right_child_);
  }
  else if(subtree_root->left_child_ != nullptr &&
          subtree_root->right_child_ != nullptr){
    subtree_root->data = findMin(subtree_root);
    remove(subtree_root->data, subtree_root->right_child_);
  }
  else{
    TreeNode *old_node = subtree_root;
    if(subtree_root->left_child_ != nullptr){
      subtree_root = subtree_root->left_child_;
    }
    else{
      subtree_root = subtree_root->right_child_;
    }
    delete old_node;
  }
  // delete subtree_root;
}

// calls private printTreeSorted
template <typename object>
void BinaryTree<object>::printTreeSorted() const{
  printTreeSorted(root);
}

// private function that prints the Tree in sorted order; assumes comparator
// overloads for object
template <typename object>
void BinaryTree<object>::printTreeSorted(TreeNode *subtree_root) const{
  if(subtree_root->left_child_ != nullptr){
    printTreeSorted(subtree_root->left_child_);
  }

  std::cout << subtree_root->data << " ";

  if(subtree_root->right_child_ != nullptr){
    printTreeSorted(subtree_root->right_child_);
  }
}

// public findMax and calls private findMax
template <typename object>
const object & BinaryTree<object>::findMin() const{
  return findMin(root);
}

// private findMax
template <typename object>
const object & BinaryTree<object>::findMin(TreeNode * subtree_root) const{
  if(subtree_root->left_child_ != nullptr){
    return findMin(subtree_root->left_child_);
  }
  return subtree_root->data;
}

// public findMin and calls private findMin
template <typename object>
const object & BinaryTree<object>::findMax() const{
  return findMax(root);
}

// private findMind
template <typename object>
const object & BinaryTree<object>::findMax(TreeNode * subtree_root) const{
  if(subtree_root->right_child_ != nullptr){
    return findMax(subtree_root->right_child_);
  }
  return subtree_root->data;
}

template <typename object>
bool BinaryTree<object>::isEmpty(){
  return root == nullptr;
}

template <typename object>
bool BinaryTree<object>::contains(const object & item) const{
  if(root == nullptr)
    return false;
  return contains(item, root);
}

template <typename object>
bool BinaryTree<object>::contains(const object & item, TreeNode * subtree_root) const{
  if(subtree_root == nullptr)
    return false;
  else if(item < subtree_root->data)
    return contains(item, subtree_root->left_child_);
  else if(item > subtree_root->data)
    return contains(item, subtree_root->right_child_);
  else
    return true;
}

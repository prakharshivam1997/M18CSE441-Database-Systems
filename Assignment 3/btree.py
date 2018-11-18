#!/usr/bin/env python
# coding: utf-8

# In[23]:


#Notations and Implementations as per - https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/


# In[24]:


import sys


# In[25]:


#Mininmum degree = t
#Mininmum number of Keys = t - 1
#Maximum number of keys = 2*t - 1
#Minimum number of children = t & max = 2*t
#Root = min number of keys = 1 & 


# In[60]:


class BTreeNode:
    '''
    Class that describes a B-Tree Node
    '''
    def __init__(self, t, leaf):
        '''
        keys = list of keys of node
        t = minimum degree i.e. t - 1 is the minimum number of keys a node can have except root 
        n = number of keys 
        children = list containing children of this node, there will be n+1 children
        leaf = True is node is a leaf node
        '''
        self.keys = []
        self.t = t
        self.n = 0
        self.c = []
        self.leaf = leaf
        
    def traverse(self):
        '''
        There are n keys and n+1 children
        '''
        #Traverse through the "left" children and all the keys
        for i in range(0,self.n):
            if not self.leaf:
                self.c[i].traverse()
            print(self.keys[i]," ",end = '')
        
        #If node not leaf, the there exists a "right" child for the last key
        if not self.leaf:
            self.c[self.n].traverse()
            
    def search(self, k):
        '''
        Function that searches in the subtree that is rooted by the self node
        Returns True if the key k is present in the tree
        Returns False otherwise
        '''
        i = 0
        
        #Find first key that is not smaller than k
        while (i<self.n and k > self.keys[i]):
            i = i + 1
        #If the key is equal to the search key
        if i < self.n and self.keys[i] == k:
            return True
        
        #If we reached leaf node and key is not search key => no subtree to look into either
        if self.leaf:
            return False
        
        #Search through subtree
        return (self.c[i]).search(k)
    
    def splitChild(self,i, node):
        '''
        Function that splits the the ith child of the current node
        it appends a key to the current "self" node and splits the node in the argument
        Here node is supposed to be full i.e. have 2*t -1 
        '''
        #Create new node that hold t-1 keys and t children
        new_node = BTreeNode(node.t, node.leaf)
        new_node.n = self.t - 1
        
        #Copy last t-1 keys
        for j in range(0,self.t-1):
            new_node.keys.append(node.keys[self.t+j])
        
        #Copy last t children
        if not node.leaf:
            for j in range(0,self.t):
                new_node.c.append(node.c[self.t+j])
        
        #Now the old node size reduces to t-1, the tth key becomes the new key added to self node
        node.n = self.t - 1 
        
        #Insert new key to the self node that is the new node created as one child
        self.c.insert(i+1, new_node)
        
        #Insert new key created as new child 
        self.keys.insert(i, node.keys[self.t-1])
        
        #Number of keys of self node increases by 1
        self.n = self.n+1
        
        #Number of keys of node that has been split is not the first t-1 keys of the original node
        node.keys = node.keys[:self.t-1]
        
        #The Number of Children is the first t children of the original node
        node.c = node.c[:self.t]
        
    def insertNotFull(self, k):
        '''
        Funtion to insert key k into current "self" node
        Condition on self node: self node is NOT full
        '''
        #Initilise i to hold the largest index
        i = self.n - 1
        
        #In the given node find where to insert the key, could be in the subtree of node could be a leaf
        while i>=0 and k < self.keys[i]:
            i = i -1
        i = i + 1
        
        #i now holds the index of the first key of the current that is greater than the key we're trying to insert
        #if self is a leaf, insert into the leaf's key list in ith position
        if self.leaf:
            self.keys.insert(i, k)
            self.n = self.n + 1
            return
        
        #self node is not a leaf but the "left" child is full
        if self.c[i].n == 2*self.t -1:
            #Split the "left" child
            self.splitChild(i, self.c[i])
            #now check in whick child this node needs to be inserted after split, left or right
            if self.keys[i] < k:
                i = i + 1
        self.c[i].insertNotFull(k)


# In[61]:


class BTree:
    '''
    Class that describes the B-Tree
    '''
    def __init__(self, t):
        '''
        root = root of B-Tree, of type BTreeNode
        t = minimum degree of whole tree
        '''
        self.root = None
        self.t = t
        
    def traverse(self):
        '''
        Function that traverses through the whole tree
        '''
        if not self.root is None:
            print("Root: ",end='')
            for i in range(0,self.root.n):
                print(self.root.keys[i]," ",end='')
            print("")
            self.root.traverse()
        print("")
            
    def search(self, k):
        '''
        Function that searches for a key k in the whole tree
        Returns True if found
        Returns False if not found and when the tree is empty - duh
        '''
        if self.root is None:
            return False
        return self.root.search(k)
    
    def insert(self, k):
        '''
        Function to insert k into Tree
        Function specifically for the root and the root node decides if the height of the tree increases
        '''
        
        #If Tree is empty, initialise Tree
        if self.root is None:
            self.root = BTreeNode(self.t, True) #Create new root
            self.root.keys.append(k)
            self.root.n = 1
            return
        
        #If root is full, need to split root 
        if self.root.n == 2*self.t - 1: 
            new_root = BTreeNode(self.t, False)#Created new root
            new_root.c.append(self.root)
            new_root.splitChild(0, self.root)#Split child of new_root, that is split the 0th child of new root
            
            #now new root has 2 children
            if new_root.keys[0] < k:
                new_root.c[1].insertNotFull(k)
            else:
                new_root.c[0].insertNotFull(k)
            
            #setting new root to be the root of the BTree
            self.root = new_root
        
        #If the root is not fulls
        else:
            self.root.insertNotFull(k)
        return


# In[ ]:





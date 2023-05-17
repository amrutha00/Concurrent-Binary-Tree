#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>


typedef struct tnode //tree node
{
  int data;
  struct tnode *left;
  struct tnode *right;
}tnode;

typedef struct tree //root node of the tree
{
    tnode *head;
}tree;

tree *Tree;

tnode* create(int key); //returns a new node with key and left and right as null pointers
void insert(int key); //inserts the nodes in a level order
tnode* delete(int key); //finds the node to be deleted and replaces it with the value of the last node and calls the delete_last function
void delete_last(tree *Tree,tnode *last);//deletes the last node in the tree
void printLevelOrder(tnode* root); //prints the tree nodes in a level order
int search(int key); //searches for the given key in tree and returns 1 if found else 0
void* p_insert(void *key); //thread acquires the write lock for the insert function and after insertion releases the lock
void* p_delete(void *key);//thread acquires the write lock for the delete function and after deletion releases the lock
void* p_search(void *key);//thread acquires the read lock for the search function and after search releases the lock
void* p_print(void *Node);//thread acquires the read lock for the print function and after printing releases the lock

pthread_rwlock_t rwlock;

int main()
{
    
  int n,choice;
  scanf("%d",&n); //reads the number of test cases
  

     int i;
    typedef struct input //structure for storing the choice and the key
    {
            int choice;
            int key;
    }input;
    input arr[n];
    for(i=0;i<n;i++)
    {
            //printf("enter choice and value(0 for print)");
            scanf("%d %d",&arr[i].choice,&arr[i].key);
    }
    int thread_count=n; //for each input a thread is spawned, hence its equal to number of test cases.
    long thread;
    pthread_t* thread_handles = malloc(thread_count*sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL); //lock is intialised
    Tree=malloc(sizeof(tree));
    Tree->head=NULL;
  
     for(i=0;i<n;i++) //for each input based on the choice, a thread is spawned and the assosiated function is called.
    {
      switch(arr[i].choice)
      {
          case 1: //insert in level order
                pthread_create(&thread_handles[i],NULL,p_insert,(void*)&arr[i].key);
                break;

          case 2: //delete element
                
                pthread_create(&thread_handles[i],NULL,p_delete,(void*)&arr[i].key);
               
                
                break;
          case 3://search
        
                pthread_create(&thread_handles[i],NULL,p_search,(void*)&arr[i].key);
               
                break;

          case 4://print in sequential order
                pthread_create(&thread_handles[i],NULL,p_print,(void*)Tree);
               break;

        default:break;
      }
     
  }
    
    for (i = 0; i < thread_count; i++)//the join operation is performed.
    {
    pthread_join(thread_handles[i], NULL);
    }
    free(thread_handles);

   pthread_rwlock_destroy(&rwlock);//the lock is destroyed.
  return 0;
  
}

void* p_insert(void *key)
{
    int *val=(int*)key;
    int v=*val;
    pthread_rwlock_wrlock(&rwlock);
    insert(v);
    pthread_rwlock_unlock(&rwlock);
}
void* p_delete(void *key)
{
    int *val=(int*)key;
    int k=*val;
     pthread_rwlock_wrlock(&rwlock);
    delete(k);
     pthread_rwlock_unlock(&rwlock);
}
void* p_search(void *key)
{
    int *val=(int*)key;
    int k=*val;
     pthread_rwlock_rdlock(&rwlock);
    int res=search(k);))
     pthread_rwlock_unlock(&rwlock);
    printf("%d\n",res);
}
void* p_print(void *Node)
{

   tree*node=(tree*)Node;
    pthread_rwlock_rdlock(&rwlock);
    
    printLevelOrder(node->head);
     pthread_rwlock_unlock(&rwlock);
}




tnode* create(int key)
{
    tnode *new=malloc(sizeof(tnode));
    new->data=key;
    new->left=NULL;
    new->right=NULL;
    return new;
}

void insert(int key)
{
    
    int front=0;int rear=0;
    tnode *temp=malloc(sizeof(tnode));
    tnode *curr=Tree->head;
    if(curr==NULL)
    {
        temp=create(key);
        Tree->head=temp;
        
    }
    else
    {
        //printf("\n entering else in insert");
        temp=create(key);
        tnode **queue=malloc(sizeof(tnode*)*10);
        queue[rear++]=curr; 
        while(front!=rear)
        {
            curr=queue[front++];
           
            if(curr->left==NULL)
            {
                curr->left=create(key);
                break;
            }
            else
            {
                queue[rear++]=curr->left;
            }
            if(curr->right==NULL)
            {
                curr->right=create(key);
                break;
            }
            else
            {
                queue[rear++]=curr->right;
            }

            
        }
    }
     //printf("root value %d",Tree->head->data);

    
}


int search(int key) 
{ 
    tnode *node=Tree->head;
    if (node == NULL) 
        return 0; 
  
   tnode **queue=malloc(sizeof(tnode*)*5);
   int f=0,r=0;
  
    // Enqueue Root and initialize height  
    queue[r++]=node;  
  
    // Queue based level order traversal  
    while (f!=r)  
    {  
        // See if current node is same as x  
        tnode *temp = queue[f++];  
        if (temp->data == key)  
            return 1;  
  
        if (temp->left != NULL)  
            queue[r++]=temp->left; 
        if (temp->right != NULL)  
            queue[r++]=temp->right;  
    }  
   
    return 0;  
    
}   

tnode* delete(int key)
{
    
    tnode *root=Tree->head;
    tnode *temp;tnode *dlt=NULL;
    if(root==NULL)
        return;
    if(root->left==NULL && root->right==NULL)
    {
        if(root->data==key)
       {
            Tree->head=NULL;
            return;
       }    
        else
        {
            return root;
        }            
    }
    tnode **queue=malloc(sizeof(tnode*)*5);
    int f=0;int r=0;
    queue[r++]=root;

    while(f!=r)//traverse till the end of the tree
    {
        temp=queue[f++];//dequeue the node
        if(temp->data==key)
            dlt=temp;//store the value of the node whose data was to be deleted.
        if(temp->left)
            queue[r++]=temp->left;
        if(temp->right)
            queue[r++]=temp->right;
    }
    if(dlt!=NULL)
    {
        int val=temp->data;//replace the node's value with the righmost leaf node value in the tree
        delete_last(Tree,temp);//pass the root node and the rightmost leaf node.
        dlt->data=val;
    }
    
}


void delete_last(tree *Tree,tnode *last)
{
    tnode *temp;
    tnode *root=Tree->head;
    tnode **queue=malloc(sizeof(tnode*)*5);
    int f=0;int r=0;
    queue[r++]=root;
    while(f!=r)
    {
        temp=queue[f++];//dequeue the node
        if(temp->data==last->data)//check if its value is equal to the last node's value
        {
            temp=NULL; //delete the node
            free(temp);
            break;
        }
        if(temp->right)
        {
            if(temp->right->data==last->data)
            {
                temp->right=NULL;//make the right pointer null if the last node was the right child
                free(temp->right);
                break;
            }
            else
            {
                queue[r++]=temp->right;//enqueue the right child 
            }
            
        }
        if(temp->left)
        {
            if(temp->left->data==last->data)
            {
                temp->left=NULL;//make the left pointer null if the last node was the left child
                free(temp->left);
                break;
            }
            else
            {
                queue[r++]=temp->left;//enqueue the left child
            }
            
        }
    }


}
void printLevelOrder(tnode *root) 
{ 
    
  
  
    int r=0;int f=0; 
    tnode **q =malloc(sizeof(tnode*)*10); 
    tnode *temp_node;
    q[r++]=root;
    if(root==NULL) {return;}
    while (f!=r) 
    { 
       temp_node=q[f++]; //dequeue the node
        printf("%d ", temp_node->data); //print the node data
  
        /*Enqueue left child */
        if (temp_node->left) 
        {
            q[r++]=temp_node->left;
        }
        /*Enqueue right child */
        if (temp_node->right) 
        {
            q[r++]=temp_node->right;
        }
        
    } 
   
} 
 




#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "bi_tree.h"
#include "connect.h"

void copy_node(struct user_states *dst, struct user_states *src)
{
    if(dst == NULL || src == NULL)
        return ;
    strcpy(dst -> user_name, src -> user_name);
    dst -> user_ip_addr = src -> user_ip_addr;
    dst -> user_sockfd = src -> user_sockfd;

    //dst -> user_ex_info = src -> user_ex_info;
}

void trans_node(struct user_info *dst, struct user_states *src)
{
    if(dst == NULL || src == NULL)
        return ;
    strcpy(dst -> user_name, src -> user_name);
    dst -> user_ip_addr = src -> user_ip_addr;
    dst -> user_sockfd = src -> user_sockfd;
}

void release_user_tree(struct user_states** root)
{
    if((*root) == NULL)
        return ;

    release_user_tree(&((*root) -> lchild));
    release_user_tree(&((*root) -> rchild));
    struct user_states* temp = (*root);
    if((*root) -> parent != NULL)
    {
        if((*root) == (*root) -> parent -> lchild)
            ((*root) -> parent -> lchild) = NULL;
        else if((*root) == (*root) -> parent -> rchild)
            ((*root) -> parent -> rchild) = NULL;
    }
    (*root) = NULL;
    free(temp);
}

struct user_states* create_user_tree()
{
    return NULL;
}

bool is_leaf(struct user_states *node)
{
    return node -> lchild == NULL && node -> rchild == NULL;
}

void add_user(struct user_states** root, struct user_states* new_user)
{
    if(new_user == NULL || root == NULL)
        return ;

    if((*root) == NULL)
    {
        (*root) = new_user;
        new_user -> lchild = NULL;
        new_user -> rchild = NULL;
        new_user -> parent = NULL;
        return ;
    }
    struct user_states *pointer = *root;
    while(pointer)
    {
        if(new_user -> user_ip_addr > pointer -> user_ip_addr)
        {
            if(pointer -> rchild == NULL)
            {
                pointer -> rchild = new_user;
                new_user -> parent = pointer;
                break;
            }
            else
                pointer = pointer -> rchild;
        }
        else if(new_user -> user_ip_addr < pointer -> user_ip_addr)
        {
            if(pointer -> lchild == NULL)
            {
                pointer -> lchild = new_user;
                new_user -> parent = pointer;
                break;
            }
            else
                pointer = pointer -> lchild;
        }
        else
            return ;
    }

    if(new_user != NULL)
    {
        new_user -> lchild = NULL;
        new_user -> rchild = NULL;
    }
}

struct user_states* find_user(struct user_states* root, unsigned long ip)
{
    if(root == NULL)
        return NULL;// find failed

    if(root -> user_ip_addr < ip)
    {
        return find_user(root -> rchild, ip);
    }
    else if(root -> user_ip_addr > ip)
    {
        return find_user(root -> lchild, ip);
    }
    else
    {
        return root;
    }
}

struct user_states* find_max_user(struct user_states* root)
{
    if(root == NULL)
        return NULL;
    struct user_states* pointer = root;

    while(pointer -> rchild)
    {
        pointer = pointer -> rchild;
    }

    return pointer;

}

struct user_states* find_min_user(struct user_states* root)
{
    if(root == NULL)
        return NULL;

    struct user_states* pointer = root;

    while(pointer -> lchild)
    {
        pointer = pointer -> lchild;
    }

    return pointer;
}

int remove_user(struct user_states** root, unsigned long ip)
{
    if(root == NULL || *root == NULL)
        return 0;
    struct user_states* pointer = find_user(*root, ip);
    if(pointer == NULL)
        return -1;
    if(is_leaf(pointer))
    {
        release_user_tree(&pointer);
        return 0;
    }

    struct user_states* replace = pointer -> lchild == NULL ? find_min_user(pointer -> rchild) : find_max_user(pointer -> lchild);
    struct user_states temp_node;
    copy_node(&temp_node, replace);
    remove_user(&replace, replace -> user_ip_addr);
    copy_node(pointer, &temp_node);
    return 0;
}

void rev_trav(struct user_states *root, struct user_info* dst, int* size)
{
   if(root == NULL)
        return ;

    rev_trav(root -> lchild, dst, size);
    trans_node(&dst[(*size) ++], root);
    rev_trav(root -> rchild, dst, size);
}

int trav_tree(struct user_states *root, struct user_info* dst)
{
    int size = 0;
    rev_trav(root, dst, &size);
    return size;
}

struct user_states* create_user_node(int sockfd, unsigned long ip, char* username)
{
    struct user_states* new_node = (struct user_states*) malloc(sizeof(struct user_states));
    new_node -> user_sockfd = sockfd;
    new_node -> user_ip_addr = ip;
    strcpy(new_node -> user_name, username);
    return new_node;
}




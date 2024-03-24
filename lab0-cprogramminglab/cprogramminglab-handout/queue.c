/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if(q == NULL) return q; // 这里如果malloc失败，返回NULL即可，这是构成一个NULL队列；
    // 否则，初始化为empty队列
    // 初始化head、tail为NULL
    q->head = NULL;
    q->tail = NULL; 
    q->qsize = 0; // 队列大小初始化为0
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
/*
free队列的时候，注意每一个元素都要free！

*/

void queue_free(queue_t *q) {
    if(q == NULL) // 如果是NULL队列
        return;   // 啥也不用干
    list_ele_t *node = q->head; // 找出链表元素头
    while(node != NULL)
    {
        char *str = node->value;
        if(str != NULL) // 如果数据域有内容要free
            free(str); // 直接free
        list_ele_t *now = node;
        node = node->next;
        free(now); // 找到下一个后，将当前节点free掉
    }
   // free(q->qsize); // q->tail已经在前面一轮被free了，这里只需要free q->qsize

    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    free(q);
}

/**
 * @brief Attempts to insert an element at head of a queue
 * 头插入
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    if(q == NULL) // 如果是空队列
        return false; // 直接返回false
    // 是非空队列

    newh = malloc(sizeof(list_ele_t));
    if(newh == NULL) // malloc失败
        return false;
    /* Don't forget to allocate space for the string and copy it */
    size_t len = strlen(s);
    /* What if either call to malloc returns NULL? */
    newh->value = (char *)malloc(len + 1);
    if(newh->value == NULL) // string malloc失败
    {
        free(newh); // 直接free之前malloc的节点，无法插入了
        return false;
    }

    strcpy(newh->value, s); // 这里可以copy字符串了
    // 手动维持一下copy的约束
    newh->value[len] = '\0';
    newh->next = q->head;
    /* 只有这里才会return true */
    if((q->head == NULL) && (q->tail == NULL)) // 是第一个插入的元素
        {
            q->head = newh;
            q->tail = newh;
        }
    else    // 只更新头即可
        q->head = newh;
    
    q->qsize++;

    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */

// 执行尾插入
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    list_ele_t *newt = NULL;
    if(q == NULL)
        return false;
    newt = (list_ele_t *)malloc(sizeof(list_ele_t));
    // malloc node failed
    if(newt == NULL) return false;
    
    size_t len = strlen(s);
    newt->value = (char *)malloc(len + 1);
    
    // malloc str failed
    if(newt->value == NULL)
    {
        free(newt);
        return false;
    }

    strcpy(newt->value, s);
    newt->value[len] = '\0';
    /* 此时执行插入 */
    // 这里，若q->tail为空呢？会出现问题！不空的话，就接上
    newt->next = NULL;
    if(q->tail != NULL)
        q->tail->next = newt;

    if((q->head == NULL) && (q->tail == NULL)) // 如果是尾插入第一个元素
    {
        q->head = newt;
        q->tail = newt;
    }
    else
        q->tail = newt; // 否则，只更新尾节点即可
    // 当前尾节点的下一个，是新的插入的尾巴

    // 更新队列大小
    q->qsize++;

    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */
    // 是NULL queue或empty队列
    
    if(q == NULL) return false;
    if(q->qsize == 0) return false;

    // 这里的q->head一定等于q->tail
    // assert(q->head == q->tail);
    list_ele_t *head_node = q->head;
    // assert(q->head->value != NULL);
    if(buf != NULL) // 这部分是拷贝逻辑
    {
        size_t val_len = strlen(head_node->value);
        if(val_len < bufsize - 1) // 无法拷贝bufsize - 1个字符
        {
            strcpy(buf, head_node->value); // 那就拷贝0,val_len-1
            buf[val_len] = '\0';
        }
        else // val_len >= bufsize - 1
        {
            strncpy(buf, head_node->value, bufsize - 1);
            buf[bufsize - 1] = '\0';
        }        
    }
    free(head_node->value);    // 原head_node的str域free
    // 现在没有节点了
    if(q->qsize == 1)
    {
        q->head = NULL;
        q->tail = NULL;
    }
    else
        q->head = head_node->next; // 剥离指针域
    q->qsize--;     
    free(head_node); // free原有head_node
    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    // NULL queue
    if(q == NULL)
        return 0;
    return q->qsize;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */

/*
queue_reverse要求原址实现链表翻转
其实只需要正向遍历，改变其next方向即可
*/
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if(q == NULL) return;
    if(q->qsize == 0 || q->qsize == 1) return;

    // 此时，维护的队列非空
    list_ele_t *first = q->head;
    list_ele_t *second = q->head->next;
    // 在first是倒数第二，second是最后一个节点前；
    // 最后：
    // head->原有第二个
    // 倒数第一->倒数第二->倒数第三->倒数第四->.....->第二->第一
    while(second != NULL)
    {
        list_ele_t *node = second->next;
        second->next = first; // second指向first
        first = second;
        second = node;
    }
    // 此时，只需要置换q->tail和q->head即可
    q->head->next = NULL;
    list_ele_t *tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
    return;
}

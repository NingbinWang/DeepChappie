#ifndef  _SYS_LIST_H_
#define  _SYS_LIST_H_


typedef struct node		/* node of a linked list. */
{
	struct node* next;	/* Points at the next node in the list */
	struct node* previous;	/* Points at the previous node in the list */
} NODE_T;


typedef struct		/* Header for a linked list. */
{
	NODE_T node;	/* Header list node */
	int count;	/* Number of nodes in list */
} LIST_T;



void list_init(LIST_T *pList);

void list_add(LIST_T *pList, NODE_T *pNode);

int list_count(LIST_T *pList);

void list_concat(LIST_T *pDstList,LIST_T *pAddList);

void list_delete(LIST_T *pList,NODE_T *pNode);

void list_extract(LIST_T *pSrcList,NODE_T *pStartNode, NODE_T *pEndNode,LIST_T *pDstList);

NODE_T *list_first(LIST_T *pList);

NODE_T *list_get(LIST_T *pList);

void list_insert(LIST_T *pList,NODE_T *pPrev,NODE_T *pNode);

NODE_T *list_next(NODE_T *pNode);
NODE_T *list_last(LIST_T *pList);


NODE_T *list_nth(LIST_T *pList,int nodenum);

NODE_T *list_nstep(NODE_T *pNode, int nStep);
NODE_T *list_previous(NODE_T *pNode);
void list_free(LIST_T *pList);

/* 列表静态初始化 */
#define LIST_INITIALIZER { .node.next = NULL, .node.previous = NULL, .count = 0, }

#define LIST_FOR_EACH(type,var,list) for(var = (type*)list_first(list); (var) != NULL; var=(type*)list_next(&(var)->node))

/*从后往前*/
#define LIST_FOR_EACH_RESERVER(type,var,list) for(var = (type*)list_last(list); (var) != NULL; var=(type*)list_previous(&(var)->node))






#endif
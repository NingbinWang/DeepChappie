#include "sys_list.h"
#include <stdlib.h>
#include <stdio.h>
#include "sys_mem.h"

#define HEAD	node.next		/* first node in list */
#define TAIL	node.previous		/* last node in list */



void list_init(LIST_T *pList)
{
	pList->HEAD  = NULL;
	pList->TAIL  = NULL;
	pList->count = 0;
}


void list_add(LIST_T *pList, NODE_T *pNode)
{
	list_insert (pList, pList->TAIL, pNode);
}


void list_concat(LIST_T *pDstList,LIST_T *pAddList)
{
	if (pAddList->count == 0)		/* nothing to do if AddList is empty */
		return;

	if (pDstList->count == 0)
		*pDstList = *pAddList;
	else
	{
		/* both lists non-empty; update DstList pointers */

		pDstList->TAIL->next     = pAddList->HEAD;
		pAddList->HEAD->previous = pDstList->TAIL;
		pDstList->TAIL           = pAddList->TAIL;

		pDstList->count += pAddList->count;
	}

	/* make AddList empty */

	list_init (pAddList);
}


int list_count(LIST_T *pList)
{
	return (pList->count);
}


void list_delete(LIST_T *pList,NODE_T *pNode)
{
	/* modified by Hu Jiexun, Jan22,2009. */
	int firstNode = 0, lastNode = 0;

	if (pNode->previous == NULL)
	{
		if (pList->HEAD != pNode)
		{
			printf("list_Delete: HEAD = %p, pNode = %p, it's a dummy node!\n", pList->HEAD, pNode);
			return;
		}
		
		firstNode = 1;
	}
	else
	{
		if (pNode->previous->next != pNode)
		{
			printf("list_Delete: previous->next = %p, pNode = %p, it's a dummy node!\n", pNode->previous->next, pNode);
			return;
		}
		
		firstNode = 0;
	}

	if (pNode->next == NULL)
	{
		if (pList->TAIL != pNode)
		{
			printf("list_Delete: TAIL = %p, pNode = %p, it's a dummy node!\n", pList->TAIL, pNode);
			return;
		}
		
		lastNode = 1;
	}
	else
	{
		if (pNode->next->previous != pNode)
		{
			printf("list_Delete: next->previous = %p, pNode = %p, it's a dummy node!\n", pNode->next->previous, pNode);
			return;
		}

		lastNode = 0;
	}

	if (1 == firstNode)
	{
		pList->HEAD = pNode->next;
	}
	else
	{
		pNode->previous->next = pNode->next;
	}

	if (1 == lastNode)
	{
		pList->TAIL = pNode->previous;
	}
	else
	{
		pNode->next->previous = pNode->previous;
	}

	/* update node count */

	pList->count--;
}


void list_extract(LIST_T *pSrcList,NODE_T *pStartNode, NODE_T *pEndNode,LIST_T *pDstList)
{
	int i;
	NODE_T *pNode;

	/* fix pointers in original list */

	if (pStartNode->previous == NULL)
		pSrcList->HEAD = pEndNode->next;
	else
		pStartNode->previous->next = pEndNode->next;

	if (pEndNode->next == NULL)
		pSrcList->TAIL = pStartNode->previous;
	else
		pEndNode->next->previous = pStartNode->previous;


	/* fix pointers in extracted list */

	pDstList->HEAD = pStartNode;
	pDstList->TAIL = pEndNode;

	pStartNode->previous = NULL;
	pEndNode->next       = NULL;


	/* count number of nodes in extracted list and update counts in lists */

	i = 0;

	for (pNode = pStartNode; pNode != NULL; pNode = pNode->next)
		i++;

	pSrcList->count -= i;
	pDstList->count = i;
}


NODE_T *list_first(LIST_T *pList)
{
	return (pList->HEAD);
}

NODE_T *list_get(LIST_T *pList)
{
	NODE_T *pNode = pList->HEAD;

	if (pNode != NULL)                      /* is list empty? */
	{
		pList->HEAD = pNode->next;          /* make next node be 1st */

		if (pNode->next == NULL)            /* is there any next node? */
			pList->TAIL = NULL;             /*   no - list is empty */
		else
			pNode->next->previous = NULL;   /*   yes - make it 1st node */

		pList->count--;                     /* update node count */
	}

	return (pNode);
}


void list_insert(LIST_T *pList,NODE_T *pPrev,NODE_T *pNode)
{
	NODE_T *pNext;

	if (pPrev == NULL)
	{				/* new node is to be first in list */
		pNext = pList->HEAD;
		pList->HEAD = pNode;
	}
	else
	{				/* make prev node point fwd to new */
		pNext = pPrev->next;
		pPrev->next = pNode;
	}

	if (pNext == NULL)
		pList->TAIL = pNode;		/* new node is to be last in list */
	else
		pNext->previous = pNode;	/* make next node point back to new */


	/* set pointers in new node, and update node count */

	pNode->next = pNext;
	pNode->previous	= pPrev;

	pList->count++;
}


NODE_T *list_last(LIST_T *pList)
{
	return (pList->TAIL);
}


NODE_T *list_next(NODE_T *pNode)
{
	return (pNode->next);
}


NODE_T *list_nth(LIST_T *pList,int nodenum)
{
	NODE_T *pNode;

	/* verify node number is in list */

	if ((nodenum < 1) || (nodenum > pList->count))
		return (NULL);


	/* if nodenum is less than half way, look forward from beginning;
	   otherwise look back from end */

	if (nodenum < (pList->count >> 1))
	{
		pNode = pList->HEAD;

		while (--nodenum > 0)
			pNode = pNode->next;
	}

	else
	{
		nodenum -= pList->count;
		pNode = pList->TAIL;

		while (nodenum++ < 0)
			pNode = pNode->previous;
	}

	return (pNode);
}


NODE_T *list_previous(NODE_T *pNode)
{
	return (pNode->previous);
}


NODE_T *list_nstep(NODE_T *pNode, int nStep)
{
	int i;

	for (i = 0; i < abs (nStep); i++)
	{
		if (nStep < 0)
			pNode = pNode->previous;
		else if (nStep > 0)
			pNode = pNode->next;
		if (pNode == NULL)
			break;
	}
	return (pNode);
}



int list_find(LIST_T *pList,NODE_T *pNode)
{

	NODE_T *pNextNode;
	int index = 1;

	pNextNode = list_first (pList);

	while ((pNextNode != NULL) && (pNextNode != pNode))
	{
		index++;
		pNextNode = list_next (pNextNode);
	}

	if (pNextNode == NULL)
		return (-1);
	else
		return (index);
}



void list_free(LIST_T *pList)
{
	NODE_T *p1, *p2;

	if (pList->count > 0)
	{
		p1 = pList->HEAD;
		while (p1 != NULL)
		{
			p2 = p1->next;
			sys_mem_free ((char *)p1);
			p1 = p2;
		}
		pList->count = 0;
		pList->HEAD = pList->TAIL = NULL;
	}
}


void list_free_ext(LIST_T *pList, void (* sys_free)(void *p))
{
	NODE_T *p1, *p2;

	if (pList->count > 0)
	{
		p1 = pList->HEAD;
		while (p1 != NULL)
		{
			p2 = p1->next;
			sys_free ((char *)p1);
			p1 = p2;
		}
		pList->count = 0;
		pList->HEAD = pList->TAIL = NULL;
	}
}


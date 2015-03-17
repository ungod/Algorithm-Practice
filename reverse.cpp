#include "iostream"
using namespace std;

//
//链表反转（非递归和递归）
//2013.4.16
//

struct node
{
	int val;
	node* next;
};

struct plist
{
	node* head;
	node* back;

	void init()
	{
		head = new node;
		head->val = 0;
		head->next = NULL;

		back = head;
	}

	void push(int val)
	{
		back->next = new node;

		back = back->next;
		back->val = val;
		back->next = NULL;
	}

	void printList()
	{
		node* cur = head->next;
		while (cur)
		{
			printf("%d \n", cur->val);
			cur = cur->next;
		}
	}

	void reverse()
	{
		node* cur = head->next;
		node* nxt = NULL;
		node* lst = NULL;

		while (cur)
		{
			nxt = cur->next;
			cur->next = lst;

			lst = cur;
			cur = nxt;
		}
		head->next = lst;
	}

	void reverseRecur()
	{
		head->next = recur(head->next, NULL);
	}

	node* recur(node* cur, node* lst)
	{
		if (cur != NULL)
		{
			node* nxt = cur->next;
			cur->next = lst;

			return recur(nxt, cur);
		}
		return lst;
	}
};

void main()
{
	plist* pl = new plist;
	pl->init();

	for (int i=0; i<20; i++)
	{
		pl->push(i);
	}
	pl->reverseRecur();
	pl->printList();

	system("pause");
}
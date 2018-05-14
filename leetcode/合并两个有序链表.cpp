/**
* Definition for singly-linked list.
* struct ListNode {
*     int val;
*     ListNode *next;
*     ListNode(int x) : val(x), next(NULL) {}
* };
*/
class Solution {
public:
	void insert(ListNode **p, int v)
	{
		ListNode*temp = new ListNode(v);
		(*p)->next = temp;
		(*p) = temp;
	}
	ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
		ListNode *result = new ListNode(-10000000);
		ListNode *p = result;
		ListNode *temp1 = l1; ListNode*temp2 = l2;
		while (temp1&&temp2)
		{
			if (temp1->val < temp2->val)
			{
				insert(&p, temp1->val);
				temp1 = temp1->next;
			}
			else
			{
				insert(&p, temp2->val);
				temp2 = temp2->next;
			}
		}
		if (temp1)
		{
			p->next = temp1;
			//while (temp1)
			//{
			//insert(&p, temp1->val);
			//temp1 = temp1->next;

			//	}
		}
		else if (temp2)
		{
			p->next = temp2;
			//while (temp2)
			//{
			//	insert(&p, temp2->val);
			//	temp2 = temp2->next;
			//}
		}
		result = result->next;
		return result;
	}
};
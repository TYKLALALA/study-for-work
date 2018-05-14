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
	void insert(int v, ListNode* l2, ListNode** p)
	{
		ListNode* temp = new ListNode(v);
		(*p)->next = temp;
		(*p) = temp;
	}
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		int sum1;
		int sum2 = 0;
		int count = 1;
		ListNode* result = new ListNode(0);
		ListNode* p = result;
		ListNode* temp1 = l1;
		ListNode* temp2 = l2;
		while (temp1&&temp2)
		{

			if (count == 1)
			{
				sum1 = temp1->val + temp2->val + sum2;


				if (sum1<10)
				{
					result->val = sum1;
					sum2 = 0;
				}
				else
				{
					result->val = sum1 - 10;
					sum2 = 1;
				}
				count++;
				temp1 = temp1->next;
				temp2 = temp2->next;
				continue;
			}
			sum1 = temp1->val + temp2->val + sum2;
			if (sum1<10)
			{
				insert(sum1, result, &p);
				sum2 = 0;
			}
			else
			{
				insert(sum1 - 10, result, &p);
				sum2 = 1;

			}
			temp1 = temp1->next;
			temp2 = temp2->next;
		}

		if (temp1&&sum2 == 0)
		{
			p->next = temp1;
		}
		else if (temp2&&sum2 == 0)
		{
			p->next = temp2;
		}
		else if (temp1&&sum2 == 1)
		{
			while (temp1)
			{
				sum1 = temp1->val + sum2;
				if (sum1 >= 10)
				{
					insert(sum1 - 10, result, &p);
					sum2 = 1;
				}
				else
				{
					insert(sum1, result, &p);
					sum2 = 0;

				}
				temp1 = temp1->next;
			}
			if (sum2 == 1)
				insert(1, result, &p);
		}
		else if (temp2&&sum2 == 1)
		{
			while (temp2)
			{
				sum1 = temp2->val + sum2;
				if (sum1 >= 10)
				{
					insert(sum1 - 10, result, &p);
					sum2 = 1;
				}
				else
				{
					insert(sum1, result, &p);
					sum2 = 0;

				}
				temp2 = temp2->next;
			}
			if (sum2 == 1)
				insert(1, result, &p);
		}
		else if (sum2 == 1)
			insert(1, result, &p);
		return result;
	}
};
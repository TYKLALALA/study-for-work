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
	int getlen(ListNode* head, ListNode** head1)
	{
		int i = 0;
		while (head)
		{
			ListNode* temp1 = new  ListNode(head->val);
			(*head1)->next = temp1;
			(*head1) = (*head1)->next;
			i++;
			head = head->next;
		}
		return i;
	}
	ListNode* removeNthFromEnd(ListNode* head, int n) {

		ListNode* head2 = new ListNode(0);
		ListNode* head1 = head2;
		ListNode* temp = head1;
		int len = getlen(head, &head1) - n;
		while (len>0)
		{
			temp = temp->next;
			len--;
		}
		if (!temp->next)
			temp = NULL;
		else if (temp->next&&temp->next->next)
			temp->next = temp->next->next;
		else if (temp->next&&!temp->next->next)
			temp->next = NULL;
		return head2->next;
	}
};
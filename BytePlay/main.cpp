#include <iostream>
using std::cout;
using std::endl;


struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode *reverseKGroup(ListNode *head, int k) {
        if (head == nullptr) return nullptr;
        if (k <= 0) return nullptr;
        if (k==1) return head;
        
        ListNode *curr = head;
        ListNode *tail = head;
        while (curr){
            for (int i = 0; i < k-1 && tail; ++i){
                tail = tail->next;
            }
            if (tail == nullptr) return head;
            //now curr is at start and tail at end of my first list to reverse
            reverseList(&head, &curr, &tail, k);
            curr = tail != nullptr ? tail->next : nullptr;
            tail = curr;
        }
        
        return head;
    }
    void display(ListNode *head){
        cout << endl;
        while(head){
            cout << head->val << " ";
            head = head->next;
        }
        cout << endl;
    }
private:
    void reverseList(ListNode **head, ListNode **start, ListNode **end, int k){
        //swap start with end and reverse sub list k-1
        if (*start == *end) return;
        if (k <= 1) return;
        if (*start == nullptr || *end == nullptr) return;
        ListNode *test = *head;
        while (test!=*start){
            if (test == *end) {
                return; //end before start
            }
            test = test->next;
        }
        
        ListNode *s1 = *head;
        ListNode *e1 = *head;
        if (*start != *head){
            while (s1->next!= *start){
                s1 = s1->next;
            }
        }
        ListNode *tempS = (*start)->next;
        
        while(e1->next != *end){
            e1 = e1->next;
        }
        ListNode * temp;
        if (*start == *head){
            if (e1 != *head){
                temp = (*head)->next;
                e1->next = *head;
                (*head)->next = (*end)->next;
                *head = *end;
                (*head)->next = temp;
                *end = e1->next;
            }
            else {
                temp = *head;
                //e1->next = head;
                (*head)->next = (*end)->next;
                (*head) = (*end);
                (*head)->next = temp;
                *end = temp;
            }
            
        }
        else {
            temp = (*start);
            e1->next = *start;
            (*start)->next = (*end)->next;
            s1->next = *end;
            (*end)->next = temp;
            *end = (*start)->next;
        }
        
        display(*head);
        reverseList(head, &tempS, &e1, k-2);
    }
};

int main(){
    ListNode * node1 = new ListNode(1);
    ListNode * node2 = new ListNode(2);
    ListNode * node3 = new ListNode(3);
    ListNode * node4 = new ListNode(4);
    ListNode * node5 = new ListNode(5);
    ListNode * node6 = new ListNode(6);
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;
    node5->next = node6;
    Solution test;
    test.display(node1);
    test.reverseKGroup(node1, 6);
    
    return 0;
}
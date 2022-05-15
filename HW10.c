/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20			// Node*가 들어가는 stack 구조체 (전역)
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();							// pop()함수로 나오는건 가장 마지막에 들어갔던 Node*변수
void push(Node* aNode);					// push()로 Node*변수를 stack에 넣는다.

/* for queue */
#define MAX_QUEUE_SIZE		20			// Node*가 들어가는 queue 구조체(전역)
Node* queue[MAX_QUEUE_SIZE];			// Queue의 크기를 만들어 준다.
int front = -1;
int rear = -1;

Node* deQueue();						// dequeue()로 나오는건 맨처음으로 들어갔던 queue
void enQueue(Node* aNode);				// enqueue()로 Node*변수를 queue에 넣는다.


int initializeBST(Node** h);			

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printStack();

int main()
{
	char command;
	int key;
	Node* head = NULL;

	printf("[----- [이정인] [2019038015] -----]\n");


	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);		// 사용하기전 초기화
			break;
		case 'q': case 'Q':
			freeBST(head);				// BST에 할당된 메모리 모두초기화
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);			// 노드 생성하기
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);		// 노드 삭제하기
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);		// 재귀호출을 이용한 중위순회
			break;
		case 't': case 'T':
			iterativeInorder(head->left);		// 순환호출을 이용한 중위순회
			break;

		case 'l': case 'L':
			levelOrder(head->left);				// 트리를 레벨 순서대로 순회하는 방식 pre,in,post와는 다른 방식이다.
			break;

		case 'p': case 'P':
			printStack();						// Stack에 들어있는 노드 출력하기
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) 			// 초기화 작업
{

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);					// headNode 초기화

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));	// NULL로 되어있는 headNode의 값에 메모리를 할당해 준다.
	(*h)->left = NULL;	/* root */		// 그리고 메모리가 존재하는 headNode에 값을 넣어준다. left에는 root가 되는 노드를 right는 headNode를 가르켜 headNode가 root노드만 가르키게 만든다.
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;							// stack에 시작값 지정

	front = rear = -1;					// queue에 시작값 지정

	return 1;
}



void recursiveInorder(Node* ptr)			// 매개변수로 root노드의 주소가 들어온다.
{
	if(ptr) 								// root가 NULL이 아니면
	{
		recursiveInorder(ptr->left);		// 중위순회이기 때문에 왼쪽 먼저 들어가고
		printf(" [%d] ", ptr->key);			// 왼쪽출력 -> 부모출력 -> 오른쪽으로 들어간다.
		recursiveInorder(ptr->right);		// 맨 아래 서브트리의 오른쪽 출력
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)			// 매개변수로 root노드의 주소값이 들어온다.
{
	for(;;)									// 무한루프
	{
		for(; node; node = node->left)		// node가 false가 나올 때 까지 stack에 계속 넣는다. + 왼쪽 노드로 계속 이동
			push(node);
		node = pop();						// 맨 마지막 node를 pop한다.

		if(!node) break;					// if(!node) 해석 :  node는 Node* 변수이다. 즉 !node란 Node*이 아니라는 뜻이고 --> !node == NULL로 볼 수 있다.
		printf(" [%d] ", node->key);		// 맨 마지막 node의 부모노드를 pop한다.

		node = node->right;					// 그리고  오른쪽 서브트리로 간다. -> 그럼 오른쪽 서브트리에서 왼쪽으로 가는 작업을 반복함..
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)						// level 0 -> level 1 ->level 2 ... 이렇게 출력한다.
{
	// int front = rear = -1;

	if(!ptr) return;							// ptr이 NULL 일때, 종료

	enQueue(ptr);								// ptr의 enqueue작업

	for(;;)										// 무한루프
	{
		ptr = deQueue();						// dequeue로 Node 1개 출력
		if(ptr) 								// ptr !=NULL 이면
		{
			printf(" [%d] ", ptr->key);			// 일단 값출력 (처음으로는 level 0 <root>가 나온다.)

			if(ptr->left)						// ptr->left이 존재하면 
				enQueue(ptr->left);				// 왼쪽부터 enqueue	(처음으로 들어가면 level 1의 왼쪽이 enqueue)
			if(ptr->right)						// ptr->right이 존재하면 
				enQueue(ptr->right);			// 오른쪽 enqueue   (level 1의 왼쪽노드 다음으로 오른쪽 노드 enqueue)
												// Queue 특성상  level별로 출력하게 된다.
		}
		else									// 없으면 ==> 무한루프 탈출
			break;

	}

}


int insert(Node* head, int key)		// 노드 생성하기
{
	Node* newNode = (Node*)malloc(sizeof(Node));	
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) 		// 만약 headNode만 존재할 때 (root노드 없음)
	{
		head->left = newNode;		// 그냥 바로 밑에 붙인다. (root노드로 만든다.)
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;			// headNode밑에 노드가 존재할 때 root노드를 복사한 임시의 ptr노드를 생성

	Node* parentNode = NULL;		// 새로운 노드를 넣을 때 필요한 parentNode를 생성
	while(ptr != NULL)				// root 노드가 있을 때
	{

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;	// 이미 존재하는 key값을 가진 노드이므로 중복해서 만들어줄 필요 없음

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;				// 부모노드에 값을 넣는다. +  ptr이 이동하면서 부모노드도 같이 이동한다.

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)				// root의 값보다 크면
			ptr = ptr->right;			// 오른쪽 subtree로 ptr 이동하면서 계속 검색
		else
			ptr = ptr->left;			// 작으면 왼쪽 subtree로 ptr 이동하면서 계속 검색
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)			// 새로운 노드가 들어갈 공간을 찾으면 부모노드의 값과 비교해서
		parentNode->left = newNode;		// 부모노드값이 크면 왼쪽sub로 
	else
		parentNode->right = newNode;	// 부모노드보다 크면 오른쪽 sub로 넣는다.
	return 1;
}


int deleteNode(Node* head, int key)				// 노드 삭제하기
{
	if (head == NULL) 							// headNode 조차 없으면 삭제할 수 없음
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) 					// 또한 root노드가 없어도 삭제할 수 없음
	{
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left is the root */
	Node* root = head->left;					// root노드를 생성

	Node* parent = NULL;						// 부모노드와 임시의 ptr노드도 생성
	Node* ptr = root;

	while((ptr != NULL)&&(ptr->key != key)) 	// ptr이 존재하고, ptr의 key와 입력한 key가 다르면 ptr을 이동 시켜야 할 때
	{
		if(ptr->key != key) 					// key값이 서로 다를 때
		{

			parent = ptr;						// 현위치 저장 (parent노드 사용)

			if(ptr->key > key)					// key가 작으면 오른쪽으로
				ptr = ptr->left;
			else
				ptr = ptr->right;				// 크면 된쪽으로
		}
	}

	/* there is no node for the key */
	if(ptr == NULL)									// while문의 첫번째 조건에서 false일 때
	{
		printf("No node for key [%d]\n ", key);		// 더이상 찾을 노드도 없고, key값을 가진 노드 발견 실패
		return -1;
	}

		/* key값을 가진 노드 발견!! (삭제할 노드 발견) */

	if(ptr->left == NULL && ptr->right == NULL)		// 경우 1) 삭제할 노드가 leaf node 일때
	{
		if(parent != NULL) 				// root노드가 아니라는 걸 확인
		{ /* parent exists, parent's left and right links are adjusted */
			if(parent->left == ptr)		// 해당하는 노드를 삭제한다.
				parent->left = NULL;
			else
				parent->right = NULL;
		} 
		
		else 	// 찾은 ptr노드가 root노드일 때 초기화 해서 생성한 parent노드가 NULL이기 때문
			head->left = NULL;		// root노드 삭제

		free(ptr);		// 할일 다한 임시로 만든 ptr 노드 삭제
		return 1;
	}

	if ((ptr->left == NULL || ptr->right == NULL))		// 삭제할 노드 밑에 자식노드가 1개 있을 때
	{
		Node* child;						// 임시의 자식노드 생성
		if (ptr->left != NULL)				// 왼쪽 서브트리에 자식이 있을 때
			child = ptr->left;
		else								// 오른쪽 서브트리에 자식이 있을 때
			child = ptr->right;

		if(parent != NULL)					// 삭제할 노드가 root노드가 아니라면
		{
			if(parent->left == ptr)			// 삭제할 노드가 부모의 왼쪽이면 ptr의 자식을 부모의 왼쪽과 연결시켜 ptr을 tree에서 분리
				parent->left = child;		
			else							// 오른쪽도 마찬가지
				parent->right = child;
		} 
		
		else 								// 삭제할 노드가 root노드라면
		{
			/* parent is null, which means the node to be deleted is the root
			 * and the root has one child. Therefore, the child should be the root
			 */
			root = child;					// 그냥 자식노드를 root노드로 올려버린다.
		}

		free(ptr);			// 할일 다한 ptr노드 메모리 해제
		return 1;
	}

	/**
	 * 경우 3) 자식노드가 2개 모두 있는경우
	 *
	 * 이럴땐 왼쪽 subtree에서 가장 큰값을 찾아서 삭제한 노드 대신 넣거나
	 * 		  오른쪽 subtree에서 가장 작은 값을 찾아서 삭제한 노드 대신 넣거나
	 * 	->둘중 하나를 해야한다.
	 *
	 * 여기서는 오른쪽 subtree에서 가장 작은 값을 찾는 방법을 선택한다.
	 *
	 */

	Node* candidate;			// 삭제할 노드 대신 넣을 노드
	parent = ptr;				// 부모노드 잡아주고


	candidate = ptr->right;		// 오른쪽 서브트리에서 가장 작은 값으로 이동

	/* the smallest node is left deepest node in the right subtree of the ptr */
	while(candidate->left != NULL)		// 오른쪽 서브트리에서 가장 작은 값은 가장 왼쪽에 있는 노드이다.
	{
		parent = candidate;				// 계속 이동
		candidate = candidate->left;
	}

	/* the candidate node is the right node which has to be deleted.
	 * note that candidate's left is null
	 */
	if (parent->right == candidate)			// 부모 노드 바로 오른쪽에 있는게 오른쪽 서브트리에서 가장 작은 노드 일때
		parent->right = candidate->right;	// 부모의 오른쪽에 삭제할 노드 오른쪽 노드를 이어준다.
	else									// 부모노드와 삭제할 노드의 레벨 차이가 2이상 날때
		parent->left = candidate->right;	// 부모의 왼쪽에 이어준다.

	/* instead of removing ptr, we just change the key of ptr
	 * with the key of candidate node and remove the candidate node
	 */

	ptr->key = candidate->key;				// key를 바꿈으로써 노드가 가지고 있는 값을 바꾼다.

	free(candidate);						// 삭제할 노드 삭제
	return 1;
}


void freeNode(Node* ptr)		// 하위 노드 삭제
{
	if(ptr) 					// if(ptr) == if(!=ptr)이 암묵적으로 성립한다.
	{
		freeNode(ptr->left);	// ptr에 연결되어 있는 노드를 제거한다.
		freeNode(ptr->right);
		free(ptr);				// 연결 끊은 후 ptr제거
	}
}

int freeBST(Node* head)			// headNode를 없애준다.
{

	printf("Free BST...\n");

	if(head->left == head)		// headNode만 있을 때
	{
		free(head);				// headNode만 없애준다.
		return 1;

	}

	Node* p = head->left;		// headNode 밑에 하위 노드가 있을 때

	freeNode(p);				// 하위노드 삭제

	free(head);					// 헤더노드 삭제
	return 1;
}


Node* pop()						// stack에서 빼기
{
	if (top < 0) 				// top이 음수이면 stack에 쌓은 값이 없다.
		return NULL;	

	return stack[top--];		// 후위연산이므로, stack[top]을 하고 top-1을 연산한다.
}

void push(Node* aNode)
{
	stack[++top] = aNode;		//  초기의 top은 -1로 되어있으므로 먼저 +1하고 stack을 쌓는다.
}


void printStack()				// stack 출력하기
{
	int i = 0;
	printf("--- stack ---\n");
	while(i <= top)				// 여기서 top은 여때까지 쌓아진 노드 갯수-1개 이다. (왜냐면 0부터 시작했으니까)
	{	
		printf("stack[%d] = %d\n", i, stack[i]->key);	
	}
}


Node* deQueue()
{
	if (front == rear) 						// 초기 설정해둔 front = rear = -1 이거나 Queue가 비어있거나
	{
		// printf("\n....Now Queue is empty!!\n" );
		return NULL;						// dequeue 작업 종료
	}

	front = (front + 1) % MAX_QUEUE_SIZE;	// -1로 초기화 했던 front의 값을 0으로 만들어 줘야 데이터가 들어있는 queue범위로 front가 들어간다.
	return queue[front];					// front가 가리키는 node* 리턴

}

void enQueue(Node* aNode)					// Queue구조에 넣는 함수
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;		// rear가 가질수 있는 범위는 0~Queue의 마지막 인덱스 까지이다.+  초기에 front와 rear를 같게 설정했는데, 여기서 부터 달라짐
	if (front == rear) 						// front와 rear가 같아지면 rear이 한바퀴 돌고 온것 == 꽉찼다.
	{
		// printf("\n....Now Queue is full!!\n");
		return;								// rear가 front와 같은지 확인하고 틀리면 enqueue를 하는것이다. (순서 중요!!)
	}

	queue[rear] = aNode;					// rear에 enqueue작업을 한다.
}

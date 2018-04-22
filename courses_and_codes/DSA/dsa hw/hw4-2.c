2.1.(a)
assume that the quicksort is not modified,
in other words, always choose the leftmost one as pivot
instead of pick one randomly. And it aims to sort from small to large.
QUICKSORT(A, p, r)
if p<r
	q=PARTITION(A, p, r)
	QUICKSORT(A, p, q-1)
	QUICKSORT(A, q+1, r)

example of size N:
[N N-1 N-2 N-3 ..... 3 2 1]
each time it calls QUICKSORT,q=r
so QUICKSORT will be called N times,
each time cost O(N) to PARTITION(just throwing all elements to the left side)
->running time = O(N^2)
< actually: N+(N-1)+(N-2)+....+1 = N*(N+1)/2 -> O(N^2) >

2.1.(b)

Insertion-Sort(A)
for j = 2 to A.length
	key = A[j] // Insert A[j] into the sorted sequence A[1 . . j − 1].
	i=j−1 
	while(i>0 and A[i]>key)
		A[i + 1] = A[i]
		i=i−1 
	A[i+1] = key

Merge sort is non-adaptive and has running time O(NlogN)
no matter what kind of input is given.
However, Insertion Sort is adaptive, so if we give a input
that is already sorted, that is, A[i] always < key
it will lead to O(N) running time, since it never enters the while loop.
example of size N:[1 2 3 4 ... N-1 N]

2.2
use Counting Sort:
int C[];
void CountingSort (int A[], int n, int B[], int k) { 
	int i, j;
	for (i=0;i<=k;i++) C[i]=0;			//O(k)
	for (j=1;j<=n;j++) C[A[j]]++;		//O(n)
	for (i=1;i<=k;i++) C[i]+=C[i-1];	//O(k)
	for (j=n;j>=1;j--) { 				//O(n)
		B[C[A[j]]]=A[j];
		C[A[j]]--;
	}						// running time: theta(n+k)									
int answer(int a,int b){
	if(a==0)	return C[b];
	return C[b]-C[a-1];	//O(1)
}

2.3.(a)
list (501, 939, 1137, 2345, 666, 34, 218)
C[10] // from 0 to 9
個位數：(501, 34, 2345, 666, 1137, 218, 939)
		C=[0,1,0,0,1,1,1,1,1,1]
十位數：(501, 218, 34, 1137, 939, 2345, 666)
		C=[1,1,0,3,1,0,1,0,0,0]
百位數：(34, 1137, 218, 2345, 501, 666, 939)
		C=[1,1,1,1,0,1,1,0,0,1]
千位數：(34, 218, 501, 666, 939, 1137, 2345)
		C=[5,1,1,0,0,0,0,0,0,0]

2.3.(b)
implement RadixSort with CountingSort:	O(d*(n+k)) ,where d=位數 when r=10
only using CountingSort:				O(n+k)
in this case,
(n,k,r)=(7,10,10) for implement RadixSort with CountingSort, where d=4,k from 0 to 9
		(7,10000,(dont care)) for using CountingSort only, where k from 0 to 9999
-> 4*(7+10) = 68 < 10007 = 7+10000

2.4
sequence [20, 29, 57, 37, 36, 50, 59]
LSD RadixSort using MergeSort
個位數：[20, 50, 26, 57, 37, 29, 59]
十位數：[20, 26, 29, 37, 50, 57, 59] (done)

LSD RadixSort using HeapSort
個位數：[20, 36, 50, 37, 29, 57, 59]
十位數：[20, 29, 50, 37, 36, 57, 59] (done)

結果不一樣，因為 heap sort 對每個subtree，不會比較children之間的大小，只取最大的放到root
而 merge sort 是 non-adaptive，所以兩者的結果不一樣，也可預期與使用 counting sort 來實作的結果會不同。

2.5
bogosort





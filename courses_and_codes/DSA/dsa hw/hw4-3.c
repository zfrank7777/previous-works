3.1
(1,4,6), (1,4,7), (1,5,6), (1,4,7),
(2,4,6), (2,4,7), (2,5,6), (2,4,7),
(3,4,6), (3,4,7), (3,5,6), (3,4,7)


3.2
let C(a,b)= a!/(b!*(a-b)!) (combination)
use disjoint set.
find_tuple_number(E[],N){
	//initialize:make N set, each contain a node
	int number_of_set=N;
	int size[N];
	for(i=1;i<=N;i++){
		size[i]=1;
		Make_Set(i);//make set initialization: head(group_num)=self
	}
	//let the nodes connected by black edge be one set
	for e in E:
		if(e.color=='b') {
			Union(nodeA, nodeB);
			size[nodeA->head]+=size[nodeB->head];
			size[nodeB]=0;
			number_of_set--;
			delete(e);
		}

	//build another graph, vertex = set, edge = red edge
	// 因為原本是棵樹，新作的圖也是樹，沒有重複連接的問題(各個set之間不會有兩條路)
	// 所以只要任選三個vertice，每個vertex選一個node，即是一組tuple
	rename each group:
		new group_num start from 1 to number_of_set
	int sum=0;
	for(i=3;i<=number_of_set;i++)
		for (j=2;j<i;j++)
			for (k=1;k<j;k++)
				sum+=size[i]*size[j]*size[k];
	return sum;
}
in the given example,new graph:
(group1)--(group2)--(group3)
group1:(1,2,3),size=3
group2:(4,5),size=2
group3:(6,7),size=2
sum=3*2*2=12.





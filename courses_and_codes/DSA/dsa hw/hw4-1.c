1.1.(a)
struct Node
{
	int data;
	struct Node* children[]
};
weight[N]={};	// indexed from 1 to N
//record weight in array
int find_weight(Node* root){
	if(root==NULL) return 0;
	weight[root->data]= 1;
	if(is_leaf(root)) return 1;
	for each_child in children:
		weight[root->data]+=find_weight(each_child);
	return weight[root->data];
}
1.1.(b)
weight[N];	// indexed from 1 to N

int find_centroid(Node* root){
	int all_weight=find_weight(root);
	Node* centroid=root; 
	Node* temp=root;
	Node* largest_child;
	int temp_max = all_weight; // finding max tree weight
	while(all_weight-weight[temp] >= weight[temp]){ //離開 while的前個temp即為centroid
		centroid = temp;
		temp_max=0;
		// find and goto largest subtree
		for i in temp->children:
			if(weight[i->data]>temp_max){
				temp_max = weight[i->data];
				largest_child = i;
			}
			//有兩個同樣重的子樹->不會再往下走
			else if(weight[i->data]==temp_max) break;
		temp = largest_child;
	}
	return centroid->data;
}

1.2.(a)
struct Node
{
	int data;
	struct Node* children[]
};
height_array[N]={};	// indexed from 1 to N
//record weight in array
int find_height(Node* root){
	if(root==NULL) return -1;
	height_array[root->data]= 0;
	if(is_leaf(root)) return 0;
	int max_height=0;
	for each_child in children:
		if(find_height(each_child)>=max_height)
			max_height=1+find_height(each_child);
	height_array[root->data]= max_height;
	return max_height;
}

1.2.(b)
//filling height_array first
int only_to_call_function = find_height(root);
int find_diameter(Node* root){
	if(root==NULL) return -1;
	if(is_leaf(root)) return 0;
	int diameter=0;
	int first_max=0;
	int second_max=0;
	//find largest two height
	for i in children:
		int h=height_array[i];
		if(h>=first_max)
			second_max=first_max;
			first_max=h;
		else if(h>=second_max):
			second_max=h;
		//not necessarily go through root, so go recursion
		//但只需往最長的那條走就可以了
		//assume max=a,second_max=b
		// -> a+b > 2b-2
		// -> 保證其他路不會更長
		// 等同1.3.(a)的證明
		int child_path=find_diameter(first_max_child);
		if(child_path>diameter)
			diameter=child_path;
	diameter = (first_max+second_max>diameter)? (first_max+second_max):diameter;
	return diameter;
}

1.3.(a)
we can give recursive definition,start from root:
case(diameter path pass through root):
	choose largest height from two different subtree(diameter path=倒V型)
		-> include the farthest node from the root
case(diameter path does not pass through root):
	choose the subtree which has the largest height
		-> (called recursively) include the farthest node from the root
	<if not>
		let the largest height of the subtree you choose = h,
			the distance between root and the farthest root = h0 
		-> h0>h => h0+h > 2h 
		-> diameter will be h+h0+1
		-> diameter path will pass through root,which lead to a contradiction

1.3.(b)
section A:
	previous[u]=father
section B:
	midpoint=b
	for(i=0;i<distance[b]/2;i++)
		midpoint=previous[midpoint]
	return midpointø




struct Node *root;
void insert(char word[],int N){
	struct Node *temp=root;
	for(int i=0;i<N;i++){
		temp=temp->children[word[i]-'a'];
		temp->is_word++;
	}
}
void delete(char word[],int N){
	struct Node *temp=root;
	for(int i=0;i<N;i++){
		temp=temp->children[word[i]-'a'];
		if(temp->is_word>0) temp->is_word--;		
	}
}
int query(char word[], int N){
	struct Node *temp=root;	
	for(int i=0;i<N;i++){
		if(temp->children[word[i]-'a']->is_word==0)
			return 0;
		temp=temp->children[word[i]-'a'];
	}
	return 1;
}

struct Node *root;
//initialize: level=0,temp=root;
int is_leaf(struct Node *temp){
	int k=1;
	for(int i=0;i<26;i++)
		if(temp->children[i]->is_word!=0)
			k=0;
	return k;
}

int first_player_strategy(struct Node *temp,int level){
//when is_leaf and level is odd,first player win,return true
	if (is_leaf) 
		return (level%2);
//when level is odd,second player's turn
//if all choices lead to first player's win,return true
	if(level%2){ 
		int now=1;, 
		for(int i=0;i<26;i++){
			if(!first_player_strategy(temp->children[i],level+1))
				now=0;
			break;
		}		
		return now;
	}
//when level is even,first player's turn
//if any choice lead to first player's win,return true
	else{
		int now=0;
		for(int i=0;i<26;i++){
			if(first_player_strategy(temp->children[i],level+1))
				now=1;
			break;
		}	
		return now;
	}
}
int second_player_strategy(struct Node *root){
	int ans=1;						//if all choices lead to second player's win,return true
	for(int i=0;i<26;i++){
		if(!first_player_strategy(root->children[i],0))
			ans=0;
	}
	return ans;
}
int neither_has(struct Node *root){
	if(!first_player_strategy(root,0) && !second_player_strategy(root))
		return 1;
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_{
	int data;
	struct linked_ *next;
}LinkedList;
typedef struct{
	int *data;
	int start,end;
	int size;
}Queue;

typedef struct{
	char *fullName;
	LinkedList *Connections;
	int enabled,in_degree,fame,id;
}PersonNode;

PersonNode 	initPerson(char* fullName,int id);
LinkedList* initLinkedList(int dataP);
PersonNode* readFromFile(FILE* fp,int*);
PersonNode*	findPersonById(int id, PersonNode* persons, int sizeP);
Queue *createQueue(int size);
void 	addLL(int data, LinkedList *head);
void 	printPersons(PersonNode* persons,int sizem);
void 	printPersonsJustName(PersonNode* persons,int sizem);
void 	decreaseInfluencers(int M, PersonNode* persons, int sizeP);
void 	updateNeighborsBFS(int M, int index, PersonNode *persons, int sizeP);
void 	queuePush(int data,Queue *queue);
int 	queuePop(Queue *queue);
int 	**createRelationMatrix(PersonNode *persons,int sizeP);
void 	calculateIndegree(PersonNode *persons, int **rlMx, int sizeP);
void 	clearArr(int *arr,int size);
void 	composeArrays(int* arr1, int* arr2,int size);
void 	calculateFameR(int **mx,int *visited, int index,int size);
void 	calculateFame(PersonNode *persons,int sizeP);
void 	printMatrix(int **mx,int size);
void 	printInfluencer(PersonNode *persons,int sizeP,int X,int Y);
void 	normalMode(FILE *fp);
void 	detailMode(FILE *fp);

int main(){
	char fileName[20],*tmpName,*tmpSurname,tmpStr[100],*tmpNum;
	int tmpIndex,personSize, **relationMatrix,mode;
	FILE* fp;
	PersonNode *personArr;
	printf("FileName: ");
	scanf("%s", fileName);
	
	if((fp = fopen (fileName, "r")) == NULL){
		printf("\nDosya acilamadý!!!!");
		return -1;
	}
	while(1){
		printf("\n1 - Simple MODE\n2 - Detail MODE\n 0 - Exit\n(int):");
		scanf("%d",&mode);
		printf("\n");
		if(mode==1){
			normalMode(fp);
		}else if(mode == 2){
			detailMode(fp);
		}else if(mode == 0){
			printf("\nBye Bye! :D 20011068 Ahmet Akib GULTEKIN\n");
			fclose(fp);
			return 0;
		}else{
			printf("Wrong Input... Please type integer value..\n");
		}
		rewind(fp);
	}
	return 0;
}

void detailMode(FILE *fp){
	int personSize, **relationMatrix,M,X,Y;
	PersonNode *personArr;
	personArr = readFromFile(fp,&personSize);
	printf(" M X Y:");
	scanf("%d %d %d",&M,&X,&Y);
	
	relationMatrix = createRelationMatrix(personArr,personSize);
	
	calculateIndegree(personArr,relationMatrix,personSize);
	printf("\n		Start Status\n");
	printPersons(personArr,personSize);
	
	decreaseInfluencers(M,personArr,personSize);
	printf("\n		Decreased Status\n");
	printPersonsJustName(personArr,personSize);
	calculateFame(personArr,personSize);
	printf("\n		Influencers\n");
	printInfluencer(personArr,personSize,X,Y);
}

void normalMode(FILE *fp){
	int personSize, **relationMatrix,M,X,Y;
	PersonNode *personArr;
	personArr = readFromFile(fp,&personSize);
	printf(" M X Y:");
	scanf("%d %d %d",&M,&X,&Y);
	relationMatrix = createRelationMatrix(personArr,personSize);
	calculateIndegree(personArr,relationMatrix,personSize);
	
	decreaseInfluencers(M,personArr,personSize);
	calculateFame(personArr,personSize);
	printInfluencer(personArr,personSize,X,Y);
}

void printInfluencer(PersonNode *persons,int sizeP,int X,int Y){
	int i;
	PersonNode *tmpPerson;
	for(i = 0;i<sizeP;i++){
		if(persons[i].enabled == 1 && persons[i].in_degree >= X &&persons[i].fame >= Y){
			printf("%d: %s | in_degree:%d fame:%d\n",persons[i].id,persons[i].fullName,persons[i].in_degree,persons[i].fame);
		}
	}
}


void printMatrix(int **mx,int size){
	int i,j;
	for(i = 0;i<size;i++){
		for(j = 0;j<size;j++){
			printf("%d ", mx[i][j]);
		}
		printf("\n");
	}
}

void calculateFame(PersonNode *persons,int sizeP){
	int i,j,**mx,tmp;
	int *visited;
	PersonNode *tmpPerson;
	mx = createRelationMatrix(persons,sizeP);
	visited =(int*)calloc(sizeP,sizeof(int));
	printf("\n");
	printMatrix(mx,sizeP);

	for(i=0;i<sizeP;i++){
		visited[i] = 2;
		calculateFameR(mx,visited,i,sizeP);
		clearArr(visited,sizeP);
	}
	printf("\n");
	printMatrix(mx,sizeP);

	for(i=0;i<sizeP;i++){
		tmp=0;
		tmpPerson = findPersonById(i+1,persons,sizeP);
		if(tmpPerson->enabled ==1){
			for(j=0;j<sizeP;j++){
				tmp += (mx[i][j]>0);
			}
			tmp--;
		}
		tmpPerson->fame = tmp;
		
	}
}

void calculateFameR(int **mx,int *visited, int index,int size){
	int i;
	for(i = 0;i<size;i++){
		if(mx[index][i] == 1 && visited[i] == 0 && index !=i ){
			visited[i] = 1;
			calculateFameR(mx,visited,i,size);
			composeArrays(mx[index],mx[i],size);
			mx[index][i] = 2;
			mx[i][i] = 2;
		}	
	}
}

void composeArrays(int* arr1, int* arr2,int size){
	int i;
	for(i=0;i<size;i++){
		if(arr1[i]<arr2[i]){
			arr1[i] = arr2[i];
		}
	}
}
void calculateIndegree(PersonNode *persons, int **rlMx, int sizeP){
	int i,j,tmp;
	PersonNode *tmpPerson;
	for(i = 0;i<sizeP; i++ ){
		tmp = 0;
		for(j = 0;j<sizeP; j++ ){
			tmp += rlMx[i][j];
		}
		tmpPerson = findPersonById(i+1,persons,sizeP);
		tmpPerson->in_degree = tmp;
	}
}
void decreaseInfluencers(int M, PersonNode* persons, int sizeP){
	int i,j,tmp;
	PersonNode *tmpPerson;
	for(i = 0; i<sizeP; i++ ){
		if(persons[i].in_degree < M && persons[i].enabled == -1){
			persons[i].enabled = 0;
			updateNeighborsBFS(M,i,persons,sizeP);
		}
	}
	for(i = 0; i<sizeP; i++ ){
		if(persons[i].in_degree >= M){
			persons[i].enabled = 1;
		}
	}
}
int **createRelationMatrix(PersonNode *persons,int sizeP){
	int **mx,i;
	LinkedList *itr;
	PersonNode *tmpPerson;
	mx = (int**) malloc(sizeof(int*)*(sizeP));
	
	for(i = 0;i<=sizeP;i++){
		mx[i] = (int*) calloc((sizeP),sizeof(int));	
	}
	for(i = 0;i<sizeP;i++){
		for(itr = persons[i].Connections; itr->next!=NULL; itr = itr->next){
			tmpPerson = findPersonById(itr->data,persons,sizeP);
			mx[(itr->data)-1][(persons[i].id)-1] = 1;
		}
	}
	return mx;
}

void updateNeighborsBFS(int M, int index, PersonNode *persons, int sizeP){
	LinkedList* itr;
	PersonNode* tmpPerson;
	Queue *queue = createQueue(sizeP);
	
	for(itr = persons[index].Connections ; itr->next!=NULL ; itr = itr->next){
		tmpPerson = findPersonById(itr->data,persons,sizeP);
		tmpPerson->in_degree--;
		queuePush(itr->data,queue);
	}
	
	while(queue->end >=0){
		tmpPerson = findPersonById(queuePop(queue),persons,sizeP);
		if(tmpPerson->enabled == -1 &&tmpPerson->in_degree < M){
			tmpPerson->enabled = 0;	
			for(itr = tmpPerson->Connections;itr->next!=NULL;itr = itr->next){
				tmpPerson = findPersonById(itr->data,persons,sizeP);
				tmpPerson->in_degree--;
				queuePush(itr->data,queue);
			}	
		}
	}
	free(queue->data);
	free(queue);
}
Queue *createQueue(int size){
	Queue *tmp = (Queue*) malloc(sizeof(Queue));
	tmp->data = (int*) malloc(sizeof(int)*size);
	tmp->end = -1;
	tmp->start = -1;
	tmp->size = size;
	return tmp;
}
void queuePush(int dataP,Queue *queue){
	if(queue->end == queue->size-1){
		printf("queue size not enough!!!\n");
	}else{
		if(queue->start == -1){
			queue->start = 0;
		}
		queue->data[++(queue->end)] = dataP;
	}
}
int queuePop(Queue *queue){
	int dataP;
	if(queue->end == -1){
		return -1;
	}
	dataP = queue->data[(queue->start)++];
	if(queue->start > queue->end){
		queue->start = -1;
		queue->end 	 = -1;
	}
	return dataP;
}
PersonNode *findPersonById(int id, PersonNode* persons, int sizeP){
	int i;
	if(persons[id-1].id = id) return &persons[id-1];
	for(i = 0;i<sizeP;i++){
		if(persons[i].id = id) return &persons[i];
	}
	return NULL;
}
PersonNode initPerson(char* fullName,int id){
	PersonNode tmp;
	tmp.fullName = strdup(fullName);
	tmp.Connections = initLinkedList(-1);
	tmp.enabled = -1;
	tmp.id = id;
	tmp.fame = 0;
	tmp.in_degree = 0;
	return tmp;
}

LinkedList *initLinkedList(int dataP){
	LinkedList* tmp =(LinkedList*) malloc(sizeof(LinkedList));
	tmp->data = dataP;
	tmp->next = NULL;
	return tmp;
}

PersonNode *readFromFile(FILE* fp,int *personSize){
	char *tmpName,*tmpSurname,tmpStr[100],*tmpNum,*strhlpr;
	int tmpIndex,indexP;
	PersonNode *persons = (PersonNode*) malloc(sizeof(PersonNode)*200);
	while(fscanf(fp,"%d,%s",&tmpIndex,tmpStr)!=EOF){
		tmpName = strtok(tmpStr,",");
		tmpSurname = strtok(NULL,",");
		strhlpr = strdup(tmpName);
		strcat(strcat(strhlpr, " "),tmpSurname);
		persons[indexP] = initPerson(strhlpr,tmpIndex);
		
		if(fscanf(fp,"%s",tmpStr)!=EOF){
			tmpNum = strtok(tmpStr,",");
			persons[indexP].Connections->data = atoi(tmpNum);
			while(tmpNum != NULL){
				tmpNum = strtok(NULL,",");
				addLL(atoi(tmpNum),persons[indexP].Connections);
			}
		}
		indexP++;
	}
	*personSize = indexP;
	return persons;
}
void addLL(int data, LinkedList *head){
	LinkedList *itr = head;
	while(itr->next != NULL){
		itr = itr->next;
	}
	itr->next = initLinkedList(data);
}
void printPersons(PersonNode* persons,int sizem){
	int i;
	LinkedList *tmp;
	for(i = 0;i<sizem;i++){
		printf("%d: %s | in_degree:%d \n",persons[i].id,persons[i].fullName,persons[i].in_degree);
		tmp = persons[i].Connections;
		printf("FOLLOWS:");
		for(;tmp->next!=NULL;tmp = tmp->next){
			printf(" %d,",tmp->data);
		}
		printf("\n\n");
	}
}
void printPersonsJustName(PersonNode* persons,int sizem){
	int i;
	LinkedList *tmp;
	for(i = 0;i<sizem;i++){
		if(persons[i].enabled == 1){
			printf("%d: %s \n",persons[i].id,persons[i].fullName);
		}
	}
}
void clearArr(int *arr,int size){
	int i;
	for(i =0;i<size;i++){
		arr[i]=0;
	}
}


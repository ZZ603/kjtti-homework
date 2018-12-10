#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>

//��Ϸ�к���Ϊy������Ϊx 
//������Ϊsnake������ͷ�⣬ÿһ���ߵ���һ��Ϊsnake_nextx��snake_nexty 
//���ΪrestartΪ������Ϸ�ã���¼��ʼ״̬
//���ΪplaygameΪ��Ϸ����ʱ��¼��λ����
 
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define SNAKE_FOOD '$'
#define WALL_CELL '*'

char if_start_game = 'y';//�����ж��Ƿ����û���Ըֹͣ��Ϸ 
int if_end_game = 0;//�����ж��Ƿ���ײǽ������ʳ��ֹͣ��Ϸ 
int food[2] = {0,0};
char map_restart[12][13] = {//��ʼ״̬��ͼ 
	"************",      //������������Ϸ
	"*XXXXH *   *",
	"*      *   *",
	"*  ***     *",
	"*     * *  *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"************"
};
int headx = 1,heady = 5,tailx = 1,taily = 1;//������ʾ��ͷ����β��������� 
char map_playgame[12][13];//��Ϸ�����ڼ�¼�ߡ�ʳ�������ġ���Ϸʱ ����ͼ

char snake_nextx_restart[12][12] = {//��ʼ״̬ʱ���ڼ�¼����ͷ��ÿһ���ߵ���һ�ڵ�x���� 
	{0,0,0,0,0,0,0,0,0,0,0,0},   //������������Ϸ 
	{0,1,1,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0}
};
char snake_nextx_playgame[12][12];//��Ϸʱ���ڼ�¼����ͷ��ÿһ���ߵ���һ�ڵ�x����  

char snake_nexty_restart[12][12] = {//��ʼ״̬ʱ���ڼ�¼����ͷ��ÿһ���ߵ���һ�ڵ�y����
	{0,0,0,0,0,0,0,0,0,0,0,0},   //������������Ϸ
	{0,2,3,4,5,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
};
char snake_nexty_playgame[12][12];//��Ϸʱ���ڼ�¼����ͷ��ÿһ���ߵ���һ�ڵ�y����

void print_map_playgame();//��ӡ��ͼ 
void put_food(int *food);//Ͷ��ʳ�� 
void snake_move(int dx,int dy,int *food);//���ƶ� 
void restart();//������Ϸ
void playgame();//������Ϸ 

int main() { 
	
	while(if_start_game) {
		restart();//������Ϸ ���ڴ˴�ѡ���Ƿ�������Ϸ��y���У�������˳� 
		playgame();//����Ϸ 
	}
	return 0;
}

void print_map_playgame() {
	int i;
	for(i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
    
} 
void put_food(int *food){    //  ���Ͷ��ʳ�� 

    srand(time(0)); //  ��ʼ��α���
    int x,y;
    while (1) { //  һֱѭ�����ֱ�����ֿ������� 
        x=rand() % 12; y=rand() % 12;    //  ���һ������ 
        if (map_playgame[x][y]==BLANK_CELL) {    //  ��������Ϊ�հ׸� 
            map_playgame[x][y] = SNAKE_FOOD ;   //  ��Ϊʳ�� 
            food[0] = x;
    		food[1] = y;
            return;     //  ���� 
        }  
    }
    
}
void snake_move(int dx,int dy,int *food) {//���ƶ� 
    
	int is_eat = 0;//bool�����ж��Ƿ�Ե�ʳ��
	int x,y;//�����ݴ����� 
	if(map_playgame[headx + dx][heady + dy] == SNAKE_FOOD) {//�ж�ͷ������moveʱ�Ƿ������ʳ�� 
		is_eat = 1;//����Ե�ʳ�������bool 
	}
	if(!is_eat) {//���û�Ե�ʳ�β�ͽ�����һ���ƶ� 
		usleep(400000);
		map_playgame[tailx][taily] = BLANK_CELL;//β����ɿո�
		x = snake_nextx_playgame[tailx][taily];
		y = snake_nexty_playgame[tailx][taily];//��ȡ����β������ 
		tailx = x; taily = y;
	}
	if (map_playgame[headx+dx][heady+dy] == WALL_CELL || map_playgame[headx+dx][heady+dy] == SNAKE_BODY) {  
	     // �ж���ͷ�ƶ�����λ���Ƿ�ǽ������ 
        if_end_game = 1; //  ����ǽ����������Ϸ���� 
        return; 
    }
    //����ײǽ�������Ƿ��ʳ��ͷ�������ƶ�
    x = headx + dx ;
	y = heady + dy ;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//������ͷ�������һ���ߵ���һ�ڵ����꣬��ͷ������ 
	map_playgame[x][y] = SNAKE_HEAD;//�ƶ�ͷ�� 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//����ͷ��������
	if(is_eat) {
		put_food(food);
	} 
} 
void restart() {//������Ϸ 

	memcpy(map_playgame,map_restart,sizeof(map_restart));
	
    memcpy(snake_nextx_playgame,snake_nextx_restart,sizeof(snake_nextx_restart));
    
    memcpy(snake_nexty_playgame,snake_nexty_restart,sizeof(snake_nexty_restart));
    
    headx = 1,heady = 5,tailx = 1,taily = 1;if_end_game = 0;//������ͷ��β��λ��,���Ҹ���end���� 
    
	printf("Welcome to Greedy Snake!\n");
	printf("Do you want to start?\ny for yes;n for no\n");
	scanf("%c",&if_start_game);
	getchar(); //����һ��scanf�����Ļس���� 
	if(if_start_game != 'y') exit(1); //����'y',������˳� 
}
void playgame() {//������Ϸ

	printf("\033[2J");//���� 
	put_food(food);//Ͷ��ʳ�� 
	printf("x = %d y = %d\n",food[0],food[1]);

	print_map_playgame();//��ӡ��ʼ��ͼ 
	system("pause");
	char direction = 'd';//��ʼ����Ϊ'd',��������
	while(!if_end_game) {
		int possible_move[4] = {1,1,1,1};
		printf("\033[2J");//���� 
		int xx[4] = {-1,0,1,0};
		int yy[4] = {0,-1,0,1};
		for(int i = 0;i < 4;i ++) {
			if(map_playgame[headx+xx[i]][heady+yy[i]] == WALL_CELL || map_playgame[headx+xx[i]][heady+yy[i]] == SNAKE_BODY)
			possible_move[i] = 0;
		}
		
		int cur_distance[4];
		
		cur_distance[0] = abs(headx - 1 - food[0]) + abs(heady - food[1]);//w
		cur_distance[1] = abs(headx - food[0]) + abs(heady - 1 - food[1]);//a
		cur_distance[2] = abs(headx + 1 - food[0]) + abs(heady - food[1]);//s
		cur_distance[3] = abs(headx - food[0]) + abs(heady + 1 - food[1]);//d
		
		char next_direction;
		int k;
		for(k = 0;k < 4;k ++) {
			if(possible_move[k] == 1) break;
		}
		int min = cur_distance[k];
		int min_pos = k;
		for(int i = 0;i < 4;i ++) {
			if( possible_move[i] == 1 && cur_distance[i] < min) 
			{
				min = cur_distance[i];
				min_pos = i;
			}
		}
		
		
		switch(min_pos) {
			
			case 0: next_direction = 'w';break;
			case 1: next_direction = 'a';break;
			case 2: next_direction = 's';break;
			case 3: next_direction = 'd';break;
			
		}
		
		
		direction = next_direction;
		
		switch(direction) {
			
			case 'w': snake_move(-1,0,food);break;
			case 'a': snake_move(0,-1,food);break;
			case 's': snake_move(1,0,food);break;
			case 'd': snake_move(0,1,food);break;
			
		}
		print_map_playgame();//��ӡ����һ��snake_move��ĵ�ͼ 

	}
	printf("\033[2J");//��������ѭ��ʱ����Ϸ���������� 
	
}

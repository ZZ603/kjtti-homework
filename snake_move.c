#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<conio.h>
#include<windows.h>

//��Ϸ�к���Ϊy������Ϊx 
//������Ϊsnake������ͷ�⣬ÿһ���ߵ���һ��Ϊsnake_nextx��snake_nexty 
//���ΪrestartΪ������Ϸ�ã���¼��ʼ״̬
//���ΪplaygameΪ��Ϸ����ʱ��¼��λ����
 
#define SNAKE_HEAD 'H'
#define SNAKE_BODY 'X'
#define BLANK_CELL ' '
#define WALL_CELL '*'

char if_start_game = 'y';//�����ж��Ƿ����û���Ըֹͣ��Ϸ 
int if_end_game = 0;//�����ж��Ƿ���ײǽ������ʳ��ֹͣ��Ϸ 

char map_restart[12][13] = {//��ʼ״̬��ͼ 
	"************",      //������������Ϸ
	"*XXXXH     *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"*          *",
	"************"
};
int headx = 1,heady = 5,tailx = 1,taily = 1;//������ʾ��ͷ����β��������� 
char map_playgame[12][13];//��Ϸ�����ڼ�¼�ߵ�����ġ���Ϸʱ ����ͼ

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
	{0,0,0,0,0,0,0,0,0,0,0,0}
};
char snake_nexty_playgame[12][12];//��Ϸʱ���ڼ�¼����ͷ��ÿһ���ߵ���һ�ڵ�y����

void print_map_playgame();//��ӡ��ͼ 
void snake_move(int dx,int dy);//���ƶ� 
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
	for(int i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
} 

void snake_move(int dx,int dy) {//���ƶ� 
    int x,y; 
	Sleep(100);
	map_playgame[tailx][taily] = BLANK_CELL;//β����ɿո�
	x = snake_nextx_playgame[tailx][taily];
	y = snake_nexty_playgame[tailx][taily];//��ȡ����β������ 
	tailx = x; taily = y;
	
	if (map_playgame[headx+dx][heady+dy] == WALL_CELL || map_playgame[headx+dx][heady+dy] == SNAKE_BODY) {  
	     // �ж���ͷ�ƶ�����λ���Ƿ�ǽ������ 
        if_end_game = 1; //  ����ǽ����������Ϸ���� 
        return; 
    }
    //����ײǽ��ͷ�������ƶ�
    x = headx + dx ; y = heady + dy;
	snake_nextx_playgame[headx][heady] = x;
	snake_nexty_playgame[headx][heady] = y;//������ͷ�������һ���ߵ���һ�ڵ����꣬��ͷ������ 
	map_playgame[x][y] = SNAKE_HEAD;//�ƶ�ͷ�� 
	map_playgame[headx][heady] = SNAKE_BODY;
	headx = x ; heady = y;//����ͷ��������

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

	system("cls");//���� 
	int if_hit = 0;//�жϼ����Ƿ������� 
	print_map_playgame();//��ӡ��ʼ��ͼ 
	char direction = 'd';//��ʼ����Ϊ'd',�������� 
	int first_press = 0;//����Ϊ�˷�ֹ��Ϊ������Ϸǰ���������õ�bool���� 
	while(!if_end_game) {
		char next_direction ;
		if(kbhit()) {//���������û�ʱ�Ÿ���һ�η���ֵ 
        	next_direction = getch();
        	first_press = 1;//�����ʾ���˵�һ�μ������� 
        }
		if(next_direction != direction) {//�����һ�η����뵱ǰ����ͬ���ж�������� 
			if(next_direction + direction == 'w'+'s' || next_direction + direction == 'a'+'d');
										//�����һ�η����뵱ǰ�෴�������ı� 
			else direction = next_direction;
										//����ı䷽�� 
		}//�����һ�η����뵱ǰ��ͬ�������ı� 
		switch(direction) {
			case 'w': snake_move(-1,0);break;
			case 'a': snake_move(0,-1);break;
			case 's': snake_move(1,0);break;
			case 'd': snake_move(0,1);break;
		}
		if(first_press) {//�����˵�һ�μ�������֮�������������ͼ������δ������Ϸǰ���� 
			system("cls");//���� 
			print_map_playgame();//��ӡ����һ��snake_move��ĵ�ͼ
		} 
	}
	system("cls");//��������ѭ��ʱ����Ϸ���������� 
}
/*	*/

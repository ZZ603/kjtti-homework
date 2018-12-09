#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <assert.h>

int getch(void) 
{
        int c=0;
        struct termios org_opts, new_opts;
        int res=0;
        //-----  store old settings -----------
        res=tcgetattr(STDIN_FILENO, &org_opts);
        assert(res==0);
        //---- set new terminal parms --------
        memcpy(&new_opts, &org_opts, sizeof(new_opts));
        new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
        tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
        c=getchar();
            //------  restore old settings ---------
        res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);assert(res==0);
        return c;
}

static struct termios ori_attr, cur_attr;

static __inline 
int tty_reset(void)
{
        if (tcsetattr(STDIN_FILENO, TCSANOW, &ori_attr) != 0)
                return -1;

        return 0;
}


static __inline
int tty_set(void)
{
        
        if ( tcgetattr(STDIN_FILENO, &ori_attr) )
                return -1;
        
        memcpy(&cur_attr, &ori_attr, sizeof(cur_attr) );
        cur_attr.c_lflag &= ~ICANON;
//        cur_attr.c_lflag |= ECHO;
        cur_attr.c_lflag &= ~ECHO;
        cur_attr.c_cc[VMIN] = 1;
        cur_attr.c_cc[VTIME] = 0;

        if (tcsetattr(STDIN_FILENO, TCSANOW, &cur_attr) != 0)
                return -1;

        return 0;
}

static __inline
int kbhit(void) 
{
                   
        fd_set rfds;
        struct timeval tv;
        int retval;


        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        
        tv.tv_sec  = 0;
        tv.tv_usec = 0;

        retval = select(1, &rfds, NULL, NULL, &tv);


        if (retval == -1) {
                perror("select()");
                return 0;
        } else if (retval)
                return 1;

        else
                return 0;
        return 0;
}

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
void put_food();//Ͷ��ʳ�� 
void snake_move(int dx,int dy);//���ƶ� 
void restart();//������Ϸ
void playgame();//������Ϸ 

int main() { 
	
	while(if_start_game) {
		int tty_set_flag;
    	tty_set_flag = tty_set();
		restart();//������Ϸ ���ڴ˴�ѡ���Ƿ�������Ϸ��y���У�������˳� 
		playgame();//����Ϸ 
		//�ָ��ն�����
        if(tty_set_flag == 0) 
                tty_reset();
        return 0;
	}
	
	return 0;
}

void print_map_playgame() {
	int i;
	for(i=0;i<12;i++) {
        printf("%s\n",map_playgame[i]);
    }
    
} 
void put_food(){    //  ���Ͷ��ʳ�� 

    srand(time(0)); //  ��ʼ��α���
    int x,y;
    while (1) { //  һֱѭ�����ֱ�����ֿ������� 
        x=rand() % 12; y=rand() % 12;    //  ���һ������ 
        if (map_playgame[x][y]==BLANK_CELL) {    //  ��������Ϊ�հ׸� 
            map_playgame[x][y] = SNAKE_FOOD ;   //  ��Ϊʳ�� 
            return;     //  ���� 
        }  
    }
    
}
void snake_move(int dx,int dy) {//���ƶ� 
    
	int is_eat = 0;//bool�����ж��Ƿ�Ե�ʳ��
	int x,y;//�����ݴ����� 
	if(map_playgame[headx + dx][heady + dy] == SNAKE_FOOD) {//�ж�ͷ������moveʱ�Ƿ������ʳ�� 
		is_eat = 1;//����Ե�ʳ�������bool 
	}
	if(!is_eat) {//���û�Ե�ʳ�β�ͽ�����һ���ƶ� 
		usleep(200000);
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
		put_food();
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
	int if_hit = 0;//�жϼ����Ƿ������� 
	put_food();//Ͷ��ʳ�� 
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
		//printf("%c", direction);
		switch(direction) {
			case 'w': snake_move(-1,0);break;
			case 'a': snake_move(0,-1);break;
			case 's': snake_move(1,0);break;
			case 'd': snake_move(0,1);break;
		}
		if(first_press) {//�����˵�һ�μ�������֮�������������ͼ������δ������Ϸǰ����
			printf("\033[2J");//���� 
			print_map_playgame();//��ӡ����һ��snake_move��ĵ�ͼ 
		}
	}
	printf("\033[2J");//��������ѭ��ʱ����Ϸ���������� 
	
}

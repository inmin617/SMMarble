//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"
#include "smm_object.c"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"
#define EXPERIMENT_SUCCESS_THRESHOLD // 실험 성공 기준값 

#ifndef MAX_PLAYER
#define MAX_PLAYER     100
#endif

//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

typedef struct player {
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
} player_t;

static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
# endif

//function prototypes

#if 0
int isGraduated(void); //check if any player is graduated
void printGrades(int player); //print grade history of the player
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
#endif


void printGrades(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
     }
}



void printPlayerStatus(void)
{
     int i;
     
     for (i=0;i<player_nr;i++)
     {
         printf("%s : credit %i, energy %i, position %i\n", 
                      cur_player[i].name,
                      cur_player[i].accumCredit,
                      cur_player[i].energy,
                      cur_player[i].position);
     }
}


void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name: ", i);  
         scanf("%s", cur_player[i].name);
         
         while (getchar() != '\n');
         
         //set position
         cur_player[i].position = 0;
         
         //set energy
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit;
         cur_player[i].flag_graduate=0;
     }
     
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}



//action code when a player stays at a node
void actionNode(int player)
{
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getNodeName( boardPtr );
    void *gradePtr;
    
	
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
            if (cur_player[player].energy > smmObj_getNodeCredit(boardPtr)) {
            	cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr );
            	cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr );
				
            
            	int credit = smmObj_getNodeCredit(boardPtr);
            	//grade generation
        		gradePtr = smmObj_genObject(name, smmObjType_grade, 0, credit, 0, ??);
            	smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        	}
            break;
            
        default:
            break;
            
        //case food:
        case SMMNODE_TYPE_RESTAURANT:
        	cur_player[player].energy += smmObj_getNodeEnergy((smmObject_t*)boardPtr); // 음식으로 에너지 보충 
        	break;
		default;
		break; 
        
        //case laboratory:
        case SMMNODE_TYPE_LABORATORY:
        	if (type==SMMNODE_TYPE_GOTOLAB) {
        		
        		// 실험 성공 여부를 판단하는 함수
				int isExperimentSuccess() {
    			// 주사위 굴리기
    			int diceResult = rand() % 6 + 1;

    			// 실험 성공 여부 판단
    			return diceResult >= EXPERIMENT_SUCCESS_THRESHOLD;
				}
				
				
			}
        	
        	// 실험 중 상태에서의 액션을 처리하는 함수
			void handleExperimentAction(int player) {
    			printf("%s is in the experiment state.\n", cur_player[player].name);

    			// 실험 성공 여부 판단
    			if (isExperimentSuccess()) {
        			printf("Experiment successful! Move to the next.\n");
        			goForward(cur_player, int step);
    			} 
				else {
        				printf("Experiment failed. Stay in the current node.\n");
        				// 실험 실패로 인한 동작
    			}
    			

    			// 실험 중 상태이면 실험 중 액션 수행
   				 if (isInExperimentState(nodeType)) {
        		 	 handleExperimentAction(player);
    			 } 
				 else {
        				// 그냥 지나감  
   				 }
			}

			// main 함수에서 호출하는 부분
			void goForward(int player, int step) {
    			void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    			int nodeType = smmObj_getNodeType(boardPtr);

    			// 실험 중 상태이면 실험 중 액션 수행, 아니면 다른 액션 수행
    			if (isInExperimentState(nodeType)) {
       				 handleExperimentAction(player);
    			} else {
        			// 다른 액션 처리 로직 추가
        			cur_player[player].position += step;
       			 	printf("%s go to node %i (name: %s)\n", cur_player[player].name, cur_player[player].position, smmObj_getNodeName(boardPtr));
    			}
			}
			break;
    		
    }
    		
        	
        //case house:
        case SMMNODE_TYPE_HOME : 
        	if (smmObj_getNodeType(Board_Ptr)){
        		int energyGain = 18;

                // 에너지에 더해짐
                cur_player.energy += energyGain;

                // 이동한 노드 정보 출력
                printf("Moved through HOME node and gained %d energy. Current energy: %d\n",
                       energyGain, cur_player.energy);
                break;
			}

        
        
        	
        //case experiment:
        case SMMNODE_TYPE_GOTOLAB :
        	
        	cur_player.position = isInExperimentState(nodeType) 
        	case SMMNODE_TYPE_LABORATORY
        	
        break;
        	
        //case foodchance:
        case SMMNODE_TYPE_FOODCHANCE: 
        	// 전역 변수
			static FoodCard foodCards[MAX_FOOD_CARDS];
			static int food_nr = 0; // 음식 카드의 총 개수

			// 파일에서 음식 카드 정보를 읽어오는 함수
			void readFoodCards() {
    			FILE *fp = fopen("marblefoodconfig.txt", "r");

    			if (fp == NULL) {
    		    	printf(stderr, "[ERROR] failed to open marblefoodconfig.txt.\n");
    		    	exit(EXIT_FAILURE);
    			}

    			while (fscanf(fp, "%s %d", foodCards[food_nr].name, &foodCards[food_nr].energyBonus) == 2) {
        			food_nr++;
    			}

    			fclose(fp);
			}

			// 현재 에너지에 보충 에너지를 더하는 함수
			void addEnergyFromFoodCard(int player) {
    			int randomFoodIndex = rand() % food_nr;

    			// 현재 에너지에 보충 에너지를 더함
    			cur_player[player].energy += foodCards[randomFoodIndex].energyBonus;

    		// 선택된 음식 카드의 정보 출력
    		printf("Selected Food Card: %s, Energy Bonus: %d\n", foodCards[randomFoodIndex].name, foodCards[randomFoodIndex].energyBonus);
			}


			int main() {
    			srand(time(NULL));

    			// 파일에서 음식 카드 정보를 읽어옴
    			readFoodCards();



    			// 특정 노드에 도달했을 때 음식 카드 사용
    			case SMMNODE_TYPE_RESTAURANT:
        			addEnergyFromFoodCard(player);
        		break;


    			return 0;
			}
        	
		//case festival:  
		case SMMNODE_TYPE_FESTIVAL:
			static FestivalCard festivalCards[MAX_FESTIVAL_CARDS];
			static int festival_nr = 0; // 축제 카드의 총 개수

			// 파일에서 축제 카드 정보를 읽어오는 함수
			void readFestivalCards() {
    			FILE *fp = fopen("marbleFestivalConfig.txt", "r");

    			if (fp == NULL) {
    			    fprintf(stderr, "[ERROR] failed to open marbleFestivalConfig.txt.\n");
    			    exit(EXIT_FAILURE);
    			}

    			while (fscanf(fp, "%[^\n]%*c", festivalCards[festival_nr].mission) == 1) {
        			festival_nr++;
    			}

    		fclose(fp);
			}

			// 축제 카드를 사용하여 미션을 수행하는 함수
			void performFestivalMission(int player) {
    			int randomFestivalIndex = rand() % festival_nr;

    			// 선택된 축제 카드의 미션을 수행
    			printf("Performing Festival Mission: %s\n", festivalCards[randomFestivalIndex].mission);

    			// 플레이어에게 답변을 입력받음
    			char playerAnswer[MAX_LINELENGTH];
    			printf("Your Answer: ");
    			fgets(playerAnswer, sizeof(playerAnswer), stdin);
    			
    			printf("Mission Completed!\n");

			}


			int main() {
    			srand(time(NULL));

    			// 파일에서 축제 카드 정보를 읽어옴
    			readFestivalCards();
-
    			// 특정 노드에 도달했을 때 축제 카드 사용
    			case SMMNODE_TYPE_FESTIVAL:
        			performFestivalMission(player);
        		break;

    		return 0;
			}
			
		
    }
}

void goForward(int player, int step)
{
     void *boardPtr;
     cur_player[player].position += step;
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     
     printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr);
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen("marbleBoardConfig","r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        boar_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
	
	}
	
    // printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //2. food card config 
	
	static FoodCard foodCards[MAX_FOOD_CARDS];
	static int food_nr = 0; // 초기값을 적절하게 설정



	if ((fp = fopen("marbleFoodConfig", "r")) == NULL) {
    	printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
    	return -1;
	}

	printf("\n\nReading food card component......\n");
	while (food_nr < MAX_FOOD_CARDS && fscanf(fp, "%s %d", foodCards[food_nr].name, &foodCards[food_nr].energyBoost) == 2) {
    	// 파일에서 읽어온 음식 카드 정보를 배열에 저장
    	food_nr++;
	}

	fclose(fp);
	printf("Total number of food cards : %i\n", food_nr);

    
    
    
    //3. festival card config 

	// 전역 변수
	static FestivalCard festivalCards[MAX_FESTIVAL_CARDS];
	static int festival_nr = 0; // 초기값을 적절하게 설정


	if ((fp = fopen(FESTFILEPATH, "r")) == NULL) {
    	printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
    	return -1;
	}

	printf("\n\nReading festival card component......\n");
	while (festival_nr < MAX_FESTIVAL_CARDS && fscanf(fp, "%s", festivalCards[festival_nr].name) == 1) {
    	// 파일에서 읽어온 페스티벌 카드 정보를 배열에 저장
    	festival_nr++;
	}

	fclose(fp);
	printf("Total number of festival cards : %i\n", festival_nr);

    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
     

    

    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)       
		die_result=rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn=(turn+1)%player_nr;
        
    }
    
    free(cur_player);
    system("PAUSE");
    return 0;
}

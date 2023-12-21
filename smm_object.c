//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100
#define MAX_CHARNAME 	200
#define MAX_LINELENGTH	4
#define SMMNODE_TYPE_MAX	7


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
       "강의",
       "식당",
       "실험실",
       "집",
       "실험실로이동",
       "음식찬스",
       "축제시간"
}; // node name

char* smmObj_getTypeName(int type)
{
      return (char*)smmNodeName[type];
}


static smmObject_t smm_node[MAX_NODE];

//3. 관련 함수 변경
//object generation
void smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade){
	smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = smmObjType_board;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

//member retrieving
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}


int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

int smmObj_getGradeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}

// 구조체 정의: foodcard
	typedef struct FoodCard {
    	char name[MAX_CHARNAME];
    	int energyBoost;
    	int energyBonus;
    	// 다른 필요한 정보들을 추가할 수 있음
	} FoodCard;

//구조체 정의: festivalcard

	// 구조체 정의
	typedef struct FestivalCard {
    	char name[MAX_CHARNAME];
    	char mission[MAX_CHARNAME];
    	// 다른 필요한 정보들을 추가할 수 있음
	} FestivalCard; 

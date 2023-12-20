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

typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e; //defining grade 

//1. 구조체 형식 정의
typedef enum smmObjType {
    smmObjType_board = 0,
    smmObjType_card,
    smmObjType_grade
} smmObjType_e;

typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
}; smmObject_t; 

//2. 구조체 배열 변수 정의 

static smmObject_t = smm_node[MAX_NODE];

//3. 관련 함수 변경
//object generation
 smmObj_genObject(char* name, int type, int credit, int energy, smmObjGrade_e grade)
{
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

char* smmObj_getNodeName(int node_nr)
{
	return smm_node[node_nr].name;
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

// 소요에너지 가져오는 함수 만들기


int getEnergyCost(const char* filename)
{
    FILE* file = fopen(filname, "r");

    if (file == NULL) {
        fprintf(stderr, "파일을 열 수 없습니다.\n");
        return -1; // 오류 발생 시 음수 반환
    }

    int energyCost = 0;
    char line[MAX_LINELENGTH];

    // 파일의 각 줄을 읽어 마지막 노드 정보 저장
    while (fgets(line, sizeof(line), file) != NULL) {
        // 공백으로 분리된 각 노드 정보 가져오기
        char* token = strtok(line, " ");
        while (token != NULL) {
            energyCost = atoi(token); // 문자열을 정수로 변환
            token = strtok(NULL, " ");
        }
    }

    fclose(file);

    return energyCost;
}

int main() {
    const char* filename = "marbleBoardConfig.txt";
    int energyCost = getEnergyCost(filename);

    if (energyCost == -1) {
        return 1; // 파일 열기 오류
    }

    // energyCost 변수를 사용하여 원하는 작업 수행

    return 0;
}

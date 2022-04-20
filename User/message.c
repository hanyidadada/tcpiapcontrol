#include "message.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

void getRegisterMessage
(const char *DeviceCategory, const int DeviceBlockNum, 
const char *AccessPoint, const char *RegisterType, 
char **regisMsg) {
    char *regisMsgTarget = malloc(sizeof(char) * MAX_MSG_LENGTH);
    sprintf(regisMsgTarget, registerMessageTemplate, 
        "Register", DeviceCategory, DeviceBlockNum, AccessPoint, RegisterType);

    *regisMsg = regisMsgTarget;;   
}

void getReportMessage
(const char *DeviceType, const char *GroupID,
int *statuses, const int blockNum, char **reportMsg) {
	int i = 0; 
    char *insertList = malloc(sizeof(char) * (blockNum * 2 - 1));
	char *result = malloc(sizeof(char) * MAX_MSG_LENGTH);
    for (i = 0; i < blockNum; i++) {
        insertList[2 * i] = statuses[i] + 0x30;
        insertList[2*i+1] = (i == (blockNum - 1) ? '\0' : ',');
    }
    
    sprintf(result, reportMessageTemplate, DeviceType, GroupID, insertList);
    *reportMsg = result;    
}



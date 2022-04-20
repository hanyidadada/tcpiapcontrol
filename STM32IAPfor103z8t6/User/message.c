#include "message.h"
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>

void hello() {
    printf("Hello, world");
}

const char *registerMessageTemplate = "{\"Type\": \"%s\", \"Info\": \
    {\"DeviceCategoryType\": \"%s\", \"DeviceBlockNum\": %d, \
    \"AccessPoint\": \"%s\", \"RegisterType\": \"%s\"}}\n";

    
// {"Type": "Report","Info": {"DeviceType": "%s","GroupID": "%s","BlockStatuses": [%s]}}

const char *reportMessageTemplate = "{\"Type\": \"Report\",\"Info\": \
{\"DeviceType\": \"%s\",\"GroupID\": \"%s\",\"BlockStatuses\": [%s]}}";

void getRegisterMessage
(const char *DeviceCategory, const int DeviceBlockNum, 
const char *AccessPoint, const char *RegisterType, 
char **regisMsg) {
    char *regisMsgTarget = malloc(sizeof(char) * MAX_MSG_LENGTH);
    sprintf(regisMsgTarget, registerMessageTemplate, 
        "Register", DeviceCategory, DeviceBlockNum, AccessPoint, RegisterType);
    *regisMsg = regisMsgTarget;   
}

void getReportMessage
(const char *DeviceType, const char *GroupID,
int *statuses, const int blockNum, char **reportMsg) {
    char *insertList = malloc(sizeof(char) * (blockNum * 2 - 1));
    for (int i = 0; i < blockNum; i++) {
        insertList[2 * i] = statuses[i] + 0x30;
        insertList[2*i+1] = (i == (blockNum - 1) ? '\0' : ',');
    }
    char *result = malloc(sizeof(char) * MAX_MSG_LENGTH);
    sprintf(result, reportMessageTemplate, DeviceType, GroupID, insertList);
    *reportMsg = result;    
}



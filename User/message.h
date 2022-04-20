#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MSG_LENGTH 100
#define cmdMessageTemplate  "%s %d %d"

#define registerMessageTemplate "{\"Type\": \"%s\", \"Info\": \
    {\"DeviceCategoryType\": \"%s\", \"DeviceBlockNum\": %d, \
    \"AccessPoint\": \"%s\", \"RegisterType\": \"%s\"}}\n"

#define reportMessageTemplate  "{\"Type\": \"Report\",\"Info\": \
{\"DeviceType\": \"%s\",\"GroupID\": \"%s\",\"BlockStatuses\": [%s]}}"

void getRegisterMessage
(const char *DeviceCategory, const int DeviceBlockNum, 
const char *AccessPoint, const char *RegisterType, 
char **regisMsg);


void getReportMessage
(const char *DeviceType, const char *GroupID,
int *statuses, const int blockNum, char **reportMsg);

#endif

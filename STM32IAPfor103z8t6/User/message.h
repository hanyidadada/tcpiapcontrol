#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MSG_LENGTH 0x400

void hello();

void getRegisterMessage
(const char *DeviceCategory, const int DeviceBlockNum, 
const char *AccessPoint, const char *RegisterType, 
char **regisMsg);


void getReportMessage
(const char *DeviceType, const char *GroupID,
int *statuses, const int blockNum, char **reportMsg);

#endif
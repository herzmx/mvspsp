/***************************************************************************

	adhoc.c

	PSP AdHoc functions.

***************************************************************************/

#ifndef PSP_ADHOC_INTERFACE_H
#define PSP_ADHOC_INTERFACE_H

int pspSdkLoadAdhocModules(void);

int adhocInit(const char *matchingData);
int adhocTerm(void);
int adhocSelect(void);
int adhocReconnect(char *ssid);

int adhocSend(void *buffer, int length);
int adhocRecv(void *buffer, int length);

int adhocSendBlocking(void *buffer, int length);
int adhocRecvBlocking(void *buffer, int length);
int adhocRecvBlockingTimeout(void *buffer, int length, int timeout);

int adhocSendRecvAck(void *buffer, int length);
int adhocRecvSendAck(void *buffer, int length);

#endif /* PSP_ADHOC_INTERFACE_H */

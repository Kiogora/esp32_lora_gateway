/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#if !defined(__MQTT_LINUX_)
#define __MQTT_LINUX_

#if defined(WIN32_DLL) || defined(WIN64_DLL)
#define DLLImport __declspec(dllimport)
#define DLLExport __declspec(dllexport)
#elif defined(LINUX_SO)
#define DLLImport extern
#define DLLExport __attribute__((visibility("default")))
#else
#define DLLImport
#define DLLExport
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "MQTTErrors.h"

typedef struct Timer
{
   struct timeval end_time;
} Timer;

void TimerInit(Timer *);
char TimerIsExpired(Timer *);
void TimerCountdownMS(Timer *, unsigned int);
void TimerCountdown(Timer *, unsigned int);
int TimerLeftMS(Timer *);

typedef struct Network
{
   int my_socket;
   int (*mqttread)(struct Network *, unsigned char *, int, int);
   int (*mqttwrite)(struct Network *, unsigned char *, int, int);
} Network;

int linux_read(Network *, unsigned char *, int, int);
int linux_write(Network *, unsigned char *, int, int);

DLLExport void NetworkInit(Network *);
DLLExport int NetworkConnect(Network *, char *, int);
DLLExport void NetworkDisconnect(Network *);

typedef struct Mutex
{
   pthread_mutex_t m;
} Mutex;

void MutexInit(Mutex *);
int MutexLock(Mutex *);
int MutexUnlock(Mutex *);
int MutexDestroy(Mutex *);

typedef struct Condition
{
   pthread_cond_t c;
} Condition;

void ConditionInit(Condition *);
int ConditionWait(Condition *, Mutex *);
int ConditionTimedWait(Condition *, Mutex *, Timer *);
int ConditionSignal(Condition *);
int ConditionDestroy(Condition *);

typedef struct Queue
{
   unsigned short item;
   Condition c;
   Mutex m;
} Queue;

void QueueInit(Queue *);
int Enqueue(Queue *, unsigned short);
int Dequeue(Queue *, unsigned short *, Timer *);
int QueueDestroy(Queue *);

typedef struct Thread
{
   pthread_t t;
   int started;
} Thread;

int ThreadStart(Thread *, void (*fn)(void *), void *arg);
int ThreadStarted(Thread *);
int ThreadJoin(Thread *);
void ThreadExit();

#endif

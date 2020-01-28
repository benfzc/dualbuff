#include "DualBuff.h"
//#include <iostream>

DualBuff::DualBuff()
{
    DB_TRACE_ENTER;
    InitializeCriticalSection(&cs);
    memset(buffs, 0, sizeof(buffs));
    currentIdx = 0;
    DB_TRACE_QUIT;
}

DualBuff::~DualBuff()
{
    DB_TRACE_ENTER;

    /* NOTE: assume derived class will release data pointed by buffs[] */
    DeleteCriticalSection(&cs);
    DB_TRACE_QUIT;
}

/*
 * only get the buffer pointed by currentIdx, don't care the other buffer
 */
void *DualBuff::GetLatestData()
{
    void *data = NULL;

    EnterCriticalSection(&cs);
    data = buffs[currentIdx];
    buffs[currentIdx] = NULL;
    LeaveCriticalSection(&cs);

    return data;
}

void *DualBuff::UpdateNewData(void *data)
{
    int nextIdx = !currentIdx;
    void *olddata = buffs[nextIdx];
    buffs[nextIdx] = data;

    EnterCriticalSection(&cs);
    currentIdx = nextIdx;
    LeaveCriticalSection(&cs);

    return olddata;
}

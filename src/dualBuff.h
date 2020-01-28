#include <Windows.h>

#define DB_TRACE_PRINT
#ifdef DB_TRACE_PRINT
#include <iostream>
#define DB_TRACE_ENTER                                      \
    do                                                      \
    {                                                       \
        std::cout << __FUNCTION__ << " enter" << std::endl; \
    } while (0)
#define DB_TRACE_QUIT                                      \
    do                                                     \
    {                                                      \
        std::cout << __FUNCTION__ << " quit" << std::endl; \
    } while (0)
#else
#define DB_TRACE_ENTER
#define DB_TRACE_QUIT
#endif



class DualBuff
{
public:
    DualBuff();
    ~DualBuff();

    // Get latest data from the buffer
    void *GetLatestData();

    // Replace the older buffer data by the input data, and the older data is returned.
    // Caller have to release the returned data.
    void *UpdateNewData(void *data);

private:
    CRITICAL_SECTION cs;
    int currentIdx; // 0 or 1

protected:
    void *buffs[2];
};

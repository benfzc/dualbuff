#include "DualBuff.h"


struct RecvedFrame
{
    char *data;
    unsigned size;
    unsigned sn;

    RecvedFrame(const char *frameData, unsigned frameSize, unsigned frameSn) : 
        size(frameSize), sn(frameSn)
    {
        data = new char[frameSize];
        memcpy(data, frameData, frameSize);
    }

    ~RecvedFrame()
    {
        delete[] data;
    }
};


class RecvBuff : private DualBuff
{
public:
    ~RecvBuff()
    {
        DB_TRACE_ENTER;
        // free data pointed by buffs[] before parent class is destructed
        for (int i = 0; i < 2; i++)
        {
            RecvedFrame *frame = (RecvedFrame *)buffs[i];
            if (frame)
            {
                delete frame;
            }
        }
        DB_TRACE_QUIT;
    }

    RecvedFrame *GetLatestData()
    {
        RecvedFrame *frame = (RecvedFrame *)DualBuff::GetLatestData();
        return frame;
    }

    void UpdateNewData(char *data, unsigned size, unsigned sn)
    {
        RecvedFrame *frame = new RecvedFrame(data, size, sn);
        RecvedFrame *oldframe = (RecvedFrame *)DualBuff::UpdateNewData(frame);
        if (oldframe)
        {
            delete oldframe;
        }
    }
};


int main()
{
    RecvedFrame *recvframe;
    RecvBuff *recvbuff = new RecvBuff;
    char *buf[3] = {"packet 0", "packet 1", "packet 2"};

    std::cout << "push packet 0 to recvbuff\n";
    recvbuff->UpdateNewData(buf[0], strlen(buf[0])+1, 0);
    std::cout << "push packet 1 to recvbuff\n";
    recvbuff->UpdateNewData(buf[1], strlen(buf[1])+1, 1);

    recvframe = recvbuff->GetLatestData();
    if (recvframe) {
        std::cout << "pop data " << recvframe->sn << ", content: " << recvframe->data << "\n";
        delete recvframe;
    }
    else {
        std::cout << "no new data in recvbuff\n";
    }
    recvframe = recvbuff->GetLatestData();
    if (recvframe) {
        std::cout << "pop packet " << recvframe->sn << ", content: " << recvframe->data << "\n";
        delete recvframe;
    }
    else {
        std::cout << "no new data in recvbuff\n";
    }

    std::cout << "push packet 2 to recvbuff\n";
    recvbuff->UpdateNewData(buf[2], strlen(buf[2])+1, 2);

    recvframe = recvbuff->GetLatestData();
    if (recvframe) {
        std::cout << "pop packet " << recvframe->sn << ", content: " << recvframe->data << "\n";
        delete recvframe;
    }
    else {
        std::cout << "no new data in recvbuff\n";
    }

    delete recvbuff;

    std::cout << "quit\n";

    return 0;
}
#ifndef MESSAGE_H
#define MESSAGE_H


#define MESSAGE_SIZE 1024
#define RATE_BYTES 8
struct Message
{
    char audio_data[MESSAGE_SIZE];
    int frequency;
    bool disconect;
    bool call;
    bool sending;
    int id;

public:

    bool operator==(const Message& other) const
    {
        if (frequency != other.frequency)
            return false;
        if (disconect != other.disconect)
            return false;
        if (call != other.call)
            return false;
        if (sending != other.sending)
            return false;

        for(int i = 0; i < MESSAGE_SIZE; i++)
            if (audio_data[i] != other.audio_data[i])
                return false;

        return true;
    }
};

#endif // MESSAGE_H

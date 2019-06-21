#ifndef R123BPSTATECONTROLLER_H
#define R123BPSTATECONTROLLER_H


class R123BPStateController
{
    bool ziem_connected;
    bool pit_rst_launched;
    bool pit_launched;
public:
    R123BPStateController();

    enum Action {
        ZIEM, PIT, PIT_RST,
    };

    void actionResolver(Action action);

    bool canTouch();
    bool isGood();
};

#endif // R123BPSTATECONTROLLER_H

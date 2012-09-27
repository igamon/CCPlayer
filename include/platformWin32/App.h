#ifndef APP_H
#define APP_H

namespace CCPlayer
{

class CCApp
{
public:
    CCApp(int argc, char* argv[]);
    int Exec();
    virtual ~CCApp();

private:
    bool m_bInited;
};

}

#endif // APP_H

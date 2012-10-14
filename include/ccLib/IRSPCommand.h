#ifndef IRSPCOMMAND_H
#define IRSPCOMMAND_H

namespace CCPlayer
{

class IRSPCommand
{
public:
    virtual void OpenResponse(int ErrCode) = 0;
};

}
#endif

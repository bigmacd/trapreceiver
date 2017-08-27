
#ifdef _WIN32
#include "ntservice.h"
#include "NtSysUpTime.h"
#endif

class Packet;

#ifdef _WIN32
class TrapReceiverService : public CNTService
#else
class TrapReceiverService
#endif
{
  private:


  protected:


  public:
	
    TrapReceiverService();
    ~TrapReceiverService();

#ifdef _WIN32
    virtual BOOL OnInit();
    virtual BOOL OnUserControl(DWORD dwOpcode);
    void SaveStatus();

    // Control parameters
    int m_iStartParam;
    int m_iIncParam;

    // Current state
    int m_iState;
#endif
    virtual void Run();
    
};



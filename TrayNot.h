// CTrayNot window

class CTrayNot : public CObject
{
// Construction
public:
	CTrayNot ( CWnd* pWnd, UINT uCallbackMessage,
				  LPCTSTR szTip, HICON* pList ) ;

// Attributes
public:
	BOOL			m_bEnabled ;	
	NOTIFYICONDATA	m_tnd ;
	HICON*			m_pIconList ;


public:
	void SetState ( int id = 0 ) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrayNot)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrayNot();

};

/////////////////////////////////////////////////////////////////////////////

#pragma once


// TrAttention dialog

class TrAttention : public CDialog
{
	DECLARE_DYNAMIC(TrAttention)

public:
	TrAttention(CWnd* pParent = NULL);   // standard constructor
	virtual ~TrAttention();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

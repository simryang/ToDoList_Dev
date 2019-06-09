#if !defined(AFX_TDLPRINTDIALOG2_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)
#define AFX_TDLPRINTDIALOG2_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TDLPrintDialog.h : header file
//

#include "TaskSelectionDlg.h"
#include "TDLDialog.h"

#include "..\shared\fileedit.h"
#include "..\shared\historycombobox.h"
#include "..\shared\TabbedPropertyPageHost.h"

#include "..\interfaces\ImportExportComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// 
// enum TDLPD_STYLE
// {
// 	TDLPDS_NONE = -1,
// 
// 	TDLPDS_STYLESHEET,
// 	TDLPDS_WRAP,
// 	TDLPDS_TABLE,
// 	TDLPDS_PARA,
// 	TDLPDS_IMAGE,
// 	TDLPDS_OTHEREXPORTER,
// };

/////////////////////////////////////////////////////////////////////////////

class CTDLPrintStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTDLPrintStylePage)

// Construction
public:
	CTDLPrintStylePage(LPCTSTR szStylesheet = NULL, LPCTSTR szPrefsKey = NULL,
					   BOOL bSupportsExportToImage = TRUE);
	~CTDLPrintStylePage();
	
	TDLPD_STYLE GetExportStyle() const;
	BOOL GetStylesheet(CString& sStylesheet) const;

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintStylePage)
	enum { IDD = IDD_PRINT_STYLE_PAGE };
	CStatic	m_stSimpleIcon;
	CComboBox	m_cbSimpleOptions;
	//}}AFX_DATA

	COwnerdrawComboBoxBase	m_cbOtherExporters;
	CFileEdit m_eStylesheet;
	CString	m_sStylesheet;
	int m_nStyleOption;
	BOOL m_bSupportsExportToImage;
	CString m_sPrefsKey;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintStylePage)
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTDLPrintStylePage)
	afx_msg void OnSelchangeSimplePageOption();
	//}}AFX_MSG
	afx_msg void OnChangeStylesheet();
	afx_msg void OnChangeStyle();
	afx_msg void OnConfigureStylesheet();
	afx_msg void OnUsestylesheet();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	void InitStylesheet(LPCTSTR szStylesheet);
	void EnableDisableControls();

	CString GetBaseStylesheetPath() const;
};

/////////////////////////////////////////////////////////////////////////////
// CTDLPrintTaskSelectionPage dialog

class CTDLPrintTaskSelectionPage : public CPropertyPage
{
// Construction
public:
	CTDLPrintTaskSelectionPage(const CTDCCustomAttribDefinitionArray& aAttribDefs, 
							   LPCTSTR szRegKey, FTC_VIEW nView);
	~CTDLPrintTaskSelectionPage();

	const CTaskSelectionDlg& GetTaskSelection() const { return m_dlgTaskSel; }

	void SetOutputStyle(TDLPD_STYLE nStyle);

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintTaskSelectionPage)
	enum { IDD = IDD_PRINT_TASKSEL_PAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CTaskSelectionDlg m_dlgTaskSel;
	TDLPD_STYLE	m_nExportStyle;

// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintTaskSelectionPage)
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTDLPrintTaskSelectionPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Implementation
protected:
};
/////////////////////////////////////////////////////////////////////////////
// CTDLPrintDialog dialog

class CTDLPrintDialog2 : public CTDLDialog
{
// Construction
public:
	CTDLPrintDialog2(LPCTSTR szTitle, BOOL bPreview, FTC_VIEW nView, LPCTSTR szStylesheet,
					const CTDCCustomAttribDefinitionArray& aAttribDefs, 
					BOOL bSupportsExportToImage, CWnd* pParent = NULL);

	CString GetTitle() const { return m_sTitle; }
	COleDateTime GetDate() const;

	TDLPD_STYLE GetExportStyle() const { return m_pageStyle.GetExportStyle(); }
	BOOL GetStylesheet(CString& sStylesheet) const { return m_pageStyle.GetStylesheet(sStylesheet); }

	const CTaskSelectionDlg& GetTaskSelection() const { return m_pageTaskSel.GetTaskSelection(); }

protected:
// Dialog Data
	//{{AFX_DATA(CTDLPrintDialog2)
	enum { IDD = IDD_PRINT_DIALOG2 };
	//}}AFX_DATA
	CString	m_sTitle;
	BOOL	m_bDate;
	BOOL	m_bPrintPreview;
	CHistoryComboBox m_cbTitle;
	
	CTDLPrintStylePage m_pageStyle;
	CTDLPrintTaskSelectionPage m_pageTaskSel;
	CTabbedPropertyPageHost m_ppHost;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDLPrintDialog2)
protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTDLPrintDialog2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDLPRINTDIALOG2_H__1A62F94F_687F_421C_97D2_300BAC4A3E7C__INCLUDED_)

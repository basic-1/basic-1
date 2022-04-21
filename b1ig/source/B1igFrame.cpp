#include "B1igApp.h"
#include "B1igFrame.h"

#include <wx/persist/toplevel.h>
#include <wx/config.h>
#include <wx/fontdata.h>
#include <wx/fontdlg.h>
#include <wx/aboutdlg.h>
#include <wx/artprov.h>
#include <wx/filename.h>

#include "errors.h"

#include "../../common/source/version.h"
#include "../../common/source/gitrev.h"

extern "C"
{
#include "b1dbg.h"
}


// strings for various parameter names, control names, etc.
#define B1_MW_MAIN_WINDOW "MW"
#define B1_MW_AUI_PERSPECTIVE "MWAuiPer"
#define B1_MW_AUI_PROGRAM_PANE_NAME "TxtPrg"
#define B1_MW_AUI_OUTPUT_PANE_NAME "ProgramOutput"
#define B1_MW_AUI_OUTPUT_PANE_WIDTH "MWAuiOPw"
#define B1_MW_AUI_OUTPUT_PANE_HEIGHT "MWAuiOPh"
#define B1_MW_AUI_DEBUG_PANE_NAME "Debug"
#define B1_MW_AUI_DEBUG_PANE_WIDTH "MWAuiDPw"
#define B1_MW_AUI_DEBUG_PANE_HEIGHT "MWAuiDPh"
#define B1_MW_FONT_FAMILY "MWFntFm"
#define B1_MW_FONT_FACE_NAME "MWFntFN"
#define B1_MW_FONT_SIZE "MWFntSz"
#define B1_MW_FONT_STYLE "MWFntSt"
#define B1_MW_FONT_WEIGHT "MWFntNW"
#define B1_WATCH_VAR_GRID_WIDTH_NAME "MWDPVNamW"
#define B1_WATCH_VAR_GRID_WIDTH_TYPE "MWDPVTypW"
#define B1_WATCH_VAR_GRID_WIDTH_VALUE "MWDPVValW"

#define B1_MW_MAIN_WINDOW_DEF_SIZE wxSize(900, 500)
#define B1_MW_OUTPUT_PANE_DEF_SIZE wxSize(420, 350)
#define B1_MW_DEBUG_PANE_DEF_SIZE wxSize(850, 100)


#define B1_MARKER_MASK(MARKER) (1 << (int)(MARKER))


wxBEGIN_EVENT_TABLE(B1igFrame, wxFrame)
    EVT_CLOSE(B1igFrame::OnClose)

    EVT_IDLE(B1igFrame::OnIdle)
    
    EVT_STC_MODIFIED(wxID_TXTPRG, B1igFrame::OnTxtPrgModified)
    
    EVT_TEXT_ENTER(wxID_TXTIN, B1igFrame::OnTxtInEnterPressed)
    EVT_BUTTON(wxID_BTNENTER, B1igFrame::OnTxtInEnterPressed)
    
    EVT_MENU(wxID_NEW, B1igFrame::OnNew)
    EVT_MENU(wxID_OPEN, B1igFrame::OnOpen)
    EVT_MENU(wxID_SAVE, B1igFrame::OnSave)
    EVT_MENU(wxID_SAVEAS, B1igFrame::OnSaveAs)
    EVT_MENU(wxID_EXIT, B1igFrame::OnExit)
    
    EVT_MENU(wxID_UNDO, B1igFrame::OnUndo)
    EVT_MENU(wxID_REDO, B1igFrame::OnRedo)
    EVT_MENU(wxID_CUT, B1igFrame::OnCut)
    EVT_MENU(wxID_COPY, B1igFrame::OnCopy)
    EVT_MENU(wxID_PASTE, B1igFrame::OnPaste)
    EVT_MENU(wxID_DELETE, B1igFrame::OnDelete)
    EVT_MENU(wxID_SELECTALL, B1igFrame::OnSelectAll)
    
    EVT_MENU_OPEN(B1igFrame::OnMenuOpen)
    
    EVT_AUI_PANE_CLOSE(B1igFrame::OnAuiPaneClose)
    
    EVT_MENU(wxID_MENU_OUTPUT_PANE, B1igFrame::OnOutputPane)
    EVT_MENU(wxID_MENU_DEBUG_PANE, B1igFrame::OnDebugPane)
    EVT_MENU(wxID_MENU_DEBUG_PANE_ERRORS, B1igFrame::OnDebugPaneErrors)
    EVT_MENU(wxID_MENU_DEBUG_PANE_VARS, B1igFrame::OnDebugPaneVars)
    
    EVT_MENU(wxID_SELECT_FONT, B1igFrame::OnFont)
    
    EVT_MENU(wxID_MENU_RUN, B1igFrame::OnRun)
    EVT_MENU(wxID_MENU_STOP, B1igFrame::OnStop)
    EVT_MENU(wxID_MENU_STEP, B1igFrame::OnStep)
    EVT_MENU(wxID_MENU_TOGGLE_BREAK, B1igFrame::OnToggleBreak)
    EVT_MENU(wxID_MENU_TERMINATE, B1igFrame::OnTerminate)
    EVT_MENU(wxID_MENU_CLEAR_OUTPUT, B1igFrame::OnClearOutput)

    EVT_MENU(wxID_ABOUT, B1igFrame::OnAbout)
    
    EVT_THREAD(wxID_B1THREAD_STARTED, B1igFrame::OnStart)
    EVT_THREAD(wxID_B1THREAD_BREAK, B1igFrame::OnBreak)
    EVT_THREAD(wxID_B1THREAD_FINISHED, B1igFrame::OnFinish)
    
    EVT_THREAD(wxID_B1THREAD_PRINT, B1igFrame::OnPrint)
    EVT_THREAD(wxID_B1THREAD_NEWLINE, B1igFrame::OnNewLine)
    EVT_THREAD(wxID_B1THREAD_INPUT, B1igFrame::OnInput)

    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_NB_DEBUG, B1igFrame::OnDebugPageClose)

    EVT_GRID_CELL_CHANGED(B1igFrame::OnGridVarsCellChanged)
wxEND_EVENT_TABLE()


wxColour B1igFrame::wxBROWN(0xA5, 0x2A, 0x2A);


B1igFrame::B1igFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, B1_MW_MAIN_WINDOW_DEF_SIZE)
    , m_pInterpreter(NULL)
    , m_nIntState(B1_INT_STATE_IDLE)
    , m_nIntPrevState(B1_INT_STATE_IDLE)
    , m_nCurrentLine(-1)
    , m_nBreakPointsCount(0)
    , m_nErrorLineNum(0)
{
    wxMenuBar *pMenuBar = new wxMenuBar();
    
    // File menu
    wxMenu *pMenu = new wxMenu();
    // to use stock wxArtProvider icons
    //auto pItem = new wxMenuItem(NULL, wxID_NEW, _("&New\tCTRL+N"));
    //pItem->SetBitmaps(wxArtProvider::GetBitmap(wxART_NEW, wxART_MENU));
    //pMenu->Append(pItem);
    pMenu->Append(wxID_NEW, _("&New\tCTRL+N"));
    pMenu->Append(wxID_OPEN, _("&Open\tCTRL+O"));
    pMenu->Append(wxID_SAVE, _("&Save\tCTRL+S"));
    pMenu->Append(wxID_SAVEAS, _("Save &As..."));
    pMenu->AppendSeparator();
    pMenu->Append(wxID_EXIT, _("&Exit\tCTRL+E"));
    pMenuBar->Append(pMenu, _T("&File"));
    
    // Edit menu
    pMenu = new wxMenu();
    pMenu->Append(wxID_UNDO, _("&Undo\tCTRL+Z"));
    pMenu->Append(wxID_REDO, _("&Redo\tCTRL+Y"));
    pMenu->AppendSeparator();
    pMenu->Append(wxID_CUT, _("&Cu&t\tCTRL+X"));
    pMenu->Append(wxID_COPY, _("&Copy\tCTRL+C"));
    pMenu->Append(wxID_PASTE, _("&Paste\tCTRL+V"));
    pMenu->Append(wxID_DELETE, _("&Delete\tDEL"));
    pMenu->Append(wxID_SELECTALL, _("&Select &All\tCTRL+A"));
    pMenuBar->Append(pMenu, _T("&Edit"));
    
    // View menu
    pMenu = new wxMenu();
    wxMenu *pSubmenu = new wxMenu();
    pSubmenu->Append(wxID_MENU_OUTPUT_PANE, _("Program &Output Pane"), _("Show/hide program output pane"), wxITEM_CHECK);
    pSubmenu->Append(wxID_MENU_DEBUG_PANE, _("&Debug Pane"), _("Show/hide debug pane"), wxITEM_CHECK);
    pSubmenu->Append(wxID_MENU_DEBUG_PANE_ERRORS, _("&Errors Page"), _("Show/hide errors page"), wxITEM_CHECK);
    pSubmenu->Append(wxID_MENU_DEBUG_PANE_VARS, _("&Watch Variables Page"), _("Show/hide watch variables page"), wxITEM_CHECK);
    pMenu->AppendSubMenu(pSubmenu, _("&Windows"));
    //pMenu->Append(wxID_ANY, _("&Encoding"));
    pMenu->Append(wxID_SELECT_FONT, _("Change &Font..."));
    pMenuBar->Append(pMenu, _T("&View"));
    
    // Program menu
    pMenu = new wxMenu();
    pMenu->Append(wxID_MENU_RUN, _("&Run/Continue\tF5"), _("Run the program"));
    pMenu->Append(wxID_MENU_STOP, _("&Stop"), _("Stop the program"));
    pMenu->Append(wxID_MENU_STEP, _("St&ep\tF10"), _("Execute current statement"));
    pMenu->Append(wxID_MENU_TOGGLE_BREAK, _("To&ggle Breakpoint\tF9"));
    pMenu->Append(wxID_MENU_TERMINATE, _("&Terminate\tSHIFT+F5"), _("Terminate the program"));
    pMenu->Append(wxID_MENU_CLEAR_OUTPUT, _("&Clear program output"));
    pMenuBar->Append(pMenu, _T("&Program"));
    
    // Help menu
    pMenu = new wxMenu();
    pMenu->Append(wxID_ABOUT, _("&About..."));
    pMenuBar->Append(pMenu, _T("&Help"));
    
    SetMenuBar(pMenuBar);
    
    
    CreateStatusBar(5);
    const int widths[] = {-1, 80, 64, 48, 64};
    SetStatusWidths(5, widths);
    SetStatusText(_("Ready"));
    
    int w, h;

    m_OutputPaneSize = B1_MW_OUTPUT_PANE_DEF_SIZE;
    if( wxConfig::Get()->Read(B1_MW_AUI_OUTPUT_PANE_WIDTH, &w) &&
        wxConfig::Get()->Read(B1_MW_AUI_OUTPUT_PANE_HEIGHT, &h))
    {
        m_OutputPaneSize = wxSize(w, h);
    }
    
    m_DebugPaneSize = B1_MW_DEBUG_PANE_DEF_SIZE;
    if( wxConfig::Get()->Read(B1_MW_AUI_DEBUG_PANE_WIDTH, &w) &&
        wxConfig::Get()->Read(B1_MW_AUI_DEBUG_PANE_HEIGHT, &h))
    {
        m_DebugPaneSize = wxSize(w, h);
    }


    m_Font = wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    int fntFm;
    if(wxConfig::Get()->Read(B1_MW_FONT_FAMILY, &fntFm))
    {
        m_Font.SetFamily((wxFontFamily)fntFm);
    }
    wxString fntFN;
    if(wxConfig::Get()->Read(B1_MW_FONT_FACE_NAME, &fntFN))
    {
        m_Font.SetFaceName(fntFN);
    }
    int fntSz;
    if(wxConfig::Get()->Read(B1_MW_FONT_SIZE, &fntSz))
    {
        m_Font.SetPointSize((wxFontFamily)fntSz);
    }
    int fntSt;
    if(wxConfig::Get()->Read(B1_MW_FONT_STYLE, &fntSt))
    {
        m_Font.SetStyle((wxFontStyle)fntSt);
    }
    int fntNW;
    if(wxConfig::Get()->Read(B1_MW_FONT_WEIGHT, &fntNW))
    {
        m_Font.SetNumericWeight(fntNW);
    }

    
    m_AuiManager.SetManagedWindow(this);
    
    wxAuiPaneInfo pi;
    //create program code text control
    m_pTxtPrg = new wxStyledTextCtrl(this, wxID_TXTPRG);
    // add the program code control to AUI manager
    m_AuiManager.AddPane(m_pTxtPrg, pi.Dockable()
        .Name(B1_MW_AUI_PROGRAM_PANE_NAME).CenterPane().MinSize(100, 100));
    // set default style (0) font
    m_pTxtPrg->StyleSetFont(0, m_Font);
    m_pTxtPrg->SetUseTabs(false);
    m_pTxtPrg->SetBackSpaceUnIndents(true);
    m_pTxtPrg->SetTabWidth(4);
    //m_pTxtPrg->SetIndent(4);
    m_nDefaultEOLMode = m_pTxtPrg->GetEOLMode();

    // create program output panel
    wxPanel *pOutPan = new wxPanel(this, wxID_ANY, wxDefaultPosition, m_OutputPaneSize);
    // create sizer for the panel
    m_pBSOut = new wxBoxSizer(wxVERTICAL);
    // create program output text control on the panel
    m_pTxtOut = new wxTextCtrl(pOutPan, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
    m_pTxtOut->SetFont(m_Font);
    // add the control to the sizer
    m_pBSOut->Add(m_pTxtOut, 1, wxEXPAND, 0);
    // panel with input text control and <Enter> button
    m_pPanIn = new wxPanel(pOutPan, wxID_ANY);
    // create input panel sizer
    wxBoxSizer *pBSIn = new wxBoxSizer(wxHORIZONTAL);
    // create program input text control on the panel
    m_pTxtIn = new wxTextCtrl(m_pPanIn, wxID_TXTIN, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxTE_PROCESS_ENTER);
    m_pTxtIn->SetFont(m_Font);
    pBSIn->Add(m_pTxtIn, 1, wxTOP | wxBOTTOM | wxEXPAND, 4);
    // create <Enter> button
    wxButton *pEnterBtn = new wxButton(m_pPanIn, wxID_BTNENTER, _T("<Enter>"));
    pBSIn->Add(pEnterBtn, 0, wxTOP | wxBOTTOM | wxLEFT | wxEXPAND, 4);
    m_pPanIn->SetSizer(pBSIn);
    // add the input panel to the sizer
    m_pBSOut->Add(m_pPanIn, 0, wxEXPAND, 0);
    // set the panel sizer
    pOutPan->SetSizer(m_pBSOut);
    m_pBSOut->Show(m_pPanIn, false);
    wxAuiPaneInfo pio;
    // add program output panel to AUI manager
    m_AuiManager.AddPane(pOutPan, pio.Dockable().Right()
        .Name(B1_MW_AUI_OUTPUT_PANE_NAME).Caption(_("Program output:")).CaptionVisible().MinSize(100, 100));

    // create wxAuiNotebook control (for debugging pages such as error messages, variables, etc.)
    wxAuiNotebook *pNB = new wxAuiNotebook(this, wxID_NB_DEBUG, wxDefaultPosition, m_DebugPaneSize,
        wxAUI_NB_DEFAULT_STYLE | wxNO_BORDER);
    m_pTxtErr = new wxTextCtrl(pNB, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
            wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
    m_pTxtErr->SetFont(m_Font);
    pNB->AddPage(m_pTxtErr, _("Error messages:"));
    m_pGridVars = new wxGrid(pNB, wxID_GRID_VARS);
    pNB->AddPage(m_pGridVars, _("Watch:"));
    m_pGridVars->SetLabelFont(m_Font);
    m_pGridVars->SetDefaultCellFont(m_Font);

    wxAuiPaneInfo pid;
    // add the dubug control to AUI manager
    m_AuiManager.AddPane(pNB, 
        pid.Dockable().Bottom().Name(B1_MW_AUI_DEBUG_PANE_NAME)
            .Caption(_("Debug:")).CaptionVisible().MinSize(100, 100));

    bool bNoAuiSettings = true;
    
    if(wxPersistentRegisterAndRestore(this, B1_MW_MAIN_WINDOW))
    {
        wxString auipers;
        
        if(wxConfig::Get()->Read(B1_MW_AUI_PERSPECTIVE, &auipers))
        {
            m_AuiManager.LoadPerspective(auipers, false);
            bNoAuiSettings = false;
        }
    }

    m_AuiManager.Update();


    InitGridVars();
    UpdateGridVars();


    // restore watch variables grid column sizes
    int colWidth[3];
    if(wxConfig::Get()->Read(B1_WATCH_VAR_GRID_WIDTH_NAME, colWidth) &&
       wxConfig::Get()->Read(B1_WATCH_VAR_GRID_WIDTH_TYPE, colWidth + 1) &&
       wxConfig::Get()->Read(B1_WATCH_VAR_GRID_WIDTH_VALUE, colWidth + 2))
    {
        m_pGridVars->SetColSize(0, colWidth[0]);
        m_pGridVars->SetColSize(1, colWidth[1]);
        m_pGridVars->SetColSize(2, colWidth[2]);
    }


    // for wxStyledTextCtrl::SaveFile and wxStyledTextCtrl::LoadFile to use UTF-8 encoding
    wxConvCurrent = &wxConvUTF8;
    
    m_pTxtPrg->MarkerDefine(B1_MARKER_BREAKPOINT, wxSTC_MARK_CIRCLE, wxBROWN, wxBROWN);
    m_pTxtPrg->MarkerDefine(B1_MARKER_CURRENT_LINE, wxSTC_MARK_BACKGROUND);
    m_pTxtPrg->MarkerSetBackground(B1_MARKER_CURRENT_LINE, *wxYELLOW);
    m_pTxtPrg->MarkerDefine(B1_MARKER_ERROR, wxSTC_MARK_BACKGROUND);
    m_pTxtPrg->MarkerSetBackground(B1_MARKER_ERROR, wxColour(0xFF, 0x40, 0x40));


    // do not show Debug pane on the first run
    if(bNoAuiSettings)
    {
        wxCommandEvent ceDP(wxEVT_MENU, wxID_MENU_DEBUG_PANE);
        AddPendingEvent(ceDP);
    }

    wxCommandEvent ce;
    OnNew(ce);

    if(wxGetApp().argc > 1)
    {
        wxFileName fn(wxGetApp().argv[1]);
        fn.MakeAbsolute();
        Open(fn.GetFullPath());
    }
}

B1igFrame::~B1igFrame()
{
    wxConfig::Get()->Write(B1_MW_FONT_FAMILY, (int)m_Font.GetFamily());
    wxConfig::Get()->Write(B1_MW_FONT_FACE_NAME, m_Font.GetFaceName());
    wxConfig::Get()->Write(B1_MW_FONT_SIZE, m_Font.GetPointSize());
    wxConfig::Get()->Write(B1_MW_FONT_STYLE, (int)m_Font.GetStyle());
    wxConfig::Get()->Write(B1_MW_FONT_WEIGHT, m_Font.GetNumericWeight());

    wxAuiPaneInfo &pio = m_AuiManager.GetPane(B1_MW_AUI_OUTPUT_PANE_NAME);
    wxConfig::Get()->Write(B1_MW_AUI_OUTPUT_PANE_WIDTH, pio.rect.GetWidth());
    wxConfig::Get()->Write(B1_MW_AUI_OUTPUT_PANE_HEIGHT, pio.rect.GetHeight());
    
    wxAuiPaneInfo &pid = m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME);
    wxConfig::Get()->Write(B1_MW_AUI_DEBUG_PANE_WIDTH, pid.rect.GetWidth());
    wxConfig::Get()->Write(B1_MW_AUI_DEBUG_PANE_HEIGHT, pid.rect.GetHeight());

    int colWidth[3] =
    {
        m_pGridVars->GetColSize(0),
        m_pGridVars->GetColSize(1),
        m_pGridVars->GetColSize(2)
    };

    wxConfig::Get()->Write(B1_WATCH_VAR_GRID_WIDTH_NAME, colWidth[0]);
    wxConfig::Get()->Write(B1_WATCH_VAR_GRID_WIDTH_TYPE, colWidth[1]);
    wxConfig::Get()->Write(B1_WATCH_VAR_GRID_WIDTH_VALUE, colWidth[2]);

    wxConfig::Get()->Write(B1_MW_AUI_PERSPECTIVE, m_AuiManager.SavePerspective());

    m_AuiManager.UnInit();
}

B1Interpreter *B1igFrame::GetInterpreter()
{
    return m_pInterpreter;
}

wxString GetEOLName(int eol)
{
    return eol == wxSTC_EOL_CRLF ? _("CRLF") :
        eol == wxSTC_EOL_CR ? _("CR") : _("LF");
}

void B1igFrame::SetEOLMode()
{
    wxString ln1 = m_pTxtPrg->GetLine(0);

    if(ln1.EndsWith("\r\n"))
    {
        m_pTxtPrg->SetEOLMode(wxSTC_EOL_CRLF);
    }
    else
    if(ln1.EndsWith("\r"))
    {
        m_pTxtPrg->SetEOLMode(wxSTC_EOL_CR);
    }
    else
    if(ln1.EndsWith("\n"))
    {
        m_pTxtPrg->SetEOLMode(wxSTC_EOL_LF);
    }
    else
    {
        m_pTxtPrg->SetEOLMode(m_nDefaultEOLMode);
    }

    SetStatusText(GetEOLName(m_pTxtPrg->GetEOLMode()), 4);
}

bool B1igFrame::IsContentChanged()
{
    return m_pTxtPrg->IsModified();
}

void B1igFrame::UpdateChangedFlag()
{
    bool changed = IsContentChanged();
    wxString title = GetTitle();
    
    if(title.EndsWith("*"))
    {
        if(!changed)
        {
            title.RemoveLast(2);
        }
    }
    else
    {
        if(changed)
        {
            title += " *";
        }
    }
    
    SetTitle(title);
}

void B1igFrame::UpdateTitle()
{
    SetTitle(_("BASIC1 interpreter") + (m_sFileName.IsEmpty() ? wxString() : (" - " + m_sFileName)));
    UpdateChangedFlag();
}

void B1igFrame::OnTxtPrgModified(wxStyledTextEvent &event)
{
    if(m_nErrorLineNum > 0)
    {
        // calling MarkerDelete fires this event again so clear m_nErrorLineNum first
        auto line_num = m_nErrorLineNum;
        m_nErrorLineNum = 0;
        m_pTxtPrg->MarkerDelete(line_num - 1, B1_MARKER_ERROR);
    }

    UpdateChangedFlag();
}

bool B1igFrame::Save(const wxString &sCustomTitle /*= wxEmptyString */)
{
    wxString filename = m_sFileName;

    if(filename.IsEmpty())
    {
        wxFileDialog svdlg(this, sCustomTitle.IsEmpty() ? _("Save file:") : sCustomTitle, "", "", 
            _("Basic files (*.bas, *.bsc)|*.bas;*.bsc|Text files (*.txt)|*.txt|Other files|*.*"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        
        if(svdlg.ShowModal() == wxID_CANCEL)
        {
            return false;
        }
        
        filename = svdlg.GetPath();
    }

    if(m_pTxtPrg->SaveFile(filename))
    {
        m_sFileName = filename;
//        m_pTxtPrg->DiscardEdits();
        m_pTxtPrg->SetSavePoint();
        UpdateTitle();
        return true;
    }
    
    return false;
}

bool B1igFrame::SaveModified(const wxString &text, bool noCancel /*= false */)
{
    if(IsContentChanged())
    {
        int buttons = noCancel ? wxYES_NO : (wxYES_NO | wxCANCEL);
        
        buttons = wxMessageBox(text, _("Save changes"), buttons | wxCENTER, this);
        
        if(buttons == wxCANCEL || (noCancel && buttons == wxNO))
        {
            return false;
        }
        
        if(buttons == wxYES)
        {
            return Save();
        }
    }
    
    return true;
}

bool B1igFrame::TerminateProgram(bool askforterm)
{
    if(askforterm)
    {
        if( wxMessageBox(_("The program is running. Do you want to terminate it?"),
            _("Stop program"), wxYES_NO | wxCENTER, this) == wxNO)
            {
                return false;
            }
    }
    
    wxCommandEvent ce;
    OnTerminate(ce);
    
    return true;
}

void B1igFrame::OnNew(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_IDLE &&
        m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_INPUT)
    {
        return;
    }

    if( m_nIntState == B1_INT_STATE_RUNNING ||
        m_nIntState == B1_INT_STATE_STOPPED ||
        m_nIntState == B1_INT_STATE_INPUT)
    {
        // terminate interpreter and re-trigger the event
        if(!TerminateProgram(true))
        {
            return;
        }

        wxCommandEvent ce(wxEVT_MENU, wxID_NEW);
        AddPendingEvent(ce);
        return;
    }

    if(!SaveModified(m_sFileName.IsEmpty() ?
            _("Save changes before creating new document?") :
            _("Save changed file before creating new document?")
            )
        )
    {
        return;
    }

    m_pTxtPrg->ClearAll();
    m_pTxtPrg->DiscardEdits();
    m_sFileName.Empty();
    SetEOLMode();
    
    UpdateTitle();
    UpdateMenu();
}

void B1igFrame::Open(const wxString &filename)
{
    if(m_pTxtPrg->LoadFile(filename))
    {
        SetEOLMode();
        m_sFileName = filename;
        UpdateTitle();
    }
}

void B1igFrame::OnOpen(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_IDLE &&
        m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_INPUT)
    {
        return;
    }

    if( m_nIntState == B1_INT_STATE_RUNNING ||
        m_nIntState == B1_INT_STATE_STOPPED ||
        m_nIntState == B1_INT_STATE_INPUT)
    {
        // terminate interpreter and re-trigger the event
        if(!TerminateProgram(true))
        {
            return;
        }

        wxCommandEvent ce(wxEVT_MENU, wxID_OPEN);
        AddPendingEvent(ce);
        return;
    }
    
    if(!SaveModified(m_sFileName.IsEmpty() ?
            _("Save changes before opening file?") :
            _("Save changed file before opening another one?")
            )
        )
    {
        return;
    }

    wxFileDialog opdlg(this, _("Open file:"), "", "", 
        _("Basic files (*.bas, *.bsc)|*.bas;*.bsc|Text files (*.txt)|*.txt|Other files|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if(opdlg.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    
    Open(opdlg.GetPath());
}

void B1igFrame::OnSave(wxCommandEvent &event)
{
    Save();
}

void B1igFrame::OnSaveAs(wxCommandEvent &event)
{
    wxString filename = m_sFileName;
    m_sFileName.Empty();
    if(!Save(_("Save file as:")))
    {
        m_sFileName = filename;
    }
}

void B1igFrame::OnClose(wxCloseEvent &event)
{
    if( m_nIntState != B1_INT_STATE_IDLE &&
        m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_INPUT)
    {
        return;
    }

    bool saved = SaveModified(_("Program text is changed do you want to save the changes before quit?"));
    
    if(!saved && event.CanVeto())
    {
        event.Veto();
        return;
    }

    if( m_nIntState == B1_INT_STATE_RUNNING ||
        m_nIntState == B1_INT_STATE_STOPPED ||
        m_nIntState == B1_INT_STATE_INPUT)
    {
        if(!TerminateProgram(event.CanVeto()))
        {
            event.Veto();
            return;
        }
    }
    
    event.Skip();
}

void B1igFrame::OnExit(wxCommandEvent &event)
{
    Close();
}

void B1igFrame::OnIdle(wxIdleEvent &event)
{
    if(!m_sProgramOutput.IsEmpty())
    {
        m_pTxtOut->AppendText(m_sProgramOutput);
        m_sProgramOutput.Empty();
    }

    SetStatusText(wxString::Format(_("Ln %d/%d"), m_pTxtPrg->GetCurrentLine() + 1, m_pTxtPrg->GetLineCount()), 1);
    SetStatusText(wxString::Format(_("Col %d"), m_pTxtPrg->GetColumn(m_pTxtPrg->GetCurrentPos()) + 1), 2);
    SetStatusText(m_pTxtPrg->GetOvertype() ? _("OVR") : _("INS"), 3);
}

void B1igFrame::OnUndo(wxCommandEvent &event)
{
    if(m_pTxtPrg->HasFocus())
    {
        m_pTxtPrg->Undo();
    }
}

void B1igFrame::OnRedo(wxCommandEvent &event)
{
    if(m_pTxtPrg->HasFocus())
    {
        m_pTxtPrg->Redo();
    }
}

void B1igFrame::OnCut(wxCommandEvent &event)
{
    m_pTxtPrg->Cut();
}

void B1igFrame::OnCopy(wxCommandEvent &event)
{
    m_pTxtPrg->Copy();
}

void B1igFrame::OnPaste(wxCommandEvent &event)
{
    m_pTxtPrg->Paste();
}

void B1igFrame::OnDelete(wxCommandEvent &event)
{
    m_pTxtPrg->Clear();
}

void B1igFrame::OnSelectAll(wxCommandEvent &event)
{
    m_pTxtPrg->SelectAll();
}

void B1igFrame::OnAuiPaneClose(wxAuiManagerEvent &event)
{
    wxAuiPaneInfo *pi = event.GetPane();
    
    if(pi->name == B1_MW_AUI_OUTPUT_PANE_NAME)
    {
        m_OutputPaneSize = wxSize(pi->rect.GetWidth(), pi->rect.GetHeight());
    }

    if(pi->name == B1_MW_AUI_DEBUG_PANE_NAME)
    {
        m_DebugPaneSize = wxSize(pi->rect.GetWidth(), pi->rect.GetHeight());
    }
}

void B1igFrame::OnMenuOpen(wxMenuEvent &event)
{
    bool dbgenabled = false;

    auto item = event.GetMenu()->FindItem(wxID_MENU_OUTPUT_PANE);
    if(item != NULL)
    {
        item->Check(m_AuiManager.GetPane(B1_MW_AUI_OUTPUT_PANE_NAME).IsShown());
    }
    
    item = event.GetMenu()->FindItem(wxID_MENU_DEBUG_PANE);
    if(item != NULL)
    {
        dbgenabled = m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).IsShown();
        item->Check(dbgenabled);
    }

    auto pNB = static_cast<wxAuiNotebook *>(m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).window);
    item = event.GetMenu()->FindItem(wxID_MENU_DEBUG_PANE_ERRORS);
    if(item != NULL)
    {
        item->Check(pNB->GetPageIndex(m_pTxtErr) != wxNOT_FOUND);
        item->Enable(dbgenabled);
    }

    item = event.GetMenu()->FindItem(wxID_MENU_DEBUG_PANE_VARS);
    if(item != NULL)
    {
        item->Check(pNB->GetPageIndex(m_pGridVars) != wxNOT_FOUND);
        item->Enable(dbgenabled);
    }
}

void B1igFrame::OnOutputPane(wxCommandEvent &event)
{
    wxAuiPaneInfo &pi = m_AuiManager.GetPane(B1_MW_AUI_OUTPUT_PANE_NAME);
    pi.Show(event.IsChecked());
    if(event.IsChecked())
    {
        pi.BestSize(m_OutputPaneSize);
    }
    m_AuiManager.Update();
}

void B1igFrame::OnDebugPane(wxCommandEvent &event)
{
    wxAuiPaneInfo &pi = m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME);
    pi.Show(event.IsChecked());
    if(event.IsChecked())
    {
        pi.BestSize(m_DebugPaneSize);
    }
    m_AuiManager.Update();
}

void B1igFrame::OnFont(wxCommandEvent &event)
{
    wxFontData fd;
    fd.SetInitialFont(m_pTxtPrg->StyleGetFont(0));
    fd.EnableEffects(false);
    wxFontDialog fntdlg(this, fd);
    if(fntdlg.ShowModal() != wxID_CANCEL)
    {
        m_Font = fntdlg.GetFontData().GetChosenFont();
        m_pTxtPrg->StyleSetFont(0, m_Font);
        m_pTxtOut->SetFont(m_Font);
        m_pTxtIn->SetFont(m_Font);
        m_pTxtErr->SetFont(m_Font);
        m_pGridVars->SetLabelFont(m_Font);
        m_pGridVars->SetDefaultCellFont(m_Font);
        UpdateGridVarsFont();
    }
}

void B1igFrame::UpdateBreakpoints()
{
    m_pInterpreter->IntBrkPntRemoveAll();
    
    int linecnt = m_pTxtPrg->GetLineCount();
    for(int linenum = 0; linenum < linecnt; linenum++)
    {
        if(m_pTxtPrg->MarkerGet(linenum) & B1_MARKER_MASK(B1_MARKER_BREAKPOINT))
        {
            m_pInterpreter->IntBrkPntAdd(linenum + 1);
        }
    }
}

wxString B1igFrame::HideInputCtrl()
{
    // hide input control
    wxString input = m_pTxtIn->GetValue();
    m_pBSOut->Show(m_pPanIn, false);
    m_pTxtIn->Clear();
    m_pTxtIn->DiscardEdits();
    m_pBSOut->Layout();
    m_pTxtPrg->SetFocus();

    // restore state
    m_nIntState = m_nIntPrevState;

    return input;
}

void B1igFrame::InterruptInput()
{
    HideInputCtrl();
    // send interruption signal to interpreter
    m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_INPUT_INTERRUPT, 0));
}

void B1igFrame::OnRun(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_IDLE &&
        m_nIntState != B1_INT_STATE_STOPPED)
    {
        return;
    }
    
    if(!SaveModified(_("The program text is changed and should be saved before run. Continue?"), true))
    {
        return;
    }

    bool start = (m_nIntState == B1_INT_STATE_IDLE);
    
    if(start)
    {
        m_pTxtPrg->SetReadOnly(true);
        m_pInterpreter = new B1Interpreter(m_pTxtPrg->GetText());
    }
    else
    {
        if(m_nCurrentLine >= 0)
        {
            m_pTxtPrg->MarkerDelete(m_nCurrentLine, B1_MARKER_CURRENT_LINE);
        }
    }

    UpdateBreakpoints();
    
    if(start)
    {
        m_pInterpreter->Run();
    }

    m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_RUN, 0));
    m_nIntState = B1_INT_STATE_RUNNING;
    
    UpdateMenu();
}

void B1igFrame::OnStop(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_RUNNING)
    {
        return;
    }
    
    m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_STOP, 0));
    m_pInterpreter->IntStop();
    m_nIntState = B1_INT_STATE_STOPPING;
    UpdateMenu();
 }

void B1igFrame::OnStep(wxCommandEvent &event)
{
    if(m_nIntState != B1_INT_STATE_STOPPED)
    {
        return;
    }
    
    UpdateBreakpoints();

    m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_STEP, 0));
}

void B1igFrame::OnToggleBreak(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_IDLE &&
        m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_INPUT)
    {
        return;
    }
    
    m_nBreakpointLineNum = m_pTxtPrg->GetCurrentLine();
    
    if(m_pTxtPrg->MarkerGet(m_nBreakpointLineNum) & B1_MARKER_MASK(B1_MARKER_BREAKPOINT))
    {
        m_pTxtPrg->MarkerDelete(m_nBreakpointLineNum, B1_MARKER_BREAKPOINT);
        m_nBreakPointsCount--;
        if(m_nIntState == B1_INT_STATE_RUNNING)
        {
            m_nIntState = B1_INT_STATE_REMOVING_BREAKPOINT;
            m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_STOP, 0));
            m_pInterpreter->IntStop();
        }
    }
    else
    {
        if(m_nBreakPointsCount == B1_MAX_BREAKPOINT_NUM)
        {
            return;
        }
        
        m_pTxtPrg->MarkerAdd(m_nBreakpointLineNum, B1_MARKER_BREAKPOINT);
        m_nBreakPointsCount++;
        if(m_nIntState == B1_INT_STATE_RUNNING)
        {
            m_nIntState = B1_INT_STATE_ADDING_BREAKPOINT;
            m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_STOP, 0));
            m_pInterpreter->IntStop();
        }
    }

    if(m_nIntState == B1_INT_STATE_INPUT)
    {
        UpdateBreakpoints();
    }
}

void B1igFrame::OnTerminate(wxCommandEvent &event)
{
    if( m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_INPUT)
    {
        return;
    }

    if(m_nIntState == B1_INT_STATE_INPUT)
    {
        InterruptInput();
    }
    else
    {
        m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_TERMINATE, 0));
        m_pInterpreter->IntStop();
    }

    m_pInterpreter->Wait();
    delete m_pInterpreter;
    m_pInterpreter = NULL;
}

void B1igFrame::OnClearOutput(wxCommandEvent &event)
{
    m_pTxtOut->Clear();
    m_pTxtOut->DiscardEdits();
}

void B1igFrame::OnAbout(wxCommandEvent &event)
{
    wxAboutDialogInfo adi;
    adi.SetName(_("BASIC1 interpreter"));
    adi.SetDescription(_("just one more BASIC interpreter"));
    wxString version = B1_CORE_VERSION;
#ifdef B1_GIT_REVISION
    version += wxString(".") + B1_GIT_REVISION;
#endif
    adi.SetVersion(version);
    adi.SetCopyright(_("(c) 2020-2021 Nikolay Pletnev\nMIT license"));
    //adi.SetLicense(_("license text"));
    wxAboutBox(adi, this);
}

void B1igFrame::OnStart(wxThreadEvent &event)
{

}

void B1igFrame::OnBreak(wxThreadEvent &event)
{
    if( m_nIntState != B1_INT_STATE_RUNNING &&
        m_nIntState != B1_INT_STATE_STOPPING &&
        m_nIntState != B1_INT_STATE_STOPPED &&
        m_nIntState != B1_INT_STATE_ADDING_BREAKPOINT &&
        m_nIntState != B1_INT_STATE_REMOVING_BREAKPOINT)
    {
        return;
    }
    
    if(m_nIntState == B1_INT_STATE_ADDING_BREAKPOINT)
    {
        m_pInterpreter->IntBrkPntAdd(m_nBreakpointLineNum + 1);
        m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_RUN, 0));
        m_nIntState = B1_INT_STATE_RUNNING;
    }
    else
    if(m_nIntState == B1_INT_STATE_REMOVING_BREAKPOINT)
    {
        m_pInterpreter->IntBrkPntRemove(m_nBreakpointLineNum + 1);
        m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_RUN, 0));
        m_nIntState = B1_INT_STATE_RUNNING;
    }
    else
    {
        m_nIntState = B1_INT_STATE_STOPPED;
        
        if(m_nCurrentLine >= 0)
        {
            m_pTxtPrg->MarkerDelete(m_nCurrentLine, B1_MARKER_CURRENT_LINE);
        }
        
        m_nCurrentLine = event.GetInt() - 1;
        m_pTxtPrg->MarkerAdd(m_nCurrentLine, B1_MARKER_CURRENT_LINE);
        m_pTxtPrg->GotoLine(m_nCurrentLine);
        
        UpdateMenu();

        FillGridVars();
    }
}

void B1igFrame::OnFinish(wxThreadEvent &event)
{
    if(m_pInterpreter != NULL)
    {
        m_pInterpreter->Wait();
        delete m_pInterpreter;
        m_pInterpreter = NULL;
    }
    
    m_nIntState = B1_INT_STATE_IDLE;
    
    if(m_nCurrentLine >= 0)
    {
        m_pTxtPrg->MarkerDelete(m_nCurrentLine, B1_MARKER_CURRENT_LINE);
    }

    m_pTxtPrg->SetReadOnly(false);
    
    int err = event.GetInt();
    if(err != B1_RES_OK && err != B1_RES_STOP && err != B1_RES_END)
    {
        // print error message
        int line_num = event.GetExtraLong();
        wxString err_msg = line_num > 0 ?
            wxString::Format(_("error %d on line %d: %s"), err, line_num, _err_msgs[err - B1_RES_FIRSTERRCODE]) :
            wxString::Format(_("error %d: %s"), err, _err_msgs[err - B1_RES_FIRSTERRCODE]);
        m_pTxtErr->AppendText(err_msg + '\n');

        // show errors page
        auto pNB = static_cast<wxAuiNotebook *>(m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).window);       
        auto i = pNB->GetPageIndex(m_pTxtErr);
        if(i != wxNOT_FOUND)
        {
            pNB->SetSelection(i);
        }
        else
        {
            pNB->AddPage(m_pTxtErr, _("Error messages:"), true);
        }

        // go to the program line that caused the error
        if(line_num > 0)
        {
            // set m_nErrorLineNum after MarkerAdd call because it fires EVT_STC_MODIFIED event
            // which clears error marker
            m_pTxtPrg->MarkerAdd(line_num - 1, B1_MARKER_ERROR);
            m_nErrorLineNum = line_num;
            m_pTxtPrg->GotoLine(m_nErrorLineNum - 1);
        }

        m_pTxtPrg->SetFocus();
    }

    m_sProgramOutput += '\n';
    
    UpdateMenu();
}

void B1igFrame::OnPrint(wxThreadEvent &event)
{
    m_sProgramOutput += event.GetString();
}

void B1igFrame::OnNewLine(wxThreadEvent &event)
{
    m_sProgramOutput += '\n';
}

void B1igFrame::OnInput(wxThreadEvent &event)
{
    m_pBSOut->Show(m_pPanIn);
    m_pBSOut->Layout();
    m_pTxtIn->SetFocus();
    m_nIntPrevState = m_nIntState;
    m_nIntState = B1_INT_STATE_INPUT;
    UpdateMenu();
}

void B1igFrame::OnTxtInEnterPressed(wxCommandEvent &event)
{
    wxString input = HideInputCtrl() + '\n';
    // send entered string to interpreter
    m_pInterpreter->SetInputString(input);
    m_pInterpreter->Post(std::pair<int, int>(wxID_B1THREAD_INPUT, 0));
    
    UpdateMenu(); 
}

void B1igFrame::UpdateMenu()
{
    GetMenuBar()->FindItem(wxID_MENU_RUN)->Enable(m_nIntState == B1_INT_STATE_IDLE ||
        m_nIntState == B1_INT_STATE_STOPPED);
    GetMenuBar()->FindItem(wxID_MENU_STOP)->Enable(m_nIntState == B1_INT_STATE_RUNNING);
    GetMenuBar()->FindItem(wxID_MENU_STEP)->Enable(m_nIntState == B1_INT_STATE_STOPPED);
    GetMenuBar()->FindItem(wxID_MENU_TOGGLE_BREAK)->Enable(m_nIntState == B1_INT_STATE_IDLE ||
        m_nIntState == B1_INT_STATE_RUNNING || m_nIntState == B1_INT_STATE_STOPPED ||
        m_nIntState == B1_INT_STATE_INPUT);
    GetMenuBar()->FindItem(wxID_MENU_TERMINATE)->Enable(m_nIntState == B1_INT_STATE_RUNNING ||
        m_nIntState == B1_INT_STATE_STOPPED || m_nIntState == B1_INT_STATE_INPUT);

    wxString sttext = _("Ready");
    switch(m_nIntState)
    {
	case B1_INT_STATE_RUNNING:
            sttext = _("Running...");
            break;
	case B1_INT_STATE_STOPPING:
            sttext = _("Stopping...");
            break;
	case B1_INT_STATE_STOPPED:
            sttext = _("Stopped");
            break;
	case B1_INT_STATE_INPUT:
            sttext = _("Input");
            break;
    }
    SetStatusText(sttext);
}

void B1igFrame::OnDebugPageClose(wxAuiNotebookEvent &event)
{
    auto pNB = static_cast<wxAuiNotebook *>(m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).window);
    auto page = pNB->GetPage(event.GetSelection());
    pNB->RemovePage(event.GetSelection());
    page->Show(false);
    event.Veto();
}

void B1igFrame::OnDebugPaneErrors(wxCommandEvent &event)
{
    auto pNB = static_cast<wxAuiNotebook *>(m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).window);       

    if(event.IsChecked())
    {
        pNB->AddPage(m_pTxtErr, _("Error messages:"), true);
    }
    else
    {
        auto i = pNB->GetPageIndex(m_pTxtErr);
        auto page = pNB->GetPage(i);
        pNB->RemovePage(i);
        page->Show(false);
    }
}

void B1igFrame::OnDebugPaneVars(wxCommandEvent &event)
{
    auto pNB = static_cast<wxAuiNotebook *>(m_AuiManager.GetPane(B1_MW_AUI_DEBUG_PANE_NAME).window);       

    if(event.IsChecked())
    {
        pNB->AddPage(m_pGridVars, _("Watch:"), true);
    }
    else
    {
        auto i = pNB->GetPageIndex(m_pGridVars);
        auto page = pNB->GetPage(i);
        pNB->RemovePage(i);
        page->Show(false);
    }
}

void B1igFrame::UpdateGridVarsFont()
{
    m_pGridVars->BeginBatch();

    m_pGridVars->AutoSizeColLabelSize(0);
    auto width = m_pGridVars->GetColSize(0);
    m_pGridVars->SetColSize(0, width > 100 ? width : 100);

    m_pGridVars->AutoSizeColLabelSize(1);
    width = m_pGridVars->GetColSize(1);
    m_pGridVars->SetColSize(1, width > 100 ? width : 100);

    width = 0;
    auto cols = m_pGridVars->GetNumberCols() - 1;
    for(auto c = 0; c < cols; c++)
    {
        width += m_pGridVars->GetColSize(c);
    }
    width = m_pGridVars->GetClientSize().GetWidth() - width;
    m_pGridVars->SetColSize(cols, width > 100 ? width : 100);

    auto rowheight = m_pTxtErr->GetTextExtent(_("Name")).GetHeight() + 4;
    m_pGridVars->SetColLabelSize(rowheight);
    m_pGridVars->SetDefaultRowSize(rowheight, true);

    m_pGridVars->EndBatch();
}

void B1igFrame::InitGridVars()
{
    m_pGridVars->HideRowLabels();
    m_pGridVars->CreateGrid(0, 3);
    m_pGridVars->SetColLabelValue(0, _("Name"));
    m_pGridVars->SetColLabelValue(1, _("Type"));
    m_pGridVars->SetColLabelValue(2, _("Value"));
    UpdateGridVarsFont();
}

int B1igFrame::UpdateGridVars(int rowtocheck /*= -1*/)
{
    m_pGridVars->BeginBatch();

    auto rows = m_pGridVars->GetNumberRows();

    for(auto r = 0; r < rows; r++)
    {
        if(m_pGridVars->GetCellValue(r, 0).IsEmpty())
        {
            if(r == rowtocheck)
            {
                rowtocheck = -1;
            }

            m_pGridVars->DeleteRows(r);
            r--;
            rows--;
        }
    }
    m_pGridVars->AppendRows();
    m_pGridVars->SetReadOnly(rows, 1, true);
    m_pGridVars->SetReadOnly(rows, 2, true);

    m_pGridVars->EndBatch();

    return rowtocheck;
}

void B1igFrame::OnGridVarsCellChanged(wxGridEvent &event)
{
    // update the grid and check that the changed row is not deleted
    int row = UpdateGridVars(event.GetRow());
    if(m_nIntState == B1_INT_STATE_STOPPED && row >= 0)
    {
        // evaluate changed expression
        FillGridVars(row);
    }
}

void B1igFrame::FillGridVars(int row /*= -1*/)
{
    auto r = row >= 0 ? row : 0;
    auto rows = row >= 0 ? row + 1 : m_pGridVars->GetNumberRows();

    m_pGridVars->BeginBatch();

    for(; r < rows; r++)
    {
        wxString expr = m_pGridVars->GetCellValue(r, 0);
        if(!expr.IsEmpty())
        {
            wxString type, val;
            auto err = m_pInterpreter->Eval(expr, type, val);
            if(err != B1_RES_OK)
            {
                val = wxString::Format(_("error %d: %s"), err, _err_msgs[err - B1_RES_FIRSTERRCODE]);
            }
            m_pGridVars->SetCellValue(r, 1, type);
            m_pGridVars->SetCellValue(r, 2, val);
        }
    }

    m_pGridVars->EndBatch();
}
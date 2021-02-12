#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/stc/stc.h>
#include <wx/grid.h>

#include "B1Interpreter.h"


enum
{
    wxID_TXTPRG = wxID_HIGHEST + 100,
    wxID_TXTIN,
    wxID_BTNENTER,
    wxID_NB_DEBUG,
    wxID_GRID_VARS,
    
    wxID_MENU_OUTPUT_PANE,
    wxID_MENU_DEBUG_PANE,
    wxID_MENU_DEBUG_PANE_ERRORS,
    wxID_MENU_DEBUG_PANE_VARS,
    
    wxID_MENU_RUN,
    wxID_MENU_STOP,
    wxID_MENU_STEP,
    wxID_MENU_TOGGLE_BREAK,
    wxID_MENU_TERMINATE,
    wxID_MENU_CLEAR_OUTPUT,

    // sent by interpreter thread on start
    wxID_B1THREAD_STARTED,
    // sent by interpreter thread on STOP statement or breakpoint
    wxID_B1THREAD_BREAK,
    // sent by interpreter thread on finish working
    wxID_B1THREAD_FINISHED,
    // sent by interpreter thread to print a string
    wxID_B1THREAD_PRINT,
    // sent by interpreter thread to move printing caret on a new line
    wxID_B1THREAD_NEWLINE,
    
    wxID_B1THREAD_INPUT,

    wxID_B1THREAD_INPUT_INTERRUPT,

    wxID_B1THREAD_RUN,
    // sent by GUI thread, request for execution stop
    wxID_B1THREAD_STOP,
    // sent by GUI thread, request for a single program line execution
    wxID_B1THREAD_STEP,
    // sent by GUI thread, request to terminate execution
    wxID_B1THREAD_TERMINATE,
    
};

enum
{
    B1_MARKER_BREAKPOINT = 0,
    B1_MARKER_CURRENT_LINE = 1,
};

enum B1_INT_STATE
{
    B1_INT_STATE_IDLE = 1,
    B1_INT_STATE_RUNNING = 2,
    B1_INT_STATE_STOPPING = 4,
    B1_INT_STATE_STOPPED = 8,
    B1_INT_STATE_ADDING_BREAKPOINT = 16,
    B1_INT_STATE_REMOVING_BREAKPOINT = 32,
    B1_INT_STATE_INPUT = 64,
};


class B1igFrame : public wxFrame
{
public:
    static wxColour wxBROWN;

    B1igFrame(const wxString &title);
    ~B1igFrame();
    
    B1Interpreter *GetInterpreter();


protected:
    B1Interpreter *m_pInterpreter;
    
    wxAuiManager m_AuiManager;
    
    wxStyledTextCtrl *m_pTxtPrg;

    wxTextCtrl *m_pTxtOut;
    wxTextCtrl *m_pTxtIn;
    wxBoxSizer *m_pBSOut;
    wxPanel *m_pPanIn;

    wxTextCtrl *m_pTxtErr;
    wxGrid *m_pGridVars;

    wxFont m_Font;
    
    wxSize m_DebugPaneSize;
    wxSize m_OutputPaneSize;
    
    wxString m_sFileName;
    
    int m_nIntState;
    int m_nIntPrevState;
    
    int m_nCurrentLine;
    
    int m_nBreakPointsCount;
    int m_nBreakpointLineNum;

    wxString m_sProgramOutput;

    int m_nDefaultEOLMode;
    
    
    wxDECLARE_EVENT_TABLE();
    
    
    void OnClose(wxCloseEvent &event);

    void OnIdle(wxIdleEvent &event);

    void OnTxtPrgModified(wxStyledTextEvent &event);
    
    void OnTxtInEnterPressed(wxCommandEvent &event);
    
    void OnNew(wxCommandEvent &event);
    void OnOpen(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnSaveAs(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    
    void OnUndo(wxCommandEvent &event);
    void OnRedo(wxCommandEvent &event);
    void OnCut(wxCommandEvent &event);
    void OnCopy(wxCommandEvent &event);
    void OnPaste(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);
    void OnSelectAll(wxCommandEvent &event);
    
    void OnAuiPaneClose(wxAuiManagerEvent &event);
    
    void OnMenuOpen(wxMenuEvent &event);
    
    void OnOutputPane(wxCommandEvent &event);
    void OnDebugPane(wxCommandEvent &event);
    void OnDebugPaneErrors(wxCommandEvent &event);
    void OnDebugPaneVars(wxCommandEvent &event);
    
    void OnFont(wxCommandEvent &event);

    void SetEOLMode();    
    bool IsContentChanged();
    bool SaveModified(const wxString &text, bool noCancel = false);
    void UpdateChangedFlag();
    void UpdateTitle();
    bool Save(const wxString &sCustomTitle = wxEmptyString);
    bool TerminateProgram(bool askforterm);
    
    void UpdateBreakpoints();
    wxString HideInputCtrl();
    void InterruptInput();
    
    void OnRun(wxCommandEvent &event);
    void OnStop(wxCommandEvent &event);
    void OnStep(wxCommandEvent &event);
    void OnToggleBreak(wxCommandEvent &event);
    void OnTerminate(wxCommandEvent &event);
    void OnClearOutput(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    void OnStart(wxThreadEvent &event);
    void OnBreak(wxThreadEvent &event);
    void OnFinish(wxThreadEvent &event);
    
    void OnPrint(wxThreadEvent &event);
    void OnNewLine(wxThreadEvent &event);
    void OnInput(wxThreadEvent &event);
    
    void UpdateMenu();

    void OnDebugPageClose(wxAuiNotebookEvent &event);

    void InitGridVars();
    void UpdateGridVarsFont();
    int UpdateGridVars(int rowtocheck = -1);
    void FillGridVars(int row = -1);

    void OnGridVarsCellChanged(wxGridEvent &event);
};

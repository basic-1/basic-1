#pragma once

#include <wx/wx.h>
#include <wx/msgqueue.h>

extern "C"
{
#include "b1itypes.h"
}


class B1Interpreter : protected wxTimer, public wxMessageQueue<std::pair<int, int>>, public wxThread
{
protected:
    wxCriticalSection m_PrintCS;
    wxString m_sPrintBuffer;
    
    wxString m_sInput;
    int m_nNextChar;

    wxString m_sProgram;
    
    B1_T_ERROR LoadProgram();


    // from wxThread
    wxThread::ExitCode Entry() override;
    

    // from wxTimer
    void Notify() override;


    wxString fromb1str(const B1_T_CHAR *s);


public:
    B1Interpreter() = delete;
    B1Interpreter(const B1Interpreter &) = delete;
    B1Interpreter &operator =(const B1Interpreter &) = delete;

    B1Interpreter(const wxString &program);

    virtual ~B1Interpreter();


    void PrintChar(B1_T_CHAR c);
    void NewLine();
    bool InputChar(B1_T_CHAR *c);
    
    
    // interrupts program execution, can be called from other threads
    void IntStop();
    // adds breakpoint, can be called only when program is not executed
    void IntBrkPntAdd(int linenum);
    // remove breakpoint, can be called only when program is not executed
    void IntBrkPntRemove(int linenum);
    // remove all breakpoints, can be called only when program is not executed
    void IntBrkPntRemoveAll();
    
    
    void SetInputString(const wxString &s);

    B1_T_ERROR Eval(const wxString &expr, wxString &type, wxString &val);
};

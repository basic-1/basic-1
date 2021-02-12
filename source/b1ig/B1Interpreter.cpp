#include "B1igApp.h"
#include "B1Interpreter.h"
#include "B1igFrame.h"
#include "errors.h"

#include <wx/txtstrm.h>
#include <wx/sstream.h>


extern "C"
{
#include "b1ex.h"
#include "b1eval.h"
#include "b1int.h"
#include "b1dbg.h"
}


extern B1_T_ERROR b1_ex_prg_set_program(const std::vector<std::vector<B1_T_CHAR>> *program_lines);


#define GETINT (static_cast<B1igFrame *>(wxGetApp().GetTopWindow()))->GetInterpreter()


extern "C" B1_T_ERROR b1_ex_io_print_char(B1_T_CHAR c)
{
    GETINT->PrintChar(c);
    return B1_RES_OK;
}

extern "C" B1_T_ERROR b1_ex_io_print_newline()
{
    GETINT->NewLine();
    return B1_RES_OK;
}

extern "C" B1_T_ERROR b1_ex_io_print_margin_newline()
{
    GETINT->NewLine();
    return B1_RES_OK;
}

extern "C" B1_T_ERROR b1_ex_io_input_char(B1_T_CHAR *c)
{
    if(!GETINT->InputChar(c))
    {
        wxThreadEvent *te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_INPUT);
        wxQueueEvent(wxGetApp().GetTopWindow(), te);
        
        // wait for input end
        std::pair<int, int> msg;
        wxMessageQueueError mqerr = GETINT->Receive(msg);
        if(mqerr == wxMSGQUEUE_MISC_ERROR)
        {
            return B1_RES_EMSQUFAT;
        }

        if(msg.first == wxID_B1THREAD_INPUT_INTERRUPT)
        {
            return B1_RES_EININTR;
        }
        else
        if(msg.first != wxID_B1THREAD_INPUT)
        {
            return B1_RES_EENVFAT;
        }
        
        if(!GETINT->InputChar(c))
        {
            return B1_RES_EEOF;
        }
    }
    
    return B1_RES_OK;
}
    
    
B1Interpreter::B1Interpreter(const wxString &program)
    : wxTimer()
    , wxMessageQueue<std::pair<int, int>>()
    , wxThread(wxTHREAD_JOINABLE)
    , m_nNextChar(0)
{
    wxThreadEvent *te;

    m_sProgram = program;

    // initialize interpreter
    B1_T_ERROR err = b1_int_reset();
    if(err != B1_RES_OK)
    {
        te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_FINISHED);
        te->SetInt(err);
        te->SetExtraLong(-1);
        wxQueueEvent(wxGetApp().GetTopWindow(), te);
        return;
    }
    b1_int_input_echo = 1;

    // load program
    err = LoadProgram();
    if(err != B1_RES_OK)
    {
        te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_FINISHED);
        te->SetInt(err);
        te->SetExtraLong(-1);
        wxQueueEvent(wxGetApp().GetTopWindow(), te);
    }
}

B1Interpreter::~B1Interpreter()
{
    b1_int_reset();
}

void B1Interpreter::PrintChar(B1_T_CHAR c)
{
    wxCriticalSectionLocker l(m_PrintCS);
    m_sPrintBuffer += (wchar_t)c;
}

void B1Interpreter::NewLine()
{
    wxTimer::Stop();
    Notify();

    wxThreadEvent *te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_NEWLINE);
    wxQueueEvent(wxGetApp().GetTopWindow(), te);
}

B1_T_ERROR B1Interpreter::LoadProgram()
{
    std::vector<std::vector<B1_T_CHAR>> lines;
    wxStringInputStream sis(m_sProgram);
    wxTextInputStream tis(sis);

    while(!tis.GetInputStream().Eof())
    {
        wxString s = tis.ReadLine();
        std::vector<B1_T_CHAR> line;
        
        for(const auto c : s)
        {
			/*if(c.isSurrogate())
			{
				return B1_RES_ECVTUCS2;
			}*/

            line.push_back((B1_T_CHAR)(wchar_t)c);
        }
        
        line.push_back((B1_T_CHAR)0);

        lines.push_back(line);
    }
    
    return b1_ex_prg_set_program(&lines);
}

void B1Interpreter::Notify()
{
    wxThreadEvent *te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_PRINT);
    {
        wxCriticalSectionLocker l(m_PrintCS);
        te->SetString(m_sPrintBuffer.Clone());
        m_sPrintBuffer.Empty();
    }
    wxQueueEvent(wxGetApp().GetTopWindow(), te);
    wxTimer::StartOnce();
}

wxThread::ExitCode B1Interpreter::Entry()
{
    wxThreadEvent *te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_STARTED);
    wxQueueEvent(wxGetApp().GetTopWindow(), te);
    
    // perform the first program run
    B1_T_ERROR err = b1_int_prerun();
    if(err != B1_RES_OK)
    {
        te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_FINISHED);
        te->SetInt(err);
        te->SetExtraLong(-1);
        wxQueueEvent(wxGetApp().GetTopWindow(), te);
        return (wxThread::ExitCode)0;
    }
    
    wxTimer::StartOnce(100);

    // program executing loop
    std::pair<int, int> msg;
    wxMessageQueueError mqerr;
    bool run = false;
    err = B1_RES_OK;

    while(true)
    {
        if(run)
        {
            err = b1_int_run();
            
            if(err == B1_RES_STOP)
            {
                run = false;
                B1_T_PROG_LINE_CNT line_cnt;
                err = b1_dbg_get_break_line_cnt(&line_cnt);
                if(err != B1_RES_OK)
                {
                    break;
                }
                te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_BREAK);
                te->SetInt(line_cnt);
                wxQueueEvent(wxGetApp().GetTopWindow(), te);
            }
            else
            {
                break;
            }
        }
        
        mqerr = Receive(msg);
        if(mqerr == wxMSGQUEUE_MISC_ERROR)
        {
            err = B1_RES_EMSQUFAT;
            break;
        }
        
        if(msg.first == wxID_B1THREAD_RUN)
        {
            run = true;
            b1_int_exec_stop = 0;
        }
        else
        if(msg.first == wxID_B1THREAD_STOP)
        {
            run = false;
        }
        else
        if(msg.first == wxID_B1THREAD_STEP)
        {
            run = true;
            b1_int_exec_stop = 1;
        }
        else
        if(msg.first == wxID_B1THREAD_TERMINATE)
        {
            err = B1_RES_OK;
            break;
        }
    }
    
    wxTimer::Stop();
    Notify();
    wxTimer::Stop();
    
    te = new wxThreadEvent(wxEVT_THREAD, wxID_B1THREAD_FINISHED);
    te->SetInt(err);
    te->SetExtraLong(b1_int_curr_prog_line_cnt);
    wxQueueEvent(wxGetApp().GetTopWindow(), te);
    return (wxThread::ExitCode)0;
}

void B1Interpreter::IntStop()
{
    b1_int_exec_stop = 1;
}

void B1Interpreter::IntBrkPntAdd(int linenum)
{
    b1_dbg_add_breakpoint(linenum);
}

void B1Interpreter::IntBrkPntRemove(int linenum)
{
    b1_dbg_remove_breakpoint(linenum);
}

void B1Interpreter::IntBrkPntRemoveAll()
{
    b1_dbg_remove_all_breakpoints();
}

void B1Interpreter::SetInputString(const wxString &s)
{
    m_sInput += s;
}

bool B1Interpreter::InputChar(B1_T_CHAR *c)
{
    if(m_sInput.IsEmpty())
    {
        return false;
    }
    
    if((int)m_sInput.Length() == m_nNextChar)
    {
        m_sInput.Empty();
        m_nNextChar = 0;
        return false;
    }
    
    *c = (B1_T_CHAR)(wchar_t)m_sInput[m_nNextChar++];
    
    return true;
}

wxString B1Interpreter::fromb1str(const B1_T_CHAR *s)
{
    wxString res;
    {
        wxStringBuffer sb(res, *s + 1);
        wxStringCharType *ptr = (wxStringCharType *)sb;

        auto len = *s;
        for(auto i = 0; i < len; i++)
        {
            *ptr++ = (wxStringCharType)*(s + i + 1);
        }

        *ptr = 0;
    }

    return res;
}

B1_T_ERROR B1Interpreter::Eval(const wxString &expr, wxString &type, wxString &val)
{
    // copy expression (B1_MAX_PROGLINE_LEN)
    B1_T_CHAR expr_buf[B1_MAX_PROGLINE_LEN];
    auto i = 0;
    for(auto c: expr)
    {
        *(expr_buf + i++) = (B1_T_CHAR)(wchar_t)c;
        if(i == B1_MAX_PROGLINE_LEN)
        {
            return B1_RES_ELINLONG;
        }
    }
    *(expr_buf + i) = 0;

    auto prev_prgline = b1_int_progline;
    b1_int_progline = expr_buf;

    // turn off RPN caching
    auto prev_rpn_caching = b1_dbg_rpn_caching_enabled;
    b1_dbg_rpn_caching_enabled = 0;
    // build expression RPN
    auto err = b1_rpn_build(0, NULL, NULL);
    b1_dbg_rpn_caching_enabled = prev_rpn_caching;
    if(err != B1_RES_OK)
    {
        b1_int_progline = prev_prgline;
        return err;
    }

    // set OPTION EXPLICIT
    auto prev_expl = b1_int_opt_explicit_val;
    b1_int_opt_explicit_val = 1;
    // evaluate the expression
    err = b1_eval(0, NULL);
    b1_int_progline = prev_prgline;
    b1_int_opt_explicit_val = prev_expl;
    if(err != B1_RES_OK)
    {
        return err;
    }

    // get result type name
    for(auto t = 0; t < B1_VAR_TYPE_COUNT; t++)
    {
        if(b1_var_types[t] == B1_TYPE_GET(b1_rpn_eval[0].type))
        {
            type = fromb1str(b1_var_type_names[t]);
            break;
        }
    }

    // convert result value to string
    err = b1_var_convert(b1_rpn_eval, B1_TYPE_STRING);
    if(err != B1_RES_OK)
    {
        return err;
    }

    // copy the value and free the string
    err = b1_var_var2str(b1_rpn_eval, b1_tmp_buf);
    if(err != B1_RES_OK)
    {
        return err;
    }

    val = fromb1str(b1_tmp_buf);

    return B1_RES_OK;
}
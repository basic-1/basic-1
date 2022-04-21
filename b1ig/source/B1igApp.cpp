#include "B1igApp.h"
#include "B1igFrame.h"

#include <clocale>


wxIMPLEMENT_APP(B1igApp);


B1igApp::B1igApp()
{
    setlocale(LC_ALL, "");
    // set numeric properties from C locale for sprintf to use dot as decimal delimiter
    setlocale(LC_NUMERIC, "C");
}

B1igApp::~B1igApp()
{
}

bool B1igApp::OnInit()
{
    B1igFrame *mainframe = new B1igFrame("");
    mainframe->Show(true);
    SetTopWindow(mainframe);
    return true;
}
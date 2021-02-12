#pragma once

#include <wx/wx.h>


class B1igApp : public wxApp
{
public:
    B1igApp();
    ~B1igApp();

    virtual bool OnInit() override;
};


wxDECLARE_APP(B1igApp);

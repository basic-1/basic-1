#pragma once

#include <wx/wx.h>

extern "C"
{
#include "b1err.h"
}


#define B1_RES_ECVTUCS2 (B1_RES_LASTERRCODE + 1)
#define B1_RES_EMSQUFAT (B1_RES_LASTERRCODE + 2)
#define B1_RES_EININTR (B1_RES_LASTERRCODE + 3)

#define B1_RES_LASTERRCODE_ALL B1_RES_EININTR


extern wxString _err_msgs[B1_RES_LASTERRCODE_ALL - B1_RES_FIRSTERRCODE + 1];
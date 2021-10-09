#pragma once

#include <windows.h>
#include <tchar.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "ticketmachine.h"

#define PERCENTAGE_BANNER       30
#define PERCENTAGE_CLIENT       60

#define HEIGHT_BANNER           600
#define HEIGHT_CLIENT           1220
#define HEIGHT_CLIENT_HEADER    90
#define HEIGHT_CLIENT_FOOTER    90
#define HEIGHT_KEYBOARD         280

#define DEF_FONT_FACE           TEXT("Arial")
#define DEF_FONT_HEIGHT         10

#define RGB_WHITE               RGB(255, 255, 255)
#define RGB_BLACK               RGB(0, 0, 0)
#define RGB_RED                 RGB(255, 0, 0)
#define RGB_BLUE                RGB(0, 0, 255)
#define RGB_PRIMARY             RGB(26, 54, 117)
#define RGB_TERTIARY            RGB(2, 33, 102)

#define BRUSH_WHITE             ::CreateSolidBrush(RGB_WHITE)
#define BRUSH_BLACK             ::CreateSolidBrush(RGB_BLACK)
#define BRUSH_RED               ::CreateSolidBrush(RGB_RED)
#define BRUSH_BLUE              ::CreateSolidBrush(RGB_BLUE)
#define BRUSH_PRIMARY           ::CreateSolidBrush(RGB_PRIMARY)
#define BRUSH_TERTIARY          ::CreateSolidBrush(RGB_TERTIARY)
#define BRUSH_TRANSPARENT       (HBRUSH)::GetStockObject(HOLLOW_BRUSH))

#define BRUSH_CLIENT_BK         ::CreateSolidBrush(RGB(233, 233, 233))
#define BRUSH_KIOSKLOCATION     ::CreateSolidBrush(RGB(169, 81 , 5))
#define BRUSH_KIOSKLOCATION_BTN ::CreateSolidBrush(RGB(80, 50 , 5))
#define BRUSH_KEYBOARD          ::CreateSolidBrush(RGB(26, 26, 26))
#define BRUSH_KEY               ::CreateSolidBrush(RGB(31, 31, 31))
#define BRUSH_DISABLED_KEY      ::CreateSolidBrush(RGB(101, 101, 101))
#define BRUSH_LOADING           ::CreateSolidBrush(RGB(241, 250, 254))

#define MONGOLIAN_KEYS          51
#define ENGLISH_KEYS            MONGOLIAN_KEYS - 9

#define METAKIOSK_SERVER        TEXT("www.metakiosk.mn")
#define OBJ_REQUIRE_ID          TEXT("service/require/id")
#define OBJ_REPORT_ME           TEXT("service/report/me")

#define TIMER_HEADER_SESSION    (WM_USER + 5000)
#define TIMER_SPLASH            (WM_USER + 5001)
#define TIMER_REPORTER          (WM_USER + 5002)

#define INTERVAL_1SECOND        1000
#define INTERVAL_TRANSDEP       15*60
#define INTERVAL_KIOSKLOCATION  3*60

#define TRANSDEP_MAX_RESERV     1

#define PATH_SPLASH             cfg.DataPath(TEXT("splash.png")).c_str()
#define PATH_CLIENT_BK          cfg.DataPath(TEXT("background.jpg")).c_str()
#define PATH_GETTING_DATA_GIF   cfg.DataPath(TEXT("loading-server-mn.gif")).c_str()
#define PATH_LOADING_HOURGLASS  cfg.DataPath(TEXT("loading-hourglass.gif")).c_str()

#define SWF_REPLAYER            cfg.CurrentDirectory() + cfg.DataPath(TEXT("replayer.swf"), FALSE)
#define SWF_TUTORIAL            cfg.CurrentDirectory() + cfg.DataPath(TEXT("tutorial.swf"), FALSE)
#define SWF_MAPPER              cfg.CurrentDirectory() + cfg.DataPath(TEXT("mapper.swf"), FALSE)
#define SWF_MARQUEE             cfg.CurrentDirectory() + cfg.DataPath(TEXT("marquee.swf"), FALSE)

#define FLASH_EXTERNAL_STOP     TEXT("externalStop")
#define FLASH_EXTERNAL_PAUSE    TEXT("externalPause")
#define FLASH_EXTERNAL_RESUME   TEXT("externalResume")
#define FLASH_EXTERNAL_PLAY     TEXT("externalPlay")
#define FLASH_EXTERNAL_RESTART  TEXT("externalRestart")
#define FLASH_EXTERNAL_SETTEXT  TEXT("externalSetText")
#define FLASH_EXTERNAL_TRUE     TEXT("<true/>")
#define FLASH_EXTERNAL_FALSE    TEXT("<false/>")

#define BITMAP_LOGO             Bmp::LoadGDI(cfg.DataPath(TEXT("gerege.png")).c_str())
#define BITMAP_FLAG             Bmp::LoadGDI(cfg.DataPath(TEXT("flag-") + cfg.LanguageStr() + TEXT(".png")).c_str())
#define BITMAP_MENUITEM(x)      Bmp::LoadGDI((cfg.DataPath(TEXT("menu\\")) + x + TEXT("-") + cfg.LanguageStr() + TEXT(".png")).c_str())
#define BITMAP_TIMER            Bmp::ReplaceColor(Bmp::GetMonochrome(Bmp::LoadGDI(cfg.DataPath(TEXT("header\\timer.png")).c_str())), NULL, RGB_WHITE - 5)
#define BITMAP_BACK_ARROW       Bmp::ReplaceColor(Bmp::GetMonochrome(Bmp::LoadGDI(cfg.DataPath(TEXT("footer\\back-arrow.png")).c_str())), NULL, RGB_WHITE - 5)
#define BITMAP_NEXT_ARROW       Bmp::ReplaceColor(Bmp::GetMonochrome(Bmp::LoadGDI(cfg.DataPath(TEXT("footer\\next-arrow.png")).c_str())), NULL, RGB_WHITE - 5)
#define BITMAP_TRANSDEP         Bmp::ReplaceColor(Bmp::GetMonochrome(Bmp::LoadGDI(cfg.DataPath(TEXT("header\\transdep.png")).c_str())), NULL, RGB_WHITE - 5)
#define BITMAP_MAPPER           Bmp::ReplaceColor(Bmp::GetMonochrome(Bmp::LoadGDI(cfg.DataPath(TEXT("header\\mapper.png")).c_str())), NULL, RGB_WHITE - 5)
#define BITMAP_APPVIDEO         Bmp::LoadGDI(cfg.DataPath(TEXT("menu\\appvideo-disable.png")).c_str() );
#define BITMAP_KIOSKLOCATION    Bmp::LoadGDI(cfg.DataPath(TEXT("menu\\kiosklocation.png")).c_str());
#define BITMAP_APPQRCODE        Bmp::LoadGDI(cfg.DataPath(TEXT("menu\\appqr-disable.png")).c_str())

#define PEN_FOOTER_BUTTON       ::CreatePen(PS_SOLID, 7, RGB_WHITE)

#define RECT_SPLASH             coord::to_rect(0, 0, 600, 400)
#define RECT_BARMENU            coord::to_rect(cfg.Client().left, cfg.Client().top + coord::height(cfg.Client()) - 275, cfg.Client().left + coord::width(cfg.Client()), 230)
#define RECT_APPVIDEO           coord::to_rect(rect.left + 474, rect.top, 330, 230)
#define RECT_KIOSKLOCATION      coord::to_rect(rect.left + 725, rect.top + 11, 356, 215)


#define MENU_LOGO_TOP           50
#define MENU_ITEM_HEIGHT        150
#define MENU_ITEM_WIDTH         450
#define MENU_ITEM_BETWEEN       40
#define MENU_ITEMS_IN_ROW       2

#define MESSAGEBOX_GETLASTERROR ::MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), TEXT("GetLastError"), MB_OK);

#include "config.hpp"
#include "framework.h"
#include "Karateka.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "ErrH.h"
#include "FCheck.h"
#include "D2BMPLOADER.h"
#include "karengine.h"
#include "karfsm.h"
#include <vector>
#include <ctime>
#include <fstream>
#include <chrono>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "karengine.lib")
#pragma comment(lib, "karfsm.lib")

#define bWinClassName L"MyKarateka"
#define mNew 1001
#define mExit 1002
#define mSave 1003
#define mLoad 1004
#define mHoF 1005

#define tmp_file ".\\res\\data\\temp.dat"
#define Ltmp_file L".\\res\\data\\temp.dat"
#define snd_file L".\\res\\snd\\main.wav"
#define hlp_file L".\\res\\data\\help.dat"
#define save_file L".\\res\\data\\save.dat"
#define record_file L".\\res\\data\\record.dat"

#define record 2001
#define no_record 2002
#define first_record 2003

WNDCLASS bWinClass = { 0 };
HINSTANCE bIns = nullptr;
HWND bHwnd = nullptr;
MSG bMsg = { 0 };
BOOL bRet = 0;
PAINTSTRUCT bPaint = { 0 };
HCURSOR mainCursor = nullptr;
HCURSOR outCursor = nullptr;
HICON mainIcon = nullptr;
HDC PaintDC = nullptr;
POINT cur_pos = { 0,0 };
HMENU bBar = nullptr;
HMENU bMain = nullptr;
HMENU bStore = nullptr;
UINT bTimer = 0;

D2D1_RECT_F b1Rect = { 0, 0, 250.0f, 50.0f };
D2D1_RECT_F b2Rect = { 275.0f, 0, 525.0f, 50.0f };
D2D1_RECT_F b3Rect = { 550.0f, 0, 800.0f, 50.0f };

D2D1_RECT_F Visb1Rect = { 20.0f, 0, 250.0f, 50.0f };
D2D1_RECT_F Visb2Rect = { 295.0f, 0, 525.0f, 50.0f };
D2D1_RECT_F Visb3Rect = { 570.0f, 0, 800.0f, 50.0f };

bool pause = false;
bool sound = true;
bool in_client = true;
bool show_help = false;
bool b1Hglt = false;
bool b2Hglt = false;
bool b3Hglt = false;
bool name_set = false;
bool win_game = false;

wchar_t current_player[16] = L"ONE NINJA";

float clH = 0;
float clW = 0;
int level = 1;
int score = 0;
int fall_cooldown = 100;
int max_hero_lifes = 0;
int max_evil_lifes = 0;
int seconds = 0;

///////////////////////////////////////////////

ID2D1Factory* iFactory = nullptr;
ID2D1HwndRenderTarget* Draw = nullptr;

IDWriteFactory* iWriteFactory = nullptr;
IDWriteTextFormat* nrmText = nullptr;
IDWriteTextFormat* bigText = nullptr;

ID2D1RadialGradientBrush* butBckg = nullptr;
ID2D1SolidColorBrush* TxtBrush = nullptr;
ID2D1SolidColorBrush* TxtHgltBrush = nullptr;
ID2D1SolidColorBrush* TxtInactBrush = nullptr;

ID2D1SolidColorBrush* LifeBrush = nullptr;
ID2D1SolidColorBrush* HurtBrush = nullptr;
ID2D1SolidColorBrush* CritBrush = nullptr;

ID2D1Bitmap* bmpField1 = nullptr;
ID2D1Bitmap* bmpField2 = nullptr;
ID2D1Bitmap* bmpField3 = nullptr;
ID2D1Bitmap* bmpBossField = nullptr;
ID2D1Bitmap* bmpHit = nullptr;
ID2D1Bitmap* bmpCloud1 = nullptr;
ID2D1Bitmap* bmpCloud2 = nullptr;

ID2D1Bitmap* bmpHeroFallL = nullptr;
ID2D1Bitmap* bmpHeroFallR = nullptr;

ID2D1Bitmap* bmpHeroWalkL = nullptr;
ID2D1Bitmap* bmpHeroWalkR = nullptr;

ID2D1Bitmap* bmpHeroKick1L = nullptr;
ID2D1Bitmap* bmpHeroKick2L = nullptr;
ID2D1Bitmap* bmpHeroKick1R = nullptr;
ID2D1Bitmap* bmpHeroKick2R = nullptr;

ID2D1Bitmap* bmpHeroPunch1L = nullptr;
ID2D1Bitmap* bmpHeroPunch2L = nullptr;
ID2D1Bitmap* bmpHeroPunch1R = nullptr;
ID2D1Bitmap* bmpHeroPunch2R = nullptr;

//////////////////////////////////////////

ID2D1Bitmap* bmpNin1FallL = nullptr;
ID2D1Bitmap* bmpNin1FallR = nullptr;

ID2D1Bitmap* bmpNin1Walk1L = nullptr;
ID2D1Bitmap* bmpNin1Walk1R = nullptr;
ID2D1Bitmap* bmpNin1Walk2L = nullptr;
ID2D1Bitmap* bmpNin1Walk2R = nullptr;
ID2D1Bitmap* bmpNin1Walk3L = nullptr;
ID2D1Bitmap* bmpNin1Walk3R = nullptr;
ID2D1Bitmap* bmpNin1Walk4L = nullptr;
ID2D1Bitmap* bmpNin1Walk4R = nullptr;

ID2D1Bitmap* bmpNin1Kick1L = nullptr;
ID2D1Bitmap* bmpNin1Kick2L = nullptr;
ID2D1Bitmap* bmpNin1Kick1R = nullptr;
ID2D1Bitmap* bmpNin1Kick2R = nullptr;

ID2D1Bitmap* bmpNin1Punch1L = nullptr;
ID2D1Bitmap* bmpNin1Punch2L = nullptr;
ID2D1Bitmap* bmpNin1Punch1R = nullptr;
ID2D1Bitmap* bmpNin1Punch2R = nullptr;

///////////////////////////////////////////

ID2D1Bitmap* bmpNin2FallL = nullptr;
ID2D1Bitmap* bmpNin2FallR = nullptr;

ID2D1Bitmap* bmpNin2WalkL = nullptr;
ID2D1Bitmap* bmpNin2WalkR = nullptr;

ID2D1Bitmap* bmpNin2Kick1L = nullptr;
ID2D1Bitmap* bmpNin2Kick2L = nullptr;
ID2D1Bitmap* bmpNin2Kick1R = nullptr;
ID2D1Bitmap* bmpNin2Kick2R = nullptr;

ID2D1Bitmap* bmpNin2PunchL = nullptr;
ID2D1Bitmap* bmpNin2PunchR = nullptr;

////////////////////////////////////////////

ID2D1Bitmap* bmpNin3FallL = nullptr;
ID2D1Bitmap* bmpNin3FallR = nullptr;

ID2D1Bitmap* bmpNin3WalkL = nullptr;
ID2D1Bitmap* bmpNin3WalkR = nullptr;

ID2D1Bitmap* bmpNin3Kick1L = nullptr;
ID2D1Bitmap* bmpNin3Kick2L = nullptr;
ID2D1Bitmap* bmpNin3Kick1R = nullptr;
ID2D1Bitmap* bmpNin3Kick2R = nullptr;

ID2D1Bitmap* bmpNin3Punch1L = nullptr;
ID2D1Bitmap* bmpNin3Punch2L = nullptr;
ID2D1Bitmap* bmpNin3Punch1R = nullptr;
ID2D1Bitmap* bmpNin3Punch2R = nullptr;

////////////////////////////////////////////

ID2D1Bitmap* bmpBossFallL = nullptr;
ID2D1Bitmap* bmpBossFallR = nullptr;

ID2D1Bitmap* bmpBossWalk1L = nullptr;
ID2D1Bitmap* bmpBossWalk1R = nullptr;
ID2D1Bitmap* bmpBossWalk2L = nullptr;
ID2D1Bitmap* bmpBossWalk2R = nullptr;
ID2D1Bitmap* bmpBossWalk3L = nullptr;
ID2D1Bitmap* bmpBossWalk3R = nullptr;
ID2D1Bitmap* bmpBossWalk4L = nullptr;
ID2D1Bitmap* bmpBossWalk4R = nullptr;

ID2D1Bitmap* bmpBossKick1L = nullptr;
ID2D1Bitmap* bmpBossKick2L = nullptr;
ID2D1Bitmap* bmpBossKick1R = nullptr;
ID2D1Bitmap* bmpBossKick2R = nullptr;
ID2D1Bitmap* bmpBossKick3L = nullptr;
ID2D1Bitmap* bmpBossKick3R = nullptr;

ID2D1Bitmap* bmpBossPunchL = nullptr;
ID2D1Bitmap* bmpBossPunchR = nullptr;

/////////////////////////////////////////

perPtr Hero = nullptr;
perPtr Evil = nullptr;

objPtr Cloud1 = nullptr;
objPtr Cloud2 = nullptr;

ACTIONCLASS* EvilFSM = nullptr;

std::vector<HITPLOT> vHits;

//////////////////////////////////////////
template <typename COM> BOOL LoadOff(COM** which)
{
    if ((*which))
    {
        (*which)->Release();
        return DL_OK;
    }
    return DL_FAIL;
}
void LogError(LPCWSTR err_text)
{
    std::wofstream err_file(L".\\res\\data\\errors.log", std::ios::app);
    err_file << err_text << L" ! Time stamp: " << std::chrono::system_clock::now() << std::endl;
    err_file.close();
}
void ReleaseCOMs()
{
    if (LoadOff(&iFactory) == DL_FAIL)LogError(L"Error releasing iFactory");
    if (LoadOff(&Draw) == DL_FAIL)LogError(L"Error releasing Draw");
    if (LoadOff(&iWriteFactory) == DL_FAIL)LogError(L"Error releasing iWriteFactory");
    if (LoadOff(&nrmText) == DL_FAIL)LogError(L"Error releasing nrmText");
    if (LoadOff(&bigText) == DL_FAIL)LogError(L"Error releasing bigText");
    if (LoadOff(&butBckg) == DL_FAIL)LogError(L"Error releasing butBckg");
    if (LoadOff(&TxtBrush) == DL_FAIL)LogError(L"Error releasing TxtBrush");
    if (LoadOff(&TxtHgltBrush) == DL_FAIL)LogError(L"Error releasing TxtHgltBrush");
    if (LoadOff(&TxtInactBrush) == DL_FAIL)LogError(L"Error releasing TxtInactBrush");

    if (LoadOff(&LifeBrush) == DL_FAIL)LogError(L"Error releasing LifeBrush");
    if (LoadOff(&HurtBrush) == DL_FAIL)LogError(L"Error releasing HurtBrush");
    if (LoadOff(&CritBrush) == DL_FAIL)LogError(L"Error releasing CritBrush");

    if (LoadOff(&bmpField1) == DL_FAIL)LogError(L"Error releasing bmpField1");
    if (LoadOff(&bmpField2) == DL_FAIL)LogError(L"Error releasing bmpField2");
    if (LoadOff(&bmpField3) == DL_FAIL)LogError(L"Error releasing bmpField3");
    if (LoadOff(&bmpBossField) == DL_FAIL)LogError(L"Error releasing bmpBossField");
    if (LoadOff(&bmpCloud1) == DL_FAIL)LogError(L"Error releasing bmpCloud1");
    if (LoadOff(&bmpCloud2) == DL_FAIL)LogError(L"Error releasing bmpCloud2");
    if (LoadOff(&bmpHit) == DL_FAIL)LogError(L"Error releasing bmpHit");
    //////////////////////////////////////////////////

    if (LoadOff(&bmpHeroFallL) == DL_FAIL)LogError(L"Error releasing bmpHeroFallL");
    if (LoadOff(&bmpHeroFallR) == DL_FAIL)LogError(L"Error releasing bmpHeroFallR");
    if (LoadOff(&bmpHeroWalkL) == DL_FAIL)LogError(L"Error releasing bmpHeroWalkL");
    if (LoadOff(&bmpHeroWalkR) == DL_FAIL)LogError(L"Error releasing bmpHeroWalkR");
    if (LoadOff(&bmpHeroKick1L) == DL_FAIL)LogError(L"Error releasing bmpHeroKick1L");
    if (LoadOff(&bmpHeroKick1R) == DL_FAIL)LogError(L"Error releasing bmpHeroKick1R");
    if (LoadOff(&bmpHeroKick2L) == DL_FAIL)LogError(L"Error releasing bmpHeroKick2L");
    if (LoadOff(&bmpHeroKick2R) == DL_FAIL)LogError(L"Error releasing bmpHeroKick2R");
    if (LoadOff(&bmpHeroPunch1L) == DL_FAIL)LogError(L"Error releasing bmpHeroPunch1L");
    if (LoadOff(&bmpHeroPunch1R) == DL_FAIL)LogError(L"Error releasing bmpHeroPunch1R");
    if (LoadOff(&bmpHeroPunch2L) == DL_FAIL)LogError(L"Error releasing bmpHeroPunch2L");
    if (LoadOff(&bmpHeroPunch2R) == DL_FAIL)LogError(L"Error releasing bmpHeroPunch2R");

    ////////////////////////////////////////////////////

    if (LoadOff(&bmpNin1FallL) == DL_FAIL)LogError(L"Error releasing bmpNin1FallL");
    if (LoadOff(&bmpNin1FallR) == DL_FAIL)LogError(L"Error releasing bmpNin1FallR");
    
    if (LoadOff(&bmpNin1Walk1L) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk1L");
    if (LoadOff(&bmpNin1Walk1R) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk1R");
    if (LoadOff(&bmpNin1Walk2L) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk2L");
    if (LoadOff(&bmpNin1Walk2R) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk2R");
    if (LoadOff(&bmpNin1Walk3L) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk3L");
    if (LoadOff(&bmpNin1Walk3R) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk3R");
    if (LoadOff(&bmpNin1Walk4L) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk4L");
    if (LoadOff(&bmpNin1Walk4R) == DL_FAIL)LogError(L"Error releasing bmpNin1Walk4R");
    
    if (LoadOff(&bmpNin1Kick1L) == DL_FAIL)LogError(L"Error releasing bmpNin1Kick1L");
    if (LoadOff(&bmpNin1Kick1R) == DL_FAIL)LogError(L"Error releasing bmpNin1Kick1R");
    if (LoadOff(&bmpNin1Kick2L) == DL_FAIL)LogError(L"Error releasing bmpNin1Kick2L");
    if (LoadOff(&bmpNin1Kick2R) == DL_FAIL)LogError(L"Error releasing bmpNin1Kick2R");
   
    if (LoadOff(&bmpNin1Punch1L) == DL_FAIL)LogError(L"Error releasing bmpNin1Punch1L");
    if (LoadOff(&bmpNin1Punch1R) == DL_FAIL)LogError(L"Error releasing bmpNin1Punch1R");
    if (LoadOff(&bmpNin1Punch2L) == DL_FAIL)LogError(L"Error releasing bmpHNin1Punch2L");
    if (LoadOff(&bmpNin1Punch2R) == DL_FAIL)LogError(L"Error releasing bmpNin1Punch2R");

    ///////////////////////////////////////////////////////

    if (LoadOff(&bmpNin2FallL) == DL_FAIL)LogError(L"Error releasing bmpNin2FallL");
    if (LoadOff(&bmpNin2FallR) == DL_FAIL)LogError(L"Error releasing bmpNin2FallR");

    if (LoadOff(&bmpNin2WalkL) == DL_FAIL)LogError(L"Error releasing bmpNin2Walk1L");
    if (LoadOff(&bmpNin2WalkR) == DL_FAIL)LogError(L"Error releasing bmpNin2Walk1R");
    
    if (LoadOff(&bmpNin2Kick1L) == DL_FAIL)LogError(L"Error releasing bmpNin2Kick1L");
    if (LoadOff(&bmpNin2Kick1R) == DL_FAIL)LogError(L"Error releasing bmpNin2Kick1R");
    if (LoadOff(&bmpNin2Kick2L) == DL_FAIL)LogError(L"Error releasing bmpNin2Kick2L");
    if (LoadOff(&bmpNin2Kick2R) == DL_FAIL)LogError(L"Error releasing bmpNin2Kick2R");

    if (LoadOff(&bmpNin2PunchL) == DL_FAIL)LogError(L"Error releasing bmpNin2Punch1L");
    if (LoadOff(&bmpNin2PunchR) == DL_FAIL)LogError(L"Error releasing bmpNin2Punch1R");
    
    ///////////////////////////////////////////////////////////

    if (LoadOff(&bmpNin3FallL) == DL_FAIL)LogError(L"Error releasing bmpNin3FallL");
    if (LoadOff(&bmpNin3FallR) == DL_FAIL)LogError(L"Error releasing bmpNin3FallR");

    if (LoadOff(&bmpNin3WalkL) == DL_FAIL)LogError(L"Error releasing bmpNin3Walk1L");
    if (LoadOff(&bmpNin3WalkR) == DL_FAIL)LogError(L"Error releasing bmpNin3Walk1R");
    
    if (LoadOff(&bmpNin3Kick1L) == DL_FAIL)LogError(L"Error releasing bmpNin3Kick1L");
    if (LoadOff(&bmpNin3Kick1R) == DL_FAIL)LogError(L"Error releasing bmpNin3Kick1R");
    if (LoadOff(&bmpNin3Kick2L) == DL_FAIL)LogError(L"Error releasing bmpNin3Kick2L");
    if (LoadOff(&bmpNin3Kick2R) == DL_FAIL)LogError(L"Error releasing bmpNin3Kick2R");

    if (LoadOff(&bmpNin3Punch1L) == DL_FAIL)LogError(L"Error releasing bmpNin3Punch1L");
    if (LoadOff(&bmpNin3Punch1R) == DL_FAIL)LogError(L"Error releasing bmpNin3Punch1R");
    if (LoadOff(&bmpNin3Punch2L) == DL_FAIL)LogError(L"Error releasing bmpHNin31Punch2L");
    if (LoadOff(&bmpNin3Punch2R) == DL_FAIL)LogError(L"Error releasing bmpNin3Punch2R");

    ///////////////////////////////////////////////

    if (LoadOff(&bmpBossFallL) == DL_FAIL)LogError(L"Error releasing bmpBossFallL");
    if (LoadOff(&bmpBossFallR) == DL_FAIL)LogError(L"Error releasing bmpBossFallR");

    if (LoadOff(&bmpBossWalk1L) == DL_FAIL)LogError(L"Error releasing bmpBossWalk1L");
    if (LoadOff(&bmpBossWalk1R) == DL_FAIL)LogError(L"Error releasing bmpBossWalk1R");
    if (LoadOff(&bmpBossWalk2L) == DL_FAIL)LogError(L"Error releasing bmpBossWalk2L");
    if (LoadOff(&bmpBossWalk2R) == DL_FAIL)LogError(L"Error releasing bmpBossWalk2R");
    if (LoadOff(&bmpBossWalk3L) == DL_FAIL)LogError(L"Error releasing bmpBossWalk3L");
    if (LoadOff(&bmpBossWalk3R) == DL_FAIL)LogError(L"Error releasing bmpBossWalk3R");
    if (LoadOff(&bmpBossWalk4L) == DL_FAIL)LogError(L"Error releasing bmpBossWalk4L");
    if (LoadOff(&bmpBossWalk4R) == DL_FAIL)LogError(L"Error releasing bmpBossWalk4R");

    if (LoadOff(&bmpBossKick1L) == DL_FAIL)LogError(L"Error releasing bmpBossKick1L");
    if (LoadOff(&bmpBossKick1R) == DL_FAIL)LogError(L"Error releasing bmpBossKick1R");
    if (LoadOff(&bmpBossKick2L) == DL_FAIL)LogError(L"Error releasing bmpBossKick2L");
    if (LoadOff(&bmpBossKick2R) == DL_FAIL)LogError(L"Error releasing bmpBossKick2R");
    if (LoadOff(&bmpBossKick3L) == DL_FAIL)LogError(L"Error releasing bmpBossKick3L");
    if (LoadOff(&bmpBossKick3R) == DL_FAIL)LogError(L"Error releasing bmpBossKick3R");

    if (LoadOff(&bmpBossPunchL) == DL_FAIL)LogError(L"Error releasing bmpBossPunch1L");
    if (LoadOff(&bmpBossPunchR) == DL_FAIL)LogError(L"Error releasing bmpBossPunch1R");
    



}
void ErrExit(int which_err)
{
    MessageBeep(MB_ICONERROR);
    MessageBox(NULL, ErrHandle(which_err), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

    ReleaseCOMs();
    std::remove(tmp_file);
    exit(1);
}
void InitGame()
{
    score = 0;
    level = 1;
    fall_cooldown = 100;
    
    wcscpy_s(current_player, L"ONE NINJA");
    name_set = false;
    win_game = false;

    LoadOff(&Cloud1);
    LoadOff(&Cloud2);
    LoadOff(&Hero);
    LoadOff(&Evil);
    LoadOff(&EvilFSM);

    vHits.clear();

    Cloud1 = OBJECT::CreateObject(-100.0f, 103.0f, 100.0f, 53.0f);
    Cloud2 = OBJECT::CreateObject(clW, 102.0f, 80.0f, 42.0f);
    Cloud1->SetDir(dirs::right);
    Cloud2->SetDir(dirs::left);

    Hero = PersFactory(50.0f, clH - 100.0f, types::hero);
    Evil = PersFactory(clW - 100.0f, clH - 100.0f, types::evil1);
    Evil->SetDir(dirs::right);

    max_hero_lifes = Hero->lifes;
    max_evil_lifes = Evil->lifes;

    ACTIONCLASS::CreateFSM(Evil->lifes, Hero->lifes, Evil->x - Hero->ex, &EvilFSM);
}
BOOL CheckRecord()
{
    if (score < 1)return no_record;
    if (win_game)score += 500;
    int result = 0;
    CheckFile(record_file, &result);
    if (result == FILE_NOT_EXIST)
    {
        std::wofstream rec(record_file);
        rec << score << std::endl;
        for (int i = 0; i < 16; i++)rec << static_cast<int>(current_player[i]) << std::endl;
        rec.close();
        return first_record;
    }
    else
    {
        std::wifstream check(record_file);
        check >> result;
        check.close();
    }

    if (result < score)
    {
        std::wofstream rec(record_file);
        rec << score << std::endl;
        for (int i = 0; i < 16; i++)rec << static_cast<int>(current_player[i]) << std::endl;
        rec.close();
        return record;
    }
    return no_record;
}
void GameOver()
{
    PlaySound(NULL, NULL, NULL);
    if (win_game)
    {
        if (sound)mciSendString(L"play .\\res\\snd\\win.wav", NULL, NULL, NULL);
        for (int i = 0; i < 21; i++)
        {
            if (i % 2 == 0)
            {
                Draw->BeginDraw();
                Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkViolet));
                if (bigText && TxtBrush)
                    Draw->DrawTextW(L"ПРЕВЪРТЯ ИГРАТА !", 18, bigText, D2D1::RectF(100.0f, clH / 2 - 100.0f, clW, clH), TxtBrush);
                Draw->EndDraw();
                Sleep(15);
            }
            else
            {
                Draw->BeginDraw();
                Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkViolet));
                Draw->EndDraw();
                Sleep(15);
            }
        }
        Sleep(2000);
    }

    wchar_t fintxt[50] = L"О, О, О ! ЗАГУБИ !";
    D2D1_RECT_F txtbox = { 150.0f,clH / 2 - 100.0f,clW,clH };
    int textsize = 0;

    switch (CheckRecord())
    {
    case no_record:
        if (sound)PlaySound(L".\\res\\snd\\loose.wav", NULL, SND_ASYNC);
        break;

    case first_record:
        if (sound)PlaySound(L".\\res\\snd\\record.wav", NULL, SND_ASYNC);
        wcscpy_s(fintxt, L"ПЪРВИ РЕКОРД !");
        break;

    case record:
        if (sound)PlaySound(L".\\res\\snd\\record.wav", NULL, SND_ASYNC);
        wcscpy_s(fintxt, L"СВЕТОВЕН РЕКОРД !");
        break;
    }

    for (int i = 0; i < 50; i++)
    {
        if (fintxt[i] != '\0')textsize++;
        else break;
    }

    Draw->BeginDraw();
    Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkViolet));
    if (bigText && TxtBrush)
        Draw->DrawTextW(fintxt, textsize, bigText, txtbox, TxtBrush);
    Draw->EndDraw();
    Sleep(6500);

    bMsg.message = WM_QUIT;
    bMsg.wParam = 0;
}
void NextLevel()
{
    if (sound)mciSendString(L"play .\\res\\snd\\levelup.wav", NULL, NULL, NULL);
    level++;
    fall_cooldown = 100;
    vHits.clear();

    LoadOff(&Cloud1);
    LoadOff(&Cloud2);
    LoadOff(&Hero);
    LoadOff(&Evil);
    LoadOff(&EvilFSM);

    Cloud1 = OBJECT::CreateObject(-100.0f, 103.0f, 100.0f, 53.0f);
    Cloud2 = OBJECT::CreateObject(clW, 102.0f, 80.0f, 42.0f);
    Cloud1->SetDir(dirs::right);
    Cloud2->SetDir(dirs::left);

    Hero = PersFactory(50.0f, clH - 100.0f, types::hero);
    switch (level)
    {
    case 2:
        Evil = PersFactory(clW - 100.0f, clH - 100.0f, types::evil2);
        break;

    case 3:
        Evil = PersFactory(clW - 100.0f, clH - 100.0f, types::evil3);
        break;

    case 4:
        Evil = PersFactory(clW - 100.0f, clH - 100.0f, types::boss);
        break;
    }
    Evil->SetDir(dirs::right);

    ACTIONCLASS::CreateFSM(Evil->lifes, Hero->lifes, Evil->x - Hero->ex, &EvilFSM);

    
    bool ready = false;
    float tx = clW;
    float ty = clH / 2 - 50.0f;
    float tex = tx + 400.0f;
    float tey = clH;
    D2D1_RECT_F TxtR = { tx,ty,tex,tey };

    max_hero_lifes = Hero->lifes;
    max_evil_lifes = Evil->lifes;

    while (!ready)
    {
        Draw->BeginDraw();
        Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkMagenta));
        if (bigText && TxtBrush)
            Draw->DrawText(L"НИВОТО ПРЕМИНАТО !", 19, bigText, TxtR, TxtBrush);
        Draw->EndDraw();
        TxtR.left -= 3.0f;
        TxtR.right -= 3.0f;
        if (TxtR.left <= clW / 2 - 100.0f)
        {
            Sleep(1000);
            ready = true;
        }
    }
    if (sound)
    {
        PlaySound(NULL, NULL, NULL);
        mciSendString(L"play .\\res\\snd\\fight.wav", NULL, NULL, NULL);
        Sleep(1000);
        PlaySound(snd_file, NULL, SND_ASYNC | SND_LOOP);
    }
}
void HallOfFame()
{
    int result = 0;
    CheckFile(record_file, &result);
    if (result == FILE_NOT_EXIST)
    {
        if (sound)MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(bHwnd, L"Все още няма рекорд на играта !\n\nПостарай се повече !",
            L"Липсва файл !", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
        return;
    }
    
    wchar_t rectxt[150] = L"KARATEKA: ";
    wchar_t add[10] = L"\0";
    wchar_t saved_player[16] = L"\0";
    
    D2D1_RECT_F txtbox = { 20.0f,100.0f,clW,clH };
    int textsize = 0;
    
    std::wifstream check(record_file);
    check >> result;
    for (int i = 0; i < 16; i++)
    {
        int letter = 0;
        check >> letter;
        saved_player[i] = static_cast<wchar_t>(letter);
    }
    check.close();
    wsprintf(add, L"%d", result);
    wcscat_s(rectxt, saved_player);
    wcscat_s(rectxt, L"\n\nСВЕТОВЕН РЕКОРД: ");
    wcscat_s(rectxt, add);

    if (sound)mciSendString(L"play .\\res\\snd\\tada.wav", NULL, NULL, NULL);
    result = 0;
    for (int i = 0; i < 150; i++)
    {
        if (rectxt[i] != '\0')result++;
        else break;
    }

    for (int i = 0; i < 31; i++)
    {
        if (i % 2 == 0)
        {
            Draw->BeginDraw();
            Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkViolet));
            if (bigText && TxtBrush)
                Draw->DrawTextW(rectxt, result, bigText, txtbox, TxtBrush);
            Draw->EndDraw();
            Sleep(15);
        }
        else
        {
            Draw->BeginDraw();
            Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkViolet));
            Draw->EndDraw();
            Sleep(15);
        }
    }
    Sleep(3000);
}
void SaveGame()
{
    int result = 0;
    CheckFile(save_file, &result);
    if (result == FILE_EXIST)
    {
        if (sound)MessageBeep(MB_ICONEXCLAMATION);
        if (MessageBox(bHwnd, L"Съществува записана игра, която ще загубиш !\n\nДа я презапиша ли ? !",
            L"Презапис !", MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL) == IDNO) return;
    }

    std::wofstream save(save_file);

    save << score << std::endl;
    save << level << std::endl;
    for (int i = 0; i < 16; i++)save << static_cast<int>(current_player[i]) << std::endl;
    save << name_set << std::endl;

    if (!Hero)save << 0 << std::endl;
    else
    {
        save << Hero->x << std::endl;
        save << Hero->y << std::endl;
        save << Hero->lifes << std::endl;
    }

    if (!Evil)save << 0 << std::endl;
    else
    {
        save << Evil->x << std::endl;
        save << Evil->y << std::endl;
        save << Evil->lifes << std::endl;
    }

    if (sound)mciSendString(L"play .\\res\\snd\\save.wav", NULL, NULL, NULL);
    MessageBox(bHwnd, L"Играта е запазена !", L"Запис !", MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
}
void LoadGame()
{
    int result = 0;
    CheckFile(save_file, &result);
    if (result == FILE_NOT_EXIST)
    {
        if (sound)MessageBeep(MB_ICONEXCLAMATION);
        MessageBox(bHwnd, L"Все още няма записана игра !\n\nПостарай се повече !",
            L"Липсва файл !", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
        return;
    }
    else
    {
        if (sound)MessageBeep(MB_ICONEXCLAMATION);
        if (MessageBox(bHwnd, L"Настоящата игра ще бъде загубена !\n\nДа я презапиша ли ? !",
            L"Презапис !", MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL) == IDNO) return;
    }

    std::wifstream save(save_file);
    float tx = 0;
    float ty = 0;

    save >> score;
    save >> level;

    fall_cooldown = 100;
    vHits.clear();

    LoadOff(&Cloud1);
    LoadOff(&Cloud2);
    LoadOff(&Hero);
    LoadOff(&Evil);
    LoadOff(&EvilFSM);

    Cloud1 = OBJECT::CreateObject(-100.0f, 103.0f, 100.0f, 53.0f);
    Cloud2 = OBJECT::CreateObject(clW, 102.0f, 80.0f, 42.0f);
    Cloud1->SetDir(dirs::right);
    Cloud2->SetDir(dirs::left);

    for (int i = 0; i < 16; i++)
    {
        int letter = 0;
        save >> letter;
        current_player[i] = static_cast<wchar_t>(letter);
    }
    save >> name_set;

    save >> tx;
    if (tx == 0)GameOver();
    else
    {
        save >> ty;
        save >> result;
        Hero = PersFactory(tx, ty, types::hero);
        Hero->lifes = result;
    }

    save >> tx;
    if (tx == 0)NextLevel();
    else
    {
        save >> ty;
        save >> result;
        switch (level)
        {
        case 1:
            Evil = PersFactory(tx, ty, types::evil1);
            break;

        case 2:
            Evil = PersFactory(tx, ty, types::evil2);
            break;

        case 3:
            Evil = PersFactory(tx, ty, types::evil3);
            break;

        case 4:
            Evil = PersFactory(tx, ty, types::boss);
            break;

        }

        Evil->lifes = result;
    }
    save.close();
    Evil->SetDir(dirs::right);

    ACTIONCLASS::CreateFSM(Evil->lifes, Hero->lifes, Evil->x - Hero->ex, &EvilFSM);
    if (sound)mciSendString(L"play .\\res\\snd\\save.wav", NULL, NULL, NULL);
    MessageBox(bHwnd, L"Играта е заредена !", L"Зареждане !", MB_OK | MB_APPLMODAL | MB_ICONINFORMATION);
}
void ShowHelp()
{
    wchar_t hlp_txt[1000] = L"\0";
    D2D1_RECT_F hlp_box = { 100.0f,clH,clW,clH + 400.0f };

    int result = 0;
    CheckFile(hlp_file, &result);
    if (result == FILE_NOT_EXIST)
    {
        if (sound)MessageBeep(MB_ICONERROR);
        MessageBox(bHwnd, L"Не е налична помощ за играта !\n\nСвържете се с разработчика !",
            L"Липсва файл !", MB_OK | MB_ICONERROR | MB_APPLMODAL);
        return;
    }

    std::wifstream help(hlp_file);
    help >> result;
    for (int i = 0; i < result; i++)
    {
        int letter = 0;
        help >> letter;
        hlp_txt[i] = static_cast<wchar_t>(letter);
    }
    help.close();

    bool ready = false;

    while (!ready)
    {
        Draw->BeginDraw();
        Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));
        if (nrmText && TxtBrush)
            Draw->DrawTextW(hlp_txt, result, nrmText, hlp_box, TxtBrush);
        Draw->EndDraw();

        hlp_box.top -= 0.5f;
        hlp_box.bottom -= 0.5f;
        if (hlp_box.bottom <= 50.0f)ready = true;
    }
}

INT_PTR CALLBACK bDlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_INITDIALOG:
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIcon));
        return (INT_PTR)TRUE;
        break;

    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hwnd, IDCANCEL);
            break;

        case IDOK:
            if (GetDlgItemText(hwnd, IDC_NAME, current_player, 15) < 1)
            {
                wcscpy_s(current_player, L"ONE NINJA");
                if (sound)MessageBeep(MB_ICONASTERISK);
                MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONASTERISK);
                EndDialog(hwnd, IDCANCEL);
                break;
            }
            EndDialog(hwnd, IDOK);
        }
        break;
    }

    return(INT_PTR)(FALSE);
}
LRESULT CALLBACK bWinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
    switch (ReceivedMsg)
    {
    case WM_CREATE:
        {
            RECT clR = { 0 };
            GetClientRect(hwnd, &clR);
            clW = (float)(clR.right);
            clH = (float)(clR.bottom);

            srand((unsigned int)time(0));

            bBar = CreateMenu();
            bMain = CreateMenu();
            bStore = CreateMenu();

            AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
            AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

            AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
            AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bMain, MF_STRING, mExit, L"Изход");

            AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
            AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
            AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
            AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");

            SetMenu(hwnd, bBar);
            InitGame();
        }
        break;

    case WM_CLOSE:
        pause = true;
        if (sound)MessageBeep(MB_ICONEXCLAMATION);
        if (MessageBox(hwnd, L"Ако продължиш, губиш тази игра !\n\nНаистина ли излизаш ?",
            L"Изход", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
        {
            pause = false;
            break;
        }
        GameOver();
        break;

    case WM_PAINT:
        PaintDC = BeginPaint(hwnd, &bPaint);
        FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(50, 50, 50)));
        EndPaint(hwnd, &bPaint);
        break;

    case WM_SETCURSOR:
        GetCursorPos(&cur_pos);
        ScreenToClient(hwnd, &cur_pos);
        if (LOWORD(lParam) == HTCLIENT)
        {
            if (!in_client)
            {
                in_client = true;
                pause = false;
            }

            if (cur_pos.y <= 50)
            {
                if ((float)(cur_pos.x) >= b1Rect.left && (float)(cur_pos.x) <= b1Rect.right)
                {
                    if (!b1Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = true;
                    }
                    if (b2Hglt || b3Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b2Hglt = false;
                        b3Hglt = false;
                    }
                }
                if ((float)(cur_pos.x) >= b2Rect.left && (float)(cur_pos.x) <= b2Rect.right)
                {
                    if (!b2Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b2Hglt = true;
                    }
                    if (b1Hglt || b3Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b3Hglt = false;
                    }
                }
                if ((float)(cur_pos.x) >= b3Rect.left && (float)(cur_pos.x) <= b3Rect.right)
                {
                    if (!b3Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b3Hglt = true;
                    }
                    if (b1Hglt || b2Hglt)
                    {
                        if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                        b1Hglt = false;
                        b2Hglt = false;
                    }
                }
            
                SetCursor(outCursor);
                return true;
            }
            else
            {
                if (b1Hglt || b2Hglt || b3Hglt)
                {
                    if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                    b1Hglt = false;
                    b2Hglt = false;
                    b3Hglt = false;
                }
                SetCursor(mainCursor);
                return true;
            }
        }
        else
        {
            if (in_client)
            {
                in_client = false;
                pause = true;
            }

            if (b1Hglt || b2Hglt || b3Hglt)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
                b1Hglt = false;
                b2Hglt = false;
                b3Hglt = false;
            }

            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return true;
        }
        break;

    case WM_TIMER:
        if (pause)break;
        seconds++;
        if (seconds > 10)
        {
            seconds = 0;
            if (Hero->lifes + 10 <= max_hero_lifes)Hero->lifes += 10;
            if (Evil->lifes + 10 <= max_evil_lifes)Evil->lifes += 10;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case mNew:
            pause = true;
            if (sound)MessageBeep(MB_ICONEXCLAMATION);
            if (MessageBox(hwnd, L"Ако продължиш, губиш тази игра !\n\nНаистина ли рестартираш ?",
                L"Рестарт", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
            {
                pause = false;
                break;
            }
            InitGame();
            break;

        case mExit:
            SendMessage(hwnd, WM_CLOSE, NULL, NULL);
            break;

        case mSave:
            pause = true;
            SaveGame();
            pause = false;
            break;

        case mLoad:
            pause = true;
            LoadGame();
            pause = false;
            break;

        case mHoF:
            pause = true;
            HallOfFame();
            pause = false;
            break;
        }
        break;

    case WM_KEYDOWN:
        if (Hero)
        {
            switch (wParam)
            {
            case VK_LEFT:
                Hero->SetDir(dirs::right);
                Hero->Move((float)(level));
                break;

            case VK_RIGHT:
                Hero->SetDir(dirs::left);
                Hero->Move((float)(level));                 
                break;

            case VK_CONTROL:
                Hero->SetState(states::punch);
                break;

            case VK_SHIFT:
                Hero->SetState(states::kick);
                break;
            }
        }
        break;

    case WM_LBUTTONDOWN:
        if (cur_pos.y > 50)break;
        if (cur_pos.x > b1Rect.left && cur_pos.x < b1Rect.right)
        {
            if (name_set)
            {
                if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
                break;
            }
            else
            {
                if (sound)mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);
                if (DialogBox(bIns, MAKEINTRESOURCE(IDD_PLAYER), hwnd, &bDlgProc) == IDOK)name_set = true;
                break;
            }
        }
        if (cur_pos.x > b2Rect.left && cur_pos.x < b2Rect.right)
        {
            if (sound)
            {
                mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);
                sound = false;
                PlaySound(NULL, NULL, NULL);
                break;
            }
            else
            {
                mciSendString(L"play .\\res\\snd\\select.wav", NULL, NULL, NULL);
                sound = false;
                PlaySound(snd_file, NULL, SND_ASYNC | SND_LOOP);
                break;
            }
            
        }
        if (cur_pos.x > b3Rect.left && cur_pos.x < b3Rect.right)
        {
            if (!show_help)
            {
                show_help = true;
                ShowHelp();
            }
            show_help = false;
        }
        break;

    default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
    }

    return (LRESULT)(FALSE);
}

void Init2D()
{
    int fresult = 0;
    CheckFile(Ltmp_file, &fresult);
    if (fresult == FILE_EXIST)ErrExit(eStarted);
    else
    {
        std::wofstream temp(Ltmp_file);
        temp << L"Tuy neshto bachka be !";
        temp.close();
    }

    int ax = GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2);
    int ay = 100;

    if (GetSystemMetrics(SM_CXSCREEN) < ax + (int)(scr_width)
        || GetSystemMetrics(SM_CYSCREEN) < ay + (int)(scr_height))ErrExit(eScreen);

    mainIcon = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 64, 64, LR_LOADFROMFILE));
    if (!mainIcon)ErrExit(eIcon);

    mainCursor = LoadCursorFromFile(L".\\res\\bcursor.ani");
    outCursor = LoadCursorFromFile(L".\\res\\out.ani");

    if (!mainCursor || !outCursor)ErrExit(eCursor);

    bWinClass.lpszClassName = bWinClassName;
    bWinClass.hInstance = bIns;
    bWinClass.lpfnWndProc = &bWinProc;
    bWinClass.hbrBackground = CreateSolidBrush(RGB(50, 50, 50));
    bWinClass.hIcon = mainIcon;
    bWinClass.hCursor = mainCursor;
    bWinClass.style = CS_DROPSHADOW;

    if (!RegisterClass(&bWinClass))ErrExit(eClass);

    bHwnd = CreateWindowW(bWinClassName, L"КАРАТЕКА NEW", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, ax, ay,
        (int)(scr_width), (int)(scr_height), NULL, NULL, bIns, NULL);

    if (!bHwnd)ErrExit(eWindow);
    else ShowWindow(bHwnd, SW_SHOWDEFAULT);
    ////////////////////////////////////////////////////////////////////////////

    HRESULT hr = S_OK;
    D2D1_GRADIENT_STOP gstops[2] = { 0 };
    ID2D1GradientStopCollection* gstopcol = nullptr;

    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw factory"); 
        ErrExit(eD2D);
    }

    if (bHwnd)
        hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(bHwnd, D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw hwnd target");
        ErrExit(eD2D);
    }

    gstops[0].position = 0;
    gstops[0].color = D2D1::ColorF(D2D1::ColorF::DarkBlue);
    gstops[1].position = 1.0f;
    gstops[1].color = D2D1::ColorF(D2D1::ColorF::DarkCyan);
    
    hr = Draw->CreateGradientStopCollection(gstops, 2, &gstopcol);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw gradientstopcollection");
        ErrExit(eD2D);
    }

    if (gstopcol)
        hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(
            D2D1::Point2F(clW / 2, 25.0f), D2D1::Point2F(0, 0), clW / 2, 25.0f), gstopcol, &butBckg);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw butBckg");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Bisque), &TxtBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw TxtBrush");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::YellowGreen), &TxtHgltBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw TxtHgltBrush");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkGray), &TxtInactBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw TxtInactBrush");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &LifeBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw LifeBrush");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &HurtBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw HurtBrush");
        ErrExit(eD2D);
    }

    hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &CritBrush);
    if (hr != S_OK)
    {
        LogError(L"Error creating draw CritBrush");
        ErrExit(eD2D);
    }

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&iWriteFactory));
    if (hr != S_OK)
    {
        LogError(L"Error creating iWriteFactory");
        ErrExit(eD2D);
    }

    if (iWriteFactory)
    {
        hr = iWriteFactory->CreateTextFormat(L"GABRIOLA", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
            DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_EXPANDED, 25.0f, L"", &nrmText);
        if (hr != S_OK)
        {
            LogError(L"Error creating nrmText");
            ErrExit(eD2D);
        }

        hr = iWriteFactory->CreateTextFormat(L"GABRIOLA", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
            DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_EXPANDED, 72.0f, L"", &bigText);
        if (hr != S_OK)
        {
            LogError(L"Error creating bigText");
            ErrExit(eD2D);
        }
    
    }
    //////////////////////////////////////////

    bool bmp_ok = true;

    bmpField1= Load(L".\\res\\img\\field1.png", Draw);
    if (!bmpField1)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpField1");
    }
    bmpField2 = Load(L".\\res\\img\\field2.png", Draw);
    if (!bmpField2)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpField2");
    }
    bmpField3 = Load(L".\\res\\img\\field3.png", Draw);
    if (!bmpField3)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpField2");
    }
    bmpBossField = Load(L".\\res\\img\\bossfield.png", Draw);
    if (!bmpBossField)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossField2");
    }
    bmpCloud1 = Load(L".\\res\\img\\cloud1.png", Draw);
    if (!bmpCloud1)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpCloud1");
    }
    bmpCloud2 = Load(L".\\res\\img\\cloud2.png", Draw);
    if (!bmpCloud2)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpCloud2");
    }
    bmpHit = Load(L".\\res\\img\\hit.png", Draw);
    if (!bmpHit)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHit");
    }
    //////////////////////////////////////////////////////

    bmpHeroFallL = Load(L".\\res\\img\\hero\\left\\fall.png", Draw);
    if (!bmpHeroFallL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroFallL");
    }
    bmpHeroKick1L = Load(L".\\res\\img\\hero\\left\\kick1.png", Draw);
    if (!bmpHeroKick1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroKick1L");
    }
    bmpHeroKick2L = Load(L".\\res\\img\\hero\\left\\kick2.png", Draw);
    if (!bmpHeroKick2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroKick2L");
    }
    bmpHeroPunch1L = Load(L".\\res\\img\\hero\\left\\punch1.png", Draw);
    if (!bmpHeroPunch1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroPunch1L");
    }
    bmpHeroPunch2L = Load(L".\\res\\img\\hero\\left\\punch2.png", Draw);
    if (!bmpHeroPunch2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroPunch2L");
    }
    bmpHeroWalkL = Load(L".\\res\\img\\hero\\left\\walk.png", Draw);
    if (!bmpHeroWalkL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroWalkL");
    }

    bmpHeroFallR = Load(L".\\res\\img\\hero\\right\\fall.png", Draw);
    if (!bmpHeroFallR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroFallR");
    }
    bmpHeroKick1R = Load(L".\\res\\img\\hero\\right\\kick1.png", Draw);
    if (!bmpHeroKick1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroKick1R");
    }
    bmpHeroKick2R = Load(L".\\res\\img\\hero\\right\\kick2.png", Draw);
    if (!bmpHeroKick2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroKick2R");
    }
    bmpHeroPunch1R = Load(L".\\res\\img\\hero\\right\\punch1.png", Draw);
    if (!bmpHeroPunch1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroPunch1R");
    }
    bmpHeroPunch2R = Load(L".\\res\\img\\hero\\right\\punch2.png", Draw);
    if (!bmpHeroPunch2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroPunch2R");
    }
    bmpHeroWalkR = Load(L".\\res\\img\\hero\\right\\walk.png", Draw);
    if (!bmpHeroWalkR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpHeroWalkR");
    }
    ////////////////////////////////////////////////////////////////////

    bmpBossFallL = Load(L".\\res\\img\\evil\\boss\\left\\fall.png", Draw);
    if (!bmpBossFallL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossFallL");
    }
    bmpBossKick1L = Load(L".\\res\\img\\evil\\boss\\left\\kick1.png", Draw);
    if (!bmpBossKick1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick1L");
    }
    bmpBossKick2L = Load(L".\\res\\img\\evil\\boss\\left\\kick2.png", Draw);
    if (!bmpBossKick2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick2L");
    }
    bmpBossKick3L = Load(L".\\res\\img\\evil\\boss\\left\\kick3.png", Draw);
    if (!bmpBossKick3L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick3L");
    }
    bmpBossPunchL = Load(L".\\res\\img\\evil\\boss\\left\\punch.png", Draw);
    if (!bmpBossPunchL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossPunch1L");
    }
    bmpBossWalk1L = Load(L".\\res\\img\\evil\\boss\\left\\move1.png", Draw);
    if (!bmpBossWalk1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk1L");
    }
    bmpBossWalk2L = Load(L".\\res\\img\\evil\\boss\\left\\move2.png", Draw);
    if (!bmpBossWalk2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk2L");
    }
    bmpBossWalk3L = Load(L".\\res\\img\\evil\\boss\\left\\move3.png", Draw);
    if (!bmpBossWalk1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk3L");
    }
    bmpBossWalk4L = Load(L".\\res\\img\\evil\\boss\\left\\move4.png", Draw);
    if (!bmpBossWalk4L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk4L");
    }

    bmpBossFallR = Load(L".\\res\\img\\evil\\boss\\right\\fall.png", Draw);
    if (!bmpBossFallR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossFallR");
    }
    bmpBossKick1R = Load(L".\\res\\img\\evil\\boss\\right\\kick1.png", Draw);
    if (!bmpBossKick1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick1R");
    }
    bmpBossKick2R = Load(L".\\res\\img\\evil\\boss\\right\\kick2.png", Draw);
    if (!bmpBossKick2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick2R");
    }
    bmpBossKick3R = Load(L".\\res\\img\\evil\\boss\\right\\kick3.png", Draw);
    if (!bmpBossKick3R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossKick3R");
    }
    bmpBossPunchR = Load(L".\\res\\img\\evil\\boss\\right\\punch.png", Draw);
    if (!bmpBossPunchR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossPunchR");
    }
    bmpBossWalk1R = Load(L".\\res\\img\\evil\\boss\\right\\move1.png", Draw);
    if (!bmpBossWalk1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk1L");
    }
    bmpBossWalk2R = Load(L".\\res\\img\\evil\\boss\\right\\move2.png", Draw);
    if (!bmpBossWalk2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk2R");
    }
    bmpBossWalk3R = Load(L".\\res\\img\\evil\\boss\\right\\move3.png", Draw);
    if (!bmpBossWalk1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk3R");
    }
    bmpBossWalk4R = Load(L".\\res\\img\\evil\\boss\\right\\move4.png", Draw);
    if (!bmpBossWalk4R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpBossWalk4R");
    }
    
    ////////////////////////////////////////////////////////////////////

    bmpNin1FallL = Load(L".\\res\\img\\evil\\ninja1\\left\\fall.png", Draw);
    if (!bmpNin1FallL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1FallL");
    }
    bmpNin1Kick1L = Load(L".\\res\\img\\evil\\ninja1\\left\\kick1.png", Draw);
    if (!bmpNin1Kick1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Kick1L");
    }
    bmpNin1Kick2L = Load(L".\\res\\img\\evil\\ninja1\\left\\kick2.png", Draw);
    if (!bmpNin1Kick2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Kick2L");
    }
    bmpNin1Punch1L = Load(L".\\res\\img\\evil\\ninja1\\left\\punch1.png", Draw);
    if (!bmpNin1Punch1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Punch1L");
    }
    bmpNin1Punch2L = Load(L".\\res\\img\\evil\\ninja1\\left\\punch2.png", Draw);
    if (!bmpNin1Punch2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Punch2L");
    }
    bmpNin1Walk1L = Load(L".\\res\\img\\evil\\ninja1\\left\\move1.png", Draw);
    if (!bmpNin1Walk1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk1L");
    }
    bmpNin1Walk2L = Load(L".\\res\\img\\evil\\ninja1\\left\\move2.png", Draw);
    if (!bmpNin1Walk2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Walk1L");
    }
    bmpNin1Walk3L = Load(L".\\res\\img\\evil\\ninja1\\left\\move3.png", Draw);
    if (!bmpNin1Walk3L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk3L");
    }
    bmpNin1Walk4L = Load(L".\\res\\img\\evil\\ninja1\\left\\move4.png", Draw);
    if (!bmpNin1Walk4L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk4L");
    }

    bmpNin1FallR = Load(L".\\res\\img\\evil\\ninja1\\right\\fall.png", Draw);
    if (!bmpNin1FallR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1FallR");
    }
    bmpNin1Kick1R = Load(L".\\res\\img\\evil\\ninja1\\right\\kick1.png", Draw);
    if (!bmpNin1Kick1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Kick1R");
    }
    bmpNin1Kick2R = Load(L".\\res\\img\\evil\\ninja1\\right\\kick2.png", Draw);
    if (!bmpNin1Kick2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Kick2R");
    }
    bmpNin1Punch1R = Load(L".\\res\\img\\evil\\ninja1\\right\\punch1.png", Draw);
    if (!bmpNin1Punch1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Punch1R");
    }
    bmpNin1Punch2R = Load(L".\\res\\img\\evil\\ninja1\\right\\punch2.png", Draw);
    if (!bmpNin1Punch2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Punch2R");
    }
    bmpNin1Walk1R = Load(L".\\res\\img\\evil\\ninja1\\right\\move1.png", Draw);
    if (!bmpNin1Walk1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk1R");
    }
    bmpNin1Walk2R = Load(L".\\res\\img\\evil\\ninja1\\right\\move2.png", Draw);
    if (!bmpNin1Walk2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Walk1R");
    }
    bmpNin1Walk3R = Load(L".\\res\\img\\evil\\ninja1\\right\\move3.png", Draw);
    if (!bmpNin1Walk3R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk3R");
    }
    bmpNin1Walk4R = Load(L".\\res\\img\\evil\\ninja1\\right\\move4.png", Draw);
    if (!bmpNin1Walk4R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin1Walk4R");
    }
    //////////////////////////////////////////////////////////////////////////

    bmpNin2FallL = Load(L".\\res\\img\\evil\\ninja2\\left\\fall.png", Draw);
    if (!bmpNin2FallL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2FallL");
    }
    bmpNin2Kick1L = Load(L".\\res\\img\\evil\\ninja2\\left\\kick1.png", Draw);
    if (!bmpNin2Kick1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Kick1L");
    }
    bmpNin2Kick2L = Load(L".\\res\\img\\evil\\ninja2\\left\\kick2.png", Draw);
    if (!bmpNin2Kick2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Kick2L");
    }
    bmpNin2PunchL = Load(L".\\res\\img\\evil\\ninja2\\left\\punch.png", Draw);
    if (!bmpNin2PunchL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Punch2L");
    }
    bmpNin2WalkL = Load(L".\\res\\img\\evil\\ninja2\\left\\move1.png", Draw);
    if (!bmpNin2WalkL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Walk1L");
    }
    
    bmpNin2FallR = Load(L".\\res\\img\\evil\\ninja2\\right\\fall.png", Draw);
    if (!bmpNin2FallR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2FallR");
    }
    bmpNin2Kick1R = Load(L".\\res\\img\\evil\\ninja2\\right\\kick1.png", Draw);
    if (!bmpNin2Kick1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Kick1L");
    }
    bmpNin2Kick2R = Load(L".\\res\\img\\evil\\ninja2\\right\\kick2.png", Draw);
    if (!bmpNin2Kick2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Kick2L");
    }
    bmpNin2PunchR = Load(L".\\res\\img\\evil\\ninja2\\right\\punch.png", Draw);
    if (!bmpNin2PunchR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2Punch2R");
    }
    bmpNin2WalkR = Load(L".\\res\\img\\evil\\ninja2\\right\\move.png", Draw);
    if (!bmpNin2WalkR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin2WalkR");
    }
    //////////////////////////////////////////////////////////////////////////

    bmpNin3FallL = Load(L".\\res\\img\\evil\\ninja3\\left\\fall.png", Draw);
    if (!bmpNin3FallL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3FallL");
    }
    bmpNin3Kick1L = Load(L".\\res\\img\\evil\\ninja3\\left\\kick1.png", Draw);
    if (!bmpNin3Kick1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Kick1L");
    }
    bmpNin3Kick2L = Load(L".\\res\\img\\evil\\ninja3\\left\\kick2.png", Draw);
    if (!bmpNin3Kick2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Kick2L");
    }
    bmpNin3Punch1L = Load(L".\\res\\img\\evil\\ninja3\\left\\punch1.png", Draw);
    if (!bmpNin3Punch1L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Punch1L");
    }
    bmpNin3Punch2L = Load(L".\\res\\img\\evil\\ninja3\\left\\punch2.png", Draw);
    if (!bmpNin3Punch2L)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Punch2L");
    }
    bmpNin3WalkL = Load(L".\\res\\img\\evil\\ninja3\\left\\walk.png", Draw);
    if (!bmpNin3WalkL)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3WalkL");
    }

    bmpNin3FallR = Load(L".\\res\\img\\evil\\ninja3\\right\\fall.png", Draw);
    if (!bmpNin3FallR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3FallR");
    }
    bmpNin3Kick1R = Load(L".\\res\\img\\evil\\ninja3\\right\\kick1.png", Draw);
    if (!bmpNin3Kick1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Kick1R");
    }
    bmpNin3Kick2R = Load(L".\\res\\img\\evil\\ninja3\\right\\kick2.png", Draw);
    if (!bmpNin3Kick2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Kick2R");
    }
    bmpNin3Punch1R = Load(L".\\res\\img\\evil\\ninja3\\right\\punch1.png", Draw);
    if (!bmpNin3Punch1R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Punch1R");
    }
    bmpNin3Punch2R = Load(L".\\res\\img\\evil\\ninja3\\right\\punch2.png", Draw);
    if (!bmpNin3Punch2R)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3Punch2R");
    }
    bmpNin3WalkR = Load(L".\\res\\img\\evil\\ninja3\\right\\walk.png", Draw);
    if (!bmpNin3WalkR)
    {
        bmp_ok = false;
        LogError(L"Error loading bmpNin3WalkR");
    }
    ///////////////////////////////////////////////////////////

    if (!bmp_ok)ErrExit(eD2D);

    float x1 = clW;
    float y1 = clH / 2 - 50.0f;
    float ex1 = x1 + 600.0f;
    float ey1 = y1 + 200.0f;
    
    float x2 = 0;
    float y2 = clH - 200.0f;
    float ex2 = x2 + 600.0f;
    float ey2 = y2 + 200.0f;
    
    D2D1_RECT_F Line1R = { x1, y1, ex1, ey1 };
    D2D1_RECT_F Line2R = { x2, y2, ex2, ey2 };

    wchar_t firstline[13] = L"KARATEKA 1.0";
    wchar_t secondline[12] = L"dev. Daniel";

    bool ready1 = false;
    bool ready2 = false;
    if (sound)mciSendString(L"play .\\res\\snd\\intro.wav", NULL, NULL, NULL);
    
    while (!ready1 && !ready2)
    {
        Draw->BeginDraw();
        Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));
        if (x1 > clW / 2 - 150.0f)
        {
            x1 -= 3.1f;
            ex1 = x1 + 600.0f;
            Line1R.left = x1;
            Line1R.right = ex1;
            Line1R.top = y1;
            Line1R.bottom = ey1;
        }
        else ready1 = true;

        if (x2 < clW / 2 + 100.0f)
        {
            x2 += 3.1f;
            ex2 = x2 + 600.0f;
            Line2R.left = x2;
            Line2R.right = ex2;
            Line2R.top = y2;
            Line2R.bottom = ey2;
        }
        else ready2 = true;

        if (bigText && TxtBrush)
            Draw->DrawText(firstline, 13, bigText, Line1R, TxtBrush);
        if (bigText && TxtBrush)
            Draw->DrawText(secondline, 11, bigText, Line2R, TxtBrush);
        Draw->EndDraw();
    }
    Sleep(1500);
    if (sound)
    {
        PlaySound(NULL, NULL, NULL);
        mciSendString(L"play .\\res\\snd\\fight.wav", NULL, NULL, NULL);
        Sleep(1000);
        PlaySound(snd_file, NULL, SND_ASYNC | SND_LOOP);
    }
}

///////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    bIns = hInstance;
    if (!bIns)ErrExit(eClass);
    Init2D();
    PlaySound(snd_file, NULL, SND_ASYNC | SND_LOOP);


    while (bMsg.message != WM_QUIT)
    {
        if ((bRet = PeekMessage(&bMsg, bHwnd, NULL, NULL, PM_REMOVE)) != 0)
        {
            if (bRet == -1)ErrExit(eMsg);
            TranslateMessage(&bMsg);
            DispatchMessage(&bMsg);
        }
        if (pause)
        {
            if (show_help)continue;
            Draw->BeginDraw();
            Draw->Clear(D2D1::ColorF(D2D1::ColorF::DarkCyan));
            if (bigText && TxtBrush)
                Draw->DrawText(L"ПАУЗА", 6, bigText, D2D1::RectF(clW / 2 - 50.0f, clH / 2 - 50.0f, clW, clH), TxtBrush);
            Draw->EndDraw();
            continue;
        }
        ///////////////////////////////////////////////

        //GAME ENGINE ********************************

        //CLOUDS **********************************
        if (Cloud1)
        {
            if (Cloud1->x <= clW)
            {
                Cloud1->x += 1.2f;
                Cloud1->SetEdges();
            }
            else
            {
                LoadOff(&Cloud1);
                Cloud1 = OBJECT::CreateObject(-100.0f, 103.0f, 100.0f, 53.0f);
                Cloud1->SetDir(dirs::right);
            }
        }
        if (Cloud2)
        {
            if (Cloud2->ex >= 0)
            {
                Cloud2->x -= 0.8f;
                Cloud2->SetEdges();
            }
            else
            {
                LoadOff(&Cloud2);
                Cloud2 = OBJECT::CreateObject(clW, 102.0f, 80.0f, 42.0f);
                Cloud2->SetDir(dirs::left);
            }
        }
        ///////////////////////////////////////////

        //FSM *************************************

        if (Evil && EvilFSM && Hero && Hero->GetState() != states::fall && Evil->GetState() != states::fall)
        {
            EvilFSM->SetInfo(Evil->lifes, Hero->lifes, abs(Evil->x - Hero->x - 20.0f));
            
            if (Hero->x -20.0f  >= Evil->ex && Evil->GetDir() == dirs::right) Evil->SetDir(dirs::left);
            if (Hero->ex + 20.0f <= Evil->x && Evil->GetDir() == dirs::left)Evil->SetDir(dirs::right);
            
            switch (EvilFSM->WhatToDo())
            {
            case actions::walk:
                if (Evil->Move((float)(level)) == DL_FAIL)
                {
                    if (Evil->GetDir() == dirs::right)Evil->SetDir(dirs::left);
                    else Evil->SetDir(dirs::right);
                }
                break;

            case actions::punch:
                if (Evil->GetState() == states::fall)break;
                if (rand() % (80 - level * 10) == 1)Evil->SetState(states::punch);
                break;

            case actions::kick:
                if (Evil->GetState() == states::fall)break;
                if (rand() % (85 - level * 10) == 1)Evil->SetState(states::kick);
                break;

            case actions::stop:
                Evil->SetState(states::walk);
                break;
            }
            
        }

        if (Hero && Evil)
        {
            if (Hero->GetState() == states::punch && rand() % 3 == 1)
            {
                if (abs(Hero->x - Evil->x) <= 40)
                {
                    Evil->lifes -= Hero->GetHit();
                    if (sound)mciSendString(L"play .\\res\\snd\\badhurt.wav", NULL, NULL, NULL);
                    Evil->SetHitArea(Evil->x, Evil->y);
                    vHits.push_back(Evil->HitArea);
                    score += 9 + level;
                    if (Evil->GetDir() == dirs::left && Evil->x - 20 >= 0)
                    {
                        Evil->x -= 30;
                        Evil->SetEdges();
                    }
                    if (Evil->GetDir() == dirs::right && Evil->ex + 20 <= clW)
                    {
                        Evil->x += 30;
                        Evil->SetEdges();
                    }

                }
            }
            if (Hero->GetState() == states::kick && rand() % 3 == 1)
            {
                if (abs(Hero->x - Evil->x) <= 50)
                {
                    Evil->lifes -= Hero->GetHit() + 5;
                    if (sound)mciSendString(L"play .\\res\\snd\\badhurt.wav", NULL, NULL, NULL);
                    Evil->SetHitArea(Evil->x, Evil->y);
                    vHits.push_back(Evil->HitArea);
                    score += 9 + level;
                    if (Evil->GetDir() == dirs::left && Evil->x - 20 >= 0)
                    {
                        Evil->x -= 30;
                        Evil->SetEdges();
                    }
                    if (Evil->GetDir() == dirs::right && Evil->ex + 20 <= clW)
                    {
                        Evil->x += 30;
                        Evil->SetEdges();
                    }

                }
            }

            if (Evil->GetState() == states::punch && rand() % 2 == 0)
            {
                if (abs(Hero->ex - Evil->x) <= 40)
                {
                    Hero->lifes -= Evil->GetHit();
                    if (sound)mciSendString(L"play .\\res\\snd\\hurt.wav", NULL, NULL, NULL);
                    Hero->SetHitArea(Hero->x, Hero->y);
                    vHits.push_back(Hero->HitArea);
                    if (Hero->GetDir() == dirs::left && Hero->x - 20 >= 0)
                    {
                        Hero->x -= 40;
                        Hero->SetEdges();
                    }
                    if (Hero->GetDir() == dirs::right && Hero->ex + 20 <= clW)
                    {
                        Hero->x += 40;
                        Hero->SetEdges();
                    }
                }
               
            }
            if (Evil->GetState() == states::kick && rand() % 2 == 0)
            {
                if (abs(Hero->x - Evil->x) <= 50)
                {
                    Hero->lifes -= Evil->GetHit() + 5;
                    if (sound)mciSendString(L"play .\\res\\snd\\hurt.wav", NULL, NULL, NULL);
                    Hero->SetHitArea(Hero->x, Hero->y);
                    vHits.push_back(Hero->HitArea);
                    if (Hero->GetDir() == dirs::left && Hero->x - 20 >= 0)
                    {
                        Hero->x -= 40;
                        Hero->SetEdges();
                    }
                    if (Hero->GetDir() == dirs::right && Hero->ex + 20 <= clW)
                    {
                        Hero->x += 40;
                        Hero->SetEdges();
                    }
                }
                
            }
        }

        if (Evil)
        {
            if (Evil->lifes <= 0 && Evil->GetState() != states::fall)
            {
                Evil->SetState(states::fall);
                score += 50;
            }
        }

        if (Hero)
        {
            if (Hero->lifes <= 0)
            {
               Hero->SetState(states::fall);
            }
        }

        ///////////////////////////////////////////

        //DRAW THINGS ********************************
        Draw->BeginDraw();
        if (butBckg)
            Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), butBckg);
        if (nrmText && TxtBrush && TxtHgltBrush && TxtInactBrush)
        {
            if (name_set)
                Draw->DrawText(L"Име на играч", 13, nrmText, Visb1Rect, TxtInactBrush);
            else
            {
                if (b1Hglt)
                    Draw->DrawText(L"Име на играч", 13, nrmText, Visb1Rect, TxtHgltBrush);
                else
                    Draw->DrawText(L"Име на играч", 13, nrmText, Visb1Rect, TxtBrush);
            }
            if (b2Hglt)
                Draw->DrawText(L"Звуци ON / OFF", 15, nrmText, Visb2Rect, TxtHgltBrush);
            else
                Draw->DrawText(L"Звуци ON / OFF", 15, nrmText, Visb2Rect, TxtBrush);
            if (b3Hglt)
                Draw->DrawText(L"Помощ за играта", 16, nrmText, Visb3Rect, TxtHgltBrush);
            else
                Draw->DrawText(L"Помощ за играта", 16, nrmText, Visb3Rect, TxtBrush);

        }
        switch (level)
        {
        case 1:
            Draw->DrawBitmap(bmpField1, D2D1::RectF(0, 50.0f, scr_width, scr_height));
            break;

        case 2:
            Draw->DrawBitmap(bmpField2, D2D1::RectF(0, 50.0f, scr_width, scr_height));
            break;

        case 3:
            Draw->DrawBitmap(bmpField3, D2D1::RectF(0, 50.0f, scr_width, scr_height));
            break;

        case 4:
            Draw->DrawBitmap(bmpBossField, D2D1::RectF(0, 50.0f, scr_width, scr_height));
            break;
        }
        if (Cloud1)
            Draw->DrawBitmap(bmpCloud1, D2D1::RectF(Cloud1->x, Cloud1->y, Cloud1->ex, Cloud1->ey));
        if (Cloud2)
            Draw->DrawBitmap(bmpCloud2, D2D1::RectF(Cloud2->x, Cloud2->y, Cloud2->ex, Cloud2->ey));
        ///////////////////////////////////////////

        if(Hero)
            switch (Hero->GetState())
            {
            case states::walk:
                if (Hero->GetDir() == dirs::left)
                    Draw->DrawBitmap(bmpHeroWalkL, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));

                else 
                    Draw->DrawBitmap(bmpHeroWalkR, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                break;

            case states::fall:
                if (Hero->GetDir() == dirs::left)
                    Draw->DrawBitmap(bmpHeroFallL, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                else
                    Draw->DrawBitmap(bmpHeroFallR, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                break;

            case states::punch:
                if (Hero->GetDir() == dirs::left)
                {
                    if (Hero->GetStateFrame(states::punch) == 0)
                        Draw->DrawBitmap(bmpHeroPunch1L, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                    else if (Hero->GetStateFrame(states::punch) == 1)
                        Draw->DrawBitmap(bmpHeroPunch2L, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                }
                else
                {
                    if (Hero->GetStateFrame(states::punch) == 0)
                        Draw->DrawBitmap(bmpHeroPunch1R, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                    else if (Hero->GetStateFrame(states::punch) == 1)
                        Draw->DrawBitmap(bmpHeroPunch2R, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                }
                break;

            case states::kick:
                if (Hero->GetDir() == dirs::left)
                {
                    if (Hero->GetStateFrame(states::kick) == 0)
                        Draw->DrawBitmap(bmpHeroKick1L, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                    else if (Hero->GetStateFrame(states::kick) == 1)
                        Draw->DrawBitmap(bmpHeroKick2L, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                }
                else
                {
                    if (Hero->GetStateFrame(states::kick) == 0)
                        Draw->DrawBitmap(bmpHeroKick1R, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                    else if (Hero->GetStateFrame(states::kick) == 1)
                        Draw->DrawBitmap(bmpHeroKick2R, D2D1::RectF(Hero->x, Hero->y, Hero->ex, Hero->ey));
                }
                break;
            }

        if (Evil)
        {
            if (level == 1)
            {
                switch (Evil->GetState())
                {
                case states::walk:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::walk) == 0)
                            Draw->DrawBitmap(bmpNin1Walk1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 1)
                            Draw->DrawBitmap(bmpNin1Walk2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 2)
                            Draw->DrawBitmap(bmpNin1Walk3L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 3)
                            Draw->DrawBitmap(bmpNin1Walk3L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::walk) == 0)
                            Draw->DrawBitmap(bmpNin1Walk1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 1)
                            Draw->DrawBitmap(bmpNin1Walk2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 2)
                            Draw->DrawBitmap(bmpNin1Walk3R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 3)
                            Draw->DrawBitmap(bmpNin1Walk3R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;

                case states::fall:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin1FallL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpNin1FallR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::punch:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::punch) == 0)
                            Draw->DrawBitmap(bmpNin1Punch1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::punch) == 1)
                            Draw->DrawBitmap(bmpNin1Punch2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::punch) == 0)
                            Draw->DrawBitmap(bmpNin1Punch1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::punch) == 1)
                            Draw->DrawBitmap(bmpNin1Punch2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;

                case states::kick:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin1Kick1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin1Kick2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin1Kick1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin1Kick2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;
                }
            }
            if (level == 2)
            {
                switch (Evil->GetState())
                {
                case states::walk:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin2WalkL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));

                    else
                        Draw->DrawBitmap(bmpNin2WalkR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::fall:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin2FallL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpNin2FallR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::punch:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin2PunchL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpNin2PunchR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::kick:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin2Kick1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin2Kick2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin2Kick1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin2Kick2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;
                }
            }
            if (level == 3)
            {
                switch (Evil->GetState())
                {
                case states::walk:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin3WalkL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpNin3WalkR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::fall:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpNin3FallL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpNin3FallR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::punch:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::punch) == 0)
                            Draw->DrawBitmap(bmpNin3Punch1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::punch) == 1)
                            Draw->DrawBitmap(bmpNin3Punch2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::punch) == 0)
                            Draw->DrawBitmap(bmpNin3Punch1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::punch) == 1)
                            Draw->DrawBitmap(bmpNin3Punch2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;

                case states::kick:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin3Kick1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin3Kick2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpNin3Kick1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpNin3Kick2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;
                }
            }
            if (level == 4)
            {
                switch (Evil->GetState())
                {
                case states::walk:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::walk) == 0)
                            Draw->DrawBitmap(bmpBossWalk1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 1)
                            Draw->DrawBitmap(bmpBossWalk2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 2)
                            Draw->DrawBitmap(bmpBossWalk3L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 3)
                            Draw->DrawBitmap(bmpBossWalk3L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::walk) == 0)
                            Draw->DrawBitmap(bmpBossWalk1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 1)
                            Draw->DrawBitmap(bmpBossWalk2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 2)
                            Draw->DrawBitmap(bmpBossWalk3R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::walk) == 3)
                            Draw->DrawBitmap(bmpBossWalk3R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;

                case states::fall:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpBossFallL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpBossFallR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::punch:
                    if (Evil->GetDir() == dirs::left)
                        Draw->DrawBitmap(bmpBossPunchL, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    else
                        Draw->DrawBitmap(bmpBossPunchR, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    break;

                case states::kick:
                    if (Evil->GetDir() == dirs::left)
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpBossKick1L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpBossKick2L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 2)
                            Draw->DrawBitmap(bmpBossKick3L, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    else
                    {
                        if (Evil->GetStateFrame(states::kick) == 0)
                            Draw->DrawBitmap(bmpBossKick1R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 1)
                            Draw->DrawBitmap(bmpBossKick2R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                        else if (Evil->GetStateFrame(states::kick) == 2)
                            Draw->DrawBitmap(bmpBossKick3R, D2D1::RectF(Evil->x, Evil->y, Evil->ex, Evil->ey));
                    }
                    break;
                }
            }
        }

        if (!vHits.empty())
        {
            for (int i = 0; i < vHits.size(); ++i)
            {
                Draw->DrawBitmap(bmpHit, D2D1::RectF(vHits[i].x, vHits[i].y + 20.0f, vHits[i].ex, vHits[i].ey + 20.0f));
                vHits[i].cooldown--;
                if (vHits[i].cooldown <= 20)
                {
                    vHits.erase(vHits.begin() + i);
                    break;
                }
            }
        }

        if (Hero && Evil)
        {
         
            if (Hero->lifes > 80)
                Draw->DrawLine(D2D1::Point2F(Hero->x, Hero->y - 5.0f), D2D1::Point2F(Hero->x + (float)(Hero->lifes / 2), 
                    Hero->y - 5.0f), LifeBrush, 5.0f);
            else if (Hero->lifes > 30)
                Draw->DrawLine(D2D1::Point2F(Hero->x, Hero->y - 5.0f), D2D1::Point2F(Hero->x + (float)(Hero->lifes / 2), 
                    Hero->y - 5.0f), HurtBrush, 5.0f);
            else if (Hero->lifes > 0)
                Draw->DrawLine(D2D1::Point2F(Hero->x, Hero->y - 5.0f), D2D1::Point2F(Hero->x + (float)(Hero->lifes / 2), 
                    Hero->y - 5.0f), CritBrush, 5.0f);

            if (Evil->lifes > 80)
                Draw->DrawLine(D2D1::Point2F(Evil->x, Evil->ey + 5.0f), D2D1::Point2F(Evil->x + (float)(Evil->lifes / 2), 
                    Evil->ey + 5.0f), LifeBrush, 5.0f);
            else if (Evil->lifes > 30)
                Draw->DrawLine(D2D1::Point2F(Evil->x, Evil->ey + 5.0f), D2D1::Point2F(Evil->x + (float)(Evil->lifes / 2), 
                    Evil->ey + 5.0f), HurtBrush, 5.0f);
            else if (Evil->lifes > 0)
                Draw->DrawLine(D2D1::Point2F(Evil->x, Evil->ey + 5.0f), D2D1::Point2F(Evil->x + (float)(Evil->lifes / 2), 
                    Evil->ey + 5.0f), CritBrush, 5.0f);
        }

        ////////////////////////////////////////////

        wchar_t status_text[100] = L"\0";
        wchar_t add[10] = L"\0";
        int txt_size = 0;
        wcscpy_s(status_text, current_player);
        wcscat_s(status_text, L", ниво: ");
        wsprintf(add, L"%d", level);
        wcscat_s(status_text, add);
        wcscat_s(status_text, L", резултат: ");
        wsprintf(add, L"%d", score);
        wcscat_s(status_text, add);

        for (int i = 0; i < 100; i++)
        {
            if (status_text[i] != '\0')txt_size++;
            else break;
        }

        if (nrmText && TxtBrush)
            Draw->DrawTextW(status_text, txt_size, nrmText, D2D1::RectF(5.0f, 50.0f, clW, clH), TxtBrush);

        ////////////////////////////////////////////
        Draw->EndDraw();

        if(Hero)
            if (Hero->GetState() == states::fall)
            {
                fall_cooldown--;
                if (fall_cooldown < 0)GameOver();
            }

        if (Evil)
        {
            if (Evil->GetState() == states::fall)
            {
                if (Evil->GetType() != types::boss)
                {
                    fall_cooldown--;
                    if (fall_cooldown < 0)NextLevel();
                }
                else
                {
                    win_game = true;
                    GameOver();
                }
            }
        }
    }

    std::remove(tmp_file);
    ReleaseCOMs();
    return (int) bMsg.wParam;
}
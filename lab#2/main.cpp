#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "resource.h"

//MinHeight and MinWidth of the main Window
int iMinWindowWidth  = 600;
int iMinWindowHeight = 550;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hIconSm = LoadIcon (hThisInstance, MAKEINTRESOURCE(IDI_ICON));
    wincl.hCursor = LoadCursor (NULL, IDC_HAND);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDM_MENU);
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(173,216,230));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("WP lab2"),       /* Title Text */
           WS_OVERLAPPEDWINDOW , /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           iMinWindowWidth,                 /* The programs width */
           iMinWindowHeight,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HINSTANCE hInstance;
    HDC hdc;
	PAINTSTRUCT ps;
	SCROLLINFO si;
	TEXTMETRIC tm;
	RECT rect ;
	HBRUSH newbrush;

    // Child windows' handles
    static HWND hwndListBox, hwndNewItem, hwndAddButton, hwndRemoveButton, hwndClearButton,
                hwndBackgroundScroll, hwndHeightScroll, hwndWidthScroll, hwndLabel1, hwndLabel2, hwndLabel3;

    // Size and position variables
    int iSysWidth, iSysHeight;
    int iWinWidth, iWinHeight;

	 // Text size
    int cxChar, cyChar;

	int iWidth, iHeight, iListBoxWidth, iListBoxHeight;
	int x, y,i;

    // String
    char* szText;
    int iTextLength;

    //Variable for changing color of a static text
    int alfa = 0;

	hdc = GetDC(hwnd);
    GetTextMetrics(hdc, &tm);
    cxChar = tm.tmAveCharWidth;
    cyChar = tm.tmHeight;
    ReleaseDC(hwnd, hdc);
    static int xPos, xMin, xMax;

    switch (message)                  /* handle the messages */
    {
         case WM_CREATE:
            hwndListBox = CreateWindowEx(
                (DWORD)NULL,
                TEXT("ListBox"),
                NULL,
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | LBS_NOTIFY | LBS_WANTKEYBOARDINPUT | LBS_DISABLENOSCROLL,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_LIST_BOX,
                GetModuleHandle(NULL),
                NULL);

            hwndNewItem = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Edit"),
                TEXT(""),
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_NEW_ITEM,
                GetModuleHandle(NULL),
                NULL);

            hwndAddButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Add"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_ADD_BUTTON,
                GetModuleHandle(NULL),
                NULL);

            hwndRemoveButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Remove"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_REMOVE_BUTTON,
                GetModuleHandle(NULL),
                NULL);

            hwndClearButton = CreateWindowEx(
                (DWORD)NULL,
                TEXT("Button"),
                TEXT("Clear"),
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_CLEAR_BUTTON,
                GetModuleHandle(NULL),
                NULL);

            hwndLabel1 = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Color"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_LABEL1,
                GetModuleHandle(NULL),
                NULL);

            hwndLabel2 = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Width"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_LABEL2,
                GetModuleHandle(NULL),
                NULL);

            hwndLabel3 = CreateWindowEx(
                (DWORD)NULL,
                TEXT("static"),
                TEXT("Window Height"),
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0,
                hwnd,
                (HMENU)IDC_LABEL3,
                GetModuleHandle(NULL),
                NULL);

            hwndBackgroundScroll = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0, 0, 0, 0,
                hwnd,
                (HMENU)IDC_BACKGROUND_SCROLL,
                GetModuleHandle(NULL),
                NULL);

            SetScrollRange(hwndBackgroundScroll, SB_CTL, 0, 255, TRUE);
            SetScrollPos(hwndBackgroundScroll, SB_CTL, 125, TRUE);

            hwndWidthScroll = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_WIDTH_SCROLL,
                GetModuleHandle(NULL),
                NULL);
            SetScrollRange(hwndWidthScroll, SB_CTL, 0, 50, TRUE);

            hwndHeightScroll = CreateWindow(
                "Scrollbar",
                NULL,
                WS_CHILD | WS_VISIBLE | SBS_HORZ | SBS_BOTTOMALIGN,
                0, 0, 0, 0, hwnd,
                (HMENU)IDC_HEIGHT_SCROLL,
                GetModuleHandle(NULL),
                NULL);

            SetScrollRange(hwndHeightScroll, SB_CTL, 0, 100, TRUE);

            RegisterHotKey(hwnd, HK_EXIT, MOD_CONTROL, 0x51);
            RegisterHotKey(hwnd, HK_ABOUT, MOD_CONTROL, 0x49);
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;

            //Set Default Cursor Over all created Child
          case WM_SETCURSOR:
                if (LOWORD(lParam) == HTCLIENT)
                {
                    SetCursor(LoadCursor (NULL, IDC_HAND));
                    return TRUE;
                }
            break;

        case WM_PAINT: {
            HFONT hFontOld;
            HFONT CustomFont;
            CustomFont = CreateFont(-12, 0, 0, 0, FW_DONTCARE, false, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Mangal");
            hdc = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &rect);

            hFontOld = (HFONT)SelectObject(hdc, CustomFont);

            //Sending message to the static text about changing defaultfont to customFont
            SendMessage(hwndLabel1, WM_SETFONT, WPARAM(CustomFont), TRUE);
            SendMessage(hwndLabel2, WM_SETFONT, WPARAM(CustomFont), TRUE);
            SendMessage(hwndLabel3, WM_SETFONT, WPARAM(CustomFont), TRUE);

            DeleteObject(SelectObject(hdc,hFontOld));
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(25, 25,112));
            DrawText(hdc, TEXT("Shopping List"), -1, &rect, DT_SINGLELINE| DT_TOP| DT_CENTER);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO mmi = (LPMINMAXINFO)lParam;
            mmi->ptMinTrackSize.x = 450;
            mmi->ptMinTrackSize.y = 300;
            mmi->ptMaxTrackSize.x = 1150;
            mmi->ptMaxTrackSize.y = 750;
            break;
        }

         case WM_SIZE:
            iWidth  = LOWORD(lParam); // 600
            iHeight = HIWORD(lParam); // 550
            iListBoxWidth = iWidth/2 + 50;
            iListBoxHeight = iHeight - 80;

            // Set vertical scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = ((iMinWindowHeight - 150) / cyChar);
            si.nPage = iHeight / cyChar;
            SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

            // Set horizontal scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE;
            si.nMin = 0;
            si.nMax = ((iMinWindowWidth - 100) / cxChar);
            si.nPage = iWidth / cxChar;
            SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);

            //Child window reposition
            MoveWindow(hwndListBox, 20, 65, iListBoxWidth, iListBoxHeight, TRUE);
            MoveWindow(hwndNewItem, 20, 30, iListBoxWidth/2 + 45, 25, TRUE);
            MoveWindow(hwndAddButton, iListBoxWidth/2 + 75,30, iListBoxWidth/2 - 55, 25, TRUE);

            MoveWindow(hwndRemoveButton, iWidth/2 + 90, 70, (iWidth - (iWidth/2 + 90) - 20), 25, TRUE);
            MoveWindow(hwndClearButton, iWidth/2 + 90, 105, (iWidth - (iWidth/2 + 90) - 20), 25, TRUE);

            MoveWindow(hwndLabel1, iWidth/2 + 90, 175, (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);
            MoveWindow(hwndBackgroundScroll, iWidth/2 + 90, 200, (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);

            MoveWindow(hwndLabel2, iWidth/2 + 90, 240,  (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);
            MoveWindow(hwndWidthScroll, iWidth/2 + 90, 265,  (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);

            MoveWindow(hwndLabel3, iWidth/2 + 90, 305,  (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);
            MoveWindow(hwndHeightScroll, iWidth/2 + 90, 330,  (iWidth - (iWidth/2 + 90) - 20), 20, TRUE);

            GetWindowRect(hwnd, &rect);
            iWinWidth = rect.right - rect.left;
            iWinHeight = rect.bottom - rect.top;
            iSysWidth = GetSystemMetrics(SM_CXSCREEN);
            iSysHeight = GetSystemMetrics(SM_CYSCREEN);

            xPos = 0;
            xMin = 0;
            xMax = 255;
            SetScrollRange(hwndBackgroundScroll, SB_CTL, xMin, xMax, FALSE);
            SetScrollPos(hwndBackgroundScroll, SB_CTL, xPos, TRUE);

            // Set width scrollbar position
            SetScrollPos(hwndWidthScroll, SB_CTL, (iWinWidth * 100 / iSysWidth), TRUE);

            // Set height scrollbar position
            SetScrollPos(hwndHeightScroll, SB_CTL, (iWinHeight * 100 / iSysHeight), TRUE);
			break;

        case WM_VSCROLL:
				// Get all the vertical scroll bar information
				si.cbSize = sizeof(si);
				si.fMask = SIF_ALL;
				GetScrollInfo(hwnd, SB_VERT, &si);

				// Save the position for later comparison
				y = si.nPos;

				switch(LOWORD(wParam))
				{
					case SB_TOP:
						si.nPos = si.nMin;
					break;

					case SB_BOTTOM:
						si.nPos = si.nMax;
					break;

					case SB_LINEUP:
						si.nPos -= 1;
					break;

					case SB_LINEDOWN:
						si.nPos += 1;
					break;

					case SB_PAGEUP:
						si.nPos -= si.nPage;
					break;

					case SB_PAGEDOWN:
						si.nPos += si.nPage;
					break;

					case SB_THUMBTRACK:
						si.nPos = si.nTrackPos;
					break;

					default:
						break;
				}

				// Set the position and then retrieve it
				si.fMask = SIF_POS;
				SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
				GetScrollInfo(hwnd, SB_VERT, &si);

				// If the position has changed, scroll the window and update it
				if(si.nPos != y) {
					ScrollWindow(hwnd, 0, cyChar * (y - si.nPos), NULL, NULL);
					UpdateWindow(hwnd);
				}
            break;

         case WM_CTLCOLORSTATIC:
                SetTextColor((HDC)wParam, RGB(alfa, 0, 255 - alfa));
				SetBkMode((HDC)wParam, TRANSPARENT);
				newbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                return (LRESULT)newbrush;
			break;

        case WM_CTLCOLOREDIT:
        {
            SetTextColor((HDC)wParam, RGB(xPos+1, 123, 255-xPos));
            SetBkMode((HDC)wParam, RGB(255, 0, 100));
            HBRUSH locBrush=(HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
            return(LRESULT) locBrush;
        }

        case WM_HSCROLL:
				GetWindowRect(hwnd, &rect);
				iWinWidth = rect.right - rect.left;
				iWinHeight = rect.bottom - rect.top;
				iSysWidth = GetSystemMetrics(SM_CXSCREEN);
				iSysHeight = GetSystemMetrics(SM_CYSCREEN);

                if((HWND)lParam == hwndBackgroundScroll)
                {
                    switch(LOWORD(wParam))
                    {
                        case SB_LINELEFT:
                        {
                            xPos -= 1;
                            break;
                        }
                        case SB_LINERIGHT:
                        {
                            xPos += 1;
                            break;
                        }
                        case SB_PAGELEFT:
                        {
                            xPos -= 10;
                            break;
                        }
                        case SB_PAGERIGHT:
                        {
                            xPos += 10;
                            break;
                        }
                        case SB_TOP:
                        {
                            xPos = xMin;
                            break;
                        }
                        case SB_BOTTOM:
                        {
                            xPos = xMax;
                            break;
                        }
                        case SB_THUMBPOSITION:
                        {
                            break;
                        }
                        case SB_THUMBTRACK:
                        {
                            xPos = HIWORD(wParam);
                            break;
                        }
                        default:
                            break;
                    }
                    SetScrollPos(hwndBackgroundScroll, SB_CTL, xPos, TRUE);
                    InvalidateRect(hwndNewItem, NULL, TRUE);
                    break;
                }
					if(GetWindowLong((HWND)lParam, GWL_ID) == IDC_WIDTH_SCROLL)
					{
						si.cbSize = sizeof(si);
						si.fMask = SIF_ALL;
						GetScrollInfo(hwndWidthScroll, SB_CTL, &si);
						x = si.nPos;

						switch(LOWORD(wParam))
						{
							case SB_LINELEFT:
								si.nPos -= 1;
							break;

							case SB_LINERIGHT:
								si.nPos += 1;
							break;

							case SB_THUMBPOSITION:
								si.nPos = si.nTrackPos;
							break;
							default: break;
						}

						si.fMask = SIF_POS;
						SetScrollInfo(hwndWidthScroll, SB_CTL, &si, TRUE);
						GetScrollInfo(hwndWidthScroll, SB_CTL, &si);

						if(si.nPos != x)
						{
							SetScrollPos(hwndWidthScroll, SB_CTL, si.nPos, TRUE);
						}

						// Set window width
						MoveWindow(hwnd, rect.left, rect.top, (si.nPos * iSysWidth / 100), iWinHeight, TRUE);
						break;
					}

					if(GetWindowLong((HWND)lParam, GWL_ID) == IDC_HEIGHT_SCROLL)
					{
						si.cbSize = sizeof(si);
						si.fMask = SIF_ALL;
						GetScrollInfo(hwndHeightScroll, SB_CTL, &si);
						x = si.nPos;

						switch(LOWORD(wParam))
						{
							case SB_LINELEFT:
								si.nPos -= 1;
							break;

							case SB_LINERIGHT:
								si.nPos += 1;
							break;

							case SB_THUMBPOSITION:
								si.nPos = si.nTrackPos;
							break;

							default: break;
						}

						si.fMask = SIF_POS;
						SetScrollInfo(hwndHeightScroll, SB_CTL, &si, TRUE);
						GetScrollInfo(hwndHeightScroll, SB_CTL, &si);

						if(si.nPos != x)
						{
							SetScrollPos(hwndHeightScroll, SB_CTL, si.nPos, TRUE);
						}

						// Set window height
						MoveWindow(hwnd, rect.left, rect.top, iWinWidth, (si.nPos * iSysHeight / 100), TRUE);
						break;
					}

					// Get all the vertical scroll bar information
					si.cbSize = sizeof(si);
					si.fMask = SIF_ALL;
					GetScrollInfo(hwnd, SB_HORZ, &si);

					// Save the position for later comparison
					x = si.nPos;
					switch(LOWORD(wParam))
					{
						case SB_LINELEFT:
							si.nPos -= 1;
						break;

						case SB_LINERIGHT:
							si.nPos += 1;
						break;

						case SB_PAGELEFT:
							si.nPos -= si.nPage;
						break;

						case SB_PAGERIGHT:
							si.nPos += si.nPage;
						break;

						case SB_THUMBPOSITION:
							si.nPos = si.nTrackPos;
						break;

						default:break;
					}

					// Set the position and then retrieve it
					si.fMask = SIF_POS;
					SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
					GetScrollInfo(hwnd, SB_HORZ, &si);

					// If the position has changed, scroll the window and update it
					if(si.nPos != x)
					{
						ScrollWindow(hwnd, cxChar * (x - si.nPos), 0, NULL, 0);
						UpdateWindow(hwnd);
					}
			break;


        case WM_COMMAND:
        {
             switch (LOWORD(wParam)) {
                 case IDC_LIST_BOX:
                {
                    if(HIWORD(wParam) == LBN_DBLCLK)
                    {
                        int index = SendMessage(GetDlgItem(hwnd, IDC_LIST_BOX), LB_GETCURSEL, 0, 0);
                        int textLength = SendMessage(hwndListBox, LB_GETTEXTLEN, (WPARAM)index, 0);
                        TCHAR * buffer = new TCHAR[textLength + 1];
                        buffer[textLength + 1] = _T('\0');
                        SendMessage(hwndListBox, LB_GETTEXT, (WPARAM)index, (LPARAM)buffer);
                        char buffer1[50] = "You've selected \0";
                        char * buffer2 = new char[textLength + 1 + strlen(buffer1) + 20];
                        buffer2[0] = '\0';
                        strcat(buffer2, buffer1); strcat(buffer2, buffer);
                        MessageBox(NULL, buffer2, "About", MB_OK);
                        delete [] buffer;
                        delete [] buffer2;
                    }
                    break;
                }

                case IDC_ADD_BUTTON:
                    iTextLength = SendMessage(hwndNewItem, WM_GETTEXTLENGTH, 0, 0);
                    szText = (char*)malloc(iTextLength+1);
                    SendMessage(hwndNewItem, WM_GETTEXT, iTextLength+1, (LPARAM)szText);
                    SendMessage(hwndListBox, LB_ADDSTRING, 0, (LPARAM)szText);
                    SendMessage(hwndNewItem, WM_SETTEXT, 0, (LPARAM)"");
                    free(szText);
                    break;

                case IDC_REMOVE_BUTTON:
                    i = SendMessage(hwndListBox, LB_GETCURSEL, 0, 0);
                    if(i != LB_ERR) {
                        SendMessage(hwndListBox, LB_DELETESTRING, i, 0);
                    }
                    break;

                case IDC_CLEAR_BUTTON:
                    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
                    break;

                case ID_EXIT:
                    SendMessage(hwnd, WM_DESTROY, 0, 0);
                    break;

                case ID_DATA:
                    SendMessage(hwndListBox, LB_RESETCONTENT, 0, 0);
                    break;

                case ID_ABOUT:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, AboutDlgProc);
                    break;

                default:
                    DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
                    break;
            }
            break;
        }

        case WM_HOTKEY:
        {
            switch(wParam)
            {
                case HK_EXIT:
                    PostQuitMessage(0);
                    break;

                case HK_ABOUT:
                    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG), hwnd, AboutDlgProc);
                    break;
                default:
                    break;
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return TRUE;
            }
    }
    return FALSE;
}

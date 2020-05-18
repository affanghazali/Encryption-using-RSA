// RSAGUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RSAGUI.h"
#include <iostream>
# include <fstream>

#define MAX_LOADSTRING 100

using namespace std;
// extern "C" instruct the compiler to use C calling conventions
extern"C" {
	void Entrypoint(int x ,  int y ,int z  , int mode);
	void Decrypt (int x , int y , int z);
	void FormulaForPrime(int val);
	void PublicKey (int   x  , int y);
	void PrivateKey (int x  , int y  , int z ); 

	void GetEncryptedData (int data , int Nval , int Dval);

	int gcd(int a, int h) ; 
	void TRYERROR(int D  , int N , int Data);
	int powerP (int a  , int b , int c );
}


int count  =0  ;  

void readFile ( string data){
	ofstream myfile ("Encrypted.txt");
  if (myfile.is_open())
  {
    
	  myfile << data.c_str();
    myfile.close();
  }
  else cout << "Unable to open file";
  

	  
}


// Global Variables:

int EncrpytedData= 0 ;
int N = 0 ; 
int D =0  ;

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RSAGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RSAGUI));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RSAGUI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RSAGUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
int pointer = 0 ;
char * arrayOfEncrypt =  new char [100];
char * arrayOfEncryptChar =  new char [100];
	//for ( int  i=0  ; i <  100  ; i ++)  arrayOfEncrypt[i] = NULL ;  
bool flag =  false  ;
char * container =  new char [100] ;  
string decrypt  = "" ;
	//for ( int  i=0  ; i <  100  ; i ++)  container[i] = NULL ; 
HWND textbox , b1 , b2 ;
HWND	display;
int index  =0 ; 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_CREATE:
		 
		textbox = CreateWindow("EDIT","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,10,10,700,200,hWnd,NULL,NULL,NULL);
		b1 = CreateWindow("BUTTON","ENCRYPT",WS_VISIBLE|WS_CHILD|WS_BORDER,800,30,120,30,hWnd,(HMENU) 1,NULL,NULL);
		b2 = CreateWindow("BUTTON","DECRYPT",WS_VISIBLE|WS_CHILD|WS_BORDER,800,90,120,30,hWnd,(HMENU) 2,NULL,NULL);
		
		//cout << "kmk" ;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case 1 :{
			for ( int  i=0  ; i <  100  ; i ++)  arrayOfEncrypt[i] = NULL ;  
			for ( int  i=0  ; i <  100  ; i ++)  arrayOfEncryptChar[i] = NULL ;  
			for ( int  i=0  ; i <  100  ; i ++)  container[i] = NULL ; 

			int exception =  GetWindowText(textbox,container,100);
			for ( int i =0  ;i <  100 ;i ++ ) {
				if (container[i] != NULL && container[i] != '\n'){
					Entrypoint(3, 7 , container[i] , 0);
					//Entrypoint(x, y ,EncrpytedData  , 1);
					arrayOfEncryptChar[pointer] =  char(EncrpytedData + 105 );
					arrayOfEncrypt [pointer] =  EncrpytedData ;
					//cout << char(EncrpytedData + 105) ; 
					pointer++;
				}
			}
	
				 flag  = true  ; 
			if (exception){ 
				display =  CreateWindow("STATIC",arrayOfEncryptChar,WS_VISIBLE|WS_CHILD|WS_BORDER,10 ,220,700,100,hWnd,NULL,NULL,NULL);
				readFile(arrayOfEncryptChar);
			}
				}
			break ;
		

		case 2 :{
	
				if (!flag){
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				}
				else {
					for ( int i =0  ;i <  pointer ;i ++){
						Decrypt(D , N , int(arrayOfEncrypt[i]));
					}
					DestroyWindow(display);
					CreateWindow("STATIC",decrypt.c_str(),WS_VISIBLE|WS_CHILD|WS_BORDER,10 ,220,700,100,hWnd,NULL,NULL,NULL);
					readFile(decrypt);
					pointer  = 0  ; 
					decrypt ="";
				}
				

				}
			break ;

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
int gcd(int a, int h) 
{ 
    int temp; 
    while (1) 
    { 
        temp = a%h; 
        if (temp == 0) 
          return h; 
        a = h; 
        h = temp; 
    } 
} 
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
int powerP  (int x , int y , int e) {
	//cout  <<  "Ans :" ;
	double n =  pow(double(e) , double(x));
	double p =  fmod( n, double (y) );

	return int (p);
}

void  GetEncryptedData (int data , int Dval , int Nval ) { 
	EncrpytedData = data  ; 
	D = Dval ; 
	N = Nval ;
}

void TRYERROR (int D ,  int N ,  int data ){
	
	if (data == 32  ) decrypt.append(1 ,' ') ;
	else {
	double x  =  pow(double (data) , double (D));
	x = fmod(x , double(N));
	decrypt.append(1 ,char(x + 105));
	}
	
}

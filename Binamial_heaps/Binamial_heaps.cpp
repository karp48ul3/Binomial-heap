// Binamial_heaps.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Binamial_heaps.h"
#include <atlstr.h>
#include <random>
#include <string>
#include <fstream>
#include <list>
#include <tuple>
#include <charconv>

#define IDC_BUTTON_INSERT_NODE 1001
#define IDC_BUTTON_EXTRACT_MIN 1002
#define IDC_BUTTON_DECREASE_KEY 1003
#define IDC_BUTTON_DELETE_NODE 1004
#define IDC_EDIT_INSERT_NODE 1005
#define IDC_EDIT_EXTRACT_MIN 1006
#define IDC_EDIT_DECREASE_KEY 1007


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BINAMIALHEAPS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BINAMIALHEAPS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BINAMIALHEAPS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BINAMIALHEAPS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

HWND hWnd;

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
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_VSCROLL | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   /*RECT rect;
   rect.left = 0;
   rect.top = 0;
   rect.right = 1280;
   rect.bottom = 1280;
   AdjustWindowRectEx(&rect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, 0);

   wchar_t* wString = new wchar_t[4096];
   MultiByteToWideChar(CP_ACP, 0, "Test window", -1, wString, 4096);

   hWnd = CreateWindowEx(0, szWindowClass, wString, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPED, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);*/


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
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
bool windowShown = false;

int ButtonWidth = 150;
int editTextWidth = 50;

static int  cxChar, cxCaps, cyChar, cyClient, VscrollPos = 0;
bool isScrollDown = true;
int i;
PAINTSTRUCT ps;
char buffer[15];
TEXTMETRIC tm;
HFONT hFont;
int leftMargin = 10;
int topMarginAdjust = 10;
int topMargin = 20;
int circleWidth = 50;
int circleHeight = 50;
bool createFont = true;
std::vector<int> vec_rnd_num{};
int tree_height = 50;
std::vector<int> vec_Numbers{};
std::vector < std::pair<int, std::tuple<int, int, int, int>>> circleNodesCoordinates;
int selectedNode = -1;
int selectedNodeActual = -1; // points to green border node, if none is selected then -1;
bool isNodeSelected = false;

HWND hwndButtonInsertNode;
HWND hwndButtonExtractMin;
HWND hwndButtonDecreaseKey;
HWND hwndButtonDeleteNode;

HWND hwndEditInsertNode;
HWND hwndEditExtractMin;
HWND hwndEditDecreaseKey;
RECT rc2;
RECT previousRect;

struct Node {
    Node* parent;
    int key;
    int degree;
    Node* leftChild;
    Node* rightSibling;
};
Node* nodeSelectedType;
std::list<Node*> mainHeap;

void BinomialLink(Node* y, Node* z) {
    y->parent = z;
    y->rightSibling = z->leftChild;
    z->leftChild = y;
    z->degree = z->degree + 1;
}
std::list<Node*> BinomialHeapUnion(std::list<Node*> H1, std::list<Node*> H2) {
    if (H1.size() == 0) {
        return H2;
    }
    else if (H2.size() == 0) {
        return H1;
    }

    //H1 and H2 have at least 1 element
    //make binomial heap H
    std::list<Node*> H;
    auto itH1 = H1.begin();
    auto itH2 = H2.begin();
    auto isEnd = false;
    Node* n1 = NULL;

    while (!isEnd) {
        int h1Degree = 100000;// = (*itH1)->degree;
        int h2Degree = 100000;
        if ((*itH1) == NULL && (*itH2) == NULL) {
            isEnd = true;
            break;
        }
        else if((*itH1) == NULL) {
            h2Degree = (*itH2)->degree;
        }
        else if ((*itH2) == NULL) {
            h1Degree = (*itH1)->degree;
        }
        else {
            h2Degree = (*itH2)->degree;
            h1Degree = (*itH1)->degree;
        }


        if (h1Degree < h2Degree){
            H.push_back((*itH1));
            if (n1 != NULL) {
                n1->rightSibling = *itH1;
            }
            n1 = (*itH1);
            //n1->rightSibling = NULL;
            *itH1 = n1->rightSibling;
        }
        else {
            H.push_back((*itH2));
            if (n1 != NULL) {
                n1->rightSibling = *itH2;
                
            }
            n1 = (*itH2);
            *itH2 = n1->rightSibling;
        }
    }
    //we have merged heap H
    if (H.size() == 0) {
        return H;
    }
    Node* prevX = NULL;
    Node* x = *(H.begin());
    int xItInt = 0;
    auto xIt = H.begin();
    Node* nextX = x->rightSibling;
    while (nextX != NULL) {
        if (x->key == 4) {
            auto breakpointt = 5;
        }
        if ((x->degree != nextX->degree) || (nextX->rightSibling != NULL && (nextX->rightSibling)->degree == x->degree)) {
            prevX = x;
            x = nextX;
            xItInt++;
        }
        else if (x->key <= nextX->key) {
            x->rightSibling = nextX->rightSibling;
            BinomialLink(nextX, x);
            //auto idx = H.
            H.remove(nextX);
            
        }
        else {
            if (prevX == NULL) {
                //xIt += xItInt;
                H.remove(x);
                //*(H.begin()) = nextX;
            }
            else {
                prevX->rightSibling = nextX;
            }
            BinomialLink(x, nextX);
            H.remove(x);
            x = nextX;
        }
        nextX = x->rightSibling;
    }
    return H;
}

std::list<Node*> BinomialHeapInsert(std::list<Node*> H, int x) {
    Node* h = new Node();
    h->parent = NULL;
    h->leftChild = NULL;
    h->rightSibling = NULL;
    h->degree = 0;
    h->key = x;
    std::list<Node*> listWithARoot;
    listWithARoot.push_back(h);
    H = BinomialHeapUnion(H, listWithARoot);
    return H;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BUTTON_INSERT_NODE:
            {
                //SetWindowPos(hwndButtonInsertNode, HWND_TOP, 100 - 100, 0, 100, 25, NULL);
                TCHAR buff[1024];
                GetWindowText(hwndEditInsertNode, buff, 1024);
                //buffToStr;
                std::wstring wStr = buff;
                std::string buffToStr = std::string(wStr.begin(), wStr.end());
                int number = -1000;
                bool numberAlreadyexists = false;
                if (buffToStr.empty() || buffToStr.size() == 0) {
                    MessageBox(hWnd, L"Type number to be inserted into the heap", L"Error", MB_ICONERROR);
                    break;
                }
                try {
                    std::from_chars(buffToStr.data(), buffToStr.data() + buffToStr.size(), number);
                }
                catch (std::exception& ex) {
                    MessageBox(hWnd, L"Text you specified is not a valid number", L"Error", MB_ICONERROR);
                    break;
                }
                if (number == -1000) {
                    MessageBox(hWnd, L"Text you specified is not a valid number", L"Error", MB_ICONERROR);
                }
                else if (number < 0) {
                    MessageBox(hWnd, L"Number must be greater than zero", L"Error", MB_ICONERROR);
                }
                else {// check if we have this number in vector input
                    for (int i = 0; i < vec_Numbers.size(); ++i) {
                        if (vec_Numbers[i] == number) {
                            MessageBox(hWnd, L"Specified number already exists in the heap", L"Error", MB_ICONERROR);
                            numberAlreadyexists = true;
                            break;;
                        }
                    }
                    if (!numberAlreadyexists) {
                        vec_Numbers.push_back(number);

                        mainHeap = BinomialHeapInsert(mainHeap, number);
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        InvalidateRect(hWnd, &rect, TRUE);
                    }
                }
            }
                break;
            case IDC_BUTTON_EXTRACT_MIN:
            {
                //hwndEditExtractMin
                //mainHeap
                if (mainHeap.size() == 0) {
                    MessageBox(hWnd, L"Heap empty. Cannot extract min", L"Error", MB_ICONERROR);
                    break;
                }
                auto mainHeapIterator = mainHeap.begin();
                auto currentMinIterator = mainHeap.begin();
                int minimum = 10000000;
                while (mainHeapIterator != mainHeap.end()) {
                    if ((*mainHeapIterator)->key < minimum) {
                        minimum = (*mainHeapIterator)->key;
                        currentMinIterator = mainHeapIterator;
                    }
                    mainHeapIterator++;
                }

                auto nodeToRemove = (*currentMinIterator);
                auto nodeToRemoveIterator = currentMinIterator;

                if (currentMinIterator != mainHeap.begin()) {
                    currentMinIterator++;
                    if (currentMinIterator != mainHeap.end()) {
                        auto itToNextCurrent = currentMinIterator;
                        currentMinIterator--;
                        currentMinIterator--;
                        (*currentMinIterator)->rightSibling = (*itToNextCurrent);
                    }
                    else {
                        currentMinIterator--;
                        currentMinIterator--;
                        (*currentMinIterator)->rightSibling = NULL;
                    }
                }
                else {

                }

                for (int i = 0; i < vec_Numbers.size(); ++i) {
                    if (vec_Numbers[i] == minimum) {
                        auto i1 = vec_Numbers.begin();
                        std::advance(i1, i);
                        vec_Numbers.erase(i1);
                        break;
                    }
                }

                mainHeap.remove(*nodeToRemoveIterator);

                std::vector<Node*> childrenOfNodeToRemove;
                if (nodeToRemove->leftChild == NULL) {
                    //do nothing 
                    auto minimumStr = std::to_string(minimum);
                    std::wstring stemp = std::wstring(minimumStr.begin(), minimumStr.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowText(hwndEditExtractMin, sw);
                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    InvalidateRect(hWnd, &rect, TRUE);
                    break;
                }
                
                Node* firstChild = nodeToRemove->leftChild;
                Node* tempNode = firstChild->rightSibling;
                for (int i = 0; i < nodeToRemove->degree; ++i) {
                    auto firstChildTemp = firstChild;
                    childrenOfNodeToRemove.push_back(firstChild);
                    firstChild = tempNode;
                    if (i == 0) {
                        firstChildTemp->rightSibling = NULL;
                        firstChildTemp->parent = NULL;
                    }
                    if (firstChild == NULL) {
                        break;
                    }
                    firstChild->parent = NULL;
                    tempNode = firstChild->rightSibling;
                    firstChild->rightSibling = firstChildTemp;
                    
                }
                //reverse the vector
                std::vector<Node*> childrenOfNodeToRemoveReverse;

                for (int i = childrenOfNodeToRemove.size()-1; i >= 0; --i) {
                    childrenOfNodeToRemoveReverse.push_back(childrenOfNodeToRemove[i]);
                }

                std::list<Node*> heapOfchildrenOfNodesToRemove;

                for (int i = 0; i < childrenOfNodeToRemoveReverse.size(); ++i) {
                    heapOfchildrenOfNodesToRemove.push_back(childrenOfNodeToRemoveReverse[i]);
                }

                auto h1 = BinomialHeapUnion(mainHeap, heapOfchildrenOfNodesToRemove);
                mainHeap = h1;

                hwndEditExtractMin;
                auto minimumStr = std::to_string(minimum);
                std::wstring stemp = std::wstring(minimumStr.begin(), minimumStr.end());
                LPCWSTR sw = stemp.c_str();
                SetWindowText(hwndEditExtractMin, sw);
                RECT rect;
                GetClientRect(hWnd, &rect);
                InvalidateRect(hWnd, &rect, TRUE);
            }
                break;
            case IDC_BUTTON_DECREASE_KEY:
            {
                if (selectedNodeActual == -1) {
                    MessageBox(hWnd, L"Please select a node to decrease key", L"Error", MB_ICONERROR);
                    break;
                }

                TCHAR buff[1024];
                GetWindowText(hwndEditDecreaseKey, buff, 1024);
                //buffToStr;
                std::wstring wStr = buff;
                std::string buffToStr = std::string(wStr.begin(), wStr.end());
                int number = -1000;
                bool numberAlreadyexists = false;
                if (buffToStr.empty() || buffToStr.size() == 0) {
                    MessageBox(hWnd, L"Type key to decrease selected node", L"Error", MB_ICONERROR);
                    break;
                }
                try {
                    std::from_chars(buffToStr.data(), buffToStr.data() + buffToStr.size(), number);
                }
                catch (std::exception& ex) {
                    MessageBox(hWnd, L"Key you specified is not a valid number", L"Error", MB_ICONERROR);
                    break;
                }
                if (number == -1000) {
                    MessageBox(hWnd, L"Key you specified is not a valid number", L"Error", MB_ICONERROR);
                }
                else if (number < 0) {
                    MessageBox(hWnd, L"Key must be greater than zero", L"Error", MB_ICONERROR);
                }
                else {// check if we have this number in vector input
                    for (int i = 0; i < vec_Numbers.size(); ++i) {
                        if (vec_Numbers[i] == number) {
                            MessageBox(hWnd, L"Specified key already exists in the heap", L"Error", MB_ICONERROR);
                            numberAlreadyexists = true;
                            break;;
                        }
                    }
                    if (!numberAlreadyexists) {
                        //vec_Numbers.push_back(number);

                        //mainHeap = BinomialHeapInsert(mainHeap, number);
                        //nodeSelectedType;
                        if (number >= nodeSelectedType->key) {
                            MessageBox(hWnd, L"Specified key must be less than selected key", L"Error", MB_ICONERROR);
                            break;
                        }
                        else {
                            for (int i = 0; i < vec_Numbers.size(); ++i) {
                                if (vec_Numbers[i] == nodeSelectedType->key) {
                                    vec_Numbers[i] = number;
                                    break;
                                }
                            }
                        }
                        nodeSelectedType->key = number;
                        Node* y = nodeSelectedType;
                        Node* z = y->parent;
                        while (z != NULL && y->key < z->key) {
                            int temp = y->key;
                            y->key = z->key;
                            z->key = temp;
                            y = z;
                            z = y->parent;
                        }
                        selectedNodeActual = -1;
                        selectedNode = -1;
                        isNodeSelected = false;
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        InvalidateRect(hWnd, &rect, TRUE);
                    }
                }
            }
                break;
            case IDC_BUTTON_DELETE_NODE:
            {
                if (selectedNodeActual == -1) {
                    MessageBox(hWnd, L"Please select a node to delete", L"Error", MB_ICONERROR);
                    break;
                }

                
                    //vec_Numbers.push_back(number);
                    auto number = -10000;
                    //mainHeap = BinomialHeapInsert(mainHeap, number);
                    //nodeSelectedType;
                    if (number >= nodeSelectedType->key) {
                        MessageBox(hWnd, L"Specified key must be less than selected key", L"Error", MB_ICONERROR);
                        break;
                    }
                    else {
                        for (int i = 0; i < vec_Numbers.size(); ++i) {
                            if (vec_Numbers[i] == nodeSelectedType->key) {
                                vec_Numbers[i] = number;
                                break;
                            }
                        }
                    }
                    nodeSelectedType->key = number;
                    Node* y = nodeSelectedType;
                    Node* z = y->parent;
                    while (z != NULL && y->key < z->key) {
                        int temp = y->key;
                        y->key = z->key;
                        z->key = temp;
                        y = z;
                        z = y->parent;
                    }
                    selectedNodeActual = -1;
                    selectedNode = -1;
                    isNodeSelected = false;
                    /*RECT rect;
                    GetClientRect(hWnd, &rect);
                    InvalidateRect(hWnd, &rect, TRUE);*/

                    if (mainHeap.size() == 0) {
                        MessageBox(hWnd, L"Heap empty. Cannot delete Node", L"Error", MB_ICONERROR);
                        break;
                    }
                    auto mainHeapIterator = mainHeap.begin();
                    auto currentMinIterator = mainHeap.begin();
                    int minimum = 10000000;
                    while (mainHeapIterator != mainHeap.end()) {
                        if ((*mainHeapIterator)->key < minimum) {
                            minimum = (*mainHeapIterator)->key;
                            currentMinIterator = mainHeapIterator;
                        }
                        mainHeapIterator++;
                    }

                    auto nodeToRemove = (*currentMinIterator);
                    auto nodeToRemoveIterator = currentMinIterator;

                    if (currentMinIterator != mainHeap.begin()) {
                        currentMinIterator++;
                        if (currentMinIterator != mainHeap.end()) {
                            auto itToNextCurrent = currentMinIterator;
                            currentMinIterator--;
                            currentMinIterator--;
                            (*currentMinIterator)->rightSibling = (*itToNextCurrent);
                        }
                        else {
                            currentMinIterator--;
                            currentMinIterator--;
                            (*currentMinIterator)->rightSibling = NULL;
                        }
                    }
                    else {

                    }

                    for (int i = 0; i < vec_Numbers.size(); ++i) {
                        if (vec_Numbers[i] == minimum) {
                            auto i1 = vec_Numbers.begin();
                            std::advance(i1, i);
                            vec_Numbers.erase(i1);
                            break;
                        }
                    }

                    mainHeap.remove(*nodeToRemoveIterator);

                    std::vector<Node*> childrenOfNodeToRemove;
                    if (nodeToRemove->leftChild == NULL) {
                        //do nothing 
                        /*auto minimumStr = std::to_string(minimum);
                        std::wstring stemp = std::wstring(minimumStr.begin(), minimumStr.end());
                        LPCWSTR sw = stemp.c_str();*/
                        //SetWindowText(hwndEditExtractMin, sw);
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        InvalidateRect(hWnd, &rect, TRUE);
                        break;
                    }

                    Node* firstChild = nodeToRemove->leftChild;
                    Node* tempNode = firstChild->rightSibling;
                    for (int i = 0; i < nodeToRemove->degree; ++i) {
                        auto firstChildTemp = firstChild;
                        childrenOfNodeToRemove.push_back(firstChild);
                        firstChild = tempNode;
                        if (i == 0) {
                            firstChildTemp->rightSibling = NULL;
                            firstChildTemp->parent = NULL;
                        }
                        if (firstChild == NULL) {
                            break;
                        }
                        firstChild->parent = NULL;
                        tempNode = firstChild->rightSibling;
                        firstChild->rightSibling = firstChildTemp;

                    }
                    //reverse the vector
                    std::vector<Node*> childrenOfNodeToRemoveReverse;

                    for (int i = childrenOfNodeToRemove.size() - 1; i >= 0; --i) {
                        childrenOfNodeToRemoveReverse.push_back(childrenOfNodeToRemove[i]);
                    }

                    std::list<Node*> heapOfchildrenOfNodesToRemove;

                    for (int i = 0; i < childrenOfNodeToRemoveReverse.size(); ++i) {
                        heapOfchildrenOfNodesToRemove.push_back(childrenOfNodeToRemoveReverse[i]);
                    }

                    auto h1 = BinomialHeapUnion(mainHeap, heapOfchildrenOfNodesToRemove);
                    mainHeap = h1;

                    /*hwndEditExtractMin;
                    auto minimumStr = std::to_string(minimum);
                    std::wstring stemp = std::wstring(minimumStr.begin(), minimumStr.end());
                    LPCWSTR sw = stemp.c_str();
                    SetWindowText(hwndEditExtractMin, sw);*/
                    RECT rect2;
                    GetClientRect(hWnd, &rect2);
                    InvalidateRect(hWnd, &rect2, TRUE);
                
            }
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_SIZE:
    {
        //RECT rt;
        //GetWindowRect(hwndButtonInsertNode, &rt);
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        // Now convert those with regards to the control's parent
        /*ScreenToClient(GetParent(hwndButtonInsertNode), (LPPOINT) & ((LPPOINT)&rt)[0]);
        ScreenToClient(GetParent(hwndButtonInsertNode), (LPPOINT) & ((LPPOINT)&rt)[1]);*/
        //MoveWindow(hwndButtonInsertNode, rc2.right-rc2.left, rc2.top, 100, 25, 1);

        SetWindowPos(hwndButtonInsertNode, HWND_TOP, width - ButtonWidth- editTextWidth, 0, ButtonWidth, 25, NULL );
        SetWindowPos(hwndEditInsertNode, HWND_TOP, width - editTextWidth, 0, editTextWidth, 25, NULL);
        //hwndEditExtractMin
        SetWindowPos(hwndButtonExtractMin, HWND_TOP, width - ButtonWidth - editTextWidth, 25, ButtonWidth, 25, NULL);
        SetWindowPos(hwndEditExtractMin, HWND_TOP, width - editTextWidth, 25, editTextWidth, 25, NULL);

        SetWindowPos(hwndButtonDecreaseKey, HWND_TOP, width - ButtonWidth- editTextWidth, 50, ButtonWidth, 25, NULL);
        SetWindowPos(hwndEditDecreaseKey, HWND_TOP, width - editTextWidth, 50, editTextWidth, 25, NULL);

        SetWindowPos(hwndButtonDeleteNode, HWND_TOP, width - ButtonWidth, 75, ButtonWidth, 25, NULL);
        /*RECT rect;
        GetClientRect(hWnd, &rect);
        InvalidateRect(hWnd, &rect, TRUE);*/
    }
    break;

    case WM_CREATE:
    {
        //HWND EditExample = CreateWindowEx("EDIT", "Text Goes Here", WS_VISIBLE | WS_CHILD | ES_LEFT, 10, 10, 100, 100, hWnd, NULL, hInst, NULL);
        /*SetScrollRange(hWnd, SB_VERT, 0, 50, FALSE);
        SetScrollPos(hWnd, SB_VERT, VscrollPos, TRUE);*/
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 99); // distribution in range [1, 6]
        for (int i = 0; i < tree_height; ++i) {
            vec_rnd_num.push_back(dist6(rng));
        }

        leftMargin = 10;
        topMargin = 10;
        circleWidth = 50;
        circleHeight = 50;

        hFont = CreateFont((int)((double)50 * 0.75), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);

        std::ifstream myfile("numbers.txt");
        if (myfile.is_open()) {
            std::string line{ "" };
            while (std::getline(myfile, line)) {

                if (line.size() > 0) {
                    int i = 0;
                    int liczba = 0;
                    for (; i < line.size(); ) {
                        char a = line.at(i);
                        if (a == ' ') {
                            if (liczba != 0) {
                                vec_Numbers.push_back(liczba);
                            }
                            liczba = 0;
                            ++i;
                        }
                        else if (a == '\n') {
                            if (liczba != 0) {
                                vec_Numbers.push_back(liczba);
                            }
                            break;
                        }
                        else {
                            int int_a = a - '0';
                            liczba = liczba * 10 + int_a;
                            ++i;
                        }
                    }
                    if (liczba != 0) {
                        vec_Numbers.push_back(liczba);
                    }
                    liczba = 0;
                }
            }
            myfile.close();
        }
        else {
            MessageBox(hWnd, L"Could not find a text file", L"Error", MB_ICONERROR);
        }
        //std::list<Node*>* emptyHeap= new std::list<Node*>();
        
        for (int i = 0; i < vec_Numbers.size(); ++i) {
            mainHeap = BinomialHeapInsert(mainHeap, vec_Numbers[i]);
        }
        // add button here
        RECT rect;
        GetClientRect(hWnd, &rect);


        //IDC_BUTTON_INSERT_NODE
        //    IDC_BUTTON_EXTRACT_MIN
        //    IDC_BUTTON_DECREASE_KEY
        //    IDC_BUTTON_DELETE_NODE
        hwndButtonInsertNode = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"INSERT NODE",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - ButtonWidth- editTextWidth,         // x position 
            rect.top,         // y position 
            ButtonWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_BUTTON_INSERT_NODE,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.
        hwndEditInsertNode = CreateWindow(
            L"EDIT",  // Predefined class; Unicode assumed 
            /*L"INSERT NODE"*/0,      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - editTextWidth,         // x position 
            rect.top,         // y position 
            editTextWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_EDIT_INSERT_NODE,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        hwndButtonExtractMin = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"EXTRACT MIN",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - ButtonWidth - editTextWidth,         // x position 
            rect.top+25,         // y position 
            ButtonWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_BUTTON_EXTRACT_MIN,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);

        hwndEditExtractMin = CreateWindow(
            L"EDIT",  // Predefined class; Unicode assumed 
            /*L"INSERT NODE"*/0,      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - editTextWidth,         // x position 
            rect.top+25,         // y position 
            editTextWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_EDIT_EXTRACT_MIN,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndEditExtractMin, EM_SETREADONLY, TRUE, NULL);

        hwndButtonDecreaseKey = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"DECREASE KEY",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - ButtonWidth- editTextWidth,         // x position 
            rect.top + 50,         // y position 
            ButtonWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_BUTTON_DECREASE_KEY,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);

        hwndEditDecreaseKey = CreateWindow(
            L"EDIT",  // Predefined class; Unicode assumed 
            /*L"INSERT NODE"*/0,      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - editTextWidth,         // x position 
            rect.top+50,         // y position 
            editTextWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_EDIT_DECREASE_KEY,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        hwndButtonDeleteNode = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"DELETE NODE",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            rect.right - ButtonWidth,         // x position 
            rect.top + 75,         // y position 
            ButtonWidth,        // Button width
            25,        // Button height
            hWnd,     // Parent window
            (HMENU)IDC_BUTTON_DELETE_NODE,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);



        GetClientRect(hwndButtonInsertNode, &rc2);
        MapWindowPoints(hwndButtonInsertNode, GetParent(hwndButtonInsertNode), (LPPOINT)&rc2, 2);
        previousRect = rc2;

            }
    break;
    case WM_LBUTTONDOWN:

        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);

        for (int i = 0; i < circleNodesCoordinates.size(); ++i) {
            auto leftCoordinate = std::get<0>(circleNodesCoordinates[i].second);
            auto rightCoordinate = std::get<1>(circleNodesCoordinates[i].second);
            auto toptCoordinate = std::get<2>(circleNodesCoordinates[i].second);
            auto bottomCoordinate = std::get<3>(circleNodesCoordinates[i].second);

            if (pt.x > leftCoordinate && pt.x < rightCoordinate && pt.y>toptCoordinate && pt.y < bottomCoordinate) {
                if (selectedNode == circleNodesCoordinates[i].first) {
                    isNodeSelected = !isNodeSelected;
                }
                else {
                    isNodeSelected = false;
                    selectedNode = circleNodesCoordinates[i].first;
                    
                }
                RECT rect;
                GetClientRect(hWnd, &rect);
                InvalidateRect(hWnd, &rect, TRUE);
                break;
            }

        }
        //POINT rectTL;
        //rectTL.x = rect.left;
        //rectTL.y = rect.top;
        //ScreenToClient(hWnd, &pt);
        break;
    case WM_PAINT:
    {
        
        //HDC hdc = BeginPaint(hWnd, &ps);

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        SetTextColor(hdc, RGB(0, 255, 0));      /* set text colour to green */
        SetBkColor(hdc, RGB(255, 255, 255));          /* set background colour to black */

        HPEN hPen = CreatePen(PS_SOLID, 1, NULL);
        SelectObject(hdc, hPen);
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        if (createFont) {
            
            createFont = false;
        }
        hFont = CreateFont((int)((double)50 * 0.75), 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        SelectObject(hdc, hFont);
        
        topMargin = topMarginAdjust;

        if (mainHeap.empty()) {

            break;
        }

        auto mainHeapIterator = mainHeap.begin();
        Node* mainHeapRoot = *mainHeapIterator;
        std::vector<Node*> vecAtLevelNodeI{};
        std::vector<std::pair<int, int>> vecAtLevelNodeIParentRect{};
        //vecAtLevelNodeIParentRect.push_back(std::make_pair());
        vecAtLevelNodeI.push_back(mainHeapRoot);

        int level = 0;
        circleNodesCoordinates.clear();

        for (int i = 0; ; ++i) {
            

            

            for (int j = 0; j < vecAtLevelNodeI.size(); ++j) {
                SetDCBrushColor(hdc, RGB(255, 0, 0));
                RECT  rect;
                HPEN hPen2;
                auto num = vecAtLevelNodeI[j]->key;
                if (num == selectedNode) {
                    if (!isNodeSelected) {
                        hPen2 = CreatePen(PS_SOLID, 4, RGB(0, 255, 0));
                        SelectObject(hdc, hPen2);

                        SetDCPenColor(hdc, RGB(0, 255, 0));
                        
                        selectedNodeActual = selectedNode;
                        nodeSelectedType = vecAtLevelNodeI[j];
                    }
                    else {
                        hPen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                        SelectObject(hdc, hPen2);

                        SetDCPenColor(hdc, RGB(0, 0, 0));
                        
                        selectedNodeActual = -1;
                        nodeSelectedType = NULL;
                    }
                    
                }
                else {
                    hPen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(hdc, hPen2);

                    SetDCPenColor(hdc, RGB(0, 0, 0));
                    //selectedNodeActual = -1;
                }
                Ellipse(hdc, leftMargin, topMargin, leftMargin + circleWidth, topMargin + circleHeight);

                DeleteObject(hPen2);
                //GetClientRect(hWnd, &rect);
                SetTextColor(hdc, RGB(0x00, 0xFF, 0x00));
                SetBkMode(hdc, TRANSPARENT);
                //SetBkMode(hdc, TRANSPARENT);
                rect.left = leftMargin;
                rect.top = topMargin;
                rect.right = leftMargin + circleWidth;
                rect.bottom = topMargin + circleHeight;
                std::string str = std::to_string(num);
                std::wstring stemp = std::wstring(str.begin(), str.end());
                LPCWSTR sw = stemp.c_str();

                DrawText(hdc, sw, -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);

                circleNodesCoordinates.push_back(std::make_pair(num, std::make_tuple(rect.left, rect.right, rect.top, rect.bottom)));

            
                

                if (level > 0) {
                    int topM = vecAtLevelNodeIParentRect[j].first;
                    int leftM = vecAtLevelNodeIParentRect[j].second;
                    HPEN hPen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(hdc, hPen2);

                    SetDCPenColor(hdc, RGB(0, 0, 0));
                    MoveToEx(hdc, (rect.left+rect.right)/2, rect.top, NULL);
                    LineTo(hdc, leftM+25, topM+50);

                    DeleteObject(hPen2);
                }

                leftMargin += circleWidth + 10;
            }

            leftMargin = 10;

            topMargin += circleWidth + 20;


            
            std::vector<Node*> nodesAtLevelN; 
            vecAtLevelNodeIParentRect.clear();
            bool cond = true;
            int leftM = leftMargin - circleWidth -10;
            int topM = topMargin - circleHeight - 20;
            for (auto node : vecAtLevelNodeI) {
                topM = topM;
                leftM = leftM + circleWidth + 10;
                Node* childNode = node->leftChild;
                if (childNode == NULL) {
                    continue;
                }
                else {
                    while (cond) {
                        nodesAtLevelN.push_back(childNode);
                        vecAtLevelNodeIParentRect.push_back(std::make_pair(topM, leftM));
                        childNode = childNode->rightSibling;
                        if (childNode == NULL) {
                            break;
                        }
                    }
                }
                
            }

            vecAtLevelNodeI = nodesAtLevelN;
            if (!vecAtLevelNodeI.empty()) {
                level++;
            }
            else {
                mainHeapIterator++;
                if (mainHeapIterator == mainHeap.end()) {
                    break;
                }
                level = 0;
                leftMargin = 10;

                topMargin += circleWidth + 20;

                Node* mainHeapRoot = *mainHeapIterator;
                vecAtLevelNodeI.push_back(mainHeapRoot);
            }
        }
            DeleteObject(hPen);
            DeleteObject(hFont);

            EndPaint(hWnd, &ps);
            ReleaseDC(hWnd, hdc);

            
        }
        break;
    case WM_VSCROLL:
    {
        switch (LOWORD(wParam))
        {
        case SB_LINEUP:
            VscrollPos -= 1;
            isScrollDown = false;
            break;
        case SB_LINEDOWN:
            VscrollPos += 1;
            isScrollDown = true;

            break;
        case SB_PAGEUP:
            //VscrollPos -= cyClient / cyChar;
            break;
        case SB_PAGEDOWN:
            //VscrollPos += cyClient / cyChar;
            break;
        case SB_THUMBPOSITION:
            VscrollPos = HIWORD(wParam);
            break;
        default:
            break;
        }
        if (VscrollPos != GetScrollPos(hWnd, SB_VERT)) {
            if (VscrollPos >= 0) {
                SetScrollPos(hWnd, SB_VERT, VscrollPos, TRUE);
                //InvalidateRect(hwnd, NULL, TRUE);
                if (isScrollDown) {
                    topMarginAdjust -= circleWidth + 10;
                }
                else {
                    topMarginAdjust += circleWidth + 10;
                }
                RECT rect;
                GetClientRect(hWnd, &rect);
                InvalidateRect(hWnd, &rect, TRUE);
                MapWindowPoints(hWnd, hWnd, (POINT*)&rect, 2);
                RedrawWindow(hWnd, &rect, NULL, RDW_ERASE | RDW_INVALIDATE);
            }
            else {
                VscrollPos = 0;
            }
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        //delete mainHeap;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
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

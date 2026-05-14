#include <windows.h>
#include <cmath>

#define ROWS 60
#define COLS 60

struct Point {
    float x, y;
};

Point grid[ROWS][COLS];
bool mask[ROWS][COLS];

float t = 0.0f;

void generateFace() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            float x = -1.0f + 2.0f * j / (COLS - 1);
            float y = -1.0f + 2.0f * i / (ROWS - 1);


            float jaw = 1.0f - powf(fabs(y), 2.2f) * 0.45f;
            x /= jaw;

            x = -x;

            grid[i][j] = { x, y };

            bool head = ((x * x) / 0.6f + (y * y) <= 1.0f);

  
            bool leftEye =
                ((x + 0.35f)*(x + 0.35f) + (y + 0.15f)*(y + 0.15f) < 0.04f);
            bool rightEye =
                ((x - 0.35f)*(x - 0.35f) + (y + 0.15f)*(y + 0.15f) < 0.04f);

           
            bool noseBridge =
                fabs(x) < 0.05f &&
                y > -0.10f && y < 0.25f;

            bool noseTip =
                (x*x + (y + 0.12f)*(y + 0.12f)) < 0.015f;

           
            float mouthY = -0.15f;
            float mouthCurve = 0.010f * (1.0f - (x * x) / 0.15f);

            bool mouthHole =
                fabs(y - (mouthY + mouthCurve)) < 0.025f &&
                fabs(x) < 0.80f;

           
            mask[i][j] =
                head &&
                !leftEye &&
                !rightEye &&
                !mouthHole;
        }
    }
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        HBRUSH bg = CreateSolidBrush(RGB(0, 3, 0));
        FillRect(hdc, &ps.rcPaint, bg);
        DeleteObject(bg);

      
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        SelectObject(hdc, pen);

    
        for (int i = 0; i < ROWS; i++) {
            for (int j = 1; j < COLS; j++) {

                if (!mask[i][j] || !mask[i][j - 1])
                    continue;

                float wave1 = sin(t + grid[i][j - 1].y * 8) * 12;
                float wave2 = sin(t + grid[i][j].y * 8) * 12;

                int x1 = 400 + grid[i][j - 1].x * 360 + wave1;
                int y1 = 400 + grid[i][j - 1].y * 300;

                int x2 = 400 + grid[i][j].x * 360 + wave2;
                int y2 = 400 + grid[i][j].y * 300;

                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);
            }
        }

        DeleteObject(pen);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {

    generateFace();

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "XRayFace";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        "XRayFace",
        "X-Ray Human Face (Flipped)",
        WS_OVERLAPPEDWINDOW,
        200, 100, 900, 900,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (true) {
        t += 0.04f;
        InvalidateRect(hwnd, NULL, TRUE);

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Sleep(16);
    }

    return 0;
}

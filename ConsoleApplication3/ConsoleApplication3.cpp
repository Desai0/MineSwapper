#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include "../WinAPI_OOP/wapi_oop.h"
#include "../WinAPI_OOP/wapi_interactive.h"
#include "../WinAPI_OOP/wapi_control.h"
#include "../WinAPI_OOP/wapi_nontopcontrol.h"
#include "../WinAPI_OOP/wapi_paintbox.h"
#include "../WinAPI_OOP/wapi_window.h"

const int GRID_SIZE = 10;
const int NUM_MINES = 11;

class MainWindow : public Window {
    std::unique_ptr<Button> button[GRID_SIZE][GRID_SIZE];
    short int matrica[GRID_SIZE][GRID_SIZE];
    bool revealed[GRID_SIZE][GRID_SIZE];
    bool game_over = false;
    bool first_click = true;
    int revealed_count = 0;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib;

public:
    MainWindow(HINSTANCE hInstance) :
        Window(hInstance),
        gen(rd()),
        distrib(0, GRID_SIZE - 1)
    {
        InitComponent(nullptr);

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                button[i][j] = std::make_unique<Button>(hInstance, i * GRID_SIZE + j, this);
                button[i][j]->SetPosition(j * 50, i * 50);
                button[i][j]->SetSize(50, 50);
                button[i][j]->SetText(L" ");
                matrica[i][j] = 0;
                revealed[i][j] = false;
            }
        }

        SetText(L"Minesweeper");
        SetSize(GRID_SIZE * 50 + 16, GRID_SIZE * 50 + 40);
    }

    void SetupBoard(int initial_r, int initial_c) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                matrica[i][j] = 0;
                revealed[i][j] = false;
            }
        }
        game_over = false;
        revealed_count = 0;
        first_click = false;

        int mines_placed = 0;
        while (mines_placed < NUM_MINES) {
            int r = distrib(gen);
            int c = distrib(gen);
            if (matrica[r][c] != -1 && !(r == initial_r && c == initial_c)) {
                matrica[r][c] = -1;
                mines_placed++;
            }
        }

        for (int r = 0; r < GRID_SIZE; ++r) {
            for (int c = 0; c < GRID_SIZE; ++c) {
                if (matrica[r][c] == -1) continue;
                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) continue;
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < GRID_SIZE && nc >= 0 && nc < GRID_SIZE) {
                            if (matrica[nr][nc] == -1) count++;
                        }
                    }
                }
                matrica[r][c] = count;
            }
        }
    }

    void RevealCell(int r, int c) {
        // Проверки границ и уже открытых клеток
        if (r < 0 || r >= GRID_SIZE || c < 0 || c >= GRID_SIZE || revealed[r][c]) {
            return;
        }

        revealed[r][c] = true;
        button[r][c]->SetEnabled(false);
        revealed_count++;

        short value = matrica[r][c];
        if (value > 0) {
            button[r][c]->SetText(std::to_wstring(value));
        }
        else {
            button[r][c]->SetText(L" ");
        }

        if (value == 0) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    RevealCell(r + dr, c + dc);
                }
            }
        }
    }

    void CheckWinCondition() {
        if (game_over) return;

        int total_cells = GRID_SIZE * GRID_SIZE;
        int safe_cells = total_cells - NUM_MINES;

        if (revealed_count == safe_cells) {
            game_over = true;
            MessageBox(hwnd, L"Поздравляем, вы победили!", L"Победа!", MB_OK | MB_ICONINFORMATION);

            for (int i = 0; i < GRID_SIZE; ++i) {
                for (int j = 0; j < GRID_SIZE; ++j) {
                    if (!revealed[i][j]) {
                        button[i][j]->SetEnabled(false);
                        button[i][j]->SetText(L"🚩"); //Можно так иконки ставить, рофлан
                    }
                }
            }
        }
    }

    void GameOver() {
        if (game_over) return;

        game_over = true;
        MessageBox(hwnd, L"Вы проиграли!", L"Игра окончена", MB_OK | MB_ICONSTOP);

        // Показ мин (как и раньше)
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (!revealed[i][j]) {
                    button[i][j]->SetEnabled(false);
                    if (matrica[i][j] == -1) {
                        button[i][j]->SetText(L"💣");
                    }
                }
                else if (matrica[i][j] == -1) {
                    button[i][j]->SetEnabled(false);
                    button[i][j]->SetText(L"💥");
                }
            }
        }
    }

    LRESULT OnCommand(WPARAM wp, LPARAM lp) override {
        WORD id = LOWORD(wp);
        WORD state = HIWORD(wp);

        if (state == BN_CLICKED && !game_over) {
            int r = id / GRID_SIZE;
            int c = id % GRID_SIZE;

            if (r < 0 || r >= GRID_SIZE || c < 0 || c >= GRID_SIZE) return 0;

            if (first_click) {
                SetupBoard(r, c);
            }

            if (revealed[r][c]) {
                return 0;
            }

            if (matrica[r][c] == -1) {
                revealed[r][c] = true;
                GameOver();
            }
            else {
                RevealCell(r, c);
                CheckWinCondition();
            }
        }
        return 0;
    }

    LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp) override {
        FillRect(hdc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
        return 0;
    }

protected:
    DECLARE_CLASS_NAME(MainWindow);
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR cmdline, int nCmdShow) {
    try {
        RegisterControl(MainWindow, hInstance);
        MainWindow mw(hInstance);
        mw.Show();
        mw.ProcessMessages();
    }
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Unhandled Exception", MB_OK | MB_ICONERROR);
        return 1;
    }
    catch (...) {
        MessageBoxA(NULL, "An unknown error occurred.", "Unhandled Exception", MB_OK | MB_ICONERROR);
        return 1;
    }
    return 0;
}
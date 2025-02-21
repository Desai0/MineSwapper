#include <iostream>
#include <wapi_oop.h>
#include <random>

//size_t check(size_t x, size_t y) {
//	size_t 
//}

class MainWindow : public Window {
	std::unique_ptr<Button> button[10][10];
	short int matrica[10][10];
	size_t game_started = 0;
	short int tapped;

public:
	MainWindow(HINSTANCE hInstance) : Window(hInstance) {
		InitComponent(nullptr);

		for (size_t i = 0; i < 10; ++i) {
			for (size_t j = 0; j < 10; ++j) {
				button[i][j] = std::make_unique<Button>(hInstance, 10 * i + j, this);
				button[i][j]->SetPosition(10 * j * 5, 10 * i * 5);
				button[i][j]->SetSize(50, 50);
				button[i][j]->SetText(L" ");
			}
		}


		for (size_t i = 0; i < 10; ++i) {
			for (size_t j = 0; j < 10; ++j) {
				matrica[i][j] = 0;
			}
		}

		
				

		/*b = std::make_unique<Button>(hInstance, 10, this);
		b->SetPosition(10, 10);
		b->SetSize(50, 50);
		b->SetText(L"Тыкай");*/


		SetText(L"Мое окно");
		SetSize(516, 540);

	}

	LRESULT OnCommand(WPARAM wp, LPARAM lp) override {
		WORD id = LOWORD(wp), state = HIWORD(wp);

		if (id == button[id / 10][id % 10]->GetId() && state == BN_CLICKED) {

			if (game_started == 0) {
				game_started = 1;
				for (size_t i = 0; i < 10; ++i) {
					while (true) {
						std::random_device dev;
						std::mt19937 rnd(dev());
						std::uniform_int_distribution<size_t> dist(0, 9);
						size_t random = dist(rnd);
						size_t random2 = dist(rnd);
						if (matrica[random][random2] == 0) {
							matrica[random][random2] = -1;

							for (short int i = -1; i < 2; ++i) {
								for (short int j = -1; j < 2; ++j) {
									if (!(random == 0 || random == 9 || random2 == 0 || random2 == 9)) {
										if (matrica[random + i][random2 + j] != -1) {
											matrica[random + i][random2 + j] += 1;
										}
									}
									else {
										continue;
									}
									
								}
							}
							
							break;
						}
						else {
							continue;
						}
					}
				}
				tapped = matrica[id / 10][id % 10];
				button[id / 10][id % 10]->SetText(std::to_wstring(tapped));
			} ///////////////
			else if (id == button[id / 10][id % 10]->GetId() && state == BN_CLICKED) {
				tapped = matrica[id / 10][id % 10];
				button[id / 10][id % 10]->SetText(std::to_wstring(tapped));
			}
			/*MessageBoxW(hwnd, L"Вы нажали кнопку!", L"Оповещение", MB_OK);*/
			
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
	RegisterControl(MainWindow, hInstance);

	MainWindow mw(hInstance);
	mw.Show();

	mw.ProcessMessages();

	return 0;
}
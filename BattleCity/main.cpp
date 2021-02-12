#include <iostream>
#include <string>
#include "Framework.h"
#include "Game.h"
#include "Enemy.h"

int WIDTH = 320, HEIGHT = 240;

class MyFramework : public Framework {
private:
	CGame &game = CGame::Get();
	int ticks_o, ticks_n;
	double accumulator;
	double max_time;
	int fps_count;
	int fps_max;
	double fps_time;
	double delta_time;
	bool cursor;
public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = WIDTH;
		height = HEIGHT;
		fullscreen = false;

	}

	virtual bool Init() {
		game.Init();
		max_time = 1.0;
		accumulator = 0.0;
		cursor = true;
		return true;
	}

	virtual void Close() {
	}
	virtual bool Tick() {
		showCursor(cursor);
		
		ticks_n = getTickCount();
		delta_time = double((ticks_n - ticks_o)) / 1000.0;
		ticks_o = ticks_n;
		if (delta_time < 0) delta_time = 0;

		accumulator += delta_time;
		if (accumulator < 0)                 accumulator = 0;
		else if (accumulator > max_time)     accumulator = max_time;


		// счетчик FPS
		fps_time += delta_time;
		++fps_count;
		if (fps_time >= 1.0) {
			int fps_num = fps_count / double(fps_time);
			std::cout << "FPS: " << fps_num << "\n";
			fps_count = 0;
			fps_time = 0;
			
		}
		if (accumulator > 1.0/60) {
			game.Update();
			accumulator -= 1.0/60;
		}
		game.Draw();
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::LEFT) {
			if (isReleased)
				game.PlayerFire();
		}
		else if (button == FRMouseButton::RIGHT) {
			if (isReleased)
				cursor=!cursor;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		game.PlayerMove(k, false);
	}

	virtual void onKeyReleased(FRKey k) {
		game.PlayerMove(k, true);
	}

	virtual const char* GetTitle() override {
		return "Tanks";
	}
};

int main(int argc, char* argv[]) {
	int i = 0;
	if (argc > 0) {
		int i = 1;
		while (i < argc) {
			if ((std::string)argv[i++] == "-window") {
				std::string w, h;
				bool findX = false;
				for (int j = 0; j < sizeof(argv[i]); j++) {
					if (argv[i][j] == 'x') {
						findX = true;
						j++;
					}
					!findX ?
						w += argv[i][j] :
						h += argv[i][j];
				}
				WIDTH = std::stoi(w);
				HEIGHT = std::stoi(h);
			}
		}
	}
	return run(new MyFramework);
}
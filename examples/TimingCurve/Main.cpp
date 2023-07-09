#define OLC_PGE_APPLICATION
#include "PixelGameEngine.hpp"
#include "Animatable.hpp"

#include <functional>

class Demo : public olc::PixelGameEngine {
protected:

	std::vector<olc::vf2d> points;

	virtual bool OnUserCreate() override {
		

		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLACK);

		float w = (float)ScreenWidth();
		float h = (float)ScreenHeight();

		if (GetMouse(olc::Mouse::LEFT).bPressed) {
			points.clear();

			x = 0;
			x.WithAnimation(
				ui::TimingCurve(
					3.0f,
					std::vector{ 0.0f, 0.25f, 0.5f, 0.75f, 0.5f },
					std::vector{ &ui::ease::InOutBounce, &ui::ease::InOutBack, &ui::ease::Linear }
				),
				1.0f
			);
		}

		if (x.IsAnimating()) {
			x.Update(fElapsedTime);
			points.emplace_back(x * w, points.size());
		}
		
		for (auto x : { 0.25f, 0.5f, 0.75f }) {
			DrawLineDecal({ x * w, 0 }, { x * w, h }, olc::RED);
		}

		for (auto i = 1; i < points.size(); ++i) {
			auto prev = points[i - 1];
			auto curr = points[i];

			DrawLineDecal(prev, curr, olc::CYAN);
		}

		return true;
	}

	ui::Animatable<float> x = 0.0f;

};

int main()
{
	Demo demo;

	if (demo.Construct(1280, 720, 1, 1)) {
		demo.Start();
	}
	
	return 0;
}
#include <cstdint>
#define SAMPLE_RATE 44100

class Z3583 {
	uint8_t vol_lut[16] = {0, 16, 32, 48, 64, 76, 84, 96, 108, 127, 144, 176, 204, 232, 248, 255};
	enum Waveform {
		W_RECT = 0,
		W_SAW,
		W_NOISE,
		W_RSAW,
	};
	public:
		struct {
			uint32_t phase;
			uint32_t frequency;
			int16_t out;
			uint8_t type;
			uint8_t volume;
			uint32_t lfsr = 0xEC48;
			bool muted;
		} channel[4];
		void clockLFSR();
		void getSample(short* l, short* r);
		void reset();
		Z3583();
};
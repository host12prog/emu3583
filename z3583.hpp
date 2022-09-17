#ifndef Z3583_H
#define Z3583_H

#include <cstdint>

class Z3583 {
	// FIXME: make this enum better (i don't have the patience today)
	// OPNA2608: "it's easier to tell that you're expecting the enum, and not a random number"
	enum {
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

#endif
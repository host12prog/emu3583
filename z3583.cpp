// license: GPLv2-or-later
// Z3583 fantasy sound chip emulation core: Aleksi Knutsi

#include "z3583.hpp"
#define SAMPLE_RATE 44100

uint8_t vol_lut[16] = {0, 16, 32, 48, 64, 76, 84, 96, 108, 127, 144, 176, 204, 232, 248, 255};

void Z3583::clockLFSR() {
	for (int j = 0; j < 3; j++) {
		channel[j].lfsr = (channel[j].lfsr >> 1 | (((channel[j].lfsr) ^ (channel[j].lfsr >> 2) ^ (channel[j].lfsr >> 3) ^ (channel[j].lfsr >> 5) ) & 1) << 31);
		if ((channel[j].lfsr & 63) == 0)
			channel[j].lfsr = 0xEC48; // initial state of LFSR at power on
	}
}

void Z3583::getSample(short* l, short* r) {
	int16_t mix = 0;

	for (int j = 0; j < 3; j++) {
		if (channel[j].frequency == 0 || channel[j].muted) {
			channel[j].out = 0;
		} else {
			switch (channel[j].type) {
				case W_RECT: 
					// square
					channel[j].out = (channel[j].phase > (1 << 21)) ? 8192 : -8192;
					break;
				case W_SAW: 
					// saw
					channel[j].out = (channel[j].phase > >9) * 2 - 8192;
					break;
				case W_NOISE: 
					// noise
					// yeah... this was the only solution to noise being too loud
					channel[j].out = channel[j].lfsr;
					channel[j].out = channel[j].out / 2; 
					break;
				case W_RSAW: 
					// reverse saw
					channel[j].out = -(channel[j].phase >> 9) * 2 + 8192;
					break;
			}
		}
		channel[j].out = channel[j].out * vol_lut[channel[j].volume & 15] / 255;
		channel[j].phase += channel[j].frequency;
		channel[j].phase &= (1 << 22) - 1; // phase accumulator is only 22 bits long
		
		// allow for dynamic noise pitches
		if (channel[j].phase <= channel[j].frequency && channel[j].type == W_NOISE)
			clockLFSR();
		
		mix += channel[j].out;
	}
	*l = mix;
	*r = mix;
}


void Z3583::reset() {
	for (int j = 0; j < 3; j++) {
		channel[j].volume = 15;
		channel[j].phase = 0;
		channel[j].lfsr = 0xEC48;
		channel[j].muted = false;
	}
}

Z3583::Z3583() {
	reset();
}
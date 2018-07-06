#include <iostream>
#include "olcNoiseMaker.h"

using namespace std;


// sound is vibrations in the air
// sound waves describe these vibrations


atomic<double> frequencyOutput = 0.0;

// dTime = time that has passed since beginning of program
double makeNoise(double dTime)
{
	double amplitude = 0.05;
	// double frequency = 440; // Hz, an A4 on a keyboard, reason for this
	// multiply by 2 * 3.14159 to convert from frequency to angular frequency
	double output = amplitude * sin(frequencyOutput * 2 * PI * dTime);

	// square wave
	// sine wave made by just returning output
	if (output > 0.0)
	{
		return amplitude;
	}
	return -amplitude;
}

int main()
{
	wcout << "onelonecoder.com - Synthesizer Part 1" << endl;

	// Get all sound hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	// Display findings
	for (auto d : devices) wcout << "FoundOutput Device:" << d << endl;

	// Create sound machine
	wstring outputDevice = devices[0];
	// should be double highest frequency we want to record
	// human range is 20 Hz to 20k Hz
	// 16 bit at 44k is a common setup for sound synthesis
	unsigned int sampleRate = 44100;
	unsigned int channels = 1; // stereo system or not
	unsigned int blocks = 8; // latency management
	unsigned int blockSamples = 512; // latency management
	// short makes for a 16 bit system
	olcNoiseMaker<short> sound(outputDevice, sampleRate, channels, blocks, blockSamples);

	// Link noise function with sound machine
	sound.SetUserFunction(makeNoise);

	// doubling frequency raises octave
	double octaveBaseFrequency = 110.0; // A2
	// conventional scale = 12 notes per octave
	// can't just divide by twelve because of doubling -> use power
	double twelvethRootOfTwo = pow(2.0, 1.0 / 12.0);

	while (1)
	{
		bool keyPressed = false;
		for (int k = 0; k < 15; k++)
		{
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe"[k])) & 0x8000)
			{
				frequencyOutput = octaveBaseFrequency * pow(twelvethRootOfTwo, k);
				keyPressed = true;
			}
		}

		if (!keyPressed)
		{
			frequencyOutput = 0.0;
		}
	}

	return 0;
}
#pragma once

#include <Windows.h>
#include <stdio.h>

class BMPBasics
{
public:
	BMPBasics(const char* readPath1, const char *readPath2,
		const char* writePath1, const char *writePath2, const char *writePath3);
	~BMPBasics();

	void ReverseBMP();
	void OverlayBMP();
	void MeanFilterBMP();
	void ProcessBMP();

private:
	const char* readPath1;
	const char* readPath2;
	const char* writePath1;
	const char* writePath2;
	const char* writePath3;
};


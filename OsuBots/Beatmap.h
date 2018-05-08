#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//#include <filesystem>
#include <stdlib.h> 

#include "Functions.h"
#include "beatmapRelatedStructs.h"
#include "HitObject.h"
#include "TimingPoint.h"

using namespace std;
//namespace fs = std::experimental::filesystem; // for C++ 14


// ------------------------------------------------Beatmap class---------------------------------------------------
class Beatmap
{
public:
	// constructor & destructor
	Beatmap(string fullPathAfterSongsFolder);

	// variables
	const static string FOLDERPATH; 
	//string fileName; // format: "xxxxxxxx.osu"   *remember .osu !!!
	string fullPathBeatmapFileName;
	bool allSet; // for checking if beatmap is successfully set
	GeneralS General;
	DifficultyS Difficulty;
	vector<TimingPoint> TimingPoints;
	vector<HitObject> HitObjects;

	// calculated variables
	float circleRadius;
	float timeRange50;
	float timeRange100;
	float timeRange300;

private:
	void processBeatmap();
	//void setFullPathBeatmapFileName(); // deprecated
};
// -------------------------------------------------------End of Beatmap class----------------------------------------


// TODO: support mods (DT might change the timeRanges and so on)
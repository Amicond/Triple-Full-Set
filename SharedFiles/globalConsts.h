//global consts identical to the whole pipeline
#pragma once
const int RouteTypeAmount = 3;
const int MaxOrder = 5;



const std::string TypeStr[RouteTypeAmount] = { "0","1","2" };
const int minSubOrder[RouteTypeAmount] = { 1,3,1 };


//Section: paths and so on
//delimeter
const std::string Delimiter = "\\";


//directories for result files from different stages
const std::string ProjectFolder="d:\\Andrew\\Practice\\!!!_4 Triplet Square set";
const std::string ResultsFolder = ProjectFolder+ Delimiter +"Results"+ Delimiter;
const std::string InputDataFolder = ProjectFolder + Delimiter + "InputData" + Delimiter;


//full paths
//initial input data dirs
const std::string ConfigFilePath = InputDataFolder + "config.txt";
const std::string DirForMatrixInput = InputDataFolder + "matrixes" + Delimiter;
const std::string MatrixVersionPrefix = "b3_";
const std::string DirForPerturbationSeries = InputDataFolder + "perturbationTerms" + Delimiter;
//paths for results dirs
const std::string DirForResultsFromStage0 = ResultsFolder + "Stage0" + Delimiter;
const std::string DirForResultsFromStage1  = ResultsFolder + "Stage1" + Delimiter;
const std::string DirForResultsFromStage11 = ResultsFolder + "Stage1.1" + Delimiter;
const std::string DirForResultsFromStage2 = ResultsFolder + "Stage2" + Delimiter;
const std::string DirForResultsFromStage35 = ResultsFolder + "Stage3.5" + Delimiter;
const std::string DirForResultsFromStage4 = ResultsFolder + "Stage4" + Delimiter;
const std::string DirForResultsFromStage5 = ResultsFolder + "Stage5" + Delimiter;
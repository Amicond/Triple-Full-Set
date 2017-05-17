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
const std::string DirForResultsFromStage1 = "";
const std::string DirForResultsFromStage11 = "d:\\Andrew\\Practice\\!!!_4 Triplet Square set\\Results\\Stage1.1\\";
const std::string DirForResultsFromStage2 = "";
const std::string DirForResultsFromStage35 = "d:\\Andrew\\Practice\\!!!_4 Triplet Square set\\Results\\Stage3.5\\";
const std::string DirForResultsFromStage4 = "d:\\Andrew\\Practice\\!!!_4 Triplet Square set\\Results\\Stage4\\";
const std::string DirForResultsFromStage5 = "d:\\Andrew\\Practice\\!!!_4 Triplet Square set\\Results\\Stage5\\";
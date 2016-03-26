#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <map>

#define VERSION "V1.0"

#include "utils/math.hpp"

using namespace std;

enum State { IDLE, PROCESSING, FINISHED};

enum Command {GETSTAT = 0, GETAVG = 1,GETHEADER = 2, GETMAX = 3, GETMIN = 4, GETDAT = 5, ABOUT = 6, DISPFILE = 7, WHEN = 8,  DELTATIME = 9, QUIT = 10};

void printTitle () {
        cout <<"  _                          "
        		"\n | |   ___  __ _             "
        		"\n | |__/ _ \\/ _` |            "
        		"\n |____\\___/\\__, | _          "
        		"\n | _ \\___ _|___/_| |___ _ _  "
        		"\n |   / -_) _` / _` / -_) '_| "
        		"\n |_|_\\___\\__,_\\__,_\\___|_|   "
        		"\n                             \n\n\n";
        
        }

struct logStats {
	int iNumOfLines;
	int iNumOfElements;
	int iNumOfChars;
};

string sReadLine( ifstream& _ifFile, int _iLineNum) {
    
    //_ifFile.clear();
    _ifFile.seekg(0, _ifFile.beg);
    string sLineText_;
    for (int i = 0; i < _iLineNum; i++) {
        getline(_ifFile, sLineText_);
        if(!_ifFile.good()) {
        	return "ERROR";
#ifdef DEBUG
            cerr << "Invalid Line in File\n";
#endif
        }
    }
    _ifFile.clear();
    _ifFile.seekg(0, _ifFile.beg);
    return sLineText_;
    
}

vector <string > sGetCSVHeader(ifstream& _ifFile) {
    
    _ifFile.clear();
    _ifFile.seekg(0, _ifFile. beg);
    
    int iIndex = 0;
    unsigned int iCharNum = 0;
    
    char tmp;
    
    string sLine;
    string sOutLine;
    string sTempString;
    
    vector <string > sOutput_; 
    
    sLine = sReadLine(_ifFile, 1);
    do {
        
        tmp = sLine[iCharNum];
        if (tmp == ',' || tmp == '\n') {
            sOutput_.push_back(sOutLine);
            iIndex++;
            sOutLine = "";
        }
        else
        	sOutLine += tmp;
        iCharNum++;
    }
    while (iCharNum <= sLine.size());
    //_ifFile.clear();
    _ifFile.seekg(0, _ifFile.beg);
    return sOutput_;
}

string sGetElement(ifstream& _ifFile, int _iLineNum, int _iColNum) {
    
    //_ifFile.clear();
    _ifFile.seekg(0, _ifFile.beg);
    
    int iIndex = 1;
    
    char tmp;
    
    string sLine = sReadLine( _ifFile, _iLineNum);
    if (sLine == "ERROR")
    	return "ERROR";
    #ifdef DEBUG
    //cout << "String Contains " << sLine << "\n";
    #endif
    string sOutLine_;
    
    for (unsigned int i = 0; i <= sLine.size(); i++) {
        tmp = sLine[i];
        if ((tmp == ',' || tmp == '\n') && iIndex == _iColNum) {
        	return sOutLine_;
        } else if (tmp == ',') {
        	iIndex++;
        	sOutLine_ = "";
        } else {

        		sOutLine_ += tmp;
        	}

        }
    //_ifFile.clear();
    _ifFile.seekg(0, _ifFile.beg);
    return sOutLine_;
    
}

int iGetNumLines (ifstream& _ifFile) {

	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	int iNumLines = 0;
	int i = 1;
	string sLine;
	while (sLine != "ERROR") {
		sLine = sReadLine(_ifFile, i);
		i++;
		iNumLines++;
		if (sLine == "ERROR") {
			break;
		}
	}
#ifdef DEBUG
cout << iNumLines << " Lines\n";
#endif

	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	return iNumLines;
}

int iGetNumElements(ifstream& _ifFile, int _iNumLines) {
	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	int iNumElements;
	int x = 0;
	string sElement;
	string sLine; //Just for reasons

	for (int i = 1; i <= _iNumLines; i++) {
		while (_ifFile.good() && (sElement != "ERROR" || sLine != "ERROR")) {
			sLine = sReadLine(_ifFile, i);
			sElement = sGetElement(_ifFile, i, x);
			iNumElements++;
			x++;
#ifdef DEBUG
			cout << sElement << "\n";
#endif
#ifdef DEBUG
			cout << "Iteration x " << x << "\n";
#endif
		}
		if (sElement == "ERROR")
			break;
#ifdef DEBUG
			cout << "Iteration i " << i << "\n";
#endif
	}

#ifdef DEBUG
cout << iNumElements << " Elements\n";
#endif
	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	return iNumElements;
}

int iGetNumChars (ifstream& _ifFile, int _iNumLines, int _iNumElements) {
	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	int iNumChars;

	string sElement;
	for (int i = 1; i <= _iNumLines; i++) {
		for (int x = 1; x <= _iNumElements; x++) {
			sElement = sGetElement(_ifFile, i, x);
			if (sElement == "ERROR")
				break;
		iNumChars += sElement.size();
		}
		if (sElement == "ERROR")
			break;
	}

#ifdef DEBUG
cout << iNumChars << " Non Space Characters\n";
#endif
	_ifFile.clear();
	_ifFile.seekg(0, _ifFile.beg);
	return iNumChars;
}

logStats getLogStats (ifstream& _ifFile) {
	logStats logStats_;

	int iNumLines = iGetNumLines(_ifFile);
	int iNumElements = iGetNumElements(_ifFile, iNumLines);
	int iNumChars = iGetNumChars(_ifFile, iNumLines, iNumElements);
	logStats_.iNumOfLines = iNumLines;
	logStats_.iNumOfElements = iNumElements;
	logStats_.iNumOfChars = iNumChars;

	return logStats_;
}

void printLogStats (string _sFileName, logStats _logStats) {
	cout << "The File " << _sFileName << " Has the Following Statistics\n\n";

	cout << _logStats.iNumOfLines << " Lines\n\n";
	cout << _logStats.iNumOfElements << " Elements\n\n";
	cout << _logStats.iNumOfChars << " Characters";

}

map <string, int> vecToMap(vector <string> vsVec) {
	map <string, int> m;
	for (unsigned int i = 0; i < vsVec.size(); i++) {
		m[vsVec[i]] = i+1;
		//cout << i << " = " << vsVec[i] << "\n";
	}
	return m;

}

int main (int argc, char*argv[] ) {
    string sTestString = "--TEST";

    int iOption;
#ifdef TEST
    int iCurrentItr = -1;
#endif
    if (argc > 1 && argv[1] == sTestString)
    	cout << "Not Supported Yet\n";
    else if (argc > 1 && (string) argv[1] == "--help"){
        	cout << "Options:"
        			""
        			"\n\n1) Get Stats (1 or getstats)"
        			"\n2) Get Average (2 or getavg)"
					"\n3) Get CSV Header (3 or getheader)"
        			"\n4) Get Maximum (4 or getmax)"
					"\n5) Get Minimum (5 or getmin)"
					"\n6) Get Data Element (6 or getdat)"
					"\n7) About The Reader (7 or about)"
					"\n8) Display File (8 or dispfile)"
					"\n9) When Did... (9 or when)"
					"\n10) Get Time Stats (10 or deltatime)"
    				"\n11) Quit (11 or quit)\n";
        	exit(1);
    }
    int iTmp = 0;
    bool bFirstRun = true;
    bool bFileOpen = false;
    
    logStats logStats;
    string sLine;
    string sInput;
    string sFileName;
    string sContinue;
    string sFileChoice;
    string sTemp;
    string sData;
    
    Command command = GETSTAT;

    State state = IDLE;
    
    vector <string > vsColumn;
    vector <string > vsFiles;
    vector <string > vsHeader;
    vector <string > vsArgs;
    map <string, int> mHeader;
    map <string, int> mOptions = {
    {"getstat",0 },
    {"getavg",1 },
    {"getheader",2 },
    {"getmax",3 },
    {"getmin",4 },
    {"getdat",5 },
    {"about",6 },
    {"dispfile",7 },
    {"when",8 },
    {"deltatime",9 },
    {"quit",10 }
};
    for (int i = 0; i < argc; i++) {
    	vsArgs.push_back((string) argv[i]);
    }
#ifdef TEST
    ifstream ifFile("test.csv");
    if (!ifFile.good() || ifFile.eof()) {
    	cout << "ERROR OPENING FILE\n";
        exit(1);
    }

#endif
#ifndef TEST
    ifstream ifFile;
#endif

#ifdef TEST
    float fMaxFloat = 0;
    float fMinFloat = 0;
    float fAvgFloat = 0;
#endif

#ifndef TEST
	if ( argc > 1) {

	} else {
		do {
				system("clear");
				printTitle();
				cout << "Please Enter a File Name, With the Extension\n\n";
				cin >> sFileName;
				ifFile.open(sFileName);

				if (!ifFile.good()) {
					cout << "Invalid File or Cannot Open File.\n";
					system("clear");
				}
				else {
					bFileOpen = true;
				}
		}
		while (!bFileOpen);
	}
	#endif
    ifFile.clear();
    ifFile.seekg(0, ifFile.beg);

	if ( argc > 1) {
			ifFile.open(argv[1]);
			state = IDLE;
			if (argc > 2) {
				state = PROCESSING;
				sscanf(argv[2], "%d",& iTmp);
				if(iTmp < 1 || iTmp > 11) {
					iTmp = mOptions[vsArgs[2]];
					cout << iTmp << "\n";
					cout  << vsArgs[2] << "\n";
				} else
					iTmp -= 1;

				state = PROCESSING;
				command = (Command) iTmp;
			} else {

			}
		}

#ifdef DEBUG
    //cout << iLines << " Line\n";
#endif
    mHeader = vecToMap(sGetCSVHeader(ifFile));
    vsHeader = sGetCSVHeader(ifFile);
    while (true ) {
    	switch (state) {
        case IDLE:
        {
#ifdef TEST
        	ifFile.clear();
        	ifFile.seekg(0, ifFile.beg);
        	iCurrentItr++;
        	//command = DISPFILE;
        	//break;
        	if (iCurrentItr == 0 || iCurrentItr == 5 || iCurrentItr == 6 || iCurrentItr == 7)
        		iCurrentItr++;
        	if (iCurrentItr >= 5) {
        		cout << "The Test Results are: \n\n";
        		cout << "The Maximum Data for column 3 is " << fMaxFloat << "\n\n";
        		cout << "The Minimum Data for column 1 is " << fMinFloat << "\n\n";
        		cout << "The Average Data for column 2 is " << fAvgFloat << "\n\n";
        		cout << "The Data in Line 6 column 4 is " << sData <<"\n\n";
        		exit(1);
        	} else {
        		command = static_cast<Command >(iCurrentItr);
        		state = PROCESSING;
        		break;
        	}

#endif
        	ifFile.clear();
        	ifFile.seekg(0, ifFile.beg);
            if (bFirstRun){
                cout << "Welcome to LogReader V0.01."; 
                }
                bFirstRun = false;
            
            
            cout << "\n\n Please enter an option."
            		""
            		"\n\n1) Get Log Statistics (WIP) "
            		"\n2) Get Average"
            		"\n3) Get CSV Header "
            		"\n4) Get Maximum"
            		"\n5) Get Minimum"
            		"\n6) Get Data Element"
            		"\n7) About"
            		"\n8) When Did ..."
            		"\n9) Display File"
            		"\n10) Get Time Stats"
            		"\n11) Quit\n\n";
            
            /**************************************************************
           
            Welcome to LogReader V0.01. Please enter an option.
            
            1) Get Log Statistics
            2) Get Average
            3) Get Maximum
            4) Get Minimum
            5) Get Data Element
            6) About
            
            
            
            **************************************************************/
            
            cin >> sInput;
            size_t lastChar;
            int i;
            if (sscanf(sInput.c_str(), "%d", &i) <1) {
                cout << "Error: Invalid input. Please input a number within the range below\n\n";
                break;
            }
            iOption = stoi(sInput, &lastChar, 10);
            
            if (iOption > 11 || iOption < 1)
            {
                cout << "Please pick a valid option\n";
                system("clear");
                break;
            } else if (iOption == 1) {
            		state = PROCESSING;
            		command = GETSTAT;
            		cout << "\n\n";
            		system("clear");
            		break;
            } else if (iOption == 2) {
            		state = PROCESSING;
            		command = GETAVG;
            		cout << "\n\n";
            		system("clear");
            		break;
            } else if (iOption == 3) {
            		state = PROCESSING;
            		command = GETHEADER;
            		cout << "\n\n";
            		system("clear");
            		break;
            } else if (iOption == 4) {
            		state = PROCESSING;
            		command = GETMAX;
            		cout << "\n\n";
              	 system("clear");
              	 break;
            } else if (iOption == 5) {
            		state = PROCESSING;
            		command = GETMIN;
            		cout << "\n\n";
            		system("clear");
            		break;
            } else if (iOption == 6) {
                	state = PROCESSING;
                	command = GETDAT;
                	cout << "\n\n";
                	system("clear");
                	break;
            }  else if (iOption == 7) {
                	state = PROCESSING;
                	command = ABOUT;
                	cout << "\n\n";
                	system("clear");
                	break;
            }	else if (iOption == 8) {
            	 	 state = PROCESSING;
            	 	 command = WHEN;
            	 	 cout << "\n\n";
            	 	 system("clear");
            	 	 break;
            }	else if (iOption == 9) {
            		state = PROCESSING;
            		command = DISPFILE;
            		cout << "\n\n";
            		system("clear");
            		break;
            }	else if (iOption == 10) {
            		state = PROCESSING;
            		command = DELTATIME;
            		cout << "\n\n";
            		system("clear");
            		break;
            }		if (iOption == 11) {
            	 state = FINISHED;
            }
            break;
        }
        case PROCESSING:
        {
        	switch (command) {
        	case GETSTAT:
 			{
 				logStats = getLogStats(ifFile);
 				printLogStats(sFileName, logStats);
 				state = FINISHED;
 				break;
 			}
        	case GETAVG:
        	{
        		int iColNum;
        		int i = 1;

        		float fTempFloat = 0;
#ifndef TEST
        		float fAvgFloat = 0;
#endif
#ifdef TEST
        		//fAvgFloat = 0;
#endif
        		float fSum = 0;

        		string sTempLine;
        		string sColNum;

        		size_t lastChar;

        		vector <float > vfColData;
#ifndef TEST

        		cout << "What Column Number?\n\n";
        		cin >> sColNum;

        		iColNum = stoi(sColNum, &lastChar, 10);
#endif
#ifdef TEST
        		iColNum = 2;
#endif
        		ifFile.clear();
        		ifFile.seekg(0, ifFile.beg);
        		while (ifFile.good() && !ifFile.eof()) {
        			sTempLine = sGetElement(ifFile, i, iColNum);
        			if (sTempLine != "ERROR") {
        				if(sscanf(sTempLine.c_str(), "%f", &fTempFloat) < 1) {
        					fTempFloat = 0.00000;
        					vfColData.push_back(fTempFloat);
        				}
        				else
        					vfColData.push_back(fTempFloat);
        				i++;
        			}
        		}

        		for (unsigned int i = 0; i < vfColData.size(); i++) {
        			fSum += vfColData[i];
        		}
        		fAvgFloat = fSum / (float) vfColData.size();
#ifndef TEST
        		if (fSum == 0 || fAvgFloat == 0)
        			cout << "Either your lines are full of strings, or you have all 0s in your column\n\n";
        		else
        			cout << "Average Data in Column is " << fAvgFloat << ".\n\n";
#endif
#ifdef TEST
        		state = IDLE;
        		break;
#endif

        		state = FINISHED;
        		break;
        	}
        	case GETHEADER:
        	{
        		vector<string > vsCSVHeader;
        		vsCSVHeader = sGetCSVHeader(ifFile);

        		cout << "The CSV Header Contains:\n\n";
        		for (unsigned int i = 0; i < vsCSVHeader.size(); i++) {
        			if (vsCSVHeader[i] == "ERROR") {
        				state = FINISHED;
        				break;
        			}
        			cout <<"Column " << i+1 <<": " << vsCSVHeader[i] <<"\n\n";
        		}
        		state = FINISHED;
        		break;
        	}

        	case GETMAX:
        	{

        		int iColNum;
    			int i = 1;

        		float fTempFloat;
#ifndef TEST
        		float fMaxFloat;
#endif
        		string sTempLine;
        		string sColNum;

        		size_t lastChar;

        		vector <float > vfColData;
#ifndef TEST
        		cout << "What Column Number?\n\n";
        		cin >> sColNum;

        		iColNum = stoi(sColNum, &lastChar, 10);
#endif
#ifdef TEST
        		iColNum = 3;
#endif

        		ifFile.clear();
        		ifFile.seekg(0, ifFile.beg);
        		while (ifFile.good() && !ifFile.eof()) {
        			sTempLine = sGetElement(ifFile, i, iColNum);
        			if (sTempLine != "ERROR") {
        				if(ifFile.eof() || ifFile.bad())
        					break;
        				if(sscanf(sTempLine.c_str(), "%f", &fTempFloat) < 1) {
        					fTempFloat = 0.00000;
        					vfColData.push_back(fTempFloat);
        				}
        				else
        					vfColData.push_back(fTempFloat);
        				i++;
        			}
        		}

        		fMaxFloat = getMax(vfColData);
#ifndef TEST
        		cout << "Maximum Data in Column is " << fMaxFloat << ".\n\n";
#endif
#ifdef TEST
        		state = IDLE;
        		break;
#endif
        		state = FINISHED;
        		break;
        	}
        	case GETMIN:
        	{

        		int iColNum;
    			int i = 1;

        		float fTempFloat;
#ifndef TEST
        		float fMinFloat;
#endif
        		string sTempLine;
        		string sColNum;

        		size_t lastChar;

        		vector <float > vfColData;
#ifndef TEST
        		cout << "What Column Number?\n\n";
        		cin >> sColNum;

        		iColNum = stoi(sColNum, &lastChar, 10);
#endif
#ifdef TEST
        		iColNum = 1;
#endif
        		//ifFile.clear();
        		ifFile.seekg(0, ifFile.beg);
        		while (ifFile.good() && !ifFile.eof()) {
        			sTempLine = sGetElement(ifFile, i, iColNum);
        			if (sTempLine != "ERROR") {
        				if(sscanf(sTempLine.c_str(), "%f", &fTempFloat) >= 1)
        					vfColData.push_back(fTempFloat);
        				i++;
        			}
        		}

        		fMinFloat = getMin(vfColData);
#ifndef TEST
        		cout << "Minimum Data in Column is " << fMinFloat << ".\n\n";
#endif
#ifdef TEST
        		state = IDLE;
        		break;
#endif
        		//cout << "WIP\n";
        		state = FINISHED;
        		break;
        	}
        	case GETDAT:
        	{
        		int iLineNum = 0;
        		int iColNum = 0;

        		string sLineNum;
        		string sColNum;
        		string sLineOut;

        		size_t lastChar;
#ifndef TEST
        		cout << "What Line?\n\nLine: ";
        		cin >> sLineNum;

        		if (sscanf(sLineNum.c_str(), "%d", &iLineNum) <1) {
        			cout << "Error: Invalid input.\n\n";
        		}


        		cout << "What Column?\n\nColumn: ";
        		cin >> sColNum;

        		if (sscanf(sColNum.c_str(), "%d", &iColNum) <1) {
        			cout << "Error: Invalid input.\n\n";
        		}
        		iLineNum = stoi(sLineNum, &lastChar, 10);
        		iColNum = stoi(sColNum, &lastChar, 10);
#endif
#ifdef TEST
        		iLineNum = 6;
        		iColNum = 4;
#endif
        		//try {
        			sData = sGetElement(ifFile, iLineNum, iColNum );
#ifndef TEST
        			cout << "The data on line " << iLineNum <<", Column " << iColNum << " is " << sData << "\n\n";
#endif
#ifdef TEST
        			state = IDLE;
        			break;
#endif
        			state = FINISHED;
        			break;
        			//throw 20;

        	/*	}
        		catch (int e) {
        			cout << "Error: Error encountered whilst getting column data\n";
        			state = FINISHED;
        			break;

        		}


        		state = FINISHED;
        		break;*/
        	}
        	case DISPFILE:
        	{
        		cout << "\n\n";
        		ifFile.clear();
        		ifFile.seekg(0, ifFile.beg);
        		while (!ifFile.eof() && ifFile.good()) {
        			getline(ifFile, sLine);
        			cout << sLine << "\n";

        		}
        		break;
        	}
        	case ABOUT:
        	{

        		cout << "LogReader Version " << VERSION << ", Written By Nicholas Burrell\nPlease Note: The CSV Header MUST Have the Data Type Contained Inside of it, Seperated By Spaces\n e.g.)\n    int CarYear, string CarModel, string Company\n";

 			/*****************************************************

                LogReader Version V0.1 Written By Nicholas Burrell

                Please Note: The CSV Header MUST Have the Data Type Contained Inside of it, Seperated By Spaces

                Example)

                    int CarYear, string CarModel, string Company


    
    
                *****************************************************/
                
        		state = IDLE;
        		break;
        	}
        	case QUIT:
        	{
        		state = FINISHED;
        		break;
        	}
        	case WHEN:
        	{
        		string sEvent;
        		string sWhen;
        		string sTmp;

        		float fWhen = 0;
        		int iLine = 1;
        		int i = 0;
        		int iSumOfHeader = 0;
        		int iCol = mHeader["in_now"];

        		float y = 0;
        		float fTmp = 0.0;
        		for (unsigned int i = 0; i < vsHeader.size(); i++) {
        			iSumOfHeader += sscanf(vsHeader[i].c_str(), "%f", &fTmp);
        		}
        		i = 0;

        		cout << "What event are you looking for?\n\n";
        		cin >> sEvent;

        		cout << "\n\nWhat Value In That Column?\n\n";
        		cin >> sWhen;

        		if (sscanf(sWhen.c_str(), "%f", &fWhen) < 1) {
        			cout << "Error! Non Valid Integer entered\n";
        			state = FINISHED;
        			break;
        		}
        		else {
        			if (iSumOfHeader != 0)
        				iLine++;
        			while (y != fWhen) {
        				y = 0;
        				i++;
    					iLine++;
        				//cout << mHeader[sWhen] << "\n";
        				//cout << sGetElement(ifFile, iLine, iCol) << "\n";
        				sTmp = sGetElement(ifFile,i, mHeader[sEvent]);
        				//cout << sTmp << "\n";
        				sscanf(sTmp.c_str(), "%f", &y);
        			}
        			iLine--;
        			if (state == FINISHED)
        				break;
        			cout << "That event takes place at " << sGetElement(ifFile, iLine, iCol) << "; And the value is " << sGetElement(ifFile, iLine, mHeader[sEvent]) << "\n\n";
        		}
        	}
        	case DELTATIME: {

        		vector<float > vfColData;
        		string sTempLine;
        		int i = 0;
        		int iColNum = mHeader["in_now"];
        		float fTempFloat = 0;
        		float fLastFloat = 0;
        		float fSum = 0;
        		float fAvgFloat = 0;
        		ifFile.clear();
        		ifFile.seekg(0, ifFile.beg);
        		if (iColNum == 0) {
        			cout << "Error: No Time Column\n\n";
        			state = FINISHED;
        			break;
        		}
        		while (ifFile.good() && !ifFile.eof()) {
        			sscanf(sGetElement(ifFile, i-1, iColNum).c_str(), "%f", &fLastFloat);
        			sTempLine = sGetElement(ifFile, i, iColNum);
        			if (sTempLine != "ERROR") {
        				//cout << fLastFloat;
        				if(sscanf(sTempLine.c_str(), "%f", &fTempFloat) < 1) {
        					fTempFloat = 0.00000;
        					//fTempFloat -= fLastFloat;
        					vfColData.push_back(fTempFloat);
        				}
        				else
        					fTempFloat =- fLastFloat;
        				fLastFloat = fTempFloat;
        				vfColData.push_back(fTempFloat);
        				i++;
        				//cout << " : " << fTempFloat << "\n";
        			}
        		}

        		for (unsigned int i = 0; i < vfColData.size(); i++) {
        			fSum += vfColData[i];
        		}
        		fAvgFloat = fSum / (float) vfColData.size();

        		cout << fAvgFloat << " is the average time delta\n";
        	//	cout << "The maximum delta was" << getMax(vfColData) << "\n";
        	//	cout << "The minimum delta was" << getMin(vfColData) << "\n";
        		state = FINISHED;
        		break;
        	}
        	break;
        }
        case FINISHED:
        	{
 			cout << "Would You Like To Quit?\n\n[Y/n] ";
 			cin >> sContinue;
 			if (argc < 1) {
 				if (sContinue == "y" || sContinue == "Y" || sContinue == "yes" || sContinue == "YES" || sContinue == "Yes" ) {

 					cout << "Good Bye\n\n";
 					system("clear");
 					exit (1);

 					}
 				else if (sContinue == "n" || sContinue == "N" || sContinue == "no" || sContinue == "NO" || sContinue == "No" ) {

 					state = IDLE;
 					system("clear");
 					break;

 				}
 			} else {
 				exit(1);
 			}
        	}
        }
    	}
    }
}

///
/// Written by: Meynard Khel A. Yadao
///

#include "PhoneMessagesRecordParser.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include<map>
using std::string;

constexpr auto DEFAULT_INPUT_FILE = "input.csv";
constexpr auto DEFAULT_OUTPUT_FILE = "output.csv";

// Add/Modify values depending on input file order
// e.g. If DateSentTimestamp is first in input.csv, set its value to 0
enum InputFields
{
	Timestamp = 0,				//<Timestamp>
	DateSentTimestamp = 1,		//<DateSentTimestamp>
	DateReceivedTimestamp = 2,	//<DateReceivedTimestamp>
	SenderPhoneNumber = 3,		//<SenderPhoneNumber>
	ReceipientPhoneNumber = 4	//<RecepientPhoneNumber>
};

enum OutputFields
{
	PhoneNumber = 0,			//<PhoneNumber>
	TotalSent = 1,				//<TotalSent>
	TotalReceived = 2,			//<TotalReceived>
	HighestDay = 3,				//<The day with highest number of sent messages (dd-mm-yyyy)>
	MostSentNumber = 4,			//<The phone number to which the most messages were sent>
	MostTimestampGap = 5		//<The phone number of the interlocutor for whom the difference between received and sent messages from this phone number is the greatest>
};

bool ComparePhoneNumber(PhoneNumberStats pn1, PhoneNumberStats pn2)
{
	return pn1.phoneNumber < pn2.phoneNumber;
}

void PhoneMessagesRecordParser::SetInputFileName(const char * newFileName)
{
	strInputFileName.assign(newFileName);
}

void PhoneMessagesRecordParser::SetDelimiter(const char newDelimiter)
{
	delimiter = newDelimiter;
}

const char * PhoneMessagesRecordParser::GetInputFileName()
{
	return strInputFileName.c_str();
}

const char PhoneMessagesRecordParser::GetDelimiter()
{
	return delimiter;
}

void PhoneMessagesRecordParser::ProcessInputFile()
{
	if (strInputFileName == "")
	{
		strInputFileName.assign(DEFAULT_INPUT_FILE);
	}

	std::ifstream inputStream(strInputFileName); 

	if (inputStream.is_open())
	{
		ProcessInputFile(inputStream);
	}
	else
	{
		throw("Input file cannot be opened!");
	}
}

void PhoneMessagesRecordParser::ProcessInputFile(std::istream &inputStream)
{
	string currentLine{ "" };
	std::vector<string> lineData;
	long i = 0;
	while (std::getline(inputStream, currentLine))
	{
		//i++; //for debugging only
		//printf("\nentry %ld: %s", i, currentLine.c_str()); //for debugging only
		ProcessLine(currentLine, lineData);
		ProcessLineData(lineData);
		lineData.clear();
		//if (i > 100) break; //debugging purposes only
	}

	// # START FOR DEBUGGING PURPOSES ONLY
	//size_t j;
	//for (j = 0; j < metricData.size(); j++)
	//{
	//	printf("\npn: %s sent: %ld received: %ld maxDiff: %ld maxDiffNum: %s", metricData[j].phoneNumber.c_str(), metricData[j].totalSent, metricData[j].received, metricData[j].maxDiff, metricData[j].maxDiffNumber.c_str());
	//	for (auto testIt1 = metricData[j].receipientCount.begin(); testIt1 != metricData[j].receipientCount.end(); testIt1++)
	//	{
	//		printf("\n-receipient: %s count:%d", testIt1->first.c_str(), testIt1->second);
	//	}
	//	for (auto testIt2 = metricData[j].dayCount.begin(); testIt2 != metricData[j].dayCount.end(); testIt2++)
	//	{
	//		printf("\n-date: %s count:%d", testIt2->first.c_str(), testIt2->second);
	//	}
	//}
	//// # END
	PostProcessMetrics();
	BuildOutputMetricString();
}

void PhoneMessagesRecordParser::CreateMetricFile() 
{
	if (outputMetricString != "")
	{
		CreateMetricFile(outputMetricString);
	}
	else
	{
		throw "Input file not processed yet!";
	}
}

void PhoneMessagesRecordParser::CreateMetricFile(string& output)
{
	std::ofstream outputFile(DEFAULT_OUTPUT_FILE);
	std::stringstream sstr(output);

	if (outputFile.is_open())
	{
		outputFile << sstr.rdbuf();
		outputFile.close();
	}
	else
	{
		outputFile.open(DEFAULT_OUTPUT_FILE, std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
		outputFile.write(output.c_str(), sizeof(output));
		outputFile.close();
	}
}

string PhoneMessagesRecordParser::GetOutputMetricString()
{
	return outputMetricString;
}

void PhoneMessagesRecordParser::ProcessLine(const string& line, std::vector<string>& output)
{
	std::stringstream strStream(line);
	string data;
	while (std::getline(strStream, data, delimiter)) {
		output.push_back(data);
	}
}

void PhoneMessagesRecordParser::ProcessLineData(std::vector<string> &lineData)
{
	// Modify here the data formats as needed
	lineData[(int)SenderPhoneNumber] = ReadNumberWithoutSymbols(lineData[(int)SenderPhoneNumber]);
	lineData[(int)ReceipientPhoneNumber] = ReadNumberWithoutSymbols(lineData[(int)ReceipientPhoneNumber]);
	AddToMetrics(lineData);
	//inputData.push_back(lineData);
}


string PhoneMessagesRecordParser::ReadNumberWithoutSymbols(string numberString)
{
	string numericNumber{ "" };
	std::regex notNumeric("[^\\d]");
	numericNumber = std::regex_replace(numberString, notNumeric, "");

	return numericNumber;
}

string PhoneMessagesRecordParser::ConvertTimeFormat(string timeString)
{
	string outputString{ "" };
	const long SECS_PER_YEAR = 31536000;
	const long SECS_PER_MONTH = 2592000;
	const long SECS_PER_DAY = 86400;

	int baseYear = 1970;
	int baseMonth = 1;
	int baseDay = 1;

	long numericTimeString = atol(timeString.c_str());
	
	baseYear += (numericTimeString / SECS_PER_YEAR);
	numericTimeString %= SECS_PER_YEAR;
	baseMonth += (numericTimeString / SECS_PER_MONTH);
	numericTimeString %= SECS_PER_MONTH;
	baseDay += (numericTimeString / SECS_PER_DAY);
	
	if (baseDay > 9)
	{
		outputString.append(std::to_string(baseDay) + "-");
	}
	else
	{
		outputString.append("0" + std::to_string(baseDay) + "-");
	}

	if (baseMonth > 9)
	{
		outputString.append(std::to_string(baseMonth) + "-");
	}
	else
	{
		outputString.append("0" + std::to_string(baseMonth) + "-");
	}

	outputString.append(std::to_string(baseYear));

	return outputString;
}

// Add here operations that can only be done once all data are collected
void PhoneMessagesRecordParser::PostProcessMetrics()
{
	// Traverse each entry's receipientCount property
	// Update every receipient's received property
	size_t j;
	for (j = 0; j < metricData.size(); j++)
	{
		// Iteration on PhoneNumberStats.receiptCount
		for (auto k = metricData[j].receipientCount.begin(); k != metricData[j].receipientCount.end(); k++)
		{
			auto rcvIt = std::find_if(metricData.begin(), metricData.end(), [k](const PhoneNumberStats& c) { return c.phoneNumber == k->first; });
			if (rcvIt != metricData.end())
			{
				rcvIt->received += k->second;
			}
		}
	}
}

void PhoneMessagesRecordParser::AddToMetrics(std::vector<string> lineData)
{
	long i = 0;
	size_t j = 0;
	string temp{};
	string currentEntry{""};
	string tempDate{ "" };
	string tempReceipient{ "" };
	bool bEntryExists{ false }, bReceipientEntryExists{ false };
	std::map<string, int> tempMap1{}, tempMap2{}, tempMap3{};
	PhoneNumberStats tempStats;
	long tempDifference{ 0 };

		bEntryExists = false;
		bReceipientEntryExists = false;
		tempMap1.clear();
		tempMap2.clear();
		tempDate = "";
		tempReceipient = "";
		tempDifference = 0;

		temp = lineData.at((int)SenderPhoneNumber);

		//Search for Phone number if it exists
		for (j = 0; j < metricData.size(); j++)
		{
			//Update existing entry in the metricData if it exists
			if (metricData[j].phoneNumber == temp)
			{
				bEntryExists = true;
				
				metricData[j].totalSent++;

				// Check if current day is already in the map of messages with the current date
				// Increment count if yes, create new entry if not.
				tempDate = ConvertTimeFormat(lineData.at((int)DateSentTimestamp));
				auto findIndex = metricData[j].dayCount.find(tempDate);
				if (findIndex == metricData[j].dayCount.end())
				{
					metricData[j].dayCount.insert({ tempDate , 1L });
				}
				else
				{
					findIndex->second = findIndex->second + 1;
				}
				
				// Check if current receipient already has an entry in the receipient map
				// Increment count if yes, create new if not.
				tempReceipient = lineData.at((int)ReceipientPhoneNumber);
				auto findIndex2 = metricData[j].receipientCount.find(tempReceipient);
				if (findIndex2 == metricData[j].receipientCount.end())
				{
					metricData[j].receipientCount.insert({ tempReceipient, 1L });
				}
				else
				{
					findIndex2->second = findIndex2->second + 1;
				}

				//Check if current difference if bigger than existing
				//Replace existing if yes
				tempDifference = atol(lineData.at((int)DateReceivedTimestamp).c_str()) - atol(lineData.at((int)DateSentTimestamp).c_str()); // Date received minus Date sent
				if (tempDifference > metricData[j].maxDiff)
				{
					metricData[j].maxDiff = tempDifference;
					metricData[j].maxDiffNumber = lineData.at((int)ReceipientPhoneNumber);
				}
				break;
			}
		}

		//Add new record to metricData if it does not exist
		if (!bEntryExists)
		{
			tempMap1.insert({lineData.at((int)ReceipientPhoneNumber), 1L });
			tempMap2.insert({ ConvertTimeFormat(lineData.at((int)DateSentTimestamp)), 1L });
			tempDifference = atol(lineData.at((int)DateReceivedTimestamp).c_str()) - atol(lineData.at((int)DateSentTimestamp).c_str()); // Date received minus Date sent
			tempStats = {
				lineData.at((int)SenderPhoneNumber),
				1L,
				0L,
				tempDifference,
				lineData.at((int)ReceipientPhoneNumber),
				tempMap1,
				tempMap2,
			};

			metricData.push_back(tempStats);
			
		}
}

void PhoneMessagesRecordParser::BuildOutputMetricString()
{
	std::sort(metricData.begin(), metricData.end(), ComparePhoneNumber); // Sort by phone number, ascending order
	
	int i = 0;
	std::map<string, int>::iterator dayIt;		//iterator for PhoneNumberStats.dayCount
	std::map<string, int>::iterator rcpIt;		//iterator for PhoneNumberStats.receipientCount
	for (size_t j = 0; j < metricData.size(); j++)
	{
		for (i = 0; i < 6; i++)
		{
			if (i != 0)
			{
				outputMetricString.append(",");
			}
			switch (i)
			{
				case (int)PhoneNumber:
					outputMetricString.append(metricData[j].phoneNumber);
					break;
				case (int)TotalSent:
					outputMetricString.append(std::to_string(metricData[j].totalSent));
					break;
				case (int)TotalReceived:
					outputMetricString.append(std::to_string(metricData[j].received));
					break;
				case (int)HighestDay:
					 dayIt = std::max_element(metricData[j].dayCount.begin(), metricData[j].dayCount.end(),
						[](const std::pair<string, int>& a, const std::pair<string, int>& b)->bool 
						{ return a.second < b.second; });
					 outputMetricString.append( dayIt->first);
					break;
				case (int)MostSentNumber:
					rcpIt = std::max_element(metricData[j].receipientCount.begin(), metricData[j].receipientCount.end(),
						[](const std::pair<string, int>& a, const std::pair<string, int>& b)->bool
						{ return a.second < b.second; });
					outputMetricString.append(rcpIt->first);
					break;
				case (int)MostTimestampGap:
					outputMetricString.append(metricData[j].maxDiffNumber);
					break;
			}
			
		}

		outputMetricString.append("\n");
	}

	//printf("\n%s\n", outputMetricString.c_str()); //debugging purposes only
}

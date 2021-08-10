///
/// Written by: Meynard Khel A. Yadao
///

#include <vector>
#include <fstream>
#include <map>
using std::string;

struct PhoneNumberStats
{
	string phoneNumber;
	long totalSent = 0;
	long received = 0;
	long maxDiff = 0;
	string maxDiffNumber;
	std::map<std::string, int> receipientCount;
	std::map<std::string, int> dayCount;
};

#pragma once
class PhoneMessagesRecordParser
{

// Variables
private:

	string strInputFileName{ "" };
	char delimiter{ ',' };
	string outputMetricString{ "" };
	std::vector<PhoneNumberStats> metricData;
// Methods
public:
	PhoneMessagesRecordParser() = default;
	virtual void SetInputFileName(const char*);
	virtual void SetDelimiter(const char);
	virtual const char* GetInputFileName();
	virtual const char GetDelimiter();
	virtual void ProcessInputFile();
	virtual void ProcessInputFile(std::istream &);
	virtual void CreateMetricFile();
	virtual void CreateMetricFile(string&);
	virtual string GetOutputMetricString();
protected:
	virtual void ProcessLine(const string&, std::vector<string> &);
	virtual void ProcessLineData(std::vector<string> &);
	virtual void AddToMetrics(std::vector<string>);
	virtual void PostProcessMetrics();
	virtual string ReadNumberWithoutSymbols(string);
	virtual string ConvertTimeFormat(string);
	void BuildOutputMetricString();


};


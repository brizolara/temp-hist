//	Histogram of alphabet characters (case-insensitive) for an arbitrary set of files informed by the user
//	Usage: ./text_histogram file1 file2 file3 etc
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

typedef std::pair<int, long unsigned int> THistBin;

int nletters;
int ascii_A_asInt = (int)'A';
int ascii_Z_asInt = (int)'Z';
int ascii_a_asInt = (int)'a';
int ascii_z_asInt = (int)'z';

void fillHistogram(long unsigned int* p_hist, std::vector<THistBin>* p_histVector , std::ifstream& p_file)
{
	int character = 0;

	//	We first fill our non-sorted array hist...

	if (p_file.is_open()) {
		while (!p_file.eof()) {
			character = p_file.get();
			if (character >= ascii_a_asInt && character <= ascii_z_asInt) {	//	a to z
				p_hist[character - ascii_a_asInt]++;
			}
			else if (character >= ascii_A_asInt && character <= ascii_Z_asInt) {	//	Not case sensitive: A to Z counts as a to z
				p_hist[character - ascii_A_asInt]++;
			}
		}
	}

	//	...	then init our vector of pairs <index,frequency> to be sorted
	for (int i = 0; i < nletters; i++) {	//	a to z
		THistBin bin(i+ ascii_a_asInt, p_hist[i]);
		p_histVector->push_back(bin);
	}
}

int compare(const THistBin& a, const THistBin& b)
{
	return (a.second) > (b.second);
}

void sortHistogram(std::vector<THistBin>* p_hist)
{
	std::sort(p_hist->begin(), p_hist->end(), compare);
}

//---------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	nletters = ascii_Z_asInt - ascii_A_asInt + 1;	// number of possible letters = a to z

	std::ifstream f;
 
	std::vector<THistBin>* histSorted = new std::vector<THistBin>;	//	To sort. Each THistBin is a pair <index, frequency>
	unsigned long int* hist = new unsigned long int [nletters];		//	Unsorted , zero-indexed. Ex.: hist[0] is the histogram value for the letters a and A
	
	for (int i = 1; i < argc; i++)
	{
		memset(hist, 0, sizeof(unsigned long int)*nletters);

		f.open(argv[i]);
		fillHistogram(hist, histSorted, f);
		f.close();

		sortHistogram(histSorted);

		std::cout << std::endl << argv[i]  << std::endl;
		for (int n = 0; n < nletters; n++) {
			std::cout << (char)(histSorted->at(n).first) << ": " << histSorted->at(n).second << std::endl;
		}

		histSorted->clear();
	}

	delete [] hist;
	delete histSorted;
}

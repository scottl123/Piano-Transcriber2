//============================================================================
// Name        : WavTest.cpp
// Author      : Derrs
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include    <iostream>
#include	<stdio.h>
#include	<string.h>
#include    <math.h>
#include 	<cmath>
#include 	<vector>
#include	<set>
/* Include this header file to use functions from libsndfile. */
#include 	<sndfile.h>
#include 	<fftw3.h>

using namespace std;

/*	This will be the length of the buffer used to hold.frames while
**	we process them.
*/
#define		BUFFER_LEN	5000000

/* libsndfile can handle more than 6 channels but we'll restrict it to 2. */
#define		MAX_CHANNELS	2

/* Function prototype. */
static void process_data (short *data, int count, int channels) ;


int main (void)
{	/* This is a buffer of double precision floating point values
	** which will hold our data while we process it.
	*/
	static short data [BUFFER_LEN] ;

	/* A SNDFILE is very much like a FILE in the Standard C library. The
	** sf_open function return an SNDFILE* pointer when they successfully
	** open the specified file.
	*/
	SNDFILE	*infile, *outfile ;

	/* A pointer to an SF_INFO struct is passed to sf_open.
	** On read, the library fills this struct with information about the fil
e.
	** On write, the struct must be filled in before calling sf_open.
	*/
	SF_INFO		sfinfo ;
	int			readcount ;
	const char	*infilename = "input1.wav" ; //works
	//const char	*infilename = "Piano_Slower1.wav" ;
	const char	*outfilename = "output.wav" ;
	double signal_max;

	/* The SF_INFO struct must be initialized before using it.
	*/
	memset (&sfinfo, 0, sizeof (sfinfo));

	cout <<  "Size of sfinfo = " << sizeof(sfinfo) << endl;

	/* Here's where we open the input file. We pass sf_open the file name an
d
	** a pointer to an SF_INFO struct.
	** On successful open, sf_open returns a SNDFILE* pointer which is usedfind the sndfile.h file.
	** for all subsequent operations on that file.
	** If an error occurs during sf_open, the function returns a NULL pointe
r.
	**
	** If you are trying to open a raw headerless file you will need to set
the
	** format and channels fields of sfinfo before calling sf_open(). For
	** instance to open a raw 16 bit stereo PCM file you would need the foll
owing
	** two lines:
	**
	**		sfinfo.format   = SF_FORMAT_RAW | SF_FORMAT_PCM_16 ;
	**		sfinfo.channels = 2 ;
	*/
	if (! (infile = sf_open(infilename, SFM_READ, &sfinfo)))
	{	/* Open failed so print an error message. */
		printf ("Not able to open input file %s.\n", infilename) ;
		/* Print the error message from libsndfile. */
		puts (sf_strerror (NULL)) ;
		return 1 ;
		} ;

	cout << "There are " << sfinfo.channels << " channels " << endl;
	cout << "frames " << sfinfo.frames << endl;
	cout << "samplerate " << sfinfo.samplerate << endl;
	cout << "format " << sfinfo.format << endl;
	cout << "sections " << sfinfo.sections << endl;
	cout << "seekable " << sfinfo.seekable << endl;

	sf_command (infile, SFC_CALC_SIGNAL_MAX, &signal_max, sizeof (signal_max)) ;
	cout << "Signal MAX is " << signal_max << endl;


	if (sfinfo.channels > MAX_CHANNELS)
	{	printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
		return 1 ;
		} ;
	/* Open the output file. */
	if (! (outfile = sf_open (outfilename, SFM_WRITE, &sfinfo)))
	{	printf ("Not able to open output file %s.\n", outfilename) ;
		puts (sf_strerror (NULL)) ;
		return 1 ;
		} ;

	/* While there are.frames in the input file, read them, process
	** them and write them to the  output file.
	*/
	while ((readcount = sf_read_short (infile, data, BUFFER_LEN)))
	{
		cout << "entering_process_data  " << readcount << endl;
		process_data (data, readcount, sfinfo.channels) ;
		cout << "Back from process_data, writing = " << readcount << " bytes" << endl;
		sf_write_short (outfile,  data, readcount) ;
		} ;

	cout << "Readcount from sf_read_short = " << readcount << endl;

	/* Close input and output files. */
	sf_close (infile) ;
	sf_close (outfile) ;

	return 0 ;
} /* main */

static void process_data (short *data, int count, int channels)
{
//	double channel_gain [MAX_CHANNELS] = {3.5, 1};
	cout << "this works at the beginning" << endl;
	int k = 0;
	int chan = 0;
	int nmin = 0;
	int nmax = 0;
	int lmin = 0;
	int lmax = 0;
	int ampMinBound = 0;
	cout << "k is " << k << endl;
	cout << "lmax is " << lmax << endl;
	int ampMaxBound = 0;
	int onsetWindowNumber = 0;
	int numberOfAmps = 0;
	int iLower = 0;
	int numberOfOnsets = 0;
	int onsetInPreviousWindow = 0;
	int onsetInThisWindow = 0;
	double energyBefore = 0;
	short max = 0;
	short min = 0;
	float dataNew[count] = {};
	cout << "this works at the beginning2" << endl;
	float dataInnerNormalized[count] = {0}; //issue
	cout << "this works at the beginning3" << endl;
	vector<double> amp;
	vector<double> energyVectorWindows;
	vector<set<int, double> > energyVectorSamples;
	vector<int> onsetWindows;
	vector<int> onsetSampleNumbers;
	/* Process the data here.
	** If the soundfile contains more then 1 channel you need to take care o
f
	** the data interleaving youself.
	** Current we just apply a channel dependant gain.
	*/
	cout << "Number of channels is " << channels << endl;
	for (chan = 0 ; chan < channels ; chan++)
	{
		cout << "this for loop works\n\n" << endl;
		min = data[0];
		max = data[0];
		for (k = chan ; k < count ; k+= channels)
		{
	//		data [k] *= channel_gain [chan] ;

			if(k == 0)
			{
				min = data[k];
				max = data[k];
			}
			else if(data[k] > max)
			{
				max = data[k];
			}
			else if(data[k] < min)
			{
				min = data[k];
			}

			if(k < 10)
			{
				cout << "Sample Value = " << data[k] << " , Sample # " << k << endl;
			}
			if(k == count - 1)
			{
				cout << "The last sample is " << k << endl;
			}

		}
	}

	cout << "The Maximum signed value = " << max << endl;//find the sndfile.h file.
	cout << "The Minimum signed value = " << min << endl;

 // normalization starts here
	for(int i = 0; i < count; i++) {
		long double x = 1.0;
		long double y = -1.0;
		if( data[i] > 0) {
			dataNew[i] = x/(max)*data[i];
		}
		else if( data[i] < 0) {
			dataNew[i] = y/(min)*data[i];
		}
		}
	for (chan = 0 ; chan < channels ; chan ++)
		{
		nmin = dataNew[0];
		nmax = dataNew[0];
		for (k = chan ; k < count ; k+= channels)
		{
	//		data [k] *= channel_gain [chan] ;

			if(k == 0)
			{
				nmin = dataNew[k];
				nmax = dataNew[k];
			}
			else if(dataNew[k] > nmax)
			{
				nmax = dataNew[k];
			}
			else if(dataNew[k] < nmin)
			{
				nmin = dataNew[k];
			}
			if(k < 10)
					{
						cout << "Sample Value = " << dataNew[k] << " , Sample # " << k << endl;
					}
						if(k == count - 1)
					{
						cout << "The last sample is " << k << endl;
					}
		}

			cout << "The New Maximum signed value = " << nmax << endl;//find the sndfile.h file.
			cout << "The New Minimum signed value = " << nmin << endl;
		}
			//global normalization ends here. Stored in dataNew[] array
			//inner normalization begins

	for (int i = 0; i < count; i++)
	dataInnerNormalized[i] = dataNew[i];
			for( int i = 0; i < count; i++) { //to identify places where amp is greater than .5
				if( dataNew[i] > .5) {
					amp.push_back(i);
					numberOfAmps++;
				}
			}
			cout << "Fourth .5 value is " << amp[3] << endl;
			cout << "NumberofAmps = " << numberOfAmps << endl;
			for( int i = 0; i < numberOfAmps; i++) { //identifies where amps > .5 are more than 4.5 s apart, normalizes it

				if(amp[i++] - amp[i] > 4.5*41000) {

				/*	for (chan = 0 ; chan < channels ; chan ++)
						{ */
					ampMinBound = amp[i];
					ampMaxBound = amp[i++];
						lmin = ampMinBound;
						lmax = ampMinBound;
							for (int b = chan ; b < ampMaxBound ; b+= channels)
							{
								if(b == amp[i])
								{
									lmin = data[b];
									lmax = data[b];
								}
								else if(data[b] > lmax)
								{
									lmax = data[b];
								}
								else if(data[b] < lmin)
								{
									lmin = data[b];
								}

					/*			cout << "Inner Normalized max is " << lmax << endl;
								cout << "Inner Normalized min is " << lmin << endl; */
								for(int p = ampMinBound; p < ampMaxBound; p++) {
										long double x = 1.0;
										long double y = -1.0;
										if( data[p] > 0) {
											dataInnerNormalized[p] = x/(lmax)*data[p];
										}
										else if( data[i] < 0) {
											dataInnerNormalized[p] = y/(lmin)*data[p];
										}
								}

							}

				}
			}
			//to find first window
	while (onsetWindowNumber == 0) {
		double Energy = 0.0;
		for(int j = iLower; j < iLower + 3000; j++) {
			 //energy in window calculation begins
			Energy = Energy + dataInnerNormalized[j]*dataInnerNormalized[j]; }
	/*	cout << "While iLower is " << iLower << endl; //test to see all energy and iLowers
		cout << "Energy is " << Energy << endl; */
			if (Energy > .75) {
				//find max, subtract by 600
				int onsetNumberOne = iLower;
				for(int k = iLower; k < iLower + 3000; k++) {
					if( dataInnerNormalized[k] > dataInnerNormalized[onsetNumberOne]) {
						onsetNumberOne = k;
					}
				}
				onsetSampleNumbers.push_back(onsetNumberOne);
				Energy = 0.0;
				iLower = onsetNumberOne - 600;
				for(int j = iLower; j < iLower + 3000; j++) {
						 //energy in window calculation begins
						Energy = Energy + dataInnerNormalized[j]*dataInnerNormalized[j];
				}
				cout << "onsetNumberOne is " << onsetNumberOne << endl;
				cout << "iLower of first window is " << iLower << endl;
				onsetWindowNumber = onsetWindowNumber + 1;
				//set<double> energyWindowSet(onsetWindowDoubleNumber, Energy);//storing energy in vector (program's error in these lines
				//set<int, double> energyWindowSet(onsetWindowDoubleNumber, Energy);
				//energyVectorWindows.push_back(energyWindowSet);
			/*	set<int, double> energySet(iLower, Energy); //storing energy in vector (program's error in these two lines)
				energyVectorSamples.push_back(energySet);
*/
				energyVectorWindows.push_back(Energy);
				cout << "Energy #1 is " << energyVectorWindows.at(0) << endl;
				numberOfOnsets = numberOfOnsets + 1;
				onsetInPreviousWindow = 1;
				energyBefore = Energy;
			}
			else {
				Energy = 0.0;
				iLower = iLower + 3000;
			}
		//to find first window ends
	}
//sliding windows begins
	int windowNumber = 0;
	while(iLower < count - 3000) {
		double Energy = 0.0; //how/where should energy should be declared?
		for(int j = iLower; j <= iLower + 2999; j++) {
			Energy = Energy + dataInnerNormalized[j]*dataInnerNormalized[j];
		}
		/*if(((Energy > .75) && (Energy > energyVectorWindows.at(windowNumber)) && (energyVectorWindows.at(windowNumber) < .75))||(
		(Energy > .75) && (Energy > energyVectorWindows.at(windowNumber)) && !(energyVectorWindows.at(windowNumber) < .75) && (windowNumber))//window number is in onsetWindows
		) */
		if(Energy > .75) {
			if (Energy > energyBefore) {
				if (energyBefore < .75) {
					onsetInThisWindow = 1;
				}
				else {
					if (onsetInPreviousWindow == 1) {
						if (Energy > energyBefore*1.9) {
							onsetInThisWindow = 2;
						}
						else {
							onsetInThisWindow = 0;
						}
					}
					else {
						if (Energy > energyBefore + 6.05) {
							onsetInThisWindow = 3;
						}
						else {
							if (Energy > energyBefore + 3) {
								double energyStar = 0.0;
								for (k = iLower + 3000; k <= iLower + 5999; k++) {
									energyStar = energyStar + dataInnerNormalized[k]*dataInnerNormalized[k];
								}
								if (energyStar > Energy + 3) {
									onsetInThisWindow = 4;
								}
								else {
									onsetInThisWindow = 0;
								}
							}
							else {
								onsetInThisWindow = 0;
							}
						}
					}
				}
			}
			else {
				onsetInThisWindow = 0;
			}
		}
		else {
			onsetInThisWindow = 0;
			//also silent
		}
		//what to do once onset is found or not in window
		if (onsetInThisWindow >= 1){
			int onsetNumberX = iLower;
			for(int k = iLower; k < iLower + 3000; k++) {
				if( dataInnerNormalized[k] > dataInnerNormalized[onsetNumberX]) {
					onsetNumberX = k;
					}
				}
			cout << "Onset on sample " << onsetNumberX << " Type " << onsetInThisWindow << endl;
			onsetSampleNumbers.push_back(onsetNumberX);
			Energy = 0.0;
			iLower = onsetNumberX - 600;
			for(int j = iLower; j < iLower + 3000; j++) {
									 //energy in window calculation begins
				Energy = Energy + dataInnerNormalized[j]*dataInnerNormalized[j];
				}
			energyVectorWindows.push_back(Energy);
			windowNumber = windowNumber + 1;
			onsetWindows.push_back(windowNumber);
			onsetInPreviousWindow = 1;
			iLower = iLower + 3000;
			energyBefore = Energy;
			Energy = 0.0;
			numberOfOnsets = numberOfOnsets + 1;
		/*	cout << "Energy #1 is " << energyVectorWindows.at(0) << endl;
			cout << "Window number is " << windowNumber << endl;
			iLower = count; */
		}
		else {
			energyBefore = Energy;
			iLower = iLower + 3000;
			energyVectorWindows.push_back(Energy);
			windowNumber = windowNumber + 1;
			onsetInPreviousWindow = 0;
			Energy = 0.0;
		}
	}
for (int l = 0; l < numberOfOnsets; l++) {
	cout << "Onset #" << l + 1 << " is on Sample " << onsetSampleNumbers.at(l) << endl;
}
	return;
}/* process_data */


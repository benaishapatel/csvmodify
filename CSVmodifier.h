#ifndef CSVMODIFIER_HPP
#define CSVMODIFIER_HPP

/* Filename: CSVmodifier.h
 * Author: Benaisha Patel
 * Description: Function declarations for CSVmodifier.cpp
 */

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
//imports for file opening, reading
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
//imports for image comparison from openCV library
#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


    /* 
	 * Function name: findAllCSV()
	 * Function prototype:
	 *    			int findAllCSV(ifstream &ifile)
	 * Description: This method will search for ASSETLISTs, and call recursively if
	 *				any ASSETLIST is found, it will also search for IMAGE types and
	 *				parse the contents of the .csv file by itype, GUID, and filepath
	 *				If neither is found, we've reached the base case where we break
	 *				Once all images are parsed, the GUID and filePath will be
	 *				inserted into an unordered map with GUID as the key and
	 *				filePath as the value.
	 *              
	 * Parameters:
	 *      arg1: ifstream &ifile-- reference to opened file of type ifstream
	 * Side Effects: 
	 * Error Conditions: returns int -1 if file cannot be opened
	 *                   
	 * Return Value: int 0 if no errors
	 */

	int findAllCSV(ifstream &filename);

	/* 
	 * Function name: compareImg()
	 * Function prototype:
	 *    	bool compareImg (const Mat & a, const Mat & b)
	 * Description: This method will compare two images by comparing matrix values
	 *				using openCV library function bitwise_xor to find duplicates
	 *              
	 * Parameters:
	 *      arg1: const Mat & a-- reference to first image's matrix
	 *      arg2: const Mat & b-- reference to second image's matrix    
	 * Side Effects: 
	 * Error Conditions: 
	 *                   
	 * Return Value: True if duplicate, false if not
	 */

	bool compareImg (const Mat& a, const Mat& b);

	/* 
	 * Function name: checkForDuplicates()
	 * Function prototype:
	 *    bool checkForDuplicates(string image1, string image2)
	 * Description: This method will convert two images into Mat, then it will
	 *				call compareImg which will return true or false
	 *              
	 * Parameters:
	 *      arg1: string image1 -- first image
	 *      arg2: string image2 -- image to compare with image1     
	 * Side Effects: 
	 * Error Conditions: -1 -- if an image is invalid
	 *                   
	 * Return Value: True if images are the same, False if not
	 */
	bool checkForDuplicates(string image1, string image2);

	/* 
	 * Function name: writeBack()
	 * Function prototype:
	 *    int writeBack(ifstream &ifile, ofstream &ofile, string filename)
	 * Description: This function will write the original CSV on a temp file
	 *				and check for any modifications while doing so for the new line
	 *				after this is complete, it will remove the existing csv and 
	 *				replace it with temp
	 *              
	 * Parameters:
	 *      arg1: ifstream &ifile - file to be read
	 *      arg2: ofstream &ofile - temp file to write on   
	 *		arg3: string filename - name of original csv
	 * Side Effects: 
	 * Error Conditions: -1 if file cannot be opened
	 *                   
	 * Return Value: 0
	 */


	int writeBack(ifstream &ifile, ofstream &ofile, string filename);

#endif //CSVMODIFIER_HPP
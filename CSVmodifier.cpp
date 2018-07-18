/* Filename: CSVmodifier.cpp
 * Author: Benaisha Patel
 * Description: C++ module that will read the contents of multiple .csv files 
 *				check recursively for image files, and see if the image files
 *				are duplicates with different references. If they are then the
 *				program will write back to the csv file with the same reference
 *				for each duplicate image pair.
 * Date: June 25, 2017
 */

#include "CSVmodifier.h"

/* global variables*/
static unordered_map<string, string> imgMap;
static unordered_map<string, string> final;
static vector<string> all_csv;
static char delim = '\"';
static int assetCount = 1;

/* 
 * Function name: main()
 * Function prototype:
 *    int main(int argc, char *argv[])
 * Description: This main method will open each file and call findAllCSV
 *				then it will write back with modified changes to duplicate 
 *				images within the csv(s) by calling writeBack.
 *              
 * Parameters:
 *      arg1: argc-- this is the number of arguments inputted
 *      arg2: char *argv[]-- this is the char pointer to the array      
 * Side Effects: 
 * Error Conditions: -1 -- if the file can't be opened, invalid filename
 *							or any other function throws and error
 *                   
 * Return Value: 0, but the modified csv file back in the directory.
 */


int main(int argc, char * argv[]) {

	//loop to have the function run for each input csv
	for(int i =1; i < argc; i++){

		ofstream ofile;

		ifstream ifile (argv[i], ios::in | ios::binary);

		if(ifile.is_open()) {
			if(findAllCSV(ifile) == -1){
				cout<<"Problem calling findAllCSV"<<endl;
				return -1;
			}
		}
		else{
			cerr<< "File: " << argv[i] <<" could not be opened"<<endl;
			return -1;
		}
				
	    int pos;
	    int posMod;

		for(auto it = imgMap.begin(); it != imgMap.end(); it++){
			for(auto it2 = imgMap.begin(); it2 != imgMap.end(); it2++){

				pos = it->second.find('\"'); //change to delim

				//we have -3 here because the length must be line.length-1
				//however we also want 2 less characters, '\"' and '\"'
				int end_str = it->second.length()-pos-3;
				string imagePath = it->second.substr(pos+1,end_str);

				posMod = it2->second.find('\"'); //change to delim

				//we have -3 here because the length must be line.length-1
				//however we also want 2 less characters, '\"' and '\"'
				int end_strMod = it2->second.length()-posMod-3;
				string imagePath2 = it2->second.substr(posMod+1,end_strMod);

				//call checkForDuplicates and if dupl, insert modified vals
				if(checkForDuplicates(imagePath, imagePath2))
				{

					final.insert(make_pair(it->first, it->second));
					final.insert(make_pair(it2->first, it->second));
				}
				
			}
		}

		//Close the file
		ifile.close();

		//reopen file to get to beginning
		ifile.open (argv[i], ios::in | ios::binary);

		if(ifile.is_open()) {
			//call writeBack with a temporary csv to write onto
			ofile.open("temp.csv", ios::out| ios::binary);
			string filename = argv[i];
			if(writeBack(ifile, ofile, filename)==-1){
				cout<<"Problem calling writeBack"<<endl;
				return -1;
			}

		}

		//Close the file
		ifile.close();

		//clear all vals for next csv input to run program
		imgMap.clear();
		final.clear();
		assetCount = 1;

	}


	return 0;

}

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


int findAllCSV(ifstream &ifile){
	string line = "";
	int pos;
	while(getline(ifile,line)){
		if(ifile.eof()){
			break;
		}

		//create string stream
		istringstream ss(line);
		string itype;
			 
		while(getline(ss, itype, ',')) {


			if(itype=="ASSETLIST"){
				assetCount++;
				pos = line.find(delim);

				//we have -3 here because the length must be line.length-1
				//however we also want 2 less characters, '\"' and '\"'
				int end_str = line.length()-pos-3;
				string filename1 = line.substr(pos+1,end_str);
				ifstream ifile1 (filename1, ios::in | ios::binary);
				if(ifile1.is_open()){
					findAllCSV(ifile1);
				}
				else{
					cerr<< "File: " << filename1 <<" could not be opened"<<endl;
					return -1;
				}
			
			}
			//check only for IMAGE types
	    	else if(itype == "IMAGE") {
				//parse GUID
				string GUID;
				getline(ss, GUID, ',');
	    						
	 			//parse so last part before new line is the file path
				string filePath;
				getline(ss, filePath, '\n');

				//insert pair into unordered map, where key is GUID, 
				//value is filePath
				imgMap.insert(make_pair(GUID, filePath));
						
			}
			//base case
			else {
				break;
			}
		}			
	}
		
	return 0;

}

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

bool checkForDuplicates(string image1, string image2) {

	Mat images1;
	Mat images2;

	//read images
	images1 = imread(image1);
	images2 = imread(image2);

	//check if image file is empty
	if(!images1.data || !images2.data)
	{
		cerr<<images1<<" or "<<images2<<" could not be found"<<endl;
		return -1;
	}
	//call compareImg and print based on true or false
	if( compareImg(images1, images2)  == 1) {
  		return true;
	}
	else {
		return false;
	}

	return false;

}

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

bool compareImg (const Mat & a, const Mat & b){
	//if not even the same size file, false
	if ( (a.rows != b.rows) || (a.cols != b.cols) )
        return false;

    //create Mat for difference, input a, input b
    Mat difference[3];
    Mat channel[3];
    Mat channels[3];
    
    //split by rgb val
    split(a, channel);
    Mat Blue(channel[0]);
    Mat Green(channel[1]);
    Mat Red(channel[2]);

    split(b, channels);
    Mat Blue1(channels[0]);
    Mat Green1(channels[1]);
    Mat Red1(channels[2]);

    //xor for difference and insert into difference Mat
    bitwise_xor(Blue, Blue1, difference[0]);
    bitwise_xor(Green, Green1, difference[1]);
    bitwise_xor(Red, Red1, difference[2]);

    //if no difference, xor should make all vals 0, so check for nonZero
    if(countNonZero(difference[0])==0 && countNonZero(difference[1])==0 && 
    	countNonZero(difference[2])==0)
    {
    	return true;
    }

    return false;
}

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

int writeBack(ifstream &ifile, ofstream &ofile, string filename){
	//all_csv.push_back(filename);
	string current = filename;
	for(int i = 0; i<assetCount; i++){
		string line = "";
		int pos;

		while(getline(ifile,line)){
			if(ofile.eof()){
				break;
			}

			//create string stream
			istringstream ss(line);
			string itype;
				 
			while(getline(ss, itype, ',')) {
				//cout<<"else if"<<itype<<endl;

				if(itype=="ASSETLIST"){

					pos = line.find(delim);

					//we have -3 here because the length must be line.length-1
					//however we also want 2 less characters, '\"' and '\"'
					int end_str = line.length()-pos-3;
					string filename1 = line.substr(pos+1,end_str);

					//add filename to vector for all csv
					all_csv.push_back(filename1);
					//add current line to temp ofile
					ofile<<line;
					break;
				}

				//check only for IMAGE types
		    	else if(itype == "IMAGE") {
					//parse GUID
					string GUID;
					getline(ss, GUID, ',');

		    		//loop through final for any potential key matches
					for(auto it = final.begin(); it != final.end(); it++){
						//check if key from final unordered map matches GUID
						if(GUID == it->first){
							string newFilePath = it->second;
							//new line with modified filePath
							string toChange = "IMAGE,"+GUID+ ","+newFilePath;
							//write on temp ofile with new line
							ofile<<toChange;
							continue;
						}
					}
					break;			
				}
				else {
					//add current line to temp ofile
					ofile<<line;
					break;
				}
			}

		}
		//close current ifile and ofile
		ifile.close();
		ofile.close();

		//delete current, unmodified csv in directory
		remove(current.c_str());
		//rename temp to deleted csv's name (overwrite old csv essentially)
		rename("temp.csv",current.c_str());

		//if we have more files in vector, we need to continue looping to write
		if(!all_csv.empty()){
			//set new val for current string for filename when renaming
			current = all_csv.front();
			//remove this string name from vector to avoid going back to the csv
			all_csv.erase(all_csv.begin());

			//open current file
			ifile.open(current);
			//if open, also open a new temp ofile
			if(ifile.is_open()) {
				ofile.open("temp.csv", ios::out| ios::binary);
			}
			else{
				cerr<< "File: " << current <<" could not be opened"<<endl;
				return -1;
			}
		}

	}
	return 0;
}

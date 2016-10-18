#include "mainSample.h"

int main(int argc, char **argv)
{
	int k = 4;
	int L = 5;
	int Radius = 0;
	int dataLength;     //used for hamming size, or number of vector attributes
	ifstream inputFile;
	ifstream queryFile;
	ofstream outputFile;
	string metric_space;
	string genericStr;

	srand (time(NULL));

	if (argc > 1) {
		if (argc % 2 == 0) {
			cout << "Missing the correct number of parameters" << endl;
			cout << "Suggested use: $./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file>" << endl;
			return -1;
		}
		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-d") == 0) {
				inputFile.open(argv[i+1]);		//input file comes next on argv
				if (inputFile == NULL)
				{
					cout << "You've given a wrong input file. " << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl << endl;
				}
				inputFile >> metric_space;  //read "@metric space"
				inputFile >> metric_space;	//read etc, "hamming"

				i++;


			}
			else if (strcmp(argv[i], "-q") == 0) {
				queryFile.open(argv[i+1]);		//query file comes next on argv
				if (queryFile == NULL)
				{
					cout << "You've given a wrong query file. " << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl << endl;
				}
				queryFile >> genericStr;  //read "@metric space"
				queryFile >> genericStr;	//read etc, "hamming"
				Radius = atoi(genericStr.c_str());
				//Radius = stoi(genericStr);

				i++;
			}
			else if (strcmp(argv[i], "-k") == 0) {
				//k = stoi(argv[i+1]);
				k = atoi(argv[i+1]);
				i++;
			}
			else if (strcmp(argv[i], "-L") == 0) {
				//L = stoi(argv[i+1]);
				L = atoi(argv[i+1]);
				i++;
			}
			else if (strcmp(argv[i], "-o") == 0) {
				queryFile.open(argv[i+1]);		//output file comes next on argv
				if (queryFile == NULL)
				{
					cout << "You've given a wrong output file. " << endl;
					exit(1);
				}
				else
				{
					cout << "File : " << argv[i+1] << " opened successfully!" << endl << endl;
				}

				i++;
			}
			else {
				cout << "You've given wrong input" <<endl;
				return -1;
			}
		}
	}
	//CASE: HAMMING


    inputFile.open("DataHamming.csv");
    queryFile.open("testHamming.txt");


    inputFile >> genericStr;    //read "@metric space"      //NOT NEEDED
    inputFile >> genericStr;	//read etc, "hamming"       //NOT NEEDED
    inputFile >> genericStr;	//read itemno
    inputFile >> genericStr;	//read data size

    dataLength = genericStr.length();

    if (k > dataLength) {
        cout << "Warning: LSH does not support so many mini-hashing functions. Switching to highest number available" << endl;
        k = dataLength;
    }

    cout << "The size of each hamming code is: " << dataLength <<endl;
    inputFile.clear();      //restart

    inputFile >> genericStr;    //read "@metric space"
    inputFile >> genericStr;	//read etc, "hamming"

    ListDataHamming<string>* hammingList = new ListDataHamming<string>();
    Node<string>* nodeHammingPtr = NULL;        //haming node pointer

    //HASTABLE CREATION
    Hash<string>* hashTableList = new Hash<string>[L];
    hashTableList->Hash::initHash(k, metric_space);
    int* miniHashIndex = new int[k];
    int currentIndex = 0;
    int hashResult = 0;

    //which mini-hashing functions should I choose?
    for (int i=0; i < k; i++) {
        //int r = Μ + (rand() / RAND_MAX + 1.0)*(N - M+1);        //generate uniform  [M, N]: we want k numbers from 1 to size of Hamming
        miniHashIndex[i] = (int)(1.0+ ((double)rand() / (double)RAND_MAX +1.0)*((double)k));
        cout << "miniHashIndex[" << i << "]: " << miniHashIndex[i] <<endl;

    }
    cin >> genericStr;      //to wait
    //LSH works this way for Hamming strings
    //we pick randomly k bits of the Hamming bitstring (k mini-hash h functions) and use the concatenation of those to find the bucket
    while (!inputFile.eof()){
        inputFile >> genericStr;	//item etc
        inputFile >> genericStr;	//data we want to store
        hammingList->Insert(genericStr);    //add on item list
        nodeHammingPtr = hammingList->ReturnHead();     //return the head of the list
        for (int i=0; i < k; i++) {
            currentIndex = miniHashIndex[i];        //current index regarding the Hamming string;
            hashResult += pow (2, i) * (genericStr[currentIndex] - '0');    //creates the binary as an int
            //cout << "The (unfinished) hash result: " << hashResult << "("<< pow(2, i)<< "-" << genericStr[currentIndex] - '0' <<")" << endl;
            //cin >>genericStr;
        }
        cout << hashResult <<endl;
        //cin >>genericStr;



        //int hdis = hammingList->Distance(myString, theCode);
        //cout << "------->  THE HAMMING DISTANCE IS : " << hdis << endl;
        hashResult = 0;
    }
    hammingList->PrintData();




/*
    ifstream read1;
    read1.open("DataHamming.csv");
	int choice;
	string myString;
	cout << "********************* Hamming space LSH testing ********************* " << endl << endl;

	ListDataHamming<string>* hammingList = new ListDataHamming<string>();
	cout << " 1 : Enter a 64-bit code. " << endl;
	cout << " 2 : Enter an input's file name." << endl << endl;
	cin >> choice;

	if (choice == 1)
	{
		string code;
		cout << "------->  Give a 64 bit code : " << endl << endl;
		cin >> code;
		if (code.size() != 64)
		{
			cout << "------->  You've given a wrong input. " << endl;
			exit(0);
		}
		read1 >> myString;  //read "@metric space"
		read1 >> myString;	//read etc, "hamming"

		while (!read1.eof())
		{
			read1 >> myString;	//item etc
			read1 >> myString;	//data we want to store
			hammingList->Insert(myString);
			int hdis = hammingList->Distance(myString, code);
			cout << endl;
			cout << "------->  THE HAMMING DISTANCE IS : " << hdis << endl;
		}
	}
	else if (choice == 2)
	{
		string theCode;
		string fileName;
		ifstream inputFile;
		cout << "Give an input's file name (+ extension) : " << endl << endl;
		cin >> fileName;
		inputFile.open(fileName.c_str());

		if (inputFile == NULL)
		{
			cout << "------->  You've given a wrong input file. " << endl;
			exit(1);
		}
		else
		{
			cout << "File : " << fileName << " opened successfully!" << endl << endl;
		}


		read1 >> myString;  //read "@metric space"
		read1 >> myString;	//read etc, "hamming"

		while (!inputFile.eof())
		{
			inputFile >> theCode;	//item etc
			inputFile >> theCode;	//data we want to compare

			while (!read1.eof())
			{
				read1 >> myString;	//item etc
				read1 >> myString;	//data we want to store
				hammingList->Insert(myString);
				int hdis = hammingList->Distance(myString, theCode);
				cout << "------->  THE HAMMING DISTANCE IS : " << hdis << endl;
			}

			read1.clear();
			read1.seekg(0, ios::beg);   //data file back from start
		}

		inputFile.close();
	}
	else
	{
		cout << "------->  You've given a wrong input. " << endl;
	}

	//hammingList->PrintData();

	delete hammingList;
	read1.close();
	cin.get();
	*/

	return 0;
}

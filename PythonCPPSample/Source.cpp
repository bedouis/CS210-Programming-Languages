#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <stdexcept> // for exception handling
#include <conio.h> // for _getch() function
#include <fstream> // to enable use of ifstream class
#include <algorithm>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printall");
Output:
	Python will print on the screen (item: frequency) from the input file
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python function you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("getfrequency", userInput);
Output:
	None
Return:
	frequency is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

int main()
{
	char userInput = '0';

	//while loop to display menu until user enters 4
	while (userInput != '4') {
		cout << "1: Display a list of all items purchased/frequency" << endl;
		cout << "2: Display how many times an item was purchased" << endl;
		cout << "3: Display a histogram with all items/frequency" << endl;
		cout << "4: Exit" << endl << endl;
		cout << "Enter your selection as a number 1, 2, 3 or 4: " << endl << endl;
		//exception handling constructs to handle any user input that is not 1, 2 or 3
		try {
			/* _getch() take user input without echo for a better user experience.
			Once the user enter a value they get directly to their menu choice without pressing 'enter'.
			_getch() has also the advantage over 'cin' to read non alphanumeric keys*/
			userInput = _getch();
			//switch statement to handle the different menu options
			switch (userInput) {

			case '1':
				//calls python printall function
				CallProcedure("printall");
				break;

			case '2':
			{
				string itemName;
				int itemFrequency;
				cout << endl << "Please enter an item's name: ";
				cin >> itemName;
				//make item name not case sensitive
				transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);

				// call python getfrequency function with user input as parameter
				itemFrequency = callIntFunc("getfrequency", itemName);
				//if user input is not matching any of the input file's items, display error message
				if (itemFrequency == 0) {
					cout << "Invalid item name. Please make another selection from the Menu" << endl << endl;
				}
				//if item was purchased only one time print a specific message.
				else if (itemFrequency == 1) {
					cout << itemName << " have been purchased " << itemFrequency << " time only" << endl << endl;
				}
				else {
					cout << itemName << " have been purchased " << itemFrequency << " times" << endl << endl;
				}
				break;
			}

			case '3':
			{
				// call python writefile function to create frequency.dat
				CallProcedure("writefile");
				ifstream frequencyFS;
				frequencyFS.open("frequency.dat");

				string item;
				int freq;

				if (!frequencyFS.is_open()) {
					cout << "Could not open file frequency.dat." << endl;
					return 1;
				}

				// read file lines and output item name and frequency as #
				cout << "Displaying a text-based histogram with all items/frequency" << endl << endl;
				while (frequencyFS >> item >> freq) {
					cout << item << " " << string(freq, '#') << endl;
				}
				frequencyFS.close();
				cout << endl;
				break;
			}
				//for this case a goodbye message is printed before exiting the while loop and the program	
			case '4':
				cout << "Thank you for using this program. Goodbye!" << endl;
				break;

				// for any key different than 1, 2, 3 or 4 throw an exception
			default:
				throw runtime_error("\n Invalid entry. Please enter 1, 2, 3 or 4 \n");
				break;
			}
		}
		//catch clause will execute if the user enter non(1,2,3,4) values
		catch (runtime_error& excpt) {
			// Prints the error message passed by throw statement
			cout << excpt.what() << endl;
		}
	}

	return 0;
}

#include "stdafx.h"
#include "menu.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
////////////////////////////////////////////////////////

struct AppUser {
	char login[32];
	char password[32];
	bool admin;
};

struct Employee {
	char lastname[25];
	char firstname[25];
	char department[15];
	char position[10];
	double salary;
};

///// users global variables
AppUser* users = 0;
int usersAmount = 0;
bool isAdmin = false;
////////////////////////////

////Employee global variables
Employee* employees = 0;
int employeesAmount = 0;
/////////////////////////////


int addUsersTest();
void printUsersMenu();
void printEmployeeViewMenu();
void printEmployeeDataMenu();
void printEmployeeEditMenu();

void printAdministratorMenu();
void printUsersManagementMenu();

void individualTask1();
void individualTask2();

void sortByDepartment();
void sortByLastName();
void sortBySalary();

void searchByLastName();
void searchByDepartment();
void searchBySpeciality();

void printEmployeesSearchMenu();
void printEmployeeHeader();
void printEmployeeHeaderWoId();

void deleteExistingUser();

AppUser* addNewUser(AppUser * userObj, const int amount);
AppUser* addAdminUser();
void setUserData(AppUser * userObj, const int amount);
void printUsers(const AppUser* userObj, const int amount);

const string userFileName = "users.bin";
const string employeeFileName = "employee.bin";
////////////////////////////////////////////////////////////////////////

void writeUsersToFile();
void readUsersFromFile();

void writeEmployeeToFile();
void readEmployeeFromFile();
void deleteEmployeeFile();

AppUser* authorization(char* login, char* password);
//////////////////////////////////////////////////////////////////////
Employee* addNewEmployee(Employee * employeeObj, int employeesAmount);
void setEmployeeData(Employee * employeeObj, const int employeesAmount);
void printEmployees();
void printEmployee(Employee e);
void deleteEmployee();
void editEmployee();
//////////////////////////////////////////////////////////////////////


int main()
{
	int invalidAttemptsCount = 0;
	readUsersFromFile();
	readEmployeeFromFile();

	if (users == nullptr) {
		addAdminUser();
		writeUsersToFile();
		cout << "File with first user was created; \n";
	}

	while (true)
	{
		system("cls");
		cout << "Authorization\n";
		cout << "Login: ";
		char login[32];
		cin.getline(login, 32);
		cout << "Password: ";
		char password[32];
		cin.getline(password, 32);


		AppUser *user = authorization(login, password);
		if (user != nullptr && strcmp(user->login, login) == 0)
		{
			if (user->admin)
			{
				system("cls");
				isAdmin = true;
				printAdministratorMenu();
			}
			else
			{
				system("cls");
				isAdmin = false;
				printUsersMenu();
			}
		}
		else
		{
			invalidAttemptsCount++;
			cout << "Error: Invalid credentials. "<< 3 - invalidAttemptsCount <<" attempts left..."<< endl;
			
			if (invalidAttemptsCount > 3)
			{
				cout << "Invalid user and password were entered 3 times.\n";
				break;
			}
			cin.ignore();
			cin.get();
		}
	}
	return 0;
}

AppUser* authorization(char* login, char* password)
{
	cout << "File with users exists... Trying to find a user with entered credentials...\n";

	for (int i = 0; i < usersAmount; i++)
	{
		if (strcmp(users[i].login, login) == 0 && strcmp(users[i].password, password) == 0)
		{
			return &users[i];
		}
	}
	return nullptr;
}

void printEmployeeHeader()
{
	int maxNameWitdh = 15;
	int maxPositionWitdh = 10;
	int maxDepartmentWitdh = 10;

	cout << "# | " << left << setw(maxNameWitdh) << setfill(' ') << "Firstname" << " | ";
	cout << left << setw(maxNameWitdh) << setfill(' ') << "Lastname" << " | ";
	cout << left << setw(maxPositionWitdh) << setfill(' ') << "Position" << " | ";
	cout << left << setw(maxDepartmentWitdh) << setfill(' ') << "Department" << " | ";
	cout << left << setw(10) << setfill(' ') << "Salary" << " | " << endl << endl;
}

void printEmployeeHeaderWoId()
{
	int maxNameWitdh = 15;
	int maxPositionWitdh = 10;
	int maxDepartmentWitdh = 10;

	cout << "|" << left << setw(maxNameWitdh) << setfill(' ') << "Firstname" << " | ";
	cout << left << setw(maxNameWitdh) << setfill(' ') << "Lastname" << " | ";
	cout << left << setw(maxPositionWitdh) << setfill(' ') << "Position" << " | ";
	cout << left << setw(maxDepartmentWitdh) << setfill(' ') << "Department" << " | ";
	cout << left << setw(10) << setfill(' ') << "Salary" << " | " << endl << endl;
}

void printEmployees()
{
	system("cls");
	printEmployeeHeader();
	for (int i = 0; i < employeesAmount; i++)
	{
		cout << i << " ";
		printEmployee(employees[i]);
	}
	cin.ignore();
	cin.get();
}

void printEmployee(Employee e)
{
	int maxNameWitdh = 15;
	int maxPositionWitdh = 10;
	int maxDepartmentWitdh = 10;
	////////
	//////aa
	////aaaa
	cout << " | " << left << setw(maxNameWitdh) << setfill(' ') << e.firstname << " | ";
	cout << left << setw(maxNameWitdh) << setfill(' ') << e.lastname << " | ";
	cout << left << setw(maxPositionWitdh) << setfill(' ') << e.position << " | ";
	cout << left << setw(maxDepartmentWitdh) << setfill(' ') << e.department << " | ";
	cout << left << setw(10) << setfill(' ') << e.salary << " | " << endl;
};


int addEmployeeTest()
{
	int continueEntering = 0; //  продолжить или остановить ввод данных
	do
	{
		employees = addNewEmployee(employees, employeesAmount);
		setEmployeeData(employees, employeesAmount);
		employeesAmount++;
		cout << "Do you want to continue to enter new employees (1 - yes, 0 - no): ";
		cin >> continueEntering;
		cin.get();
	} while (continueEntering != 0);
	return 0;
}

Employee* addNewEmployee(Employee * employeeObj, int employeesAmount)
{
	if (employeesAmount == 0)
	{
		employeeObj = new Employee[employeesAmount + 1]; // выделение памяти для первой структуры
	}
	else
	{
		Employee* tempEmployeeObj = new Employee[employeesAmount + 1];
		for (int i = 0; i < employeesAmount; i++)
		{
			tempEmployeeObj[i] = employeeObj[i]; // копируем во временный объект
		}
		delete[] employeeObj;
		employeeObj = tempEmployeeObj;
	}
	return employeeObj;
}

void setEmployeeData(Employee * employeeObj, const int employeesAmount)
{
	cin.ignore();
	cout << "Enter Firstname: ";
	cin.getline(employeeObj[employeesAmount].firstname, 25);
	cout << "Enter Lastname: ";
	cin.getline(employeeObj[employeesAmount].lastname, 25);
	cout << "Enter Position: ";
	cin.getline(employeeObj[employeesAmount].position, 15);
	cout << "Enter Department: ";
	cin.getline(employeeObj[employeesAmount].department, 32);
	cout << "Enter Salary: ";
	string mystr;
	getline(cin, mystr);
	(stringstream)mystr >> employeeObj[employeesAmount].salary;
	cout << endl;
}
////////////////////////////////////////////////////////////////////////////

int addUsersTest()
{
	int continueEntering = 0; //  продолжить или остановить ввод данных
	do
	{
		users = addNewUser(users, usersAmount);
		setUserData(users, usersAmount);
		usersAmount++;
		cout << "Do you want to continue to enter new users (1 - yes, 0 - no): ";
		cin >> continueEntering;
		cin.get();
	} while (continueEntering != 0);
	return 0;
}

AppUser* addNewUser(AppUser * userObj, const int amount)
{
	if (amount == 0)
	{
		userObj = new AppUser[amount + 1]; // выделение памяти для первой структуры
	}
	else
	{
		AppUser* tempUserObj = new AppUser[amount + 1];
		for (int i = 0; i < amount; i++)
		{
			tempUserObj[i] = userObj[i]; // копируем во временный объект
		}
		delete[] userObj;
		userObj = tempUserObj;
	}
	return userObj;
}

void setUserData(AppUser * userObj, const int amount)
{
	cin.ignore();
	cout << "Enter login: ";
	cin.getline(userObj[amount].login, 32);
	cout << "Enter password: ";
	cin.getline(userObj[amount].password, 32);
	userObj[amount].admin = false;
	cout << endl;
}


AppUser* addAdminUser()
{
	users = new AppUser[1];
	usersAmount = 1;
	strcpy_s(users[0].login, "root");
	users[0].admin = true;
	strcpy_s(users[0].password, "root");
	return users;
}

void printUsers(const AppUser* userObj, const int amount)
{
	cout << "№  " << "Login\t" << "Password\t" << "Admin\t" << endl;
	cout << "========================================" << endl;
	for (int i = 0; i < amount; i++)
	{
		cout << i + 1 << "  " << userObj[i].login << '\t' << "**********" << '\t' << userObj[i].admin << endl;
	}



	int choice = getchar();
	getchar();
}

void deleteExistingUser()
{
	printUsers(users, usersAmount);
	cin.clear();
	cout << "\n Which user do you want to delete?\n";
	char userToDelete[32];
	cin.getline(userToDelete, 32);
	bool userFound = false;
	int indDel = 0;
	for (int i = 0; i < usersAmount; i++)
	{
		if (strcmp(users[i].login, userToDelete) == 0)
		{
			userFound = true;
			indDel = i;
			break;
		}
	}
	if (userFound)
	{
		AppUser* tempUserObj = new AppUser[usersAmount - 1];
		int t = 0;
		int i = 0;
		while (i < usersAmount)
		{
			if (i != indDel)
			{
				tempUserObj[t] = users[i];
				t++;
				i++;
			}
			else {
				i++;
			}
		}
		delete[] users;
		usersAmount--;
		users = tempUserObj;
	}
}

/*
Employee file operations
*/

void writeEmployeeToFile()
{
	ofstream ofs("employee.bin", ios::binary);
	for (int i = 0; i < employeesAmount; i++)
	{
		Employee employee = employees[i];
		ofs.write((char *)&employee, sizeof(employee));
	}
	ofs.close();

	cin.ignore();
	cout << "Employees file was created successfully!!!!" << endl;
	cin.get();
}

void readEmployeeFromFile()
{
	try {
		ifstream ifs("employee.bin", ios::binary);
		if (!ifs.good()) {
			throw 1;
		}
		long size = 0;

		ifs.seekg(0, std::ios::end);
		size = ifs.tellg();
		if (size == -1)
		{
			return;
		}

		ifs.seekg(0, std::ios::beg);
		int employeesArraySize = size / sizeof(Employee);
		delete[] employees;
		employees = new Employee[employeesArraySize];
		employeesAmount = employeesArraySize;
		for (int i = 0; i < employeesArraySize; i++)
		{
			ifs.read((char *)&employees[i], sizeof(Employee));
		}
	}
	catch (int)
	{
		cout << "Error: File with employees doesn't exist!";
		cin.ignore();
		cin.get();
	}
}
////////////////////////////////////////////////////////////////////

void writeUsersToFile()
{
	ofstream ofs("users.bin", ios::binary);
	for (int i = 0; i < usersAmount; i++)
	{
		AppUser user = users[i];
		ofs.write((char *)&user, sizeof(user));
	}
	ofs.close();
	cin.ignore();
	cout << "Users file was created successfully;" << endl;
	cin.get();
}

void readUsersFromFile()
{
	try 
	{	
		ifstream ifs("users.bin", ios::binary);
		if (!ifs.good()) 
		{
			throw 1;
		}

		long size = 0;

		ifs.seekg(0, std::ios::end);
		size = ifs.tellg();
		if (size == -1)
		{
			return;
		}

		ifs.seekg(0, std::ios::beg);
		int usersArraySize = size / sizeof(AppUser);
		delete[] users;
		users = new AppUser[usersArraySize];
		usersAmount = usersArraySize;
		for (int i = 0; i < usersArraySize; i++)
		{
			ifs.read((char *)&users[i], sizeof(AppUser));
		}
	}
	catch (int)
	{
		cout << "Error: File with users doesn't exist!";
		cin.ignore();
		cin.get();
	}
}

void updateExistingUser()
{
	cout << "Not implemented yet";
}
void deleteEmployee()
{
	printEmployees();
	string indToDeleteStr;
	cout << "\n Which employee do you want to delete?\n";
	cin >> indToDeleteStr;
	try {
		int indDel = stoi(indToDeleteStr);
		if (indDel < 0 || indDel > employeesAmount - 1)
		{
			throw 1;
		}

		cin.ignore();
		while (true)
		{
			cout << "Are you sure to delete employee with id = " << indDel << endl;
			cout << "Press Y for confirmation, N for cancellation." << endl;
			char choice = getchar();
			if ('Y' == choice || 'y' == choice) {
				break;
			}
			if ('N' == choice || 'n' == choice)
			{
				return;
			}

		}

		Employee* tempObj = new Employee[employeesAmount - 1];
		int t = 0;
		int i = 0;
		while (i < employeesAmount)
		{
			if (i != indDel)
			{
				tempObj[t] = employees[i];
				t++;
				i++;
			}
			else {
				i++;
			}
		}

		delete[] employees;
		employeesAmount--;
		employees = tempObj;
	}
	catch (invalid_argument)
	{
		cout << "Error: Invalid index to delete!!! ";
	}
	catch (int)
	{
		cout << "Error: Index out of range!!! ";
	}

	cin.get();
}

void editEmployee()
{
	cout << "------Employee edit------\n";
	printEmployees();

	string indToEditStr;
	cout << "\n Which employee do you want to Edit?\n";
	cin >> indToEditStr;
	try {
		int indToEdit = stoi(indToEditStr);
		if (indToEdit < 0 || indToEdit > employeesAmount - 1)
		{
			throw 1;
		}

		cin.ignore();
		while (true)
		{
			cout << "Are you sure that you want to edit employee with id = " << indToEdit << endl;
			cout << "Press Y for confirmation, N for cancellation." << endl;
			char choice = getchar();
			if ('Y' == choice || 'y' == choice) {
				break;
			}
			if ('N' == choice || 'n' == choice)
			{
				return;
			}
		}

		cout << "Current employee data: " << endl;
		printEmployeeHeader();
		printEmployee(employees[indToEdit]);
		cout << "Enter new data:" << endl;
		setEmployeeData(employees, indToEdit);
	}
	catch (int)
	{
		cout << "Error: Incorrect ID is entered!!!";
	}
}

void deleteEmployeeFile()
{
	if (remove("employee.bin") != 0)
		cout<<"Error deleting file";
	else
		cout<<"File successfully deleted";
	cin.ignore();
	cin.get();
}

void printEmployeeViewMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Employee view------\n";
		cout << "1. Print list of all employees \n";
		cout << "2. Print list of all employees sorted by lastName \n";
		cout << "3. Print list of all employees sorted by salary \n";
		cout << "4. Print list of all employees sorted by department \n";

		cout << "0. Back \n";

		int choice = getchar();
		switch (choice)
		{
			case '1': printEmployees(); break;
			case '2': sortByLastName(); printEmployees(); break;
			case '3': sortBySalary(); printEmployees(); break;
			case '4': sortByDepartment(); printEmployees(); break;
			case '0': isAdmin ? printEmployeeDataMenu() : printUsersMenu();  break;
		}
	}
}

void printEmployeeEditMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Employee edit------\n";
		cout << "1. Add new employee \n";
		cout << "2. Delete employee \n";
		cout << "3. Edit employee \n";
		cout << "4. Read data from file \n";
		cout << "5. Write data to file \n";
		cout << "6. Delete file \n";
		cout << "0. Back \n";

		int choice = getchar();
		switch (choice)
		{
			case '1': addEmployeeTest(); break;
			case '2': deleteEmployee(); break;
			case '3': editEmployee(); break;
			case '4': readEmployeeFromFile(); break;
			case '5': writeEmployeeToFile(); break;
			case '6': deleteEmployeeFile(); break;
			case '0': printEmployeeDataMenu(); break;
		}
	}
}

void printEmployeeDataMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Employee data management------\n";
		cout << "1. View employee data \n";
		cout << "2. Search employee data \n";
		cout << "3. Edit employee data \n";
		cout << "4. Individual task 1 \n";
		cout << "5. Individual task 2 \n";
		cout << "0. Back \n";
		int choice = getchar();
		switch (choice)
		{
			case '1': printEmployeeViewMenu(); break;
			case '2': printEmployeesSearchMenu(); break;
			case '3': printEmployeeEditMenu(); break;
			case '4': individualTask1(); break;
			case '5': individualTask2(); break;
			case '0': printAdministratorMenu(); break;
		}
	}
}

void printEmployeesSearchMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Employee search------\n";
		cout << "1. Search by LastName \n";
		cout << "2. Search by Department \n";
		cout << "3. Search by Speciality \n";

		cout << "0. Exit \n";
		cin.clear();
		int choice = getchar();
		switch (choice)
		{
			case '1': searchByLastName(); break;
			case '2': searchByDepartment(); break;
			case '3': searchBySpeciality();  break;
			case '0': isAdmin ? printEmployeeDataMenu() : printUsersMenu();  break;
		}
	}
}


////SEARCH
void searchByLastName()
{
	system("cls");
	cout << "------Employee search by lastname------\n";
	cin.ignore();
	cout << "Enter lastname to search employee: ";
	char str[32];
	cin.getline(str, 32);

	cout << endl;

	bool isResultExist = false;

	try
	{
		for (int i = 0; i < employeesAmount; i++)
		{
			if (strcmp(employees[i].lastname, str) == 0)
			{

				if (!isResultExist)
				{
					printEmployeeHeader();
					isResultExist = true;
				}
				cout << left << i;
				printEmployee(employees[i]);
				isResultExist = true;
			}
		}
		if (!isResultExist)
		{
			throw 1;
		}
	}
	catch (int)
	{
		cout << "Error: Empty search result" << endl;
		getchar();
	}
	cin.clear();
	getchar();
}


void searchByDepartment()
{
	system("cls");
	cout << "------Employee search by department------\n";
	cin.ignore();
	cout << "Enter department to search employee: ";
	char str[32];
	cin.getline(str, 32);
	cout << endl;
	bool isResultExist = false;
	try
	{
		
		for (int i = 0; i < employeesAmount; i++)
		{
			if (strcmp(employees[i].department, str) == 0)
			{
				if (!isResultExist)
				{
					printEmployeeHeader();
					isResultExist = true;
				}
				printEmployee(employees[i]);
			}
		}
		if (!isResultExist)
		{
			throw 1;
		}
	}
	catch (int)
	{
		cout << "Error: Empty search result" << endl;
		getchar();
	}
	cin.clear();
	getchar();
}

void searchBySpeciality()
{
	system("cls");
	cout << "------Employee search by speciality------\n";
	cin.ignore();
	cout << "Enter speciality to search employee: ";
	char str[32];
	cin.getline(str, 32);
	cout << endl;
	bool isResultExist = false;
	try
	{

		for (int i = 0; i < employeesAmount; i++)
		{
			if (strcmp(employees[i].position, str) == 0)
			{
				if (!isResultExist)
				{
					printEmployeeHeader();
					isResultExist = true;
				}
				printEmployee(employees[i]);
			}
		}
		if (!isResultExist)
		{
			throw 1;
		}
	}
	catch (int)
	{
		cout << "Error: Empty search result" << endl;
		getchar();
	}
	cin.clear();
	getchar();

}

////END SEARCH

void printUsersMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Employee data menu for User------\n";
		cout << "1. Employee View \n";
		cout << "2. Employee Search \n";
		cout << "3. Individual task 1 \n";
		cout << "4. Individual task 2 \n";
		cout << "0. Exit \n";
		int choice = getchar();
		switch (choice)
		{
			case '1': printEmployeeViewMenu(); break;
			case '2': printEmployeesSearchMenu(); break;
			case '3': individualTask1(); break;
			case '4': individualTask2(); break;
			case '0': exit(0);  break;
		}
	}
}


void individualTask1()
{
	system("cls");
	
	cout << "Individual task \n";
	sortByDepartment();
	char * currentDepartment = 0;
	double sumByDepartment = 0;

	currentDepartment = employees[0].department;
	int i = 0;
	printEmployeeHeader();

	while (i < employeesAmount)
	{
		int empCountForDep = 0;
		while (strcmp(currentDepartment, employees[i].department) == 0)
		{
			sumByDepartment = sumByDepartment + employees[i].salary;
			printEmployee(employees[i]);
			i++;
			empCountForDep++;
		}
		cout << endl;
		cout << "Summary for department " << currentDepartment << ":" << sumByDepartment << endl;
		cout << "Average for department " << currentDepartment << ":" << sumByDepartment / empCountForDep << endl << endl;
		currentDepartment = employees[i].department;
		sumByDepartment = 0;
	}
	//system("pause");
	cin.ignore();
	cin.get();
}

void individualTask2()
{
	system("cls");
	cin.ignore();
	cout << "Enter the salary amount: " << endl;
	string str;
	cin >> str;
	try
	{
		double amount = stod(str);
		cout << "\nEmployees with salary less than " << amount << endl;
		printEmployeeHeader();
		for (int i = 0; i < employeesAmount; i++)
		{
			if (employees[i].salary < amount)
			{
				printEmployee(employees[i]);
			}
		}
	}
	catch (invalid_argument)
	{
		cout << "Error: Incorrect input." << endl;
		cin.get();
	}
	cin.ignore();
	cin.get();
}


void sortByDepartment()
{
	Employee temp;
	for (int i = 0; i < employeesAmount - 1; i++)
	{
		for (int j = i + 1; j < employeesAmount; j++)
		{
			if (strcmp(employees[i].department, employees[j].department) > 0)  //comparing departments
			{
				temp = employees[i];    //swapping entire struct
				employees[i] = employees[j];
				employees[j] = temp;
			}
		}
	}
	return;
}

void sortByLastName()
{
	Employee temp;
	for (int i = 0; i < employeesAmount - 1; i++)
	{
		for (int j = i + 1; j < employeesAmount; j++)
		{
			if (strcmp(employees[i].lastname, employees[j].lastname) > 0)  //comparing
			{
				temp = employees[i];    //swapping entire struct
				employees[i] = employees[j];
				employees[j] = temp;
			}
		}
	}
	return;
}

void sortBySalary()
{
	Employee temp;
	for (int i = 0; i < employeesAmount - 1; i++)
	{
		for (int j = i + 1; j < employeesAmount; j++)
		{
			if (employees[i].salary > employees[j].salary)  //comparing
			{
				temp = employees[i];    //swapping entire struct
				employees[i] = employees[j];
				employees[j] = temp;
			}
		}
	}
	return;
}

void printUsersManagementMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Users management menu------\n";
		cout << "1. List users \n";
		cout << "2. Add new user \n";
		cout << "3. Delete existing user \n";
		cout << "4. Write to file \n";
		cout << "5. Read from file \n";
		cout << "0. Exit \n";
		int choice = getchar();
		switch (choice)
		{
			case '1': printUsers(users, usersAmount); break;
			case '2': addUsersTest(); break;
			case '3': deleteExistingUser(); break;
			case '4': writeUsersToFile(); break;
			case '5': readUsersFromFile(); break;
			case '0': printAdministratorMenu(); break;
		}
	}
}

void printAdministratorMenu()
{
	while (true)
	{
		system("cls");
		cout << "------Administrator's menu------\n";
		cout << "1. User's management \n";
		cout << "2. Employees management \n";
		cout << "0. Exit \n";
		int choice = getchar();
		switch (choice)
		{
			case '1': printUsersManagementMenu(); break;
			case '2': printEmployeeDataMenu(); break;
			case '0': exit(0);  break;
		}
	}
}

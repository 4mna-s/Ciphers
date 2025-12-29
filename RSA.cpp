#include <iostream>
#include <fstream>
#include <ctime>

//Function Declarations:
int division(int, int);
int modulusFinder(int, int);
int prime();
int gcdFinder(int, int);
int privateKey(int, int);
int publicKey(int);
int power(int, int, int);
void Encryption(char[], int, int, int);
void Decryption(int, int, int);


using namespace std;

int main()
{
	srand(time(0));

	cout << "\n--------R-S-A--------\n";

	int p = prime();
	int q = prime();
	cout << "\nPrime (p): " << p;
	cout << "\nPrime (q): " << q;

	int n = p * q;
	int n2 = (p - 1) * (q - 1);
	cout << "\nn: " << n;
	cout << "\nphi n: " << n2;
	
	int e = publicKey(n2);
	int d = privateKey(e, n2);
	cout << "\nPublic Key (e): " << e;
	cout << "\nPrivate Key (d):" << d;

	char PlainText[100];
	cout << "Enter Plaintext to Encrypt:\n";
	cin.getline(PlainText, 100);

	Encryption(PlainText, 100, e, n);

	cout << "Encryption Complete! \nCheck\'EncryptedText.txt\' file";
	cout << "\n---------------------\n";

	Decryption(d, p, q);
	cout << "Decryption Complete! \nCheck\'DecryptedText.txt\' file";
	cout << "\n---------------------\n";


}

//defining a function for division
int division(int a, int b)
{
	int n = 0;
	while (a >= b)
	{
		a = a - b;
		n++;
	}
	return n;
}

//defining a function for finding modulus
int modulusFinder(int a, int b)
{
	int q = division(a, b);
	int r = a - q * b;
	return r;
}

//Finding gcd using recrusive function
int gcdFinder(int a, int b)
{
	if(b==0)
		return a;

	return (gcdFinder(b, modulusFinder(a, b)));
}


//Private Key Generator
int privateKey(int e, int n2)
{
	for(int d=1; d<n2; d++)
	{
		if(modulusFinder(d*e, n2)==1)
			return d;
	}

	return 3;	//iska kuch aur socho bro (failsafe for now)
}

//Public Key Generator
int publicKey(int n2)
{
	//finds co-prime of n2
	for(int i=2; i<n2; i++)
	{
		if(gcdFinder(i, n2)==1)
			return i;
	}
	
	return 1;
}



//defining a function that generates a random prime number less than 50
int prime()
{
	//prime numbers between 1 and 50
	int primes[15] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
	int n = modulusFinder(rand(),15);	
	
	return primes[n];
}

int modInverse(int a, int m)
{
	for(int i=1; i<m; i++)
	{
		if(modulusFinder(a*i, m) == 1)
			return i;
	}
	return 1;
}


//modular exponention (power function)
int power(int base, int exp, int mod)
{
	long long result = 1;
	long long b = modulusFinder(base, mod);

	while(exp > 0)
	{
		if(modulusFinder(exp, 2)==1) 
			result = modulusFinder(result*b, mod);

		exp = division(exp, 2);

		b = modulusFinder(b*b, mod);
	}

	return (int)result;

}


//Encryption Function
void Encryption(char PlainText[], int size, int e, int n)
{
	int i;

	//opening a file to write in
	ofstream fout;
	fout.open("EncryptedText.txt");

	//checking if the file opened successfully
	if(!fout.is_open())
	{
		cout << "Failed to open File: \"EncryptedText.txt\"";
		return;
	}

	//The actual encruption:
	for(i=0; PlainText[i]!='\0' ; i++)
	{
		int M = (int)PlainText[i];

		if(M < 0)
			M += 256;
		
		int C = power(M, e, n);

		//writing in the file
		fout << C << " ";
	}
	fout.close();

	return;
}

void Decryption(int d, int p, int q)
{
	ifstream fin("EncryptedText.txt");
	ofstream fout("DecryptedText.txt");


	//checking if the files opened successfully
	if(!fin.is_open())
	{
		cout << "Failed to open File: \"EncryptedText.txt\"";
		return;
	}
	if(!fout.is_open())
	{
		cout << "Failed to open File: \"DecryptedText.txt\"";
		return;
	}

	int n = p * q;

	int dp = modulusFinder(d, p-1);
	int dq = modulusFinder(d, q-1);
	int qInverse = modInverse(q, p);

	int cipher;

	while(fin >> cipher)
	{
		//m1 = c^dp mod p
		int m1 = power(modulusFinder(cipher, p), dp, p);

		//m2 = c^dq mod q
		int m2 = power(modulusFinder(cipher, q), dq, q);

		//h = qInverse * (m1 - m2) mod p
		int diff = m1 - m2;
		if(diff < 0)
			diff += p;
		int h = modulusFinder(qInverse * diff, p);

		//m = m2 + h * q
		int m = m2 + h * q;
		m = modulusFinder(m, n);

		char decryptedChar = (char)m;
		fout << decryptedChar;
	}

	fin.close();
	fout.close();

	return;
}


#include <iostream>
#include <fstream>
#include <random>
#include <string>
using namespace std;

string generate_random_ip() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(0, 255);

	return to_string(dist(gen)) + "." +
	       to_string(dist(gen)) + "." +
	       to_string(dist(gen)) + "." +
	       to_string(dist(gen));
}

int main() {
	ofstream fout("ips.txt");
	int total = 1000000;

	for(int i = 0; i < total; i++) {
		fout << generate_random_ip() << "\n";
	}

	fout.close();
	cout << "Generated 1 million IP addresses in ips.txt\n";
	return 0;
}

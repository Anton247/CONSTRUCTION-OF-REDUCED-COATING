#include <windows.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;

struct FD//Functional dependence
{
	string left;
	string right;
	FD(const string& _l, const string& _r) : left(_l), right(_r) {};
};

void ReadFile(string& name, string& X, vector<FD>& F);
void SX(const string& X, const vector<FD>& F, string& X_plus);
bool PRF(const FD& X_struct, const vector<FD>& F);
void NPOK(const vector<FD>& F, vector<FD>& G);
void LRED(const vector<FD>& F, vector<FD>& Fl);
void PRED(const vector<FD>& Fl, vector<FD>& G);
void LRED_for_key(const vector<FD>& F, vector<FD>& Fl);
void PRED_for_key(const vector<FD>& Fl, vector<FD>& G);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string file("Input.txt");
	string X;
	vector<FD> F;
	vector<FD> G;
	ReadFile(file, X, F);
	NPOK(F, G);
	cout << "Неизбыточное покрытие\n";
	if (G.size() == 0)
		cout << "0\n";
	for (int i = 0; i < G.size(); i++)
	{
		if (G[i].left == "")
			cout << "0";
		else
			cout << G[i].left;
		cout << "->";
		if (G[i].right == "")
			cout << "0";
		else
			cout << G[i].right << endl;
	}
	vector<FD> Fl;
	vector<FD> Fr;
	cout << endl;
	LRED(G, Fl);
	cout << "\nЛевое редуцирование\n";
	for (int i = 0; i < Fl.size(); i++)
	{
		if (Fl[i].left == "")
			cout << "0";
		else
			cout << Fl[i].left;
		cout << "->";
		if (Fl[i].right == "")
			cout << "0";
		else
			cout << Fl[i].right << endl;
	}
	PRED(Fl, Fr);
	cout << "\nПравое редуцирование\n";
	cout << endl;
	for (int i = 0; i < Fr.size(); i++)
	{
		if (Fr[i].left == "")
			cout << "0";
		else
			cout << Fr[i].left;
		cout << "->";
		if (Fr[i].right == "")
			cout << "0";
		else
			cout << Fr[i].right << endl;
	}

	vector<FD> Key_p = Fr;
	vector<FD> Key;
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].left = X;
	Key_p[Key_p.size() - 1].right = "&";

	LRED_for_key(Key_p, Key);
	cout << "\nКлюч: " << Key[Key.size() - 1].left << endl;

	/*
	Key_p = Fr;
	Key.clear();
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].left = "&";
	Key_p[Key_p.size() - 1].right = X;

	PRED_for_key(Key_p, Key);
	cout << "\nPKey: " << Key[Key.size() - 1].right<< endl;
	*/

	system("pause");
	return 0;
}

void ReadFile(string& name, string& X, vector<FD>& F)
{
	ifstream In(name);
	if (!In) {
		cout << "Файл не найден!";
		exit(-1);
	}
	In >> X; //считываем множество атрибутов до \n
	X.shrink_to_fit();
	FD Func("", "");
	string input_1 = "";
	string input_2 = "";
	char ch = 0;
	cout << "Считал из файла:\n";
	while (!In.eof()) {
		input_1 = "";
		input_2 = "";
		ch = 0;
		while (true) {
			In >> ch;
			if (ch == '-')
				break;
			input_1 += ch;
			if (ch == '0' and In.eof()) {
				input_1.clear();
				break;
			}
		}
		if (input_1 == "")
			cout << '0';
		else
			cout << input_1 << '-';
		if (!In.eof()) {
			In >> ch;
			cout << ch;
			In >> input_2;
			cout << input_2;
		}
		else {
			input_2.clear();
			In >> ch;
		}
		if (input_1 == "0")
			input_1.clear();
		if (input_2 == "0")
			input_2.clear();

		F.push_back(FD(input_1, input_2));
		cout << "\n";
	}
	for (int i = 0; i < F.size(); i++) {
		F[i].left.shrink_to_fit();
		F[i].right.shrink_to_fit();
	}
	F.shrink_to_fit();
	cout << "\n\n";
}

void SX(const string& X, const vector<FD>& F, string& X_plus)
{
	string OLD = "";
	string NEW = X;
	string Q = "";
	string A;
	string B;
	string C;
	int k = 0;
	while (NEW != OLD || (NEW == "" && k < F.size()))
	{
		Q = "";
		OLD = NEW;
		k++;
		for (int i = 0; i < F.size(); i++)
		{
			A = NEW;
			B = F[i].left;
			C = F[i].right;
			sort(A.begin(), A.end());
			sort(B.begin(), B.end());
			sort(C.begin(), C.end());
			if (includes(A.begin(), A.end(), B.begin(), B.end()))//строка содержит
			{
				Q.clear();
				set_union(A.begin(), A.end(), C.begin(), C.end(), std::back_inserter(Q));//объединене множеств
				NEW = Q;
			}
		}
	}
	X_plus = NEW;
}

bool PRF(const FD& X_struct, const vector<FD>& F)
{
	string X_plus;
	string X = X_struct.left;
	SX(X, F, X_plus);
	if (std::includes(X_plus.begin(), X_plus.end(), X_struct.right.begin(), X_struct.right.end()))//строка содержит
		return true;
	else
		return false;
}

void NPOK(const vector<FD>& F, vector<FD>& G)
{
	G = F;
	vector<FD> G_minus = F;
	for (int i = 0; i < F.size(); i++)
	{
		G_minus = G;
		int k = 0;
		while (true)
		{
			if (k < G_minus.size())
			{
				if (F[i].left == G_minus[k].left && F[i].right == G_minus[k].right)
					G_minus.erase(G_minus.begin() + k);
				else
					k++;
			}
			else
				break;
		}
		if (PRF(F[i], G_minus))
			G = G_minus;
	}
}

void LRED(const vector<FD>& F, vector<FD>& Fl)
{
	Fl = F;
	for (int i = 0; i < F.size(); i++)
	{
		for (int j = 0; j < F[i].left.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = F[i].left;
			string A;
			A = F[i].left[j];
			n = S.find(A);
			S.erase(S.begin() + n);
			FD X(S, A); //(X\A)->A
			if (PRF(X, Fl))
			{
				n = Fl[i].left.find(A);
				Fl[i].left.erase(Fl[i].left.begin() + n);
			}
		}
	}
}

void LRED_for_key(const vector<FD>& F, vector<FD>& Fl)
{
	Fl = F;
	for (int i = 0; i < F.size(); i++)
	{
		for (int j = 0; j < Fl[i].left.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = Fl[i].left;
			string A;
			A = Fl[i].left[j];
			n = S.find(A);
			S.erase(S.begin() + n);
			FD X(S, A); //(X\A)->A
			if (PRF(X, Fl))
			{
				n = Fl[i].left.find(A);
				Fl[i].left.erase(Fl[i].left.begin() + n);
				j--;
			}
		}
	}
}

void PRED(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < Fl.size(); i++)
	{
		for (int j = 0; j < Fl[i].right.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = Fl[i].right;
			string A;
			A = Fl[i].right[j];
			n = S.find(A);
			S.erase(S.begin() + n);

			G_minus = G;
			int k = 0;
			while (true)
			{
				if (k < G_minus.size())
				{
					if (Fl[i].left == G_minus[k].left && Fl[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			FD X(Fl[i].left, A); //(X->A)
			FD Xn(Fl[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus))
			{
				n = G[i].right.find(A);
				G[i].right.erase(G[i].right.begin() + n);
				//j--;//чтобы после удаления не перескочить
			}
		}
	}
	for (int i = 0; i < G.size(); i++)
		if (G[i].right == "")
		{
			G.erase(G.begin() + i);
			i--;
		}
}

void PRED_for_key(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].right.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = G[i].right;
			string A;
			A = G[i].right[j];
			n = S.find(A);
			S.erase(S.begin() + n);

			G_minus = G;
			int k = 0;
			while (true)
			{
				if (k < G_minus.size())
				{
					if (G[i].left == G_minus[k].left && G[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			FD X(G[i].left, A); //(X->A)
			FD Xn(G[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus))
			{
				n = G[i].right.find(A);
				G[i].right.erase(G[i].right.begin() + n);
				j--;//чтобы после удаления не перескочить
			}
		}
	}
	for (int i = 0; i < G.size(); i++)
		if (G[i].right == "")
		{
			G.erase(G.begin() + i);
			i--;
		}
}

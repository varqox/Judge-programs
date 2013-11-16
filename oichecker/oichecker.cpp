#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include <cmath>
#include "checker.hpp"
#include "gen.hpp"

using namespace std;

#ifdef WIN32

void buffer(string &w)
{
	w="";
	char z=cin.get();
	while(z!='\n')
	{
		w+=z;
		z=cin.get();
	}
}

#else

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

bool kbhit()
{
	struct termios term, oterm;
	int fd=0;
	int c=0;
	tcgetattr(fd, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag=term.c_lflag & (!ICANON);
	term.c_cc[VMIN]=0;
	tcsetattr(fd, TCSANOW, &term);
	c=getchar();
	tcsetattr(fd, TCSANOW, &oterm);
	if(c!=-1)
	ungetc(c, stdin);
return (c!=-1 ? true:false);
}

int getch()
{
	static int ch = -1, fd = 0;
	struct termios neu, alt;
	fd = fileno(stdin);
	tcgetattr(fd, &alt);
	neu = alt;
	neu.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(fd, TCSANOW, &neu);
	ch = getchar();
	tcsetattr(fd, TCSANOW, &alt);
	return ch;
}

int getmaxx()
{
	int x=0;
	#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	x=ts.ts_cols;
	#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	x=ts.ws_col;
	#endif /* TIOCGSIZE */
return x;
}

int getmaxy()
{
	int y=0;
	#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	y=ts.ts_lines;
	#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	y=ts.ws_row;
	#endif /* TIOCGSIZE */
return y;
}

void buffer(string& w)
{
	static vector<string> name_base;
	vector<string> v=name_base;
	int poz=0, qy=name_base.size();
	string k;
	name_base.push_back(k);
	char KEY_UP[]={27, '[', 'A', '\0'}, KEY_DOWN[]={27, '[', 'B', '\0'}, KEY_RIGHT[]={27, '[', 'C', '\0'}, KEY_LEFT[]={27, '[', 'D', '\0'}, DELETE[]={27, '[', 51, 126, '\0'}, BACKSPACE[]={127, '\0'};
	int width=getmaxx();
	char z=getch();
	while(true)
	{
		k="";
		k+=z;
		while(kbhit())
		{
			if(z=='\n') break;
			z=getch();
			k+=z;
		}
		if(k==BACKSPACE)
		{
			if(poz>0)
			{
				name_base[qy].erase(poz-1,1);
				/******* KEY_LEFT *******/
				if(poz%width==0)
				{
					cout << KEY_UP;
					for(int i=0; i<width; ++i)
					{
						cout << KEY_RIGHT;
					}
				}
				else cout << KEY_LEFT;
				--poz;
				/******* END *******/
				int r=poz;
				for(; r<signed(name_base[qy].size()); ++r)
					cout << name_base[qy][r];
				++r;
				cout << ' ';
				if(r%width==0) cout << "\033[E"; // move cursor 1 line down at begin
				for(; r>poz; --r)
				{
					if(r%width==0)
					{
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
				}
			}
			cout.flush();
		}
		else if(k==DELETE)
		{
			if(poz<signed(name_base[qy].size()))
			{
				name_base[qy].erase(poz,1);
				int r=poz;
				for(; r<signed(name_base[qy].size()); ++r)
					cout << name_base[qy][r];
				++r;
				cout << ' ';
				if(r%width==0){cout << "\033[E";cout.flush();} // move cursor 1 line down at begin
				for(; r>poz; --r)
				{
					if(r%width==0)
					{
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
				}
			}
			cout.flush();
		}
		else if(k==KEY_UP)
		{
			if(qy>0)
			{
				--qy;
				while(poz>0)
				{
					cout << ' ';
					if(poz+1%width==0)
					{
						cout << ' ' << KEY_LEFT;
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
					if(poz%width==0)
					{
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
					--poz;
				}
				poz=name_base[qy].size();
				cout << name_base[qy];
				if(poz%width==0) cout << ' ' << KEY_LEFT;
			}
			cout.flush();
		}
		else if(k==KEY_DOWN)
		{
			if(qy<signed(name_base.size())-1)
			{
				++qy;
				while(poz>0)
				{
					cout << ' ';
					if(poz+1%width==0)
					{
						cout << ' ' << KEY_LEFT;
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
					if(poz%width==0)
					{
						cout << KEY_UP;
						for(int i=0; i<width; ++i)
						{
							cout << KEY_RIGHT;
						}
					}
					else cout << KEY_LEFT;
					--poz;
				}
				poz=name_base[qy].size();
				cout << name_base[qy];
				if(poz%width==0) cout << ' ' << KEY_LEFT;
			}
			cout.flush();
		}
		else if(k==KEY_LEFT)
		{
			if(poz>0)
			{
				if(poz%width==0)
				{
					cout << KEY_UP;
					for(int i=0; i<width; ++i)
					{
						cout << KEY_RIGHT;
					}
				}
				else cout << KEY_LEFT;
				--poz;
			}
			cout.flush();
		}
		else if(k==KEY_RIGHT)
		{
			if(poz<signed(name_base[qy].size()))
			{
				if(poz%width-width==-1) cout << "\033[E"; // move cursor 1 line down at begin
				else cout << KEY_RIGHT;
				++poz;
			}
			cout.flush();
		}
		else
		{
			if(k[k.size()-1]=='\n')
			{
				k.erase(k.size()-1, 1);
				name_base[qy].insert(poz,k);
				break;
			}
			int r=poz;
			name_base[qy].insert(poz,k);
			for(; r<signed(name_base[qy].size()); ++r)
				cout << name_base[qy][r];
			if(r%width==0) cout << ' ' << KEY_LEFT;
			for(; r>signed(k.size())+poz; --r)
			{
				if(r%width==0)
				{
					cout << KEY_UP;
					for(int i=0; i<width; ++i)
					{
						cout << KEY_RIGHT;
					}
				}
				else cout << KEY_LEFT;
			}
			poz+=k.size();
			cout.flush();
			//cout << k;
		}
		z=getch();
	}
	while(poz<signed(name_base[qy].size()))
	{
		cout << name_base[qy][poz];
		// if(poz%width-width==-1) cout << "\033[E"; // move cursor 1 line down at begin
		// else cout << KEY_RIGHT;
		++poz;
	}
	cout << endl;
	if(qy<signed(name_base.size())-1) name_base.pop_back();
	w=name_base[qy];
	if(!w.empty() && (v.empty() || v.back()!=w)) v.push_back(w);
	name_base.swap(v);
}

#endif

void get_command(vector<string>& que)
{
	cout << "oichecker:$ ";cout.flush();
	que.clear();
	string in, tmp;
	buffer(in);
	for(int i=0, size=in.size(); i<size; ++i)
	{
		if(in[i]==' ')
		{
			if(!tmp.empty()) que.push_back(tmp);
			tmp="";
		}
		else tmp+=in[i];
	}
	if(!tmp.empty()) que.push_back(tmp);
}

int main()
{
	ios_base::sync_with_stdio(false);
	vector<string> command;
	get_command(command);
	while(!(command.size()>0 && command.front()=="exit"))
	{
		if(command.size()==0)
		{
			get_command(command);
			continue;
		}
		const char **comm=new const char*[command.size()];
		for(int i=0; i<command.size(); ++i)
			comm[i]=command[i].c_str();
	#ifdef WIN32
		if(command[0]=="help") cout << "Polecenia:\n   help - wy" << static_cast<char>(-104) << "wietla ten opis\n   exit - wyj" << static_cast<char>(-104) << "cie\n   spr <nazwa_zadania> <exec> [nazwy test" << static_cast<char>(-94) << "w...] - ocenia program exec na testach w folderze tests/nazwa_zadania, mo" << static_cast<char>(-66) << "na poda" << static_cast<char>(-122) << " nazwy test" << static_cast<char>(-94) << "w na kt" << static_cast<char>(-94) << "rych program ma by" << static_cast<char>(-122) << " oceniony, w przypadku nie podania " << static_cast<char>(-66) << "adnych test" << static_cast<char>(-94) << "w program sprawdzi rozwi" << static_cast<char>(-91) << "zanie na wszystkich testach znajduj" << static_cast<char>(-91) << "cych si" << static_cast<char>(-87) << " w danym katalogu\n   sprklo - specjalna sprawdzaczka do zadania KLO - u" << static_cast<char>(-66) << "ywa si" << static_cast<char>(-87) << " jak spr\n   gen <nazwa zadania> <liczba test" << static_cast<char>(-94) << "w> [dodatkowe opcje...] - generuje losowe testy do podanego zadania, dost" << static_cast<char>(-87) << "pne zadania: HOT, BAR, KUR, dodatkowe opcje to np. ograniczenie dla n... (parz przyk" << static_cast<char>(-120) << "ady)\n\nPrzyk" << static_cast<char>(-120) << "ady:\n   spr hot hot - ocenia rozwi" << static_cast<char>(-91) << "zanie w pliku wykonywalnym hot na wszystkich testach w katalogu test/hot/\n   spr hot-big prog 1 2 5 - ocenia rozwi" << static_cast<char>(-91) << "zanie w pliku wykonywalnym prog na testach 1, 2, 5 w katalogu test/hot-big/\n   gen HOT 1037 - generuje 1037 test" << static_cast<char>(-94) << "w do zadania HOT (do katalogu tests/hot)\n   gen kur 17 n<100 m<=20 - generuje 17 test" << static_cast<char>(-94) << "w do zadania kur z ograniczeniami n < 100 oraz m <= 20 (do katalogu tests/kur)\n\nInformacje o generowanych testach:\n1. Testy s" << static_cast<char>(-91) << " generowane do katalog" << static_cast<char>(-94) << "w odpowiadaj" << static_cast<char>(-91) << "cych nazwom zada" << static_cast<char>(-28) << " (ma" << static_cast<char>(-120) << "e litery) np. tests/bar, tests/kur.\n2. Dodatkowe opcje mog" << static_cast<char>(-91) << " zawiera" << static_cast<char>(-122) << " tylko ograniczenia na niekt" << static_cast<char>(-94) << "re sta" << static_cast<char>(-120) << "e w zadaniach w postaci [nazwa zmiennej][symbol][liczba], ca" << static_cast<char>(-120) << "o" << static_cast<char>(-104) << static_cast<char>(-122) << " nie mo" << static_cast<char>(-66) << "e zawiera" << static_cast<char>(-122) << " spacji, dost" << static_cast<char>(-87) << "pne symbole: '<', '<=', '='. Dost" << static_cast<char>(-87) << "pne nazwy zmiennych do zadania:\nHOT - n\nKUR - n, m\nBAR - n\nWAZ - n\nLiczba musi nale" << static_cast<char>(-66) << "e" << static_cast<char>(-122) << " do przedzia" << static_cast<char>(-120) << "u okre" << static_cast<char>(-104) << "lonego w zadaniu.\n3. Domy" << static_cast<char>(-104) << "lne ograniczenia dla generatora w zadaniach:\nHOT: 0 < n <= 5000\nKUR: 0 < n <= 500 000; 0 < m <= 500 000\nBAR: 0 < n <= 10 000" << endl;
	#else
		if(command[0]=="help") cout << "Polecenia:\n   help - wyświetla ten opis\n   exit - wyjście\n   spr <nazwa_zadania> <exec> [nazwy testów...] - ocenia program exec na testach w folderze tests/nazwa_zadania, można podać nazwy testów na których program ma być oceniony, w przypadku nie podania żadnych testów program sprawdzi rozwiązanie na wszystkich testach znajdujących się w danym katalogu\n   sprklo - specjalna sprawdzaczka do zadania KLO - używa się jak spr\n   gen <nazwa zadania> <liczba testów> [dodatkowe opcje...] - generuje losowe testy do podanego zadania, dostępne zadania: HOT, BAR, KUR, dodatkowe opcje to np. ograniczenie dla n... (parz przykłady)\n\nPrzykłady:\n   spr hot hot - ocenia rozwiązanie w pliku wykonywalnym hot na wszystkich testach w katalogu test/hot/\n   spr hot-big prog 1 2 5 - ocenia rozwiązanie w pliku wykonywalnym prog na testach 1, 2, 5 w katalogu test/hot-big/\n   gen HOT 1037 - generuje 1037 testów do zadania HOT (do katalogu tests/hot)\n   gen kur 17 n<100 m<=20 - generuje 17 testów do zadania kur z ograniczeniami n < 100 oraz m <= 20 (do katalogu tests/kur)\n\nInformacje o generowanych testach:\n1. Testy są generowane do katalogów odpowiadających nazwom zadań (małe litery) np. tests/bar, tests/kur.\n2. Dodatkowe opcje mogą zawierać tylko ograniczenia na niektóre stałe w zadaniach w postaci [nazwa zmiennej][symbol][liczba], całość nie może zawierać spacji, dostępne symbole: '<', '<=', '='. Dostępne nazwy zmiennych do zadania:\nHOT - n\nKUR - n, m\nBAR - n\nWAZ - n\nLiczba musi należeć do przedziału określonego w zadaniu.\n3. Domyślne ograniczenia dla generatora w zadaniach:\nHOT: 0 < n <= 5000\nKUR: 0 < n <= 500 000; 0 < m <= 500 000\nBAR: 0 < n <= 10 000" << endl;
	#endif
		else if(command[0]=="spr") checker::main(command.size(), comm);
		else if(command[0]=="sprklo") checker::sprKLO(command.size(), comm);
		else if(command[0]=="gen") gen::main(command.size(), comm);
		else
		{
			string comm=command[0];
			for(int i=1; i<command.size(); ++i)
			{
				comm+=' ';
				comm+=command[i];
			}
			system(comm.c_str());
		}
		delete[] comm;
		get_command(command);
	}
return 0;
}
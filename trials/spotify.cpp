#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

//using namespace std;


/*

I've built this little program just to take some rust off my coding skills
since it's been ages from the last time I coded something.

The purpose of it is to organize my music, so I've built the classes
'song' and 'playlist' with some methods which could come in handy - maybe -
such as printing on file the list of songs in the playlist and so on.
I'm not sure that std::vector is the right choice to store the songs in the 
playlist (as private member). Would a std::set be more efficient?

Given a list of songs I wanted to sort out if each and every
one of the songs in the list appeared at least in one playlist,
with each playlist representing a musical genre.

I was also interested in listing the "overlaps" between two different
playlists, to get a sort of Venn Diagram of my music. I've accomplished 
that by overloading the operators + and - for the playlist, and by
defining a function "intersection".

*/

class song {
private:
	std::string title;
	std::string author;
	std::string album;
public:
	/*song(std::string tit) : title(tit), author("? "), album("? ") { }
	song(std::string tit, std::string au, std::string al) : title(tit), author(au), album(al) { }
	*/

	explicit song(std::string title, std::string author = "? ", std::string album = "? ") 
		: title(std::move(title)),
		  author(std::move(author)),
		  album(std::move(album))
	{ }

	std::string gettitle() const {return title;}
	std::string getauthor() const {return author;}
	std::string getalbum() const {return album;}

	bool operator==(song & a) const {
		/*if(title == a.gettitle()) {
			if(author == a.getauthor()) {
				if(album == a.getalbum()) {
					return true;
				}
			}
		}else{ return false; }*/
		return title == a.title
			&& author == a.author
			&& album == a.album;
		 
	}

	void playsong() { 
		std::cout << std::endl;
		std::cout << "\"" << title << "\"" << ", by " << author << ", \n";
		std::cout << "from the album \"" << album << "\"." << std::endl;
	}
	void showtitle() { std::cout << title << std::endl; }
	void showauthor() { std::cout << author << std::endl; }
	void showalbum() { std::cout << album << std::endl; }
	void showall() { 
		std::cout << title << " | " << author;
		std::cout << " | " << album << std::endl;
	}
	
};

class playlist {
private:
	std::string name;
	std::vector<song> list;
public:
	playlist() : name(" ") {}
	playlist(std::string nam) : name(nam) { }
	playlist(playlist & x, std::string nam) 
		: name(nam),
		 list(x.list)
	{ }

	std::vector<song> getlist() const { return list; }
	std::string getname() const { return name; }
	//song getsong(std::vector<song>::iterator it) const { return *it; } 
	std::vector<song>::iterator getsong(song & x) {
		std::vector<song>::iterator it;
		it = list.begin();
		while (it != list.end()) {
			if(*(it) == x) break;
			it ++;
		}
		
		return it;
	}

	/*playlist& operator=(const playlist & x) {
		//list.erase(list.begin(), list.end());
		list = x.getlist();
		name = x.getname();
		return *this;
	}*/

	const playlist operator+( const playlist & x) {
		playlist z(*this, name + "+" + x.name);
		/*for(int i=0; i<(x.getlist()).size(); i++) {	
			if (false == this->findsong((x.getlist())[i])){ 
				z.addsong((x.getlist())[i]);
			}
		}*/
		for(auto& song : x.list) {
			if (!findsong(song)) {
				z.addsong(song);
			}
		}
		return z;
	}

	const playlist operator-(const playlist & x) {
		playlist z(*this);
		for(int i=0; i<(x.getlist()).size(); i++) {
			if (this->findsong((x.getlist())[i])){ 
				z.deletesong((x.getlist())[i]);
			}
		}
		z.setname(this->getname() + "-" + x.getname());
		return z;
	}
	
	void addsong(song x) { list.push_back(x); }
	void setname(std::string nam) { name = nam; }
	void addlist(std::string file) {
		std::ifstream in(file);
		std::string line;
		while(std::getline(in, line)) {

			std::stringstream linestream(line);
			std::string tit, au, al;
			getline(linestream, tit, '\t');
			getline(linestream, au, '\t');
			getline(linestream, al);

			song x(tit, au, al);		
			this->addsong(x);	
		}
		in.close();
	}

	void deletesong(const song& x) {
		/*auto it = list.begin();
		while(it != list.end()) {
			if(*it == x){
				it = list.erase(it);
			} else {
				++it;
		}
		*/
		list.erase(std::remove(list.begin(), list.end(), x), list.end());
	}
	
	bool findsong(song x) {
		/*bool tobe = false;
		for (auto el : list) {
			if(el == x) tobe = true;
		}
		return tobe;
		*/
		return std::find(list.begin(), list.end(), x) != list.end();
	}

	void showplaylist() {
		int cont=0;
		std::vector<song>::iterator it;
		it = list.begin();
		std::cout << std::endl << name << std::endl << std::endl;
		while (it != list.end()) {
			cont++;
			std::cout << cont << ") ";
			it->showall();
			it++;
		}
	}
	
	void playplaylist() {
		int cont=0;
		std::vector<song>::iterator it;
		it = list.begin();
		std::cout << std::endl << name << std::endl << std::endl;
		while (it != list.end()) {
			cont++;
			std::cout << cont << ") ";
			it->playsong();
			it++;
		}
	}

	std::ofstream printplaylist(std::string nam) {
		std::ofstream out(nam + ".dat");
		std::vector<song>::iterator it;
		it = list.begin();
		out << std::endl << name << std::endl << std::endl;
		while (it != list.end()) {
			out << it->gettitle() << "\t";
			out << it->getauthor() << "\t";
			out << it->getalbum() << std::endl;
			
			it++;
		}
		out.close();
		return out;
	}
};

playlist intersection(playlist & x, playlist & y) {
	playlist z, q, t;
	z = x + y;
	t = x - y;
	q = y - x; 
	z = z - q;
	z = z - t;
	z.setname("(" + x.getname() + ")^(" + y.getname() + ")" );
	return z;
}

int main() {

	playlist tutte("tutte");
	playlist ottobre("1910");
	playlist settembre("1909");

	tutte.addlist("tutte.dat");
	ottobre.addlist("1910.dat");
	settembre.addlist("1909.dat");
	

//	tutte.playplaylist();
	ottobre.playplaylist();
	settembre.playplaylist();

	playlist autunno("autunno");
	autunno = (ottobre + settembre); 
	autunno.playplaylist();
	//autunno = (autunno - settembre);

	(intersection(autunno, ottobre)).playplaylist();

//	tutte.printplaylist("prova.dat");



return 0;
}







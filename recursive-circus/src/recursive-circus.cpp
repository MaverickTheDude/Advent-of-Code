#include "rec_circ.h"

using std::cout;
using std::endl;


int main() {
	using set_pair_it = set_pair::const_iterator;
	using set_prog_it = std::unordered_set<program>::iterator;

// unordered_set<customowej_klasy> wymaga zdefiniowania:
// operator() dla funkcji hash() oraz operator==
	std::unordered_set<program> bucket;
	std::vector<tier> tree;
	std::string line;
	std::ifstream myfile("input.txt");

	// baza indukcyjna
	getline(myfile, line);
	tree.emplace_back(line);

	// przepisz input do kontenera
	while ( getline(myfile, line) )
		bucket.emplace(line);


	// Zbuduj drzewo od korzenia do lisci
	int level = 1;
	while(bucket.size() > 0) {
		tier next_tier;
		set_pair ch_names = tree[level-1].child_names();
// biezace pietro
		bool complete = false;
		while (!complete) {
// dla kazdego elementu w wiadrze...
			for (set_prog_it pit = bucket.begin(); pit != bucket.end(); ) {
				const std::string& entry = pit->name;
				bool pit_valid = true;
// ... sprawdz wszystkich kandydatow (dzieci)
				for (set_pair_it it = ch_names.begin(); it != ch_names.end(); ) {
					if (it->first.compare(entry) == 0) {
						const program* parent = it->second;
						next_tier.programs.emplace(*pit, parent);
						pit = bucket.erase(pit);
						it = ch_names.erase(it);
						pit_valid = false;
						break;
					}
					else
						++it;
				}
				if (pit_valid)
					++pit;
				if (ch_names.size() == 0) {
					tree.push_back(next_tier);
					complete = true;
					cout << "pietro " << level++ << " zapelnione." << endl;
					break;
				}
			}
		}
		cout << "cale wiadro przetoczone. Zawiera " << bucket.size() << " elementow" << endl;
	}

	int i = 0;
	for (auto x : tree)
		cout << "pietro " << i++ << " zawiera: "  << x.programs.size() << " programow" << endl;

	// Nie uzywamy petli z lenistwa ^^
	std::map<const program*, std::pair<int, int>> x  = tree[tree.size()-1].finalize();
	std::map<const program*, std::pair<int, int>> x2 = tree[tree.size()-2].finalize(x);
	std::map<const program*, std::pair<int, int>> x3 = tree[tree.size()-3].finalize(x2);

	return 0;
}


//					   parent                weight N_wystapien
using map = std::map<const program*, std::pair<int, int>>;
map tier::finalize() const {
	map tier_map;
	map::iterator it;
	// Dla kazdego programu na pietrze przypisz wskaznik na rodzica (klucz) oraz pare (waga, N_wystapien)
	for (const program& x : programs) {
		it = tier_map.find(x.parent);
		if (it == tier_map.end())  // parent not found
			tier_map[x.parent] = std::make_pair(x.weight, 1);  // Dodaj pare (waga, N_wystapien) do klucza program*
		else {
			int ref_weight = it->second.first;
			assert(ref_weight == x.weight);  // Blad znajduje sie w drugim przeciazeniu funkcji
			it->second.second += 1;
		}
	}
	return tier_map;
}

map tier::finalize(const map& upper_map) const {
	map tier_map;
	map::iterator it;
	for (const program& x : programs) {
		int weight;
		const program* main_key = nullptr;

//  uwzgledniamy wage programow powyzej
		map::const_iterator upper_it = upper_map.find(&x);
		if (upper_it == upper_map.end())  // Lisc grafu na nizszym pietrze
			weight = x.weight;
		else
			weight = upper_it->second.first * upper_it->second.second + x.weight;

//  dopisz program do mapy...
		it = tier_map.find(x.parent);
		if (it == tier_map.end())  // parent not found
			tier_map[x.parent] = std::make_pair(weight, 1);
//  ...albo porownaj z tymi wpisanymi
		else {
			/* it->second       -- wartosc mapy dla danego rodzica (para waga-N_wyst)
			 * it->second.first -- waga wpisana przy pierwszym wpisaniu do mapy */
			int ref_weight = it->second.first;

//	sprawdzamy czy elementy grafu sa zrownowazone
			// Na wypadek, gdyby bledne bylo pierwsze wystapienie (okazalo sie ostatnie)
			if (main_key != nullptr && it->first == main_key) {
				cout << "trzeci element znaleziony!" << endl;
				cout << "waga w slowniku: " << ref_weight << endl;
				cout << "waga biezaca: " << weight << endl;
				cout << "program: " << x.name << " o wadze: " << x.weight << endl;
			}
			if (ref_weight != weight) {
				cout << "blad znaleziony!" << endl;
				cout << "waga w slowniku: " << ref_weight << endl;
				cout << "waga biezaca: " << weight << endl;
				cout << "program: " << x.name << " o wadze: " << x.weight << endl;
				main_key = it->first;
				cout << "liczba braci w mapie: " << it->second.second+1 << endl; // 3 braci w mapie, nasz element jest czwarty
				cout << "Odpowiedz: " << x.weight + (ref_weight - weight) << endl;
			}
			it->second.second += 1;
		}
	}
	return tier_map;
}

set_pair tier::child_names() const {
	set_pair ch_names;

	/* Lekcja:
	 * 1) foreach musi przegladac program& (referencje), zeby names() zwrocilo
	 *    'this' do WLASCIWEGO obiektu program, a nie tymczasowego.
	 * 2) Poniewaz type(x) = const program&, funkcja name() musi obiecac
	 *    niezmienienie elementow x (const na koncu)
	 * 3) This w funkcji names musi miec kwalifikator const
	 *    (zatem drugi typ w std::pair tez tak musi miec)*/
	for (const program& x : programs) {
		set_pair cur_names = x.names();
		ch_names.insert(cur_names.begin(), cur_names.end());
	}
	return ch_names;
}

/* (c) https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ */
// fail: delimiter moze byc tylko jednym znakiem...
/*std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}*/

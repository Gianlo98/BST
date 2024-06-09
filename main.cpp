#include <iostream>
#include <assert.h>
#include <string>
#include "BST.hpp"


using namespace std;

const string LEFT_DIVIDER = "-----------------------------[ ";
const string RIGHT_DIVIDER = " ]-----------------------------";


struct only_even_pred{ //Predicato che torna true solo se l'intero passato è pari
	bool operator()(int a){
		return a % 2 == 0;
	}
};


void test_bst_int(){
	cout << LEFT_DIVIDER << "BST di int (classe int e confronto default)" << RIGHT_DIVIDER << endl;
	cout << "Questi test verificano la corretta esecuzione di un albero di interi di 15 elementi (vedi schema relazione)." << endl;
	cout << LEFT_DIVIDER << "Test creazione BST di interi" << endl;
	BST<int> bst; //Test costruttore di default

	assert(bst.size() == 0);
	assert(bst.height() == 0);

	cout << "Passato!" << endl;

	cout << LEFT_DIVIDER << "Test popolazione BST di interi" << endl;
	bst.add_node(13);
	assert(bst.size() == 1);
	assert(bst.height() == 0);
	assert(bst.contains(13));

	bst.add_node(14);
	bst.add_node(18);
	bst.add_node(3);
	bst.add_node(4);
	bst.add_node(12);
	bst.add_node(1);
	bst.add_node(10);
	bst.add_node(11);
	bst.add_node(2);
	bst.add_node(5);
	bst.add_node(8);
	bst.add_node(7);
	bst.add_node(9);
	bst.add_node(6);

	assert(bst.size() == 15);
	assert(bst.height() == 8);
	assert(bst.contains(10));
	cout << "Passato!" << endl;

	cout << LEFT_DIVIDER << "Test inserimento duplicato BST di interi" << endl;
	try{
		bst.add_node(9);
		abort();
	}catch(bst_duplicated_value_exception& exception){
		cout << "Passato!" << endl;
	}

	cout << LEFT_DIVIDER << "Test stampa BST di interi" << endl;
	cout << bst << endl;
	cout << "Sizee: " << bst.size() << endl;
	cout << "Height: " << bst.height() << endl;

	cout << LEFT_DIVIDER << "Test iteratori BST di interi" << endl;
	
	BST<int>::const_iterator it = bst.begin(), itd = bst.end();
	cout << "Iteratore inizio: " << *it << endl;
	cout << "Iteratore fine: " << *itd << endl;

	do{
		cout << *it << endl;
		++it;
	}while(it != itd);
	cout << *it << endl;

	it = bst.begin();
	while(*it != 10 && it != itd){
		it++;
	}

	cout << LEFT_DIVIDER << "Test subtree BST di interi" << endl;
	BST<int> sub = bst.subtree(10);
	BST<int>::const_iterator its = sub.begin(), itsd = sub.end();

	cout << sub << endl;
	it = sub.begin();
	itd = sub.end();
	for (; its != itsd; ++its) {
		cout << *its << endl;
	}
	cout << *its << endl;

	its = sub.begin();
	while(*its != 10 && its != itsd){
		its++;
	}

	assert(it != itsd); //L'albero sub deve tornare una copia e non puntare agli stessi nodi
	cout << "Passato!" << endl;


	cout << LEFT_DIVIDER << "Test printIF BST di interi pari" << endl;
	struct only_even_pred pred;

	printIF(bst, pred);
	
	cout << endl;
}

/**
 @brief Struttura custom usata nei test
 */
struct record{ //Struttura di test
	string key;
	double value;

	record(const string& k,const double val) : key(k), value(val){ }
};

/**
 @brief Funtore custom per la comparaziione della struttura record usata nei test
 @param record a
 @param record b
 @return 0 se la differenza fra i due valori in valore assoluto è < 1, 1 se a > b, -1 altrimenti. 
 */
struct record_custom_comparison{
	short operator()(const record& a, const record& b) const{
		if(static_cast<int>(a.value - b.value) == 0){
			return 0;
		}
		return a.value > b.value ? 1 : -1;
	}
};

/**
 @brief Predicato da usare con la struttura custom nei test. Torna tutti i record con lunghezza stringa > 3 e < 7.
 @param record un record.
 @return true se la lunghezza della chiave del record è > 3 e < 7 false altrimenti.
 */
struct string_contains_pred{
	bool operator()(const record& a){
		string::size_type sz = a.key.size();
		return sz > 3 && sz < 7;
	}
};

ostream& operator<<(std::ostream& os, const record& r) {
	os << "[" << r.key << " : " << r.value << "]";
	return os;
}

void test_bst_custom_class(){
	cout << LEFT_DIVIDER << "BST di record (classe e confronto custom)"<< RIGHT_DIVIDER << endl;
	cout << "Questi test verificano la corretta esecuzione di un albero con una classe custom e con un operatore di confronto custom. Le stringhe rappresentano l'ordine di esecuzione mentre i double sono i valori dei nodi." << endl;
	cout << LEFT_DIVIDER << "Test creazione BST di record" << endl;
	BST<record, record_custom_comparison> bst(record("primo", 6.7));

	assert(bst.size() == 1);
	assert(bst.height() == 0);

	cout << "Passato!" << endl;


	cout << LEFT_DIVIDER << "Test popolazione BST di record" << endl;
	bst.add_node(record("secondo", 4.5));
	bst.add_node(record("terzo", 5.6));
	bst.add_node(record("quarto", 8.9));
	bst.add_node(record("quinto", 9.9));
	bst.add_node(record("sesto", 2.3));
	bst.add_node(record("settimo", 3.4));
	bst.add_node(record("ottavo", 1.2));
	bst.add_node(record("nono", 7.8));


	assert(bst.size() == 9);
	assert(bst.height() == 3);
	assert(bst.contains(record("TEST OPERAZIONE CONFRONTO", 5.9)));
	assert(bst.contains(record("TEST OPERAZIONE CONFRONTO", 4))); //La differenza tra il nodo 4 e il nodo 4.5 è troncata a 0 perciò li considero UGUALI
	cout << "Passato!" << endl;

	cout << LEFT_DIVIDER << "Test inserimento duplicato BST di record" << endl;
	try{
		bst.add_node(record("TEST OPERATORE CONFRONTO", 8.1));
		abort();
	}catch(bst_duplicated_value_exception& exception){
		cout << "Passato!" << endl;
	}

	cout << LEFT_DIVIDER << "Test stampa BST di record" << endl;
	cout << bst << endl;
	cout << "Sizee: " << bst.size() << endl;
	cout << "Height: " << bst.height() << endl;

	cout << LEFT_DIVIDER << "Test iteratori BST di record" << endl;
	
	BST<record, record_custom_comparison>::const_iterator it = bst.begin(), itd = bst.end();
	cout << "Iteratore inizio: " << *it << endl;
	cout << "Iteratore fine: " << *itd << endl;

	do{
		cout << *it << endl;
		++it;
	}while(it != itd);
	cout << *it << endl;

	it = bst.begin();
	while(it->value != 8.1 && it != itd){
		it++;
	}

	cout << LEFT_DIVIDER << "Test subtree BST di record" << endl;
	BST<record, record_custom_comparison> sub = bst.subtree(record("TEST OPERATORE CONFRONTO", 8.1));
	BST<record, record_custom_comparison>::const_iterator its = sub.begin(), itsd = sub.end();

	cout << sub << endl;
	it = sub.begin();
	itd = sub.end();
	for (; its != itsd; ++its) {
		cout << *its << endl;
	}
	cout << *its << endl;

	its = sub.begin();
	while(its->value != 8.1 && its != itsd){
		its++;
	}

	assert(it != itsd); //L'albero sub deve tornare una copia e non puntare agli stessi nodi
	cout << "Passato!" << endl;


	cout << LEFT_DIVIDER << "Test printIF BST con lunghezza stringa > 3 e < 7" << endl;
	string_contains_pred pred;

	printIF(bst, pred);
}	

void test_bst_method(){
	cout << LEFT_DIVIDER << "Test metodi BST" << RIGHT_DIVIDER << endl;
	
	cout << "Questi test verificano la corretta esecuzione di tutti i metodi implementati nella classe BST." << endl;
	
	cout << LEFT_DIVIDER << "Test Costruttori" << endl;
	BST<int> bst; //test costruttore default
	assert(bst.size() == 0);
	bst.add_node(13);
	bst.add_node(3);
	bst.add_node(14);
	assert(bst.size() == 3);
	
	BST<short> bsts(3); //test costruttore secondario
	assert(bsts.size() == 1);
	
	BST<int> bsti(bst); //test costruttore copia
	assert(bsti.size() == 3);
	
	bsti.add_node(2);
	assert(bst.size() == 3);
	assert(bsti.size() == 4);
	cout << "Passato!" << endl;
	
	cout << LEFT_DIVIDER << "Test Clear" << endl;
	bsti.clear(); //Test clear
	assert(bsti.size() == 0);
	cout << "Passato!" << endl;
	
	cout << LEFT_DIVIDER << "Test Operator=" << endl;
	bsti = bst; //Test operatore =
	assert(bsti.size() == 3);
	cout << "Passato!" << endl;
	
	cout << LEFT_DIVIDER << "Test Contains" << endl;
	assert(!bsti.contains(133)); //test contains
	assert(bsti.contains(13));
	cout << "Passato! " << endl;
	
	cout << LEFT_DIVIDER << "Test Iteratori" << endl;
	BST<int>::const_iterator it = bsti.begin(); //test iteraotore begin
	BST<int>::const_iterator itd = bsti.end(); //test iteraotore end
	assert(*it == 3);
	assert(*itd == 14);
	cout << "Passato! " << endl;
	
	cout << LEFT_DIVIDER << "Test add_node" << endl;
	bsti.add_node(1);
	bsti.add_node(2);
	bsti.add_node(4);
	bsti.add_node(12);
	try{
		bsti.add_node(1);
		assert(false);
	}catch(bst_duplicated_value_exception &exc){
		cout << "Passato! " << endl;
	}

	cout << LEFT_DIVIDER << "Test Subtree" << endl;
	BST<int> sub = bsti.subtree(3); //test subtree
	assert(sub.height() == 2);
	assert(sub.size() == 5);
	
	BST<int> subd = bsti.subtree(5900); //test subtree con radice inesistente
	assert(subd.size()== 0);
	cout << "Passato!" << endl;
	
	cout << LEFT_DIVIDER << "Test Height" << endl;
	assert(bsti.height() == 3); //Test height;
	cout << "Passato!" << endl ;
	
	cout << LEFT_DIVIDER << "Test Stampa" << endl;
	cout << bsti << endl;
	cout << subd << endl; //Vuoto non stampa nulla 
}

/**
	@brief Metodo main per i test.
	@author Gianlorenzo Occhipinti 829524
 */
int main(){
	try{
		test_bst_method();
		test_bst_int();
		test_bst_custom_class();
		cout << LEFT_DIVIDER << "Tutti i test hanno dato esito positivo" << RIGHT_DIVIDER << endl;
		return 0;
	} catch (const std::exception& exc){
		std::cerr << exc.what();
	}
}

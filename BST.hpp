#ifndef BST_H
#define BST_H

#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t
#include <iostream>
#include <string>
#include <stdexcept> //std::runtime_error

/**
 @brief Operatore di confronto di default
 @author Gianlorenzo Occhipinti 829524
 
 Questa struttura verrà usata dalla classe BST come funtore di confronto se non ne viene passato uno in fase di dichiarazione.
 
 @param x primo operando
 @param y secondo operando
 @return Uno short positivo, negativo o zero se ripettivamente x è maggiore, minore o uguale a y.
 **/
template <typename T>
struct bst_default_comparison_operator{
	short operator() (const T& x, const T& y) const {
		if(x == y){
			return 0;
		}else{
			return x > y ? 1 : -1;
		}
	}
};

/**
 @brief Eccezione custom di valore duplicato
 @author Gianlorenzo Occhipinti 829524
 @exception <bst_duplicated_value_exception> { Eccezione lanciata in presenza di un inseriimento di valore duplicato }
 */
class bst_duplicated_value_exception : public std::runtime_error{
	public:
	bst_duplicated_value_exception(const std::string& message);
};


/**
 @brief Classe che rappresenta un BST
 @author Gianlorenzo Occhipinti 829524
 @param T tipi di dato
 @param C funtore di confronto di due dati
 
 Questa classe rappresensta un BST con datti di tipo T. Il funtore di confronto C deve tornare un intero negativo, positivo o uguale  a zero se il primo operando del confronto è minore, maggiore o uguale al secondo.
 */
template <typename T, typename C = bst_default_comparison_operator<T> >
class BST{
	
	/**
	 Struttura che rappresenta un generico nod dell'albero.
	 
	 @brief Nodo dell'albero
	 */
	struct node{
		node* left; 	//puntatore al nodo sinistro
		node* right;	//puntatore al nodo destro
		node* root;		//puntatore al nodo padre
		T value;		//dato del nodo
		
		/**
			Costruttore di default
		 */
		node() : left(nullptr), right(nullptr), root(nullptr) { }
		
		/**
			Costruttore secondario per inizializzare un nodo dell'albero
			@param p_value valore del nodo
		 */
		node(const T& p_value) : left(nullptr), right(nullptr), root(nullptr), value(p_value) { }
		
		/**
			Costruttore secondario per inizializzare un nodo dell'albero
			@param p_value valore del nodo
			@param p_root puntatore al padre del nodo
		 */
		node(const T& p_value, node* p_root) : left(nullptr), right(nullptr), root(p_root), value(p_value) { }
		
		/**
			Distruttore
		 */
		~node(void){
			left = nullptr;
			right = nullptr;
			root = nullptr;
		}
		
	};
	
	/**
	 	Funzione di supporto per effettuare la copia ricorsiva di un albero.
		@param p puntatore al nodo dell'albero da cui si sta effettuando la copia
	 */
	void recursive_copy(const node* p){
		if(p != nullptr){
			add_node(p->value);
			recursive_copy(p->left);
			recursive_copy(p->right);
		}
	}
	
	/**
	 Funzione di supporto per distruggere  tutti i nodi dell'albero
	 @param p puntatore al nodo dell'albero che si sta distruggendo
	 */
	void clear_helper(node* n){
		if(n != nullptr){
			clear_helper(n->left);
			clear_helper(n->right);
			delete n;
			m_size--;
			n = nullptr;
		}
	}
	
	/**
	 Metodo di supporto che torna il puntatore al nodo che contiene il valore
	 
	 @param val il valore da cercare
	 @return il puntatore al nodo che contiene il valore, nullptr altrimenti.
	 */
	node* find_node(const T& val) const {
		node* point = m_root;
		
		while(point != nullptr && m_comp(val, point->value) != 0){
			point = (m_comp(val, point->value) > 0) ? point->right : point->left;
		}
		
		return point;
	}
	
	/**
	 Metodo aggiuntivo che torna l'altezza dell'albero
	 
	 @param n puntatore al  nodo da cui calcolare l'altezza
	 @return l'altezza dell'albero dal nodo n
	 */
	unsigned int height(const node* n) const{
		if(n == nullptr){
			return 0;
		}else{
			unsigned int l = height(n->left);
			unsigned int r = height(n->right);
			return ((l > r) ? l : r) + 1;
		}
	}
	
	node* m_root; 				/// root dell'albero
	unsigned int m_size;		/// dimensione dell'albero
	C m_comp;					/// oggetto funtore per confronto
	
public:
	
	/**
		@brief Costruttore di default
	 */
	BST() : m_root(nullptr), m_size(0) { }
	
	/**
		@brief Costruttore secondario
		@param t_root_val valore del nodo radice
	 */
	BST(const T& t_root_val) : m_size(0), m_root(nullptr){
		add_node(t_root_val);
	}
	
	/**
		 @brief Costruttore di copia
		 
		 @param other albrero da copiare
		 @throw eccezione di allocazione di memoria
	 */
	BST(const BST& other) : m_size(0), m_root(nullptr) {
		try{
			recursive_copy(other.m_root);
		}catch(...) {
			clear();
			throw;
		}
	}
	
	/**
		@brief Distruttore
	 */
	~BST(void){
		clear();
	}
	
	/**
	 @brief cancella tutti i nodi dell'albero
	 */
	void clear(){
		clear_helper(m_root);
		m_root = nullptr;
	}
	
	/**
		 @brief Operatore di assegnamento
		 
		 @param other albero da copiare
		 @return reference a this
		 
		 @throw eccezione di allocazione di memoria (dal copy constructor)
	 */
	BST& operator= (const BST& other){
		if(&other != this){
			BST tmp(other);
			std::swap(m_root,tmp.m_root);
			std::swap(m_size,tmp.m_size);
		}
		return *this;
	}
	
	/**
		 @brief Aggiunta di un altro nodo
		 Inserisce un elemento nell'albero secondo l'operatore di confronto istanziato.
		 
		 @param p_value valore da inserire
		 @throw eccezione inserimento valore duplicato
		 @throw eccezione di allocazione di memoria
	 */
	void add_node(const T& p_value){
		node* node = new struct node(p_value);
		
		if(m_root == nullptr){
			m_root = node;
			m_size++;
			return;
		}
		
		struct node* previous = m_root;
		struct node* current = m_root;
		int diff = 0;
		
		do {
			diff = m_comp(current->value, p_value);
			previous = current;
			
			if(diff > 0){
				current = current->left;
			}else if(diff < 0){
				current = current->right;
			}else if(diff == 0){
				delete node;
				node = nullptr;
				throw bst_duplicated_value_exception("Errore di inserimento dati duplicato");
			}
		}while(current != nullptr); /// navigo l'albero finchè non trovo la posizione giusta
		
		node->root = previous; 		/// collego il figlio al padre
		
		if(diff > 0){				/// collego il padre al figlio
			previous->left = node;
		}else{
			previous->right = node;
		}
		m_size++;
	}

	/**
		 @brief dimensione dell'albero
		 Ritorna il numero totale di dati inseriti nell'albero (1)
		 
		 @return numero di dati inseriti
	 */
	unsigned int size() const {
		return m_size;
	}
	
	/**
		 @brief controlla se un elemento si trova nell'albero
		 Controlla se esiste un elemento T nell'albero (2)
		 
		 @param p_element elemento da cercare
		 
		 @return true se esiste l'elemento, false altrimenti.
	 */
	bool contains(const T& p_element) const{
		node* node_p = m_root;
		int diff = 0;
		
		do{
			diff = m_comp(p_element, node_p->value);
			node_p = (diff > 0) ? node_p->right : node_p->left;
		}while(node_p != nullptr && diff != 0);
		return diff==0; //Se diff== 0 sono uscito dal while perchè ho trovato 2 valori uguali, se è != 0 è perchè node_p = nullptr
	}
	
	/**
	 @brief Iteratore costante dell'albero
	 @class const_iterator
	 
	 Iteratore di sola lettura di tipo forward per l'accesso ai dati presenti nell'albero. (3)
	 
	 */
	class const_iterator{
		
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
		typedef const T*                  pointer;
		typedef const T&                  reference;
		
		/**
			Costruttore di default
		 */
		const_iterator() : m_point(nullptr) { }
		
		/**
			Copy constructor
		 */
		const_iterator(const const_iterator& other) : m_point(other.m_point) { }
		 
		/**
		 	Operatore assegnamento
		 */
		const_iterator& operator= (const const_iterator& other){
			if(*this != other){
				m_point = other.m_point;
			}
			return *this;
		}
		
		/**
			Disitruttore
		 */
		~const_iterator() {
			m_point = nullptr;
		}
		
		/**
		 	Operatore di derefenziamento
		 */
		reference operator* () const { return m_point->value; }
		
		/**
			Operatore puntatore
		 */
		pointer operator-> () const { return &(m_point->value); }
	
		/**
			Operatore post-incremento
		 */
		const_iterator operator++ (int) {
			const_iterator tmp(*this);
			move_to_next_node();
			return tmp;
		}
		
		/**
		  Operatore pre-incremento
		 */
		const_iterator& operator++ () {
			move_to_next_node();
			return *this;
		}
		
		/**
			Operatore uguaglianza
		 */
		bool operator== (const const_iterator& other) {
			return m_point == other.m_point;
		}
		
		/**
		 Operatore disuguaglianza
		 */
		bool operator!= (const const_iterator& other) {
			return m_point != other.m_point;
		}
		
	private:
		const node* m_point;
		friend class BST;
		
		/**
			Costruttore privato dell'iteratore costante (può essere usato da BST perchè messa come friend)
			@param p_n puntatore al nodo da cui partire
		 */
		const_iterator(const node* p_n) : m_point(p_n) { }
		
		/**
			Funzione per trovare il nodo successivo seguendo una visita in-order dell'albero.
		 */
		void move_to_next_node(){
			if(m_point->right == nullptr){ 			///Nodo senza dx
				const node* from;
				do{
					from = m_point;
					m_point = m_point->root;
				} while(from != m_point->left);		 /// Mi fermo solo se salgo da sinistra
			}else{ 									/// Vado sempre avanti perciò scendo a destra
				m_point = m_point->right;
				while(m_point->left != nullptr){
					m_point = m_point->left;
				}
			}
		}
	}; // class const_iterator
	
	/**
		 @brief iteratore che punta al dato più piccolo
		 Ritorna l'iteratore al dato più piccolo nell'albero
		 
		 @return iteratore al dato più piccolo
	 */
	const_iterator begin() const{
		node* current = m_root;
		
		while(current->left != nullptr){
			current = current->left;
		}
		
		return const_iterator(current);
	}
	
	/**
		 @brief iteratore che punta al dato più grande
		 Ritorna l'iteratore al dato più grande nell'albero
		 
		 @return iteratore al dato più grande
	 */
	const_iterator end() const{
		node* current = m_root;
		
		while(current->right != nullptr){
			current = current->right;
		}
		
		return const_iterator(current);
	}
	
	/**
		@brief sottoalbero con radice il valore passato
		Metodo subtree che ritorna un sottoalbero del bst corrente con radice val (5)
	  
		@param val la radice del sottoalbero
		@return il sottoalbero di bst con radice val (vuoto se la radice non esiste).
	 */
	BST subtree(const T& val) const {
		node* node = find_node(val);
		
		BST<T, C> bst;
		bst.recursive_copy(node);
		return bst;
	}
	
	/**
		 @brief altezza dell'albero.
		 Metodo aggiuntivo che torna l'altezza dell'albero
		 
		 @return l'altezza dell'albero dalla radice
	 */
	unsigned int height() const{
		return (m_root == nullptr) ? 0 : height(m_root) - 1;
	}
};// class BST

/**
	 @brief stampa dell'albero
	 Ridefinizione dell'operatore di stream per la stampa del contenuto dell'albero .(4)
	 
	 @param os oggetto stream di output
	 @param ol lista da stampare
	 
	 @return reference allo stream di output
 */
template <typename T, typename C>
std::ostream& operator<<(std::ostream& os, const BST<T, C>& bst) {
	typename BST<T, C>::const_iterator i, ie;
	
	if (bst.size() == 0) //Se l'albero è vuoto non esistono iteratori!
		return os;
	
	i = bst.begin();
	ie = bst.end();
	
	while(i != ie) {
		os << *i << " ";
		++i;
	}
	
	return os;
}

/**
	 @brief stampa dell'albero se predicato verificato
	 Funzione globale che stampa a schermo tutti i valori che soddifano il predicato. (5)
	 
	 @param os oggetto stream dove stampare
	 @param ol lista da stampare
	 
	 @return reference allo stream di output
 */
template<typename T, typename C, typename P>
void printIF(const BST<T, C>& bst, P pred){
	
	if (bst.size() == 0) //Se l'albero è vuoto non esistono iteratori!
		return;
	
	typename BST<T,C>::const_iterator i,ie;

	i = bst.begin();
	ie = bst.end();
	
	while(i!=ie) {
		if(pred(*i)) {
			std::cout << *i << std::endl;
		}
		++i;
	}
	
	if(pred(*i)) {
		std::cout << *i << std::endl;
	}
}
#endif


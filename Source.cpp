#include<iostream>
#include<string>
#include<fstream>
using namespace std;


//--------------------------------------Node Class--------------------------------------
template<class type>
class Node {
public:
	Node() {
		data = 0;
		prev = 0;
		next = 0;
	}
	Node(type d, Node<type>* p = 0, Node<type>* n = 0) {
		data = d;
		prev = p;
		next = n;
	}
	template<class type>friend class List;
private:
	type data;
	Node<type>* next;
	Node<type>* prev;
};



//----------------------------------List Class to make DLL------------------------------
template<class type>
class List {
private:
	Node<type>* head;
	Node<type>* tail;
	int size;
	//-----------------------Iterator Class as nested class of List---------------------
	class ListIterator {
	public:
		template<class type>friend class List;
		ListIterator(Node<type>* t = NULL) {
			iptr = t;
		};

		ListIterator& operator++(int) {
			ListIterator old = *this;
			++(*this);
			return old;
		}

		ListIterator& operator++() {
			if (iptr)
				iptr = iptr->next;
			return (*this);
		}

		type& operator*() {
			return iptr->data;
		}

		bool operator==(const ListIterator& l) const
		{
			return iptr == l.iptr;
		}

		bool operator!=(const ListIterator& l) const {
			return !(iptr == l.iptr);
		}
	private:
		Node<type>* iptr;
	};
public:
	typedef ListIterator Iterator;
	Iterator begin() {
		Iterator I(head);
		return I;
	}
	Iterator end() {
		Iterator I(tail);
		return I;
	}

	//---Default Constructor of List Class---
	List() {
		head = NULL;
		tail = NULL;
		size = 0;
	}

	//---Copy Constructor of List Class----
	List(const List<type>& list)
	{
		head = NULL;
		tail = NULL;
		for (Node<type>* tmp = list.head; tmp != 0; tmp = tmp->next)
			insertAtEnd(tmp->data);
	}

	//---Function to Insert the Node At End-----
	void insertAtEnd(type const element) {
		if (tail == 0) {
			head = tail = new Node<type>(element);
			size++;
		}
		else {
			Node<type>* newnode = new Node<type>(element, tail, 0);
			tail->next = newnode;
			newnode->prev = tail;
			tail = newnode;
			size++;
		}
	}

	//-----Destructor-------
	~List()
	{
		for (Node<type>* tmp = head; tmp != 0; tmp = head)
		{
			head = head->next;
			if (tmp != NULL) {
				tmp=NULL;
			}
		}
	}
};




//------------------------------------Doc_Info Class------------------------------------
class Term_Info;
class Search_Engine;
class Doc_Info {
	int Doc_iD;
	int term_f;
public:
	friend class Search_Engine;
	friend class Term_Info;
	Doc_Info() {
		Doc_iD = 0;
		term_f = 0;
	}
	Doc_Info(Doc_Info const& obj) {
		Doc_iD = obj.Doc_iD;
		term_f = obj.term_f;
	}
	Doc_Info(int Id, int freq) {
		Doc_iD = Id;
		term_f = freq;
	}

};



//------------------------------------Term_Info Class-----------------------------------
class Search_Engine;
class Term_Info {
	string key;
	List<Doc_Info> doc;
public:
	friend class Search_Engine;

	Term_Info() {
		key = "\0";
	}

	Term_Info(string k) {
		key = k;
	}
};



//---------------------Global Template Function to Grow the array-----------------------
template <class type>
void Grow(type*& array, int& size)
{
	type* temp = new type[size + 1];

	for (int i = 0; i < size; i++)
	{
		temp[i] = array[i];
	}
	size++;
	delete[]array;
	array = temp;
}



//------------------------------------Search_Engine Class-------------------------------
class Search_Engine {
	List<Term_Info>* Index;
public:
	Search_Engine() {
		Index = new List<Term_Info>;
	}

	//</----------------Algorithm-1---------------\> 
	void Create_Index(string* fileName, int Dsize) {
		int asize = 0;
		int index = -1;
		string* arr;
		arr = new string[asize];
		for (int i = 0; i < Dsize; i++) {
			ifstream fin(fileName[i]);			                    //Adding words of each document in array
			while (!fin.eof()) {                                    //then converting all the words to lower
				Grow(arr, asize);                                   //case and then removing duplicates 
				index++;                                            //from array
				fin >> arr[index];
			}
			fin.close();
		}
		for (int i = 0; i < asize; i++)
		{
			for (int j = 0; j < arr[i].length(); j++)
				arr[i][j] = tolower(arr[i][j]);
		}
		removeDuplicateWord(arr, asize);
		int termf;
		for (int i = 0; i < asize; i++) {								//Creating a new term and 
			Term_Info t(arr[i]);										//adding all documents 
			for (int j = 0; j < Dsize; j++) {							//information in it
				termf = termfreq(arr[i], fileName[j]);					//then adding this term 
				if (termf != 0) {										//object to Index
					Doc_Info D(j + 1, termf);
					t.doc.insertAtEnd(D);
				}
			}
			Index->insertAtEnd(t);
		}
	}

	//----Function to Calculate Term Frequency-----
	int termfreq(string key, string fname) {
		int size = 0;
		int ind = -1;
		int count = 0;
		string* word = new string[size];
		ifstream fin(fname.c_str());
		while (!fin.eof()) {
			Grow(word, size);
			ind++;
			fin >> word[ind];
		}
		fin.close();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < word[i].length(); j++)
				word[i][j] = tolower(word[i][j]);
		}
		for (int i = 0; i < size; i++) {
			if (key == word[i]) {
				count++;
			}
		}
		return count;
	}

	void removeSpecificWord(string* arr, int idx, int& size)
	{
		int i;
		for (i = idx; i < size - 1; i++)
			arr[i] = arr[i + 1];
		size--;
	}
	//-----Function to Remove Duplicate Words------
	void removeDuplicateWord(string* arr, int& size)
	{
		int i, j;
		string target;
		for (i = 0; i < size; i++)
		{
			arr[i];
			target = arr[i];
			for (j = i + 1; j < size; j++)
			{
				if (target == arr[j]) {
					removeSpecificWord(arr, j, size);
					arr[i];
					j--;
				}
			}
		}
	}



	void Add_Doc_to_Index(string fName) {
		ifstream fin(fName.c_str());
		int size = 0;
		int index = -1;
		string* arr = new string[size];
		while (!fin.eof()) {                                       //Initializing array with 
			Grow(arr, size);                                      //words present in file 
			index++;                                         
			fin >> arr[index];
		}
		fin.close();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < arr[i].length(); j++)
				arr[i][j] = tolower(arr[i][j]);
		}
		removeDuplicateWord(arr, size);
		for (List<Term_Info>::Iterator itr = Index->begin(); itr != NULL; itr++)
		{
			for (int i = 0; i < size; i++)                         
			{
				if (arr[i] == (*itr).key)                                //if term is found then it inserts
				{                                                        //the doc_info object into the doc
					Doc_Info D(fName[3] - 48, termfreq(arr[i], fName));  //list in that term and then deleting 
					(*itr).doc.insertAtEnd(D);                           //that term from array
					shrink(arr, i, size);
					break;
				}
			}
		}
		if (size != 0)
		{
			for (int i = 0; i < size; i++)                              //if any term left in array it
			{                                                           //adds that term at the end of 
				Term_Info T(arr[i]);                                    //Index List and initialize the
				Doc_Info doc(fName[3] - 48, termfreq(arr[i], fName));   //Doc_Info List in that term
				T.doc.insertAtEnd(doc);
				Index->insertAtEnd(T);
			}
		}

	}
	void shrink(string*& arr, int index, int& size) {
		for (int i = index; i < size; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
	}
	
	//-----------------Algorithm-2----------------
	void Search_Document(string* query, int Qsize) {
		int isize = 0;
		Doc_Info* iarr = new Doc_Info[isize];
		int Iindex = -1;
		for (int i = 0; i < Qsize; i++)
		{
			for (int j = 0; j < query[i].length(); j++)
				query[i][j] = tolower(query[i][j]);
		}
		for (List<Term_Info>::Iterator itr = Index->begin(); itr != NULL; itr++)    //Iterating in the List
		{																			//if the key is matched 
			for (int i = 0; i < Qsize; i++)                                         //with query then it  
			{																		//adds them in array
				if (query[i] == (*itr).key)
				{
					List<Doc_Info>::Iterator it = (*itr).doc.begin();
					if (it == (*itr).doc.end()) {                                   //If there is only one
						Grow(iarr, isize);										    //Node in Doc_List then 
						Iindex++;													//it inserts it in array
						iarr[Iindex] = (*it);
					}
					else {
						for (it = (*itr).doc.begin(); it != NULL; it++) {           //else Inserting the whole 
							Grow(iarr, isize);										//doc_Info List from the 
							Iindex++;                                               //term_info into the array
							iarr[Iindex] = (*it);                                                        
							
						}
					}
				}
			}
		}

		if (isize == 0) {                                                          //Checking if no query 
			cout << "None!\n";                                                     //word matched to key terms
		}                                                                          //then displays some msg

		int fsize = 0;
		int* farr = new int[fsize];                                                 
		int ind = -1;															   
		for (int i = 0; i < isize; i++) {
			int count = 1;
			for (int j = i + 1; j < isize; j++) {
				if (iarr[i].Doc_iD == iarr[j].Doc_iD) {
					count++;                                                       //Removing duplicate words 
					iarr[i].term_f + iarr[j].term_f;                               //from the array and adding
					removeD(iarr, j, isize);									   //their corresponding term
					j--;														   //frequency(to rank them later)
				}
			}
			Grow(farr, fsize);
			ind++;
			farr[ind] = count;													   //Creating a parallel array
		}																		   //for storing query terms frequency
		for (int i = 0; i < fsize - 1; i++) {
			for (int j = 0; j < fsize - i - 1; j++) {
				if (farr[j] < farr[j + 1]) {
					swap(farr[j], farr[j + 1]);									   //Bubble sorting the arrays in 
					swap(iarr[j], iarr[j + 1]);									   //descending order w.r.t to query
				}																   //terms
			}
		}
		
		Doc_Info key;                                                             
		int k;
		for (int a = 0; a < fsize;) {                                          
			int tcount = 1;
			for (int i = a; i < fsize; i++) {                                     //Calculating the number of Doc_Info 
				if (farr[i] == farr[i + 1]) {									  //having same query term frequency		
					tcount++;
				}
			}
			for (int j = a + 1; j < tcount; j++)						
			{
				key = iarr[j];
				for (k = j - 1; (k >= 0) && (iarr[k].term_f < key.term_f); k--)		//Insertion sorting according to 	
				{																    //the term frequency in descending  
					iarr[k + 1] = iarr[k];                                          //order
				}
				iarr[k + 1] = key;
			}
			for (int s = a; s < tcount; s++) {
				int fcount = 1;
				for (int m = s; m < tcount; m++) {								  //Calculating the number of Doc_Info 
					if (iarr[m].term_f == iarr[m + 1].term_f) {					  //having same term frequency
						fcount++;
					}
				}
				for (int j = s + 1; j < fcount; j++)
				{
					key = iarr[j];
					for (k = j - 1; (k >= 0) && (iarr[k].Doc_iD > key.Doc_iD); k--)   //Insertion sorting according to
					{																 //the Doc_id in ascending order	
						iarr[k + 1] = iarr[k];
					}
					iarr[k + 1] = key;
				}
				s += fcount;
			}
			a += tcount;
		}
		for (int i = 0; i < isize; i++) {                                           //Displaying the result for 
			cout << "Doc " << iarr[i].Doc_iD << endl;                               //each query term
		}
	}
	void removeD(Doc_Info*& arr, int j, int& isize) {
		for (int i = j; i < isize; i++) {
			arr[i] = arr[i + 1];
		}
		isize--;
	}

	~Search_Engine()
	{
		if (Index) {
			Index = NULL;
		}
	}
};



//------------Function to tokenize Query------------------
void Querytokenize(string query, string*& Qarr, int& Qsize) {
	ofstream fout;
	fout.open("temp.txt");
	fout << query;
	fout.close();
	int Qindex = -1;                                            //Writing query string to file 
	ifstream fin;											    //then making the array of each  
	fin.open("temp.txt");										//word and then deleting the file
	while (!fin.eof()) {                                        //and then converting each word to  
		Grow(Qarr, Qsize);										//lowercase
		Qindex++;
		fin >> Qarr[Qindex];
	}
	fin.close();
	remove("temp.txt");
	
}




//----------------------------------Main--------------------------------------
int main() {
	Search_Engine S;
	int size = 4;
	string Docs[4] = { "Doc1.txt", "Doc2.txt", "Doc3.txt", "Doc4.txt" };
	S.Create_Index(Docs, size);
	S.Add_Doc_to_Index("Doc5.txt");
	string query;
	cout << "Enter Search Query: ";
	getline(cin, query);
	int Qsize = 0;
	string* Qarr = new string[Qsize];
	Querytokenize(query, Qarr, Qsize);
	S.Search_Document(Qarr, Qsize);
	return 0;
}
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

typedef struct node {
	string b;
	struct node *next;
}NODE;

class Student_list {
private:
	NODE *b, *e;
public:
	Student_list() { b = new NODE; e = b; }
	void push_back(string s);
	NODE* begin() { return b; }
	NODE* end() { return e; }
};
void Student_list::push_back(string str)
{
	e->b = str;
	NODE *next = new NODE;
	e->next = next;
	e = next;
}

int main()
{
	Student_list strlist;
	string word1 = "word1", word2 = "word2";
	strlist.push_back(word1);
	strlist.push_back(word2);
	for (NODE* p = strlist.begin(); p!= strlist.end(); p = p->next)
	cout << p->b << endl;
	return 0;
}
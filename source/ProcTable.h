#include "../include/boost/unordered_map.hpp"
#include "TNode.h"
#include <set>

using namespace std;

class ProcTable {
	/*

	Conceptual ProcTable:
	------------------------------------------------------------------------------------------------------------
	|_[key]_____|__________________|_____________________|_____________________|_______________________|_______|
	| proc name | using these vars | modfying these vars | calling these procs | called by these procs | TNode |
	|___________|__________________|_____________________|_____________________|_______________________|_______|
	------------------------------------------------------------------------------------------------------------

	Actual Implementation:
	6 separate tables - 

	All-Procs Table - contains all the procs that are defined (authoritative index of all procs)
	-----------------
	|_[key]_________|
	| name (string) |
	-----------------

	Using Table - contains all the procs whose using has been set 
	---------------------------------------
	|_[key]_________|_____________________|
	| name (string) | using (set<string>) | 
	---------------------------------------

	Modifying Table - contains all the procs whose modifying has been set 
	-------------------------------------------
	|_[key]_________|_________________________|
	| name (string) | modifying (set<string>) |
	-------------------------------------------

	Calling Table - contains all the procs whose calling has been set 
	-----------------------------------------
	|_[key]_________|_______________________|
	| name (string) | calling (set<string>) | 
	-----------------------------------------

	Called-By Table - contains all the procs whose called-by has been set 
	-------------------------------------------
	|_[key]_________|_________________________|
	| name (string) | called-by (set<string>) |
	-------------------------------------------

	TNode-Reference Table - contains all the procs whose TNode has been set
	---------------------------------------
	|_[key]_________|_____________________|
	| name (string) | TNodes (set<TNode>) |
	---------------------------------------

	*/

	typedef set<string> UsingTableRow;
	typedef set<string> ModifyingTableRow;
	typedef set<string> CallingTableRow;
	typedef set<string> CalledByTableRow;
	typedef set<TNode>	TNodeRefTableRow;

	typedef set<string>										AllProcsTable;
	typedef boost::unordered_map<string, UsingTableRow>		UsingTable;
	typedef boost::unordered_map<string, ModifyingTableRow> ModifyingTable;
	typedef boost::unordered_map<string, CallingTableRow>	CallingTable;
	typedef boost::unordered_map<string, CalledByTableRow>	CalledByTable;
	typedef boost::unordered_map<string, TNodeRefTableRow>	TNodeRefTable;

public:
	/* Explanation: const set<obj>& method(const obj &param);
		const set<obj>& = return a reference to the actual set, that cannot be mutated
		const obj &param = param is a reference to an actual obj, that cannot be mutated
	*/

	// constructor
	ProcTable();	

	// property getters
	const set<string>& getVarsUsed(const string &proc);
	const set<string>& getVarsModified(const string &proc);
	const set<string>& getProcsCalledBy(const string &callingProc);
	const set<string>& getProcsThatCall(const string &calledProc);
	const TNode& getTNode(const string &procName);

	// property setters
	void addUses(const string &proc, const string &usedVar);
	void addModifies(const string &proc, const string &modifiedVar);
	void addCalls(const string &callingProc, const string &calledProc);
	void addTNode(const string &proc);

	// general getters
	const set<string>& getAllProcs();

	// general methods
	bool contains(const string &varName);

private:

	AllProcsTable	_allProcsTable;
	UsingTable		_usingTable;
	ModifyingTable	_modifyingTable;
	CallingTable	_callingTable;
	CalledByTable	_calledByTable;
	TNodeRefTable	_tNodeRefTable;

};
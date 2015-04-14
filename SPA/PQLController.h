#include <string>
#include <set>

using namespace std;

class PQLController {

public:
	PQLController(void);
	~PQLController(void);

	set<string> parse(string query); // runs PQL controller on this string

private:
	void postProcess(set<string>& results);
};

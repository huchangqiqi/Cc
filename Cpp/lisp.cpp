#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

// return given mumber as string
std::string str(long n) {std::ostringstream os; os << n; return os.str();}

// return true if given char is '0' .. '9'
bool isdig(char c) { return isdigit(static_cast<unsigned char>(c)) != 0;}

// cell

enum cell_type{ Symbol, Number, List, Proc, Lambda };

struct environment; // forward declaration ; cell and environment reference each other

// can hold any kind of lisp value
struct cell {
    typedef cell (*proc_type)(const std::vector<cell> &);
    typedef std::vector<cell>::const_iterator iter;
    typedef std::map<std::string, cell> map;
    cell_type type;
    std::string val;
    std::vector<cell> list;
    proc_type proc;
    environment * env;
    cell(cell_type type = Symbol): type(type), env(0) {}
    cell(cell_type type, const std::string& val): type(type), val(val), env(0) {}
    cell(proc_type proc): type(Proc), proc(proc), env(0) {}
};

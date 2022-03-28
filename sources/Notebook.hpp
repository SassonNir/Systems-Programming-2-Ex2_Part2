#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <unordered_map>
#include <map>
#include <vector>
#include <ctype.h>
#include <limits.h>
#include "Direction.hpp"
using namespace std;
// using namespace std::placeholders;

namespace ariel {
class Notebook {
   private:
    // unordered_map<int, vector<
    unordered_map<int, unordered_map<int, char>> _notebook;
    char _get_char(int page, int row, int column);
    void _set_char(int page, int row, int column, char c, bool validate);
    void _set_char(int page, int row, int column, char c);
    bool _has_char(int page, int row, int column);
   public:
    Notebook();
    ~Notebook();
    void write(int page, int row, int column, Direction direction, const string& text);
    string read(int page, int row, int column, Direction direction, int length);
    void erase(int page, int row, int column, Direction direction, int length);
    void show(int page);
};
};  // namespace ariel
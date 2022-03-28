#include "Notebook.hpp"

const int LINE_LENGTH = 100;
const char PLACEHOLDER = '_';
const char ERASER = '~';
const int MINIMAL_LINES_SHOW = 5;

namespace ariel {
Notebook::Notebook() {
}
Notebook::~Notebook() {
}

int get_position(int row, int column) {
    return row * LINE_LENGTH + column;
}

template <class Key,
          class T,
          class Hash,
          class Pred,
          class Alloc>
bool has_key(std::unordered_map<Key, T, Hash, Pred, Alloc>& m, Key key) {
    return m.find(key) != m.end();
}

void validate_input(int page, int row, int column, Direction direction, int length) {
    if (page < 0 || row < 0 || column < 0 || length < 0) {
        throw std::invalid_argument("invalid argument: page, row, column and length must be positive");
    }
    if (column >= LINE_LENGTH) {
        throw std::invalid_argument("invalid argument: column must be less than " + std::to_string(LINE_LENGTH));
    }
    if (direction == Direction::Horizontal && (column + length) > LINE_LENGTH) {
        throw std::invalid_argument("invalid argument: too long for horizontal direction");
    }
}

char Notebook::_get_char(int page, int row, int column) {
    if (this->_has_char(page, row, column)) {
        return this->_notebook[page][get_position(row, column)];
    }
    return PLACEHOLDER;
}

bool Notebook::_has_char(int page, int row, int column) {
    if (has_key(this->_notebook, page)) {
        if (has_key(this->_notebook[page], get_position(row, column))) {
            return this->_notebook[page][get_position(row, column)] != PLACEHOLDER;
        }
    }
    return false;
}

void Notebook::_set_char(int page, int row, int column, char c, bool validate) {
    if (validate && this->_has_char(page, row, column)) {
        throw std::invalid_argument("cannot write to this cell (it is already occupied)");
    }
    _notebook[page][get_position(row, column)] = c;
}

void Notebook::_set_char(int page, int row, int column, char c) {
    this->_set_char(page, row, column, c, true);
}

void Notebook::write(int page, int row, int column, Direction direction, const string& text) {
    validate_input(page, row, column, direction, text.length());

    for (int i = 0; i < text.length(); i++) {
        char c = text[(size_t)i];
        if (c == ERASER || isprint(c) == 0) {
            throw std::invalid_argument("invalid argument: text contains invalid character");
        }
        if (this->_has_char(page, row, column) && this->_get_char(page, row, column) == PLACEHOLDER) {
            throw std::invalid_argument("cannot write to this cell (it is already occupied)");
        }
    }

    for (int i = 0; i < text.length(); i++) {
        char c = text[(size_t)i];
        if (direction == Direction::Horizontal) {
            this->_set_char(page, row, column + i, c);
        } else {
            this->_set_char(page, row + i, column, c);
        }
    }
}
string Notebook::read(int page, int row, int column, Direction direction, int length) {
    validate_input(page, row, column, direction, length);

    string text;
    for (int i = 0; i < length; i++) {
        if (direction == Direction::Horizontal) {
            text += this->_get_char(page, row, column + i);
        } else {
            text += this->_get_char(page, row + i, column);
        }
    }
    return text;
}
void Notebook::erase(int page, int row, int column, Direction direction, int length) {
    validate_input(page, row, column, direction, length);

    for (int i = 0; i < length; i++) {
        if (direction == Direction::Horizontal) {
            this->_set_char(page, row, column + i, ERASER, false);
        } else {
            this->_set_char(page, row + i, column, ERASER, false);
        }
    }
}
void Notebook::show(int page) {
    if (page < 0) {
        throw std::invalid_argument("invalid argument: page must be positive");
    }
    if (!has_key(this->_notebook, page)) {
        // throw std::invalid_argument("invalid argument: page does not exist");
        cout << "Nothing Intersting to show!" << endl;
        return;
    }
    unordered_map<int, char> page_lines = this->_notebook[page];
    map<int, string> lines;
    int min_row = numeric_limits<int>::max();
    int max_row = numeric_limits<int>::min();
    string output;
    for (pair<int, char> it : page_lines) {
        int row = it.first / LINE_LENGTH;
        size_t column = it.first % LINE_LENGTH;
        string line;
        if (row < min_row) {
            min_row = row;
        }
        if (row > max_row) {
            max_row = row;
        }
        if (lines.find(row) == lines.end()) {
            lines[row].append(LINE_LENGTH, PLACEHOLDER);
        }
        lines[row][column] = it.second;
        // output.append(line);
    }

    // cout << std::left;
    // for (int i = min_row; i <= max_row; i++) {
    //     cout << std::setw(6) << i << ": " << lines[i] << "\n";
    //     // output.append(std::setw(6) << i);
    //     // if (lines.find(i) != lines.end()) {
    //     //     output.append(lines[i]);
    //     // } else {
    //     //     output.append(LINE_LENGTH, PLACEHOLDER);
    //     // }
    //     // output.append("\n");
    // }

    // for (pair<int, char> it : page_lines) {
    //     int row = it.first / LINE_LENGTH;
    //     if (row < min_row) {
    //         min_row = row;
    //         output.append(LINE_LENGTH, PLACEHOLDER);
    //         output.append("\n");
    //     }
    //     if (row > max_row) {
    //         max_row = row;
    //         if (max_row != min_row) {
    //             output.append(LINE_LENGTH, PLACEHOLDER);
    //             output.append("\n");
    //         }
    //     }
    // }
    // int lines_to_show = max_row - min_row + 1;
    // bool is_uneven = lines_to_show < MINIMAL_LINES_SHOW;
    // if (is_uneven) {
    //     for (int i = 0; i < MINIMAL_LINES_SHOW - lines_to_show; i++) {
    //         output.append(LINE_LENGTH, PLACEHOLDER);
    //         output.append("\n");
    //     }
    //     lines_to_show = MINIMAL_LINES_SHOW;
    //     if (min_row > MINIMAL_LINES_SHOW/2) {
    //         min_row = min_row - MINIMAL_LINES_SHOW/2;
    //     }
    // }

    // int i = 0;
    // for (pair<int, char> it : page_lines) {
    //     int row = it.first / LINE_LENGTH - i; // - i to compensate for the newlines
    //     int column = it.first % LINE_LENGTH;

    //     row = row - min_row;
    //     if (is_uneven) {
    //         row = MINIMAL_LINES_SHOW - row - 1; // Center the text in the screen
    //     }

    //     i++;
    // }

    // cout << output;
}
}  // namespace ariel
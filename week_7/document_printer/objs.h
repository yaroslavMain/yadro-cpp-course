#pragma once
#include <ostream>

struct HTML {
  void render(std::ostream &os) const { os << "<html>Hello</html>" << "\n"; };
};

struct PDF {
  void print(std::ostream &os) const { os << "Hello from pdf" << "\n"; };
};

struct Excel {
  void info(std::ostream &os) const { os << "Hello from excel" << "\n"; };
};

struct Textedit {
  void text(std::ostream &os) const { os << "Hello from textedit" << "\n"; };
};

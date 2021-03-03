#include <iostream>
#include <vector>

void print_board(const std::vector<int> &nqueens) {
  int i, j;
  for (i = 0; i < nqueens.size(); ++i) {
    for (j = 0; j < nqueens.size(); ++j)
      std::cout << ((nqueens[i] == j) ? "Q|" : "_|");
    std::cout << std::endl;
  }
}

bool is_solveable(const std::vector<int> &nqueens,
                  const std::vector<bool> &used, const int &v, const int &i) {
  if (used[v]) return false;
  for (int j = 0; j < i; ++j)
    if (std::abs(v - nqueens[j]) == std::abs(i - j)) return false;
  return true;
}

bool solve(std::vector<int> &nqueens, std::vector<bool> &used, int i) {
  if (nqueens.size() == i) {
    return true;
  } else {
    for (int v = 0; v < nqueens.size(); ++v) {
      if (is_solveable(nqueens, used, v, i)) {
        nqueens[i] = v;
        used[v] = true;
        if (solve(nqueens, used, i + 1)) return true;
        nqueens[i] = 0;
        used[v] = false;
      }
    }
    return false;
  }
}

int main() {
  size_t n;
  std::cin >> n;
  std::vector<int> nqueens(n);
  std::vector<bool> used(n, false);
  if (solve(nqueens, used, 0)) {
    print_board(nqueens);
  } else {
    std::cout << "No solution!" << std::endl;
  }
  return 0;
}

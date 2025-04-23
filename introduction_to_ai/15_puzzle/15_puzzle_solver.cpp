class PuzzleState {
public:
    uint64_t board;
    int size;

    PuzzleState() : board(0), size(0) {}
    PuzzleState(const std::vector<int>& b, int s) : size(s) {
        board = 0;
        for (int i = 0; i < size * size; ++i) {
            board |= (static_cast<uint64_t>(b[i]) << (i * 4));
        }
    }
}
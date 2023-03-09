#include <stdio.h>
#include <stdlib.h>

struct Coordinates {
  int x;
  int y;
};
enum State { EMPTY = 0, CIRCLE = -1, CROSS = 1 };
struct Game {
  enum State **state;
  const int SIZE;
};
enum Winner {
  WINNER_NOT_YET_DECIDED,
  WINNER_CROSS,
  WINNER_CIRCLE,
  WINNER_DRAW
};

void printGame(const struct Game *game) {
  for (int i = 0; i < 2 * game->SIZE + 1; i++) {
    printf("-");
  }
  printf("\n");
  for (int i = 0; i < game->SIZE; i++) {
    printf("|");
    for (int j = 0; j < game->SIZE; j++) {
      switch (game->state[i][j]) {
      case EMPTY:
        printf(" ");
        break;
      case CIRCLE:
        printf("O");
        break;
      case CROSS:
        printf("X");
        break;
      default:
        printf("-");
      }
      printf("|");
    }
    printf("\n");
    for (int j = 0; j < 2 * game->SIZE + 1; j++) {
      printf("-");
    }
    printf("\n");
  }
  return;
}
enum Winner checkWin(const struct Game *game, const enum State *turn) {
  int check[game->SIZE * 2 + 2];
  for (int i = 0; i < game->SIZE * 2 + 2; i++) {
    check[i] = 0;
  }
  enum Winner winner;
  int cell_number;
  for (int i = 0; i < game->SIZE; i++) {
    for (int j = 0; j < game->SIZE; j++) {
      cell_number = i * game->SIZE + j;
      if (i == j) {
        check[0] += game->state[i][j];
      }
      if (game->SIZE - 1 - i == j) {
        check[1] += game->state[i][j];
      }
      for (int k = 0; k < game->SIZE; k++) {
        if (k == i) {
          check[i + game->SIZE + 2] += game->state[i][j];
        }
      }

      if (cell_number % game->SIZE == j) {
        check[j + 2] += game->state[i][j];
      }
    }
  }

  for (int i = 0; i < game->SIZE * 2 + 2; i++) {
    // printf("| %d | \n", check[i]);
    if (check[i] == game->SIZE) {
      return WINNER_CROSS;
    }
    if (check[i] == -1 * (game->SIZE)) {
      return WINNER_CIRCLE;
    }
  }
  return WINNER_NOT_YET_DECIDED;
}
struct Coordinates getNextMove(const enum State *turn) {
  char move_input[3];
  char current_player;
  switch (*turn) {
  case CIRCLE:
    current_player = 'O';
    break;
  case CROSS:
    current_player = 'X';
    break;
  case EMPTY:
    current_player = ' ';
    break;
  }
  printf("\n%c's turn:\n", current_player);
  scanf("%2s", move_input);
  if (!(move_input[0] < 58 && move_input[0] > 47) ||
      !(move_input[1] < 58 && move_input[1] > 47)) {

    printf("\ninvalid move, try again\n");
    struct Coordinates move_coords = getNextMove(turn);
    return move_coords;
  }
  struct Coordinates move_coords = {move_input[0] - 48, move_input[1] - 48};
  return move_coords;
}
int nextRound(struct Game *game, enum State *turn, enum Winner *winner) {
  printGame(game);
  struct Coordinates move = getNextMove(turn);
  game->state[move.x][move.y] = *turn;
  switch (*turn) {
  case CROSS:
    *turn = CIRCLE;
    break;
  case CIRCLE:
    *turn = CROSS;
    break;
  case EMPTY:
    return -1;
  }
  *winner = checkWin(game, turn);
  if (*winner == WINNER_NOT_YET_DECIDED) {
    nextRound(game, turn, winner);
  }
  return 0;
}

int main() {
  printf("Wie groß soll das Spielfeld sein?\n");
  int sizeInput;
  scanf("%d", &sizeInput);
  struct Game game = {(enum State **)malloc(sizeof(int *) * sizeInput),
                      sizeInput};
  for (int i = 0; i < game.SIZE; i++) {
    game.state[i] = (enum State *)malloc(sizeof(int) * game.SIZE);
    for (int j = 0; j < game.SIZE; j++) {
      game.state[i][j] = EMPTY;
    }
  }
  enum State turn = CROSS;
  enum Winner winner = WINNER_NOT_YET_DECIDED;
  nextRound(&game, &turn, &winner);
  (winner == WINNER_CROSS) ? printf("\nX wins!\n\n") : printf("\nO wins!\n\n");
  printGame(&game);
  return 0;
}

import time
import turtle
import numpy as np

# 0 -> ' '
# 1 -> '\'
# 2 -> '/'

def draw_diag(pen: turtle.Turtle, diag_type: int, x: int, y: int, x_delta: int, y_delta: int):
    pen.penup()
    pen.color("#FF0000")
    if diag_type == 1:
        pen.goto(x, y)
        pen.pendown()
        pen.goto(x+x_delta, y-y_delta)
    elif diag_type == 2:
        pen.goto(x+x_delta, y)
        pen.pendown()
        pen.goto(x, y-y_delta)
    pen.color("#000000")
    pen.penup()

def draw_board(board: np.array, pen: turtle.Turtle):
    pen.speed(0)
    pen.color("#000000")
    pen.hideturtle()
    pen.pensize(1)
    
    row_dim = 30
    col_dim = 30
    top_left_x = 1
    top_left_y = col_dim*board.shape[1]
    for row in range(board.shape[0]+1):
        pen.penup()
        pen.goto(top_left_x, top_left_y-row*row_dim)
        pen.pendown()
        pen.goto(top_left_x+col_dim*board.shape[1], top_left_y-row*row_dim)
    for col in range(board.shape[1]+1):
        pen.penup()
        pen.goto(top_left_x+col*col_dim, top_left_y)
        pen.pendown()
        pen.goto(top_left_x+col*col_dim, top_left_y-col_dim*board.shape[0])

    for row in range(board.shape[0]):
        for col in range(board.shape[1]):
            if board[row][col] != 0:
                draw_diag(pen, board[row][col], top_left_x+col*col_dim, top_left_y-row*row_dim, col_dim, row_dim)


def is_partial_solution(board: np.array, addr: int, addc: int):
    top = addr == 0
    bottom = addr == board.shape[0]-1
    left = addc == 0
    right = addc == board.shape[1] - 1

    if board[addr, addc] == 1:
        if not top:
            if not left and board[addr - 1, addc - 1] == 1:
                return False
            if board[addr - 1, addc] == 2:
                return False
        if not left and board[addr, addc - 1] == 2:
            return False
        if not right and board[addr, addc+1] == 2:
            return False
        if not bottom:
            if board[addr+1, addc] == 2:
                return False
            if not right and board[addr+1, addc+1] == 1:
                return False

    if board[addr, addc] == 2:
        if not top:
            if board[addr - 1, addc] == 1:
                return False
            if not right and board[addr - 1, addc + 1] == 2:
                return False
        if not left and board[addr, addc - 1] == 1:
            return False
        if not right and board[addr, addc+1] == 1:
            return False
        if not bottom:
            if not left and board[addr+1, addc-1] == 2:
                return False
            if board[addr+1, addc] == 1:
                return False

    return True


def solve(board: np.array, row: int, col: int, e: int):
    if np.count_nonzero(board == 0) == (board.shape[0]*board.shape[1])-e:
        print("Solution Found")
        pen = turtle.Turtle()
        draw_board(board, pen)
        pen.getscreen().update()
        turtle.exitonclick()
        exit()
    for pos in range(2, -1, -1):
        board[row, col] = pos
        if is_partial_solution(board, row, col):
            if col == board.shape[1]-1:
                next_row, next_col = row+1, 0
            else:
                next_row, next_col = row, col+1
            if next_row == board.shape[0]:
                return
            solve(board, next_row, next_col, e)


def main():
    n = int(input("Board rows: "))
    m = int(input("Board columns: "))
    e = int(input("Diagonals: "))
    board = np.zeros((n, m))
    solve(board, row=0, col=0, e=e)
    print("No possible solutions!")

if __name__ == "__main__":
    main()
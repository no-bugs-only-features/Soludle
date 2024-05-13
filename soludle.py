import tkinter as tk
import os

class Noodle():  
    def __init__(self, id, shape):
        self.id_ = id
        self.shape_ = shape

    def rotate_(self):
        res = [[0]*self.get_height() for _ in range(self.get_width())]

        for i in range(self.get_height()):
            for j in range(self.get_width()):
                res[j][self.get_height()-i-1] = self.shape_[i][j]

        self.shape_ = res

    def reflect_(self):
        res = [[0]*self.get_width() for _ in range(self.get_height())]

        for i in range(self.get_height()):
            for j in range(self.get_width()):
                res[i][self.get_width()-j-1] = self.shape_[i][j]

        self.shape_ = res

    def get_id(self):
        return self.id_

    def get_shape(self):
        return self.shape_

    def get_width(self):
        return len(self.shape_[0])

    def get_height(self):
        return len(self.shape_)

noodles = [ 
    Noodle(0, [ # A
        [0, 1], 
        [0, 1], 
        [1, 1]
    ]),

    Noodle(1, [ # B
        [0, 1], 
        [1, 1], 
        [1, 1],
    ]),

    Noodle(2, [ # C
        [0, 1],
        [0, 1],
        [0, 1],
        [1, 1],
    ]),

    Noodle(3, [ # D
        [0, 1],
        [0, 1], 
        [1, 1], 
        [0, 1],
    ]),

    Noodle(4, [ # E
        [0, 1],
        [0, 1],
        [1, 1],
        [1, 0],
    ]),

    Noodle(5, [ # F
        [0, 1], 
        [1, 1],
    ]),

    Noodle(6, [ # G
        [0, 0, 1],
        [0, 0, 1],
        [1, 1, 1],
    ]),

    Noodle(7, [ # H
        [0, 0, 1],
        [0, 1, 1],
        [1, 1, 0],
    ]),

    Noodle(8, [ # I
        [1, 0, 1],
        [1, 1, 1],
    ]),

    Noodle(9, [ # J
        [1],
        [1],
        [1],
        [1],
    ]),

    Noodle(10, [ # K
        [1, 1],
        [1, 1],
    ]),

    Noodle(11, [ # L
        [0, 1, 0],
        [1, 1, 1],
        [0, 1, 0],
    ]),
]

class Solution():
    def __init__(self, sparse_matrix):
        self.rows_ = 5
        self.cols_ = 11
        self.size_ = 50

        self.colors_ = ['orange red', 'red4', 'blue4', 'pink1', 'dark green', 'lemon chiffon', 'pale turquoise', 'DeepPink3', 'yellow', 'DarkOrchid3', 'olive drab', 'gray69']

        self.board_ = [[-1]*self.cols_ for _ in range(self.rows_)]

        for row in sparse_matrix:
            for i in row[1:]:
                x, y = (i-12) % 11, ((i-1)//11)-1
                self.board_[y][x] = row[0]
        
    def draw(self, canvas):
        canvas.delete('all')

        for i in range(self.rows_):
            for j in range(self.cols_):
                x0, y0 = j*self.size_, i*self.size_
                x1, y1 = x0 + self.size_, y0 + self.size_
                canvas.create_rectangle(x0, y0, x1, y1, outline='black', fill=self.colors_[self.board_[i][j]])

        canvas.pack()

class KanoodleTK(tk.Canvas):
    def __init__(self, root, size=50):
        self.root = root
        self.rows_ = 5
        self.cols_ = 11
        self.size_ = size
        super().__init__(root, width=(self.cols_)*self.size_, height=(self.rows_)*self.size_)

        self.colors_ = ['orange red', 'red4', 'blue4', 'pink1', 'dark green', 'lemon chiffon', 'pale turquoise', 'DeepPink3', 'yellow', 'DarkOrchid3', 'olive drab', 'gray69', 'black', 'green2', 'snow']
        self.noodles_ = noodles

        self.root.bind('<Up>', lambda event: self.move_noodle_('u'))
        self.root.bind('<Down>', lambda event: self.move_noodle_('d'))
        self.root.bind('<Left>', lambda event: self.move_noodle_('l'))
        self.root.bind('<Right>', lambda event: self.move_noodle_('r'))
        self.root.bind('r', lambda event: self.rotate_noodle_())
        self.root.bind('y', lambda event: self.reflect_noodle_())
        self.root.bind('<Return>', lambda event: self.set_noodle_())
        self.root.bind('<space>', lambda event: self.next_noodle_())

        self.cur_noodle_index_ = 0
        self.cur_noodle_ = self.noodles_[self.cur_noodle_index_]
        # x, y coordinates of top left of the noodle's shape
        self.cur_noodle_x_ = 0
        self.cur_noodle_y_ = 0

        self.reset_board()

    def redraw_board_(self):
        self.delete('all')

        board = [row[:] for row in self.game_board_]

        shape = self.cur_noodle_.get_shape()
        for i in range(self.cur_noodle_.get_height()):
            for j in range(self.cur_noodle_.get_width()):
                x, y = self.cur_noodle_x_ + j, self.cur_noodle_y_ + i
                if (shape[i][j] == 1):
                    board[y][x] = -3 if self.game_board_[y][x] != -1 else -2

        for i in range (self.rows_):
            for j in range (self.cols_):
                x0, y0 = j*self.size_, i*self.size_
                x1, y1 = x0 + self.size_, y0 + self.size_
                self.create_rectangle(x0, y0, x1, y1, outline='black', fill=self.colors_[board[i][j]])
        self.pack()

    def reset_board(self):
        self.game_board_ = [[-1]*self.cols_ for _ in range(self.rows_)]
        self.noodles_ = noodles.copy()
        self.solutions_ = []
        self.redraw_board_()

    def move_noodle_(self, dir):
        match dir:
            case 'u':
                if self.cur_noodle_y_ - 1 >= 0:
                    self.cur_noodle_y_ -= 1
            case 'd':
                if self.cur_noodle_y_ + self.cur_noodle_.get_height() < self.rows_:
                    self.cur_noodle_y_ += 1
            case 'l':
                if self.cur_noodle_x_ - 1 >= 0:
                    self.cur_noodle_x_ -= 1
            case 'r':
                if self.cur_noodle_x_ + self.cur_noodle_.get_width() < self.cols_:
                    self.cur_noodle_x_ += 1
        self.redraw_board_()

    def rotate_noodle_(self):
        self.cur_noodle_.rotate_()
        if self.cur_noodle_y_ + self.cur_noodle_.get_height() >= self.rows_:
            self.cur_noodle_y_ = self.rows_ - self.cur_noodle_.get_height()
        if self.cur_noodle_x_ + self.cur_noodle_.get_width() >= self.cols_:
            self.cur_noodle_x_ = self.cols_ - self.cur_noodle_.get_width()
        self.redraw_board_()

    def reflect_noodle_(self):
        self.cur_noodle_.reflect_()
        self.redraw_board_()

    def can_set_(self):
        shape = self.cur_noodle_.get_shape()
        for i in range(self.cur_noodle_.get_height()):
            for j in range(self.cur_noodle_.get_width()):
                x, y = self.cur_noodle_x_ + j, self.cur_noodle_y_ + i
                if (shape[i][j] == 1 and self.game_board_[y][x] != -1):
                    return False
        return True

    def set_noodle_(self):
        if self.can_set_():
            shape = self.cur_noodle_.get_shape()
            for i in range(self.cur_noodle_.get_height()):
                for j in range(self.cur_noodle_.get_width()):
                    x, y = self.cur_noodle_x_ + j, self.cur_noodle_y_ + i
                    if (shape[i][j] == 1):
                        self.game_board_[y][x] = self.cur_noodle_.get_id()

            self.noodles_.remove(self.cur_noodle_)
            self.cur_noodle_index_ -= 1
            self.next_noodle_()

    def next_noodle_(self):
        if (len(self.noodles_) != 0):
            self.cur_noodle_x_ = 0
            self.cur_noodle_y_ = 0
            self.cur_noodle_index_ = (self.cur_noodle_index_ + 1) % len (self.noodles_)
            self.cur_noodle_ = self.noodles_[self.cur_noodle_index_]
        self.redraw_board_()

    def output_board_(self):
        res = [[] for _ in noodles]
        for i in range(self.rows_):
            for j in range(self.cols_):
                if self.game_board_[i][j] >= 0:
                    res[self.game_board_[i][j]].append(i*self.cols_ + j + 12)

        with open('dlxtmp/gui_output.txt', 'w') as outfile:
            for i in range(len(res)):
                if len(res[i]) != 0:
                    print(i, end=' ', file=outfile)
                    for val in res[i]:
                        print(val, end=' ', file=outfile)
                    print('', file=outfile)

    def solve(self):
        os.system('rm -f ./dlxtmp/*')
        self.solutions_ = []
        self.output_board_()
        os.system('./dlx < dlxtmp/gui_output.txt > dlxtmp/dlx_output.txt')
        with open('dlxtmp/dlx_output.txt', 'r') as infile:
            while(True):
                try:
                    solution = [next(infile).split(' ')[:-1] for _ in range(len(noodles))]
                    sparse_matrix = []
                    for row in solution:
                        sparse_matrix.append([int(i) for i in row])
                    self.solutions_.append(Solution(sparse_matrix))
                except StopIteration:
                    break
        self.show_solutions_()

    def show_solutions_(self):
        num_solutions = len(self.solutions_)
        self.solutions_window_ = tk.Toplevel()
        self.solutions_window_.title('Solutions')
        if num_solutions < 1:
            self.solutions_window_.geometry('200x100')
            solutions_label = tk.Label(self.solutions_window_, text='No solutions found.')
            solutions_label.place(x=100, y=50, anchor=tk.CENTER)
        else:
            self.cur_solution_ = 0
            self.solutions_canvas_ = tk.Canvas(self.solutions_window_, width=self.cols_*self.size_, height=self.rows_*self.size_)
            self.solutions_[self.cur_solution_].draw(self.solutions_canvas_)

            solutions_frame = tk.Frame(self.solutions_window_)
            solutions_frame.pack(side=tk.BOTTOM)

            self.solutions_button_prev_ = tk.Button(solutions_frame, text='<', command=self.prev_solution_, state='disabled')
            self.solutions_button_prev_.pack(side=tk.LEFT, padx=5, pady=10)

            self.solutions_label_var_ = tk.StringVar(value=f'{self.cur_solution_ + 1}/{num_solutions}')
            solutions_label = tk.Label(solutions_frame, textvariable=self.solutions_label_var_)
            solutions_label.pack(side=tk.LEFT, padx=5, pady=10)

            self.solutions_button_next_ = tk.Button(solutions_frame, text='>', command=self.next_solution_, state='normal' if num_solutions > 1 else 'disabled')
            self.solutions_button_next_.pack(side=tk.LEFT, padx=5, pady=10)

        self.solutions_window_.mainloop()

    def next_solution_(self):
        self.cur_solution_ += 1 
        self.solutions_[self.cur_solution_].draw(self.solutions_canvas_)
        if self.cur_solution_ == len(self.solutions_) - 1:
            self.solutions_button_next_.config(state='disabled')
        self.solutions_button_prev_.config(state='normal')
        self.solutions_label_var_.set(f'{self.cur_solution_ + 1}/{len(self.solutions_)}')

    def prev_solution_(self):
        self.cur_solution_ -= 1
        self.solutions_[self.cur_solution_].draw(self.solutions_canvas_)
        if self.cur_solution_ == 0:
            self.solutions_button_prev_.config(state='disabled')
        self.solutions_button_next_.config(state='normal')
        self.solutions_label_var_.set(f'{self.cur_solution_ + 1}/{len(self.solutions_)}')

    
if __name__ == '__main__':
    os.mkdir('dlxtmp')

    root = tk.Tk()
    root.title('Soludle')
    app = KanoodleTK(root)
    app.pack()

    button_frame = tk.Frame(root)
    button_frame.pack(side=tk.BOTTOM)

    reset = tk.Button(button_frame, text='Reset', command=app.reset_board)
    reset.pack(side=tk.LEFT, padx=5, pady=10)

    solve = tk.Button(button_frame, text='Solve', command=app.solve)
    solve.pack(side=tk.LEFT, padx=5, pady=10)

    root.mainloop()

    os.system('rm -rf ./dlxtmp')
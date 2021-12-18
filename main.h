#pragma once
#include <iostream>
#include <exception>

template <typename type>
class Grid
{
private:
	type** memory;                      
	size_t x_size, y_size;

	Grid<type>** sub_memory;           // хранятся подсетки
    bool** is_sub;						//хранит значения для каждой ячейки сетки, есть ли там подсетка

	bool check(size_t id, bool isRow) const;   //проверка корректности индексов (первое значение - индекс, второе - находится ли этот индекс в ряде (х) или нет)

	void set_Grid(type amount, std::istream* use_stream = nullptr); //устанавливаются значения, равные amount, либо вводятся с консоли (файла)
																	//указатель на поток нужен для того, чтобы была возможность использовать null значения

	std::string separator;

public:

	Grid(size_t x_size=0, size_t y_size=0, std::string separator = "");
	Grid(const Grid<type> &grid); //копирование
	Grid(Grid<type> &&grid); //перемещение
	~Grid();

	type operator()(size_t x_idx, size_t y_idx) const; //копирование значений объекта
	type& operator()(size_t x_idx, size_t y_idx); //сам объект
	
	size_t get_xsize() const { return this->x_size; };
	size_t get_ysize() const { return this->y_size; };

	Grid& operator=(type);
	Grid& operator=(const Grid<type> &grid); //копирование
	Grid& operator=(Grid<type> &&grid); // перемещение

	friend std::ostream& operator<<(std::ostream& out, Grid const& grid)
	{
		int sub_num = 0;

		for (size_t y = 0; y < grid.get_ysize(); y++) 
		{
			for (size_t x = 0; x < grid.get_xsize(); x++)
			{
                if (grid.is_sub[x][y]) {
                    sub_num++;
                    out << "S" << sub_num << grid.separator; 
                } else {
				    out << grid.memory[x][y] << grid.separator;  
                }
			}
            out << "\n";
		}
		sub_num = 0;
		for (size_t y = 0; y < grid.get_ysize(); y++) 
		{
			for (size_t x = 0; x < grid.get_xsize(); x++)
			{
                if (grid.is_sub[x][y]) {
                    sub_num++;
                    out << "S" << sub_num << ":\n" << grid.sub_memory[x][y];
                }
			}
		}
	} 
	void set_separator(std::string separator);

	friend std::istream& operator>>(std::istream& in, Grid& grid) 
	{
		for (size_t i = 0; i < grid.x_size; i++) {
        	for (size_t j = 0; j < grid.y_size; j++)
            	grid.is_sub[i][j] = false;
    	}
		grid.set_Grid(type(), (&in));
		return in;
	}
	
	Grid& make_subgrid(
        size_t x_idx, size_t y_idx,
        size_t x_sub_size, size_t y_sub_size)
    {
		Grid<type>& a = sub_memory[x_idx][y_idx]; //ссылка на подсетку, которая будет добавляться или изменяться

		is_sub[x_idx][y_idx] = true;

		Grid<type> sample(x_sub_size, y_sub_size, separator);
		a = sample;

        return sub_memory[x_idx][y_idx];
    }

	Grid& collapse_subgrid(size_t x_idx, size_t y_idx)
    {
        if (is_sub[x_idx][y_idx]) {
            is_sub[x_idx][y_idx] = false;
            Grid<type> &a = sub_memory[x_idx][y_idx];
            type summ = type();
            for (int i = 0; i < a.x_size; i++) {
                for (int j = 0; j < a.y_size; j++) {
                    summ += a.memory[i][j];                    
                }
            }
            memory[x_idx][y_idx] = summ / (a.x_size*a.y_size);
            return sub_memory[x_idx][y_idx];
        }
    }
	Grid& get_subgrid(size_t x_idx, size_t y_idx) 
    {
        if (is_sub[x_idx][y_idx]) {
            return sub_memory[x_idx][y_idx];
        }    
    }

	bool is_subgrid(size_t x_idx, size_t y_idx) const;

	
};


template<class type>
Grid<type>::Grid(size_t x_size, size_t y_size, std::string separator)
	: x_size(x_size), y_size(y_size), separator(separator)
{
	if (x_size != 0 && y_size != 0) {
		this->memory = new type * [x_size];
    	this->sub_memory = new Grid<type> * [x_size];
    	this->is_sub = new bool * [x_size];
		for (size_t i = 0; i < x_size; i++) {
			this->memory[i] = new type[y_size]();
        	this->sub_memory[i] = new Grid<type>[y_size]();
        	this->is_sub[i] = new bool [y_size]();
    	}
    	for (size_t i = 0; i < x_size; i++) {
        	for (size_t j = 0; j < y_size; j++)
            	this->is_sub[i][j] = false;
    	}
	}
}
template<class type>
Grid<type>::Grid(const Grid<type> &grid)
	: x_size(grid.x_size), y_size(grid.y_size), separator(grid.separator)
{
	if (grid.x_size != 0 && grid.y_size != 0) {
		this->memory = new type * [grid.x_size];
    	this->sub_memory = new Grid<type> * [grid.x_size];
    	this->is_sub = new bool * [grid.x_size];
		for (size_t i = 0; i < x_size; i++) {
			this->memory[i] = new type[grid,y_size]();
        	this->sub_memory[i] = new Grid<type>[grid.y_size]();
        	this->is_sub[i] = new bool [grid.y_size]();
    	}
    	for (size_t i = 0; i < x_size; i++) {
        	for (size_t j = 0; j < y_size; j++) {
            	this->is_sub[i][j] = grid.is_sub[i][j];
				this->memory[i][j] = grid.memory[i][j];
				if (grid.is_sub[i][j]) {
					Grid<type> &source = grid.sub_memory[i][j];
					Grid<type> &cur = this->make_subgrid(i, j, source.get_xsize(), source.get_ysize());
					cur = source;
				}
			}
    	}
	}	
}

template<class type>
Grid<type>::Grid(Grid<type> &&grid)
	: x_size(grid.x_size), y_size(grid.y_size), separator(grid.separator), memory(grid.memory), sub_memory(grid.sub_memory), is_sub(grid.is_sub)
{
	std::cout << "move build\n";
	grid.x_size = 0;
	grid.y_size = 0;
	grid.separator = '\0';
	grid.memory = nullptr;
	grid.sub_memory = nullptr;
	grid.is_sub = nullptr;
}

template<class type>
Grid<type>::~Grid()
{
	if (x_size) {
		for (size_t i = 0; i < x_size; i++) {
        	delete[] this->memory[i];
			delete[] this->sub_memory[i];
        	delete[] this->is_sub[i];
    	}
		delete[] memory;
    	delete[] sub_memory;
    	delete[] is_sub;
	}
}

template<class type>
type Grid<type>::operator()(size_t x_idx, size_t y_idx) const
{
	if (!check(x_idx, true)) return type();
	if (!check(y_idx, false)) return type();

	if (!is_sub[x_idx][y_idx])
		return memory[x_idx][y_idx];
}

template<class type>
bool Grid<type>::check(size_t id, bool isRow) const
{
	if (id < 0) return false;
	if (isRow)
	{
		if (id >= x_size) return false;
	}
	else
	{
		if (id >= y_size) return false;
	}
	return true;
}

template<class type>
type& Grid<type>::operator()(size_t x_idx, size_t y_idx)
{
	if (!check(x_idx, true)) throw std::length_error("incorrect index x");
	if (!check(y_idx, false)) throw std::length_error("incorrect index y");
	if (!is_sub[x_idx][y_idx])
		return memory[x_idx][y_idx];
}

template<class type>
Grid<type>& Grid<type>::operator=(type amount)
{
	for (size_t i = 0; i < x_size; i++) {
        for (size_t j = 0; j < y_size; j++)
            this->is_sub[i][j] = false;
    }
	set_Grid(amount);
	return *this;
}

template<class type>
Grid<type>& Grid<type>::operator=(const Grid<type> &grid)
{
	if (&grid == this)
		return *this;
	if (x_size != 0 && y_size != 0) {
		for (size_t i = 0; i < x_size; i++) {
        	delete[] this->memory[i];
			delete[] this->sub_memory[i];
        	delete[] this->is_sub[i];
    	}
		delete[] memory;
    	delete[] sub_memory;
    	delete[] is_sub;
	}

	this->x_size = grid.x_size;
	this->y_size = grid.y_size;
	this->separator = grid.separator;

	if (grid.x_size != 0 && grid.y_size != 0) {
		this->memory = new type * [grid.x_size];
    	this->sub_memory = new Grid<type> * [grid.x_size];
    	this->is_sub = new bool * [grid.x_size];
		for (size_t i = 0; i < x_size; i++) {
			this->memory[i] = new type[grid,y_size]();
        	this->sub_memory[i] = new Grid<type>[grid.y_size]();
        	this->is_sub[i] = new bool [grid.y_size]();
    	}
    	for (size_t i = 0; i < x_size; i++) {
        	for (size_t j = 0; j < y_size; j++) {
            	this->is_sub[i][j] = grid.is_sub[i][j];
				this->memory[i][j] = grid.memory[i][j];
				if (grid.is_sub[i][j]) {
					Grid<type> &source = grid.sub_memory[i][j];
					Grid<type> &cur = this->make_subgrid(i, j, source.get_xsize(), source.get_ysize());
					cur = source;
				}
			}
    	}
	}
	return *this;
}

template<class type>
Grid<type>& Grid<type>::operator=(Grid<type> &&grid)
{
	std::cout << "move = \n";
	if (&grid == this)
		return *this;
	if (x_size != 0 && y_size != 0) {
		for (size_t i = 0; i < x_size; i++) {
        	delete[] this->memory[i];
			delete[] this->sub_memory[i];
        	delete[] this->is_sub[i];
    	}
		delete[] memory;
    	delete[] sub_memory;
    	delete[] is_sub;
	}

	this->x_size = grid.x_size;
	this->y_size = grid.y_size;
	this->separator = grid.separator;

	memory = grid.memory;
	sub_memory = grid.sub_memory;
	is_sub = grid.is_sub;

	grid.x_size = 0;
	grid.y_size = 0;
	grid.separator = '\0';
	grid.memory = nullptr;
	grid.sub_memory = nullptr;
	grid.is_sub = nullptr;
	return *this;	
}

template<class type>
void Grid<type>::set_Grid(type amount, std::istream* use_stream)
{
	for (size_t y = 0; y < y_size; y++)
	{
		for (size_t x = 0; x < x_size; x++)
		{
			if (use_stream == nullptr)
			{
				memory[x][y] = amount;
			}
			else
			{
				(*use_stream) >> memory[x][y];
			}
		}
	}
}

template<class type>
void Grid<type>::set_separator(std::string separator)
{
	this->separator = separator;
}

template<class type>
bool Grid<type>::is_subgrid(size_t x_idx, size_t y_idx) const
{
    if (!check(x_idx, true)) return false;
	if (!check(y_idx, false)) return false;
    return is_sub[x_idx][y_idx];
}

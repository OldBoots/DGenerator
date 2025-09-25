# Компилятор
CXX = g++

# Флаги компиляции
CXXFLAGS = -Wall -Wextra -std=c++20

# Имя исполняемого файла
TARGET = app

# Автоматический поиск исходных файлов
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = $(wildcard *.hpp)

# Основная цель - сборка программы
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Правило для компиляции каждого .cpp файла в .o
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка собранных файлов
clean:
	rm -f $(OBJECTS) $(TARGET)

# Пересборка
rebuild: clean $(TARGET)

# Отладка
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Запуск
run: $(TARGET)
	./$(TARGET)

# Помощь
help:
	@echo "Доступные цели:"
	@echo "  all       - собрать программу (по умолчанию)"
	@echo "  clean     - удалить объектные файлы и исполняемый файл"
	@echo "  rebuild   - пересобрать программу"
	@echo "  debug     - собрать с отладочной информацией"
	@echo "  run       - собрать и запустить программу"

# Псевдоним для основной цели
all: $(TARGET)

.PHONY: all clean rebuild debug run install help
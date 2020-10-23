BIN=fillsize.exe

ALL:
	$(CXX) $(CFLAGS) -o $(BIN)  -std=c++17 -s -O3 main.cpp fillsize.cpp mergeBin.cpp alignBin.cpp ddBin.cpp --static

clean:
	rm -f $(BIN)

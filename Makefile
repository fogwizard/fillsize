BIN=fillsize.exe

ALL:
	$(CXX) $(CFLAGS) -o $(BIN)  -s -O3 main.cpp fillsize.cpp mergeBin.cpp alignBin.cpp --static

clean:
	rm -f $(BIN)

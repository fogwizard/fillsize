BIN=fillsize.exe

ALL:
	$(CXX) $(CFLAGS) -o $(BIN)  -s -O3 fillsize.cpp --static

clean:
	rm -f $(BIN)

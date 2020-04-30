SRC=$(wildcat *.cpp)
HEADER=$(wildcat *.h)

pre_create:$(SRC) $(HEADER)
	g++ -o pre_create PreCreate.cpp ../Server.cpp ThreadPool.cpp -lboost_system -lboost_thread

.Phony: clean

clean:
	rm -f *~ *.o *.out pre_create

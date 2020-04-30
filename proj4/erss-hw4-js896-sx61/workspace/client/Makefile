TARGET=client

all: $(TARGET)
clean:
	rm -f $(TARGET)
client: Client.cpp
	g++ -std=c++11 -g -o $@ $< -pthread

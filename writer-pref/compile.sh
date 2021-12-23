if [ ! -d "./build" ]; then
    mkdir build
fi

# compile reader and writer
g++ -I ../headers/ reader.cpp ../source/*.cpp -o ./build/reader
g++ -I ../headers/ writer.cpp ../source/*.cpp -o ./build/writer
touch ./build/data.txt
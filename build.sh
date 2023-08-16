# sudo apt install libxtst-dev (should also install libx11-dev and libxext-dev)

mkdir -p out

if [ "$1" == "-d" ]; then
    echo "Debug build"
    g++ -O0 -g src/main.cpp -o out/not-suspicious -lX11 -lXfixes
else
    echo "Supah dupah fast build"
    g++ -O3 src/main.cpp -o out/not-suspicious -lX11 -lXfixes
fi

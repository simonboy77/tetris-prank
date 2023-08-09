# sudo apt install libxext-dev
# sudo apt install libxtst-dev

if [ "$1" == "-d" ]; then
    echo "Debug build"
    g++ -O0 -g src/main.cpp -o out/not-suspicious -lX11 -lXfixes
else
    echo "Supah dupah fast build"
    g++ -O3 src/main.cpp -o out/not-suspicious -lX11 -lXfixes
fi

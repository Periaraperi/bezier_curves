libs := -lSDL2 

output := out

include_dir := ./include
src_dir := ./src

objects := main.o\
		   $(patsubst %.cpp,%.o,$(wildcard $(src_dir)/*.cpp))

rebuildables := $(objects) $(output)

$(output): $(objects)
	g++ $(objects) -o $(output) $(libs)

%.o: %.cpp
	g++ -I $(include_dir) -o $@ -c $<

.PHONEY: clean
clean:
	rm $(rebuildables)

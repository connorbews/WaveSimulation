a: hello_triangle.o shaderClass.o VBO.o VAO.o EBO.o stb.o Texture.o
	g++ -o a.out -L./lib hello_triangle.o shaderClass.o VBO.o VAO.o EBO.o stb.o Texture.o src/glad.c -lGL -lGLU -lglfw3

hello_triangle.o: hello_triangle.cpp
	g++ -I./include -c hello_triangle.cpp

shaderClass.o: src/shaderClass.cpp include/shaderClass.h
	g++ -I./include -c src/shaderClass.cpp

VBO.o: src/VBO.cpp include/VBO.h
	g++ -I./include -c src/VBO.cpp

VAO.o: src/VAO.cpp include/VAO.h
	g++ -I./include -c src/VAO.cpp

EBO.o: src/EBO.cpp include/EBO.h
	g++ -I./include -c src/EBO.cpp

stb.o: src/stb.cpp include/stb/stb_image.h
	g++ -I./include -c src/stb.cpp

Texture.o: src/Texture.cpp include/Texture.h
	g++ -I./include -c src/Texture.cpp

clean:
	$(RM) a.out *.o

a: hello_triangle.cpp
	g++ hello_triangle.cpp glad.c -ldl -lglfw

clean: 
	$(RM) a.out

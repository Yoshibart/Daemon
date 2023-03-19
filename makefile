CC=gcc
objects = server_daemon.o client_daemon.o
# headers = areaOfRectangle.h areaOfSquare.h areaOfCircle.h area_of_triangle.h 

# myprog : $(objects)
# 	$(CC) -o areaProg $(objects) 

client : client_daemon.o
	$(CC) -o client client_daemon.o

server : server_daemon.o
	$(CC) -o server server_daemon.o

client_daemon.o : client_daemon.c
	$(CC) -c client_daemon.c

server_daemon.o : server_daemon.c
	$(CC) -c server_daemon.c

# areaOfRectangle.o : areaOfRectangle.c
# 	$(CC) -c areaOfRectangle.c

# areaOfSquare.o  :areaOfSquare.c 
# 	$(CC) -c areaOfSquare.c

# areaOfCircle.o : areaOfCircle.c
# 	$(CC) -c areaOfCircle.c

# area_of_triangle.o : area_of_triangle.c
# 	$(CC) -c area_of_triangle.c 

clean:
# 	rm areaProg $(objects)
	rm $(objects) client server




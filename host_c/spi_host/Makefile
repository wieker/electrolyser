#Makefile stays simple for this example

all:
	gcc spi_lib.c main.c -o host -lftdi
	gcc -fPIC -shared spi_lib.c main.c java_bridge.c -o libspihost.so  -lftdi -I/usr/lib/jvm/java-25-openjdk-amd64/include -I/usr/lib/jvm/java-25-openjdk-amd64/include/linux

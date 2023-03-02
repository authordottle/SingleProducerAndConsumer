all:
	gcc -o producer producer.c -lrt
	gcc -o consumer consumer.c -lrt

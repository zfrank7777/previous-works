all:
	gcc server.c -o server
	gcc file_reader.c -o file_reader1
	gcc file_reader.c -D Sleep -o file_reader2
	gcc mmap_read.c -o mmap_read	
	gcc mmap_write.c -o mmap_write
clean:
	rm -f server file_reader1 file_reader2 mmap_read mmap_write

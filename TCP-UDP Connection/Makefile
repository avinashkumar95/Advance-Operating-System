REPORT = CIS620PROJECT3REPORT.docx

All: server client servicemap

server: server.o
	cc server.o -o server

client: client.o
	cc client.o -o client

servicemap: servicemap.o
	cc servicemap.o -o servicemap

clean:
	rm *.o server client servicemap
	
submit:
	turnin -c cis620s -p proj3 client.c server.c servicemap.c Makefile ${REPORT}
all: taska taskb taskc




taska: getsyms getsections




taskb: getsyms_dl getsections_dl




taskc: copysection




clean:

	rm -rf ./lib objsect.o objsym.o getsections getsyms libobjdata.a libobjdata.so getsections_dl objcopy.o text-output objcopy getsyms_dl copysection




libobjdata.so:

	gcc -c -fPIC -o objsect.o objsect.c -lbfd

	gcc -c -fPIC -o objsym.o objsym.c -lbfd

	gcc -shared -o libobjdata.so objsect.o objsym.o	-lbfd




libobjdata.a: objsect.o objsym.o objcopy.o

	ar rs libobjdata.a objsect.o objsym.o objcopy.o

	mkdir -p lib

	cp -f libobjdata.a lib




getsyms_dl: libobjdata.so

	gcc getsyms_dl.c -L. -lobjdata -o getsyms_dl -lbfd




getsections_dl: libobjdata.so

	gcc getsections_dl.c -L. -lobjdata -o getsections_dl -lbfd




getsyms: getsyms.c libobjdata.a

	gcc -I./include -L./lib -o getsyms getsyms.c -lobjdata -lbfd




getsections: getsections.c libobjdata.a

	gcc -I./include -L./lib -o getsections getsections.c -lobjdata -lbfd




copysection: copysection.c libobjdata.a

	gcc -I./include -L./lib -o copysection copysection.c -lobjdata -lbfd




objsect.o: objsect.c

	gcc -c -o objsect.o objsect.c -lbfd




objsym.o: objsym.c

	gcc -c -o objsym.o objsym.c -lbfd




objcopy.o: objcopy.c

	gcc -c -o objcopy.o objcopy.c -lbfd




sect:

	gcc objsect.c -o objsect -lbfd




sym:

	gcc objsym.c -o objsym -lbfd




copy:

	gcc objcopy.c -o objcopy -lbfd
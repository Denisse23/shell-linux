
mi_sh: mi_sh.o comandosEje
	g++ mi_sh.cpp -o mi_sh 

mi_sh.o: mi_sh.cpp
	g++ -c mi_sh.cpp

comandosEje: comandosEje.o
	g++ comandosEje.cpp -o comandosEje

comandosEje.o: comandosEje.cpp
	g++ -c comandosEje.cpp




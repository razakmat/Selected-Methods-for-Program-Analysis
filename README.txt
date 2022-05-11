First build project by running ./build.sh

Run interpreter:
	./uc run [--ascii] ./name_of_file.uc
	
Type Analysis:
	./uc type ./name_of_file.uc
	
Dataflow Analysis:
	./uc ( cfg | sign | const | live | available | busy | reaching ) [--norm] ./name_of_file.uc
	
Points-to Analysis:
	./uc ( Andersen | Steensgaard ) ./name_of_file.uc

// Null analysis by default uses Andersen analysis
Null Analysis:
	./uc Null [--Steensgaard] ./name_of_file.uc

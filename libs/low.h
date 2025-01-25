#pragma once


// Processes
const char* PS_KLIENT_PATH = "build/klient";
const char* PS_KLIENT_NAME  = "./build/klient";

const char* PS_KASJER_PATH = "build/kasjer"; 
const char* PS_KASJER_NAME  = "./build/kasjer";

const char* PS_RATOWNIK_PATH = "build/ratownik"; 
const char* PS_RATOWNIK_NAME  = "./build/ratownik";


// Message Queue
struct MSQ_BUFFER {
	long mtype;
	int  mvalue;
} typedef MsqBuffer;
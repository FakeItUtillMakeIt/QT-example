#pragma once
#include "Rocketmodule.h"
#include<qlist.h>

class Rocketmodule;
class twoDdisplay;


class Dataspace
{
public:Dataspace();
	  ~Dataspace();
	  long base_moduleid;
	  long base_Dimgid;
	  long base_Dcompid;
	  long base_lineid;
	  long base_areaid;
	  QList<Rocketmodule*> rmlist;

};


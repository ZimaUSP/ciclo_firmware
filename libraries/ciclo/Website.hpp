#ifndef __WEBSITE_HPP__
#define __WEBSITE_HPP__

#include "Arduino.h"
#include <WebServer.h>

class WEBSITE{
  public:
	String websiteChart();
    String websiteResistivo();
    String websiteNormal();
    String websitePassivo();
    String websiteTESTE();
    String websiteGRAFICOS();
};

#endif 

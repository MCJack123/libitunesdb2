//
//  libitunesdb2.cpp
//  libitunesdb2
//
//  Created by Jack Bruienne on 12/28/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

#include <iostream>
#include "libitunesdb2.hpp"
#include "libitunesdb2Priv.hpp"

void libitunesdb2::HelloWorld(const char * s)
{
    libitunesdb2Priv *theObj = new libitunesdb2Priv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

void libitunesdb2Priv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};


/***************************************************************************
 *   Copyright (C) 2007 by Mian Zhou   *
 *   M.Zhou@reading.ac.uk   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "cvpoolparams.h"
#include "cvferet.h"
#include "cvbingabadafeatureselect.h"

int main(int argc, char *argv[])
{
  int id = 29;
  const char * filelist = "/local/FaceDB/FERET/fb/filelist.txt";
  const char * resultfile = "ftestresults.txt";

  
  int nofeatures = 0;
  bool allfeatures = true;
  
  int width = 64;
  int height = 64;
  int bound = 0;
  int interval = 0;
  int norientations = 8;
  int minscale = -1;
  int maxscale = 3;
  bool reduced = true;
  
  char *inputname = new char[100];
  char *outputname = new char[100];
  strcpy(inputname, "/local/FaceDB/FERET/fb/filelist.txt");
  strcpy(outputname, "testingresults.txt");
  char *weaksname = new char[100];
  strcpy(weaksname, "weaks.xml");
  
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-weaks" ) )
    {
      strcpy(weaksname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-input" ) )
    {
      strcpy(inputname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-output" ) )
    {
      strcpy(outputname, argv[++i]);
    }
    else if( !strcmp( argv[i], "-nofeatures" ) )
    {
      nofeatures = atoi( argv[++i] );
      allfeatures = false;
    }
    else if( !strcmp( argv[i], "-subject" ) )
    {
      id = atoi( argv[++i] );
    }
  }
  
  CvPoolParams param(height, width, minscale, maxscale, norientations, interval, bound, reduced);
  
  CvFeret feret("/windows/D/Data/feret/", "/local/FaceDB/FERET/fa/PPMS/", "/local/FaceDB/FERET/fb/PPMS/");
  
  CvBinGabAdaFeatureSelect FeatureSelec( &feret, &param, id );
  FeatureSelec.loadweaks( weaksname );
  if (allfeatures) FeatureSelec.testing( inputname, outputname );
  else FeatureSelec.testing(inputname, outputname, nofeatures);
  delete [] weaksname;
  delete [] inputname;
  delete [] outputname;
  return 0;
  
}


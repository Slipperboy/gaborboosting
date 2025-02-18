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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



#include <iostream>
#include <cstdlib>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <ml.h>
#include <sys/stat.h>
#include <vector>
#include "cvxm2vts.h"
#include "cvpoolparams.h"
#include "cvbingabadafeatureselect.h"
#include <cvgaborclientdatamaker.h>
#include <cvadaboostclientfeatureselection.h>


using namespace PrepareData;


int main(int argc, char *argv[])
{
  char dbpath[100] = "";
  char datapath[100] = "";
  
  // default setting 
  int height = 0;
  int width = 0;
  int minscale = -1;
  int maxscale = 3;
  int norientations = 8;
  int interval = 0;
  int bound = 0;
  bool reduced = false;

  int nweaks = 500;
  int weak_type = CvWeakLearner::POTSU;
  int db_type = CvGaborResponseData::FERET;
  
  int client = 29;
  for(int i = 1; i < argc; i++ )
  {
    if( !strcmp( argv[i], "-n" ) )
    {
      nweaks= atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-dbpath" ) )
    {
      strcpy( dbpath, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-datapath" ) )
    {
      strcpy( datapath, argv[++i] );
    }
    else if ( !strcmp( argv[i], "-interval" ) )
    {
      interval = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-client" ) )
    {
      client = atoi( argv[++i] );
    }
    else if ( !strcmp( argv[i], "-XM2VTS" ))
    {
      db_type = CvGaborResponseData::XM2VTS;
      if( !strcmp( dbpath, "" ) )
        strcpy( dbpath, "/home/sir02mz/XM2VTS");
      if( !strcmp( datapath, "" ) )
        strcpy( datapath, "/home/sir02mz/OUTPUT");
    }
    else if ( !strcmp( argv[i], "-FERET" ))
    {
      db_type = CvGaborResponseData::FERET;
      if( !strcmp( dbpath, "" ) )
        strcpy( dbpath, "/home/sir02mz/FERET");
      if( !strcmp( datapath, "" ) )
        strcpy( datapath, "/home/sir02mz/Meta");
    }
    else if ( !strcmp( argv[i], "-type" ) )
    {
      if ( !strcmp("fld", argv[++i])) 
        weak_type = CvWeakLearner::FLD;
      else if ( !strcmp("svm", argv[i] )) 
        weak_type = CvWeakLearner::SVM;
      else if ( !strcmp("potsu", argv[i] )) 
        weak_type = CvWeakLearner::POTSU;
      else if ( !strcmp("ann", argv[i] )) 
        weak_type = CvWeakLearner::ANN;
    }
  }

  CvFaceDB *database;
  CvSize size;
  if( db_type == CvGaborResponseData::XM2VTS )
  {
    CvXm2vts * xm2vts = new CvXm2vts( dbpath );
    xm2vts->setNumSub( 200 );
    xm2vts->setNumPic( 4 );
    size = xm2vts->getSize();
    height = size.height;
    width = size.width;
    database = xm2vts;
    

  }
  else if( db_type == CvGaborResponseData::FERET)
  {
    sprintf(datapath, "%s/FA", datapath);
    char fapath[100];
    char fbpath[100];
    sprintf(fapath, "%s/FA", dbpath);
    sprintf(fbpath, "%s/FB", dbpath);
    CvFeret *feret = new CvFeret("", fapath, fbpath);
    size = feret->getSize();
    height = size.height;
    width = size.width;

    database = feret;
  }
  
  CvPoolParams param(size, minscale, maxscale, norientations, interval, bound, reduced);
  
 
  

  CvGaborResponseData GaborData( database, &param, datapath );
  //CvGaborResponseData GaborData( database, &param, "/home/sir02mz/Meta/FA");

  // get the labels of the training data
  CvGaborFeature feature(1,1,1,1);
  CvGaborClientDataMaker maker(&GaborData, &feature, database, client);
  CvMat* labels = maker.getLabels();



  CvAdaBoostClientFeatureSelection fs( &GaborData, labels, &param, weak_type, client);
  CvGaborFeaturePool *newfeatures = fs.Select( nweaks );
  newfeatures->write("newfeatures.txt");
  delete newfeatures;

  delete database;
  return EXIT_SUCCESS;
}


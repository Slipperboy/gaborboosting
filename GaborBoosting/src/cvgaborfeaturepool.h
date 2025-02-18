/***************************************************************************
 *   Copyright (C) 2006 by Mian Zhou   *
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
#ifndef CVGABORFEATUREPOOL_H
#define CVGABORFEATUREPOOL_H
#include <vector>
#include "cvgaborfeature.h"
#include "cvpoolparams.h"

#define FAST_INDEX 1
using namespace std;
/**
@author Mian Zhou
*/
class CvGaborFeaturePool{
public:
    CvGaborFeaturePool();

    ~CvGaborFeaturePool();
    int Init(CvSize imgsize, CvMat *Scale, CvMat *Orientation, int interval, int bound);
    int Init(CvSize imgsize, int minscale, int maxscale, int norient, int interval, int bound);
    void clear();
    int getSize() const;
    void add(CvGaborFeature *gabor);
    void remove(int idex);
    CvGaborFeature* getfeature(int idex);
    void sort();
    bool SortPredicate(const CvGaborFeature& lhs, const CvGaborFeature& rhs);
    CvGaborFeature* min();
    void load(const char *filename);
    CvGaborFeaturePool* clone();
    int Init(CvPoolParams *param);
    void write(const char* filename);
    bool isReduced();
    int getIndex(CvGaborFeature *feature);
    CvGaborFeaturePool* range(int start, int end);
    void addfile(const char *filename);
    bool isIn(CvGaborFeature *feature);
    void load_a(const char* filename);
    bool remove(CvGaborFeature *feature);
    vector<CvGaborFeature> features;
    vector<CvGaborFeature>::iterator pos;

protected:
    int count;
    
    vector<int> index;
    vector<double> errors;
    CvSize winsize;
    bool isInitialised;
    bool reduce;
};

#endif

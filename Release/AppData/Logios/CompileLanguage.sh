#!/bin/bash
# Compile a language knowledge base from a grammar
# [20080707] (air)

#   ::  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  ::::::
#   ::  FIRST: Copy this script into your Project root folder                             ::::::
here="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#   ::  SECOND: Change the following line to point to your Logios installation            ::::::
#   ::  or have the environment variable set appropriately                                ::::::
cd "$( dirname "${BASH_SOURCE[0]}" )"
cd ../../../Library_Logios
LOGIOS_ROOT=$(pwd)

#   ::  CHANGE THESE LABELS AS PER YOUR PROJECT; OUTPUT FILES WILL BE NAMED ACCORDINGLY   ::::::

PROJECT="Kade"
INSTANCE="Kade"

#   ::  ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  ::::::

#   :: Compile language knowledge bases from the grammar
#   :: --inpath and --outpath give you control over actual locations
    INPATH=$here/src
    OUTPATH=$here/Grammar
    mkdir -p $OUTPATH
    perl $LOGIOS_ROOT/Tools/MakeLanguage/make_language.pl \
	--logios $LOGIOS_ROOT \
	--inpath $INPATH --outpath $OUTPATH \
	--project $PROJECT --instance $INSTANCE \
   --force
#


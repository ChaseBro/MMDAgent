/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2001                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  Simple top level program                                             */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "MMDAgent.h"

#include "flite.h"

#include "Flite.h"


extern "C" {
/* Its not very appropriate that these are declared here */
void usenglish_init(cst_voice *v);
cst_lexicon *cmu_lex_init(void);
}

/* Flite initialize */
void Flite::initialize()
{
   flite_init();
   flite_add_lang("eng",usenglish_init,cmu_lex_init);
}

/* Flite clear */
void Flite::clear()
{

}

Flite::Flite()
{
   initialize();
}

Flite::~Flite()
{
   clear();
}

/* Basic synthesize function, outputs directly to audio device, returns the duration of the synthesized text */
float Flite::synthesize(char *voice_name, char *text)
{
   cst_voice *voice = flite_voice_select(voice_name);
   return flite_text_to_speech(text,voice,"play");
}

bool Flite::load(char **modelNames, int numModels)
{
   int i;
   cst_voice *voice;
   char *modelDir;

   for (i = 0; i < numModels; i++)
   {
      modelDir = MMDAgent_pathdup(modelNames[i]);
      voice = flite_voice_load(modelDir);
      if (voice != NULL)
         flite_add_voice(voice);
      else
      {
         printf("load failed - %s", modelNames[i]);
         return false;
      }
   }
   return true;
}

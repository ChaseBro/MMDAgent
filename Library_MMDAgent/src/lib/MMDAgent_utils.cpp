/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/* headers */

#ifndef _WIN32
#include <unistd.h>
#include <dlfcn.h>
#include <dirent.h>
#endif /* !_WIN32 */
#include "MMDAgent.h"

/* MMDAgent_getcharsize: get character size */
char MMDAgent_getcharsize(const char *str)
{
   return MMDFiles_getcharsize(str);
}

/* MMDAgent_strequal: string matching */
bool MMDAgent_strequal(const char *str1, const char *str2)
{
   return MMDFiles_strequal(str1, str2);
}

/* MMDAgent_strheadmatch: match head string */
bool MMDAgent_strheadmatch(const char *str1, const char *str2)
{
   return MMDFiles_strheadmatch(str1, str2);
}

/* MMDAgent_strtailmatch: match tail string */
bool MMDAgent_strtailmatch(const char *str1, const char *str2)
{
   return MMDFiles_strtailmatch(str1, str2);
}

/* MMDAgent_strlen: strlen */
int MMDAgent_strlen(const char *str)
{
   return MMDFiles_strlen(str);
}

/* MMDAgent_strdup: strdup */
char *MMDAgent_strdup(const char *str)
{
   return MMDFiles_strdup(str);
}

/* MMDAgent_pathdup: convert charset from application to system */
char *MMDAgent_pathdup(const char *str)
{
   return MMDFiles_pathdup(str);
}

/* MMDAgent_intdup: integer type strdup */
char *MMDAgent_intdup(const int digit)
{
   int i, size;
   char *p;

   if(digit == 0) {
      size = 2;
   } else {
      if(digit < 0) {
         size = 2;
         i = -digit;
      } else {
         size = 1;
         i = digit;
      }
      for (; i != 0; size++)
         i /= 10;
   }

   p = (char *) malloc(sizeof(char) * size);
   sprintf(p, "%d", digit);
   return p;
}

/* MMDAgent_dirname: get directory name from path */
char *MMDAgent_dirname(const char *file)
{
   return MMDFiles_dirname(file);
}

/* MMDAgent_basename: get file name from path */
char *MMDAgent_basename(const char *file)
{
   return MMDFiles_basename(file);
}

/* MMDAgent_fopen: get file pointer */
FILE *MMDAgent_fopen(const char *file, const char *mode)
{
   return MMDFiles_fopen(file, mode);
}

/* MMDAgent_strtok: strtok */
char *MMDAgent_strtok(char *str, const char *pat, char **save)
{
   char *s = NULL, *e = NULL, *p;
   const char *q;
   char mbc1[MMDAGENTUTILS_MAXCHARBYTE];
   char mbc2[MMDAGENTUTILS_MAXCHARBYTE];
   int find;
   int step = 0;
   unsigned char i, size;

   if(str != NULL)
      p = str;
   else if(save != NULL)
      p = *save;
   else
      return NULL;
   while(*p != '\0') {
      if(step == 0)
         s = p;
      if(step == 1)
         e = p;
      size = MMDAgent_getcharsize(p);
      for(i = 0; i < size; i++) {
         mbc1[i] = *p;
         if(*p == '\0') {
            i = 0;
            break;
         }
         p++;
      }
      mbc1[i] = '\0';
      /* search */
      find = 0;
      q = pat;
      while(*q != '\0') {
         size = MMDAgent_getcharsize(q);
         for(i = 0; i < size; i++) {
            mbc2[i] = *q;
            if(*q == '\0') {
               i = 0;
               break;
            }
            q++;
         }
         mbc2[i] = '\0';
         if(strcmp(mbc1, mbc2) == 0) {
            find = 1;
            break;
         }
      }
      /* check */
      if(step == 0) {
         if(find == 0)
            step = 1;
      } else {
         if(find == 1) {
            *e = '\0';
            *save = p;
            return s;
         }
      }
   }

   if(step == 1) {
      *save = p;
      return s;
   }

   *save = p;
   return NULL;
}

/* MMDAgent_str2bool: convert string to boolean */
bool MMDAgent_str2bool(const char *str)
{
   if(str == NULL)
      return false;
   else if(strcmp(str, "true") == 0)
      return true;
   else
      return false;
}

/* MMDAgent_str2int: convert string to integer */
int MMDAgent_str2int(const char *str)
{
   if(str == NULL)
      return 0;
   return atoi(str);
}

/* MMDAgent_str2float: convert string to float */
float MMDAgent_str2float(const char *str)
{
   if(str == NULL)
      return 0.0f;
   return (float) atof(str);
}

/* MMDAgent_str2double: convert string to double */
double MMDAgent_str2double(const char *str)
{
   if(str == NULL)
      return 0.0;
   return atof(str);
}

/* MMDAgent_str2ivec: convert string to integer vector */
bool MMDAgent_str2ivec(const char *str, int *vec, const int size)
{
   int i = 0;
   char *buff, *p, *save = NULL;

   if(str == NULL)
      return false;
   buff = MMDAgent_strdup(str);
   for(p = MMDAgent_strtok(buff, ",", &save); p && i < size; p = MMDAgent_strtok(NULL, ",", &save))
      vec[i++] = atoi(p);
   free(buff);
   if(i == size)
      return true;
   else
      return false;
}

/* MMDAgent_str2fvec: convert string to float vector */
bool MMDAgent_str2fvec(const char *str, float *vec, const int size)
{
   int i = 0;
   char *buff, *p, *save = NULL;

   if(str == NULL)
      return false;
   buff = MMDAgent_strdup(str);
   for(p = MMDAgent_strtok(buff, ",", &save); p && i < size; p = MMDAgent_strtok(NULL, ",", &save))
      vec[i++] = (float) atof(p);
   free(buff);
   if(i == size)
      return true;
   else
      return false;
}

/* MMDAgent_str2pos: get position from string */
bool MMDAgent_str2pos(const char *str, btVector3 *pos)
{
   float vec[3];

   if (MMDAgent_str2fvec(str, vec, 3) == false)
      return false;

   pos->setValue(vec[0], vec[1], vec[2]);

   return true;
}

/* MMDAgent_str2rot: get rotation from string */
bool MMDAgent_str2rot(const char *str, btQuaternion *rot)
{
   float vec[3];

   if (MMDAgent_str2fvec(str, vec, 3) == false)
      return false;

   rot->setEulerZYX(MMDFILES_RAD(vec[2]), MMDFILES_RAD(vec[1]), MMDFILES_RAD(vec[0]));

   return true;
}

/* MMDAgent_fgettoken: get token from file pointer */
int MMDAgent_fgettoken(FILE *fp, char *buff)
{
   int i;
   char c;

   c = fgetc(fp);
   if(c == EOF) {
      buff[0] = '\0';
      return 0;
   }

   if(c == '#') {
      for(c = fgetc(fp); c != EOF; c = fgetc(fp))
         if(c == '\n')
            return MMDAgent_fgettoken(fp, buff);
      buff[0] = '\0';
      return 0;
   }

   if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
      return MMDAgent_fgettoken(fp, buff);

   buff[0] = c;
   for(i = 1, c = fgetc(fp); c != EOF && c != '#' && c != ' ' && c != '\t' && c != '\r' && c != '\n'; c = fgetc(fp))
      buff[i++] = c;
   buff[i] = '\0';

   if(c == '#')
      fseek(fp, -1, SEEK_CUR);
   if(c == EOF)
      fseek(fp, 0, SEEK_END);

   return i;
}

/* MMDAgent_chdir: change current directory */
bool MMDAgent_chdir(const char *dir)
{
#ifdef _WIN32
   return SetCurrentDirectoryA(dir) != 0 ? true : false;
#else
   bool result;
   char *path;

   path = MMDAgent_pathdup(dir);
   result = chdir(path) == 0 ? true : false;
   free(path);

   return result;
#endif /* _WIN32 */
}

/* MMDAgent_sleep: sleep in ms */
void MMDAgent_sleep(double t)
{
   glfwSleep(t * 0.001);
}

/* MMDAgent_setTime: set time in ms */
void MMDAgent_setTime(double t)
{
   glfwSetTime(t * 0.001);
}

/* MMDAgent_getTime: get time in ms */
double MMDAgent_getTime()
{
   return glfwGetTime() * 1000.0;
}

/* MMDAgent_diffTime: get difference between two times */
double MMDAgent_diffTime(double now, double past)
{
   if (past > now)
      return past - now; /* timer overflow is not taken into account */
   else
      return now - past;
}

/* MMDAgent_dlopen: open dynamic library */
void *MMDAgent_dlopen(const char *file)
{
#ifdef _WIN32
   return (void *) LoadLibraryExA(file, NULL, 0);
#else
   char *path, *error;
   void *d;

   if(file == NULL)
      return NULL;

   path = MMDFiles_pathdup(file);
   d = dlopen(path, RTLD_NOW);
   free(path);
   error = dlerror();
   if (error != NULL)
      printf("Failed to load library: %s with error: %s\n", file, error);

   return d;
#endif /* _WIN32 */
}

/* MMDAgent_dlclose: close dynamic library */
void MMDAgent_dlclose(void *handle)
{
#ifdef _WIN32
   FreeLibrary((HMODULE) handle);
#else
   dlclose(handle);
#endif /* _WIN32 */
}

/* MMDAgent_dlsym: get function from dynamic library */
void *MMDAgent_dlsym(void *handle, const char *name)
{
#ifdef _WIN32
   return (void *) GetProcAddress((HMODULE) handle, name);
#else
   return dlsym(handle, name);
#endif /* _WIN32 */
}

/* MMDAgent_opendir: open directory */
DIRECTORY *MMDAgent_opendir(const char *name)
{
#ifdef _WIN32
   DIRECTORY *dir;

   if(name == NULL)
      return NULL;

   dir = (DIRECTORY *) malloc(sizeof(DIRECTORY));
   dir->data = malloc(sizeof(WIN32_FIND_DATAA));
   char name2[MMDAGENT_MAXBUFLEN];
   if(MMDAgent_strlen(name) <= 0) {
      strcpy(name2, "*");
   } else {
      sprintf(name2, "%s%c*", name, MMDAGENT_DIRSEPARATOR);
   }
   dir->find = FindFirstFileA(name2, (WIN32_FIND_DATAA *) dir->data);
   dir->first = true;
   if(dir->find == INVALID_HANDLE_VALUE) {
      free(dir->data);
      free(dir);
      return NULL;
   }
#else
   DIRECTORY *dir;
   char *path;

   if(name == NULL)
      return NULL;

   dir = (DIRECTORY *) malloc(sizeof(DIRECTORY));

   path = MMDFiles_pathdup(name);
   dir->find = (void *) opendir(path);
   free(path);
   if(dir->find == NULL) {
      free(dir);
      return NULL;
   }
#endif /* _WIN32 */

   return dir;
}

/* MMDAgent_closedir: close directory */
void MMDAgent_closedir(DIRECTORY *dir)
{
   if(dir == NULL)
      return;

#ifdef _WIN32
   FindClose(dir->find);
   free(dir->data);
#else
   closedir((DIR *) dir->find);
#endif /* _WIN32 */
   free(dir);
}

/* MMDAgent_readdir: find files in directory */
bool MMDAgent_readdir(DIRECTORY *dir, char *name)
{
#ifdef _WIN32
   WIN32_FIND_DATAA *dp;
#else
   struct dirent *dp;
#endif /* _WIN32 */

   if(dir == NULL || name == NULL) {
      strcpy(name, "");
      return false;
   }

#ifdef _WIN32
   if(dir->first == true) {
      dir->first = false;
      dp = (WIN32_FIND_DATAA *) dir->data;
      strcpy(name, dp->cFileName); /* if no file, does it work well? */
      return true;
   } else if(FindNextFileA(dir->find, (WIN32_FIND_DATAA *) dir->data) == 0) {
      strcpy(name, "");
      return false;
   } else {
      dp = (WIN32_FIND_DATAA *) dir->data;
      strcpy(name, dp->cFileName);
      return true;
   }
#else
   dp = readdir((DIR *) dir->find);
   if(dp == NULL) {
      strcpy(name, "");
      return false;
   } else {
      strcpy(name, dp->d_name);
      return true;
   }
#endif /* _WIN32 */
}

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

#include "MMDAgent.h"
#include "VIManager.h"

/* getTokenFromString: get token from string */
static int getTokenFromString(const char *str, int *index, char *buff)
{
   char c;
   int i = 0;

   c = str[(*index)];
   while (c == '\0' || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      if (c == '\0') {
         buff[0] = '\0';
         return 0;
      }
      c = str[++(*index)];
   }

   while (c != '\0' && c != ' ' && c != '\t' && c != '\n' && c != '\r') {
      buff[i++] = c;
      c = str[++(*index)];
   }

   buff[i] = '\0';
   return i;
}

/* getArgFromString: get argument from string using separators */
static int getArgFromString(const char *str, int *index, char *buff)
{
   char c;
   int i = 0;

   c = str[(*index)];
   while (c == '\0' || c == ' ' || c == '\t' || c == '\n' || c == '\r') {
      if (c == '\0') {
         buff[0] = '\0';
         return 0;
      }
      c = str[++(*index)];
   }

   while (c != '\0' && c != ' ' && c != '\t' && c != '\n' && c != '\r' && c != VIMANAGER_SEPARATOR1 && c != VIMANAGER_SEPARATOR2) {
      buff[i++] = c;
      c = str[++(*index)];
   }
   if (c == VIMANAGER_SEPARATOR1 || c == VIMANAGER_SEPARATOR2)
      (*index)++;

   buff[i] = '\0';
   return i;
}

/* getArgs: get event arguments */
static int getArgs(const char *str, char ***args, int *argc)
{
   int i, j, len, idx = 0;
   char buff[MMDAGENT_MAXBUFLEN];

   if (str == NULL) {
      (*argc) = 0;
      (*args) = NULL;
      return 0;
   }

   len = MMDAgent_strlen(str);
   if (len <= 0) {
      (*argc) = 0;
      (*args) = NULL;
      return 0;
   }

   /* get number of separator */
   (*argc) = 1;
   for (i = 0; i < len; i++)
      if (str[i] == VIMANAGER_SEPARATOR1 || str[i] == VIMANAGER_SEPARATOR2)
         (*argc)++;
   (*args) = (char **) calloc((*argc), sizeof(char *));
   for (i = 0; i < (*argc); i++)
      (*args)[i] = NULL;

   /* get event arguments */
   for (i = 0; i < (*argc); i++) {
      j = getArgFromString(str, &idx, buff);
      (*args)[i] = (char *) calloc(j + 1, sizeof(char));
      strcpy((*args)[i], buff);
   }

   return 1;
}

/* VIManager_Arc_initialize: initialize arc */
static void VIManager_Arc_initialize(VIManager_Arc *a, char *input_event_type, char **input_event_args, int input_event_argc, char *output_command_type, char *output_command_args, VIManager_State *next_state)
{
   int i;

   a->input_event_type = MMDAgent_strdup(input_event_type);
   if (input_event_argc <= 0) {
      a->input_event_args = NULL;
      a->input_event_argc = 0;
   } else {
      a->input_event_args = (char **) calloc(input_event_argc, sizeof(char *));
      for (i = 0; i < input_event_argc; i++)
         a->input_event_args[i] = MMDAgent_strdup(input_event_args[i]);
      a->input_event_argc = input_event_argc;
   }
   a->output_command_type = MMDAgent_strdup(output_command_type);
   a->output_command_args = MMDAgent_strdup(output_command_args);
   a->next_state = next_state;
   a->next = NULL;
}

/* VIManager_Arc_clear: free arc */
static void VIManager_Arc_clear(VIManager_Arc * a)
{
   int i;

   if (a->input_event_type != NULL)
      free(a->input_event_type);
   if (a->input_event_args != NULL) {
      for (i = 0; i < a->input_event_argc; i++)
         free(a->input_event_args[i]);
      free(a->input_event_args);
   }
   if (a->output_command_type != NULL)
      free(a->output_command_type);
   if (a->output_command_args != NULL)
      free(a->output_command_args);
   VIManager_Arc_initialize(a, NULL, NULL, 0, NULL, NULL, NULL);
}

/* VIManager_AList_initialize: initialize arc list */
static void VIManager_AList_initialize(VIManager_AList *l)
{
   l->head = NULL;
}

/* VIManager_AList_clear: free arc list */
static void VIManager_AList_clear(VIManager_AList *l)
{
   VIManager_Arc *tmp1, *tmp2;

   for (tmp1 = l->head; tmp1 != NULL; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      VIManager_Arc_clear(tmp1);
      free(tmp1);
   }
   l->head = NULL;
}

/* VIManager_State_initialize: initialize state */
static void VIManager_State_initialize(VIManager_State *s, unsigned int number, VIManager_State * next)
{
   s->number = number;
   VIManager_AList_initialize(&s->arc_list);
   s->next = next;
}

/* VIManager_State_clear: free state */
static void VIManager_State_clear(VIManager_State *s)
{
   VIManager_AList_clear(&s->arc_list);
   VIManager_State_initialize(s, 0, NULL);
}

/* VIManager_SList_initialize: initialize state list */
static void VIManager_SList_initialize(VIManager_SList *l)
{
   l->head = NULL;
}

/* VIManager_SList_clear: free state list */
static void VIManager_SList_clear(VIManager_SList *l)
{
   VIManager_State *tmp1, *tmp2;

   for (tmp1 = l->head; tmp1 != NULL; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      VIManager_State_clear(tmp1);
      free(tmp1);
   }
   l->head = NULL;
}

/* VIManager_SList_searchState: search state pointer */
static VIManager_State *VIManager_SList_searchState(VIManager_SList *l, unsigned int n)
{
   VIManager_State *tmp1, *tmp2, *result = NULL;

   if (l->head == NULL) {
      l->head = (VIManager_State *) calloc(1, sizeof(VIManager_State));
      VIManager_State_initialize(l->head, n, NULL);
      result = l->head;
   } else if (l->head->number == n) {
      result = l->head;
   } else if (l->head->number > n) {
      tmp1 = l->head;
      l->head = (VIManager_State *) calloc(1, sizeof(VIManager_State));
      VIManager_State_initialize(l->head, n, tmp1);
      result = l->head;
   } else {
      for (tmp1 = l->head; tmp1 != NULL; tmp1 = tmp1->next) {
         if (tmp1->next == NULL) {
            tmp1->next = (VIManager_State *) calloc(1, sizeof(VIManager_State));
            VIManager_State_initialize(tmp1->next, n, NULL);
            result = tmp1->next;
            break;
         } else if (tmp1->next->number == n) {
            result = tmp1->next;
            break;
         } else if (n < tmp1->next->number) {
            tmp2 = tmp1->next;
            tmp1->next = (VIManager_State *) calloc(1, sizeof(VIManager_State));
            VIManager_State_initialize(tmp1->next, n, tmp2);
            result = tmp1->next;
            break;
         }
      }
   }
   return result;
}

/* VIManager_SList_addArc: add arc */
static void VIManager_SList_addArc(VIManager_SList *l, int index_s1, int index_s2, char *isymbol, char *osymbol)
{
   int i, idx;
   VIManager_State *s1, *s2;
   VIManager_Arc *a1, *a2;
   VIManager_AList *arc_list;

   char type[MMDAGENT_MAXBUFLEN];
   char **args = NULL;
   int argc = 0;
   char otype[MMDAGENT_MAXBUFLEN];
   char oargs[MMDAGENT_MAXBUFLEN];

   s1 = VIManager_SList_searchState(l, index_s1);
   s2 = VIManager_SList_searchState(l, index_s2);
   arc_list = &s1->arc_list;

   /* analysis input symbol */
   idx = 0;
   i = getArgFromString(isymbol, &idx, type);
   if (i <= 0)
      return;
   getArgs(&isymbol[idx], &args, &argc);

   /* analysis output symbol */
   idx = 0;
   i = getArgFromString(osymbol, &idx, otype);
   if (i <= 0)
      return;
   getTokenFromString(osymbol, &idx, oargs);

   /* create */
   a1 = (VIManager_Arc *) calloc(1, sizeof(VIManager_Arc));
   VIManager_Arc_initialize(a1, type, args, argc, otype, oargs, s2);

   /* set */
   if (arc_list->head == NULL) {
      arc_list->head = a1;
   } else {
      for (a2 = arc_list->head; a2->next != NULL; a2 = a2->next);
      a2->next = a1;
   }

   /* free buffer */
   if (argc > 0) {
      for (i = 0; i < argc; i++)
         free(args[i]);
      free(args);
   }
}

/* VIManager::initialize: initialize VIManager */
void VIManager::initialize()
{
   VIManager_SList_initialize(&m_stateList);
   m_currentState = NULL;
}

/* VIManager:clear: free VIManager */
void VIManager::clear()
{
   VIManager_SList_clear(&m_stateList);
   initialize();
}

/* VIManager::VIManager: constructor */
VIManager::VIManager()
{
   initialize();
}

/* VIManager::~VIManager: destructor */
VIManager::~VIManager()
{
   clear();
}

/* VIManager::load: load FST */
int VIManager::load(const char *file)
{
   FILE *fp;
   char buff[MMDAGENT_MAXBUFLEN];
   int len;
   int idx;

   char buff_s1[MMDAGENT_MAXBUFLEN];
   char buff_s2[MMDAGENT_MAXBUFLEN];
   char buff_is[MMDAGENT_MAXBUFLEN];
   char buff_os[MMDAGENT_MAXBUFLEN];
   char buff_er[MMDAGENT_MAXBUFLEN];
   int size_s1;
   int size_s2;
   int size_is;
   int size_os;
   int size_er;
   char *err_s1;
   char *err_s2;
   unsigned int index_s1;
   unsigned int index_s2;

   /* open */
   fp = MMDAgent_fopen(file, "r");
   if (fp == NULL)
      return 0;

   /* unload */
   VIManager_SList_clear(&m_stateList);
   VIManager_SList_initialize(&m_stateList);

   while (fgets(buff, MMDAGENT_MAXBUFLEN - 3, fp) != NULL) { /* string + \r + \n + \0 */
      /* remove final \n and \r */
      len = MMDAgent_strlen(buff);
      while (len > 0 && (buff[len-1] == '\n' || buff[len-1] == '\r'))
         buff[--len] = '\0';

      /* check and load arc */
      if (len > 0) {
         idx = 0;
         size_s1 = getTokenFromString(buff, &idx, buff_s1);
         size_s2 = getTokenFromString(buff, &idx, buff_s2);
         size_is = getTokenFromString(buff, &idx, buff_is);
         size_os = getTokenFromString(buff, &idx, buff_os);
         size_er = getTokenFromString(buff, &idx, buff_er);
         if (size_s1 > 0 && size_s2 > 0 && size_is > 0 && size_os > 0 && size_er == 0 && buff_s1[0] != VIMANAGER_COMMENT) {
            index_s1 = (unsigned int) strtoul(buff_s1, &err_s1, 10);
            index_s2 = (unsigned int) strtoul(buff_s2, &err_s2, 10);
            if (buff_s1 + size_s1 == err_s1 && buff_s2 + size_s2 == err_s2)
               VIManager_SList_addArc(&m_stateList, index_s1, index_s2, buff_is, buff_os);
         }
      }
      if (feof(fp) || ferror(fp))
         break;
   }

   fclose(fp);

   /* set current state to zero */
   m_currentState = VIManager_SList_searchState(&m_stateList, VIMANAGER_STARTSTATE);

   return 1;
}

/* VIManager::transition: state transition (if jumped, return arc) */
VIManager_Arc *VIManager::transition(const char *itype, const char *iargs, char *otype, char *oargs)
{
   int i, j;
   int jumped = 0;

   VIManager_Arc *arc;
   VIManager_AList *arc_list;

   char **args;
   int argc;

   strcpy(otype, VIMANAGER_EPSILON);
   strcpy(oargs, "");

   /* FST isn't loaded yet */
   if (m_currentState == NULL)
      return NULL;

   /* state don't have arc list */
   arc_list = &m_currentState->arc_list;
   if (arc_list->head == NULL)
      return NULL;

   /* get input event args */
   getArgs(iargs, &args, &argc);

   /* matching */
   for (arc = arc_list->head; arc != NULL; arc = arc->next) {
      if (MMDAgent_strequal(itype, arc->input_event_type)) {
         if (MMDAgent_strequal(itype, VIMANAGER_RECOG_EVENT_STOP)) {
            /* for recognition event */
            for (i = 0; i < arc->input_event_argc; i++) {
               jumped = 0;
               for (j = 0; j < argc; j++) {
                  if (MMDAgent_strequal(arc->input_event_args[i], args[j])) {
                     jumped = 1;
                     break;
                  }
               }
               if (jumped == 0)
                  break;
            }
         } else if (argc == arc->input_event_argc) {
            /* for others */
            jumped = 1;
            for (i = 0; i < argc; i++) {
               if (MMDAgent_strequal(args[i], arc->input_event_args[i]) == false) {
                  jumped = 0;
                  break;
               }
            }
         }
         if (jumped) { /* state transition */
            strcpy(otype, arc->output_command_type);
            strcpy(oargs, arc->output_command_args);
            m_currentState = arc->next_state;
            break;
         }
      }
   }

   if (argc > 0) {
      for (i = 0; i < argc; i++)
         free(args[i]);
      free(args);
   }

   return arc;
}

/* VIManager::getCurrentState: get current state */
VIManager_State *VIManager::getCurrentState()
{
   return m_currentState;
}

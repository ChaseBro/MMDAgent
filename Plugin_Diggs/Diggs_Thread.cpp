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
#include "Diggs_Thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "50007" // the port client will be connecting to 
#define MAXDATASIZE 100 // max number of bytes we can get at once 


/* File Globals */
bool m_pause;
int sockfd;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/* Diggs_Thread::initialize: initialize thread */
void Diggs_Thread::initialize()
{
   m_mmdagent = NULL;

   m_thread = -1;

   m_configFile = NULL;
}

/* Diggs_Thread::clear: free thread */
void Diggs_Thread::clear()
{
   if(m_thread >= 0) {
      glfwWaitThread(m_thread, GLFW_WAIT);
      glfwDestroyThread(m_thread);
      glfwTerminate();
   }

   if(m_configFile != NULL)
      free(m_configFile);

   if (sockfd)
      close(sockfd);

   initialize();
}

/* Diggs_Thread::Diggs_Thread: thread constructor */
Diggs_Thread::Diggs_Thread()
{
   initialize();
}

/* Diggs_Thread::~Diggs_Thread: thread destructor */
Diggs_Thread::~Diggs_Thread()
{
   clear();
}

/* Diggs_Thread::m_connect : Connect to remote server */
void Diggs_Thread::m_connect()
{
   struct addrinfo hints, *servinfo, *p;
   int rv;
   char s[INET6_ADDRSTRLEN];

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

   if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
     printf("getaddrinfo: %s\n", gai_strerror(rv));
     return;
   }

   // loop through all the results and connect to the first we can
   for(p = servinfo; p != NULL; p = p->ai_next) {
     if ((sockfd = socket(p->ai_family, p->ai_socktype,
             p->ai_protocol)) == -1) {
         perror("client: socket");
         continue;
     }

     if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
         close(sockfd);
         perror("client: connect");
         continue;
     }

     break;
   }

   if (p == NULL) {
     printf("client: failed to connect\n");
     return;
   }

   inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
         s, sizeof s);
   printf("client: connecting to %s\n", s);

   printf("Connected to Diggs.\n");

   freeaddrinfo(servinfo); // all done with this structure

   return;
}

/* Diggs_Thread::load: load models and start thread */
void Diggs_Thread::load(MMDAgent *mmdagent, const char *configFile)
{
   char name[MMDAGENT_MAXBUFLEN];

   m_mmdagent = mmdagent;
   m_configFile = MMDAgent_strdup(configFile);

   if(m_configFile == NULL) {
      clear();
      printf("Error Loading Config File.\n");
      return;
   }

   m_connect();

   m_pause = false;
}

/* Diggs_Thread::pause: pause recognition process */
void Diggs_Thread::pause()
{
   m_pause = true;
}

/* Diggs_Thread::resume: resume recognition process */
void Diggs_Thread::resume()
{
   m_pause = false;
}

/* Diggs_Thread::sendMessage: send message to MMDAgent */
void Diggs_Thread::sendMessage(const char *str1, const char *str2)
{
   m_mmdagent->sendEventMessage(str1, str2);
}

char* Diggs_Thread::procParse(const char *plainText, const char *parse)
{
   int numbytes;
   char buf[MAXDATASIZE];

   printf("sending: %s\n",plainText);
   if ((numbytes = send(sockfd, plainText, strlen(plainText)+1, 0)) == -1) {
     perror("send");
     return NULL;
   }

   if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
     perror("recv");
     return NULL;
   }

   buf[numbytes] = '\0';

   printf("client: received '%s'\n",buf);
   return buf;
}

char* Diggs_Thread::procPlainText(char *text)
{
   int numbytes;
   char buf[MAXDATASIZE];

   if ((numbytes = send(sockfd, text, sizeof(text), 0)) == -1) {
     perror("send");
     return NULL;
   }

   if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
     perror("recv");
     return NULL;
   }

   buf[numbytes] = '\0';

   printf("client: received '%s'\n",buf);
   return buf;
}


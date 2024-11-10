/* Copyright 2024 Alexander Neundorf, <neundorf|kde.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CommandRunner.h"

#include "Command.h"

#include <QCoreApplication>
#include <QObject>
#include <QThread>

#include <fmt/core.h>


QThread* CommandRunner::s_workerThread = nullptr;
CommandThreadWorker* CommandRunner::s_worker = nullptr;


CommandThreadWorker::CommandThreadWorker(QObject* _parent)
:QObject(_parent)
{
}


void CommandThreadWorker::runCommand(Command* _cmd)
{
   _cmd->exec();
   emit commandFinished();
}


CommandRunner::CommandRunner(QObject* _parent)
:QObject(_parent)
{
   m_localEventLoop = new QEventLoop(this);

   if (!s_workerThread)
   {
      s_workerThread = new QThread();
      s_workerThread->start();
      s_worker = new CommandThreadWorker();
      s_worker->moveToThread(s_workerThread);
   }
   connect(this, &CommandRunner::runCommandRequested, s_worker, &CommandThreadWorker::runCommand);
}


void CommandRunner::run(Command* _cmd, Mode _mode)
{
   if (!_cmd)
   {
      return;
   }

   switch(_mode)
   {
      case Mode::InThisThread:
         fmt::print("run::InThisThread\n");
         _cmd->exec();
         break;
      case Mode::SyncThread:
         fmt::print("run::SyncThread\n");
         if (QThread::currentThread() != QCoreApplication::instance()->thread())
         {
            fmt::print("run: already in worker thread\n");
            _cmd->exec();
         }
         else
         {
            fmt::print("run: running in other thread\n");
            connect(s_worker, &CommandThreadWorker::commandFinished, m_localEventLoop, &QEventLoop::quit);
            emit runCommandRequested(_cmd);
            m_localEventLoop->exec();
            disconnect(s_worker, &CommandThreadWorker::commandFinished, m_localEventLoop, &QEventLoop::quit);
            fmt::print("run: running in other thread done\n");
         }
         break;
   }
}

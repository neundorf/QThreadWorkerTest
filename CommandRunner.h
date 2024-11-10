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

#pragma once

#include <QObject>
#include <QEvent>

class QEventLoop;

class Command;
class CommandRunner;


/** Used internally by CommandRunner.
 * Objects of this class live in another thread, i.e. not the main thread.
 * They actually execute the commands. */
class CommandThreadWorker : public QObject
{
   Q_OBJECT
   public:
      CommandThreadWorker(QObject* _parent = nullptr);

   public slots:
      void runCommand(Command* _cmd);

   signals:
      void commandFinished();

};


/** This class executes commands blocking, either in the current thread,
 * or in a thread which is not the main thread. If the command is executed
 * in another thread, the Qt event loops runs while the command is executing. */
class CommandRunner : public QObject
{
   Q_OBJECT
   public:
      CommandRunner(QObject* _parent = nullptr);

      enum class Mode { InThisThread, SyncThread };

      void run(Command* _cmd, Mode _mode);

   signals:
      void runCommandRequested(Command* _cmd);

   protected:
      QEventLoop* m_localEventLoop = nullptr;

      static QThread* s_workerThread;
      static CommandThreadWorker* s_worker;
};

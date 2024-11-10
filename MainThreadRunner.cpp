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

#include "MainThreadRunner.h"

#include <QCoreApplication>
#include <QObject>
#include <QThread>

#include <fmt/core.h>


MainThreadRunnerWorker* MainThreadRunner::s_mainThreadRunner = nullptr;
QEvent::Type MainThreadRunner::s_funcInMainFinishedEvent = QEvent::MaxUser;


MainThreadRunnerWorker::MainThreadRunnerWorker(QObject* _parent)
:QObject(_parent)
{
}


void MainThreadRunnerWorker::runFunctionInMainThread(std::function<void(void)> func, MainThreadRunner* _caller)
{
   func();
   QEvent* finishedEvent = new QEvent(_caller->getFunctionInMainFinishedEventType());
   QCoreApplication::instance()->postEvent(_caller, finishedEvent);
}


MainThreadRunner::MainThreadRunner(QObject* _parent)
:QObject(_parent)
{
   m_localEventLoop = new QEventLoop(this);

   if (!s_mainThreadRunner)
   {
      s_mainThreadRunner = new MainThreadRunnerWorker();
      s_mainThreadRunner->moveToThread(QCoreApplication::instance()->thread());

      s_funcInMainFinishedEvent = static_cast<QEvent::Type>(QEvent::registerEventType());
      fmt::print("func event is {0}\n", (int)s_funcInMainFinishedEvent);
   }
   connect(this, &MainThreadRunner::runFunctionRequested, s_mainThreadRunner, &MainThreadRunnerWorker::runFunctionInMainThread);
}


void MainThreadRunner::runFunction(std::function<void(void)> func)
{
   emit runFunctionRequested(func, this);
   m_localEventLoop->exec();
}


bool MainThreadRunner::event(QEvent* _event)
{
   if (_event->type() == s_funcInMainFinishedEvent)
   {
      fmt::print("received finished event\n");
      m_localEventLoop->exit();
      return true;
   }

   return QObject::event(_event);
}


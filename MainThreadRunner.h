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


class MainThreadRunner;


/** Helper class used internally by MainThreadRunner.
 * It "lives" in the main thread and executes lambdas. */
class MainThreadRunnerWorker : public QObject
{
   Q_OBJECT
   public:
      MainThreadRunnerWorker(QObject* _parent = nullptr);

   public slots:
      void runFunctionInMainThread(std::function<void(void)> func, MainThreadRunner* _caller);
};


/** MainThreadRunner is a class which can be used to execute lambdas
 * in the main thread in a blocking way. */
class MainThreadRunner : public QObject
{
   Q_OBJECT
   public:
      MainThreadRunner(QObject* _parent = nullptr);

      void runFunction(std::function<void(void)> func);

      bool event(QEvent* _event) override;

      QEvent::Type getFunctionInMainFinishedEventType() const { return s_funcInMainFinishedEvent; }

   signals:
     void runFunctionRequested(std::function<void(void)> func, MainThreadRunner* _caller);

   protected:
      QEventLoop* m_localEventLoop = nullptr;

      static MainThreadRunnerWorker* s_mainThreadRunner;
      static QEvent::Type s_funcInMainFinishedEvent;
};


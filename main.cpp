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

#include "DoStuff.h"

#include <fmt/core.h>

#include <QCoreApplication>
#include <QThread>
#include <QTimer>

int main(int argc, char** argv)
{
   fmt::print("hallo\n");
   QCoreApplication app(argc, argv);
   DoStuff doStuff;

   QThread* thread = app.thread();

   fmt::print("main thread: {0} current thread: {1} isMain: {2}\n",
              (void*)thread, (void*)QThread::currentThread(), true/*thread->isMainThread()*/);

   QTimer::singleShot(1000, &doStuff, &DoStuff::startWork);
   int res = app.exec();
   return 0;
}

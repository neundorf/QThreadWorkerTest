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

#include "Command.h"
#include "CommandRunner.h"
#include "CommandInCommand.h"


#include <QCoreApplication>

DoStuff::DoStuff()
:QObject()
{
}


void DoStuff::startWork()
{
   fmt::print("startWork()\n");
   Command com1("com1");

   CommandRunner runner;
   runner.run(&com1, CommandRunner::Mode::SyncThread);

   Command com2("com2");
   runner.run(&com2, CommandRunner::Mode::SyncThread);

   CommandInCommand cic("NestedCommand");
   runner.run(&cic, CommandRunner::Mode::SyncThread);

   QCoreApplication::instance()->quit();
}


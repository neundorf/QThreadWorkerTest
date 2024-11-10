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

#include "Command.h"

#include <QThread>

#include <fmt/core.h>


Command::Command(const char* _s, QObject* _parent)
:QObject(_parent)
,m_s(_s)
{
}


void Command::check()
{
  fmt::print("Check[ {0} ] thread: {1}\n", m_s, (void*)QThread::currentThread());
}


void Command::exec()
{
  fmt::print("Exec[ {0} ] thread: {1}\n", m_s, (void*)QThread::currentThread());
}


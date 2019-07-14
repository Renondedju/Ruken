/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <iostream>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Threading/Worker.hpp"
#include "Threading/ThreadSafeLockQueue.hpp"

USING_DAEMON_NAMESPACE

int main()
{
	// Creating workers
	Worker worker1("Worker1");
	Worker worker2("Worker2");
	Worker worker3("Worker3");
	Worker worker4("Worker4");

	// Creating the queue
	ThreadSafeLockQueue<String> queue;

	auto const job = [&queue] {
		String text;
		DAEbool const result = queue.Dequeue(text);

		if (result)
			std::cout << text << std::endl;
		else
			std::cout << "Released" << std::endl;
	};

	queue.Enqueue("Test1");
	queue.Enqueue("Test2");
	queue.Enqueue("Test3");

	//Starting workers
	worker1.Execute(job);
	worker2.Execute(job);
	worker3.Execute(job);
	worker4.Execute(job);

	queue.WaitUntilEmpty();
	queue.Release();

	system("pause");

	return EXIT_SUCCESS;
}

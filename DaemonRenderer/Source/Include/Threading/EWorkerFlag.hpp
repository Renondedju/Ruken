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

#pragma once

#include "Config.hpp"
#include "Bitwise/Bitmask.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Enum allowing to specialize worker on certain tasks only. Bitmask safe enum.
 * 
 * Any		 => Any non specialized job can be executed
 * Logging	 => Logging/debugging jobs are allowed to be executed by the worker holding it
 * Resource	 => Resource manipulation jobs are allowed to be executed by the worker holding it
 * Rendering => Rendering jobs are allowed to be executed by the worker holding it
 */
DAEMON_BITMASK(EWorkerFlag,
	Any,
	Logging,
	Resource,
	Rendering
);

namespace internal
{
	/**
	 * \brief This variable is used by the scheduler, please update it if you modify the EWorkerFlag enum
	 * 
	 * If you have any better and cleaner alternatives to this, please let me know !
	 */
	constexpr DAEsize g_worker_flag_max = static_cast<DAEsize>(EWorkerFlag::Rendering);
}

END_DAEMON_NAMESPACE
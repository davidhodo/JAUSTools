/**
\file PrimitiveDriver.h

\par Copyright
Copyright (c) 2011, OpenJAUS, LLC
All rights reserved.

This file is part of OpenJAUS. OpenJAUS is distributed under the OpenJAUS
SDK Commercial End User License Agreement. See the LICENSE.txt file for more 
details.
 
THIS SOFTWARE IS PROVIDED BY THE LICENSOR (OPENJAUS LCC) "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE LICENSOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE. THE LICENSOR DOES NOT WARRANT THAT THE LICENSED SOFTWARE WILL MEET
LICENSEE'S REQUIREMENTS OR THAT THE OPERATION OF THE LICENSED SOFTWARE
WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ERRORS IN THE LICENSED
SOFTWARE WILL BE CORRECTED.

\ Software History
- [2011-08-23] - Added AS6057: Manipulators
- [2011-08-01] - Added AS6060: Environment Sensing
- [2011-06-16] - First Commercial Release 

*/

#ifndef PRIMITIVEDRIVER_COMPONENT_H
#define PRIMITIVEDRIVER_COMPONENT_H

#include <openjaus.h>
#include "openjaus/core/Managed.h"
#include "openjaus/mobility/PrimitiveDriverInterface.h"
#include "openjaus/mobility/Transitions/PrimitiveDriverDefaultLoop.h"
#include "openjaus/mobility/Transitions/PrimitiveDriverReadyLoop.h"
#include "openjaus/mobility/Triggers/SetWrenchEffort.h"
#include "openjaus/mobility/Triggers/QueryWrenchEffort.h"
#include "openjaus/mobility/Triggers/ReportWrenchEffort.h"
// Start of user code for additional headers:
// End of user code

namespace openjaus
{
namespace mobility
{

/// \class PrimitiveDriver PrimitiveDriver.h
/// \brief %PrimitiveDriver Component implements the urn:jaus:jss:mobility:PrimitiveDriver services.
/// The %PrimitiveDriver component provides an implementation of the following service(s). This component can 
/// be extended for customized functionality or used via instantiation.
/// <dl>
/// <dt>%PrimitiveDriver Service</dt>
/// <dd><p>
/// 
/// </p><br/><br/>
/// <b>URI:</b> urn:jaus:jss:mobility:PrimitiveDriver<br/><br/>
/// <b>Version:</b> 1.0<br/>
/// <dl><dt><b>Inherits From:</b></dt>
/// <dd>urn:jaus:jss:core:Management</dd>
/// </dl></dd>
/// </dl>
class OPENJAUS_EXPORT PrimitiveDriver : public core::Managed, public mobility::PrimitiveDriverInterface
{

public:
	PrimitiveDriver();
	virtual ~PrimitiveDriver();

	/// \brief Send action for ReportWrenchEffort with input message QueryWrenchEffort.
	/// Send action for ReportWrenchEffort with input message QueryWrenchEffort.
	/// \param[in] queryWrenchEffort - Input Trigger.
	/// \return ReportWrenchEffort Output Message.
	virtual ReportWrenchEffort getReportWrenchEffort(QueryWrenchEffort *queryWrenchEffort);

	/// \brief SetWrenchEffort action with input SetWrenchEffort.
	/// SetWrenchEffort action with input SetWrenchEffort.
	/// \param[in]  setWrenchEffort - Input Trigger.
	/// \return Whether the message was properly processed by this action. 
    virtual bool setWrenchEffort(SetWrenchEffort *setWrenchEffort);


	/// \brief isControllingPdClient condition.
	/// isControllingPdClient condition.
	/// \param[in]  setWrenchEffort - Input Trigger.
	/// \return Whether the condition is true. 
	virtual bool isControllingPdClient(SetWrenchEffort *setWrenchEffort);


	// Start of user code for additional methods:
	// End of user code

protected:
	PrimitiveDriverDefaultLoop primitiveDriverDefaultLoop;
	PrimitiveDriverReadyLoop primitiveDriverReadyLoop;


	// Start of user code for additional members:
	// End of user code
};

} // namespace component
} // namespace openjaus

#endif // PRIMITIVEDRIVER_H

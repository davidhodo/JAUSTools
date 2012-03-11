/**
\file PrimitiveDriver.cpp

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


#include "openjaus/mobility/PrimitiveDriver.h"
// Start of user code for additional headers:
// End of user code

namespace openjaus
{
namespace mobility
{

PrimitiveDriver::PrimitiveDriver() : Managed(),
	primitiveDriverDefaultLoop(),
	primitiveDriverReadyLoop()
{
	// Add Service Identification Data to implements list
	name = "PrimitiveDriver";
	
	model::Service *primitiveDriverService = new model::Service();
	primitiveDriverService->setName("PrimitiveDriver");
	primitiveDriverService->setUri("urn:jaus:jss:mobility:PrimitiveDriver");
	primitiveDriverService->setVersionMajor(1);
	primitiveDriverService->setVersionMinor(0);
	this->implements->push_back(primitiveDriverService);
	
	

	primitiveDriverDefaultLoop.setInterface(this);
	primitiveDriverDefaultLoop.setTransportInterface(this);
	accessStateMachine.addDefaultStateTransition(primitiveDriverDefaultLoop);
	
	primitiveDriverReadyLoop.setInterface(this);
	primitiveDriverReadyLoop.setTransportInterface(this);
	ready.addTransition(primitiveDriverReadyLoop);
	
    
    
	// Start of user code for Constructor:
	// End of user code
}

PrimitiveDriver::~PrimitiveDriver()
{
	// Start of user code for Destructor:
	// End of user code
}

mobility::ReportWrenchEffort PrimitiveDriver::getReportWrenchEffort(QueryWrenchEffort *queryWrenchEffort)
{
	// Start of user code for action getReportWrenchEffort(QueryWrenchEffort *queryWrenchEffort):
	mobility::ReportWrenchEffort message;
	return message;
	// End of user code
}

bool PrimitiveDriver::setWrenchEffort(SetWrenchEffort *setWrenchEffort)
{
	// Start of user code for action setWrenchEffort(SetWrenchEffort *setWrenchEffort):
	return false;
	// End of user code
}


bool PrimitiveDriver::isControllingPdClient(SetWrenchEffort *setWrenchEffort)
{
	// Start of user code for action isControllingPdClient(SetWrenchEffort *setWrenchEffort):
	return false;
	// End of user code
}


// Start of user code for additional methods
// End of user code

} // namespace component
} // namespace openjaus


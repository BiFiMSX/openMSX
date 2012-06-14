// $Id$

#include "BreakPointBase.hh"
#include "TclObject.hh"
#include "CommandException.hh"
#include "GlobalCliComm.hh"
#include "ScopedAssign.hh"

namespace openmsx {

BreakPointBase::BreakPointBase(GlobalCliComm& cliComm_,
                               std::auto_ptr<TclObject> command_,
                               std::auto_ptr<TclObject> condition_)
	: cliComm(cliComm_), command(command_), condition(condition_)
	, executing(false)
{
}

BreakPointBase::~BreakPointBase()
{
}

string_ref BreakPointBase::getCondition() const
{
	return condition.get() ? condition->getString() : "";
}

string_ref BreakPointBase::getCommand() const
{
	return command->getString();
}

std::auto_ptr<TclObject> BreakPointBase::getConditionObj() const
{
	return std::auto_ptr<TclObject>(condition.get()
	                                ? new TclObject(*condition)
	                                : NULL);
}

std::auto_ptr<TclObject> BreakPointBase::getCommandObj() const
{
	return std::auto_ptr<TclObject>(new TclObject(*command));
}

bool BreakPointBase::isTrue() const
{
	if (!condition.get()) {
		// unconditional bp
		return true;
	}
	try {
		return condition->evalBool();
	} catch (CommandException& e) {
		cliComm.printWarning(e.getMessage());
		return false;
	}
}

void BreakPointBase::checkAndExecute()
{
	if (executing) {
		// no recursive execution
		return;
	}
	ScopedAssign<bool> sa(executing, true);
	if (isTrue()) {
		try {
			command->executeCommand(true); // compile command
		} catch (CommandException& e) {
			cliComm.printWarning(e.getMessage());
		}
	}
}

Tcl_Interp* BreakPointBase::getInterpreter() const
{
	return command->getInterpreter();
}

} // namespace openmsx


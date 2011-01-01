/***************************************************************************
                        IContainerControl.h  -  description
							-------------------
	begin                : January 1st, 2011
	copyright            : (C) 2011 by Duong Khang NGUYEN
	email                : neoneurone @ gmail com

	$Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 *                                                                         *
 ***************************************************************************/

#ifndef _SPF_SYSTEM_WINDOWS_FORMS_ICONTAINERCONTROL_H_
#define _SPF_SYSTEM_WINDOWS_FORMS_ICONTAINERCONTROL_H_ 1

SPF_NAMESPACE_BEGIN(System)
SPF_NAMESPACE_NESTED_BEGIN(Windows, Forms)

// Forward System::Windows::Forms::Control class declaration
//class Control;

/**
	Provides the functionality for a control to act as a parent for other
controls.
*/
class IContainerControl
{
	public:
/**
	Gets the control that is active on the container control.
*/
		const Control& GetActiveControl() { return *mpActiveControl; }

/**
	Sets the control that is active on the container control.
*/
		void SetActiveControl(const Control& activeControl) { mpActiveControl = &activeControl; }

	protected:
		IContainerControl() {}
		virtual ~IContainerControl() {}

	private:
		// FIXME: uninitialized variable because NULL is not defined yet.
		const Control* mpActiveControl;

}; // class System::Windows::Forms::IContainerControl

// namespace Windows::Forms
SPF_NAMESPACE_NESTED_END

// namespace System
SPF_NAMESPACE_END

#endif

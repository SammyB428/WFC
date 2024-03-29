/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 1995-2022, Samuel R. Blackburn
**
** "You can get credit for something or get it done, but not both."
** Dr. Richard Garwin
**
** BSD License follows.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** Redistributions of source code must retain the above copyright notice,
** this list of conditions and the following disclaimer. Redistributions
** in binary form must reproduce the above copyright notice, this list
** of conditions and the following disclaimer in the documentation and/or
** other materials provided with the distribution. Neither the name of
** the WFC nor the names of its contributors may be used to endorse or
** promote products derived from this software without specific prior
** written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** $Workfile: CMixer.cpp $
** $Revision: 24 $
** $Modtime: 6/26/01 10:47a $
** $Reuse Tracing Code: 1 $
*/

/* SPDX-License-Identifier: BSD-2-Clause */

#include <wfc.h>
#pragma hdrstop

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#define new DEBUG_NEW
#undef THIS_FILE
static auto const THIS_FILE{ __FILE__ };
#endif // _DEBUG

#if defined( _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS )

static void dump_mixer_control_details(__in_z LPCTSTR file, _In_ UINT line, __in_z LPCTSTR get_or_set, _In_ HMIXEROBJ mixer_object, _In_ MIXERCONTROLDETAILS * details_p, _In_ DWORD option) noexcept
{
    TCHAR debug_string[513];

    if (details_p == nullptr)
    {
        OutputDebugString(TEXT("details_p is NULL!\n"));
    }

    _stprintf(debug_string, TEXT("in %s at line %d, %s( %lu, %p, %lX )\n"), file, line, get_or_set, (DWORD)mixer_object, details_p, option);
    OutputDebugString(debug_string);
    _stprintf(debug_string, TEXT("at address %p\n"), (VOID *)details_p);
    OutputDebugString(debug_string);

    if (details_p == nullptr)
    {
        return;
    }

    // We were passed a pointer, don't trust it

    WFC_TRY
    {
       _stprintf(debug_string, TEXT("MIXERCONTROLDETAILS.cbStruct       = %lu\n"), details_p->cbStruct);
       OutputDebugString(debug_string);
       _stprintf(debug_string, TEXT("MIXERCONTROLDETAILS.dwControlID    = %lu\n"), details_p->dwControlID);
       OutputDebugString(debug_string);
       _stprintf(debug_string, TEXT("MIXERCONTROLDETAILS.cChannels      = %lu\n"), details_p->cChannels);
       OutputDebugString(debug_string);
       _stprintf(debug_string, TEXT("MIXERCONTROLDETAILS.cMultipleItems = %lu\n"), details_p->cMultipleItems);
       OutputDebugString(debug_string);
       _stprintf(debug_string, TEXT("MIXERCONTROLDETAILS.cbDetails      = %lu\n"), details_p->cbDetails);
       OutputDebugString(debug_string);
    }
        WFC_CATCH_ALL
    {
    }
    WFC_END_CATCH_ALL
}

#endif // _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS

// Construction

Win32FoundationClasses::CMixer::CMixer()
{
    WFC_VALIDATE_POINTER(this);
}

Win32FoundationClasses::CMixer::~CMixer()
{
    WFC_VALIDATE_POINTER(this);
    Close();
    m_Handle = static_cast<HMIXER>(NULL);
}

// Methods

void Win32FoundationClasses::CMixer::Close(void) noexcept
{
    WFC_VALIDATE_POINTER(this);

    m_ErrorCode = ::mixerClose(m_Handle);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        //WFCTRACE( TEXT( "Close Failed" ) );
    }

    m_Handle = static_cast<HMIXER>(NULL);
}

#if defined( _DEBUG ) && ! defined( WFC_NO_DUMPING )

void CMixer::Dump(CDumpContext& dump_context) const
{
    dump_context << TEXT(" a CMixer at ") << (VOID *) this << TEXT("\n");
}

#endif // _DEBUG

_Check_return_ bool Win32FoundationClasses::CMixer::Get(_Out_ Win32FoundationClasses::CMixerCapabilities& capabilities) noexcept
{
    WFC_VALIDATE_POINTER(this);

    MIXERCAPS mixer_capabilities;

    ::ZeroMemory(&mixer_capabilities, sizeof(mixer_capabilities));

    m_ErrorCode = ::mixerGetDevCaps(m_DeviceID, &mixer_capabilities, sizeof(mixer_capabilities));

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        capabilities.Empty();
        return(false);
    }

    capabilities.Copy(mixer_capabilities);

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetAllControls(_In_ Win32FoundationClasses::CMixerLine const& line, _Out_ std::vector<CMixerControl>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    array.clear();

    MIXERLINECONTROLS line_controls;

    ::ZeroMemory(&line_controls, sizeof(line_controls));

    line_controls.cbStruct = sizeof(line_controls);

    DWORD size_of_buffer{ line.NumberOfControls * sizeof(MIXERCONTROL) };

    std::vector<MIXERCONTROL> control_array;
    
    control_array.resize(line.NumberOfControls);

    ::ZeroMemory(control_array.data(), size_of_buffer);

    // Initialize the structures

    for ( auto& entry : control_array )
    {
        entry.cbStruct = sizeof(MIXERCONTROL);
    }

    line_controls.dwLineID = line.ID;
    line_controls.cControls = line.NumberOfControls;
    line_controls.cbmxctrl = sizeof(MIXERCONTROL);
    line_controls.pamxctrl = control_array.data();

    auto flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETLINECONTROLSF_ALL };

    bool return_value{ true };

    m_ErrorCode = ::mixerGetLineControls((HMIXEROBJ)m_DeviceID, &line_controls, flags);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        return_value = false;
    }
    else
    {
        for ( auto const array_index : Range(control_array.size()) )
        {
            array.at(array_index).Copy(control_array.at(array_index));
        }

        return_value = true;
    }

    return(return_value);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetByComponent(_In_ Win32FoundationClasses::CMixerLine::ComponentType const component, _Out_ Win32FoundationClasses::CMixerLine& line) noexcept
{
    WFC_VALIDATE_POINTER(this);

    MIXERLINE mixer_line;

    ::ZeroMemory(&mixer_line, sizeof(mixer_line));

    mixer_line.cbStruct = sizeof(mixer_line);
    mixer_line.dwComponentType = static_cast<uint32_t>(component);

    auto const flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETLINEINFOF_COMPONENTTYPE };

    m_ErrorCode = ::mixerGetLineInfo((HMIXEROBJ)m_DeviceID, &mixer_line, flags);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        line.Empty();
        return(false);
    }

    line.Copy(mixer_line);

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetByDestination(_In_ DWORD const destination, _Out_ Win32FoundationClasses::CMixerLine& line) noexcept
{
    WFC_VALIDATE_POINTER(this);

    MIXERLINE mixer_line;

    ::ZeroMemory(&mixer_line, sizeof(mixer_line));

    mixer_line.cbStruct = sizeof(mixer_line);
    mixer_line.dwDestination = destination;

    auto flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETLINEINFOF_DESTINATION };

    m_ErrorCode = ::mixerGetLineInfo((HMIXEROBJ)m_DeviceID, &mixer_line, flags);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        line.Empty();
        return(false);
    }

    line.Copy(mixer_line);

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetByID(_In_ DWORD const id, _Out_ Win32FoundationClasses::CMixerLine& line) noexcept
{
    WFC_VALIDATE_POINTER(this);

    MIXERLINE mixer_line;

    ::ZeroMemory(&mixer_line, sizeof(mixer_line));

    mixer_line.cbStruct = sizeof(mixer_line);
    mixer_line.dwLineID = id;

    auto flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETLINEINFOF_LINEID };

    m_ErrorCode = ::mixerGetLineInfo((HMIXEROBJ)m_DeviceID, &mixer_line, flags);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        line.Empty();
        return(false);
    }

    line.Copy(mixer_line);

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetByConnection( _In_ DWORD const destination, _In_ DWORD const source, _Out_ Win32FoundationClasses::CMixerLine& line) noexcept
{
    WFC_VALIDATE_POINTER(this);

    MIXERLINE mixer_line;

    ::ZeroMemory(&mixer_line, sizeof(mixer_line));

    mixer_line.cbStruct = sizeof(mixer_line);
    mixer_line.dwDestination = destination;
    mixer_line.dwSource = source;

    auto flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETLINEINFOF_SOURCE };

    m_ErrorCode = ::mixerGetLineInfo((HMIXEROBJ)m_DeviceID, &mixer_line, flags);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        line.Empty();
        return(false);
    }

    line.Copy(mixer_line);

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetControlDetails(_In_ Win32FoundationClasses::CMixerLine const& line, _In_ Win32FoundationClasses::CMixerControl const& control, _Out_ std::vector<CMixerControlDetailsData>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    array.clear();

    MIXERCONTROLDETAILS control_details;

    ::ZeroMemory(&control_details, sizeof(control_details));

    auto type_of_details{ control.GetUnits() };

    DWORD size_of_element{ 0 };

    if (type_of_details == CMixerControl::Units::Boolean)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
    }
    else if (type_of_details == CMixerControl::Units::Signed or
        type_of_details == CMixerControl::Units::Decibels)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_SIGNED);
    }
    else if (type_of_details == CMixerControl::Units::Unsigned or
        type_of_details == CMixerControl::Units::Percent)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    }
    else
    {
        ASSERT(FALSE);
        return(false);
    }

    DWORD number_of_items_per_channel{ 0 };
    DWORD number_of_channels{ line.NumberOfChannels };

    if (control.IsUniform())
    {
        number_of_items_per_channel = 1;
        number_of_channels = 1;

        if (type_of_details == CMixerControl::Units::Boolean)
        {
            number_of_items_per_channel = 0;
        }
    }
    else
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    if (control.IsMultiple())
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    DWORD number_of_elements{ 0 };

    number_of_elements = number_of_channels * ((number_of_items_per_channel == 0) ? 1 : number_of_items_per_channel);

    DWORD buffer_size{ number_of_elements * size_of_element };

    auto memory_buffer{ std::make_unique<uint8_t[]>(buffer_size) };

    ASSERT(memory_buffer.get() not_eq nullptr);

    // Choose to live

    if (memory_buffer.get() == nullptr)
    {
        m_ErrorCode = MMSYSERR_NOMEM;
        return(false);
    }

    control_details.cbStruct = sizeof(control_details);
    control_details.dwControlID = control.ID;
    control_details.cChannels = number_of_channels;
    control_details.cMultipleItems = number_of_items_per_channel;
    control_details.cbDetails = size_of_element;
    control_details.paDetails = memory_buffer.get();

    DWORD flags{ 0 };

    flags = MIXER_GETCONTROLDETAILSF_VALUE;

#if defined( _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS )

    dump_mixer_control_details(THIS_FILE, __LINE__, TEXT("mixerGetControlDetails"), (HMIXEROBJ)m_Handle, &control_details, flags);

#endif // _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS

    m_ErrorCode = ::mixerGetControlDetails((HMIXEROBJ)m_Handle, &control_details, flags);

    bool return_value{ false };

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        return_value = false;
    }
    else
    {
        //CMixerControlDetailsData * data_p = nullptr;

        for ( auto const index : Range(number_of_elements) )
        {
            CMixerControlDetailsData entry;

            if (type_of_details == CMixerControl::Units::Boolean)
            {
                auto boolean_array{ reinterpret_cast<MIXERCONTROLDETAILS_BOOLEAN*>(memory_buffer.get()) };

                entry.Copy(boolean_array[index]);
            }
            else if (type_of_details == CMixerControl::Units::Signed or
                     type_of_details == CMixerControl::Units::Decibels)
            {
                auto signed_array{ reinterpret_cast<MIXERCONTROLDETAILS_SIGNED*>(memory_buffer.get()) };

               entry.Copy(signed_array[index]);
            }
            else if (type_of_details == CMixerControl::Units::Unsigned or
                     type_of_details == CMixerControl::Units::Percent)
            {
                auto unsigned_array{ reinterpret_cast<MIXERCONTROLDETAILS_UNSIGNED*>(memory_buffer.get()) };

                entry.Copy(unsigned_array[index]);
            }
            else
            {
                // We should never get here
                ASSERT(FALSE);
                return(false);
            }

            array.push_back(entry);
        }

        return_value = true;
    }

    return(return_value);
}

_Check_return_ bool Win32FoundationClasses::CMixer::GetControlListText(_In_ Win32FoundationClasses::CMixerLine const& line, _In_ Win32FoundationClasses::CMixerControl const& control, _Out_ std::vector<CMixerControlDetailsData>& array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    array.clear();

    if (control.IsList() == false)
    {
        return(false);
    }

    MIXERCONTROLDETAILS control_details;

    ::ZeroMemory(&control_details, sizeof(control_details));

    DWORD size_of_element{ 0 };

    size_of_element = sizeof(MIXERCONTROLDETAILS_LISTTEXT);

    DWORD number_of_items_per_channel{ 0 };

    if (control.IsUniform() == true)
    {
        number_of_items_per_channel = 1;
    }
    else
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    if (control.IsMultiple() == true)
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    DWORD number_of_elements{ 0 };
    DWORD number_of_channels{ line.NumberOfChannels };

    // This may be right for Lists but wrong with others...

    if (control.IsUniform() == true)
    {
        number_of_channels = 1;
    }

    number_of_elements = number_of_channels * number_of_items_per_channel;

    auto details_array{ std::make_unique<MIXERCONTROLDETAILS_LISTTEXT[]>(number_of_elements) };

    control_details.cbStruct = sizeof(control_details);
    control_details.dwControlID = control.ID;
    control_details.cChannels = number_of_channels;
    control_details.cMultipleItems = number_of_items_per_channel;
    control_details.cbDetails = size_of_element;
    control_details.paDetails = details_array.get();

    auto flags{ static_cast<DWORD>(Notifiers::notifyMixerNumber) bitor MIXER_GETCONTROLDETAILSF_LISTTEXT };

#if defined( _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS )

    dump_mixer_control_details(THIS_FILE, __LINE__, TEXT("mixerGetControlDetails"), (HMIXEROBJ)m_Handle, &control_details, flags);

#endif // _WFC_DEBUG_MIXER_GET_CONTROL_DETAILS

    m_ErrorCode = ::mixerGetControlDetails((HMIXEROBJ)m_DeviceID, &control_details, flags);

    bool return_value{ false };

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        return_value = false;
    }
    else
    {
        CMixerControlDetailsData entry;

        for ( auto const index : Range(number_of_elements) )
        {
           entry.Copy(details_array[index]);
           array.push_back(entry);
        }

        return_value = true;
    }

    return(return_value);
}

void Win32FoundationClasses::CMixer::GetErrorString(_Out_ std::wstring& error_string) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    switch (m_ErrorCode)
    {
    case MMSYSERR_NOERROR:

        error_string.assign(WSTRING_VIEW(L"No Error"));
        break;

    case MMSYSERR_ERROR:

        error_string.assign(WSTRING_VIEW(L"Unspecified Error"));
        break;

    case MMSYSERR_BADDEVICEID:

        error_string.assign(WSTRING_VIEW(L"Device ID out of range"));
        break;

    case MMSYSERR_NOTENABLED:

        error_string.assign(WSTRING_VIEW(L"Driver failed enable"));
        break;

    case MMSYSERR_ALLOCATED:

        error_string.assign(WSTRING_VIEW(L"Device already allocated"));
        break;

    case MMSYSERR_INVALHANDLE:

        error_string.assign(WSTRING_VIEW(L"Device handle is invalid"));
        break;

    case MMSYSERR_NODRIVER:

        error_string.assign(WSTRING_VIEW(L"No device driver present"));
        break;

    case MMSYSERR_NOMEM:

        error_string.assign(WSTRING_VIEW(L"Memory allocation error"));
        break;

    case MMSYSERR_NOTSUPPORTED:

        error_string.assign(WSTRING_VIEW(L"Function is not supported"));
        break;

    case MMSYSERR_BADERRNUM:

        error_string.assign(WSTRING_VIEW(L"Error value out of range"));
        break;

    case MMSYSERR_INVALFLAG:

        error_string.assign(WSTRING_VIEW(L"Invalid flag passed"));
        break;

    case MMSYSERR_INVALPARAM:

        error_string.assign(WSTRING_VIEW(L"Invalid parameter passed"));
        break;

    case MMSYSERR_HANDLEBUSY:

        error_string.assign(WSTRING_VIEW(L"Handle being used simultaneously on another thread (or callback)"));
        break;

    case MMSYSERR_INVALIDALIAS:

        error_string.assign(WSTRING_VIEW(L"Specified alias not found"));
        break;

    case MMSYSERR_BADDB:

        error_string.assign(WSTRING_VIEW(L"Bad registry database"));
        break;

    case MMSYSERR_KEYNOTFOUND:

        error_string.assign(WSTRING_VIEW(L"Registry key not found"));
        break;

    case MMSYSERR_READERROR:

        error_string.assign(WSTRING_VIEW(L"Registry read error"));
        break;

    case MMSYSERR_WRITEERROR:

        error_string.assign(WSTRING_VIEW(L"Registry write error"));
        break;

    case MMSYSERR_DELETEERROR:

        error_string.assign(WSTRING_VIEW(L"Registry delete error"));
        break;

    case MMSYSERR_VALNOTFOUND:

        error_string.assign(WSTRING_VIEW(L"Registry value not found"));
        break;

    case MMSYSERR_NODRIVERCB:

        error_string.assign(WSTRING_VIEW(L"Driver does not call DriverCallback"));
        break;

    case WAVERR_BADFORMAT:

        error_string.assign(WSTRING_VIEW(L"Unsupported Wave format"));
        break;

    case WAVERR_STILLPLAYING:

        error_string.assign(WSTRING_VIEW(L"Wave is still playing"));
        break;

    case WAVERR_UNPREPARED:

        error_string.assign(WSTRING_VIEW(L"Wave header not prepared"));
        break;

    case WAVERR_SYNC:

        error_string.assign(WSTRING_VIEW(L"Wave device is synchronous"));
        break;

    case MIDIERR_UNPREPARED:

        error_string.assign(WSTRING_VIEW(L"MIDI header not prepared"));
        break;

    case MIDIERR_STILLPLAYING:

        error_string.assign(WSTRING_VIEW(L"MIDI is still playing"));
        break;

    case MIDIERR_NOMAP:

        error_string.assign(WSTRING_VIEW(L"MIDI has no configured instruments"));
        break;

    case MIDIERR_NOTREADY:

        error_string.assign(WSTRING_VIEW(L"MIDI hardware is still busy"));
        break;

    case MIDIERR_NODEVICE:

        error_string.assign(WSTRING_VIEW(L"MIDI port no longer connected"));
        break;

    case MIDIERR_INVALIDSETUP:

        error_string.assign(WSTRING_VIEW(L"MIDI invalid MIF"));
        break;

    case MIDIERR_BADOPENMODE:

        error_string.assign(WSTRING_VIEW(L"MIDI operation unsupported with open mode"));
        break;

    case MIDIERR_DONT_CONTINUE:

        error_string.assign(WSTRING_VIEW(L"MIDI through device is eating a message"));
        break;

    case TIMERR_NOCANDO:

        error_string.assign(WSTRING_VIEW(L"Timer request not completed"));
        break;

    case TIMERR_STRUCT:

        error_string.assign(WSTRING_VIEW(L"Timer structure size error"));
        break;

    case JOYERR_PARMS:

        error_string.assign(WSTRING_VIEW(L"Joystick bad parameters"));
        break;

    case JOYERR_NOCANDO:

        error_string.assign(WSTRING_VIEW(L"Joystick request not completed"));
        break;

    case JOYERR_UNPLUGGED:

        error_string.assign(WSTRING_VIEW(L"Joystick is unplugged"));
        break;

    case MIXERR_INVALLINE:

        error_string.assign(WSTRING_VIEW(L"Mixer invalid line"));
        break;

    case MIXERR_INVALCONTROL:

        error_string.assign(WSTRING_VIEW(L"Mixer invalid control"));
        break;

    case MIXERR_INVALVALUE:

        error_string.assign(WSTRING_VIEW(L"Mixer invalid value"));
        break;

    default:

        error_string.assign(WSTRING_VIEW(L"Unknown ("));
        error_string.append(std::to_wstring(m_ErrorCode));
        error_string.push_back(')');

#if defined( _DEBUG )

        if (m_ErrorCode > MMSYSERR_BASE)
        {
            std::wstring debug_string;

            Win32FoundationClasses::format( debug_string, L"Unknown (MMSYSERR_BASE + %" PRIu32 ")", m_ErrorCode - MMSYSERR_BASE );
            //WFCTRACE( debug_string );
        }
        else
        {
            //WFCTRACE( TEXT( "I'm really confused now..." ) );
        }

#endif

        break;
    }
}

_Check_return_ std::size_t Win32FoundationClasses::CMixer::GetNumberOfDevices(void) const noexcept
{
    WFC_VALIDATE_POINTER(this);

    return(::mixerGetNumDevs());
}

_Check_return_ bool Win32FoundationClasses::CMixer::Open(_In_ UINT_PTR device_id, _In_ DWORD what_to_notify, _In_ DWORD_PTR who_to_notify, _In_ DWORD_PTR notify_data) noexcept
{
    WFC_VALIDATE_POINTER(this);

    m_ErrorCode = ::mixerOpen(&m_Handle, (UINT) device_id, who_to_notify, notify_data, what_to_notify);

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        return(false);
    }

    m_DeviceID = device_id;

    return(true);
}

_Check_return_ bool Win32FoundationClasses::CMixer::SetControlDetails(_In_ Win32FoundationClasses::CMixerLine const& line, _In_ Win32FoundationClasses::CMixerControl const& control, _In_ std::vector<Win32FoundationClasses::CMixerControlDetailsData> const& settings_array) noexcept
{
    WFC_VALIDATE_POINTER(this);

    auto const type_of_details{ control.GetUnits() };

    DWORD size_of_element{ 0 };

    if (type_of_details == CMixerControl::Units::Boolean)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
    }
    else if (type_of_details == CMixerControl::Units::Signed or
        type_of_details == CMixerControl::Units::Decibels)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_SIGNED);
    }
    else if (type_of_details == CMixerControl::Units::Unsigned or
        type_of_details == CMixerControl::Units::Percent)
    {
        size_of_element = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
    }
    else
    {
        //WFCTRACE( TEXT( "Unknown type of details to get" ) );
        return(false);
    }

    DWORD number_of_items_per_channel{ 0 };
    DWORD number_of_channels{ line.NumberOfChannels };

    if (control.IsUniform())
    {
        number_of_items_per_channel = 1;
        number_of_channels = 1;

        if (type_of_details == CMixerControl::Units::Boolean)
        {
            number_of_items_per_channel = 0;
        }
    }
    else
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    if (control.IsMultiple())
    {
        number_of_items_per_channel = control.NumberOfItemsPerChannel;
    }

    std::size_t number_of_elements{ number_of_channels * ((number_of_items_per_channel == 0) ? 1 : number_of_items_per_channel) };

    //WFCTRACEVAL( TEXT( "Number of elements is " ), (DWORD) number_of_elements );

    std::size_t const buffer_size{ number_of_elements * size_of_element };

    auto memory_buffer{ std::make_unique<uint8_t[]>(buffer_size) };

    for ( auto const loop_index : Range(settings_array.size()))
    {
        if (type_of_details == CMixerControl::Units::Boolean)
        {
            auto array{ reinterpret_cast<MIXERCONTROLDETAILS_BOOLEAN*>(memory_buffer.get()) };

            array[loop_index].fValue = ((settings_array.at(loop_index).Parameter1 == 0) ? FALSE : TRUE);
        }
        else if (type_of_details == CMixerControl::Units::Signed or
                 type_of_details == CMixerControl::Units::Decibels)
        {
            auto array{ reinterpret_cast<MIXERCONTROLDETAILS_SIGNED*>(memory_buffer.get()) };

            array[loop_index].lValue = settings_array.at(loop_index).Parameter1;
        }
        else if (type_of_details == CMixerControl::Units::Unsigned or
                 type_of_details == CMixerControl::Units::Percent)
        {
            auto array{ reinterpret_cast<MIXERCONTROLDETAILS_UNSIGNED*>(memory_buffer.get()) };

            array[loop_index].dwValue = settings_array.at(loop_index).Parameter1;
        }
        else
        {
           // We should never get here
           ASSERT(FALSE);
           return(false);
        }
    }

    MIXERCONTROLDETAILS control_details;

    ::ZeroMemory(&control_details, sizeof(control_details));

    control_details.cbStruct = sizeof(control_details);
    control_details.dwControlID = control.ID;
    control_details.cChannels = number_of_channels;
    control_details.cMultipleItems = number_of_items_per_channel;
    control_details.cbDetails = size_of_element;
    control_details.paDetails = memory_buffer.get();

    DWORD flags{ MIXER_GETCONTROLDETAILSF_VALUE };

#if defined( _WFC_DEBUG_MIXER_SET_CONTROL_DETAILS )

    dump_mixer_control_details(THIS_FILE, __LINE__, TEXT("mixerSetControlDetails"), (HMIXEROBJ)m_Handle, &control_details, flags);

#endif // _WFC_DEBUG_MIXER_SET_CONTROL_DETAILS

    m_ErrorCode = ::mixerSetControlDetails((HMIXEROBJ)m_Handle, &control_details, flags);

    bool return_value{ false };

    if (m_ErrorCode not_eq MMSYSERR_NOERROR)
    {
        //WFCTRACE( TEXT( "mixerSetControlDetails() failed" ) );
        //WFCTRACEVAL( TEXT( "Error code is " ), m_ErrorCode );
        return_value = false;
    }
    else
    {
        return_value = true;
    }

    return(return_value);
}

// End of source

/*
<HTML>

<HEAD>
<TITLE>WFC - CMixer</TITLE>
<META name="keywords" content="WFC, MFC extension library, freeware class library, Win32, source code">
<META name="description" content="The C++ class that handles the Mixer API.">
</HEAD>

<BODY>

<H1>CMixer : CObject</H1>
$Revision: 24 $<BR><HR>

<H2>Description</H2>
This is the class handles general mixer stuff. It retrieves things
like mixer capabilities, opening and closing.

<H2>Data Members</H2>None.

<H2>Methods</H2>

<DL COMPACT>

<DT><B>Close</B><DD>Closes the connection to the mixer device.
<DT><B>Get</B><DD>Retrieves the mixer's capabilities via CMixerCapabilities.
<DT><B>GetAllControls</B><DD>
<DT><B>GetByComponent</B><DD>
<DT><B>GetByConnection</B><DD>
<DT><B>GetByDestination</B><DD>
<DT><B>GetByID</B><DD>
<DT><B>GetControlDetails</B><DD>
<DT><B>GetControlListText</B><DD>
<DT><B>GetDeviceID</B><DD>
<DT><B>GetErrorCode</B><DD>
<DT><B>GetErrorString</B><DD>
<DT><B>GetHandle</B><DD>
<DT><B>GetNumberOfDevices</B><DD>
<DT><B>Open</B><DD>
<DT><B>Serialize</B><DD>
<DT><B>SetControlDetails</B><DD>
</DL>

<H2>Example</H2><PRE><CODE>Sorry.</CODE></PRE>

<H2>API's Used</H2>

<UL>
<LI>mixerClose
<LI>mixerGetControlDetails
<LI>mixerGetDevCaps
<LI>mixerGetLineControls
<LI>mixerGetLineInfo
<LI>mixerGetNumDevs
<LI>mixerOpen
<LI>mixerSetControlDetails
</UL>

<HR><I>Copyright, 2000, <A HREF="mailto:wfc@pobox.com">Samuel R. Blackburn</A></I><BR>
$Workfile: CMixer.cpp $<BR>
$Modtime: 6/26/01 10:47a $
</BODY>

</HTML>
*/

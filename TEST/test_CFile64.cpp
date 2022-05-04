/*
** Author: Samuel R. Blackburn
** Internet: wfc@pobox.com
**
** Copyright, 2000-2020, Samuel R. Blackburn
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
** $Workfile: test_CBitArray.cpp $
** $Revision: 3 $
** $Modtime: 8/06/00 3:25p $
*/

// SPDX-License-Identifier: BSD-2-Clause

#include "test.h"
#pragma hdrstop

#include <random>

#if defined( _DEBUG ) && defined( _INC_CRTDBG )
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif // _DEBUG

//#define SECURITY_SETTINGS (L"D:AI(A;;FA;;;AU)(A;;FA;;;BU)(A;ID;FA;;;SY)(A;ID;FA;;;BA)(A;ID;0x1200a9;;;BU)")
#define SECURITY_SETTINGS (L"D:AI(A;OICI;FA;;;AU)(A;OICI;FA;;;BA)(A;OICI;FA;;;BU)(A;OICIID;FA;;;SY)(A;OICIID;FA;;;BA)(A;OICIIOID;GA;;;CO)(A;OICIID;0x1200a9;;;BU)(A;CIID;DCLCRPCR;;;BU)")

struct atomic_thread_parameters
{
    Win32FoundationClasses::CFile64 * file{ nullptr };
    bool done{ false };
};

static void atomic_read_thread(void* p) noexcept
{
    static constexpr std::size_t const buffer_size{ (10 * 1024 * 1024) + 1037 };
    auto buffer{ std::make_unique<uint8_t[]>(buffer_size) };

    auto event_handle{ MANUAL_RESET_EVENT() };

    auto parameters{ static_cast<atomic_thread_parameters*>(p) };

    //std::mt19937 random(::GetCurrentThreadId());

    std::size_t const increment{ ::GetCurrentThreadId() + 329787 };

    std::size_t offset{ increment };
    auto const file_size{ parameters->file->GetLength() - buffer_size };

    //std::uniform_int_distribution<uint32_t> range_of_sizes(1, sizeof(buffer));
    //std::uniform_int_distribution<uint64_t> range_of_offsets(0, parameters->file->GetLength() - range_of_sizes.max());

    for (auto const loop_index : Range(10000))
    {
        offset += increment;

        if (offset > file_size)
        {
            offset -= file_size;
        }

        if (parameters->file->AtomicRead(event_handle, offset, buffer.get(), buffer_size) not_eq buffer_size)
        {
            printf("FAILED TO ATOMIC READ!\n");
        }
        else if (parameters->file->AtomicWaitSucceeded() == true )
        {
            printf("AtomicWaitSucceeded!\n");
            Win32FoundationClasses::wfc_close_handle(event_handle);
            return;
        }
    }

    Win32FoundationClasses::wfc_close_handle(event_handle);

    parameters->done = true;
}

_Check_return_ bool test_CFile64(_Out_ std::string& class_name, _Out_ int& test_number_that_failed) noexcept
{
    class_name.assign(STRING_VIEW("CFile64"));

    test_number_that_failed = 0;

    TCHAR temporary_path[MAX_PATH + 1];

    ZeroMemory(temporary_path, sizeof(temporary_path));

    if (::GetTempPath(static_cast<DWORD>(std::size(temporary_path)), temporary_path) == 0)
    {
        return(failure());
    }

    TCHAR temporary_filename[MAX_PATH + 1];

    ZeroMemory(temporary_filename, sizeof(temporary_filename));

    Win32FoundationClasses::CRandomNumberGenerator2 random_number_generator;

    bool file_exists{ true };

    while (file_exists not_eq false)
    {
        if (::GetTempFileName(temporary_path,
            TEXT("WFC"),
            random_number_generator.GetInteger(),
            temporary_filename) == 0)
        {
            return(failure());
        }

        file_exists = Win32FoundationClasses::wfc_does_file_exist(temporary_filename);
    }

    // If we get here, it means the filename generated above is a good one. No file
    // exists of that name.

    auto buffer{ std::make_unique<uint8_t[]>(ONE_MEGABYTE) };

    if (buffer.get() == nullptr)
    {
        return(failure());
    }

    for (auto const loop_index : Range(ONE_MEGABYTE))
    {
        buffer[loop_index] = 'A';
    }

    Win32FoundationClasses::CFile64 file;

    if (file.Open(temporary_filename, (UINT)((UINT)Win32FoundationClasses::CFile64::OpenFlags::modeCreate bitor (UINT)Win32FoundationClasses::CFile64::OpenFlags::wfcDeleteOnClose bitor (UINT)Win32FoundationClasses::CFile64::OpenFlags::modeReadWrite)) == false)
    {
        test_number_that_failed = 1;
        return(failure());
    }

    file.Write(buffer.get(), ONE_MEGABYTE);

    if (file.GetLength() not_eq ONE_MEGABYTE)
    {
        test_number_that_failed = 2;
        return(failure());
    }

    if (file.GetPosition() not_eq ONE_MEGABYTE)
    {
        test_number_that_failed = 2;
        return(failure());
    }

    if (file.Seek(0, Win32FoundationClasses::CFile64::SeekPosition::begin) not_eq 0)
    {
        test_number_that_failed = 3;
        return(failure());
    }

    if (file.GetPosition() not_eq 0)
    {
        test_number_that_failed = 4;
        return(failure());
    }

    if (file.Seek(0, Win32FoundationClasses::CFile64::SeekPosition::end) not_eq ONE_MEGABYTE)
    {
        test_number_that_failed = 5;
        return(failure());
    }

    if (file.GetPosition() not_eq ONE_MEGABYTE)
    {
        test_number_that_failed = 6;
        return(failure());
    }

    auto const position{ file.Seek(-ONE_MEGABYTE, Win32FoundationClasses::CFile64::SeekPosition::end) };

    if (position not_eq 0)
    {
        test_number_that_failed = 7;
        return(failure());
    }

    if (file.GetPosition() not_eq 0)
    {
        test_number_that_failed = 8;
        return(failure());
    }

    ZeroMemory(buffer.get(), ONE_MEGABYTE);

    if (file.Read(buffer.get(), ONE_MEGABYTE) not_eq ONE_MEGABYTE)
    {
        test_number_that_failed = 9;
        return(failure());
    }

    for (auto const loop_index : Range(ONE_MEGABYTE))
    {
        if (buffer[loop_index] not_eq 'A')
        {
            test_number_that_failed = 10;
            return(failure());
        }
    }

    Win32FoundationClasses::CMemoryFile memory_mapped_file;

    if (memory_mapped_file.FromHandle(file.GetHandle(), (UINT)Win32FoundationClasses::CFile64::OpenFlags::modeRead, 0, 64 * ONE_KILOBYTE) == false)
    {
        test_number_that_failed = 11;
        return(failure());
    }

    auto memory_mapped_pointer = memory_mapped_file.GetPointer();

    WFC_VALIDATE_POINTER(memory_mapped_pointer);

    // If we get here it means we have successfully memory mapped the first 64KB of the file

    if (file.Seek(ONE_MEGABYTE - 1, Win32FoundationClasses::CFile64::SeekPosition::begin) not_eq (ONE_MEGABYTE - 1))
    {
        test_number_that_failed = 12;
        return(failure());
    }

    if (file.GetPosition() not_eq (ONE_MEGABYTE - 1))
    {
        test_number_that_failed = 13;
        return(failure());
    }

    // This is pretty stupid. If ANY part of a file is memory mapped in Win32, you cannot
    // set the end of file for that file. Never mind the fact that you are truncating the
    // file outside of the region that is being mapped. It would be interesting to find out
    // what the child at Microsoft was thinking when they coded that one up... probably
    // late for a ping pong match in the hallway...

    // This next line will cause an ASSERTE() in debug builds. Just click "Ignore" 
    // and let the test continue.

    if (file.SetEndOfFile(ONE_MEGABYTE - 1) not_eq false)
    {
        test_number_that_failed = 14;
        return(failure());
    }

    memory_mapped_file.Close();

    if (Win32FoundationClasses::wfc_does_file_exist(temporary_filename) == false)
    {
        test_number_that_failed = 15;
        return(failure());
    }

    file.Close();

    if (Win32FoundationClasses::wfc_does_file_exist(temporary_filename) not_eq false)
    {
        test_number_that_failed = 16;
        return(failure());
    }

    // Test creating a file delete on close 
    // Test set enf of file on memory mapped portion

    // Now test the string reading methods

    if (file.Open(temporary_filename, (UINT)((UINT)Win32FoundationClasses::CFile64::OpenFlags::modeCreate bitor (UINT)Win32FoundationClasses::CFile64::OpenFlags::modeReadWrite)) == false)
    {
        test_number_that_failed = 17;
        return(failure());
    }

    char temp_string[512];

    int number_of_characters_to_write = 0;

    for (auto const loop_index : Range(100))
    {
        number_of_characters_to_write = sprintf_s(temp_string, sizeof(temp_string), "Test String %04lu", (unsigned long)loop_index);

        file.Write(temp_string, number_of_characters_to_write);

        // Terminate strings with one of four ways...
        // 1) Carriage Return and Line Feed Pair
        // 2) Carriage Return only
        // 3) Line Feed only
        // 4) NULL terminator

        if ((loop_index % 4) == 0)
        {
            BYTE end_of_line[2]{ CARRIAGE_RETURN, LINE_FEED };
            file.Write(end_of_line, 2);
        }
        else if ((loop_index % 4) == 1)
        {
            BYTE end_of_line[1]{ CARRIAGE_RETURN };
            file.Write(end_of_line, 1);
        }
        else if ((loop_index % 4) == 2)
        {
            BYTE end_of_line[1]{ LINE_FEED };
            file.Write(end_of_line, 1);
        }
        else
        {
            BYTE end_of_line[1]{ 0 };
            file.Write(end_of_line, 1);
        }
    }

    file.SeekToBegin();

    if (file.GetPosition() not_eq 0)
    {
        test_number_that_failed = 18;
        return(failure());
    }

    std::vector<std::string> strings;

    if (file.Read(strings) not_eq true)
    {
        test_number_that_failed = 19;
        return(failure());
    }

    for (auto const loop_index : Range(100))
    {
        std::size_t const number_of_characters = sprintf_s(temp_string, sizeof(temp_string), "Test String %04zu", loop_index);

        if (strings.at(loop_index).compare(std::string_view(temp_string, number_of_characters)) not_eq I_AM_EQUAL_TO_THAT)
        {
            test_number_that_failed = 20;
            return(failure());
        }
    }

    strcpy_s(temp_string, std::size(temp_string), "<ROOT><A><B><C>WFC</C></B></A></ROOT>");

    file.SetLength(0);
    file.Write(temp_string, static_cast<UINT>(strlen(temp_string)));

    file.SeekToBegin();

    Win32FoundationClasses::CExtensibleMarkupLanguageDocument xml;

    if (file.Read(xml) not_eq true)
    {
        test_number_that_failed = 21;
        return(failure());
    }

    auto element_p = xml.GetElement(WSTRING_VIEW(L"ROOT.A.B.C"));

    if (element_p == nullptr)
    {
        test_number_that_failed = 22;
        return(failure());
    }

    std::wstring xml_text;

    element_p->GetText(xml_text);

    if (xml_text.compare(WSTRING_VIEW(L"WFC")) not_eq I_AM_EQUAL_TO_THAT)
    {
        test_number_that_failed = 23;
        return(failure());
    }

    auto manual_reset_event_handle = MANUAL_RESET_EVENT();

    uint8_t buffer2[128];

    if (file.AtomicRead(manual_reset_event_handle, 0, buffer2, 37) not_eq 37)
    {
        test_number_that_failed = 23;
        return(failure());
    }

    file.Close();

    Win32FoundationClasses::CFile64 f2;

    if (f2.Open(L"E:\\Users\\Sam\\Downloads\\ContactSheet.DMP", Win32FoundationClasses::read_file_open_mode()) == true)
    {
        std::vector<atomic_thread_parameters> threads;

        for (auto const loop_index : Range(10))
        {
            atomic_thread_parameters p;

            p.file = &f2;
            p.done = false;

            threads.push_back(p);
        }

        printf("Starting %zu threads to test atomic reads\n", threads.size());

        for (auto& entry : threads)
        {
            _beginthread(atomic_read_thread, DEFAULT_THREAD_STACK_SIZE, &entry);
        }

        bool all_done{ false };

        while (all_done == false)
        {
            all_done = true;

            for (auto const& entry : threads)
            {
                if (entry.done == false)
                {
                    all_done = false;
                }
            }

            if (all_done == false)
            {
                ::Sleep(2000);
            }
        }

        if (file.AtomicWaitSucceeded() == true)
        {
            printf("PASS!!!!!\n");
        }
        else
        {
            printf("Could not force an atomic wait\n");
        }
    }
    else
    {
        printf("Can't open input file.\n");
    }

   Win32FoundationClasses::CFile64 log_file;

   log_file.SetSecurity(SECURITY_SETTINGS);

   std::wstring program_data;

   Win32FoundationClasses::wfc_get_program_data_directory(program_data);

   program_data.append(WSTRING_VIEW(L"WFC_TEST"));

   (void)Win32FoundationClasses::wfc_create_wide_path(program_data.c_str(), log_file.GetSecurityAttributes());

   if (Win32FoundationClasses::CFileDirectory::IsDirectory(program_data) == false)
   {
       test_number_that_failed = 24;
       return(failure());
   }

   (void)Win32FoundationClasses::CFileDirectory::Destroy(program_data);

   test_number_that_failed = 24;

   return( true );
}

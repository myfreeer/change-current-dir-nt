#include "dll-hijack/version.h"
#include <winternl.h>

NTSYSAPI NTSTATUS NTAPI RtlSetCurrentDirectory_U(_In_ PUNICODE_STRING PathName);

void DLLHijackAttach(bool isSucceed) {
  if (!isSucceed) return;
  wchar_t path[MAX_PATH];
  UNICODE_STRING modulePath = {
      .Buffer = path,
      .Length = 0,
      .MaximumLength = sizeof(path)
  };

  UNICODE_STRING *imagePathName =
      &NtCurrentTeb()->ProcessEnvironmentBlock->ProcessParameters->ImagePathName;
  USHORT sizeW = min(modulePath.MaximumLength, imagePathName->Length);
  memcpy(path, imagePathName->Buffer, sizeW);
  wchar_t *pathEnd = wcsrchr(path, L'\\');
  if (pathEnd) {
    *pathEnd = 0;
    modulePath.Length = (pathEnd - path) * sizeof(wchar_t);
  } else {
    modulePath.Length = sizeW;
  }
  RtlSetCurrentDirectory_U(&modulePath);
}

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#include <assert.h>
#include "stdafx.h"
#include "string_utils.h"


using namespace Microsoft::CognitiveServices::Speech::Impl;

SPXAPI TranslationResult_GetTranslationText(SPXRESULTHANDLE handle, Result_TranslationTextBufferHeader* textBuffer, size_t* lengthPointer)
{
    SPX_RETURN_HR_IF(SPXERR_INVALID_ARG, lengthPointer == nullptr);

    SPXAPI_INIT_HR_TRY(hr)
    {
        auto resulthandles = CSpxSharedPtrHandleTableManager::Get<ISpxRecognitionResult, SPXRESULTHANDLE>();
        auto result = (*resulthandles)[handle];

        auto textResult = SpxQueryInterface<ISpxTranslationTextResult>(result);
        auto translationPhrases = textResult->GetTranslationText();

        size_t entries = translationPhrases.size();
        size_t sizeInBytes = sizeof(Result_TranslationTextBufferHeader) + sizeof(wchar_t *) * entries * 2; /* space for targetLanguages and translationTexts array*/
        for (const auto& it : translationPhrases)
        {
            sizeInBytes += (it.first.size() + 1 + it.second.size() + 1) * sizeof(wchar_t);
        }

        if ((textBuffer == nullptr) || (*lengthPointer < sizeInBytes))
        {
            *lengthPointer = sizeInBytes;
            SPX_RETURN_HR(SPXERR_BUFFER_TOO_SMALL);
        }

        textBuffer->bufferSize = sizeInBytes;
        textBuffer->numberEntries = entries;
        textBuffer->targetLanguages = reinterpret_cast<wchar_t **>(textBuffer + 1);
        textBuffer->translationTexts = textBuffer->targetLanguages + entries;
        auto data = reinterpret_cast<wchar_t *>(textBuffer->translationTexts + entries);
        size_t index = 0;
        for (const auto& it : translationPhrases)
        {
            std::wstring lang, text;
            std::tie(lang, text) = it;
            PAL::wcscpy(data, lang.size() + 1, lang.c_str(), lang.size() + 1, true);
            textBuffer->targetLanguages[index] = data;
            data += lang.size() + 1;
            PAL::wcscpy(data, text.size() + 1, text.c_str(), text.size() + 1, true);
            textBuffer->translationTexts[index] = data;
            data += text.size() + 1;
            index++;
        }
        *lengthPointer = sizeInBytes;
        if (reinterpret_cast<char *>(data) - reinterpret_cast<char *>(textBuffer) != (int)sizeInBytes)
        {
            SPX_THROW_HR(SPXERR_RUNTIME_ERROR);
        }
    }
    SPXAPI_CATCH_AND_RETURN_HR(hr);
}

//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//
package com.microsoft.cognitiveservices.speech;

import com.microsoft.cognitiveservices.speech.util.Contracts;
import com.microsoft.cognitiveservices.speech.CancellationReason;
import com.microsoft.cognitiveservices.speech.CancellationErrorCode;
import com.microsoft.cognitiveservices.speech.SpeechRecognitionResult;

/**
 * Defines payload of speech recognition canceled events.
 */
public final class SpeechRecognitionCanceledEventArgs extends SpeechRecognitionEventArgs {

    /*! \cond INTERNAL */

    /**
     * Constructs an instance of a SpeechRecognitionCanceledEventArgs object.
     * @param eventArgs recognition canceled event args object.
     */
    public SpeechRecognitionCanceledEventArgs(long eventArgs) {
        super(eventArgs);
        storeEventData(false);
    }

    public SpeechRecognitionCanceledEventArgs(long eventArgs, boolean dispose) {
        super(eventArgs);
        storeEventData(dispose);
    }

    /*! \endcond */

    /**
     * The reason the recognition was canceled.
     * @return Specifies the reason canceled.
     */
    public CancellationReason getReason() {
        return this.cancellationReason;
    }

    /**
     * The error code in case of an unsuccessful recognition (when getReason() returns Error).
     * Added in version 1.1.0.
     * @return An error code that represents the error reason.
     */
    public CancellationErrorCode getErrorCode() {
        return this.errorCode;
    }

    /**
     * The error message in case of an unsuccessful recognition (when getReason() returns Error).
     * @return A String that represents the error details.
     */
    public String getErrorDetails() {
        return this.errorDetails;
    }

    /**
     * Returns a String that represents the speech recognition canceled event args.
     * @return A String that represents the speech recognition canceled event args.
     */
    @Override
    public String toString() {
        return "SessionId:" + getSessionId() +
                " ResultId:" + getResult().getResultId() +
                " CancellationReason:" + cancellationReason  +
                " CancellationErrorCode:" + errorCode +
                " Error details:<" + errorDetails;
    }

    private void storeEventData(boolean disposeNativeResources) {
        Contracts.throwIfNull(eventHandle, "eventHandle");        
        CancellationDetails cancellation = CancellationDetails.fromResult(getResult());
        this.cancellationReason  = cancellation.getReason();
        this.errorCode = cancellation.getErrorCode();
        this.errorDetails = cancellation.getErrorDetails();
        if (disposeNativeResources == true) {
            super.close();
        }
    }

    private CancellationReason cancellationReason;
    private CancellationErrorCode errorCode;
    private String errorDetails;
}
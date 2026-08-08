#pragma once

#include <MidiFile.h>
using namespace smf;
struct Note {
    //everything is in ticks!
    double absNoteOnTime, absNoteOffTime;
    int noteNumber;
    bool passed;
    Note(smf::MidiEvent & event, int ticks, double BPM, int PPQ) : absNoteOnTime(0.0), absNoteOffTime(0.0) {
        // assumes it is a note on event
        passed = false;
        absNoteOnTime = (60.0 * ticks) / (BPM * PPQ);;
        absNoteOffTime = absNoteOnTime + event.getDurationInSeconds();
        noteNumber = event.getKeyNumber();
        //std::cout << "Note with Key " << noteNumber <<  " initialized from " << absNoteOnTime << " to " << absNoteOffTime << ".\n";
    }
};
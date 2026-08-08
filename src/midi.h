#include <iostream>
#include <vector>
#include <math.h>
#include "note.h"

#include <MidiFile.h>
using namespace smf;
class MidiData {
public:
    std::vector<Note*> notes;
    double BPM, PPQ;
    std::string filename;
    MidiData (const std::string& filename_, double BPM = 120.0) {
        filename = filename_;
        std::cout << "Beginning MIDI parse... \n";
        MidiFile midiReader;
        midiReader.read(filename);
        int numTracks = midiReader.getNumTracks();
        int numEvents = midiReader.getNumEvents(0);
        if (numEvents == 0) {
            throw std::runtime_error("No events to parse, maybe bad filename?");
        }
        midiReader.doTimeAnalysis();
        std::cout << midiReader.getNumTracks() << " tracks.\n";
        PPQ = midiReader.getTicksPerQuarterNote();
        int numNotes = 0;
        for (int t = 0; t<midiReader.getNumTracks(); t++) {
            for (int i = 0; i<midiReader.getNumEvents(t); i++) {
                MidiEvent & currentEvent = midiReader.getEvent(t, i);
                if (currentEvent.isTempo()) {
                    // BPM = currentEvent.getTempoBPM();
                    // std::cout << "tempo msg! overwriting BPM to " << BPM << " \n";

                }else if (currentEvent.isNoteOn()) {
                    this->notes.push_back(new Note (currentEvent, currentEvent.tick, BPM, PPQ));
                    numNotes++;
                }
            }
        }
        std::cout << "Saved " << numNotes << " notes for processing\n";
    }
    [[nodiscard]] double tickToSec(int ticks) const {
        return (60.0 * ticks) / (BPM * PPQ);
    }
    [[nodiscard]] int secToTicks(double seconds) const {
        return floor(seconds*BPM*PPQ / 60.0);
        ///
        ///sec = 60*ticks / BPM*PPQ
        ///sec*BPM*PPQ = 60*ticks
        ///sec*BPM*PPQ/60 = ticks
        ///
    }
};

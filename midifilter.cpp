#include "RtMidi.h"
#include <string.h>

bool isVerbose = true;

void mycallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    RtMidiOut *midiout = static_cast<RtMidiOut *>(userData);

    // MIDI message format
    // byte 0 = status byte
    // byte 1 data
    // byte 2 data
    unsigned int messageSize = message->size();

    if (messageSize < 1)
    {
        // Not sure if emtpy messages can actually get here, but let's ignore
        return;
    }

    unsigned char statusByte = message->at(0);

    // Is it a status byte?
    if ((statusByte && 0xF0) > 0)
    {
        // Note on
        if (statusByte == 0x90)
        {
            // Ensure we have the 3rd value (should always be true)
            if (messageSize >= 2)
            {
                unsigned char velocity = message->at(2);
                if (velocity == 0)
                {
                    if (isVerbose)
                    {
                        std::cout << "Removing 0 velocity note off\n";
                    }
                    return;
                }
            }
        }
    }
    if (isVerbose)
    {
        std::cout << "Sending ";
        for (unsigned i=0; i < messageSize; i++)
        {
            std::cout << "byte " << i << "=" << (int)message->at(i) << " ";
        }
         std::cout << "\n";
    }
    midiout->sendMessage(message);
}

int main()
{
    RtMidiIn *midiin = 0;
    RtMidiOut *midiout = 0;
    try
    {
        midiin = new RtMidiIn(RtMidi::Api::LINUX_ALSA, "midifilter input");
        midiout = new RtMidiOut(RtMidi::Api::LINUX_ALSA, "midifilter output");
    }
    catch (RtMidiError &error)
    {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    unsigned int ports = midiin->getPortCount();
    if (ports <= 0)
    {
        // TODO: change this to a loop style behaviour - detect ports, subscribe
        std::cout << "No MIDI input ports detected. Exiting...\n";
        exit(0);
    }

    std::string portName;
    for (unsigned int i = 0; i < ports; i++)
    {
        try
        {
            portName = midiin->getPortName(i);

            // TODO: Use command line as input instead (or subscribe to all?)
            std::size_t found = portName.find("VMPK");
            if (found == std::string::npos)
            {
                found = portName.find("mio");
            }

            if (found != std::string::npos)
            {
                std::cout << "Connecting to " << portName << " on port " << i << '\n';

                midiout->openVirtualPort("midifilter output port");
                midiin->openPort(i);
                midiin->setCallback(&mycallback, midiout);

                // Disable ignoring of sysex, timing and active sense message (we want it all)
                midiin->ignoreTypes(false, false, false);

                std::cout << "Filter enabled. Press <Ctrl+c> to quit.\n";

                std::string str;
                while (true)
                {
                    std::getline(std::cin, str);
                }

                midiin->closePort();
                midiout->closePort();
            }
        }
        catch (RtMidiError &error)
        {
            error.printMessage();
            delete midiin;
            delete midiout;
            exit(EXIT_FAILURE);
        }
    }
    delete midiin;
    delete midiout;
    return 0;
}
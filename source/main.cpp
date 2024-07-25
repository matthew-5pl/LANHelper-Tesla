#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header

#include <iostream>
#include <lanhelper.h>

#define TITLE "LAN Play IP Generator"
#define VERSION "v1.0.0"

Result genRes = 0;

class MainGui;

class GenFailureGui : public tsl::Gui {
public:
    GenFailureGui() {}

    virtual tsl::elm::Element* createUI() override {
        auto frame = new tsl::elm::OverlayFrame(TITLE, VERSION);
        auto list = new tsl::elm::List();

        std::string label = "IP assign failed! Result code: " + std::format("{:x}", genRes);

        list->addItem(new tsl::elm::CategoryHeader(label), true);

        frame->setContent(list);

        return frame;
    }
};

class GenResultGui : public tsl::Gui {
public:
    GenResultGui() {}

    virtual tsl::elm::Element* createUI() override {
        int ip_int[4];
        char ip_str[15];

        genIp(ip_str, ip_int);

        auto frame = new tsl::elm::OverlayFrame(TITLE, VERSION);
        auto list = new tsl::elm::List();

        std::string label = "Generated IP address: " + std::string(ip_str);

        list->addItem(new tsl::elm::CategoryHeader(label), true);

        auto* doneButton = new tsl::elm::ListItem("Apply IP!");
        doneButton->setClickListener([ip_int, list, frame](u64 keys) {
            if (keys & HidNpadButton_A) {
                Result res = setIp(ip_int);

                if(R_SUCCEEDED(res)) {
                    tsl::changeTo<MainGui>();
                } else {
                    genRes = res;
                    tsl::changeTo<GenFailureGui>();
                }

                return true;
            }

            return false;
        });

        auto* backButton = new tsl::elm::ListItem("Cancel");
        backButton->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                tsl::changeTo<MainGui>();

                return true;
            }

            return false;
        });

        list->addItem(doneButton);
        list->addItem(backButton);

        frame->setContent(list);

        return frame;
    }
};

class MainGui : public tsl::Gui {
public:
    MainGui() { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame(TITLE, VERSION);

        // A list that can contain sub ellements and handles scrolling
        auto list = new tsl::elm::List();

        // this crashes the console so nuh uh
        // list->setClickListener([](u64 keys) {
        //     if(keys & HidNpadButton_B) {
        //         exit(0);
        // 
        //         return true;
        //     }
        // 
        //     return false;
        // });

        auto* generateButton = new tsl::elm::ListItem("Generate IP address");
        generateButton->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                tsl::changeTo<GenResultGui>();

                return true;
            }

            return false;
        });

        auto* resetButton = new tsl::elm::ListItem("Reset IP address");
        resetButton->setClickListener([](u64 keys) {
            if (keys & HidNpadButton_A) {
                defaultIP();

                return true;
            }

            return false;
        });


        // Create and add a new list item to the list
        list->addItem(generateButton);
        list->addItem(resetButton);

        // Add the list to the frame for it to be drawn
        frame->setContent(list);

        // Return the frame to have it become the top level element of this Gui
        return frame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, const HidTouchState &touchPos, HidAnalogStickState joyStickPosLeft, HidAnalogStickState joyStickPosRight) override {
        return false;   // Return true here to signal the inputs have been consumed
    }
};

class LANHelperOverlay : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {
        nifmInitialize(NifmServiceType_Admin);
    }  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {
        nifmExit();
    }  // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<MainGui>();  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<LANHelperOverlay>(argc, argv);
}

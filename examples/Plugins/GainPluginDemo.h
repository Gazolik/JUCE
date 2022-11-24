/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2022 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             GainPlugin
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Gain audio plugin.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra, juce_opengl
 exporters:        xcode_mac, vs2022

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             AudioProcessor
 mainClass:        GainProcessor

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once

#include <juce_opengl/juce_opengl.h>

//==============================================================================



class GainProcessor  : public AudioProcessor
{
public:

    //==============================================================================
    GainProcessor()
        : AudioProcessor (BusesProperties().withInput  ("Input",  AudioChannelSet::stereo())
                                           .withOutput ("Output", AudioChannelSet::stereo()))
    {
        addParameter (gain = new AudioParameterFloat ({ "gain", 1 }, "Gain", 0.0f, 1.0f, 0.5f));
    }

    //==============================================================================
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        //buffer.applyGain (*gain);
    }

    void processBlock (AudioBuffer<double>& buffer, MidiBuffer&) override
    {
        //buffer.applyGain ((float) *gain);
    }

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                        { return true;   }

    //==============================================================================
    const String getName() const override                  { return "Gain PlugIn"; }
    bool acceptsMidi() const override                      { return false; }
    bool producesMidi() const override                     { return false; }
    double getTailLengthSeconds() const override           { return 0; }

    //==============================================================================
    int getNumPrograms() override                          { return 1; }
    int getCurrentProgram() override                       { return 0; }
    void setCurrentProgram (int) override                  {}
    const String getProgramName (int) override             { return "None"; }
    void changeProgramName (int, const String&) override   {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        //MemoryOutputStream (destData, true).writeFloat (*gain);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        //gain->setValueNotifyingHost (MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat());
    }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        const auto& mainInLayout  = layouts.getChannelSet (true,  0);
        const auto& mainOutLayout = layouts.getChannelSet (false, 0);

        return (mainInLayout == mainOutLayout && (! mainInLayout.isDisabled()));
    }

public:
    //==============================================================================
    AudioProcessorParameter* gain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainProcessor)
};



class OpenGLComponent: public juce::OpenGLAppComponent
{
public:
    OpenGLComponent ();
    virtual ~OpenGLComponent();

    virtual void initialise() override;
    virtual void render() override;
    virtual void shutdown() override;
    virtual void paint(Graphics& g) override;

    std::atomic<int> rColor;
};

class TestSlider : public juce::Slider
{
public:
    TestSlider(SliderStyle style, TextEntryBoxPosition textBoxPosition);
    virtual ~TestSlider() = default;
    virtual void paint(Graphics& g) override;
};


class Editor
    : public AudioProcessorEditor
    , juce::Timer
{
public:
    Editor (GainProcessor&);
    ~Editor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainProcessor& processor;
    TestSlider gainSlider1;
    TestSlider gainSlider2;
    TestSlider gainSlider3;
    TestSlider gainSlider4;
    OpenGLComponent openglComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};


OpenGLComponent::OpenGLComponent()
{
}

OpenGLComponent::~OpenGLComponent()
{
    shutdownOpenGL();
}

void OpenGLComponent::initialise()
{
}

void OpenGLComponent::render()
{
    juce::OpenGLHelpers::clear(juce::Colour(rColor, 0, 0));
}

void OpenGLComponent::shutdown()
{
}

void OpenGLComponent::paint(Graphics& g)
{
}

TestSlider::TestSlider(SliderStyle style, TextEntryBoxPosition textBoxPosition)
    : juce::Slider(style, textBoxPosition)
{
}

void TestSlider::paint(Graphics& g)
{
    //juce::Thread::sleep(5);
    Slider::paint(g);
}

//==============================================================================
Editor::Editor (GainProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    , gainSlider1(juce::Slider::SliderStyle::RotaryHorizontalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    , gainSlider2(juce::Slider::SliderStyle::RotaryHorizontalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    , gainSlider3(juce::Slider::SliderStyle::RotaryHorizontalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    , gainSlider4(juce::Slider::SliderStyle::RotaryHorizontalDrag, juce::Slider::TextEntryBoxPosition::TextBoxBelow)
    , openglComponent()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 700);

    addAndMakeVisible(gainSlider1);
    addAndMakeVisible(gainSlider2);
    addAndMakeVisible(gainSlider3);
    addAndMakeVisible(gainSlider4);
    addAndMakeVisible(openglComponent);
    startTimerHz(60);
}

Editor::~Editor()
{
}

//==============================================================================
void Editor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    gainSlider1.setRange(0., 1.);
    gainSlider2.setRange(0., 1.);
    gainSlider3.setRange(0., 1.);
    gainSlider4.setRange(0., 1.);
}

void Editor::resized()
{
    openglComponent.setTopLeftPosition(0, 0);
    openglComponent.setSize(getWidth(),  getHeight() / 2);
    gainSlider1.setCentrePosition(0, getHeight() / 2);
    gainSlider1.setSize(100, 100);

    gainSlider2.setCentrePosition(100, getHeight() / 2);
    gainSlider2.setSize(100, 100);

    gainSlider3.setCentrePosition(200, getHeight() / 2);
    gainSlider3.setSize(100, 100);

    gainSlider4.setCentrePosition(300, getHeight() / 2);
    gainSlider4.setSize(100, 100);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Editor::timerCallback()
{
    gainSlider1.setValue(processor.gain->getValue());
    gainSlider2.setValue(processor.gain->getValue());
    gainSlider3.setValue(processor.gain->getValue());
    gainSlider4.setValue(processor.gain->getValue());
    gainSlider1.repaint();
    gainSlider2.repaint();
    gainSlider3.repaint();
    gainSlider4.repaint();
}



AudioProcessorEditor* GainProcessor::createEditor() { return new Editor (*this); }
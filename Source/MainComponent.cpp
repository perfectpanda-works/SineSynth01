#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(50.0, 5000.0);
    frequencySlider.setSkewFactorFromMidPoint(500.0); // [4]
    frequencySlider.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
            updateAngleDelta();
    };

    setSize(600, 100);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto level = 0.125f;
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        leftBuffer[sample] = currentSample * level;
        rightBuffer[sample] = currentSample * level;
    }
}

void MainComponent::releaseResources(){}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    frequencySlider.setBounds(10, 10, getWidth() - 20, 20);
}

void MainComponent::updateAngleDelta()
{
    auto cyclesPerSample = frequencySlider.getValue() / currentSampleRate;
    //DBG("cyclesPerSample:" << cyclesPerSample);
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    //DBG("angleDelta:" << angleDelta);
}
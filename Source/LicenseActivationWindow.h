/*
  ==============================================================================

    LicenseActivationWindow.h
    Created: 10 Aug 2024 9:30:25am
    Author:  Zachary Pennington

  ==============================================================================
*/

#pragma once
#include <regex>
#include "PluginProcessor.h"
class PluginProcessor; // Forward declaration

class LicenseActivationWindow : public juce::Component
{
public:
    LicenseActivationWindow()
    {
        // Email field
        addAndMakeVisible(emailLabel);
        emailLabel.setText("Email:", juce::dontSendNotification);
        addAndMakeVisible(emailField);

        // Password field
        addAndMakeVisible(passwordLabel);
        passwordLabel.setText("Password:", juce::dontSendNotification);
        addAndMakeVisible(passwordField);
        passwordField.setPasswordCharacter('*');

        // License key field
        addAndMakeVisible(licenseKeyLabel);
        licenseKeyLabel.setText("License Key:", juce::dontSendNotification);
        addAndMakeVisible(licenseKeyField);

        // Activate button
        addAndMakeVisible(activateButton);
        activateButton.setButtonText("Activate");
        activateButton.onClick = [this] { activate(); };

        // Cancel button
        addAndMakeVisible(cancelButton);
        cancelButton.setButtonText("Cancel");
        cancelButton.onClick = [this] { cancel(); };
        
        // Demo button
        addAndMakeVisible(demoButton);
        demoButton.setButtonText("Trial (14-days)");
        demoButton.onClick = [this] { startDemo(); };

        setSize(400, 250);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        emailLabel.setBounds(area.removeFromTop(20));
        emailField.setBounds(area.removeFromTop(20));
        passwordLabel.setBounds(area.removeFromTop(20));
        passwordField.setBounds(area.removeFromTop(20));
        licenseKeyLabel.setBounds(area.removeFromTop(20));
        licenseKeyField.setBounds(area.removeFromTop(20));

        activateButton.setBounds(50, 175, 100, 25); // XYWH
        cancelButton.setBounds(250, 175, 100, 25); // XYWH
        demoButton.setBounds(150, 210, 100, 25); // XYWH
    }

private:
    PluginProcessor* processor;
    
    juce::Label emailLabel, passwordLabel, licenseKeyLabel;
    juce::TextEditor emailField, passwordField, licenseKeyField;
    juce::TextButton activateButton, cancelButton, demoButton;

    void activate()
    {
        auto email = emailField.getText();
        auto password = passwordField.getText();
        auto licenseKey = licenseKeyField.getText();
        auto hashedPass = hashPassword(password);
        
        // Validate and activate the license key
        if (!isValidEmail(email))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Invalid Email",
                                                   "Please enter a valid email address.");
            return;
        }
        
        if (!isValidPassword(password))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Invalid Password",
                                                   "Please enter a valid password.\nMust contain:\n- At least 8 characters long\n- At least one uppercase letter\n- At least one lowercase letter\n- At least one digit\n- At least one special character");
            return;
        }

        
        if (!isValidLicenseKey(licenseKey))
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Invalid License Key",
                                                   "Please enter a valid license key.");
            return;
        }
        
        
        juce::String postData = "email=" + email + "&password=" + hashedPass + "&license_key=" + licenseKey;
        juce::URL activationURL("http://localhost:3000/api/activate");
        activationURL = activationURL.withPOSTData(postData);
        juce::String response = activationURL.readEntireTextStream();
        std::cout << response << std::endl;

        // Parse the JSON response
        auto parsedResponse = juce::JSON::parse(response);

        if (parsedResponse.isObject())
        {
            auto* responseObject = parsedResponse.getDynamicObject();

            if (responseObject != nullptr)
            {
                auto success = responseObject->getProperty("success");

                if (success)
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon,
                                                           "Activation Success",
                                                           responseObject->getProperty("message").toString());
                    getParentComponent()->exitModalState(1);
                    
                }
                else
                {
                    std::cout << responseObject->getProperty("message").toString() << std::endl;
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                           "Activation Failed",
                                                           responseObject->getProperty("message").toString());
                }
            }
        }
        else
        {
            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                   "Error",
                                                   "Failed to get server response. Try again.");
        }

    }

    void cancel()
    {
        // TODO: Need to implement the isActivated() to prevent plugin from processing audio.
        getParentComponent()->exitModalState(0);
    }
    
    void startDemo()
    {
        
    }
    
    bool isValidEmail(const juce::String& email)
    {
        // Converts JUCE String to std::string
        std::string emailStr = email.toStdString();

        // Email validation pattern
        const std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

        return std::regex_match(emailStr, emailPattern);
    }
    
    bool isValidPassword(const juce::String& password)
    {
        std::string passwordStr = password.toStdString();

        // Password validation pattern
        const std::regex passwordPattern(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{8,})");

        return std::regex_match(passwordStr, passwordPattern);
    }
    
    juce::String hashPassword(const juce::String& password)
    {
        auto savedPassword = juce::SHA256(*password.toRawUTF8());

        auto hashedPassword = savedPassword.toHexString();

        return hashedPassword; // Return the hashed password as a hexadecimal string
    }
    
    bool isValidLicenseKey(const juce::String& key)
    {
        std::string licenseKeyStr = key.toStdString();
        
        // License key validation pattern
        const std::regex licenseKeyPattern(R"(^[a-zA-Z0-9]{4}-[a-zA-Z0-9]{4}-[a-zA-Z0-9]{4}-[a-zA-Z0-9]{4}$)");

        return std::regex_match(licenseKeyStr, licenseKeyPattern);
    }
    
};

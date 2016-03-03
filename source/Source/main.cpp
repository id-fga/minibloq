/*
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
*/
#include <wx/app.h>
#include <wx/stdpaths.h>
#include <wx/splash.h>
#include "MainFrame.h"

#define DEBUG 0 //Deshabilitar assertions de wxWidgets



//////////////////////////////////////////////////////////////////////
// Application
//////////////////////////////////////////////////////////////////////
class Minibloq : public wxApp
{
    private:
        int initialFrameX,
            initialFrameY,
            initialFrameWidth,
            initialFrameHeight;
        bool maximized;
        bool centered;
        wxString strBoard;

        void readConfig()
        {
            wxString fileName = MainFrame::getConfigFileName();
            wxXmlDocument xmlFile;
            if ( !xmlFile.Load(fileName, wxString("UTF-8")) )
                return;
            wxXmlNode *root = xmlFile.GetRoot();
            if (root == NULL)
                return;
            //Not really necessary:
            //if (root->GetName() != wxString("miniBloq"))
            //    return;

            bool mainFrameRead = false;
            bool hardRead = false;
            wxString tempName("");
            wxXmlNode *rootChild = root->GetChildren();
            while (rootChild)
            {
                tempName = rootChild->GetName();
                if (tempName == wxString("mainFrame"))
                {
                    wxXmlNode *child = rootChild->GetChildren();
                    while (child)
                    {
                        if (child->GetName() == "x")
                        {
                            wxString returnStringValue = child->GetNodeContent();
                            long returnNumericValue = 0;
                            if (returnStringValue.ToLong(&returnNumericValue))
                                initialFrameX = (int)returnNumericValue;
                        }
                        else if (child->GetName() == "y")
                        {
                            wxString returnStringValue = child->GetNodeContent();
                            long returnNumericValue = 0;
                            if (returnStringValue.ToLong(&returnNumericValue))
                                initialFrameY = (int)returnNumericValue;
                        }
                        else if (child->GetName() == "width")
                        {
                            wxString returnStringValue = child->GetNodeContent();
                            long returnNumericValue = 1280;
                            if (returnStringValue.ToLong(&returnNumericValue))
                                initialFrameWidth = (int)returnNumericValue;
                        }
                        else if (child->GetName() == "height")
                        {
                            wxString returnStringValue = child->GetNodeContent();
                            long returnNumericValue = 800;
                            if (returnStringValue.ToLong(&returnNumericValue))
                                initialFrameHeight = (int)returnNumericValue;
                        }
                        else if (child->GetName() == "maximized")
                        {
                            maximized = Bubble::string2bool(child->GetNodeContent());
                        }
                        else if (child->GetName() == "centered")
                        {
                            centered = Bubble::string2bool(child->GetNodeContent());
                        }

                        child = child->GetNext();
                    }
                    mainFrameRead = true; //Optimization.
                }
                else if (tempName == wxString("hard"))
                {
                    wxXmlNode *child = rootChild->GetChildren();
                    while (child)
                    {
                        if (child->GetName() == "board")
                        {
                            strBoard = child->GetNodeContent();
                        }
                        child = child->GetNext();
                    }
                    hardRead = true; //Optimization.
                }

                if (mainFrameRead && hardRead) //Optimization.
                    break;

                rootChild = rootChild->GetNext();
            }
        }

    protected:
        wxLocale locale;

	public:
		//##Constructor? It seems that right now the compiler uses the default constructor.

        bool OnInit()
        {

#if DEBUG
            wxLogWarning("MODO DEBUG");
#else
            wxSetAssertHandler(NULL);
#endif

            //i18n/l10n:
            //20120603: Change suggested by Juan Pizarro to solve the i18n bug in some Windows and Linux machines:
            //if ( !locale.Init(wxLANGUAGE_DEFAULT, wxLOCALE_CONV_ENCODING) )

#if !defined(linux)
            wxLogWarning("Esta version esta desarrollada exclusivamente para Linux");
            return true;
#endif


/*
#if defined (linux)
            wxLogNull logNo;
#endif
*/
            /*
            if ( !locale.Init(wxLANGUAGE_ENGLISH, wxLOCALE_CONV_ENCODING) )
                wxLogWarning(_("Error #1: This language is not supported by the system."));
            */

            /*
            wxString lanPath(wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) +
                             wxFileName::GetPathSeparator() + wxString("GUI") +
                             wxFileName::GetPathSeparator() + wxString("Texts") //##Unhardcode this in the future? I'm not sure...
                            );
            */


            //wxString lanPath = wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()) + "/../../GUI/Texts/";
            //wxLocale::AddCatalogLookupPathPrefix(lanPath);

	    wxString lanPath = "/usr/share";
	    locale.AddCatalogLookupPathPrefix(lanPath);


            //Por defecto debe levantar en Castellano
            //wxString initialCatalogName("es.mo");

	    wxString initialCatalogName = "Minibloq.mo";

            //wxString initialCatalogName("es.mo"); //##Debug...
            if (!locale.AddCatalog(initialCatalogName))
            {
                //##Future: See if future wxLocale implementations solve this problem:
                wxLogWarning(   _("Error #1: The installation path\n\n\"") + wxStandardPaths::Get().GetExecutablePath() +
                                _("\"\n\ncontains non-English chars.\n") +
                                _("Please try to install Minibloq on a location without this kind of chars. ") +
                                _("Otherwise, the program will run, but the translation system will not work properly.")
                            );
                //wxLogWarning(_("Error #2: Can't load ") + initialCatalogName);
                printf("NO ENCUENTRO EL CATALOGO\n");
            }

            MainFrame* frame = NULL;

            wxString caption = wxString(wxString("miniBloq ") + MINIBLOQ_VERSION);
            wxPoint framePosition = wxDefaultPosition;
            wxSize frameSize = wxDefaultSize;
            long style = wxDEFAULT_FRAME_STYLE;

            //Default values:
            initialFrameX = 0;
            initialFrameY = 0;
            initialFrameHeight = 600;
            initialFrameWidth = 800;
            maximized = true;
            centered = true;
            strBoard = wxString("");

            //Try to read the configuration file:
            readConfig();

            //Priorities:
            //  maximized has priority over all the other pos and size settings.
            //  centered has priority over x and y settings.
            if (maximized)
                style = style | wxMAXIMIZE;
            if ( (initialFrameWidth > 0) && (initialFrameHeight > 0) )
            {
                framePosition = wxPoint(initialFrameX, initialFrameY);
                frameSize = wxSize(initialFrameWidth, initialFrameHeight);
            }

            //TODO: Chequeo de errores
	    wxString execPath = wxStandardPaths::Get().GetExecutablePath().BeforeLast(wxFileName::GetPathSeparator()); 
	    wxString splashImg = execPath + "/../../GUI/Images/minibloqSplash.png";

            wxBitmap splashPng;
            splashPng.LoadFile(splashImg);
            wxSplashScreen* splash = new wxSplashScreen(splashPng,
                                                        wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
                                                        3000,
                                                        NULL,
                                                        -1,
                                                        wxDefaultPosition,
                                                        wxDefaultSize,
                                                        wxBORDER_SIMPLE|wxSTAY_ON_TOP);

            wxYield();


            frame = new MainFrame(  NULL,
                                    wxID_ANY,
                                    locale,
                                    lanPath,
                                    initialCatalogName,
                                    caption,
                                    strBoard,
                                    framePosition,
                                    frameSize,
                                    style
                                  );
            /*## Future: Full screen:
            wxFrame* frame = new MainFrame(NULL,
                                         wxID_ANY,
                                         wxString("Minibloq v1.0"),
                                         wxDefaultPosition,
                                         wxSize(1280, 800),
                                         wxCLIP_CHILDREN);
            */
            if (frame)
            {
                if (centered)
                    frame->Centre();
                frame->setCentered(centered);

                SetTopWindow(frame);
                frame->Show();
                if (maximized)
                {
                    frame->Maximize();
                }
                if (argc > 0)
                {
                    wxString fileName = argv[1];
                    if (wxFile::Exists(fileName))
                    {
                        frame->loadFileComponent(fileName);
                    }
                }
            }

            return true;
        }
};

wxDECLARE_APP(Minibloq);
wxIMPLEMENT_APP(Minibloq);

#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include <boost/filesystem.hpp>

#include <OgreFrameListener.h>

#include <openengine/ogre/renderer.hpp>
#include <openengine/bullet/physic.hpp>
#include <components/compiler/extensions.hpp>
#include <components/files/collections.hpp>

#include "mwworld/environment.hpp"
#include "mwworld/ptr.hpp"
#include <boost/timer.hpp>
#include <components/cfg/configurationmanager.hpp>

namespace Compiler
{
    class Context;
}

namespace MWScript
{
    class ScriptManager;
}

namespace MWSound
{
    class SoundManager;
}

namespace MWWorld
{
    class World;
}

namespace MWGui
{
    class WindowManager;
}

namespace OEngine
{
  namespace GUI
  {
    class MyGUIManager;
  }
}

namespace OMW
{
    /// \brief Main engine class, that brings together all the components of OpenMW
    class Engine : private Ogre::FrameListener
    {
            std::string mEncoding;
            boost::filesystem::path mDataDir;
            boost::filesystem::path mResDir;
            OEngine::Render::OgreRenderer mOgre;
            OEngine::Physic::PhysicEngine* mPhysicEngine;
            std::string mCellName;
            std::string mMaster;
            bool mShowFPS;
            bool mDebug;
            bool mVerboseScripts;
            bool mNewGame;
            bool mUseSound;
            bool mCompileAll;
            int total;

            MWWorld::Environment mEnvironment;
            MWScript::ScriptManager *mScriptManager;
            Compiler::Extensions mExtensions;
            Compiler::Context *mScriptContext;
            OEngine::GUI::MyGUIManager *mGuiManager;
            ESM::Region test;
            boost::timer timer;

            int focusFrameCounter;
            static const int focusUpdateFrame = 10;

            MWWorld::Ptr mIgnoreLocalPtr;

            Files::Collections mFileCollections;
            bool mFSStrict;

            // not implemented
            Engine (const Engine&);
            Engine& operator= (const Engine&);

            /// add resources directory
            /// \note This function works recursively.

            void addResourcesDirectory (const boost::filesystem::path& path);

            /// Load all BSA files in data directory.
            void loadBSA();

            void executeLocalScripts();

            virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);

            /// Process pending commands

        public:
            Engine(Cfg::ConfigurationManager& configurationManager);
            virtual ~Engine();

            /// Enable strict filesystem mode (do not fold case)
            ///
            /// \attention The strict mode must be specified before any path-related settings
            /// are given to the engine.
            void enableFSStrict(bool fsStrict);

            /// Set data dirs
            void setDataDirs(const Files::Collections::PathContainer& dataDirs);

            /// Set resource dir
            void setResourceDir(const boost::filesystem::path& parResDir);

            /// Set start cell name (only interiors for now)
            void setCell(const std::string& cellName);

            /// Set master file (esm)
            /// - If the given name does not have an extension, ".esm" is added automatically
            /// - Currently OpenMW only supports one master at the same time.
            void addMaster(const std::string& master);

            /// Enable fps counter
            void showFPS(bool showFps);

            /// Enable debug mode:
            /// - non-exclusive input
            void setDebugMode(bool debugMode);

            /// Enable or disable verbose script output
            void setScriptsVerbosity(bool scriptsVerbosity);

            /// Disable or enable all sounds
            void setSoundUsage(bool soundUsage);

            /// Start as a new game.
            void setNewGame(bool newGame);

            /// Initialise and enter main loop.
            void go();

            /// Activate the focussed object.
            void activate();

            /// Compile all scripts (excludign dialogue scripts) at startup?
            void setCompileAll (bool all);

            /// Font encoding
            void setEncoding(const std::string& encoding);

        private:
            std::string getOgreFilesDir(const std::string& ogreFile);

            Cfg::ConfigurationManager& mCfgMgr;
    };
}

#endif /* ENGINE_H */

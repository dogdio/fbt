/** @file SystemDescription.h
    @brief System description is here

    ### introductions
    This is simple test framework \n
    Loads user-implemented tests and the framework calls them in turn \n
    User code must implement the test framework's IF @emoji :bulb: \n

    ### Operation Overview
    -# Test framework initialization
    -# Parse command line parameters
       - Set configration
    -# Load shared library
       - Install User Instance (see @ref TestSample)
       - Also load any other required libraries here (see @ref TestSample4.cpp)
    -# Traverse Instance queue
       - Registration Tests
       - Execution Tests
    -# UnLoad shared library
    -# exit

    @note List of illustrations following (described manually).

    ### Sequence diagram
      -# @ref page11
      -# @ref page12
      -# @ref page13

    @cond SECTION_PRIVATE
      ### Block diagram
        -# @ref page01

      ### Data structure
        -# @ref page02
    @endcond
*/

/** @cond SECTION_PRIVATE
      @page page01 Modules Overview
      @dotfile TestMain.dot
      @see test::TestMain
      @see test::TestBase
    @endcond */

/** @cond SECTION_PRIVATE
      @page page02 Details of internal data

      ### Pickup important Data
      - @ref test::TestBase
        @dontinclude TestBase.h
          @skip private
          @until };

      - @ref TestBasePrivate.h
        @dontinclude TestBasePrivate.h
          @skip namespace
          @until };
          @until }

      - @ref TestBase.cpp
        @dontinclude TestBase.cpp
          @skip namespace {
          @until };

      ### Private data for Test interface class
      - test::TestBase::priv is only accessible from TestMain (friend of TestBase)
      - Can not accessible from client code
      - Friend class is completely hidden in TestMain.cpp

      @dotfile PrivData1.dot

      ### Data structure
      - Base->priv is static area (common to each instances)
        - Each Instance does not work at the same time
      - @b TestList will be create and destroy each instances
      - @b VecInstances have user instance which added by test::TestBase::AddBaseQueue()

      @dotfile PrivData2.dot

      ### Sequence focused on internal data
      @mscfile Queue.msc "Details of internal data access"
    @endcond */

/** @page page11 Init Sequence
    @mscfile Init.msc "init sequence"
    @see test::TestBase */

/** @page page12 Regist Sequence
    @mscfile Regist.msc "regist sequence"
    @see test::TestBase */

/** @page page13 Test Sequence
    @mscfile Test.msc "Test sequence"
    @see test::TestBase */


// SECTION_PRIVATE: only doxygen-priv.conf is true

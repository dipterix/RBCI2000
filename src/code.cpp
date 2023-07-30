#include "BCI2000Remote.h"
#include <string>
#include <vector>
#include <iostream>
#include <Rcpp.h>
using namespace Rcpp;


// [[Rcpp::export]]
SEXP BCIDemo( std::string& path ) {
  BCI2000Remote bci;

  // Assume that Operator executable resides in the same directory as this program.
  // size_t pos = path.find_last_of( "\\/" );
  // Start the Operator module, and connect
  bci.OperatorPath( path + "Operator" );
  if( !bci.Connect() ) {
    Rcpp::stop( bci.Result() );
  }
  // Startup modules
  const char* modules[] = { "SignalGenerator --LogMouse=1", "ARSignalProcessing", "CursorTask" };
  std::vector<std::string> vModules( &modules[0], &modules[0] + sizeof( modules ) / sizeof( *modules ) );
  if( !bci.StartupModules( vModules ) ) {
    Rcout << "Cannot start modules \n";
    Rcpp::stop( bci.Result() );
  }
  // Load a parameter file, and set subject information
  bci.LoadParametersRemote( "../parms/examples/CursorTask_SignalGenerator.prm" );
  bci.SubjectID( "SUB" );
  // Start a run
  if( !bci.Start() )
  {
    Rcout << bci.Result() << "\n";
    Rcpp::stop( bci.Result() );
  }
  // Print feedback signal
  std::string state;
  double value = 0;
  while( bci.GetSystemState( state ) && state == "Running" )
  {
    bci.GetControlSignal( 1, 1, value );
    Rcout << "Control signal: " << value << ", press Enter to proceed" << "\n";
    Rcpp::checkUserInterrupt();
  }

  bci.Stop();
  return R_NilValue;
}

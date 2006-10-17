#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <iostream>
#include <string>
#include <gui/acb_gui.h>
#include <mvc/acb_mvc.h>

#include "app_icon.h"
#include "tagger_model.h"
#include "tagger_view.h"
#include "text_item.h"
#include "text_model.h"

using namespace acb_gui;
using namespace acb_mvc;

int
main(int argc, char* argv[])
{
  Application a(argc, argv);
  Tagger_model tm;
  Tagger_view w(tm);

  //#if defined(USE_GTK)
  std::string about = "[v";
  //w.set_wm_close(acb_gui::Window::HIDE);
  w.set_wm_close(acb_gui::Window::IGNORE);
  //w.set_wm_close(acb_gui::Window::DESTROY);
  w.set_title("Untitled MVC");

  w.show();
  w.set_icon(arboratrix_xpm);

  about += PACKAGE_VERSION; about += "] -> "; about += __DATE__;
  about += " / "; about +=__TIME__;
  //w.quick_message_sb(about, 5000);

  //tm.add_sentence("Frase de prueba;con:palabras");

  w.show_statusbar_message(about, 7000);
  a.run();

  //std::cout << "Aplicacion de prueba.\n";
  return a.end();
}

void listAll(const MenuItem* m, string path) // two-parameter overload
{
  if (m == nullptr) return;
  string prefix;
  if (m->name() == "") {
    // at root
    prefix = "";
  } else {
    prefix = "/";
    path += m->name();
    cout << path << endl;
  }
  if (m->menuItems() != nullptr) {
    for (auto const &s : *(m->menuItems())) {
      listAll(s, path + prefix);
    }
  }
}
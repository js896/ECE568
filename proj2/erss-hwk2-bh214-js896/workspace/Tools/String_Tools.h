#ifndef ST_TOOLS
#define ST_TOOLS
#include <string>
#include <vector>
class String_Tools {
 public:
  // split a string according to delimiter
  static std::vector<std::string> split_str(const std::string & str,
                                            const std::string & delimiter);
  // load a string into a given vector of char
  static void load_into_vector(std::vector<char> & container, const std::string & str);
  static void load_chars_into_vector(std::vector<char> & container,
                                     char * chars,
                                     int size);
  static char * vector_to_chars(const std::vector<char> & container);

  static void copy_vector(const std::vector<char> & target, std::vector<char> & copy);

  static std::string get_nowtime();

  static struct tm rqs_to_tm(const std::string & str);
  static time_t rqs_to_time_t(const std::string & str);
  static int find_int_after(const std::string & str, const std::string & target);
};

#endif

typedef struct {
  size_t size;
  size_t count;
  char** strings;
} StringList;

StringList* init_string_list(size_t size);
void string_list_append(StringList* string_list, char* string);

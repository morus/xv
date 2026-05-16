/*
 * xvexif.c - add exif data to shown image comment
 */

#include "copyright.h"
#include "xv.h"

#ifdef HAVE_EXIF

#include <libexif/exif-content.h>

char* exifTitle = "\nExif Data\n================================================================\n\n";

static void exifAddSize(ExifContent* content, void* size) {
  char out[64];
  for (int i=0; i<content->count; i++) {
    ExifEntry* data = content->entries[i];
    int title_len = strlen(exif_tag_get_title(data->tag));
    exif_entry_get_value(data, out, 64);
    *(int*)size += ( title_len > 27 ? title_len : 27 ) + strlen(out) + 2;
  }
}

static void exifAddContent(ExifContent* content, void* comment) {
  char line[100];
  char out[64];
  for (int i=0; i<content->count; i++) {
    ExifEntry* data = content->entries[i];
    snprintf(line, 100, "%-27s %s\n",
             exif_tag_get_title(data->tag),
             exif_entry_get_value(data, out, 64));
    strcat((char*)comment, line);
  }
}

static int exifSize(ExifData* exifData) {
  int size = strlen(exifTitle) + 1;

  exif_data_foreach_content(exifData, exifAddSize, &size);

  return size;
}

char* ExifComment(char* comment) {
  ExifData* exifData = exif_data_new_from_data(picExifInfo, picExifInfoSize);
  int size = exifSize(exifData);

  if (comment) {
    size += strlen(comment);
  }
  char* fullComment = malloc(size);
  if (fullComment == NULL) {
    fprintf(stderr, "could not allocate memory for exif comment\n");
    return NULL;
  }

  strcpy(fullComment, comment ? comment : "");
  strcat(fullComment, exifTitle);
  exif_data_foreach_content(exifData, exifAddContent, fullComment);
  return fullComment;
}

#endif /* HAVE_EXIF */

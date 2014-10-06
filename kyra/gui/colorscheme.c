#include "colorscheme.h"



ColorScheme*
get_color_scheme ()
{
  static ColorScheme color_scheme;

  if(color_scheme.set==0)
  {
    color_scheme.set = 1;

    // editor colors
    color_scheme.editor_top_r = 255;
    color_scheme.editor_top_g = 247;
    color_scheme.editor_top_b = 220;
    color_scheme.editor_top_a = 255;

    color_scheme.editor_bot_r = 132;
    color_scheme.editor_bot_g = 124;
    color_scheme.editor_bot_b = 110;
    color_scheme.editor_bot_a = 225;

    color_scheme.editor_highlight_r = 239;
    color_scheme.editor_highlight_g = 166;
    color_scheme.editor_highlight_b = 61; 
    color_scheme.editor_highlight_a = 200;

    color_scheme.editor_scroll_r = 255;
    color_scheme.editor_scroll_g = 247;
    color_scheme.editor_scroll_b = 229; 
    color_scheme.editor_scroll_a = 255;

    color_scheme.editor_glass_r = 255;
    color_scheme.editor_glass_g = 255; 
    color_scheme.editor_glass_b = 255;

    color_scheme.editor_text_r = 0;
    color_scheme.editor_text_g = 0;
    color_scheme.editor_text_b = 0;

    color_scheme.editor_border_r = 104;
    color_scheme.editor_border_g = 104;
    color_scheme.editor_border_b = 104;

    color_scheme.editor_hint_r = 255;
    color_scheme.editor_hint_g = 255;
    color_scheme.editor_hint_b = 225;

    color_scheme.editor_shadow_r = 0;
    color_scheme.editor_shadow_g = 0;
    color_scheme.editor_shadow_b = 0;

    // button colors
    color_scheme.button_text_r = 0;
    color_scheme.button_text_g = 0;
    color_scheme.button_text_b = 0;

    color_scheme.button_border_r = 104;
    color_scheme.button_border_g = 104;
    color_scheme.button_border_b = 104;

    color_scheme.edit_border_r = 104;
    color_scheme.edit_border_g = 104;
    color_scheme.edit_border_b = 104;

    // script other
    color_scheme.primary_r = 170;
    color_scheme.primary_g = 170;
    color_scheme.primary_b = 170;

    color_scheme.secondary_r = 255;
    color_scheme.secondary_g = 255;
    color_scheme.secondary_b = 255;

    color_scheme.cursor_r = 255;
    color_scheme.cursor_g = 255;
    color_scheme.cursor_b = 255;


    // script text
    color_scheme.operator_r = 0;
    color_scheme.operator_g = 90;
    color_scheme.operator_b = 12;

    color_scheme.keyword_r = 64;
    color_scheme.keyword_g = 76;
    color_scheme.keyword_b = 108;

    color_scheme.preprocess_r = 64;
    color_scheme.preprocess_g = 76;
    color_scheme.preprocess_b = 108;

    color_scheme.normal_r = 255;
    color_scheme.normal_g = 255;
    color_scheme.normal_b = 255;

    color_scheme.type_r = 0;
    color_scheme.type_g = 255;
    color_scheme.type_b = 100;

    color_scheme.function_r = 255;
    color_scheme.function_g = 255;
    color_scheme.function_b = 0;

    color_scheme.comment_r = 165;
    color_scheme.comment_g = 255;
    color_scheme.comment_b = 211;

    color_scheme.literal_r = 168;
    color_scheme.literal_g = 0;
    color_scheme.literal_b = 0;

    color_scheme.default_r = 0;
    color_scheme.default_g = 255;
    color_scheme.default_b = 255;

    // defaults
    color_scheme.default_axis_r = 200;
    color_scheme.default_axis_g = 200;
    color_scheme.default_axis_b = 200;

    color_scheme.default_res_r = 150;
    color_scheme.default_res_g = 150;
    color_scheme.default_res_b = 150;

    color_scheme.default_grid_r = 100;
    color_scheme.default_grid_g = 100;
    color_scheme.default_grid_b = 100;

    color_scheme.default_bg_r = 0;
    color_scheme.default_bg_g = 0;
    color_scheme.default_bg_b = 0;

    color_scheme.path_r = 255;
    color_scheme.path_g = 255;
    color_scheme.path_b = 255;
  }

  return &color_scheme;
}


void
read_color_scheme (char* filename)
{
  FILE* file = fopen(filename, "r");
  char line[256];
  int size = 256;
  ColorScheme * cs = get_color_scheme();

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_top_r));
  fscanf(file, "%d\n", &(cs->editor_top_g));
  fscanf(file, "%d\n", &(cs->editor_top_b));
  fscanf(file, "%d\n\n", &(cs->editor_top_a));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_bot_r));
  fscanf(file, "%d\n", &(cs->editor_bot_g));
  fscanf(file, "%d\n", &(cs->editor_bot_b));
  fscanf(file, "%d\n\n", &(cs->editor_bot_a));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_highlight_r));
  fscanf(file, "%d\n", &(cs->editor_highlight_g));
  fscanf(file, "%d\n", &(cs->editor_highlight_b));
  fscanf(file, "%d\n\n", &(cs->editor_highlight_a));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_scroll_r));
  fscanf(file, "%d\n", &(cs->editor_scroll_g));
  fscanf(file, "%d\n", &(cs->editor_scroll_b));
  fscanf(file, "%d\n\n", &(cs->editor_scroll_a));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_glass_r));
  fscanf(file, "%d\n", &(cs->editor_glass_g));
  fscanf(file, "%d\n\n", &(cs->editor_glass_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_text_r));
  fscanf(file, "%d\n", &(cs->editor_text_g));
  fscanf(file, "%d\n\n", &(cs->editor_text_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_border_r));
  fscanf(file, "%d\n", &(cs->editor_border_g));
  fscanf(file, "%d\n\n", &(cs->editor_border_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_hint_r));
  fscanf(file, "%d\n", &(cs->editor_hint_g));
  fscanf(file, "%d\n\n", &(cs->editor_hint_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->editor_shadow_r));
  fscanf(file, "%d\n", &(cs->editor_shadow_g));
  fscanf(file, "%d\n\n", &(cs->editor_shadow_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->button_text_r));
  fscanf(file, "%d\n", &(cs->button_text_g));
  fscanf(file, "%d\n\n", &(cs->button_text_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->button_border_r));
  fscanf(file, "%d\n", &(cs->button_border_g));
  fscanf(file, "%d\n\n", &(cs->button_border_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->edit_border_r));
  fscanf(file, "%d\n", &(cs->edit_border_g));
  fscanf(file, "%d\n\n", &(cs->edit_border_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->primary_r));
  fscanf(file, "%d\n", &(cs->primary_g));
  fscanf(file, "%d\n\n", &(cs->primary_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->secondary_r));
  fscanf(file, "%d\n", &(cs->secondary_g));
  fscanf(file, "%d\n\n", &(cs->secondary_b));
  
  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->cursor_r));
  fscanf(file, "%d\n", &(cs->cursor_g));
  fscanf(file, "%d\n\n", &(cs->cursor_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->operator_r));
  fscanf(file, "%d\n", &(cs->operator_g));
  fscanf(file, "%d\n\n", &(cs->operator_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->keyword_r));
  fscanf(file, "%d\n", &(cs->keyword_g));
  fscanf(file, "%d\n\n", &(cs->keyword_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->preprocess_r));
  fscanf(file, "%d\n", &(cs->preprocess_g));
  fscanf(file, "%d\n\n", &(cs->preprocess_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->normal_r));
  fscanf(file, "%d\n", &(cs->normal_g));
  fscanf(file, "%d\n\n", &(cs->normal_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->type_r));
  fscanf(file, "%d\n", &(cs->type_g));
  fscanf(file, "%d\n\n", &(cs->type_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->function_r));
  fscanf(file, "%d\n", &(cs->function_g));
  fscanf(file, "%d\n\n", &(cs->function_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->comment_r));
  fscanf(file, "%d\n", &(cs->comment_g));
  fscanf(file, "%d\n\n", &(cs->comment_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->literal_r));
  fscanf(file, "%d\n", &(cs->literal_g));
  fscanf(file, "%d\n\n", &(cs->literal_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->default_r));
  fscanf(file, "%d\n", &(cs->default_g));
  fscanf(file, "%d\n", &(cs->default_b));

  // defaults
  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->default_axis_r));
  fscanf(file, "%d\n", &(cs->default_axis_g));
  fscanf(file, "%d\n", &(cs->default_axis_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->default_res_r));
  fscanf(file, "%d\n", &(cs->default_res_g));
  fscanf(file, "%d\n", &(cs->default_res_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->default_grid_r));
  fscanf(file, "%d\n", &(cs->default_grid_g));
  fscanf(file, "%d\n", &(cs->default_grid_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->default_bg_r));
  fscanf(file, "%d\n", &(cs->default_bg_g));
  fscanf(file, "%d\n", &(cs->default_bg_b));

  fgets(line, size, file);
  fscanf(file, "%d\n", &(cs->path_r));
  fscanf(file, "%d\n", &(cs->path_g));
  fscanf(file, "%d\n", &(cs->path_b));

  fclose(file);
}

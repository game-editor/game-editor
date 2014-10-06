#ifndef STAND_ALONE_GAME

#ifndef __COLORSCHEME_H__
#define __COLORSCHEME_H__

#include "stdio.h"

typedef struct
{
  // editor colors
  int editor_top_r, editor_top_g, editor_top_b, editor_top_a;
  int editor_bot_r, editor_bot_g, editor_bot_b, editor_bot_a;
  int editor_highlight_r, editor_highlight_g, editor_highlight_b, editor_highlight_a;
  int editor_scroll_r, editor_scroll_g, editor_scroll_b, editor_scroll_a;
  int editor_glass_r, editor_glass_g, editor_glass_b;

  int editor_text_r, editor_text_g, editor_text_b;
  int editor_border_r, editor_border_g, editor_border_b;
  int editor_hint_r, editor_hint_g, editor_hint_b;
  int editor_shadow_r, editor_shadow_g, editor_shadow_b;

  // button colors
  int button_text_r, button_text_g, button_text_b;
  int button_border_r, button_border_g, button_border_b;
  int edit_border_r, edit_border_g, edit_border_b;

  // script other
  int primary_r, primary_g, primary_b;
  int secondary_r, secondary_g, secondary_b;
  int cursor_r, cursor_g, cursor_b;

  // script text
  int operator_r, operator_g, operator_b;
  int keyword_r, keyword_g, keyword_b;
  int preprocess_r, preprocess_g, preprocess_b;
  int normal_r, normal_g, normal_b;
  int type_r, type_g, type_b;
  int function_r, function_g, function_b;
  int comment_r, comment_g, comment_b;
  int literal_r, literal_g, literal_b;
  int default_r, default_g, default_b;

  // defaults
  int default_axis_r, default_axis_g, default_axis_b;
  int default_res_r, default_res_g, default_res_b;
  int default_grid_r, default_grid_g, default_grid_b;
  int default_bg_r, default_bg_g, default_bg_b;
  int path_r, path_g, path_b;

  int set;

}ColorScheme;


void read_color_scheme (char* filename);
ColorScheme* get_color_scheme ();
#endif // __COLORSCHEME_H__


#endif // STAND_ALONE_GAME

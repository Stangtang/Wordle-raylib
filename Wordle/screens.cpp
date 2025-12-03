#include "screens.h"

void handle_screens(std::stack<Screen>& screen_stack)
{
	switch (screen_stack.top())
	{
	case start: start_screen(screen_stack); break;
	case wordle: wordle_screen(screen_stack); break;
	}
}

Color get_color_of_state(Letter_State state)
{
	switch (state)
	{
	case Letter_State::Gray: return GRAY;
	case Letter_State::Yellow: return { 230, 210, 37, 255 };
	case Letter_State::Green: return { 75, 217, 65, 255 };
	case Letter_State::Unchecked: return LIGHTGRAY;
	}
}

Fading_Text invalid_input = {
.duration = 0.75f,
.timeLeft = 0.0f,
.active = false
};

void wordle_screen(std::stack<Screen>& screen_stack)
{
	const int screen_width = 600;
	const int screen_height = 880;
	SetWindowSize(screen_width, screen_height);

	// draw guess squares

	const int guess_square_length = 70;
	const int first_square_offset = 20;
	const int gap_between_squares = 7;
	const double square_line_thickness = 3.0;

	for (int i = -2; i <= 2; i++) {
		for (int j = 0; j <= 5; j++) {
			DrawRectangleLinesEx({ float((screen_width - guess_square_length) / 2 + i * gap_between_squares + i * guess_square_length),
								   float(first_square_offset + j * gap_between_squares + j * guess_square_length),
								   guess_square_length, guess_square_length }, square_line_thickness, BLACK);
		}
	}

	// draw keyboard and get click input from keyboard

	const int gap_between_squares_and_letters = 60;
	const int letter_box_height = 60;
	const int gap_between_letter_boxes = 6;
	const int rows_1_and_3_padding_from_edge = 10;
	const int row_2_padding_from_edge = 20;
	const double letter_box_line_thickness = 2.0;

	const int letters_in_row_1 = 10;
	const int row_1_letter_box_width = (screen_width - 2 * rows_1_and_3_padding_from_edge - (letters_in_row_1 - 1) * gap_between_letter_boxes) / letters_in_row_1;

	for (int i = 0; i < letters_in_row_1; i++) {
		DrawRectangleLinesEx({ float(rows_1_and_3_padding_from_edge + i * row_1_letter_box_width + i * gap_between_letter_boxes),
							   first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters,
							   row_1_letter_box_width, letter_box_height }, letter_box_line_thickness, BLACK);
	}

	const int letters_in_row_2 = 9;
	const int row_2_letter_box_width = (screen_width - 2 * row_2_padding_from_edge - (letters_in_row_2 - 1) * gap_between_letter_boxes) / letters_in_row_2;

	for (int i = 0; i < letters_in_row_2; i++) {
		DrawRectangleLinesEx({ float(row_2_padding_from_edge + i * row_2_letter_box_width + i * gap_between_letter_boxes),
							   first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_height + gap_between_letter_boxes,
							   row_2_letter_box_width, letter_box_height }, letter_box_line_thickness, BLACK);
	}

	const int letters_in_row_3 = 7 + 2; // + ent, bksp
	const int row_3_letter_box_width = (screen_width - 2 * rows_1_and_3_padding_from_edge - (letters_in_row_3 - 1) * gap_between_letter_boxes) / letters_in_row_3;

	for (int i = 0; i < letters_in_row_3; i++) {
		DrawRectangleLinesEx({ float(rows_1_and_3_padding_from_edge + i * row_3_letter_box_width + i * gap_between_letter_boxes),
							   first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + 2 * letter_box_height + 2 * gap_between_letter_boxes,
							   row_3_letter_box_width, letter_box_height }, letter_box_line_thickness, BLACK);
	}

	const int letters_font_size = 36;
	const int enter_and_backspace_font_size = 20;

	int clicked_button = KEY_NULL;

	const std::string row_1_letters = "QWERTYUIOP";
	for (int i = 0; i < row_1_letters.length(); i++) {
		DrawRectangle(rows_1_and_3_padding_from_edge + letter_box_line_thickness + i * row_1_letter_box_width + i * gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness,
			row_1_letter_box_width - 2 * letter_box_line_thickness, letter_box_height - 2 * letter_box_line_thickness,
			get_color_of_state(wordle_letter_state_map[row_1_letters[i]]));

		const std::string s(1, row_1_letters[i]);

		DrawText(s.c_str(),
			rows_1_and_3_padding_from_edge + (row_1_letter_box_width - MeasureText(s.c_str(), letters_font_size)) / 2 + i * row_1_letter_box_width + i * gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + (letter_box_height - letters_font_size) / 2,
			letters_font_size, BLACK);

		Rectangle letter_button_area = { float(rows_1_and_3_padding_from_edge + i * row_1_letter_box_width + i * gap_between_letter_boxes),
										 first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters,
										 row_1_letter_box_width, letter_box_height };

		Vector2 mouse_position = GetMousePosition();
		if (CheckCollisionPointRec(mouse_position, letter_button_area) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "letter clicked: " << row_1_letters[i] << '\n';
			clicked_button = row_1_letters[i];
		}
	}

	const std::string row_2_letters = "ASDFGHJKL";
	for (int i = 0; i < row_2_letters.length(); i++) {
		DrawRectangle(row_2_padding_from_edge + letter_box_line_thickness + i * row_2_letter_box_width + i * gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + letter_box_height + gap_between_letter_boxes,
			row_2_letter_box_width - 2 * letter_box_line_thickness, letter_box_height - 2 * letter_box_line_thickness,
			get_color_of_state(wordle_letter_state_map[row_2_letters[i]]));

		const std::string s(1, row_2_letters[i]);

		DrawText(s.c_str(),
			row_2_padding_from_edge + (row_2_letter_box_width - MeasureText(s.c_str(), letters_font_size)) / 2 + i * row_2_letter_box_width + i * gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + (letter_box_height - letters_font_size) / 2 + letter_box_height + gap_between_letter_boxes,
			letters_font_size, BLACK);

		Rectangle letter_button_area = { float(row_2_padding_from_edge + i * row_2_letter_box_width + i * gap_between_letter_boxes),
										 first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_height + gap_between_letter_boxes,
										 row_2_letter_box_width, letter_box_height };

		Vector2 mouse_position = GetMousePosition();
		if (CheckCollisionPointRec(mouse_position, letter_button_area) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "letter clicked: " << row_2_letters[i] << '\n';
			clicked_button = row_2_letters[i];
		}
	}

	const std::string row_3_letters = "ZXCVBNM";
	for (int i = 0; i < row_3_letters.length(); i++) {
		DrawRectangle(rows_1_and_3_padding_from_edge + letter_box_line_thickness + i * row_3_letter_box_width + i * gap_between_letter_boxes + row_3_letter_box_width + gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 2 * letter_box_height + 2 * gap_between_letter_boxes,
			row_3_letter_box_width - 2 * letter_box_line_thickness, letter_box_height - 2 * letter_box_line_thickness,
			get_color_of_state(wordle_letter_state_map[row_3_letters[i]]));

		const std::string s(1, row_3_letters[i]);

		DrawText(s.c_str(),
			rows_1_and_3_padding_from_edge + (row_3_letter_box_width - MeasureText(s.c_str(), letters_font_size)) / 2 + i * row_3_letter_box_width + i * gap_between_letter_boxes + row_3_letter_box_width + gap_between_letter_boxes,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + (letter_box_height - letters_font_size) / 2 + 2 * letter_box_height + 2 * gap_between_letter_boxes,
			letters_font_size, BLACK);

		Rectangle letter_button_area = { float(rows_1_and_3_padding_from_edge + i * row_3_letter_box_width + i * gap_between_letter_boxes + row_3_letter_box_width + gap_between_letter_boxes),
										 first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + 2 * letter_box_height + 2 * gap_between_letter_boxes,
										 row_3_letter_box_width, letter_box_height };
		
		Vector2 mouse_position = GetMousePosition();
		if (CheckCollisionPointRec(mouse_position, letter_button_area) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "letter clicked: " << row_3_letters[i] << '\n';
			clicked_button = row_3_letters[i];
		}
	}

	Vector2 mouse_position = GetMousePosition();

	const char* enter_box_text = "Entr";
	DrawRectangle(rows_1_and_3_padding_from_edge + letter_box_line_thickness + 0 * row_3_letter_box_width + 0 * gap_between_letter_boxes,
		first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 2 * letter_box_height + 2 * gap_between_letter_boxes,
		row_3_letter_box_width - 2 * letter_box_line_thickness, letter_box_height - 2 * letter_box_line_thickness, get_color_of_state(Letter_State::Unchecked));
	DrawText(enter_box_text,
		rows_1_and_3_padding_from_edge + (row_3_letter_box_width - MeasureText(enter_box_text, enter_and_backspace_font_size)) / 2 + 0 * row_3_letter_box_width + 0 * gap_between_letter_boxes,
		first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + (letter_box_height - enter_and_backspace_font_size) / 2 + 2 * letter_box_height + 2 * gap_between_letter_boxes,
		enter_and_backspace_font_size, BLACK);
	Rectangle enter_button_area = { rows_1_and_3_padding_from_edge,
									first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + 2 * letter_box_height + 2 * gap_between_letter_boxes,
									row_3_letter_box_width, letter_box_height };
	if (CheckCollisionPointRec(mouse_position, enter_button_area) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		std::cout << "button clicked: " << "Enter" << '\n';
		clicked_button = KEY_ENTER;
	}

	const char* backspace_box_text = "Bksp";
	DrawRectangle(rows_1_and_3_padding_from_edge + letter_box_line_thickness + 8 * row_3_letter_box_width + 8 * gap_between_letter_boxes,
		first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 2 * letter_box_height + 2 * gap_between_letter_boxes,
		row_3_letter_box_width - 2 * letter_box_line_thickness, letter_box_height - 2 * letter_box_line_thickness, get_color_of_state(Letter_State::Unchecked));
	DrawText(backspace_box_text,
		rows_1_and_3_padding_from_edge + (row_3_letter_box_width - MeasureText(backspace_box_text, enter_and_backspace_font_size)) / 2 + 8 * row_3_letter_box_width + 8 * gap_between_letter_boxes,
		first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + (letter_box_height - enter_and_backspace_font_size) / 2 + 2 * letter_box_height + 2 * gap_between_letter_boxes,
		enter_and_backspace_font_size, BLACK);
	Rectangle backspace_button_area = { rows_1_and_3_padding_from_edge + row_3_letter_box_width + gap_between_letter_boxes + row_3_letters.length() * row_3_letter_box_width + row_3_letters.length() * gap_between_letter_boxes,
										first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + 2 * letter_box_height + 2 * gap_between_letter_boxes,
										row_3_letter_box_width, letter_box_height };
	if (CheckCollisionPointRec(mouse_position, backspace_button_area) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		std::cout << "button clicked: " << "Backspace" << '\n';
		clicked_button = KEY_BACKSPACE;
	}

	// handle input and invalid input text

	bool is_input_invalid = !handle_input(clicked_button);

	const int bottom_text_font_size = 70;
	const int space_before_bottom_text = 15;

	const char* invalid_input_text = "INVALID INPUT";

	float dt = GetFrameTime();

	if (is_input_invalid) {
		invalid_input.timeLeft = invalid_input.duration;
		invalid_input.active = true;
	}

	if (invalid_input.active) {
		invalid_input.timeLeft -= dt;
		if (invalid_input.timeLeft <= 0) {
			invalid_input.active = false;
		}

		float alpha = invalid_input.timeLeft / invalid_input.duration;
		Color color_after_fade = Fade(RED, alpha);
		DrawText(invalid_input_text, 
			(screen_width - MeasureText(invalid_input_text, bottom_text_font_size)) / 2, 
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 3 * letter_box_height + 3 * gap_between_letter_boxes + space_before_bottom_text,
			bottom_text_font_size, color_after_fade);
	}

	// draw guesses

	const int guess_and_input_font_size = 48;

	for (int i = 0; i < guesses_used.size(); i++) {
		for (int j = 0; j <= 4; j++) {
			DrawRectangle((screen_width - guess_square_length) / 2 + (j - 2) * gap_between_squares + (j - 2) * guess_square_length + square_line_thickness,
				first_square_offset + i * gap_between_squares + i * guess_square_length + square_line_thickness,
				guess_square_length - 2 * square_line_thickness, guess_square_length - 2 * square_line_thickness,
				get_color_of_state(wordle_guess_number_letter_state_map[i + 1][j]));
		}

		std::string guess = guesses_used[i];

		for (int j = 0; j <= 4; j++) {
			char guess_char[2] = { guess[j], '\0' };
			const char* guess_char_pointer = guess_char;
			DrawText(guess_char_pointer,
				(screen_width - MeasureText(guess_char_pointer, guess_and_input_font_size)) / 2 + (j - 2) * gap_between_squares + (j - 2) * guess_square_length,
				first_square_offset + i * gap_between_squares + i * guess_square_length + (guess_square_length - guess_and_input_font_size) / 2,
				guess_and_input_font_size, BLACK);
		}
	}

	// draw current input

	for (int i = 0; i <= 4; i++) {
		if (input[i] == '\n') continue;

		char input_char[2] = { input[i], '\0' };
		const char* input_char_pointer = input_char;
		DrawText(input_char_pointer,
			(screen_width - MeasureText(input_char_pointer, guess_and_input_font_size)) / 2 + (i - 2) * gap_between_squares + (i - 2) * guess_square_length,
			first_square_offset + guesses_used.size() * gap_between_squares + guesses_used.size() * guess_square_length + (guess_square_length - guess_and_input_font_size) / 2,
			guess_and_input_font_size, BLACK);
	}

	// draw ending text

	const char* exit_text = "Press Space to Return";
	const int exit_text_font_size = 30;

	if (is_wordle_word_guessed) {
		const char* win_text = "YOU WIN!!!";
		DrawText(win_text,
			(screen_width - MeasureText(win_text, bottom_text_font_size)) / 2,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 3 * letter_box_height + 3 * gap_between_letter_boxes + space_before_bottom_text,
			bottom_text_font_size, ORANGE);

		DrawText(exit_text,
			(screen_width - MeasureText(exit_text, exit_text_font_size)) / 2,
			screen_height - 45,
			exit_text_font_size, BLACK);

		if (IsKeyPressed(KEY_SPACE)) {
			screen_stack.pop();
		}

		return;
	}

	if (guesses_used.size() == 6) {
		std::string lose_text = "The word was: " + wordle_word;
		const int lose_text_font_size = 50;
		DrawText(lose_text.c_str(),
			(screen_width - MeasureText(lose_text.c_str(), lose_text_font_size)) / 2,
			first_square_offset + 6 * gap_between_squares + 6 * guess_square_length + gap_between_squares_and_letters + letter_box_line_thickness + 3 * letter_box_height + 3 * gap_between_letter_boxes + space_before_bottom_text + (bottom_text_font_size - lose_text_font_size) / 2,
			lose_text_font_size, BLUE);

		DrawText(exit_text,
			(screen_width - MeasureText(exit_text, exit_text_font_size)) / 2,
			screen_height - 45,
			exit_text_font_size, BLACK);

		if (IsKeyPressed(KEY_SPACE)) {
			screen_stack.pop();
		}

		return;
	}
}

void start_screen(std::stack<Screen>& screen_stack)
{
	const int screen_width = 600;
	const int screen_height = 600;
	SetWindowSize(screen_width, screen_height);

	const int button_width = 375;
	const int button_height = 100;
	const int first_button_offset = 50;
	const int gap_between_buttons = 30;
	const double line_thickness = 5.0;
	const int mouse_hover_expand = 5;
	int button_1_expand = 0;
	int button_2_expand = 0;
	int button_3_expand = 0;

	const char* button_1_text = "Wordle";
	const char* button_2_text = "Duordle";
	const char* button_3_text = "Quordle";
	const int font_size = 50;
	const int button_1_text_width = MeasureText(button_1_text, font_size);
	const int button_2_text_width = MeasureText(button_2_text, font_size);
	const int button_3_text_width = MeasureText(button_3_text, font_size);

	Rectangle button_1_area = { (screen_width - button_width) / 2, first_button_offset + 0 * button_height + 0 * gap_between_buttons, button_width, button_height };
	Rectangle button_2_area = { (screen_width - button_width) / 2, first_button_offset + 1 * button_height + 1 * gap_between_buttons, button_width, button_height };
	Rectangle button_3_area = { (screen_width - button_width) / 2, first_button_offset + 2 * button_height + 2 * gap_between_buttons, button_width, button_height };

	Vector2 mouse_position = GetMousePosition();
	if (CheckCollisionPointRec(mouse_position, button_1_area)) {
		button_1_expand += mouse_hover_expand;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "Wordle button clicked.\n";
			screen_stack.push(wordle);
			initialize_wordle_game(input);
		}
	}
	else if (CheckCollisionPointRec(mouse_position, button_2_area)) {
		button_2_expand += mouse_hover_expand;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "Duordle button clicked.\n";
			screen_stack.push(duordle);
		}
	}
	else if (CheckCollisionPointRec(mouse_position, button_3_area)) {
		button_3_expand += mouse_hover_expand;
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			std::cout << "Quordle button clicked.\n";
			screen_stack.push(quordle);
		}
	}

	DrawRectangleLinesEx({ float((screen_width - button_width) / 2 - button_1_expand),
						   float(first_button_offset + 0 * button_height + 0 * gap_between_buttons - button_1_expand),
						   float(button_width + 2 * button_1_expand), float(button_height + 2 * button_1_expand) }, line_thickness, BLACK);
	DrawRectangleLinesEx({ float((screen_width - button_width) / 2 - button_2_expand),
						   float(first_button_offset + 1 * button_height + 1 * gap_between_buttons - button_2_expand),
						   float(button_width + 2 * button_2_expand), float(button_height + 2 * button_2_expand) }, line_thickness, BLACK);
	DrawRectangleLinesEx({ float((screen_width - button_width) / 2 - button_3_expand),
						   float(first_button_offset + 2 * button_height + 2 * gap_between_buttons - button_3_expand),
						   float(button_width + 2 * button_3_expand), float(button_height + 2 * button_3_expand) }, line_thickness, BLACK);

	DrawText(button_1_text, (screen_width - button_1_text_width) / 2, first_button_offset + (button_height - font_size) / 2 + 0 * button_height + 0 * gap_between_buttons, font_size, BLACK);
	DrawText(button_2_text, (screen_width - button_2_text_width) / 2, first_button_offset + (button_height - font_size) / 2 + 1 * button_height + 1 * gap_between_buttons, font_size, BLACK);
	DrawText(button_3_text, (screen_width - button_3_text_width) / 2, first_button_offset + (button_height - font_size) / 2 + 2 * button_height + 2 * gap_between_buttons, font_size, BLACK);

	DrawText("Duordle and Quordle not implemented yet", 10, 500, 25, BLUE);
}
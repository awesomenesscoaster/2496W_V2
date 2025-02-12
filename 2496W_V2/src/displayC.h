

#include "main.h"
#include "robot.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_gauge.h"

#include "images/field_img.h"

#define num_motors 8 
#define circle_count 20

using namespace pros;

namespace disp{
    // setting up shapes for auton selector graphics
    static lv_obj_t *stat_labels[5]; // To hold labels for motor stats
    static int current_motor_index = 0;
    static lv_obj_t *motor_name_label = NULL; // Global variable for the motor name label


    static const lv_point_t blue_left_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t blue_ring_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t blue_safe_awp_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t red_right_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t red_ring_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t red_safe_awp_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t blue_elims_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t red_elims_rush_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t blue_elims_right_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t red_elims_left_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t prog_skills_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t driver_skills_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t regression_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t testing_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
    static const lv_point_t none_points[] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};

    lv_obj_t *circles_anim[circle_count];
    int current_circle = 0; 

    lv_obj_t* circles[6];
    lv_obj_t* square;
    lv_obj_t* robot_square;

    lv_obj_t* line_arrow;
    lv_obj_t* end_circle;



    const lv_img_dsc_t field_img = {
    {
        LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
        0,
        0,
        240,
        240,
    },
    56700 * LV_COLOR_SIZE / 8,
    field_img_map,
    };

   
    //LV_IMG_DECLARE(field);
    // Arrays to store widget references
    lv_obj_t* motor_lmeters[num_motors];
    lv_obj_t* motor_temp_labels[num_motors];

    static const char * auton_arr[] = {
        "Blue Left Rush",
        "Blue Ring Rush",
        "Blue Safe AWP",
        "Red Right Rush",
        "Red Ring Rush",
        "Red Safe AWP",
        "Blue Elims Rush",
        "Red Elims Rush",
        "Blue Elims Right",
        "Red Elims Left",
        "Prog Skills",
        "Driver Skills",
        "Regression",
        "Testing",
        "None",

    };
    static const char * auton_desc_vec[] = {
        
        "Goal Rush for \nmiddle goal (2) and \nside goal (1) and \nalliance stake (1) \nand touches bar \n\nFULL AWP",
        "Goes for center \ntwo rings and \nscores on side \ngoal (3) and adds (2) from corner \n+ alliance stake (1) \n+ touches bar \nFULL AWP",
        "Clamp side goal \nand scores rings \n(small stack, corner, \nand preload (3)) and \nalliance stake (1) \nand touches bar \nHALF AWP",
        "Goal Rush for \nmiddle goal (2) and \nside goal (1) and \nalliance stake (1) \nand touches bar \n\nFULL AWP",
        "Goes for center \ntwo rings and \nscores on side \ngoal (3) and adds (2) from corner \n+ alliance stake (1) \n+ touches bar \nFULL AWP",
        "Clamp side goal \nand scores rings \n(small stack, corner, \nand preload (3)) and \nalliance stake (1) \nand touches bar \nHALF AWP",
        "Goal Rush for \nmiddle goal (2) and \nside goal (1) and \nalliance stake (1) \nand touches bar \n\nELIMS",
        "Goal Rush for \nmiddle goal (2) and \nside goal (1) and \nalliance stake (1) \nand touches bar \n\nELIMS",
        "Goes for center \ntwo rings and \nscores on side \ngoal (3) and adds (2) from corner \n+ alliance stake (1) \n\nELIMS",
        "Goes for center \ntwo rings and \nscores on side \ngoal (3) and adds (2) from corner \n+ alliance stake (1) \n\nELIMS",
        "Skills programming \nroute \n\n(check the \nprog notebook \nfor full route)",
        "Blank Auton",
        "Tuning with the \nregression model",
        "Temporary testing \nauton function \n\n[DO NOT USE IN \nMATCH]",
        "Blank auton \n\njust in case",

    };
    static int auton_index = 0; // Index of the currently displayed auton option
    static lv_obj_t *auton_label; // Label for displaying the current auton option
    static lv_obj_t *auton_description;

    // Global pointers for view containers
    lv_obj_t *container_temps, *container_auton, *container_ts, *container_def, *container_main;

    // Function prototypes (definitions to be added based on your content)
    void createDisplayTempsView();
    void createDispAutonView();
    void createDispTSView();
    void createDispDefView();
    void createDispMainView();

    void update_shapes();

    static lv_obj_t *img_field;

    


    static lv_res_t btn_left_action(lv_obj_t *btn) {
        auton_index--;
        if (auton_index<0) auton_index = 14;
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        lv_label_set_text(auton_description, auton_desc_vec[auton_index]);
        update_shapes();
        return LV_RES_OK;
    }
    static lv_res_t btn_right_action(lv_obj_t *btn) {
        auton_index++;
        if (auton_index>14) auton_index = 0;
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        lv_label_set_text(auton_description, auton_desc_vec[auton_index]);
        update_shapes();
        return LV_RES_OK;
    }

    static lv_res_t btn_left_action_ts(lv_obj_t *btn) {
        current_motor_index--;
        if (current_motor_index<0) current_motor_index = 7;
        lv_label_set_text(motor_name_label, glb::motor_labels[current_motor_index].c_str());
        temp::update_motor_stats();
        return LV_RES_OK;
    }
    static lv_res_t btn_right_action_ts(lv_obj_t *btn) {
        current_motor_index++;
        if (current_motor_index>7) current_motor_index = 0;
        lv_label_set_text(motor_name_label, glb::motor_labels[current_motor_index].c_str());
        temp::update_motor_stats();
        return LV_RES_OK;
    }
    

    

    
    void init_shapes() {

        // Define points for the arrow shape

        // Create a style for the line
        static lv_style_t style_line;
        lv_style_copy(&style_line, &lv_style_plain);
        style_line.line.color = LV_COLOR_RED; // Set the color of the arrow
        style_line.line.width = 3; // Set the width of the lines

        // Style for circles and square with transparent fill and yellow outline
        static lv_style_t style_circle;
        lv_style_copy(&style_circle, &lv_style_plain); // Start with a base style
        style_circle.body.empty = 1; // Make it transparent inside
        style_circle.body.border.color = LV_COLOR_YELLOW; // Set the border color to bright yellow
        style_circle.body.border.width = 4; // Set the border width
        style_circle.body.radius = LV_RADIUS_CIRCLE; // This should make the shape fully rounded

        static lv_style_t style_square;
        lv_style_copy(&style_square, &style_circle); // Start with a base style
        style_square.body.radius = 0;

        // Apply the style to each circle
        for (int i = 0; i < 6; i++) {
            circles[i] = lv_btn_create(container_auton, NULL); // Create the button
            lv_obj_set_size(circles[i], 30, 30); // Set the size to ensure it's square
            lv_btn_set_style(circles[i], LV_BTN_STYLE_REL, &style_circle); // Apply the circular style
            lv_obj_set_hidden(circles[i], true); // Initially hide it
        }// Max radius for circles

        // Style for robot_square with light purple fill and dark purple outline
        static lv_style_t style_robot_square;
        lv_style_copy(&style_robot_square, &lv_style_plain); // Copy a base style
        style_robot_square.body.main_color = LV_COLOR_MAKE(204, 153, 255); // Light purple
        style_robot_square.body.grad_color = LV_COLOR_MAKE(204, 153, 255); // Light purple
        style_robot_square.body.border.color = LV_COLOR_MAKE(128, 0, 128); // Dark purple
        style_robot_square.body.border.width = 5; // Border width
        style_robot_square.body.radius = 0; // No rounding for squares


        // Square with transparent fill and yellow outline
        //style_circle.body.radius = 0; 
        square = lv_btn_create(container_auton, NULL);
        lv_obj_set_hidden(square, true);
        lv_btn_set_style(square, LV_BTN_STYLE_REL, &style_square); // Apply the style
        lv_obj_set_size(square, 40, 40); // Example size, adjust as needed
       // Override for square

        // Robot square with light purple fill and dark purple outline
        robot_square = lv_btn_create(container_auton, NULL);
        lv_obj_set_hidden(robot_square, true);
        lv_btn_set_style(robot_square, LV_BTN_STYLE_REL, &style_robot_square); // Apply the style
        lv_obj_set_size(robot_square, 40, 40); // Example size, adjust as needed

        line_arrow = lv_line_create(container_auton, NULL); // Create the line object on the active screen
 // Set the points (the second parameter is the number of points)
        lv_line_set_style(line_arrow, &style_line); // Apply the style
        //lv_obj_align(line_arrow, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_hidden(line_arrow, true);

        end_circle = lv_btn_create(container_auton, NULL);
        lv_obj_set_hidden(end_circle, true);

        static lv_style_t style_end_circle;
        lv_style_copy(&style_end_circle, &lv_style_plain); // Start with a base style
        style_end_circle.body.empty = 1; // Make it transparent inside
        style_end_circle.body.border.color = LV_COLOR_BLUE; // Set the border color to bright yellow
        style_end_circle.body.border.width = 4; // Set the border width
        style_end_circle.body.radius = LV_RADIUS_CIRCLE; // This should make the shape fully rounded

        lv_btn_set_style(end_circle, LV_BTN_STYLE_REL, &style_end_circle); // Apply the style
        lv_obj_set_size(end_circle, 20, 20); // Example size, adjust as needed

    }

    // Adjust shapes based on auton_index

    void update_shapes() {
        // Hide all shapes first
        for (int i = 0; i < 6; i++) {
            lv_obj_set_hidden(circles[i], true);
        }
        lv_obj_set_hidden(square, true);
        lv_obj_set_hidden(robot_square, true);
        lv_obj_set_hidden(line_arrow, true);
        lv_obj_set_hidden(end_circle, true);


        // Logic to adjust positions, sizes, visibility based on auton_index
        // This example shows a simple way to manage it. You need to adapt it based on your specific requirements.
        switch (auton_index) {
            case 0:
                // BLUE RUSH POSITIONING
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 106, 178); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 143, 140); 

                lv_obj_set_hidden(end_circle, false);
                lv_obj_set_pos(end_circle, 150, 110); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 45, 20);
                lv_obj_set_pos(square, 190, 110);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 190, 140); 

                //lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, blue_left_rush_points, 5);

                break;
            
            case 1:

                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 70, 31); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 0, 0); 

                lv_obj_set_hidden(circles[2], false);
                lv_obj_set_pos(circles[2], 0, 105); 

                lv_obj_set_hidden(circles[3], false);
                lv_obj_set_pos(circles[3], 71, 67); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 20, 45);
                lv_obj_set_pos(square, 105, 25);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 20, 65); 

                //lv_obj_set_hidden(line_arrow, false);


                break;
            case 2:
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 0, 0); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 97, 105); 

                lv_obj_set_hidden(circles[2], false);
                lv_obj_set_pos(circles[2], 63, 105); 
                
                lv_obj_set_hidden(circles[3], false);
                lv_obj_set_pos(circles[3], 10, 30); 

                lv_obj_set_hidden(circles[4], false);
                lv_obj_set_pos(circles[4], 97, 65); 

                lv_obj_set_hidden(circles[5], false);
                lv_obj_set_pos(circles[5], 105, 5); 

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 40, 1); 

                //lv_obj_set_hidden(line_arrow, false);

                break;

            case 3:

                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 106, 178); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 70, 140); 

                lv_obj_set_hidden(end_circle, false);
                lv_obj_set_pos(end_circle, 110, 210); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 45, 20);
                lv_obj_set_pos(square, 0, 110);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 20, 140); 

                lv_obj_set_hidden(end_circle, false);
                lv_obj_set_pos(end_circle, 70, 110); 


                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 106, 178); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 143, 140); 

                lv_obj_set_hidden(end_circle, false);
                lv_obj_set_pos(end_circle, 110, 210);  

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 45, 20);
                lv_obj_set_pos(square, 190, 110);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 190, 140); 

                //lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, blue_left_rush_points, 5);
                break;
            case 7:
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 106, 178); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 70, 140); 

                lv_obj_set_hidden(end_circle, false);
                lv_obj_set_pos(end_circle, 110, 210); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 45, 20);
                lv_obj_set_pos(square, 0, 110);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 20, 140); 

                //lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, blue_left_rush_points, 5);   
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                break;
            case 11:
                break;
            case 12:
                break;
            case 13:
                break;
            case 14:
                break;
        }
    }
    namespace temp{
        void update_motor_stats() {
            // Assuming motorV is accessible and has a method .at()
            pros::Motor motor = glb::motorV.at(current_motor_index); 
            
            bool error;
            char buffer[64]; // Buffer for formatting text
    
            double draw = motor.get_current_draw();
            if (draw>100000) error = true;
            else error = false;
    
            snprintf(buffer, sizeof(buffer), "%.2f %%", motor.get_efficiency());
            lv_label_set_text(stat_labels[0], error ? "ERROR" : buffer);
    
            snprintf(buffer, sizeof(buffer), "%d mA", draw);
            lv_label_set_text(stat_labels[1], error ? "ERROR" : buffer);
    
            snprintf(buffer, sizeof(buffer), "%d °C", motor.get_temperature());
            lv_label_set_text(stat_labels[2], error ? "ERROR" : buffer);
    
            snprintf(buffer, sizeof(buffer), "%.2f Nm", motor.get_torque());
            lv_label_set_text(stat_labels[3], error ? "ERROR" : buffer);
    
            snprintf(buffer, sizeof(buffer), "%d", motor.get_port());
            lv_label_set_text(stat_labels[4], buffer);
        }

        void updateMotorTemps() {
            char temp_str[10];
            int temp;
            int index = 0; 
    
            for (int i = 0; i < num_motors; i++) {
                // temp = glb::temps_a[i];
                pros::Motor motor = glb::motorV.at(index); 

                temp = motor.get_temperature();
    
                // Update the line meter's value
                lv_lmeter_set_value(motor_lmeters[i], temp);
    
                // Update the label text
                sprintf(temp_str, "%d°C", temp);
                lv_label_set_text(motor_temp_labels[i], temp_str);
            }
        }
        
        
        }
    


    void switchView(lv_obj_t *view_container) {
        // hides all other containers
        lv_obj_set_hidden(container_temps, true);
        lv_obj_set_hidden(container_auton, true);
        lv_obj_set_hidden(container_ts, true);
        lv_obj_set_hidden(container_def, true);
        lv_obj_set_hidden(container_main, true); 

        // only shows whichever view is passed through
        lv_obj_set_hidden(view_container, false);
    }

    // each of these switches the current view to a different container (indicated by function name)
    // Each are set to a different taskbaar button (below)
    static lv_res_t btn_temps_action(lv_obj_t *btn) {
        switchView(container_temps);
        return LV_RES_OK; 
    }

    static lv_res_t btn_auton_action(lv_obj_t *btn) {
        switchView(container_auton);
        return LV_RES_OK;
    }

    static lv_res_t btn_ts_action(lv_obj_t *btn) {
        switchView(container_ts);
        return LV_RES_OK;
    }
    
    static lv_res_t btn_def_action(lv_obj_t *btn) {
        switchView(container_def);
        return LV_RES_OK; 
    }

    static lv_res_t btn_main_action(lv_obj_t *btn) {
        switchView(container_main);
        return LV_RES_OK; 
    }




    void setupUI() {


        static lv_style_t style_btn;
        lv_style_copy(&style_btn, &lv_style_plain); // Start with a plain style for more control

        // Taskbar button style (used to copypaste onto all buttons below)
        style_btn.body.border.color = LV_COLOR_MAKE(0x00, 0x00, 0x80); // colored navy
        style_btn.body.border.width = 3; // change for thickness
        style_btn.body.border.part = LV_BORDER_FULL; 
        style_btn.body.main_color = LV_COLOR_MAKE(0x00, 0x00, 0x00); 
        style_btn.body.grad_color = LV_COLOR_MAKE(0x00, 0x00, 0x00); 
        style_btn.body.opa = LV_OPA_TRANSP; 
        style_btn.body.radius = LV_DPI / 8; // rounded radius calculator
        style_btn.text.color = LV_COLOR_MAKE(0xFF, 0xFF, 0xFF);

        // taskbar on  right
        lv_obj_t *taskbar = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(taskbar, 60, 240); 
        lv_obj_set_pos(taskbar, 420, 0); 

        // taskbar buttons  (set individually to assign actions later)
        lv_obj_t *btn_temps = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_temps, LV_BTN_STYLE_REL, &style_btn);
        lv_obj_set_size(btn_temps, 56, 44); 
        lv_obj_set_pos(btn_temps, 2, 2);

        lv_obj_t *btn_auton = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_auton, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_auton, 56, 44); 
        lv_obj_set_pos(btn_auton, 2, 49);

        lv_obj_t *btn_ts = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_ts, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_ts, 56, 44); 
        lv_obj_set_pos(btn_ts, 2, 96);

        lv_obj_t *btn_def = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_def, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_def, 56, 44); 
        lv_obj_set_pos(btn_def, 2, 143);

        lv_obj_t *btn_main = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_main, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_main, 56, 44); 
        lv_obj_set_pos(btn_main, 2, 190);

        // button actions
        lv_btn_set_action(btn_temps, LV_BTN_ACTION_CLICK, btn_temps_action);
        lv_btn_set_action(btn_auton, LV_BTN_ACTION_CLICK, btn_auton_action);
        lv_btn_set_action(btn_ts, LV_BTN_ACTION_CLICK, btn_ts_action);
        lv_btn_set_action(btn_def, LV_BTN_ACTION_CLICK, btn_def_action);
        lv_btn_set_action(btn_main, LV_BTN_ACTION_CLICK, btn_main_action);

        // Initializing containers
        container_temps = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_temps, 420, 240); 
        lv_obj_set_pos(container_temps, 0, 0); 
        lv_obj_set_hidden(container_temps, true);
      
        container_auton = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_auton, 420, 240); 
        lv_obj_set_pos(container_auton, 0, 0); 
        lv_obj_set_hidden(container_auton, true); //set as true for default

        container_ts = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_ts, 420, 240); 
        lv_obj_set_pos(container_ts, 0, 0); 
        lv_obj_set_hidden(container_ts, true);

        container_def = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_def, 420, 240); 
        lv_obj_set_pos(container_def, 0, 0); 
        lv_obj_set_hidden(container_def, true);

        container_main = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_main, 420, 240); 
        lv_obj_set_pos(container_main, 0, 0); 
        lv_obj_set_hidden(container_main, false);

        // "turn on" each view to start displaying thigs
        createDispMainView();
        createDisplayTempsView();
        createDispAutonView();
        createDispTSView();
        createDispDefView();
    }

    // temperature monitoring view for mid-match
    void createDisplayTempsView() {

        // calculate dimensions for each view
        int temp;
        const lv_coord_t container_width = 420; 
        const lv_coord_t container_height = 240;
        const lv_coord_t motor_block_width = container_width / 4; 
        const lv_coord_t motor_block_height = container_height / 2; 

        // create style to be applied to each view
        static lv_style_t lmeter_style;
        lv_style_copy(&lmeter_style, &lv_style_pretty);
        lmeter_style.body.main_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0); // Light grey for the inactive part
        lmeter_style.body.grad_color = LV_COLOR_MAKE(0xB0, 0x00, 0x00); // Darker red for the active part
        lmeter_style.line.color = LV_COLOR_MAKE(0x5E, 0x5E, 0x5E); // Dark grey for the scale lines
        lmeter_style.line.width = 3; // Thickness of the scale lines
        

        for (int i = 0; i < num_motors; i++) {
            // creating a container for each motor temp display
            lv_coord_t x_pos = (i % 4) * motor_block_width;
            lv_coord_t y_pos = (i / 4) * motor_block_height;
            lv_obj_t *motor_container = lv_cont_create(container_temps, NULL);
            lv_obj_set_size(motor_container, motor_block_width, motor_block_height);
            lv_obj_set_pos(motor_container, x_pos, y_pos);

            // Creating stuff for container
            //label
            lv_obj_t *label = lv_label_create(motor_container, NULL);


            lv_label_set_text(label, glb::motor_labels[i].c_str());
            lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

            // meter
            lv_obj_t *lmeter = lv_lmeter_create(motor_container, NULL);
            lv_lmeter_set_range(lmeter, 15, 60); // Set the new temperature range
           
            
            // adjusting temp
            temp = glb::temps_a[i];

            lv_lmeter_set_value(lmeter, temp); 
            lv_obj_set_size(lmeter, motor_block_width * 3 / 4, motor_block_height * 3 / 4);
            lv_obj_align(lmeter, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

            // apply style
            lv_lmeter_set_style(lmeter, &lmeter_style);
            motor_lmeters[i] = lmeter;

            // Label for meter temp
            lv_obj_t *temp_label = lv_label_create(motor_container, NULL);
            char temp_str[10];
            sprintf(temp_str, "%d°C", temp); 
            lv_label_set_text(temp_label, temp_str);
            lv_obj_align(temp_label, lmeter, LV_ALIGN_CENTER, 0, 0);
            motor_temp_labels[i] = temp_label;
        }
    }


    // view for auton selector
    void createDispAutonView(){
        

        lv_obj_t *img_field = lv_img_create(container_auton, NULL);
        lv_img_set_src(img_field, &field_img);
        lv_obj_set_size(img_field, 240, 240);
        lv_obj_set_pos(img_field, 0, 0);

        // L. arrow button
        lv_obj_t *btn_left = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_left, 20, 150); 
        lv_obj_set_size(btn_left, 70, 70);
        lv_obj_t *label_left = lv_label_create(btn_left, NULL);

        // R. arrow button
        lv_obj_t *btn_right = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_right, 240-70-20, 150); 
        lv_obj_set_size(btn_right, 70, 70);
        lv_obj_t *label_right = lv_label_create(btn_right, NULL);

        // Define a custom style for the buttons
        static lv_style_t style_btn_select;
        lv_style_copy(&style_btn_select, &lv_style_pretty); // Start with a base style

        // Set the background (fill) color to light orange
        style_btn_select.body.main_color = LV_COLOR_MAKE(255, 165, 0); // Example light orange color
        style_btn_select.body.grad_color = LV_COLOR_MAKE(255, 165, 0); // Same as main_color for a solid fill

        // Make the filling semi-transparent (opaque)
        style_btn_select.body.opa = LV_OPA_70; // Adjust opacity as needed, LV_OPA_70 is ~70% opacity
        style_btn_select.body.border.color = LV_COLOR_MAKE(255, 140, 0); // Example dark orange color
        style_btn_select.body.border.width = 2;

        // Set the corner roundness
        style_btn_select.body.radius = 10; // For fully rounded corners, or use a specific pixel value for less roundness


        // Apply the custom style to the left button
        lv_btn_set_style(btn_left, LV_BTN_STYLE_REL, &style_btn_select); // Apply the style for the released state
        lv_btn_set_style(btn_right, LV_BTN_STYLE_REL, &style_btn_select);

        static lv_style_t style_btn_released;
        lv_style_copy(&style_btn_released, &style_btn_select);

        style_btn_select.body.opa = LV_OPA_50;
        lv_btn_set_style(btn_left, LV_BTN_STYLE_PR, &style_btn_released); // Apply the style for the released state
        lv_btn_set_style(btn_right, LV_BTN_STYLE_PR, &style_btn_released); // Apply the same style for the pressed state

        // Set the text for the button labels
        lv_label_set_text(label_left, "<"); // Set text for the left button
        lv_label_set_text(label_right, ">"); // Set text for the right button


        // Auton options
        auton_label = lv_label_create(container_auton, NULL);
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        lv_obj_set_pos(auton_label, 240+46, 10); // centered in between buttons

        auton_description = lv_label_create(container_auton, NULL);
        lv_label_set_text(auton_description, auton_desc_vec[auton_index]);
        lv_obj_set_pos(auton_description, 240+10, 5+30); // centered in between buttons

        // confirm button
        lv_obj_t *btn_confirm = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_confirm, 240 + 40, 195); // Positioned at the bottom of the right half
        lv_obj_set_size(btn_confirm, 100, 40);
        lv_obj_t *label_confirm = lv_label_create(btn_confirm, NULL);
        lv_label_set_text(label_confirm, "Confirm");
        lv_btn_set_style(btn_confirm, LV_BTN_STYLE_REL, &lv_style_btn_rel); 
        // Set the button to green or any other color/style adjustments as needed

        // assigning actions
        lv_btn_set_action(btn_left, LV_BTN_ACTION_CLICK, btn_left_action);
        lv_btn_set_action(btn_right, LV_BTN_ACTION_CLICK, btn_right_action);
        // lv_btn_set_action(btn_confirm, LV_BTN_ACTION_CLICK, btn_confirm_action); // TODO

        init_shapes();
        update_shapes();
    }

 // Keep track of the currently displayed motor

    void createDispTSView() {
        // Motor name label
        // Inside create_motor_infographic_screen function
        motor_name_label = lv_label_create(container_ts, NULL);
        lv_label_set_text(motor_name_label, glb::motor_labels[current_motor_index].c_str());
        lv_obj_set_pos(motor_name_label, 50, 90);

        // Left and right buttons to switch the motor
        lv_obj_t *left_btn = lv_btn_create(container_ts, NULL);
        lv_obj_set_size(left_btn, 100, 60);
        lv_obj_align(left_btn, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, -10);
        lv_obj_t *left_label = lv_label_create(left_btn, NULL);
        lv_label_set_text(left_label, "<--");

        lv_obj_t *right_btn = lv_btn_create(container_ts, NULL);
        lv_obj_set_size(right_btn, 100, 60);
        lv_obj_align(right_btn, left_btn, LV_ALIGN_IN_RIGHT_MID, 110, 0);
        lv_obj_set_size(right_btn, 100, 60);
        lv_obj_t *right_label = lv_label_create(right_btn, NULL);
        lv_label_set_text(right_label, "-->");

        // Right side for motor stats - placeholders for now
        const char *stat_names[] = {"Efficiency:", "Current Draw:", "Temperature:", "Torque:", "Port:"};
        for (int i = 0; i < 5; i++) {
            lv_obj_t *stat_label = lv_label_create(container_ts, NULL);
            lv_label_set_text(stat_label, stat_names[i]); // Placeholder text
            lv_obj_align(stat_label, container_ts, LV_ALIGN_IN_TOP_LEFT, 220, (i * 50) + 10);
            stat_labels[i] = lv_label_create(container_ts, stat_label); // Create label for value next to name
            lv_label_set_text(stat_labels[i], "-"); // Placeholder value
            lv_obj_align(stat_labels[i], stat_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
        }

        lv_btn_set_action(left_btn, LV_BTN_ACTION_CLICK, btn_left_action_ts);
        lv_btn_set_action(right_btn, LV_BTN_ACTION_CLICK, btn_right_action_ts);
        temp::update_motor_stats();
    }
    const int checklist_count =  6; // Adjust based on your actual checklist size
    const char *checklist_items[checklist_count] = {
        "Swap license plates",
        "Check battery and secure in place",
        "Clean traction wheels",
        "Check rubber bands",
        "Check chain",
        "Reset T3 + PTO"
    };

    // Button click event callback function
    static int total_count = 0; 

    static lv_res_t btn_click_action_checklist(lv_obj_t * btn) {
        static lv_style_t style_btn_rel_green; // Released style (green)
        static lv_style_t style_btn_rel_red;   // Released style (red)
        static bool is_init = false;

        if (!is_init) {
            lv_style_copy(&style_btn_rel_green, &lv_style_btn_rel);
            style_btn_rel_green.body.main_color = LV_COLOR_MAKE(0, 255, 0); // Light green
            style_btn_rel_green.body.grad_color = LV_COLOR_MAKE(0, 200, 0); // Dark green

            lv_style_copy(&style_btn_rel_red, &lv_style_btn_rel);
            style_btn_rel_red.body.main_color = LV_COLOR_MAKE(255, 0, 0); // Light red
            style_btn_rel_red.body.grad_color = LV_COLOR_MAKE(200, 0, 0); // Dark red

            is_init = true;
        }

        // Determine the current color of the button to toggle
        lv_style_t *current_style = lv_btn_get_style(btn, LV_BTN_STYLE_REL);
        if (current_style->body.main_color.full == style_btn_rel_green.body.main_color.full) {
            // If currently green, switch to red
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &style_btn_rel_red);
        } else {
            // If not green (or red), switch to green
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &style_btn_rel_green);
            total_count ++; 
        }

        if (total_count == 6) {
            lv_obj_t *title_label = lv_label_create(container_def, NULL);
            lv_label_set_text(title_label, "READY TO RUN");
            lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 5);  
        } 
        
        return LV_RES_OK;
    }

    void createDispDefView() {

        // Add title label
        lv_obj_t *title_label = lv_label_create(container_def, NULL);
        lv_label_set_text(title_label, "Checklist");
        lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

        // Style for the red buttons
        static lv_style_t style_btn_red;
        lv_style_copy(&style_btn_red, &lv_style_btn_rel);
        style_btn_red.body.main_color = LV_COLOR_MAKE(255, 0, 0); // Light red
        style_btn_red.body.grad_color = LV_COLOR_MAKE(200, 0, 0); // Dark red

        for (int i = 0; i < checklist_count; ++i) {
            // Create button
            lv_obj_t *btn = lv_btn_create(container_def, NULL);
            // Assign the click action
            lv_btn_set_action(btn, LV_BTN_ACTION_CLICK, btn_click_action_checklist); 
            // Apply the red style
            lv_btn_set_style(btn, LV_BTN_STYLE_REL, &style_btn_red); 
            lv_obj_set_size(btn, 400, 30); // Set button size
            lv_obj_set_pos(btn, 10, 28+i*34);

            // Create label for the button
            lv_obj_t *label = lv_label_create(btn, NULL);
            lv_label_set_text(label, checklist_items[i]);
            lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);
        }
    }


    lv_obj_t *battery_value_label;
    lv_obj_t *battery_label;
    lv_obj_t *optical_label;
    lv_obj_t *timer_label;
    lv_obj_t *log_label;
    lv_obj_t *log_textarea;

    void createDispMainView(){

        lv_obj_t *title_label = lv_label_create(container_main, NULL);
        lv_label_set_text(title_label, "MAIN DIAGNOSTIC SCREEN");
        lv_obj_align(title_label, NULL, LV_ALIGN_IN_TOP_RIGHT, -10, 10);    
        
        // Battery percentage display
        battery_label = lv_label_create(container_main, NULL);
        lv_label_set_text(battery_label, "Battery: ");
        lv_obj_align(battery_label, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10);

        battery_value_label = lv_label_create(container_main, NULL);
        lv_label_set_text(battery_value_label, "50%");
        
        lv_obj_align(battery_value_label, battery_label, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

        // Optical sensor hue display
        optical_label = lv_label_create(container_main, NULL);
        lv_label_set_text(optical_label, "Optical Hue: No Hue Detected");
        lv_obj_align(optical_label, battery_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

        // Match timer display
        lv_obj_t *timer_label = lv_label_create(container_main, NULL);
        lv_label_set_text(timer_label, "Match Timer: 0:00");
        lv_obj_align(timer_label, optical_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

        // Diagnostic log display
        lv_obj_t *log_label = lv_label_create(container_main, NULL);
        lv_label_set_text(log_label, "Diagnostic Log:");
        lv_obj_align(log_label, timer_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);

        lv_obj_t *log_textarea = lv_ta_create(container_main, NULL);
        lv_obj_set_size(log_textarea, 400, 100);
        lv_obj_align(log_textarea, log_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
        lv_ta_set_text(log_textarea, ""); // Initialize with empty text  
    }

    
    void update_battery() 
    {
        int battery_percentage = battery::get_capacity();

        char battery_str[64];
        sprintf(battery_str, "    %d   ", battery_percentage);

        lv_label_set_text(battery_value_label, battery_str);
        
    }  
    void update_optical(){
        int hue = optical.get_hue();
        if (hue == NULL) {
            lv_label_set_text(optical_label, "Optical Hue: No Value");
            return;
        }
        const char* color_name;

        if (hue < 30 || hue >= 330) {
            color_name = "Red";
        } else if (hue >= 30 && hue < 90) {
            color_name = "Yellow";
        } else if (hue >= 90 && hue < 150) {
            color_name = "Green";
        } else if (hue >= 150 && hue < 210) {
            color_name = "Cyan";
        } else if (hue >= 210 && hue < 270) {
            color_name = "Blue";
        } else if (hue >= 270 && hue < 330) {
            color_name = "Magenta";
        } else {
            color_name = "Unknown";
        }

        char optical_str[64];
        snprintf(optical_str, sizeof(optical_str), "Optical Hue: %d (%s)", hue, color_name);
        lv_label_set_text(optical_label, optical_str);
    }
    
    void update_match_timer(int seconds) {
        int minutes = seconds / 60;
        int remaining_seconds = seconds % 60;
        char timer_str[64];
        snprintf(timer_str, sizeof(timer_str), "Match Timer: %d:%02d", minutes, remaining_seconds);
        lv_label_set_text(timer_label, timer_str);
    }

    // Function to update the diagnostic log
    void update_diagnostic_log() {
        static int last_disconnect_status[num_motors] = {0}; // Store last disconnect status for each motor

        for (int i = 0; i < num_motors; i++) {
            pros::Motor motor = glb::motorV.at(i);
            bool is_disconnected = motor.get_current_draw() > 100000; // Example condition, replace with actual disconnect check

            if (is_disconnected && !last_disconnect_status[i]) {
                char log_message[64];
                snprintf(log_message, sizeof(log_message), "Motor %d disconnected!", motor.get_port());
                lv_ta_add_text(log_textarea, log_message);
                lv_ta_add_text(log_textarea, "\n"); // Add a new line after each log message
            }

            last_disconnect_status[i] = is_disconnected;
        }
    }
    

    


}


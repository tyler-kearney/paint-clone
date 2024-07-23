#include <iostream>
#include <gtk/gtk.h>
#include <cairo.h>

// Prototypes
void brushClicked(GtkWidget* button, gpointer data);
void save(GtkWidget* menuItem, gpointer data);
void load(GtkWidget* menuItem, gpointer data);
gboolean onDrawingAreaDraw(GtkWidget* widget, cairo_t* cr, gpointer data);

// Global Variable for simplicity
int* currentPoint = nullptr;
std::vector<int> drawingData;

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Paint Clone");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    // Create menu bar
    GtkWidget* menuBar = gtk_menu_bar_new();

    // create file menu
    GtkWidget* fileMenu = gtk_menu_new();
    GtkWidget* saveAction = gtk_menu_item_new_with_label("Save");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileMenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(saveAction), fileManu);
    GtkWidget* loadAction = gtk_menu_item_new_with_label("Load");
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), loadAction);

    // Add menubar to the window
    gtk_box_pack_start(GTK_BOX(window), menuBar, FALSE, FALSE, 0);

    // Create a vertical toolbar
    GtkWidget* toolBar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Create buttons and add them to the toolbar
    GtkWidget* brushButton = gtk_button_new_with_label("Brush");
    gtk_box_pack_start(GTK_BOX(toolBar), brushButton, TRUE, TRUE, 0);

    // Add toolbar to the window
    gtk_box_pack_start(GTK_BOX(window), tollBar, FALSE, FALSE, 0);

    // Create drawing area
    GtkWidget* drawingArea = gtk_drawing_area_new();
    gtk_widget_set_app_paintable(drawingArea, TRUE);

    // Add drawing area to the window
    gtk_box_pack_start(GTK_BOX(window), drawingArea, TRUE, TRUE, 0);

    // Connect signals and slots
    g_signal_connect(brushButton, "clicked", G_Callback(brushCLicked), window);
    g_signal_connect(saveAction, "activate", G_Callback(save), window);
    g_signal_connect(load, "activate", G_Callback(load), window);
    g_signal_connect(drawingArea, "draw", G_Callback(onDrawingAreaDraw), window);

    // Show widgets
    gtk_widget_show_all(window);

    // Main GTK Loop
    gtk_main();

    return 0;
}

// Slot for brush button click
void brushClicked(GtkWidget* button, gpointer data) {
    GtkWidget* window = (GtkWidget*) data;
    // Simulate ckicking on drawing area to set a starting point
    currentPoint = (int*)malloc(2 * sizeof(int));
    currentPoint[0] = gtk_widget_get_allocation(window)->x + 50;
    currentPoint[1] = gtk_widget_get_allocation(window)->y + 50;
    gtk_widget_queue_draw(window); // Request draw
}

void save(GtkWidget* menuItem, gpointer data) {
    GtkWidget* window = (GtkWidget*)data;
    // Open a file stream
    std::ofstream saveFile("drawingData.txt");

    if (saveFile.is_open()) {
        // save the number of points
        saveFile << drawindData.size() << std::endl;

        // Loop to save each of the points (x, y)
        for (int i = 0; i < drawingData.size(); i += 2) {
            saveFile << drawingData[i] << "," << drawingData[i + 1] << std::endl;
        }

        saveFile.close();
        g_print("Drawing data saved to drawingData.txt\n");
    } else {
        g_printerr("Error opening file for saving\n");
    }
}

void load(GtkWidget* menuItem, gpointer data) {
    GtkWidget* window (GtkWidget*)data;
    drawingData.clear();

    std::ifstream loadFile("drawingData.txt");

    if (loadFile.is_open()) {
        int numPoints;
        // read number of points
        loadFile >> numPoints;

        // Read each point (x, y)
        for (int i = 0; i < numPoints; i++) {
            int x, y;
            std::string line;
            std::getline(loadFile, line);

            // Extract x and y values from the comma separated line
            std::stringstream ss(line);
            std::getline(ss, line, ',');
            x = std::stoi(line);
            std::getline(ss, line, ',');
            y = std::stoi(line);

            drawingData.push_back(x);
            drawingData.push_back(y);
        }

        loadFile.close();
        g_print("Drawing data loaded from drawingData.txt\n");
        gtk_widget_queue_draw(window);
    } else {
        g_printerr("Error opening load file\n");
    }
}

gboolian onDrawingAreaDraw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    // Check if there is a starting point
    if (currentPoint != nullptr) {
        // Draw with a red line. I intend to add a color change button, and will fix this once I do
        cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
        cairo_set_line_width(cr, 2.0);

        cairo_move_to(cr, currentPoint[0], currentPoint[1]);

        // Get mouse position
        int x, y;
        gdk_window_get_pointer(gtk_widget_get_window(widget), &x, &y);
        cairo_line_to(cr, x, y); // Draw line to current mouse position

        cairo_stroke(cr);

        // Update drawing data with current and ending points
        drawingData.push_back(currentPoint[0]);
        drawingData.push_back(currentPoint[1]);
        drawingData.push_back(x);
        drawingData.push_back(y);

        // Free memory allocated for currentPoint
        free(currentPoint);
        currentPoint = nullptr;
    }

    return FALSE;
}
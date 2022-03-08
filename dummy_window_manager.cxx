#include "dummy_window_manager.h"

dummy_window_manager::dummy_window_manager() {
    // do nothing.
}
bool dummy_window_manager::update() {
    return true;
}
bool dummy_window_manager::is_currently_active_window_owned_by_us() {
    return false;
}
void dummy_window_manager::toggle_window_state(bool shouldUpdate) {

}
void dummy_window_manager::minimize_active_window() {

}
void dummy_window_manager::show_active_window() {

}

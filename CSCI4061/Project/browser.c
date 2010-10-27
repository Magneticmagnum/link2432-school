#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;
comm_channel channel[UNRECLAIMED_TAB_COUNTER];

/*
 * Name:		uri_entered_cb
 * Input arguments:'entry'-address bar where the url was entered
 *			 'data'-auxiliary data sent along with the event
 * Output arguments:void
 * Function:	When the user hits the enter after entering the url
 *			in the address bar, 'activate' event is generated
 *			for the Widget Entry, for which 'uri_entered_cb'
 *			callback is called. Controller-tab captures this event
 *			and sends the browsing request to the router(/parent)
 *			process.
 */

void uri_entered_cb(GtkWidget* entry, gpointer data) {
	if (!data)
		return;
	browser_window* b_window = (browser_window*) data;
	comm_channel channel = b_window->channel;
	// Get the tab index where the URL is to be rendered
	int tab_index = query_tab_id_for_request(entry, data);
	if (tab_index <= 0) {
		//Append code for error handling
		perror("Error invalid tab index");
	}

	// Get the URL.
	char* uri = get_entered_uri(entry);
	// Prepare 'request' packet to send to router (/parent) process.
	// Append your code here
	child_req_to_parent req;
	req.type = NEW_URI_ENTERED;
	strcpy(req.req.uri_req.uri, uri);
	req.req.uri_req.render_in_tab = tab_index;
	int com_pipe = channel.child_to_parent_fd[1];
	int size = sizeof(req);
	write(com_pipe, &req, size);

}

/*

 * Name:		new_tab_created_cb
 * Input arguments:	'button' - whose click generated this callback
 *			'data' - auxillary data passed along for handling
 *			this event.
 * Output arguments:    void
 * Function:		This is the callback function for the 'create_new_tab'
 *			event which is generated when the user clicks the '+'
 *			button in the controller-tab. The controller-tab
 *			redirects the request to the parent (/router) process
 *			which then creates a new child process for creating
 *			and managing this new tab.
 */
void new_tab_created_cb(GtkButton *button, gpointer data)

{
	if (!data)
		return;
	int tab_index = ((browser_window*) data)->tab_index;
	comm_channel channel = ((browser_window*) data)->channel;

	// Create a new request of type CREATE_TAB

	child_req_to_parent new_req;
	new_req.type = CREATE_TAB;
	new_req.req.new_tab_req.tab_index = tab_index;
	// write to router to fork new tab
	int to_router = channel.child_to_parent_fd[1];
	int size = sizeof(new_req);
	//Append your code here
	write(to_router, &new_req, size);

}

int main()

{
	// Append your code here
	int * ctpf = channel[0].child_to_parent_fd;
	int* ptcf = channel[0].parent_to_child_fd;
	if (pipe(ctpf) != 0) {
		perror("Error pipe");
	}
	if (pipe(ptcf) != 0) {
		perror("Error pipe 2");
	}
	int controller = fork();
	if (controller == 0) {
		close(ctpf[1]);
		close(ptcf[0]);
		browser_window *bw_controller;
		create_browser(CONTROLLER_TAB, 0, (void(*)(void)) &new_tab_created_cb,
				(void(*)(void)) &uri_entered_cb, &bw_controller, channel[0]);
		show_browser();

	} else {
		close(ctpf[0]);
		close(ptcf[1]);

		int read_from_controller = channel[0].child_to_parent_fd[0];
		int flags = fcntl(read_from_controller, F_GETFL);
		flags |= O_NONBLOCK;
		fcntl(read_from_controller, F_SETFL, flags);

		// TODO Finish this part
		// TODO then error check
		while (tabs > 0)
			;
	}

	return 0;
}

/* CSci4061 F2010 Assignment 2
* section: 2
* date: 11/01/10
* names: Joe Houlton, Dong Kim, Mary Jane Thillen
* id: 3688484, 2834324, 2854910 */

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
		//If the tab index is invalid, throw an error.
		perror("Error: invalid tab index.");
                exit(1);
	}
	// Get the URL.
	char* uri = get_entered_uri(entry);
	// Prepare 'request' packet to send to router (/parent) process.	
	child_req_to_parent req;
	req.type = NEW_URI_ENTERED;
	strcpy(req.req.uri_req.uri, uri);
	req.req.uri_req.render_in_tab = tab_index;
	//Get the pipe file descriptor, and write the request to that pipe.
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
void new_tab_created_cb(GtkButton *button, gpointer data){
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
	//Attempts to write new tab data, if it fails, prints an error.
	if (write(to_router, &new_req, size) == -1) {
		perror("Error writing new tab data!");
	}
}

// Create two pipes at the channel index.  If pipe fails, return the error.
int create_pipes(int index) {
	//Setting up function variables
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	//Creating two pipes, one in each direction.
	int p1 = pipe(ctpf);
	int p2 = pipe(ptcf);
	//If the first pipe failes, print an error, and return the error code.
	if (p1 != 0) {
		perror("Error opening pipe 1.");
		return p1;
	}
	//If the second pipe fails, print an error, and return the error code.
	if (p2 != 0) {
		perror("Error opening pipe 2.");
		return p2;
	}
	//On correct completion of pipe opening, return 0.
	return 0;
}

//Closes the 'parent' ends of the pipe at a given index.
int close_pipe_ends_parent(int index) {
	//Setting up function variables
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	//Closing specified pipe ends
	int c1 = close(ctpf[1]);
	int c2 = close(ptcf[0]);
	//If there is an error closing pipe 1, a message is printed and the error
	//is returned
	if (c1 != 0) {
		perror("Error closing pipe 1.");
		return c1;
	}
	//If there is an error closing pipe 2, a message is printed and the error
	//is returned
	if (c2 != 0) {
		perror("Error closing pipe 2.");
		return c2;
	}
	//If everything is closed properly, return 0.
	return 0;
}

//Closes the 'child' ends of a pipe at a given index.
int close_pipe_ends_child(int index) {
	//Setting up function variables.
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	//Closing specified pipe ends.
	int c1 = close(ctpf[0]);
	int c2 = close(ptcf[1]);
	//If there is an error closing pipe 1, a message is printed and the error
	//is returned
	if (c1 != 0) {
		perror("Error closing pipe 1");
		return c1;
	}
	//If there is an error closing pipe 2, a message is printed and the error
	//is returned
	if (c2 != 0) {
		perror("Error closing pipe 2");
		return c2;
	}
	return 0;
}

//Sets read to non-blocking.
void set_unblock_read(int read_fd) {
	int flags;
	flags = fcntl(read_fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(read_fd, F_SETFL, flags);
}

int main() {
	//Get parent/router PID.
	int router_pid = getpid();
	//Create pipes and fork for Controller process, prints a message on an error, 
        // and returns -1.
	if (create_pipes(0) != 0) {
		perror("Failed to make pipes for controller process."
		return -1;
	}
	int controller = fork();
	if (controller == 0) {
		//If we are the controller process:
		//We are the child, so we close the unneeded pipe ends.  If this causes
		//an error, we print an error and return -1.
		if (close_pipe_ends_child(0) != 0) {
			perror("Cannot close pipe ends on controller.  Exiting...");
			return -1;
		}
		//Create and show the browser window.
		browser_window *bw_controller;
		create_browser(CONTROLLER_TAB, 0, G_CALLBACK(new_tab_created_cb),
				G_CALLBACK(uri_entered_cb), &bw_controller, channel[0]);
		show_browser();
	} else {
		//If we are not the controller process, close the unnneded pipe ends.
		//If this causes an error, handle it.
		if (close_pipe_ends_parent(0) != 0) {
			perror("Cannot close pipe ends on router.  Exiting...");
			//Send a tab kill request
			child_req_to_parent req;
			req.type = TAB_KILLED;
			req.req.killed_req.tab_index = 0;
			//Send the request. If it fails, print the error and return -1
			if (write(channel[0].parent_to_child_fd[1], &req, sizeof(req))
					== -1) {
				perror("Unable to send kill request to controller!");
				return -1;
			}
			//Return -1 because of pipe-closing error.
			return -1;
		}
		//Set up tab-handling variables.
		int comm_index = 0;
		int pipe_status[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int read_controller_fd = channel[0].child_to_parent_fd[0];
		int read_router_fd;
		int tabs = 1;
		browser_window *bw_tab;
		set_unblock_read(read_controller_fd);
		// Loop while any tabs are opened
		while (tabs > 0) {
			child_req_to_parent request;
			usleep(10);
			//If these match, we are the router.
			if (getpid() == router_pid) {
				//If the index is 1 we have the index of a used pipe and need to 
				//check for a request.
				if (pipe_status[comm_index] == 1) {
					//Read the information from the channel.
					int read_fd = channel[comm_index].child_to_parent_fd[0];
					//If there was an error, we continue checking the pipe 
					// statuses until we have a used pipe.
					if (read(read_fd, &request, sizeof(request)) == -1 && errno
							== EAGAIN) {
						comm_index++;
						comm_index = comm_index % 10;
						continue;
					}
				//Continue checking pipe statuses until we have a used pipe.	
				} else {
					comm_index++;
					comm_index = comm_index % 10;
					continue;
				}
			//If we are not the router;
			} else {
				//If we fail to read the request from the router, we try again.
				if (read(read_router_fd, &request, sizeof(request)) == -1
						&& errno == EAGAIN) {
					process_single_gtk_event();
					continue;
				}
			}
			req_type rt = request.type;
			//Depending on the request type, we take different actions.
			if (rt == CREATE_TAB) {
				//Check to make sure there is space to create a new tab.
				int i, unused_tab = 0;
				for (i = 1; i < 10; i++) {
					if (pipe_status[i] == 0) {
						unused_tab = i;
						break;
					}
				}
				//If there is space, make the pipes.
				if (unused_tab > 0) {
					create_pipes(unused_tab);
				//If there is not space, print and pop-up an error message, then continue.
				} else {
					char error_message[] =
							"No more free tabs.  Please close a tab to open a new one.";
					perror(error_message);
					gchar alertmsg[] =
							"No more free tabs.  Please close a tab to open a new one.";
					alert(alertmsg);
					continue;
				}
				//Once the pipes are created, fork the tab process.
				int pid = fork();
				if (pid == 0) {
					//We are the tab process, create tab window.
					//Close the unneeded pipe ends, print an error and return -1
					// if necessary.
					if (close_pipe_ends_child(unused_tab) != 0) {
						perror("Cannot close pipe ends in tab process!");
						return -1;
					}
					//Set up the variables to allow for requests.
					read_router_fd = channel[unused_tab].parent_to_child_fd[0];
					set_unblock_read(read_router_fd);
					//Create the Browser for this new tab.
					create_browser(URL_RENDERING_TAB, unused_tab, G_CALLBACK(
							new_tab_created_cb), G_CALLBACK(uri_entered_cb),
							&bw_tab, channel[unused_tab]);

				} else {
					//We are the router process, increment counters.
					//Close the unneeded pipe ends, print an error and return -1
					// if necessary.
					if (close_pipe_ends_parent(unused_tab) != 0) {
						perror("Cannot close pipe ends in router process!");
						return -1;
					}
					//Set the pipe status to 1, meaning used, set the read and 
					// unblocking, increment the tab-count variable.
					pipe_status[unused_tab] = 1;
					read_router_fd = channel[unused_tab].child_to_parent_fd[0];
					set_unblock_read(read_router_fd);
					tabs++;
				}

			}
			if (rt == NEW_URI_ENTERED) {
				if (getpid() == router_pid) {
					//If we are the router, we need to set the tab_index variable
					int tab_index = request.req.uri_req.render_in_tab;
					//If the tab is not opened, we need to return/pop-up an error.
					if (pipe_status[tab_index] == 0) {
						gchar error_message[] = "That tab is not opened.";
						perror(error_message);
						alert(error_message);
						continue;
					//If the tab is open, we need to write the uri request to the 
					//tab process.
					} else {
						//If writing the request fails, print an error.
						if (write(channel[tab_index].parent_to_child_fd[1],
								&request, sizeof(request)) == -1) {
							perror("Warning, unable to send uri request to tab process");
						}
					}
				} else {
					//If we are the tab process, we need to render the page 
					// in the window.
					render_web_page_in_tab(request.req.uri_req.uri, bw_tab);
				}

			}
			if (rt == TAB_KILLED) {
				//tab_index is the index of the tab kill signal
				int tab_index = request.req.killed_req.tab_index;
				if (getpid() == router_pid) {
					//If we are the router, send the kill tab request, 
					// and print an error if needed.
					if (write(channel[tab_index].parent_to_child_fd[1],
							&request, sizeof(request)) == -1) {
						perror("Error, unable to send kill tab request from router to tab");
					}
					//Close unneeded pipe ends, print an error and return -1 if failed.
					if (close_pipe_ends_child(tab_index) != 0) {
						perror("Cannot close pipe ends in router tab kill!");
						return -1;
					}
					//Return the tab to "unused" status, decrement the number of tabs.
					pipe_status[tab_index] = 0;
					tabs--;
				} else {
					//If we are the tab process, close unnneded pipes.
					//If this causes an error, print the error.
					if (close_pipe_ends_parent(tab_index) != 0) {
						perror("Cannot close pipe ends in tab process!");
					}
					process_all_gtk_events();
					break;
				}

			}

		}
	}
	return 0;
}

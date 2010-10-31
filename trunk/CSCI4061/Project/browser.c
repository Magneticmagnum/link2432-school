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
void create_pipes(int index) {
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	if (pipe(ctpf) != 0) {
		perror("Error pipe");
	}
	if (pipe(ptcf) != 0) {
		perror("Error pipe 2");
	}
}

void close_pipe_ends_parent(int index) {
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	close(ctpf[1]);
	close(ptcf[0]);
}

void close_pipe_ends_child(int index) {
	int * ctpf = channel[index].child_to_parent_fd;
	int * ptcf = channel[index].parent_to_child_fd;
	close(ctpf[0]);
	close(ptcf[1]);
}

void set_unblock_read(int read_fd) {
	int flags;

	flags = fcntl(read_fd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(read_fd, F_SETFL, flags);
}

//TODO refactor code in the main loop to functions
//TODO implement more error handling
//TODO add comments
int main()
{

	int router_pid = getpid();
	printf("Parent PID = %d\n", router_pid);
	// create pipes and fork for controller
	create_pipes(0);
	int controller = fork();
	if (controller == 0) {
		// controller process, create window
		printf("Controller PID = %d\n", getpid());
		close_pipe_ends_child(0);
		browser_window *bw_controller;
		create_browser(CONTROLLER_TAB, 0, G_CALLBACK(new_tab_created_cb),
				G_CALLBACK(uri_entered_cb), &bw_controller, channel[0]);
		show_browser();
	} else {
		// router process
		close_pipe_ends_parent(0);
		int comm_index = 0;
		int pipe_status[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int read_controller_fd = channel[0].child_to_parent_fd[0];
		int read_router_fd;
		int tabs = 1;
		browser_window *bw_tab;
		set_unblock_read(read_controller_fd);
		// Loop while tabs are opened
		while (tabs > 0) {
			child_req_to_parent request;
			usleep(1);
			if (getpid() == router_pid) {
				if (pipe_status[comm_index] == 1) {
					int read_fd = channel[comm_index].child_to_parent_fd[0];
					if (read(read_fd, &request, sizeof(request)) == -1 && errno
							== EAGAIN) {
						comm_index++;
						comm_index = comm_index % 10;
						continue;
					}
				} else {
					comm_index++;
					comm_index = comm_index % 10;
					continue;
				}

				printf("got request\n");
			} else {
				if (read(read_router_fd, &request, sizeof(request)) == -1
						&& errno == EAGAIN) {
					process_single_gtk_event();
					continue;
				}
				printf("read pipe\n");
			}
			req_type rt = request.type;

			// check request type
			if (rt == CREATE_TAB) {
				// create pipes and fork a tab process
				int i, unused_tab = 0;
				for (i = 1; i < 10; i++) {
					if (pipe_status[i] == 0) {
						unused_tab = i;
						break;
					}
				}
				if (unused_tab > 0) {
					create_pipes(unused_tab);
				} else {
					char error_message[] = "No more free tabs.  Please close a tab to open a new one.";
					perror(error_message);
					//TODO: get popup alert to work
					//alert(error_message);
					continue;
				}

				int pid = fork();
				printf("PID = %d and parent %d\n", getpid(), getppid());
				if (pid == 0) {
					// tab process, create tab window
					close_pipe_ends_child(unused_tab);
					read_router_fd = channel[unused_tab].parent_to_child_fd[0];
					set_unblock_read(read_router_fd);
					create_browser(URL_RENDERING_TAB, unused_tab, G_CALLBACK(
							new_tab_created_cb), G_CALLBACK(uri_entered_cb),
							&bw_tab, channel[unused_tab]);

				} else {
					// router process, increment counters

					close_pipe_ends_parent(unused_tab);
					pipe_status[unused_tab] = 1;
					read_router_fd = channel[unused_tab].child_to_parent_fd[0];
					set_unblock_read(read_router_fd);
					tabs++;
				}

			}
			if (rt == NEW_URI_ENTERED) {
				if (getpid() == router_pid) {
					int tab_index = request.req.uri_req.render_in_tab;
					if (pipe_status[tab_index] == 0) {
						char error_message[] = "That tab is not opened.";
						perror(error_message);
						//TODO: get popup alert to work
						//alert(error_message);
						continue;

					} else {
						// router process, send uri request to tab process
						write(channel[tab_index].parent_to_child_fd[1],
								&request, sizeof(request));
					}
				} else {
					// tab process, render page in window
					printf("%s\n", request.req.uri_req.uri);
					render_web_page_in_tab(request.req.uri_req.uri, bw_tab);
					printf("Render page:PID = %d and parent %d\n", getpid(),
							getppid());
				}

			}
			if (rt == TAB_KILLED) {
				// index of tab kill signal
				printf("mooo\n");
				int tab_index = request.req.killed_req.tab_index;
				if (getpid() == router_pid) {
					write(channel[tab_index].parent_to_child_fd[1], &request,
							sizeof(request));
					close_pipe_ends_child(tab_index);
					pipe_status[tab_index] = 0;
					// send kill signal to tab

					//					}
					tabs--;
				} else {
					// tab process, close pipes
					close_pipe_ends_parent(tab_index);
					process_all_gtk_events();
					printf("Exiting:PID = %d and tab # = %d\n", getpid(),
							tab_index);
					break;
				}

			}

		}
	}

	printf("PID %d Exited\n", getpid());

	return 0;
}

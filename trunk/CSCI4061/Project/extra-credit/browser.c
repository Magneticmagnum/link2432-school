#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/ipc.h>

extern int errno;
extern int shared_bookmarks;
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
	}

	// Get the URL.
	char* uri = get_entered_uri(entry);

	// Prepare 'request' packet to send to router (/parent) process.
	// Append your code here
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
	//Append your code here

}
/*
 * Name:                bookmark_curr_page_cb
 * Input Arguments:     data - pointer to 'browser_window' data
 *                      which got initialized after call to 'create_window'
 * Output arguments:    void
 * Function:            The callback is called when the user-clicks the
 *                      bookmark menu in the URL-RENDERING window. The function
 *                      extracts the list of bookmarked web-pages from the
 *                      shared memory and appends the current webpage
 *                      into the list.
 */

void bookmark_curr_page_cb(void *data) {
	browser_window* b_window = (browser_window*) data;

	//get the current web-page to bookmark
	const char* curr_webpage = get_current_uri(b_window);
	bookmarks* ptr = (bookmarks*) shmat(shared_bookmarks, NULL, 0);
	if (ptr == (void*) -1) {
		perror("Unable to attach bookmark array");
	} else {
		int num_marks = ptr[0].bookmarks_count;
		if (num_marks < MAX_BOOKMARKS) {
			bookmarks* current = ptr[num_marks];
			strncpy(current.uri, curr_webpage, 256);
			ptr[0].bookmarks_count++;
		} else {
			gchar err[] = "Too many bookmarks!";
			alert(err);
		}
	}
	shmdt(ptr);
}
void assign_bookmark_id() {
	int shared_bookmarks = shmget(SHM_KEY, MAX_BOOKMARKS * sizeof(bookmarks),
			IPC_CREAT | 0777);
}
int main()

{
	assign_bookmark_id();
	bookmarks* bm = shmat(shared_bookmarks, NULL, 0);
	bm[0].bookmarks_count = 0;
	shmdt(bm);
	// Append your code here
	// then fork

	return 0;
}


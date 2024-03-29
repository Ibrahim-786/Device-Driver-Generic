/*
 *  example.c
 *
 *  This module contains the source code for the /dev/example device
 *  developed as part of the Neutrino "Writing a Resource Manager"
 *  section.
 *
 *  If using a shared target, please change EXAMPLE_NAME to something
 *  unique for you, so to avoid testing somebody else's code.
 *
 *
 *  This module contains all of the functions necessary.
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>

/* default name for this device: /dev/example */

#define EXAMPLE_NAME "/dev/example"

/* change to something else if sharing a target */
// #define EXAMPLE_NAME "/dev/dagexample"

void options(int argc, char *argv[]);

/*
 *  these prototypes are needed since we are using their names in main ()
 */

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);

/*
 *  our connect and I/O functions
 */

resmgr_connect_funcs_t connect_funcs;
resmgr_io_funcs_t io_funcs;

/*
 *  our dispatch, resource manager and iofunc variables
 */

dispatch_t *dpp;
resmgr_attr_t rattr;
dispatch_context_t *ctp;
iofunc_attr_t ioattr;

char *progname = "example";
int optv; // -v for verbose operation

int main(int argc, char *argv[])
{
	int pathID;

	printf("%s:  starting...\n", progname);

	options(argc, argv);

	/*
	 * allocate and initialize a dispatch structure for use by our
	 * main loop
	 */

	dpp = dispatch_create();
	if (dpp == NULL)
	{
		fprintf(stderr, "%s:  couldn't dispatch_create: %s\n", progname, strerror(errno));
		exit(1);
	}

	/*
	 * set up the resource manager attributes structure, we'll
	 * use this as a way of passing information to resmgr_attach().
	 * For now, we just use defaults.
	 */

	memset(&rattr, 0, sizeof(rattr)); /* using the defaults for rattr */

	/*
	 * initialize the connect functions and I/O functions tables to
	 * their defaults by calling iofunc_func_init().
	 *
	 * connect_funcs, and io_funcs variables are already declared.
	 *
	 */

	/* over-ride the connect_funcs handler for open with our io_open,
	 * and over-ride the io_funcs handlers for read and write with our
	 * io_read and io_write handlers
	 */

	/* initialize our device description structure
	 */

	iofunc_attr_init(&ioattr, S_IFCHR | 0666, NULL, NULL);
	/*
	 *  call resmgr_attach to register our prefix with the
	 *  process manager, and also to let it know about our connect
	 *  and I/O functions.
	 *
	 *  On error, returns -1 and errno is set.
	 */

	if (pathID == -1)
	{
		fprintf(stderr, "%s:  couldn't attach pathname: %s\n", progname, strerror(errno));
		exit(1);
	}

	ctp = dispatch_context_alloc(dpp);

	while (1)
	{
		if ((ctp = dispatch_block(ctp)) == NULL)
		{
			fprintf(stderr, "%s:  dispatch_block failed: %s\n", progname, strerror(errno));
			exit(1);
		}
		dispatch_handler(ctp);
	}
}

/*
 *  io_open
 *
 *  we are called here when the client does an open.
 *  It is up to us to establish a context (in this
 *  case NULL will do just fine), and return a status
 *  code.
 */

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra)
{
	if (optv)
	{
		printf("%s:  in io_open\n", progname);
	}

	return (iofunc_open_default(ctp, msg, handle, extra));
}

/*
 *  io_read
 *
 *  At this point, the client has called their library "read"
 *  function, and expects zero or more bytes.  Currently our 
 *  /dev/example resource manager returns zero bytes to
 *  indicate EOF -- no more bytes expected.
 * 
 *  After our exercises, it will return some data.
 */

int io_read(resmgr_context_t *ctp, io_read_t *msg, RESMGR_OCB_T *ocb)
{
	int status;

	if (optv)
	{
		printf("%s:  in io_read\n", progname);
	}

	if ((status = iofunc_read_verify(ctp, msg, ocb, NULL)) != EOK)
		return (status);

	// No special xtypes
	if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE)
	{
		return (ENOSYS);
	}

	_IO_SET_READ_NBYTES (ctp, 0);

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_ATIME;

	return (_RESMGR_NPARTS (0));
}

/*
 *  io_write
 *
 *  At this point, the client has called their library "write"
 *  function, and expects that our resource manager will write
 *  the number of bytes that they have specified to some device.
 *
 *  Currently, for /dev/example, all of the clients writes always
 *  work -- they just go into Deep Outer Space.
 * 
 *  After our updates, they will be displayed on standard out.
 */

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb)
{
	int status;

	if (optv)
	{
		printf("%s:  in io_write\n", progname);
	}

	if ((status = iofunc_write_verify(ctp, msg, ocb, NULL)) != EOK)
		return (status);

	// No special xtypes
	if ((msg->i.xtype & _IO_XTYPE_MASK) != _IO_XTYPE_NONE)
	{
		return (ENOSYS);
	}

	_IO_SET_WRITE_NBYTES (ctp, msg -> i.nbytes);

	if (msg->i.nbytes > 0)
		ocb->attr->flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;

	return (_RESMGR_NPARTS (0));
}

/*
 *  options
 *
 *  This routine handles the command line options.
 *  For our simple /dev/example, we support:
 *      -v      verbose operation
 */

void options(int argc, char *argv[])
{
	int opt;

	optv = 0;

	while ((opt = getopt(argc, argv, "v")) != -1)
	{
		switch (opt)
		{
		case 'v':
			optv++;
			break;
		}
	}

}

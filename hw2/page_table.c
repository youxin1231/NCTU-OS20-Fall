#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "page_table_driver.h"

#define LAST(k,n) ((k) & ((1<<(n))-1))
#define MID(k,m,n) LAST((k)>>(m),((n)-(m)))

int fd;

// Obtain my cr3 value (a.k.a. PML4 table physical address)
uint64_t get_cr3_value()
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_CR3;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Given a physical address, return the value
uint64_t read_physical_address(uint64_t physical_address)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_READ;
	cmd.request[1] = physical_address;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
	return cmd.ret;
}

// Write value to a physical address
void write_physical_address(uint64_t physical_address, uint64_t value)
{
	struct ioctl_arg cmd;
	int ret;
	cmd.request[0] = IO_WRITE;
	cmd.request[1] = physical_address;
	cmd.request[2] = value;
	ret = ioctl(fd, IOCTL_REQUEST, &cmd);
}

int main()
{
	char *x = (char*)aligned_alloc(4096, 4096) + 0x123;
	char *y = (char*)aligned_alloc(4096, 4096) + 0x123;
	strcpy(x, "This is a simple HW.");
	strcpy(y, "You have to modify my page table.");
	fd = open("/dev/os", O_RDONLY);
	if(fd < 0) 
	{
		printf("Cannot open device!\n");
		return 0;
	}

	printf("Before\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 1 */
	// ------------------------------------------------
	// Modify page table entry of y
	// Let y point to x's physical address
	// ------------------------------------------------

	uint64_t cr3_value = get_cr3_value();
	uint64_t PML4_x = MID((uint64_t)&x,39,48), PDPT_x = MID((uint64_t)&x,30,39), PD_x = MID((uint64_t)&x,21,30), PT_x = MID((uint64_t)&x,12,21), offset_x = MID((uint64_t)&x,0,12);
	uint64_t PML4_y = MID((uint64_t)&y,39,48), PDPT_y = MID((uint64_t)&y,30,39), PD_y = MID((uint64_t)&y,21,30), PT_y = MID((uint64_t)&y,12,21), offset_y = MID((uint64_t)&y,0,12);	
	
	uint64_t address_x = MID(cr3_value,12,36) << 12 | (PML4_x << 3);
	address_x = read_physical_address(address_x);
	address_x = MID(address_x,12,36) << 12 | (PDPT_x << 3);
	address_x = read_physical_address(address_x);
	address_x = MID(address_x,12,36) << 12 | (PD_x << 3);
	address_x = read_physical_address(address_x);
	address_x = MID(address_x,12,36) << 12 | (PT_x << 3);
	address_x = read_physical_address(address_x);
	address_x = MID(address_x,12,36) << 12 | offset_x;
	address_x = read_physical_address(address_x);

	uint64_t address_y = MID(cr3_value,12,36) << 12 | (PML4_y << 3);
	address_y = read_physical_address(address_y);
	address_y = MID(address_y,12,36) << 12 | (PDPT_y << 3);
	address_y = read_physical_address(address_y);
	address_y = MID(address_y,12,36) << 12 | (PD_y << 3);
	address_y = read_physical_address(address_y);
	address_y = MID(address_y,12,36) << 12 | (PT_y << 3);
	address_y = read_physical_address(address_y);
	address_y = MID(address_y,12,36) << 12 | offset_y;
	uint64_t address_y_origin = read_physical_address(address_y);

	write_physical_address(address_y,address_x);

	getchar();

	printf("After modifying page table\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	getchar();

	strcpy(y, "When you modify y, x is modified actually.");
	printf("After modifying string y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	/* TODO 2 */
	// ------------------------------------------------
	// Recover page table entry of y
	// Let y point to its original address
	// You may need to store y's original address at previous step
	// ------------------------------------------------

	write_physical_address(address_y,address_y_origin);

	getchar();

	printf("After recovering page table of y\n");
	printf("x : %s\n", x);
	printf("y : %s\n", y);

	close(fd);
}

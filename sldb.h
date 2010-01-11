#ifndef __SLDB_H__
#define __SLDB_H__

class SidTuneMod;

class sldb_sum
{
private:
	unsigned char			digest[16];
	mem_block_t<unsigned>	lengths;
public:
	bool set_digest(const char * digest_string);
	bool check_digest(const void * song_digest);

	void add_length(unsigned length);
	unsigned get_length(unsigned index);
};

class sldb
{
private:
	critical_section sync;
	ptr_list_t<sldb_sum> sums;
public:
	sldb() {}
	~sldb() { unload(); }

	bool load(const char * path, abort_callback & p_abort);
	bool loaded();
	void unload();

	unsigned get_count();

	unsigned find(SidTuneMod * tune, unsigned index);
};

#endif

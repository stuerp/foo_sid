#pragma once

class SidTuneMod;

class sldb_sum
{
public:
    bool set_digest(const char * digest_string);
    bool check_digest(const void * song_digest);

    void add_length(unsigned length);
    unsigned get_length(unsigned index);

private:
    unsigned char digest[16];
    pfc::array_t<t_uint32> lengths;
};

class sldb
{
public:
    sldb() 
    {
    }

    ~sldb()
    {
        unload();
    }

    bool load(const char * path, abort_callback & p_abort);
    bool loaded();
    void unload();

    unsigned get_count();

    unsigned find(SidTuneMod * tune, unsigned index);

private:
    critical_section sync;
    pfc::ptr_list_t<sldb_sum> sums;
};

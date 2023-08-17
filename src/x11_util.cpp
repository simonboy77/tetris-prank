static u8
get_alpha(u32 colour)
{
    return (u8)(colour >> 24);
}

static u8
get_red(u32 colour)
{
    return (u8)(colour >> 16);
}

static u8
get_green(u32 colour)
{
    return (u8)(colour >> 8);
}

static u8
get_blue(u32 colour)
{
    return (u8)(colour);
}

static u32
set_alpha(u32 colour, u8 alpha)
{
    return (colour & 0x00FFFFFF) + (alpha << 24);
}

static u32
set_red(u32 colour, u8 red)
{
    return (colour & 0xFF00FFFF) + (red << 16);
}

static u32
set_green(u32 colour, u8 green)
{
    return (colour & 0xFFFF00FF) + (green << 8);
}

static u32
set_blue(u32 colour, u8 blue)
{
    return (colour & 0xFFFFFF00) + (blue);
}

static u32
darken(u32 colour, f32 factor)
{
    colour = set_red(colour, get_red(colour) / factor);
    colour = set_green(colour, get_green(colour) / factor);
    colour = set_blue(colour, get_blue(colour) / factor);

    return colour;
}
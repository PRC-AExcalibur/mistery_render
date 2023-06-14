#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>


#pragma pack(push,1)
struct TGAHeader {
    std::uint8_t  idlength = 0;
    std::uint8_t  colormaptype = 0;
    std::uint8_t  datatypecode = 0;
    std::uint16_t colormaporigin = 0;
    std::uint16_t colormaplength = 0;
    std::uint8_t  colormapdepth = 0;
    std::uint16_t x_origin = 0;
    std::uint16_t y_origin = 0;
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t  bitsperpixel = 0;
    std::uint8_t  imagedescriptor = 0;
};
#pragma pack(pop)

struct TGAColor {
    std::uint8_t bgra[4] = {0,0,0,0};
    std::uint8_t bytespp = 4;
    std::uint8_t& operator[](const int i) { return bgra[i]; }
};

struct TGAImage {
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };

    TGAImage() = default;
    TGAImage(const int w, const int h, const int bpp): w(w), h(h), bpp(bpp), data(w*h*bpp, 0) {}
    
    inline bool read_tga_file(const std::string filename);
    inline bool write_tga_file(const std::string filename, const bool vflip=true, const bool rle=true) const;
    inline void flip_horizontally();
    inline void flip_vertically();
    inline TGAColor get(const int x, const int y) const;
    inline void set(const int x, const int y, const TGAColor &c);
    inline int width()  const;
    inline int height() const;
private:
    inline bool   load_rle_data(std::ifstream &in);
    inline bool unload_rle_data(std::ofstream &out) const;

    int w = 0;
    int h = 0;
    std::uint8_t bpp = 0;
    std::vector<std::uint8_t> data = {};
};

inline bool TGAImage::read_tga_file(const std::string filename) {
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        return false;
    }
    TGAHeader header;
    in.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!in.good()) {
        std::cerr << "an error occured while reading the header\n";
        return false;
    }
    w   = header.width;
    h   = header.height;
    bpp = header.bitsperpixel>>3;
    if (w<=0 || h<=0 || (bpp!=GRAYSCALE && bpp!=RGB && bpp!=RGBA)) {
        std::cerr << "bad bpp (or width/height) value\n";
        return false;
    }
    size_t nbytes = bpp*w*h;
    data = std::vector<std::uint8_t>(nbytes, 0);
    if (3==header.datatypecode || 2==header.datatypecode) {
        in.read(reinterpret_cast<char *>(data.data()), nbytes);
        if (!in.good()) {
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else if (10==header.datatypecode||11==header.datatypecode) {
        if (!load_rle_data(in)) {
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    } else {
        std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
        return false;
    }
    if (!(header.imagedescriptor & 0x20))
        flip_vertically();
    if (header.imagedescriptor & 0x10)
        flip_horizontally();
    std::cerr << w << "x" << h << "/" << bpp*8 << "\n";
    return true;
}

inline bool TGAImage::load_rle_data(std::ifstream &in) {
    size_t pixelcount = w*h;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;
    TGAColor colorbuffer;
    do {
        std::uint8_t chunkheader = 0;
        chunkheader = in.get();
        if (!in.good()) {
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
        if (chunkheader<128) {
            chunkheader++;
            for (int i=0; i<chunkheader; i++) {
                in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
                if (!in.good()) {
                    std::cerr << "an error occured while reading the header\n";
                    return false;
                }
                for (int t=0; t<bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        } else {
            chunkheader -= 127;
            in.read(reinterpret_cast<char *>(colorbuffer.bgra), bpp);
            if (!in.good()) {
                std::cerr << "an error occured while reading the header\n";
                return false;
            }
            for (int i=0; i<chunkheader; i++) {
                for (int t=0; t<bpp; t++)
                    data[currentbyte++] = colorbuffer.bgra[t];
                currentpixel++;
                if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
                }
            }
        }
    } while (currentpixel < pixelcount);
    return true;
}

inline bool TGAImage::write_tga_file(const std::string filename, const bool vflip, const bool rle) const {
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        return false;
    }
    TGAHeader header = {};
    header.bitsperpixel = bpp<<3;
    header.width  = w;
    header.height = h;
    header.datatypecode = (bpp==GRAYSCALE?(rle?11:3):(rle?10:2));
    header.imagedescriptor = vflip ? 0x00 : 0x20; // top-left or bottom-left origin
    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    if (!rle) {
        out.write(reinterpret_cast<const char *>(data.data()), w*h*bpp);
        if (!out.good()) {
            std::cerr << "can't unload raw data\n";
            return false;
        }
    } else if (!unload_rle_data(out)) {
            std::cerr << "can't unload rle data\n";
            return false;
        }
    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    if (!out.good()) {
        std::cerr << "can't dump the tga file\n";
        return false;
    }
    return true;
}

// TODO: it is not necessary to break a raw chunk for two equal pixels (for the matter of the resulting size)
inline bool TGAImage::unload_rle_data(std::ofstream &out) const {
    const std::uint8_t max_chunk_length = 128;
    size_t npixels = w*h;
    size_t curpix = 0;
    while (curpix<npixels) {
        size_t chunkstart = curpix*bpp;
        size_t curbyte = curpix*bpp;
        std::uint8_t run_length = 1;
        bool raw = true;
        while (curpix+run_length<npixels && run_length<max_chunk_length) {
            bool succ_eq = true;
            for (int t=0; succ_eq && t<bpp; t++)
                succ_eq = (data[curbyte+t]==data[curbyte+t+bpp]);
            curbyte += bpp;
            if (1==run_length)
                raw = !succ_eq;
            if (raw && succ_eq) {
                run_length--;
                break;
            }
            if (!raw && !succ_eq)
                break;
            run_length++;
        }
        curpix += run_length;
        out.put(raw?run_length-1:run_length+127);
        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
        out.write(reinterpret_cast<const char *>(data.data()+chunkstart), (raw?run_length*bpp:bpp));
        if (!out.good()) {
            std::cerr << "can't dump the tga file\n";
            return false;
        }
    }
    return true;
}

inline TGAColor TGAImage::get(const int x, const int y) const {
    if (!data.size() || x<0 || y<0 || x>=w || y>=h)
        return {};
    TGAColor ret = {0, 0, 0, 0, bpp};
    const std::uint8_t *p = data.data()+(x+y*w)*bpp;
    for (int i=bpp; i--; ret.bgra[i] = p[i]);
    return ret;
}

inline void TGAImage::set(int x, int y, const TGAColor &c) {
    if (!data.size() || x<0 || y<0 || x>=w || y>=h) return;
    memcpy(data.data()+(x+y*w)*bpp, c.bgra, bpp);
}

inline void TGAImage::flip_horizontally() {
    int half = w>>1;
    for (int i=0; i<half; i++)
        for (int j=0; j<h; j++)
            for (int b=0; b<bpp; b++)
                std::swap(data[(i+j*w)*bpp+b], data[(w-1-i+j*w)*bpp+b]);
}

inline void TGAImage::flip_vertically() {
    int half = h>>1;
    for (int i=0; i<w; i++)
        for (int j=0; j<half; j++)
            for (int b=0; b<bpp; b++)
                std::swap(data[(i+j*w)*bpp+b], data[(i+(h-1-j)*w)*bpp+b]);
}

inline int TGAImage::width() const {
    return w;
}

inline int TGAImage::height() const {
    return h;
}
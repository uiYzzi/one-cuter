#!/usr/bin/env python2
# encoding: utf-8

from PIL import Image
from PIL import ImageEnhance
from PIL import ImageSequence
import qrcode


def color_replace(image, color):
    """Replace black with other color

    :color: custom color (r,g,b,a)
    :image: image to replace color
    :returns: TODO

    """
    pixels = image.load()
    size = image.size[0]
    for width in range(size):
        for height in range(size):
            r, g, b, a = pixels[width, height]
            if (r, g, b, a) == (0,0,0,255):
                pixels[width,height] = color
            else:
                pixels[width,height] = (r,g,b,color[3])

def produce(txt,img,ver=5,err_crt = qrcode.constants.ERROR_CORRECT_H,bri = 1.0, cont = 1.0,\
        colourful = False, rgba = (0,0,0,255),pixelate = False):
    """Produce QR code

    :txt: QR text
    :img: Image path / Image object
    :ver: QR version
    :err_crt: QR error correct
    :bri: Brightness enhance
    :cont: Contrast enhance
    :colourful: If colourful mode
    :rgba: color to replace black
    :pixelate: pixelate
    :returns: list of produced image

    """
    if type(img) is Image.Image:
        pass
    elif type(img) is str:
        img = Image.open(img)
    else:
        return []
    frames = [produce_impl(txt,frame.copy(),ver,err_crt,bri,cont,colourful,rgba,pixelate) for frame in ImageSequence.Iterator(img)]
    return frames

def produce_impl(txt,img,ver=5,err_crt = qrcode.constants.ERROR_CORRECT_H,bri = 1.0, cont = 1.0,\
        colourful = False, rgba = (0,0,0,255),pixelate = False):
    """Produce QR code

    :txt: QR text
    :img: Image object
    :ver: QR version
    :err_crt: QR error correct
    :bri: Brightness enhance
    :cont: Contrast enhance
    :colourful: If colourful mode
    :rgba: color to replace black
    :pixelate: pixelate
    :returns: Produced image

    """
    qr = qrcode.QRCode(version = ver,error_correction = err_crt,box_size=3)
    qr.add_data(txt)
    qr.make(fit=True)
    img_qr = qr.make_image().convert('RGBA')
    if colourful and ( rgba != (0,0,0,255) ):
        color_replace(img_qr,rgba)
    img_img = img.convert('RGBA')

    img_img_size = None
    img_size = img_qr.size[0] - 24
    if img_img.size[0] < img_img.size[1]:
        img_img_size = img_img.size[0]
    else:
        img_img_size = img_img.size[1]

    img_enh = img_img.crop((0,0,img_img_size,img_img_size))
    enh = ImageEnhance.Contrast(img_enh)
    img_enh = enh.enhance(cont)
    enh = ImageEnhance.Brightness(img_enh)
    img_enh = enh.enhance(bri)
    if not colourful:
        if pixelate:
            img_enh = img_enh.convert('1').convert('RGBA')
        else:
            img_enh = img_enh.convert('L').convert('RGBA')
    img_frame = img_qr
    img_enh = img_enh.resize((img_size*10,img_size*10))
    img_enh_l = img_enh.convert("L").resize((img_size,img_size))
    img_frame_l = img_frame.convert("L")

    for x in range(0,img_size):
        for y in range(0,img_size):
            if x < 24 and (y < 24 or y > img_size-25):
                continue
            if x > img_size-25 and (y < 24 ):
                continue
            if (x%3 ==1 and  y%3 == 1):
                if (img_frame_l.getpixel((x+12,y+12)) > 70 and img_enh_l.getpixel((x,y)) < 185)\
                        or (img_frame_l.getpixel((x+12,y+12)) < 185 and img_enh_l.getpixel((x,y)) > 70) :
                    continue
            img_frame.putpixel((x+12,y+12),(0,0,0,0))
    pos = qrcode.util.pattern_position(qr.version)
    img_qr2 = qr.make_image().convert("RGBA")
    if colourful and ( rgba != (0,0,0,0) ):
        color_replace(img_qr2,rgba)
    for i in pos:
        for j in pos:
            if (i == 6 and j == pos[-1]) or (j == 6 and i == pos[-1])\
                or (i == 6 and j == 6):
                continue
            else:
                rect = (3*(i-2)+12,3*(j-2)+12,3*(i+3)+12,3*(j+3)+12)
                img_tmp = img_qr2.crop(rect)
                img_frame.paste(img_tmp,rect)

    img_res = Image.new("RGBA",(img_frame.size[0]*10,img_frame.size[1]*10),(255,255,255,255))
    img_res.paste(img_enh,(120,120),img_enh)
    img_frame = img_frame.resize((img_frame.size[0]*10,img_frame.size[1]*10))
    img_res.paste(img_frame,(0,0),img_frame)
    img_res = img_res.convert('RGB')
    if pixelate:
        return img_res.resize(img_qr.size).resize((img_img_size,img_img_size))
    return img_res


def main():
    import argparse
    parser = argparse.ArgumentParser(description="Combine your QR code with custom picture")
    parser.add_argument("image")
    parser.add_argument("text", help="QRcode Text.")
    parser.add_argument("-o", "--output", help="Name of output file.")
    parser.add_argument("-v", "--version", type=int, help="QR version.In range of [1-40]")
    parser.add_argument("-e", "--errorcorrect", choices={"L","M","Q","H"}, help="Error correct")
    parser.add_argument("-b", "--brightness", type=float, help="Brightness enhance")
    parser.add_argument("-c", "--contrast", type=float, help="Contrast enhance")
    parser.add_argument("-C", "--colourful", action="store_true",help="colourful mode")
    parser.add_argument("-r", "--rgba", nargs=4, metavar=('R','G','B','A'),type = int, help="color to replace black")
    parser.add_argument("-p", "--pixelate", action="store_true",help="pixelate")
    args = parser.parse_args()

    img = args.image
    txt = args.text
    output = args.output if args.output else 'qr.png'
    ec = qrcode.constants.ERROR_CORRECT_H
    if args.errorcorrect:
        ec_raw = args.errorcorrect
        if ec_raw == 'L':
            ec = qrcode.constants.ERROR_CORRECT_L
        if ec_raw == 'M':
            ec = qrcode.constants.ERROR_CORRECT_M
        if ec_raw == 'Q':
            ec = qrcode.constants.ERROR_CORRECT_Q
    ver = 5
    if args.version:
        if args.version >= 1 and args.version <= 40:
            ver = args.version
    cont = args.contrast if args.contrast else 1.0
    bri = args.brightness if args.brightness else 1.0
    colr = True if args.colourful else False
    pixelate = True if args.pixelate else False
    if colr :
        if args.rgba:
          rgba = tuple(args.rgba)
        else:
            rgba = (0,0,0,255)
    else:
        rgba = (0,0,0,255)
    frames = produce(txt,img,ver,ec,bri, cont ,colourful = colr,rgba=rgba,pixelate = pixelate)
    if len(frames) == 1 or output.upper()[-3:] != "GIF":
        frames[0].save(output)
    elif len(frames) > 1:
        frames[0].save(output,save_all=True,append_images=frames[1:],duration=100,optimize=True)

if __name__ == "__main__":
    main()

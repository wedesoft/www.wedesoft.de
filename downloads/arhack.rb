#!/usr/bin/env ruby
# --- creating prep.avi and prep.wav ---
# TIME=300
# mencoder /home/engjw/Documents/mp3/podcasts/ruby/d2t1p8_keynote_640x240.avi -o prep.avi -ss 185 -endpos $TIME -vf crop=320:240:320:0 -ovc lavc -lavcopts vcodec=mpeg4:vhq:vbitrate=1000 -nosound
# mencoder /home/engjw/Documents/mp3/podcasts/ruby/d2t1p8_keynote_640x240.avi -o prep.mp3 -ss 185 -endpos $TIME -of rawaudio -oac mp3lame -ovc copy
# mpg123 -w prep.wav prep.mp3
# --- recording a demo ---
# http://carthick.wordpress.com/2007/11/26/linux-recording-soundcard-output-using-arecord/
# # Content of ~/.asoundrc
# pcm.copy {
# type plug
# slave {
# pcm hw
# }
# route_policy copy
# }
# # arecord -d 300 -c 2 -f S16_LE -r 44100 -t wav -D copy test.wav
# # mplayer test.avi -audiofile test.wav -delay 0.3
# # mencoder test.avi -audiofile test.wav -delay 0.3 -o hologram.avi -ovc lavc -lavcopts vcodec=msmpeg4:vhq:vbitrate=1000 -oac mp3lame -lameopts q=2
require 'hornetseye' # version 0.33
require 'matrix'
require 'matrix_fix' # http://www.wedesoft.demon.co.uk/downloads/matrix_fix.rb
require 'linalg'
require 'RUDL'
include Hornetseye
include Linalg
include RUDL
class Matrix
  def to_dmatrix
    DMatrix[ *to_a ]
  end
  def svd
    to_dmatrix.svd.collect { |m| m.to_matrix }
  end
end
class Vector
  def norm
    Math.sqrt inner_product( self )
  end
  def normalise
    self * ( 1.0 / norm )
  end
  def reshape( *shape )
    Matrix[ *MultiArray[ *self ].reshape( *shape ).to_a ]
  end
  def to_sequence
    Sequence[ *to_a ]
  end
  def x( other )
    Vector[ self[1] * other[2] - self[2] * other[1],
            self[2] * other[0] - self[0] * other[2],
            self[0] * other[1] - self[1] * other[0] ] *
      ( 2.0 / ( norm + other.norm ) )
  end
end
class DMatrix
  def to_matrix
    Matrix[ *to_a ]
  end
end
class MultiArray
  class << self
    def complex_ramp( w, h )
      idx = int( w, h ).indgen!
      retval = MultiArray( COMPLEX( INT ), w, h ).new
      retval.real = idx % w
      retval.imag = idx / w
      retval
    end
    def gauss_gradient_filter( sigma, angle, max_error = 1.0 / 0x100 )
      dx = MultiArray.tensor( 2 ) do |i,j|
        Sequence.gauss_gradient_filter( sigma, max_error )[ i ] *
          Sequence.gauss_blur_filter( sigma, max_error )[ j ]
      end
      dy = MultiArray.tensor( 2 ) do |i,j|
        Sequence.gauss_blur_filter( sigma, max_error )[ i ] *
          Sequence.gauss_gradient_filter( sigma, max_error )[ j ]
      end
      Math.cos( angle ) * dx + Math.sin( angle ) * dy
    end
  end
end
class Sequence
  class << self
    def gauss_blur_filter( sigma, max_error = 1.0 / 0x100 )
      Sequence[ *Array.gauss_blur_filter( sigma, max_error ) ]
    end
    def gauss_gradient_filter( sigma, max_error = 1.0 / 0x100 )
      Sequence[ *Array.gauss_gradient_filter( sigma, max_error ) ]
    end
  end
end
class Sequence_
  def to_matrix
    Matrix[ *to_a ]
  end
  def otsu( hist_size = 256 )
    h = hist hist_size
    i = Sequence.uint( hist_size ).indgen!
    w1 = h.integral
    w2 = w1[ w1.size - 1 ] - w1
    s1 = ( h * i ).integral
    s2 = sum - s1
    m1 = w1 > 0
    u1 = ( s1.mask( m1 ).to_sfloat / w1.mask( m1 ) ).unmask m1
    m2 = w2 > 0
    u2 = ( s2.mask( m2 ).to_sfloat / w2.mask( m2 ) ).unmask m2
    between_variance = ( u1 - u2 ) ** 2 * w1 * w2
    max_between_variance = between_variance.max
    self > i.mask( between_variance >= max_between_variance )[0]
  end
  def warp_clipped_with_complex( field, default = typecode.default )
    if field.typecode < COMPLEX_
      field = field.typecast field.typecode.scalar, 2, *field.shape
    end
    warp_clipped_without_complex field, default
  end
  alias_method_chain :warp_clipped, :complex
  def rectangle( component, ramp, patch, cgrad, n_arg, arg_noise,
                 f, s )
    edge = component.dilate.and component.erode.not
    x = ramp.mask edge
    warp = MultiArray.tensor( 3 ) { |i,j,k| x[ k ] + patch[ i, j ] }
    regions = warp_clipped warp, 128
    d = Sequence.tensor( 1 ) { |k,i,j| regions[k][i,j] * cgrad[i,j] }
    d_q = ( ( d.arg / Math::PI + 1 ) * n_arg / 2 ).to_int % n_arg
    d_hist = d_q.hist_weighted n_arg, d.abs
    msk = d_hist >= d_hist.sum / ( 4.0 * arg_noise )
    segments = msk.components
    if msk[ 0 ] and msk[ n_arg - 1 ]
      segments = segments.eq( segments.max ).conditional 1, segments
    end
    if segments.max == 4
      partitions = d_q.map segments
      weights = partitions.hist( 5 ).major 1.0
      c = partitions.hist_weighted( 5, x ) / weights
      d = x - partitions.map( c )
      a = partitions.hist_weighted( 5, d ** 2 ) ** 0.5
      m = Sequence[ Complex( -1, -1 ), Complex(  1, -1 ),
                    Complex(  1,  1 ), Complex( -1,  1 ) ] * s / 2
      ms = Sequence[ *( 0 ... 4 ).collect do |i|
        i1, i2 = i + 1, ( i + 1 ) % 4 + 1
        l1, a1, l2, a2 = c[i1], a[i1], c[i2], a[i2]
        ( l1 * a1.conj * a2 - l2 * a1 * a2.conj -
          l1.conj * a1 * a2 + l2.conj * a1 * a2 ) /
          ( a1.conj * a2 - a1 * a2.conj )
      end ] - Complex( shape[0], shape[1] ) / 2
      constraints = []
      for i in 0 ... 4 do
        constraints.push [ m[i].real, m[i].imag, 1.0, 0.0, 0.0, 0.0,
                -ms[i].real * m[i].real, -ms[i].real * m[i].imag, -ms[i].real ]
        constraints.push [ 0.0, 0.0, 0.0, m[i].real, m[i].imag, 1.0,
                -ms[i].imag * m[i].real, -ms[i].imag * m[i].imag, -ms[i].imag ]
      end
      intr = Matrix[ [ f, 0.0, 0.0 ], [ 0.0, f, 0.0 ], [ 0.0, 0.0, 1.0 ] ]
      h = Matrix[ *constraints ].svd[ 2 ].row( 8 ).reshape 3, 3
      rt = intr.inv * h
      extr = MultiArray.object 4, 4
      scale = 0.5 * ( rt.column( 0 ).norm + rt.column( 1 ).norm )
      e1 = rt.column( 0 ).normalise
      e2 = rt.column( 1 ).normalise
      e3 = e1.x e2
      if e3.inner_product( rt.column( 2 ) ) > 0
        e1, e2, e3 = e2 * -1, e1, e3 * -1
      end
      extr[ 0, 0 .. 2 ] = e1.to_sequence
      extr[ 1, 0 .. 2 ] = e2.to_sequence
      extr[ 2, 0 .. 2 ] = e3.to_sequence
      extr[ 3, 0 .. 2 ] = rt.column( 2 ).to_sequence / scale
      extr[ 0 .. 2, 3 ] = 0.0
      extr[ 3, 3 ] = 1.0
      extr = extr.to_matrix
      extr
    else
      nil
    end
  end
end
holo_first = Sound.new 'clicking.wav'
holo_first.volume = 1.0
holo_in = Sound.new 'holoin.wav'
holo_out = Sound.new 'holoout.wav'
holo_op = Dir.glob( 'holoop*.wav' ).collect { |f| Sound.new f }
holo_in.volume = 0.5
holo_out.volume = 0.8
holo_op.each { |op| op.volume = 0.4 }
# input = XineInput.new 'ar.avi'
input = V4L2Input.new '/dev/video0', 320, 240
# input = DC1394Input.new '', 0, 0, DC1394Input::FORMAT_VGA_NONCOMPRESSED, DC1394Input::MODE_320x240_YUV422
overlay = XineInput.new 'prep.avi', false
audio = Sound.new 'prep.wav'
# 00:03:01, 00:07:00
audio.play
audio.volume = 0.0
fade_time = Time.new.to_f
fade_in = 1.3
op_delay = 10.0
op_index = 0
shape = [ overlay.width, overlay.height ]
scale = [ 0.05 / shape[0], 0.05 / shape[1] ]
rate = 25
# video = MEncoderOutput.new 'test.avi', rate
time = Time.new.to_f
display = X11Display.new
output = XVideoOutput.new
window = X11Window.new display, output, 960, 720
window.title = 'Hornetseye: Ruby computer vision extension'
window.show
threshold = 80
n_arg = 36
arg_noise = 3
track_min = 0.1
track_max = 1.4
range = ( input.width / 30 ) ** 2 .. ( input.width / 3 ) ** 2
# f = 1.45 * input.width # focal length divided by pixel size
f = 1.05 * input.width # focal length divided by pixel size
s = 0.05 # Size of object.
r = 0.061 - 0.025
grad_x = MultiArray.gauss_gradient_filter 1.2, 0.0 * Math::PI, 0.1
grad_y = MultiArray.gauss_gradient_filter 1.2, 0.5 * Math::PI, 0.1
cgrad = MultiArray.dcomplex *grad_x.shape
cgrad.real = grad_x
cgrad.imag = grad_y
ramp = MultiArray.complex_ramp input.width, input.height
patch = MultiArray.complex_ramp( 5, 5 ) - Complex( 2, 2 )
found, extr = nil, nil
2.times { input.read }
previous = input.read_ubytergb
holo_first.play 0
while display.status?
  img = input.read
  colour = img.to_ubytergb
  grey = img.to_ubyte
  components = ( grey <= threshold ).components
  # components = grey.otsu.components true
  n = components.max + 1
  if found.nil?
    hist = components.hist n
    mask = hist.between? range.min, range.max
    Sequence.int( n ).indgen!.mask( mask ).to_a.each do |c|
      component = components.eq c
      extr = grey.rectangle component, ramp, patch, cgrad,
                            n_arg, arg_noise, f, s
      if extr
        found = component
        break
      end
    end
    if found
      holo_out.stop
      holo_in.play 0
      fade_time = Time.new.to_f
      op_time = fade_time + 2.0 + rand( op_delay )
    end
  else
    hist = components.mask( found ).hist n
    found_size = hist.sum
    mask = hist.eq hist.max
    select = Sequence.int( n ).indgen!.mask( mask )[ 0 ]
    update = components.eq select
    update_size = update.to_ubyte.sum
    if hist.max >= track_min * found_size and
        update_size <= track_max * found_size and
        range.include? update_size
      extr = grey.rectangle update, ramp, patch, cgrad,
                            n_arg, arg_noise, f, s
      if extr
        found = update
      else
        found = nil
      end
    else
      found = nil
    end
    unless found
      holo_in.stop
      holo_out.play 0
      audio.volume = 0.0
    end
  end
  if found
    min_value = 255
    rotation = 0.0
    for i in 0 ... 4
      case i
      when 0
        v = extr * Vector[  r,        0.0, 0.0, 1.0 ]
      when 1
        v = extr * Vector[        0.0,  r, 0.0, 1.0 ]
      when 2
        v = extr * Vector[ -r,        0.0, 0.0, 1.0 ]
      when 3
        v = extr * Vector[        0.0, -r, 0.0, 1.0 ]
      end
      xs = ( v[0] / v[2] * f + colour.shape[0] / 2 ).to_i
      ys = ( v[1] / v[2] * f + colour.shape[1] / 2 ).to_i
      if ( 0 .. input.width - 1 ).include?( xs ) and
          ( 0 .. input.height - 1 ).include?( ys )
        value = colour[ xs.to_i, ys.to_i ]
        if value < min_value
          rotation = i * Math::PI / 2
          min_value = value
        end
      end
    end
    cr, sr = Math.cos( rotation ), Math.sin( rotation )
    intr = Matrix[ [ f, 0.0, 0.0 ], [ 0.0, f, 0.0 ], [ 0.0, 0.0, 1.0 ] ]
    extr = extr * Matrix[ [  cr, -sr, 0.0, 0.0 ],
                          [  sr,  cr, 0.0, 0.0 ],
                          [ 0.0, 0.0, 1.0, 0.0 ],
                          [ 0.0, 0.0, 0.0, 1.0 ] ]
    rt = Matrix[ [ extr[0,0], extr[0,2], extr[0,3] ],
                 [ extr[1,0], extr[1,2], extr[1,3] ],
                 [ extr[2,0], extr[2,2], extr[2,3] ] ]
    hi = ( intr * rt ).inv
    xs, ys, zs = ramp.real - input.width / 2, ramp.imag - input.height / 2, 1.0
    x = hi[0,0] * xs + hi[0,1] * ys + hi[0,2] * zs
    y = hi[1,0] * xs + hi[1,1] * ys + hi[1,2] * zs
    z = hi[2,0] * xs + hi[2,1] * ys + hi[2,2] * zs
    warp = MultiArray.sfloat 2, input.width, input.height
    warp.roll[0] = shape[0] / 2 - x / z / scale[0]
    warp.roll[1] = shape[1] - y / z / scale[1]
    hologram = overlay.read_sint
    if op_time < Time.new.to_f
      holo_op[ op_index ].play
      op_index = ( op_index + 1 ) % holo_op.size
      fade_time = Time.new.to_f
      op_time = fade_time + 2.0 + rand( op_delay )
    end
    fade = ( fade_time + fade_in - Time.new.to_f ) / fade_in
    if fade > 0
      hologram += MultiArray.sint( *shape ).random!( fade * 256 ) - 128 * fade
      # audio.volume = 1.0 - fade
    else
      audio.volume = 1.0
    end
    colour = ( 0.8 * colour +
               RGB( 0.0, 0.6, 0.0 ) * hologram.warp_clipped( warp ) ).clip
  else
    colour = 0.8 * colour
  end
  n_time = Time.new.to_f
  while time <= n_time
    # video.write previous
    time += 1.0 / rate
  end
  # time += Time.new.to_f - n_time
  previous = colour
  output.write colour
  display.processEvents
end

# update slides
# n-dimensional histogram, simplify line-fit example

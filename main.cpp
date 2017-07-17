#include <iostream>
#include <algorithm>
#include "lame.h"
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif


void set_mp3_input_attribute(lame_global_flags* gfp) {
// number of samples.  default = 2^32-1
//  lame_set_num_samples(gfp, );
// input sample rate in Hz.  default = 44100hz
  lame_set_in_samplerate(gfp, 16000);
  lame_set_num_channels(gfp, 1);
}
void set_mp3_output_attribute(lame_global_flags* gfp) {
  lame_set_out_samplerate(gfp, 8000);
  lame_set_brate(gfp,128);
  lame_set_mode(gfp,MONO);
  lame_set_quality(gfp,1);   /* 2=high  5 = medium  7=low */ 
  lame_set_write_id3tag_automatic(gfp, 0);
}

int main(int argc, char *argv[]) {
   lame_global_flags *gf;
   gf = lame_init();
   unsigned char mp3buffer[LAME_MAXMP3BUFFER];
//     set gf.samplerate, gf.num_channels, gf.num_samples.

    lame_set_write_id3tag_automatic(gf, 0);
    FILE* stream=fopen("./lalala.pcm","rb");
    FILE* streamout=fopen("./lalala.mp3","w+b");
    short int buffer[999999];
    size_t read_size = fread((void*)buffer,2,999999,stream);
    lame_set_num_samples(gf,65535);
    set_mp3_input_attribute(gf);
    set_mp3_output_attribute(gf);
    int ret = lame_init_params(gf);
    if (ret < 0) {
        std::cerr<<"fatal error during initialization"<<std::endl;
        return ret;
    }
    std::cout<<read_size <<std::endl;
    std::cout<<"num_sample is "<<lame_get_num_samples(gf)<<std::endl;
    std::cout<<"sample_rate is "<<lame_get_in_samplerate(gf)<<std::endl;
    std::cout<<"channel is "<<lame_get_num_channels(gf)<<std::endl;
    size_t step_size = 115200;
    for (int i = 0; i < read_size; i +=step_size ) {
    int mp3_input_size = std::min(read_size - i,step_size );
    int imp3 = lame_encode_buffer(gf, buffer+ i, buffer+ i, mp3_input_size , mp3buffer, sizeof(mp3buffer));
    for (int j=0;j<10;j++){
	    std::cout<<(buffer+i)[j]<<"|";
    }
    std::cout<<std::endl;
    std::cout<<"mp3 length:"<<imp3<<std::endl;
    std::cout<<"pcm not used: "<<lame_get_mf_samples_to_encode(gf)<<std::endl;
    std::cout<<"frame so far: "<<lame_get_frameNum(gf)<<std::endl;
    fwrite(mp3buffer, 1, imp3, streamout);
    }
    int imp3 = lame_encode_flush(gf, mp3buffer, sizeof(mp3buffer)); /* may return one more mp3 frame */
    std::cout<<"mp3 final length:"<<imp3<<std::endl;
    fwrite(mp3buffer, 1, imp3, streamout);
    fclose(stream);
    fclose(streamout);
    lame_close (gf);
}

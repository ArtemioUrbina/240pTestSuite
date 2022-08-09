#include <yaul.h>

//#include <assert.h>
#include <stdlib.h>

#include "svin.h"
#include "svin_textbox.h"

#include <mcufont.h>
#define UNUSED(x) (void)(x)

uint32_t JumpLinks[512];

void 
_svin_script_run(char * filename)
{
    char * script_buffer;
    char * tmp_buffer;
    char * tmp_buffer2;
    bool bFinished = false;
    int i,j,k;
    int iActor;
    int iActorColor;
    int iLayer;
    int iPosition;
    int iPalette;
    int iJump;
    int iJumpAddress;
    bool bItalics = false;
    
    //reading links
    fad_t _jumplinks_fad;
    bool b =  _svin_filelist_search(filename,&_jumplinks_fad,&i);
    assert(true == b);
    _svin_cd_block_sector_read(_jumplinks_fad, (uint8_t*)JumpLinks);

    //first let's find script FAD, browsing root folder
    //-------------- Getting FAD and index for background pack binary -------------------
    fad_t _script_fad;
    fad_t _base_fad;
    b = _svin_filelist_search(filename,&_script_fad,&i)
    assert(true == b);
    _base_fad = _script_fad;

    script_buffer = malloc(4096);
    tmp_buffer = malloc(2048);
    tmp_buffer2 = malloc(2048);
    _svin_cd_block_sector_read(_script_fad, (uint8_t*)script_buffer);
    _script_fad++;
    int iDataInBuffer=2048;
    //starting parse cycle 
    bFinished = false;

    char * pDebug = (char *)_svin_alloc_lwram(0x40000,0x20240000);
    int iDebugStringNumber = 0;
    strcpy(pDebug,"start 1");
    
    _svin_textbox_clear();
    while (false == bFinished)
    {
        if (script_buffer[0] == 0x0D)
        {
            //stupid miss
            //remove byte from buffer
            for (j=1;j<4096;j++)
                script_buffer[j-1] = script_buffer[j];
            iDataInBuffer -= 1;
        }
        if (strncmp(script_buffer,"TEXT ",5)==0)
        {
            //print text on panel
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            //getting actor number
            j=4;
            while (strncmp(&(script_buffer[j]),"ACTOR=",6)!=0)
                j++;
            j+=6;
            iActor=0;
            while (script_buffer[j]!=' ')
            {
                iActor *= 10;
                iActor += script_buffer[j]-'0';
                j++;
            }
            bItalics = false;
            switch (iActor)
            {
                case 255: 
                    strcpy(tmp_buffer2,"");//narrative, no actor name
                    iActorColor = 7;
                    break;
                case 254: 
                    strcpy(tmp_buffer2,""); //thinking, using italics font
                    iActorColor = 7;
                    bItalics = true;
                    break;
                /*case 2: 
                    strcpy(tmp_buffer2,"<me>");
                    iActorColor = 7;
                    break;
                case 3: 
                    strcpy(tmp_buffer2,"Славя");
                    iActorColor = 6;
                    break;
                case 4: 
                    strcpy(tmp_buffer2,"Лена");
                    iActorColor = 0;
                    break;
                case 5: 
                    strcpy(tmp_buffer2,"Алиса");
                    iActorColor = 2;
                    break;*/
                default:
                    sprintf(tmp_buffer2,"<actor%i>",iActor);
                    iActorColor = iActor%14;
                    break;
            }
            //moving on to text
            while (script_buffer[j]!='"')
                j++;
            j++; //skipping colon
            k=0; 
            while (script_buffer[j]!='"')
            {
                tmp_buffer[k] = script_buffer[j];
                j++;
                k++;
            }
            tmp_buffer[k] = 0;
            if (true == bItalics)
                _svin_textbox_print(tmp_buffer2,tmp_buffer,"Lato_BlackItalic15",iActorColor,iActorColor);
            else
                _svin_textbox_print(tmp_buffer2,tmp_buffer,"Lato_Black15",iActorColor,iActorColor);
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"TEXT fad %i",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
            //wait for keypress
            _svin_wait_for_key_press_and_release();
        }
        else if (strncmp(script_buffer,"BG ",3)==0)
        {
            //temporary measures - removing all sprites when changing BG
            for (i=0;i<3;i++)
                _svin_sprite_clear(i);
            //temporary measures - purging sprites cache when changing BG
            _svin_sprite_cache_purge_all();

            //set bg
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            for (j=3;j<i;j++)
                tmp_buffer[j-3] = script_buffer[j];
            tmp_buffer[i-3]=0;
            _svin_background_update(tmp_buffer);
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"BG fad %i",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"REM ",4)==0)
        {
            //comment
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"REM fad %i",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"SPRITE ",7)==0)
        {
            //draw sprite
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            j = (int)strstr(script_buffer,"LAYER ") - (int)script_buffer;
            j+=6;
            iLayer=atoi(&script_buffer[j]);
            if (iLayer<0) iLayer = 0;
            if (iLayer>2) iLayer = 2;
            j = (int)strstr(script_buffer,"POSITION ") - (int)script_buffer;
            j+=9;
            iPosition=atoi(&script_buffer[j]);
            if (iPosition<0) iPosition = 0;
            if (iPosition>2) iPosition = 2;
            j = (int)strstr(script_buffer,"PALETTE ") - (int)script_buffer;
            j+=8;
            iPalette=atoi(&script_buffer[j]);
            if (iPalette<0) iPalette = 0;
            if (iPalette>2) iPalette = 2;
            j = (int)strstr(script_buffer,"FILE ") - (int)script_buffer;
            j+=5;
            memcpy(tmp_buffer,&(script_buffer[j]),i-j);
            tmp_buffer[i-j]=0;
            _svin_sprite_draw(tmp_buffer,iLayer,iPosition,iPalette);

            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"SPpos%ilay%ipal%i",iPosition,iLayer,iPalette);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"CLEAR ",6)==0)
        {
            //draw sprite
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            j = (int)strstr(script_buffer,"POSITION ") - (int)script_buffer;
            j+=9;
            iPosition=atoi(&script_buffer[j]);
            if (iPosition<0) iPosition = 0;
            if (iPosition>2) iPosition = 2;
            _svin_sprite_clear(iPosition);
           
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"CLEAR fad %i",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"ENABLE ",6)==0)
        {
            //draw sprite
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            j = (int)strstr(script_buffer,"POSITION ") - (int)script_buffer;
            j+=9;
            iPosition=atoi(&script_buffer[j]);
            if (iPosition<0) iPosition = 0;
            if (iPosition>2) iPosition = 2;
            _svin_set_cycle_patterns_nbg();//position ignored

            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"ENBfad%ipos%i",_script_fad-_base_fad,iPosition);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"END",3)==0)
        {
            //end of script
            bFinished = true;
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"END fad %i",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
        }
        else if (strncmp(script_buffer,"MENU ",5)==0)
        {
            //populate menu
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;

            //getting jump pointer
            j=5;
            while (strncmp(&(script_buffer[j]),"JUMP=",5)!=0)
                j++;
            j+=5;
            iJump=0;
            while (script_buffer[j]!=' ')
            {
                iJump *= 10;
                iJump += script_buffer[j]-'0';
                j++;
            }
            //moving on to text
            while (script_buffer[j]!='"')
                j++;
            j++; //skipping colon
            k=0; 
            while (script_buffer[j]!='"')
            {
                tmp_buffer[k] = script_buffer[j];
                j++;
                k++;
            }
            tmp_buffer[k] = 0;
            _svin_menu_populate(iJump,tmp_buffer);
           
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"MNUfad%ijmp%i",_script_fad-_base_fad,iJump);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        else if (strncmp(script_buffer,"MENURUN",7)==0)
        {
            //disable textbox
            _svin_textbox_disable();
            //activate menu
            iJumpAddress = _svin_menu_activate();
            //re-init textbox
            _svin_textbox_init();
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"MRfd%iad%i",_script_fad-_base_fad,iJumpAddress);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //jump means we go away to a different position, so we don't care about buffer anymore
            _script_fad = _base_fad + iJumpAddress/2048;
            iDataInBuffer = 0;
            //reading only one sector, second one will be readed at the end of cycle
            _svin_cd_block_sector_read(_script_fad, (uint8_t*)tmp_buffer2);
            _script_fad++;
            for (j=iJumpAddress%2048;j<2048;j++)
            {
                script_buffer[j-iJumpAddress%2048] = tmp_buffer2[j];
                iDataInBuffer++;
            }           
        }
        else if (strncmp(script_buffer,"JUMP ",5)==0)
        {
            //jump
            j = 4;
            while (script_buffer[j] == ' ')
                j++;
            iJump=atoi(&script_buffer[j]);
            assert (iJump >= 0);
            assert (iJump < 512);
            //jump means we go away to a different position, so we don't care about buffer anymore
            iJumpAddress = JumpLinks[iJump];
            _script_fad = _base_fad + iJumpAddress/2048;
            iDataInBuffer = 0;
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"JPfd%iad%i",_script_fad-_base_fad,iJumpAddress);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //reading only one sector, second one will be readed at the end of cycle
            _svin_cd_block_sector_read(_script_fad, (uint8_t*)tmp_buffer2);
            _script_fad++;
            for (j=iJumpAddress%2048;j<2048;j++)
            {
                script_buffer[j-iJumpAddress%2048] = tmp_buffer2[j];
                iDataInBuffer++;
            }           
        }
        else
        {
            i = (int)strchr(script_buffer,'\r') - (int)script_buffer;
            if (i>2048) i=2048;
            if (i<4) i=4;
            //write debug
            sprintf(&pDebug[iDebugStringNumber*32],"UNKN fad %i ",_script_fad-_base_fad);
            for (int k=0;k<16;k++) pDebug[iDebugStringNumber*32+16+k] = script_buffer[k];
            iDebugStringNumber++;
            //remove command from buffer
            for (j=i+1;j<4096;j++)
                script_buffer[j-i-1] = script_buffer[j];
            iDataInBuffer -= (i+1);
        }
        //should we load more data?
        if (iDataInBuffer<2048)
        {
            _svin_cd_block_sector_read(_script_fad, (uint8_t*)tmp_buffer2);
            _script_fad++;
            for (j=0;j<2048;j++)
            {
                script_buffer[iDataInBuffer] = tmp_buffer2[j];
                iDataInBuffer++;
            }
        }
    }   
}
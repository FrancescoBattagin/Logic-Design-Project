#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void coordinate(void);
void mask_o(void);
void mask_i(void);
void calc_mask(void);

int i, xp, yp, xc, yc, dist_min, dist, coord[16], in_mask, out_mask, o_masks[999], i_masks[999];
char i_mask[9], o_mask[9];

int main(){
    FILE *fp = NULL;
    fp = fopen("/Users/mac/Desktop/10521394/tb.txt", "w");

    if(fp){
        srand(time(NULL));
        for (i = 0; i < 1000; i++){
            //printf("\n%d\n", i+1);
            coordinate();
            xp=rand()%256;
            yp=rand()%256;
            dist_min=511;
            dist=0;
            in_mask=rand()%255;
            mask_i();
            i_masks[i]=in_mask;
            calc_mask();
            mask_o();
            o_masks[i]=out_mask;

            fprintf(fp, "--test # %d\n\n", i+1);
            fprintf(fp, "library ieee;\nuse ieee.std_logic_1164.all;\nuse ieee.numeric_std.all;\nuse ieee.std_logic_unsigned.all;\n");
            fprintf(fp, "entity project_tb is\nend project_tb;\n");
            fprintf(fp, "architecture projecttb of project_tb is\n");
            fprintf(fp, "constant c_CLOCK_PERIOD\t\t: time := 100 ns;\n");
            fprintf(fp, "signal   tb_done\t\t\t\t: std_logic;\n");
            fprintf(fp, "signal   mem_address\t\t\t: std_logic_vector (15 downto 0) := (others => '0');\n");
            fprintf(fp, "signal   tb_rst	\t\t\t\t: std_logic := '0';\n");
            fprintf(fp, "signal   tb_start\t\t\t\t: std_logic := '0';\n");
            fprintf(fp, "signal   tb_clk	\t\t\t\t: std_logic := '0';\n");
            fprintf(fp, "signal   mem_o_data,mem_i_data	: std_logic_vector (7 downto 0);\n");
            fprintf(fp, "signal   enable_wire\t\t\t: std_logic;\n");
            fprintf(fp, "signal   mem_we	\t\t\t\t: std_logic;\n");
            fprintf(fp, "type ram_type is array (65535 downto 0) of std_logic_vector(7 downto 0);\n\n");
            
            fprintf(fp, "signal RAM: ram_type := (0 => std_logic_vector(to_unsigned( %d , 8)),\n", in_mask);
            fprintf(fp, "\t\t\t\t 1 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[0]);
            fprintf(fp, "\t\t\t\t 2 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[1]);
            fprintf(fp, "\t\t\t\t 3 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[2]);
            fprintf(fp, "\t\t\t\t 4 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[3]);
            fprintf(fp, "\t\t\t\t 5 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[4]);
            fprintf(fp, "\t\t\t\t 6 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[5]);
            fprintf(fp, "\t\t\t\t 7 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[6]);
            fprintf(fp, "\t\t\t\t 8 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[7]);
            fprintf(fp, "\t\t\t\t 9 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[8]);
            fprintf(fp, "\t\t\t\t 10 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[9]);
            fprintf(fp, "\t\t\t\t 11 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[10]);
            fprintf(fp, "\t\t\t\t 12 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[11]);
            fprintf(fp, "\t\t\t\t 13 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[12]);
            fprintf(fp, "\t\t\t\t 14 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[13]);
            fprintf(fp, "\t\t\t\t 15 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[14]);
            fprintf(fp, "\t\t\t\t 16 => std_logic_vector(to_unsigned( %d , 8)),\n", coord[15]);
            fprintf(fp, "\t\t\t\t 17 => std_logic_vector(to_unsigned( %d , 8)),\n", xp);
            fprintf(fp, "\t\t\t\t 18 => std_logic_vector(to_unsigned( %d , 8)),\n", yp);
            fprintf(fp, "\t\t\t others => (others =>'0'));\n\n");
            fprintf(fp, "signal count: integer := 0;\n\n");

            fprintf(fp, "component project_reti_logiche is\n");
            fprintf(fp, "port (\n");
            fprintf(fp, "\t\ti_clk\t\t\t: in std_logic;\n");
            fprintf(fp, "\t\ti_start\t\t\t: in std_logic;\n");
            fprintf(fp, "\t\ti_rst\t\t\t: in std_logic;\n");
            fprintf(fp, "\t\ti_data\t\t\t: in std_logic_vector(7 downto 0);\n");
            fprintf(fp, "\t\to_address\t\t: out std_logic_vector(15 downto 0);\n");      
            fprintf(fp, "\t\to_done\t\t\t: out std_logic;\n");
            fprintf(fp, "\t\to_en\t\t\t: out std_logic;\n");
            fprintf(fp, "\t\to_we\t\t\t: out std_logic;\n");
            fprintf(fp, "\t\to_data\t\t\t: out std_logic_vector (7 downto 0)\n");
            fprintf(fp, "\t\t);\n");
            fprintf(fp, "end component project_reti_logiche;\n\n");

            fprintf(fp, "begin\n");
            fprintf(fp, "UUT: project_reti_logiche\n");
            fprintf(fp, "port map (\n");
            fprintf(fp, "\t\t\ti_clk\t\t\t=> tb_clk,\n");
            fprintf(fp, "\t\t\ti_start\t\t\t=> tb_start,\n");
            fprintf(fp, "\t\t\ti_rst\t\t\t=> tb_rst,\n");
            fprintf(fp, "\t\t\ti_data\t\t\t=> mem_o_data,\n");
            fprintf(fp, "\t\t\to_address\t\t=> mem_address,\n");
            fprintf(fp, "\t\t\to_done\t\t\t=> tb_done,\n");
            fprintf(fp, "\t\t\to_en\t\t\t=> enable_wire,\n");
            fprintf(fp, "\t\t\to_we\t\t\t=> mem_we,\n");
            fprintf(fp, "\t\t\to_data\t\t\t=> mem_i_data\n");
            fprintf(fp, "\t\t\t);\n\n");
            fprintf(fp, "p_CLK_GEN : process is\n");
            fprintf(fp, "begin\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD/2;\n");
            fprintf(fp, "\ttb_clk <= not tb_clk;\n");
            fprintf(fp, "end process p_CLK_GEN;\n\n");

            fprintf(fp, "MEM : process(tb_clk)\n");
            fprintf(fp, "begin\n");
            fprintf(fp, "\tif tb_clk'event and tb_clk='1' then\n");
            fprintf(fp, "\t\tif enable_wire='1' then\n");
            fprintf(fp, "\t\t\tif mem_we='1' then\n");
            fprintf(fp, "\t\t\t\tRAM(conv_integer(mem_address)) <= mem_i_data;\n");
            fprintf(fp, "\t\t\t\tmem_o_data <= mem_i_data after 2 ns;\n");
            fprintf(fp, "\t\t\telse\n");
            
            fprintf(fp, "\t\t\t\tmem_o_data <= RAM(conv_integer(mem_address)) after 2 ns;\n");
            fprintf(fp, "\t\t\tend if;\n");
            fprintf(fp, "\t\tend if;\n");
            fprintf(fp, "\tend if;\n");
            fprintf(fp, "end process;\n\n");
            fprintf(fp, "test : process is\n");
            fprintf(fp, "begin\n");
            
            fprintf(fp, "\twait for 100 ns;\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD;\n");
            fprintf(fp, "\ttb_rst <= '1';\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD;\n");
            fprintf(fp, "\ttb_rst <= '0';\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD;\n");
            fprintf(fp, "\ttb_start <= '1';\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD;\n");
            fprintf(fp, "\twait until tb_done = '1';\n");
            fprintf(fp, "\twait for c_CLOCK_PERIOD;\n");
            fprintf(fp, "\ttb_start <= '0';\n");
            fprintf(fp, "\twait until tb_done = '0';\n\n");
            if(i_masks[i])
                fprintf(fp, "\tassert RAM(19) = std_logic_vector(to_unsigned( %d , 8)) report \"TEST FALLITO\" severity failure;\n\n", o_masks[i]);
            else
                fprintf(fp, "\tassert RAM(19) = std_logic_vector(to_unsigned( 0 , 8)) report \"TEST FALLITO\" severity failure;\n\n");
                                
            fprintf(fp, "\nassert false report \"Simulation Ended!, TEST PASSATO\" severity failure;\n");
            fprintf(fp, "end process test;\n\n");
            fprintf(fp, "end projecttb;\n\n\n\n");
        }    
        fclose(fp);
    }else
        printf("error creating file\n");
    
    return 0;
}

void coordinate(){
    int j, k, t;    
    for(j=0; j<16; j++){
        k = rand()%2;
        //printf("j: %d\n", j);
        if (j > 1 && j <= 14 && k && j%2==0 && coord[j-1]!=0){
            if (coord[j-1] < 255 - coord[j-2])
                t = rand()%(coord[j-1]);
            else
                t = rand()%(256 - coord[j-2]);
            coord[j] = coord[j-2]+t;
            coord[j+1] = coord[j-1]-t;
            //printf("cambio: %d %d\t%d %d\n", coord[j-2], coord[j-1], coord[j], coord[j+1]);
            //printf("%d\n", coord[j]);
            //printf("%d\n", coord[j+1]);
            j++;
        }else{
            coord[j]=rand()%256;
            //printf("%d\n", coord[j]);
        }
    }
    return ;
}

void mask_i(){
    int n=in_mask, j=7;

    if(n>=128)
        i_mask[0]='1';
    i_mask[8]='\0';
    while(n>1){
        if(n%2 == 0){
            i_mask[j]='0';
        }
        else{
            i_mask[j]='1';
        }
        n/=2;
        j--;
    }

    if(j>=1){
        i_mask[j]='1';
        j--;
        while(j>=0){
            i_mask[j]='0';
            j--;
        }
    }
}

void calc_mask(){
    int j, k;

    for(j=0, k=0; j<16; j+=2, k++){
        if(i_mask[7-k]=='1'){
            dist=abs(xp-coord[j])+abs(yp-coord[j+1]);
            if(dist<dist_min){
                dist_min=dist;
                out_mask=pow(2, k);
            }else if(dist==dist_min){
                out_mask+=pow(2, k);
                //printf("uguale\n");
            }
        }
    }
    return ;
}

void mask_o(){
    int n=out_mask, j=7;

    o_mask[8]='\0';
    if(n>=128)
        o_mask[0]='1';
    while(n>1){
        if(n%2 == 0){
            o_mask[j]='0';
        }
        else{
            o_mask[j]='1';
        }
        n/=2;
        j--;
    }
    if(j>=1){
        o_mask[j]='1';
        j--;
        while(j>=0){
            o_mask[j]='0';
            j--;
        }
    }
    return ;
}

library IEEE; 
use IEEE.STD_LOGIC_1164.ALL; 
use IEEE.STD_LOGIC_ARITH.ALL; 
use IEEE.STD_LOGIC_UNSIGNED.ALL; 
use IEEE.NUMERIC_STD.ALL; 
 
library UNISIM; 
use UNISIM.VComponents.all; 
 
entity project_reti_logiche is 
        port(   i_clk: in std_logic; 
                i_start: in std_logic; 
                i_rst: in std_logic; 
                i_data: in std_logic_vector(7 downto 0); 
                o_address: out std_logic_vector(15 downto 0); 
                o_done: out std_logic;                 
                o_en: out std_logic; 
                o_we: out std_logic; 
                o_data: out std_logic_vector(7 downto 0)         
             ); 
end project_reti_logiche; 
 
architecture arch_project of project_reti_logiche is  
 
    type stati is (RST, WAIT_X, WAIT_XC_2, WAIT_XC_3, X_POINT, WAIT_Y, WAIT_YC_2, WAIT_YC_3, Y_POINT, WAIT_MASK, MASK, WAIT_XC, TAKE_XC, WAIT_YC, TAKE_YC, CALC_DIST, RES, CLOSE); -- WAIT_RST); --, WAIT_FOR_NEXT_TEST); 
    signal curr_state: stati := RST; 
  
    signal xp: std_logic_vector(7 downto 0) := "00000000"; 
    signal yp: std_logic_vector(7 downto 0) := "00000000"; 
    signal xc: std_logic_vector(7 downto 0) := "00000000"; 
    signal yc: std_logic_vector(7 downto 0) := "00000000"; 
    signal beg_mask: std_logic_vector(7 downto 0) := "00000000"; 
 
    signal Y_N: std_logic_vector(7 downto 0) := "00000001";
    signal Y_N_1: integer := 0; 
 
 
begin 
    next_status: process (i_clk, i_rst)
    
    variable data: std_logic_vector(7 downto 0);
    variable address: std_logic_vector(15 downto 0); 
    variable dist: std_logic_vector(8 downto 0); 
    variable dist_min: std_logic_vector(8 downto 0);
    variable xp_op: std_logic_vector(8 downto 0);
    variable yp_op: std_logic_vector(8 downto 0); 
    variable xc_op: std_logic_vector(8 downto 0);
    variable yc_op: std_logic_vector(8 downto 0);
    
    begin
        if (i_rst = '1') then                                   --check reset signal
            curr_state <= RST;
        end if;
        if (rising_edge(i_clk)) then                            --synch on rising_edge clock
            case curr_state is
            
                when RST =>                                     --reset state
                    if(i_rst= '1') then
                        curr_state <= RST;
                    else
                        if (i_start = '1') then                 --waiting for start signal, with i_rst='0'
                            o_en <= '1';
                            o_data <= "00000000";
                            o_address <= "0000000000010001";    --address setting to get xp
                            o_we <= '0';
                            o_done <= '0';                                                    
                            dist := "000000000";                
                            dist_min := "111111111";            --dist_min initialized at its maximum ("11111111")
                            data := "00000000";                 --output mask initialized at its minimum ("00000000")
                            xp_op := "000000000";
                            yp_op := "000000000";
                            xc_op := "000000000";
                            yc_op := "000000000";
                            address := "0000000000000000";
                            xp <= "00000000";
                            yp <= "00000000"; 
                            xc <= "00000000"; 
                            yc <= "00000000"; 
                            beg_mask <= "00000000";
                            Y_N <= "00000001";                  --counter initialized at the first centroid
                            Y_N_1 <= 0;                         --beg_mask bit "counter" initialized at '0'
                            curr_state <= WAIT_X;
                        else
                            curr_state <= RST;
                        end if;
                    end if;
                              
                                    
                when WAIT_X =>                                  --waiting state, before getting xp
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        curr_state <= X_POINT;
                    end if;
                    
                    
                when X_POINT =>                                 --getting-xp state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else                
                        xp <= i_data;
                        o_address <= "0000000000010010";        --address setting to get yp
                        curr_state <= WAIT_Y;
                    end if;
                     
                     
                when WAIT_Y =>                                  --waiting state, before getting yp
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        xp_op := xp + "000000000";              --save xp in xp_op, 9 bit because of a possible overflow
                        curr_state <= Y_POINT;
                    end if;
                    
                    
                when Y_POINT =>                                 --getting-yp state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        yp <= i_data;
                        o_address <= "0000000000000000";        --address setting to get beg_mask
                        curr_state <= WAIT_MASK;
                    end if;
                    
                    
                when WAIT_MASK =>                               --waiting state, before getting beg_mask
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        yp_op := yp + "000000000";              --save yp in yp_op, 9 bit because of a possible overflow
                        curr_state <= MASK;
                    end if;
                    
                    
                when MASK =>                                    --getting-beg_mask state
                    if(i_rst='1') then 
                        curr_state <= RST;
                    else                
                        beg_mask <= i_data;
                        o_address <= "0000000000000001";        --address setting to get xc
                        address := "0000000000000001";          --saving next address
                        curr_state <= WAIT_XC;
                    end if; 
                
                
                when WAIT_XC =>                                 --waiting state, before get xc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        curr_state <= WAIT_XC_2;
                    end if;
                    
                    
                when WAIT_XC_2 =>                               --waiting state, before get xc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        o_en <='1';
                        curr_state <= WAIT_XC_3;
                   end if;  
               
               
                when WAIT_XC_3 =>                               --waiting state, before get xc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        curr_state <= TAKE_XC;
                    end if;
                       
                       
                when TAKE_XC =>                                 --getting-xc state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else                
                        if(Y_N_1 > 7) then                      --check centroid number
                            o_en <= '1';
                            o_address <= "0000000000010011";    --address setting to go the res state
                            curr_state <= RES;
                        else
                            if(beg_mask(Y_N_1) = '1') then      --check bit on the beg_mask
                                xc <= i_data;                   
                                address := address + 1;         --address setting to get yc
                                o_en <= '1';
                                curr_state <= WAIT_YC;
                            else
                                address := address + 2;         --address setting to get another xc
                                o_en <= '0'; 
                                Y_N <= Y_N + Y_N;               --updating centroid counter
                                curr_state <= WAIT_XC;
                            end if;
                            Y_N_1 <= Y_N_1 + 1;                 --updating beg_mask bit "counter" 
                            o_address <= address;
                        end if;
                    end if;
                    
                    
                when WAIT_YC =>                                 --waiting state, before getting yc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else                
                        curr_state <= WAIT_YC_2;                
                    end if;
                    
                    
                when WAIT_YC_2 =>                               --waiting state, before getting yc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else              
                        curr_state <= WAIT_YC_3;
                    end if;
                   
                when WAIT_YC_3 =>                               --waiting state, before getting yc
                    if(i_rst='1') then
                        curr_state <= RST;
                    else              
                        curr_state <= TAKE_YC;
                    end if;
                    
                when TAKE_YC =>                                 --getting yc state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else                
                        yc <= i_data;
                        curr_state <= CALC_DIST;
                    end if;
                    
                    
                when CALC_DIST =>                               --distance calculaion state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else
                        xc_op := xc + "000000000";              --save xc in xc_op, 9 bit because of a possible overflow
                        yc_op := yc + "000000000";              --save yc in yc_op, 9 bit because of a possible overflow
                        if(xp > xc) then
                            if(yp > yc) then
                                dist := (xp_op-xc_op) + (yp_op-yc_op);
                            else
                                dist := (xp_op-xc_op) + (yc_op-yp_op);
                            end if;
                        else
                            if(yp > yc) then
                                dist := (xc_op-xp_op) + (yp_op-yc_op);
                            else
                                dist := (xc_op-xp_op) + (yc_op-yp_op);
                            end if;
                        end if;
                        if(dist < dist_min) then                --updating dist
                            dist_min := dist;
                            data := Y_N;                        --updating output_mask
                        else 
                            if(dist = dist_min) then        
                                data := data or Y_N;            --updating output_mask
                            end if;
                        end if;
                        Y_N <= Y_N + Y_N;                       --shift -> updating centroid counter
                        if(Y_N_1 > 7) then
                            o_en <= '1';
                            o_address <= "0000000000010011";    --address setting, before getting to the res state
                            o_we <= '1';
                            curr_state <= RES;
                        else
                            address := address + 1;             --address setting, before getting another xc
                            o_address <= address;
                            curr_state <= WAIT_XC;
                        end if;
                    end if;
                                   
                when RES =>                                     --res state
                    if(i_rst='1') then
                        curr_state <= RST;
                    else    
                        o_en <= '1';
                        o_done <= '1';                          --setting o_done at '1'
                        o_we <= '1';                            --setting o_we at '1'
                        o_data <= data;
                        curr_state <= CLOSE;
                    end if;
               
                when CLOSE =>                                   --close state
                    if(i_rst = '1') then
                        curr_state <= RST;
                    else
                        o_done <= '0';                          --setting o_done at '0' -> finish test
                    end if;
            end case;
        end if;
    end process;                   
end arch_project;
--------------------------------------------
-- Module Name: mux_2_to_1_gate
--------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

Entity mux_2_to_1_gate Is
port (
		x : in STD_LOGIC;
		y : in STD_LOGIC;
		s : in STD_LOGIC;
		m : out STD_LOGIC
	);
end mux_2_to_1_gate;

Architecture behavior of mux_2_to_1_gate Is

Signal s_bar : STD_LOGIC;
Signal x_int : STD_LOGIC;
Signal y_int : STD_LOGIC;

begin
        
		s_bar <= not s;
		x_int <= x and s_bar;
		y_int <= y and s;
		m <= x_int or y_int;

end behavior;
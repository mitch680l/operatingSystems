--------------------------------------------
-- Module Name: mux_2bit_2_to_1_dataflow
--------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

Entity mux_2bit_2_to_1_dataflow Is
port (
		x : in STD_LOGIC_VECTOR(1 downto 0);
		y : in STD_LOGIC_VECTOR(1 downto 0);
		s : in STD_LOGIC;
		m : out STD_LOGIC_VECTOR(1 downto 0)
	);
end mux_2bit_2_to_1_dataflow;

Architecture behavior of mux_2bit_2_to_1_dataflow Is
begin
        
	m(1) <= ((not s) and x(1)) or (s and y(1)) after 3ns;
	m(0) <= ((not s) and x(0)) or (s and y(0)) after 3ns;

end behavior;
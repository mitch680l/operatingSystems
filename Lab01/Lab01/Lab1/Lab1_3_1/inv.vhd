library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

Entity inv Is
port (
		i : in STD_LOGIC;
		o : out STD_LOGIC
	);
end inv;

architecture behavior of inv is

begin
    o <= not i;
end behavior;

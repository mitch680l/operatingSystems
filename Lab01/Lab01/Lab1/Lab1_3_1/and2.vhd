library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

Entity and2 Is
port (
		i0, i1 : in STD_LOGIC;
		o : out STD_LOGIC
	);
end and2;

architecture behavior of and2 is

begin
    o <= i0 and i1;
end behavior;

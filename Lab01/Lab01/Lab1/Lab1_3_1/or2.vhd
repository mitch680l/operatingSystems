library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

Entity or2 Is
port (
		i0, i1 : in STD_LOGIC;
		o : out STD_LOGIC
	);
end or2;

architecture behavior of or2 is

begin
    o <= i0 or i1;
end behavior;

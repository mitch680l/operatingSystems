--------------------------------------------
-- Module Name: mux_2bit_2_to_1_structural
--------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


Entity mux_2bit_2_to_1_structural Is
port (
		x : in STD_LOGIC_VECTOR(1 downto 0);
		y : in STD_LOGIC_VECTOR(1 downto 0);
		s : in STD_LOGIC;
		m : out STD_LOGIC_VECTOR(1 downto 0)
	);
end mux_2bit_2_to_1_structural;

architecture behavior of mux_2bit_2_to_1_structural is
	component and2 port
	( i0, i1: in std_logic;
	  o : out std_logic);
	end component;

	component or2 port
	( i0, i1: in std_logic;
	o : out std_logic);
	end component;

	component inv port
	( i: in std_logic;
	o : out std_logic);
	end component;

	signal s_bar : std_logic;
	signal x_int : std_logic_vector(1 downto 0);
	signal y_int : std_logic_vector(1 downto 0);
begin	
	n1 : inv port map (i => s, o => s_bar);
	a1 : and2 port map (i0 => x(1), i1 => s_bar, o => x_int(1));
	a2 : and2 port map (i0 => y(1), i1 => s, o => y_int(1));
	o1 : or2 port map (i0 => x_int(1), i1 => y_int(1), o => m(1));
	a3 : and2 port map (i0 => x(0), i1 => s_bar, o => x_int(0));
	a4 : and2 port map (i0 => y(0), i1 => s, o => y_int(0));
	o2 : or2 port map (i0 => x_int(0), i1 => y_int(0), o => m(0));				
end behavior;
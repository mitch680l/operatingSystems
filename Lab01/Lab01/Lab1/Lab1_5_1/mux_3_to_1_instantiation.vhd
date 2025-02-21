--------------------------------------------
-- Module Name: mux_3_to_1_instantiation
--------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

Entity mux_3_to_1_instantiation Is
port(
		u : in STD_LOGIC_VECTOR(1 downto 0);
		v : in STD_LOGIC_VECTOR(1 downto 0);
		w : in STD_LOGIC_VECTOR(1 downto 0);
		s0 : in STD_LOGIC;
		s1: in STD_LOGIC;
		m : out STD_LOGIC_VECTOR(1 downto 0)
	);
end mux_3_to_1_instantiation;

Architecture behavior of mux_3_to_1_instantiation Is
	Component mux_2bit_2_to_1_dataflow
	port (
		x : in STD_LOGIC_VECTOR(1 downto 0);
		y : in STD_LOGIC_VECTOR(1 downto 0);
		s : in STD_LOGIC;
		m : out STD_LOGIC_VECTOR(1 downto 0)
		);	
	End Component;
	
	Signal m_int : STD_LOGIC_VECTOR(1 downto 0);
	Signal u1_o : STD_LOGIC_VECTOR(1 downto 0);	
	
begin
	U1:  mux_2bit_2_to_1_dataflow PORT MAP (
			x => u,
			y => v,
			s => s0,
			m => u1_o
		 );

	U2:  mux_2bit_2_to_1_dataflow PORT MAP (
			x => u1_o,
			y => w,
			s => s1,
			m => m_int
		 );
		 
	m <= m_int;
	
end behavior;
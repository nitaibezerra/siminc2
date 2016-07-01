<?php

function listaPassagens() {
	global $db;

	$sql = "SELECT
				'<img border=\"0\" src=\"../imagens/consultar.gif\" onclick=\"abrePopUp('');\" style=\"cursor:pointer\" />', 
				upper ( psdnoproposto ) as nome,
				aero_ori.aercidade as origem,
				aero_des.aercidade as destino,
				upper ( psdnuvoo ) as voo,
				to_char(coalesce(psdtarifapraticada,0)::numeric, '99D99') as tarifa
			FROM
				evento.papassagem_scdp scdp
			LEFT JOIN 
				evento.paaeroporto aero_ori ON aero_ori.aerid = scdp.aeridori
			LEFT JOIN
				evento.paaeroporto aero_des ON aero_des.aerid = scdp.aeriddes
			ORDER BY
				nome";
	
	$cabecalho = array("Ação","Nome do Proposto", "Origem", "Destino", "Nº do Voo" , "Tarifa");
	$db->monta_lista_simples($sql,$cabecalho,100,5,'N','95%','S', '', '', '', true);
}

include APPRAIZ."includes/cabecalho.inc";

echo'<br>';


monta_titulo( $titulo_modulo, 'Passagens SCDP' );

listaPassagens(); 
?>

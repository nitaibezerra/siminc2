<?php

$perfis = pegaPerfilGeral();
	
if(in_array(PFL_COORDENADOR_ADJUNTO_IES,$perfis)) {
	
	$iusd_aco = $db->pegaUm("SELECT i.iusd FROM sisfor.identificacaousuario i 
							 INNER JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod='".PFL_COORDENADOR_ADJUNTO_IES."'
							 WHERE t.sifid='".$_SESSION['sisfor']['sifid']."' AND i.iuscpf='".$_SESSION['usucpf']."'");
	
	
	
}

if(!$iusd_aco) {

	$iusd_aco = $db->pegaUm("SELECT i.iusd FROM sisfor.sisfor s 
							 INNER JOIN sisfor.identificacaousuario i ON i.iuscpf = s.usucpf 
							 WHERE s.sifid='".$_SESSION['sisfor']['sifid']."'");

}


include_once APPRAIZ_SISFOR."acompanharavaliacaobolsas.inc";

?>
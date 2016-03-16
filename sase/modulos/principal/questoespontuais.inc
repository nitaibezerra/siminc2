<?php

ob_start();

// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/QuestoesPontuaisPar.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --


// --------------- Decisão da Página
$pagina = "";
if( !$_GET['aba'] || $_GET['aba'] == 'lista' ) $pagina = "lista";
if( $_GET['aba'] == 'edicao' ) $pagina = "questoespontuais";
if( $_GET['aba'] == 'mapa' ) $pagina = "mapa";
// --


// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Questões Pontuais';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
	<?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
	<center><h3><?=$titulo?></h3></center>
</div>

<div id="menu_sistema">
	<ul class="nav nav-tabs">
		<li <?=($pagina=='lista')?'class="active"':''?>><a href="sase.php?modulo=principal/questoespontuais&acao=A&aba=lista">Lista</a></li>
	  	<li <?=($pagina=='questoespontuais')?'class="active"':''?>><a href="sase.php?modulo=principal/questoespontuais&acao=A&aba=edicao">Questões Pontuais</a></li>
	  	<li <?=($pagina=='mapa')?'class="active"':''?>><a href="sase.php?modulo=principal/questoespontuais&acao=A&aba=mapa">Mapa</a></li>
	</ul>
</div><?php
// --


// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/questoespontuais/" . $pagina . ".inc";
// --


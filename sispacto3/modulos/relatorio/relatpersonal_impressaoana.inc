<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Impressões da ANA</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

include_once '_funcoes_professoralfabetizador.php';

$es = estruturaImpressaoANA(array());

$modoRelatorio = true;

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
	$inner_join = "INNER JOIN sispacto2.identificacaousuario i ON i.iusd = a.iusd AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
}


$registros = $db->carregar("SELECT imaparticiparam,
									    imaacessoresultados,
									    imaresultadosescola,
									    imaaspectosorientacoes,
									    imaaspectostempoaplicacao,
									    imaaspectoshorarioaplicacao,
									    imaaspectosquantidadequestoes,
									    imaaspectosclarezaquestoes,
									    imaaspectosaplicadorexterno,
									    imaaspectoslocalavaliacao,
									    imaaspectosapresentacaoavaliacao,
									    imaaspectosapresentacaoresultados,
									    imafatoresgestaoescolar,
									    imafatoresformacaoprofessores,
									    imafatorespraticaspedagogicas,
									    imafatoresperfilalunos,
									    imafatoresrecursosdidaticos,
									    imafatoresestruturafisica,
									    imafatoresparticipacaofamilia,
									    imafatoresrelacoesinterpessoais FROM sispacto2.impressoesana a 
										$inner_join");

if($registros[0]) {
	foreach($registros as $imp) {
		$c = array_keys($imp);
		foreach($c as $indice) {
			$arrFinal[$indice][((trim($imp[$indice]))?$imp[$indice]:'vazio')]++;
		}
	}
}

include APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";

?>
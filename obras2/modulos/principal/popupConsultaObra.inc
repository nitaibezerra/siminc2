<?php

$empid = $_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid'];
$_SESSION['obras2']['empid'] = $empid;
dbg($empid);

// se não tiver obra retorna
if ( empty($empid) ){
	die("<script>
		alert('Obra nao definida, a tela será fechada');
	        self.close();
	 </script>");
}

$empreendimento = new Empreendimento( $empid );
dbg($empreendimento,d);

/*
// Cria as abas do módulo
echo '<br>';
if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
	$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
}else{
	$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
}

$empid = $_SESSION['obras2']['empid'];
$docid = pegaDocidEmpreendimento( $empid );

echo cabecalhoObra($obrid);
echo '<br>';
monta_titulo( $titulo_modulo, '' );

$licitacao = new Licitacao();
$dados = $licitacao->pegaDadosPorObra( $obrid );
*/

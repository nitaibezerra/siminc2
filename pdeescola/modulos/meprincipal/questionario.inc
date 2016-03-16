<?php

include_once APPRAIZ . "pdeescola/classes/MeParceiro.class.inc";
include_once APPRAIZ . "pdeescola/classes/MeMaisEducacao.class.inc";
require_once APPRAIZ . 'includes/classes/entidades.class.inc';
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/tabelas/Montatabela.class.inc";
include_once APPRAIZ . "pdeescola/classes/controle/CampoExternoControle.class.inc";
	
me_verificaSessao();

if( $_REQUEST['carregaAtiv'] == true ){
	
	$anoAnterior = $_SESSION["exercicio"] - 1;
	
	if(!$_REQUEST['mtmid']) $_REQUEST['mtmid'] = 0; 
	
	$sql = "SELECT
				mta.mtaid AS codigo, 
				mta.mtadescricao AS descricao
			FROM
				pdeescola.metipoatividade mta
			WHERE
				mta.mtasituacao = 'A' AND
				mta.mtaanoreferencia = ".$anoAnterior." AND
				mta.mtmid = ".$_REQUEST['mtmid'];
	
	$db->monta_combo( "atividade[]", $sql, 'S', 'Selecione a Atividade', '', '', '', 200);
	die();
}

if( $_POST['salvar_questionario'] && $_POST['idTabela'] ){
	$obMonta = new Montatabela();
	$obMonta->salvar();
}

if( $_POST['salvar_questionario'] && $_POST['identExterno'] ){
	$obMontaEx = new CampoExternoControle();
	$obMontaEx->salvar();
}


$boExisteEntidade = boExisteEntidade( $_SESSION['meentid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

$qrpid = pegaQrpidME( $_SESSION['memid'], QUESTIONARIO_MAISEDUC );

if( $_SESSION['memid'] != '' &&  $_REQUEST['rel_estimativa'] != '' ){
	$sql_combo = "SELECT
							ent.entid as codigo 
						FROM 
							entidade.entidade ent
						INNER JOIN
							pdeescola.memaiseducacao mme ON mme.entid = ent.entid AND mme.memanoreferencia = " . $_SESSION["exercicio"] . "
						WHERE mme.memid = ".$_SESSION['memid']."
							ORDER BY
							ent.entnome";
	$entid = $db->pegaUm( $sql_combo );
	echo $entid;
	//	echo "
//		<script>
//			window.open( 'pdeescola.php?modulo=merelatorio/meresultado_estimativa&acao=A&entid=$entid&tipo=html', 
//			'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
//		</script>
//		";
	die();
}
// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

if(!$_SESSION["memid"]){
	echo "<script>
			alert('Não existe escola atribuida para o seu Perfil.');
			window.location.href = '/pdeescola/pdeescola.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/questionario&acao=A");
$titulo = "Mais Educação";
$subtitulo = "Cadastro - Questionário";
echo monta_titulo($titulo, $subtitulo);
echo cabecalhoMaisEducacao();
?>
<script src="../includes/prototype.js" type="text/javascript"></script>
<script src="./js/meajax.js" type="text/javascript"></script>
<script type="text/javascript">

function comboAtividade( obj ){
	var req = new Ajax.Request('pdeescola.php?modulo=meprincipal/questionario&acao=A', {
		        method:     'post',
		        parameters: 'carregaAtiv=true&mtmid='+obj.value,
		        asynchronous: false,
		        onComplete: function (res){
		        	obj.parentNode.parentNode.cells[1].innerHTML = res.responseText;
		        }
		  });
}

</script>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td>
		<fieldset style="width: 94%; background: #fff;"  >
			<legend>Questionário</legend>
			<?php
				$habilitado = 'S';
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'relatorio' => 'modulo=merelatorio/relatorioQuestionario&acao=A', 'habilitado' => $habilitado  ) );
			?>
		</fieldset>
	</tr>
</table>
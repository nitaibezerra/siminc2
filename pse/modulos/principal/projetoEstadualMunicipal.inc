<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ. '/includes/Agrupador.php';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
include_once( APPRAIZ. "pse/classes/ItemSelecionado.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";

/*if($pflcod == SECRETARIA_MUNICIPAL){
	$nmMunEst = 'm';
} else {
	$nmMunEst = 'e';
}*/

$nmMunEst = $_SESSION['pse']['flagmun'];

$obEstadomunicipiopse = new EstadoMunicipioPSE($_SESSION['pse']['empid'],$_SESSION['pse']["muncod"], $nmMunEst );
	
$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse->empflagestmun = $nmMunEst;
	
$muncod = $_SESSION['pse']["muncod"];
$empid = $obEstadomunicipiopse->empid;

$obPergunta = new Pergunta();
$obEstadomunicipiopse->verEmp($empid);
 
$sql = "select resid from pse.resposta where perid = 6 AND empid = $empid";
$resid = $obPergunta->pegaUm($sql);
$resid = $resid ? $resid : '';

if($_REQUEST['projeto']){

	$SitSaude 	= ($_REQUEST["SitSaude"]==''?'':explode(",",$_REQUEST["SitSaude"]));
	$censo2 	= ($_REQUEST["censo2"]==''?'':$_REQUEST["censo2"]);
	$censo3 	= ($_REQUEST["censo3"]==''?'':$_REQUEST["censo3"]);
	
	foreach ($_REQUEST as $chave => $values)
	{
		if(strstr($chave,"per_"))
		{
			$pergunta = explode("_",$chave);

			if ($pergunta[1]==6)
			{
				if ($values=='s' && $SitSaude==''){
					echo "1";
					exit;
				}
			}
			if ($pergunta[1]==7)
			{
				if ($values=='s' && $censo2==''){
					echo "3";
					exit;
				}
			}
			if ($pergunta[1]==9)
			{
				if ($values=='s' && $censo3==''){
					echo "2";
					exit;
				}
			}
			
			$arrCampos = array('resid','perid','empid','resflagresposta');
			$arrDados = array('resid','perid' =>$pergunta[1] ,'empid' => $empid,'resflagresposta' => $values);
			
			$resposta =  new Resposta('',$pergunta[1], $empid);
			$resposta->popularObjeto($arrCampos, $arrDados);

			if($resposta->salvar())
				$resposta->commit();
			else
				$resposta->rollback();
		
			if ($pergunta[1]==6)
			{ 
				$obItemSelecionado = new ItemSelecionado();
				$obItemSelecionado->deletar($resposta->resid);
				if ($values=='s'){
					foreach($SitSaude as $SitSaude)
					{
						if($SitSaude)
						{
							$arrCampos	= array('iteid','resid');
							$arrDados 	= array('iteid' => $SitSaude, "resid" => $resposta->resid);
							$obItemSelecionado = new ItemSelecionado();		
							$obItemSelecionado->popularObjeto($arrCampos,$arrDados);
					
							if($obItemSelecionado->salvar()) {
								$obItemSelecionado->commit();
							} else {
								$obItemSelecionado->rollback();
							}	
						} 
					}
				}
			}
			if ($pergunta[1]==9){
				$obItemSelecionado = new ItemSelecionado();
				$obItemSelecionado->deletar($resposta->resid);
				if ($values=='s'){
					foreach($censo3 as $censo3)
					{
						if($censo3)
						{
							$arrCampos	= array('iteid','resid');
							$arrDados 	= array('iteid' => $censo3, "resid" => $resposta->resid , );
							
							$obItemSelecionado = new ItemSelecionado();		
							$obItemSelecionado->popularObjeto($arrCampos,$arrDados);
						
							if($obItemSelecionado->salvar()) {
								$obItemSelecionado->commit();
							} else {
								$obItemSelecionado->rollback();
							}
						} 
					}
				}
			}
			if ($pergunta[1]==7){
				$obItemSelecionado = new ItemSelecionado();
				$obItemSelecionado->deletar($resposta->resid);
				if ($values=='s'){
					foreach($censo2 as $censo2)
					{
						if($censo2)
						{
							$arrCampos	= array('iteid','resid');
							$arrDados 	= array('iteid' => $censo2, "resid" => $resposta->resid , );
							
							$obItemSelecionado = new ItemSelecionado();		
							$obItemSelecionado->popularObjeto($arrCampos,$arrDados);
						
							if($obItemSelecionado->salvar()) {
								$obItemSelecionado->commit();
							} else {
								$obItemSelecionado->rollback();
							}
						}
					}
				}
			}
		}
	}	
	exit;
}
$sql = "select resflagresposta from pse.resposta where perid = 6 AND empid = $empid";
		$flg1 = $obPergunta->pegaUm($sql);
$sql = "select resflagresposta from pse.resposta where perid = 7 AND empid = $empid";
		$flg2 = $obPergunta->pegaUm($sql);
$sql = "select resflagresposta from pse.resposta where perid = 9 AND empid = $empid";
		$flg3 = $obPergunta->pegaUm($sql);

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "PROJETO";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '' );

?>
<form method="POST" name="formunicipio" action="" id="formunicipio">
<?php Cabecalho($muncod,2,$nmMunEst); ?>
<table style="border-bottom: 0px;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

	<tr>
		<td class="SubTituloEsquerda" colspan="2" style="text-align: center">2.1. Cenário da Saúde</td>
	</tr>
	<?php
		$sql = "select resid from pse.resposta where perid = 6 AND empid = $empid";
		$resid = $obPergunta->pegaUm($sql);
		$resid = $resid ? $resid : '';
		$obPergunta->metodoPergunta = "validaItem(0,'item2_11[]')";
		$obPergunta->carregarPerguntas("'2.1.1'", $resid);
	?>	
	<?php
		$sql = "select resid from pse.resposta where perid = 7 AND empid = $empid";
		$resid = $obPergunta->pegaUm($sql);
		$resid = $resid ? $resid : '';
		$obPergunta->metodoPergunta = "validaItem(0,'item2_12[]')";
		$obPergunta->carregarPerguntas("'2.1.2'", $resid);	
	?>
	<?php
		$sql = "select resid from pse.resposta where perid = 8 AND empid = $empid";
		$resid = $obPergunta->pegaUm($sql);
		$resid = $resid ? $resid : '';
		$obPergunta->carregarPerguntas("'2.1.3'", $resid);	
	?>
	<?php
		$sql = "select resid from pse.resposta where perid = 9 AND empid = $empid";
		$resid = $obPergunta->pegaUm($sql);
		$resid = $resid ? $resid : '';
		$obPergunta->metodoPergunta = "validaItem(0,'item2_14[]')";
		$obPergunta->carregarPerguntas("'2.1.4'", $resid);	
	?>
	
	<?php if(($nmMunEst=='e' && $pflcod == SECRETARIA_ESTADUAL) || ($nmMunEst=='m' && $pflcod == SECRETARIA_MUNICIPAL) || $pflcod == SUPER_USUARIO){?>
	<?php 
		$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
		$arr = $db->pegaLinha( $sql );
		$dataAtual = date("Y-m-d");
		$data = new Data();
		$dataF = trim($arr['prsdata_termino']);
		$resp = 1;
			if( !empty($dataF) ){
				$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
			}
		if( $resp == NULL ){ ?>
			<tr style="background-color: #DCDCDC">
				<td  align="center" colspan="2">
					<input type="button" value="Salvar" onclick="validaProjeto();">
				</td>
			</tr>
		<?php } ?>
	<?php }?>
	<?=navegacao('grupoGestor','gestao');//volta para grupoGestor e vai para gestao?>
</table>	
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script>
	validaItem(<?php if ($flg1=='s'){?>1<? } else {?>0<? } ?>,'item2_11[]');
	validaItem(<?php if ($flg2=='s'){?>1<? } else {?>0<? } ?>,'item2_12[]');
	validaItem(<?php if ($flg3=='s'){?>1<? } else {?>0<? } ?>,'item2_14[]');
</script>
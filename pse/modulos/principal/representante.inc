<?php
include APPRAIZ. '/includes/Agrupador.php';
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
include_once( APPRAIZ. "pse/classes/ItemSelecionado.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
/*
if($pflcod == SECRETARIA_MUNICIPAL){
	$nmMunEst = 'm';
} else {
	$nmMunEst = 'e';
}*/

$nmMunEst = $_SESSION['pse']['flagmun'];

$obEstadomunicipiopse = new EstadoMunicipioPSE($_SESSION['pse']['empid'],$_SESSION['pse']["muncod"], $nmMunEst );

$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse->empflagestmun = $nmMunEst;

$muncod = $_SESSION['pse']["muncod"];
$obPergunta = new Pergunta();
$empid = $obEstadomunicipiopse->empid;
$obEstadomunicipiopse->verEmp($empid);

$sql = "select resid from pse.resposta where perid = 1 AND empid = ". $empid;
$resid = $obPergunta->pegaUm($sql);
$resid = $resid ? $resid : '';

if($_POST)
{

	$secretarias 	= $_REQUEST["secretarias"];
	$pergunta 		= $_REQUEST["idpergunta"];
	
	if($_REQUEST["per_1"] == 's' && $secretarias <> '' || $_REQUEST["per_1"] == 'n'){
		
		$resposta =  new Resposta($resid ,$pergunta);
			
		$arrCampos = array('perid','resflagresposta');
		
		$arrDados = array('perid' =>$pergunta ,'resflagresposta' => $_REQUEST["per_1"]);
		$resposta->popularObjeto($arrCampos, $arrDados);
		$resposta->resid = $resid;
		$resposta->empid = $obEstadomunicipiopse->empid;
	
		if($resposta->salvar())
			$resposta->commit();
		else
			$resposta->rollback();	
	
		$obItemSelecionado = new ItemSelecionado();
		$obItemSelecionado->deletar($resposta->resid);
		
		if($_REQUEST["per_1"]=='s'){
		foreach($secretarias as $secretaria)
		{
			if($secretaria)
			{
				$arrCampos	= array('iteid','resid');
				$arrDados 	= array('iteid' => $secretaria, "resid" => $resposta->resid , );
				
				$obItemSelecionado = new ItemSelecionado();		
				$obItemSelecionado->popularObjeto($arrCampos,$arrDados);
				
				if($obItemSelecionado->salvar())
					$obItemSelecionado->commit();
				else
					$obItemSelecionado->rollback();	
			} 
		}
		}
		echo "<script>alert('Operação realizada com sucesso!');
		window.location.href='pse.php?modulo=principal/representante&acao=A';</script>";
		//exit;
	} else {
		echo "<script>alert('A Questão 1.3 é obrigatória!');</script>";
	}
}
$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "REPRESENTANTE DO PSE";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );
?>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
<body <?php if ($resid==''){?>onload="validaItem(0,'item1_3[]')"<? } ?>>
<form method="POST" name="formunicipio" action="" id="formunicipio" onSubmit="selectAllOptions( document.getElementById('secretarias'))">
	<input type="hidden" value='' id="cadsecretarias" name="cadsecretarias">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
		$obPergunta->metodoPergunta = "validaItem(0,'item1_3[]')";
		$obPergunta->carregarPerguntas("'1.3'", $resid, $empid);	
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
		<tr style="background-color: #DCDCDC;" >
			<td align="center" colspan="2">
				<input type="submit" value="Salvar">
			</td>
		</tr>
	<?php } ?>
	<?php }?>
	<?=navegacao('identificacaoEstadoMunicipio','pse2009');?>	
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<?php
$obResposta = new Resposta($resid,$obPergunta->perid );

$resp = ($obResposta->resflagresposta == 's') ? 1 : 0;  

	echo "<script type=\"text/javascript\">";
		echo "validaItem('$resp','item1_3[]');";
	echo "</script>";
?>

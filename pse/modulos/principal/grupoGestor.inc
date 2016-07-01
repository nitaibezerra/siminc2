<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

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
$obEstadomunicipiopse->verEmp($empid);

$obPergunta = new Pergunta();

$sql = "select resid from pse.resposta where empid = ". $empid;
$resid = $obPergunta->pegaUm($sql);
$resid = $resid ? $resid : '';

if($_REQUEST['grupoGestor']){

	foreach ($_REQUEST as $chave => $values)
	{
		
		if(strstr($chave,"per"))
		{
			$pergunta = explode("_",$chave);

			$arrCampos = array('resid','perid','empid','resflagresposta');
			$arrDados = array('resid','perid' =>$pergunta[1] ,'empid' => $empid,'resflagresposta' => $values);
			
			$resposta =  new Resposta('',$pergunta[1], $empid);
			$resposta->popularObjeto($arrCampos, $arrDados);

			if($resposta->salvar())
				$resposta->commit();
			else
				$resposta->rollback();	
		}
	}
	
	exit;
}


include APPRAIZ. '/includes/Agrupador.php';

$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "GRUPO GESTOR DO PROJETO SAÚDE E PREVENÇÃO NA ESCOLA PSE/SPE";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '' );

?>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<form method="POST" name="formunicipio" action="" id="formunicipio">
	<input type="hidden" value="<?php echo $_SESSION["muncod"] ?>" id="muncod" name="muncod">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	
	<?php
		$perguntas = "'5.','6.','7.','8.'";
		$obPergunta->carregarPerguntas($perguntas, '', $empid);	
		
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
				<td align="center" colspan="2">
					<input type="button" value="Salvar" onclick="salvarGestor();">
				</td>
			</tr>
		<?php } ?>
	<?php }?>
	<?=navegacao('materialClinicoDidatico','projetoEstadualMunicipal');?>	
	<!-- FIM ITEM 7 -->
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>


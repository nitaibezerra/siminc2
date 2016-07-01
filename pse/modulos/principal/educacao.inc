<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
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

if($_REQUEST['educacao']){

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
$titulo2 = "EDUCAÇÃO/CAPACITAÇÃO";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<form method="POST" name="formunicipio" action="" id="formunicipio">
	<input type="hidden" value="<?php echo $_SESSION["pse"]["muncod"] ?>" id="muncod" name="muncod">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">3.8. Para as ações de educação permanente e capacitação de profissionais da educação, responda as questões abaixo:</td>
	</tr>
	<?php
		$perguntas = "'3.8'";
		$obPergunta->carregarPerguntas($perguntas, '', $empid);	
		
	?>
	<?php if(($nmMunEst=='e' && $pflcod == SECRETARIA_ESTADUAL) || ($nmMunEst=='m' && $pflcod == SECRETARIA_MUNICIPAL) || $pflcod == SUPER_USUARIO){?>
	<tr style="background-color: #DCDCDC">
		<td align="center" colspan="2">
			<input type="button" value="Salvar" onclick="salvarEducacao();">
		</td>
	</tr>
	<!-- FIM ITEM 7 -->
	<?php }?>
	<?=navegacao('componente04','cadastroEstadoMunicipioArvore');?>
	</table>
</form>	
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>


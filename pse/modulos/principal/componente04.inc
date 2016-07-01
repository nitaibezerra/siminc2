<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/Resposta.class.inc" );
include_once( APPRAIZ. "pse/classes/QuantidadeEscola.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";
include APPRAIZ . 'includes/cabecalho.inc';

$nmMunEst = $_SESSION['pse']['flagmun'];

$obEstadomunicipiopse = new EstadoMunicipioPSE($_SESSION['pse']['empid'],$_SESSION['pse']["muncod"], $nmMunEst );
	
$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse->empflagestmun = $nmMunEst;
	
$muncod = $_SESSION['pse']["muncod"];
$empid = $obEstadomunicipiopse->empid;

$obPergunta = new Pergunta();
$obEstadomunicipiopse->verEmp($empid);

if($_POST){
	foreach ($_REQUEST as $chave => $values)
	{
		if(strstr($chave,"per_"))
		{
			$pergunta = explode("_",$chave);
			
			$sql = "SELECT resid from pse.resposta WHERE empid=$empid AND perid=$pergunta[1]";
			$id = $db->pegaUm($sql);

			if($id==''){
				$sql = "INSERT INTO pse.resposta (perid, empid, resflagresposta)
				values ($pergunta[1], $empid, 's')";	
			} else {
				$sql = "UPDATE pse.resposta SET resflagresposta = 's' 
				WHERE perid = $pergunta[1] 
				AND   empid = $empid";				
			}		

			$db->executar($sql);
			$db->commit();

			$arrCampos = array('resid','perid','empid','resflagresposta');
			$arrDados = array('resid','perid' =>$pergunta[1] ,'empid' => $empid, 'resflagresposta' => $values);

			$resposta =  new Resposta('',$pergunta[1], $empid);
			$resposta->popularObjeto($arrCampos, $arrDados);

			if($resposta->salvar()) {
				$resposta->commit();
			}
			else
				$resposta->rollback();	
		}
	}
}


$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "COMPONENTE IV/AÇÕES";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form method="POST" name="formunicipio" action="" id="formunicipio">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
		$obPergunta->carregarPerguntas2('7', $empid);
	?>

	<tr>
		<td class="SubTituloEsquerda" colspan="2">3.9. Consta no Censo Escolar (2008) o "Encarte Saúde" (INEP/MEC):</td>
	</tr>
	<?php
		$perguntas = "'3.9'";
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
				<td  align="center" colspan="2">
					<input type="submit" value="Salvar">
				</td>
			</tr>
		<?php } ?>
	<?php }?>
	<?=navegacao('componente03','cadastroEstadoMunicipioArvore');//volta para componente03 e vai para educacao?>		
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script>
	somaCampos(4);
</script>
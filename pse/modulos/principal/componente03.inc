<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );
include_once( APPRAIZ. "pse/classes/Pergunta.class.inc" );
include_once( APPRAIZ. "pse/classes/QuantidadeEscola.class.inc" );
include_once( APPRAIZ. "pse/classes/ItemSelecionado.class.inc" );
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

$sql = "select resid from pse.resposta where perid = 18 AND empid = $empid";
$resid = $obPergunta->pegaUm($sql);
$resid = $resid ? $resid : '';

if($_POST){
	$censo4 	= $_REQUEST["censo4"];
	foreach ($_REQUEST as $chave => $values)
	{
		if(strstr($chave,"per_") )
		{
			$pergunta = explode("_",$chave);
			$sql = "SELECT qtdid from pse.quantidadeescola
				WHERE empid=$empid AND atvid=$pergunta[1]";
			$id = $db->pegaUm($sql);
			
			if($id<>''){
				$sql = "DELETE FROM pse.quantidadeescola WHERE qtdid=$id";
				$db->executar($sql);
			}
			if($values <> '' && $values <>18){
				$sql = "INSERT INTO pse.quantidadeescola (atvid,empid,qtdescola) 
						values ($pergunta[1],$empid,$values)";
			
				$db->executar($sql);	
			}
			$db->commit();
			if ($pergunta[1]==18){
				$sql = "SELECT resid from pse.resposta WHERE empid=$empid AND perid=18";
				$id = $db->pegaUm($sql);
				if(!$id<>''){
					$sql = "INSERT INTO pse.resposta (perid, empid, resflagresposta)
					values (18, $empid, 's')";	
				}				
				$db->executar($sql);
				$db->commit();
				$sql = "select resid from pse.resposta where perid = 18 AND empid = $empid";
				$resid = $obPergunta->pegaUm($sql);
				$resid = $resid ? $resid : '';
				$obItemSelecionado = new ItemSelecionado();
				$obItemSelecionado->deletar($resid);
				if($censo4)
				{
					foreach($censo4 as $censo4)
					{
						$arrCampos	= array('iteid','resid');
						$arrDados 	= array('iteid' => $censo4 , "resid" => $resid , );
				
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
		else if(strstr($chave,"p2er"))
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
	echo "<script>alert('Dados salvos com sucesso');</script>";
}



$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "COMPONENTE III/AÇÕES";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form method="POST" name="formunicipio" action="" id="formunicipio">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
		$obPergunta->carregarPerguntas2('6', $empid);
	?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2" style="text-align: right">Total <input class="normal" type="text" disabled style="width: 13ex;" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" maxlength="60" size="11" name="total">&nbsp&nbsp&nbsp</td>
	</tr>
	<?php
		$sql = "select resid from pse.resposta where perid = 18 AND empid = $empid";
		$resid = $obPergunta->pegaUm($sql);
		$resid = $resid ? $resid : '';
		$obPergunta->carregarPerguntas("'3.7. '", $resid);
	?>
	
	<tr>
		<td class="SubTituloEsquerda" colspan="2">3.8. Para as ações de educação permanente e capacitação de profissionais da educação, responda as questões abaixo:</td>
	</tr>
	<?php
		$perguntas = "'3.8'";
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
					<!--  input type="button" value="Incluir" onclick="validaComponente02();">-->
					<input type="submit" value="Salvar">
				</td>
			</tr>
		<?php } ?>
	<?php }?>
	<?=navegacao('componente02','componente04');//volta para componente02 e vai para componente04?>		
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script>
	somaCampos(3);
</script>
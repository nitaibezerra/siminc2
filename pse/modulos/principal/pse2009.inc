<?php
include APPRAIZ. '/includes/Agrupador.php';
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/PseAnoEscolaMun.class.inc" );
include_once( APPRAIZ. "pse/classes/EscolaMunPSE2009.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";

/*if($pflcod == SECRETARIA_MUNICIPAL){
	$nmMunEst = 'm';
} else {
	$nmMunEst = 'e';
}*/


$nmMunEst = $_SESSION['pse']['flagmun'];

$pamanoreferencia = $_REQUEST['ano'] == null ? 0 :  $_REQUEST['ano'];

$obEstadomunicipiopse = new EstadoMunicipioPSE($_SESSION['pse']['empid'],$_SESSION['pse']["muncod"], $nmMunEst );
$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse->empflagestmun = $nmMunEst;

$muncod = $_SESSION['pse']["muncod"];
$uf = $_SESSION["pse"]["uf"];
$empid = $obEstadomunicipiopse->empid;

$obEstadomunicipiopse->verEmp($empid);
$objPseAnoEscolaMun 	= new PseAnoEscolaMun($_REQUEST["idItem"],$_SESSION['pse']["muncod"]);

$sql = "SELECT porano + 1 FROM pse.portariapse WHERE pormunicipio = '".$muncod."'";
$ano = $db->pegaUm( $sql );
$anoMax = $ano + 3;
$anoMax = $anoMax > 2012 ? 2012 : $anoMax;

$novoEmpid = pegaNovoEmpid( $muncod );

$habilitado = "S";	

if( $novoEmpid != $empid && $_SESSION['exercicio'] == $_REQUEST['ano'] ){
	$habilitado = "N";	
}

$EscolaMunPSE2009 = new EscolaMunPSE2009();
//$EscolaMunPSE2009->empid = $objPseAnoEscolaMun->empid;

//Equipe vinculadas no CNES informado
if($_REQUEST['verEquipesAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	verEquipesPSE($_REQUEST['cnes'],$entid,$_REQUEST['equipe']);
	exit;
}

if($_REQUEST["idItem"] && $_REQUEST["excluiFormularioAjax"])
{
	
	if($objPseAnoEscolaMun->excluir($_REQUEST["idItem"]))
		$objPseAnoEscolaMun->commit();
	else
		$objPseAnoEscolaMun->rollback();	
	
	exit;
	
}

if($_REQUEST["alteraForm"])
{
	header('content-type: text/html; charset=ISO-8859-1');
	alteraPSE2009($_REQUEST["idItem"]);
	exit;
}
if($_REQUEST["carregaLista"])
{
	$arrCampos 	= array('pamanoreferencia');
	$arrDados 	= array('pamanoreferencia' => $_REQUEST['ano']); 
	$objPseAnoEscolaMun->popularObjeto($arrCampos,$arrDados);
	$objPseAnoEscolaMun->carregaListaEscolas($empid);	
	exit;
}

if($_REQUEST['cadpse2009']){
	$pamanoreferencia = $_REQUEST['pamanoreferencia'];
	//$empid = $_REQUEST['empid'];
	$entid = $_REQUEST['entid'];
	$cneid = $_REQUEST['idequipe'];
	$moeid = $_REQUEST['moeid'];
	$nieid = $_REQUEST['nieid'];
	$pamquantprevista = $_REQUEST['pamquantprevista'];
	if( $_REQUEST['novoempid'] <> NULL){
		if( $cneid == '' || empty($cneid)){
			print "<script>"
				. "    alert('Faltam dados para esta tela! Tente repetir a operação.');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
		}		
		if( $_REQUEST['novoempid'] != $empid ){
			$emp = $novoEmpid;
		} else {
			$emp = $empid;
			$where2 = "pamanoreferencia = $pamanoreferencia , empid = $emp, esfid = $cneid, entid = $entid, moeid = $moeid, nieid = $nieid, pamquantprevista = $pamquantprevista, ";
		}
	}
	$pamquantatendida = $_REQUEST['pamquantatendida'];
	$id = $_REQUEST['pamId'];
		if($id<>''){
			$sql = "UPDATE pse.pseanoescolamun set {$where2}	pamquantatendida = $pamquantatendida where pamid=$id";
			//dbg($pamquantatendida, d);
		} else {
			$sql = "select pamid from pse.pseanoescolamun where entid = $entid AND esfid = $cneid AND moeid = $moeid AND nieid = $nieid AND empid = $empid AND pamanoreferencia = $pamanoreferencia";
			$pamid = $db->pegaUm($sql);
			if($pamid==''){
				if( $pamquantprevista == '' ){
					$pamquantprevista = 0;
				}
				if($pamanoreferencia == $_SESSION['exercicio']){
					$sql = "INSERT INTO pse.pseanoescolamun (pamanoreferencia, empid, esfid, entid, moeid, nieid, pamquantprevista, pamquantatendida) 
					values ($pamanoreferencia, $empid, $cneid, $entid, $moeid, $nieid, $pamquantprevista, $pamquantatendida)";
				} else {
					$sql = "INSERT INTO pse.pseanoescolamun (pamanoreferencia, empid, esfid, entid, moeid, nieid, pamquantprevista, pamquantatendida) 
					values ($pamanoreferencia, $empid, $cneid, $entid, $moeid, $nieid, $pamquantprevista, 0)";
				}
			} else {
				echo 1;
			}
		}
//		echo $sql;
//		exit();
		$db->executar($sql);
		$db->commit();
	exit;
}

$scnesId = '';
if($objPseAnoEscolaMun->esfid <> ''){ 	
	$sql = "SELECT cnecodigocnes FROM pse.scnes WHERE cneid = $objPseAnoEscolaMun->esfid";
	$scnesId = $db->pegaUm($sql);
}

$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
$arr = $db->pegaLinha( $sql );
$dataAtual = date("Y-m-d");
$data = new Data();
$dataF = trim($arr['prsdata_termino']);
$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "PSE 2009/2010/2011";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>

<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script type="text/javascript" language="javascript" src="../includes/agrupador.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<form method="POST" name="formunicipio" action="" id="formunicipio">
	<input type="hidden" value="<?=$_SESSION['pse']['portaria'] + 1;?>" name="portaria">
	<input type="hidden" value="<?=$_REQUEST['idItem'];?>" name="idItem">
	<input type="hidden" value='<?=$empid ?>' id="empid" name="empid">
	<input type="hidden" value='<?=$novoEmpid ?>' id="novoempid" name="novoempid">
	<input type="hidden" value='' id="pamId" name="pamId">
	<input type="hidden" value='<?=$_SESSION['exercicio'] ?>' id="exercicio" name="exercicio">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<!-- ANO -->
	<tr>
	    <td class="SubTituloEsquerda" style='text-align: center' colspan="2">Selecione o Ano de Referência:
	    <?php
		if( $_SESSION['exercicio'] != $anoMax ){	
	    	$nomeVar = "select".($_SESSION['exercicio']);
			${$nomeVar} = ($_REQUEST['ano'] == ($_SESSION['exercicio'])) ? "selected=selected" : "";
		}
		if( ($_SESSION['exercicio'] + 1) != $anoMax ){	
	    	$nomeVar2 = "select".($_SESSION['exercicio'] + 1);
			${$nomeVar2} = ($_REQUEST['ano'] == ($_SESSION['exercicio'] + 1)) ? "selected=selected" : "";
		}
		if( ($_SESSION['exercicio'] + 2) != $anoMax ){	
	    	$nomeVar3 = "select".($_SESSION['exercicio'] + 2);
			${$nomeVar3} = ($_REQUEST['ano'] == ($_SESSION['exercicio'] + 2)) ? "selected=selected" : "";
		}
	 	?>
	    
	    	<select name='pamanoreferencia' id='pamanoreferencia' size='1' onchange='carregaListaAno(this.value);' title= 'ano referência'  >
    			<option value='' onclick="validaItem(0,'item2[]')" >Selecione...</option>
			    <?php
				if( $_SESSION['exercicio'] != $anoMax ){ ?>
	   				<option value='<?=$_SESSION['exercicio'] ?>' <?=${$nomeVar} ?>><?=$_SESSION['exercicio'] ?></option>
				<?php }
				if( ($_SESSION['exercicio'] + 1) != $anoMax ){ ?>	
	   				<option value='<?=$_SESSION['exercicio'] + 1 ?>' <?=${$nomeVar2} ?>><?=$_SESSION['exercicio'] + 1 ?></option>
				<?php }
				if( ($_SESSION['exercicio'] + 2) != $anoMax ){ ?>	
	   				<option value='<?=$_SESSION['exercicio'] + 2 ?>' <?=${$nomeVar3} ?>><?=$_SESSION['exercicio'] + 2 ?></option>
				<?php }
			 	?>
    		</select><img border='0' src='../imagens/obrig.gif'/>
	    </td>
	</tr>
<!--ITEM 2 -->
	<?php
	if ( $_SESSION['exercicio'] == $_REQUEST['ano'] ){
	 ?>
	<!--<tr name="item2[]">-->
	<tr>
	<?php
	$obEstadomunicipiopse = new EstadoMunicipioPSE($empid, $_SESSION["pse"]["muncod"]);
	$sql = "SELECT count(distinct e.esmid) from pse.escolamunpse2009 e WHERE empid=$empid";
	$empquantescolapse2009 = $db->pegaUm($sql);
	?>
		<td class="SubTituloEsquerda" colspan="2">2. Quantas escolas do seu município participaram do PSE nesse ano?<input type="text" readonly name="empquantescolapse2009" id="empquantescolapse2009" value="<?=$empquantescolapse2009; ?>">
	<?php
		if( $resp == NULL ){
	?>
		Quais: <input type="button" value="Selecionar" name="btpesquisa2" id="btpesquisa2" onclick="selecionaESC();"> </td>
	<?php } ?>
	</tr>
	<tr name="item2[]">
		<td colspan="2" align="center">
			<?/*php
				$escolasPadrao = $EscolaMunPSE2009->carregarEscolasPadrao($uf, $_SESSION["pse"]["muncod"]);
				$escolaDestino = $EscolaMunPSE2009->carregarEscolas();
		
				$campoAgrupador = new Agrupador( 'formunicipio' );
				$campoAgrupador->setOrigem( 'agrupadorEscolas', null, $escolasPadrao );
				$campoAgrupador->setDestino( 'Escolas', null,$escolaDestino );
				$campoAgrupador->exibir();
			*/?>
		</td>
	</tr>
	<? } ?>	
	<!-- FIM ITEM 2 -->
	<tr>
		<td class="SubTituloEsquerda" colspan="2">3. Indique, para cada Equipe 	Saúde da Família - ESF: a escola atendida pela ESF e o número de
		estudantes, suas modalidades e seus níveis de ensino, em cada escola.</td>
	</tr>
	<!-- ano aqui!!! -->
	<tr>
		<td align='right' class="SubTituloDireita">Identificação da Escola (código INEP)</td>
		<td>
			<input id="espid" name="espid" title="Identificação da Escola" maxlength="8" width="100" value="<?=$objPseAnoEscolaMun->entid ?>" class="CampoEstilo" style="border-left: 3px solid rgb(136, 136, 136); text-align: right; color: rgb(128, 128, 128);" readonly="">
			<input type="hidden" id="entid" name="entid">
			<input type="button" value="Pesquisar" name="btpesquisa" id="btpesquisa" onclick="pesquisaINEP(<?=$anoT = $_REQUEST['ano'] ? $_REQUEST['ano'] : 0 ?>, <?=$_SESSION['exercicio'] ?>);"> 
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Identificação da ESF  (código SCNES)</td>
		<td>
			<input type="hidden" name="esfid" id="esfid" value="<?=$espid;?>">
   			<input type="hidden" name="idequipe" id="idequipe" value="<?= $objPseAnoEscolaMun->esfid ?>">
   			<input type="hidden" name="idcnes" id="idcnes">
   				<?=campo_texto('codSCNES','S','N','Identificação da ESF',15,10,'','','','','','id=codSCNES','',$scnesId);?>
   			<a href="#" onclick="consultarSCNES()"><img src="../imagens/consultar.gif" id="imgcnes" title="Consultar código SCNES" style="cursor:pointer;" border="0"></a>
   		</td>
	</tr>
	<tr>
   		<td align='right' class="SubTituloDireita">Nº da Equipe:</td>
   		<td id="colequipe">
   			<?=campo_texto('nrequip','S','N','Número da Equipe',10,10,'','','','','','id="nrequip"');?>
   		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="top">Modalidade de ensino</td>
		<td>
			<? $sql ="SELECT
							moeid as codigo, 
							moedsc as descricao
						FROM 
							pse.modalidadeensino"; 
			
			
			$db->monta_combo( "moeid", $sql, 'S', 'Selecione uma modalidade', '', '', '', '215','S','moeid','',$objPseAnoEscolaMun->moeid,'Modalidade de ensino' );?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Nível de ensino</td>
		<td id="td_municipio">
			<? $sql = "SELECT
							nieid as codigo,
							niedsc as descricao
						FROM 
							pse.nivelensino	";

			$db->monta_combo( "nieid", $sql, 'S', 'Selecione um nível', '', '', '', '215', 'S','nieid','',$objPseAnoEscolaMun->nieid, 'Nível de ensino');?>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Quantitativo de Estudantes Previsto</td>
		<td><?=campo_texto('pamquantprevista','S',$habilitado,'Quantitativo de Estudantes previstos',40,7,'','','','','','id="pamquantprevista" onkeypress="return somenteNumeros(event);"','',$objPseAnoEscolaMun->pamquantprevista);?></td>
	</tr>
	<tr id="mostra2009">
		<td align='right' class="SubTituloDireita">Quantitativo de Estudantes Atendido</td>
		<td><?=campo_texto('pamquantatendida','S','S','Quantitativo de Estudantes Atendido',40,7,'','','','','','id="pamquantatendida" onkeypress="return somenteNumeros(event);"','',$objPseAnoEscolaMun->pamquantatendida);?></td>
	</tr>
	<?php if(($nmMunEst=='e' && $pflcod == SECRETARIA_ESTADUAL) || ($nmMunEst=='m' && $pflcod == SECRETARIA_MUNICIPAL) || $pflcod == SUPER_USUARIO){?>	
		<?php 
		if( $resp == NULL ){ ?>
			<tr style="background-color: #DCDCDC">
				<td  align="center" colspan="2">
					<input type="button" value="Salvar" onclick="validaPSE2009();">
				</td>
			</tr>
		<?php } ?>
	<?php } ?>
</table>
</form>
<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td colspan="2">
		<div  id='test' style="display: ''">
			<?php 
				$pamanoreferencia = $_REQUEST['ano'] == null ? 0 :  $_REQUEST['ano'];

				if( $novoEmpid != $empid && $novoEmpid != null ){
					$emp = $empid.",".$novoEmpid;
				} else {
					$emp = $empid;
				}

				$sql = "SELECT ";
						if( $resp == NULL ){
							$sql.= "'<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFormulario('||pam.pamid||',\'pse2009\');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>'";
				//			if( $_SESSION['exercicio'] == $ano){
								$sql.= " || ( CASE WHEN pam.empid = {$empid} THEN '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiFormulario('||pam.pamid||',\'pse2009\');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' ELSE '' END)";
					//		} else {
								$sql.= " as acao,";
					//		}
						}
						 	$sql.= " scnes.cnecodigocnes || ' ' as cnecodigocnes , scnes.cneseqequipe || ' ' as cneseqequipe, ent.entnome,  moe.moedsc, nie.niedsc, pam.pamquantprevista";
						 	
						if( $_REQUEST['ano'] == $_SESSION['exercicio'] ){
						 	$sql.= ", pam.pamquantatendida";
						}
						$sql.= " FROM 
							pse.pseanoescolamun pam
						
						INNER JOIN pse.modalidadeensino AS moe ON moe.moeid = pam.moeid
						INNER JOIN pse.nivelensino AS nie ON nie.nieid = pam.nieid
						INNER JOIN entidade.entidade AS ent ON ent.entid = pam.entid 
						INNER JOIN pse.scnes AS scnes ON scnes.cneid = pam.esfid 
						 
						WHERE
							pam.empid IN ( $emp ) AND
							pam.pamanoreferencia = ".$pamanoreferencia."
						ORDER BY pam.pamid ";

				if( $_REQUEST['ano'] == $_SESSION['exercicio'] ){
					if( $resp == NULL ){
						$cabecalho = array("Opções","ESF","N° Equipe", "Escola", "Modalidade", "Nível", "Previsto", "Realizado");
					} else {			
						$cabecalho = array("ESF","N° Equipe", "Escola", "Modalidade", "Nível", "Previsto", "Realizado");
					}			
				} else {
					if( $resp == NULL ){
						$cabecalho = array("Opções","ESF","N° Equipe", "Escola", "Modalidade", "Nível", "Previsto");
					} else {			
						$cabecalho = array("ESF","N° Equipe", "Escola", "Modalidade", "Nível", "Previsto");
					}			
				}
				$db->monta_lista( $sql, $cabecalho, 25, 10, 'S', '95%', 'N', '');	
				?>
		</div>
		</td>
	</tr>
</table>
<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?=navegacao('representante','materialClinicoDidatico');//volta para representante e vai para materialClinicoDidatico?>	
</table>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript">
	function teste(vr){
		document.formunicipio.idequipe.value = document.getElementById('nrequip').options[document.getElementById('nrequip').selectedIndex].value;
	}
</script>

<?php
$obEstadomunicipiopse = new EstadoMunicipioPSE('',$_SESSION['pse']["muncod"] );
echo "<script language=\"JavaScript\">";
	echo "validaItem('$obEstadomunicipiopse->empparticipapse2009','item2[]')";
echo "</script>";	
?>
<script>
	validaItem(<?php if (pamanoreferencia.value==$_SESSION['exercicio']){?>1<? } else {?>0<? } ?>,'item2[]');
	<?php if ($_REQUEST["idItem"]<>''){ ?>
		verEquipes(<?= $scnesId ?>,<?= $equipe = $objPseAnoEscolaMun->esfid; ?>);
	<? } ?>

	var x = '';
	if (<?php echo $pamanoreferencia ;?>=='<?php echo $_SESSION['exercicio'] ;?>')
	{	
		document.getElementById('mostra2009').style.display = '';
		x = '1';
		validaItem(1,'item2[]')		
	} else {
		document.getElementById('mostra2009').style.display = 'none';
		x = '0';
	}
</script>
<?php
//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

$obPTA->verificaExistePTA( $_SESSION["emenda"]["ptrid"] );
$obPTA->validaSessionPTA( $_SESSION["emenda"]["ptrid"] );
$obPTA->validaSessionPTA( $_SESSION['emenda']['federal'] );
//$obPTA->validaSessionPTA( $_SESSION["emenda"]["refid"] );

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if ($_REQUEST['montaComboIniciativa']) {
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->montaComboIniciativaTipoEnsinoAjax($_POST['iniid'],'');
	exit;
}

// Grava ação
if($_POST) {
	
	if($_POST["ptrid"] && $_REQUEST['ptidescricao'] && $_REQUEST['iniid'] && $_REQUEST['tpeid']) {
		
		$desc = '';
			
		for($i=0; $i<strlen($_REQUEST['ptidescricao']); $i++) {
			if(ord($_REQUEST['ptidescricao'][$i]) != 10 && ord($_REQUEST['ptidescricao'][$i]) != 13) {
				$desc .= $_REQUEST['ptidescricao'][$i];
				$flag = true;
			} elseif($flag) {
				$desc .= " ";
				$flag = false;
			}
		}
			
		if ( !empty($_REQUEST['ptiid']) ) {
			$obPTA->alteraIniciativaPTA( $_POST["ptrid"], $desc, $_REQUEST['iniid'], $_REQUEST['tpeid'], $_REQUEST['ptiid'] );
		}
		else {
			if( !empty($_REQUEST['ptiid']) ){
				$obPTA->alteraIniciativaPTA( $_POST["ptrid"], $desc, $_REQUEST['iniid'], $_REQUEST['tpeid'], $_REQUEST['ptiid'] );	
			} else {
				$ptiid = $obPTA->insereIniciativaPTA( $_POST["ptrid"], $desc, $_REQUEST['iniid'], $_REQUEST['tpeid'] );
				
				/*if( !empty($_REQUEST['ptiid']) && empty( $_POST['ptridpai'] ) && !empty($_POST['refid']) ){
					$ptrid = $obPTA->insereFilhosPTA( '', $_POST['ptrid'], $ptiid );
					
					if( empty($ptrid) ){
						$_SESSION["emenda"]["ptiid"] = $_POST['ptiid'];
						$_SESSION["emenda"]["iniid"] = $_REQUEST['iniid'];
						$_SESSION["emenda"]["ptrid"] = $_POST['ptrid'];
						
						echo "<script>
									alert('Falha na inserção dos filhos do plano de trabalho!');
									location.href='emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&ptiid={$_POST['ptrid']}';
							  </script>";
						exit();
					}
					
					$sql = "UPDATE emenda.ptiniciativa SET ptrid = $ptrid WHERE ptiid = $ptiid";
					$db->executar( $sql );
					$db->commit();
										
					$_SESSION["emenda"]["ptridpai"] = $_POST['ptrid'];
					unset( $_SESSION["emenda"]["ptiid"] );
					unset( $_SESSION["emenda"]["iniid"] );
				} else {*/
					$db->commit();
				//}
				$_SESSION["emenda"]["iniid"] = $_REQUEST['iniid'];
				$_SESSION["emenda"]["ptiid"] = $ptiid;			
				echo "<script>alert('Iniciativa cadastrada com sucesso!');
						  window.location.href='emenda.php?modulo=principal/insereEspecificacaoAcao&acao=A&ptiid={$_SESSION['emenda']['ptiid']}';
				  </script>";
				exit();
			}
		}
	} else {
		echo "<script>
				alert('Erro com o cadastro da iniciativa. Faltam alguns dados do formulário.');
				location.href='emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A';
			  </script>";
	}
}

$entid = $_SESSION["emenda"]["entid"];
$ptrid = $_SESSION["emenda"]["ptrid"];

include  APPRAIZ."includes/cabecalho.inc";

$estadoAtual = $obPTA->pegarEstadoAtual( $_SESSION["emenda"]["ptrid"] );

echo $obPTA->exibeInstituicaoBenefiada( $_SESSION["emenda"]["enbid"] );
print "<br/>";
montaAbasPTA( $abacod_tela, $url, '', $estadoAtual );


monta_titulo( 'Detalhamento da Iniciativa', '<img src="../imagens/obrig.gif" border="0">&nbsp;Indica Campo Obrigatório');

echo $obPTA->cabecalhoPlanoTrabalho( $ptrid );
echo "<br />";

//ALTERA JÁ CADASTRADOS

$dados = $obPTA->carregaDadosAcaoPTA( $_REQUEST['ptiid'] );
		
if($dados){
	$ptidescricao = $dados['ptidescricao'];				 
	$ptiid 		  = $dados['ptiid'];
	$iniid		  = $dados['iniid'];
	$tpeid		  = $dados['tpeid'];
	$_SESSION["emenda"]["ptiid"] = $_REQUEST['ptiid'];
	$_SESSION["emenda"]["iniid"] = $dados['iniid'];
}

if($_SESSION['emenda']['ptiid'] && !$ptiid) 
	$ptiid = $_SESSION['emenda']['ptiid'];

if($ptiid && $ptiid != "")
	echo montarAbasArray(carregaAbasPlanoTrabalho('insereAcaoPlanoTrabalho',$ptiid,$pacid), "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A&evento=A&ptiid=$ptiid");
else
	echo montarAbasArray(carregaAbasPlanoTrabalho('insereAcaoPlanoTrabalho',$ptiid,$pacid), "/emenda/emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A");

unset($_SESSION['emenda']['ptiid']);
if($pacid){
	unset($_SESSION['emenda']['pacid']);
	$_SESSION['emenda']['pacid'] = $pacid;
}
$_SESSION['emenda']['ptiid'] = $ptiid;
monta_titulo( "Iniciativa", '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.');

$arRrefid = verificaTiposReformulacao( $ptrid, 'codigo' );

$disabled = '';
if( in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) || in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arRrefid) ){ 
	$disabled = 'disabled="disabled"';
}

?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<form method="POST"  name="formulario" id="formulario">
<table style="border-bottom:0px"  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
<input type="hidden" name="ptiid" id="ptaid" value="<?=$ptiid;?>">

<tr>
	<?php 
		$sql = $obPTA->montaSQLDadosAcaoPTA( $ptrid, $ptiid );
	?>
    <td align='right' class="SubTituloDireita"><b>Iniciativa:</b></td>
    <td id="td_acao"><?=$db->monta_combo('iniid',$sql,(($iniid)?'N':'S'), 'Selecione...', "montaComboIniciativa(this.value);", '', '', '','S','iniid', '', '', 'Iniciativa');?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><b>Nível de Ensino:</b></td>
	<td><div id="comboTipoEnsino"><?
		if( (in_array(RENDIMENTO_DE_APLICACAO, $arRrefid) || in_array(ALTERACAO_DE_ITENS_DA_ESPECIFICACAO, $arRrefid)) && $tpeid ){ 
			echo $db->pegaUm("SELECT tpedsc FROM emenda.tipoensino WHERE tpeid = $tpeid"); 
			echo '<input type="hidden" name="tpeid" id="tpeid" value="'.$tpeid.'">';
		} else {
			echo $obPTA->montaComboIniciativaTipoEnsinoAjax($iniid, $tpeid);
		} ?></div></td>
</tr> 
<tr>
    <td align='right' class="SubTituloDireita"><b>Detalhamento da Iniciativa:</b></td>
    <td><?=campo_textarea('ptidescricao', 'S', 'S', 'Detalhamento da Iniciativa', 98, 5, 4000, '', '', '', '', 'Detalhamento da Iniciativa');?></td>
</tr> 
<tr bgcolor="#cccccc">
	<td colspan="2" style="text-align: center" >
	     <input type="button" class="" name="btgravar" value="Salvar" onclick="submeter()" />
	</td>
</tr>
</table>
</form>
	
<script type="text/javascript">
function montaComboIniciativa(iniid){
	var myAjax = new Ajax.Request('emenda.php?modulo=principal/insereAcaoPlanoTrabalho&acao=A', {
				        method:     'post',
				        parameters: '&montaComboIniciativa=true&iniid='+iniid,
				        onComplete: function (res){
							$('comboTipoEnsino').update(res.responseText);
				        }
				  });
}
function submeter(){
	var nomeform 		= 'formulario';
	var submeterForm 	= true;
	var campos 			= new Array();
	var tiposDeCampos 	= new Array();
	
	campos[0] 			= "iniid";
	campos[1] 			= "tpeid";
	campos[2] 			= "ptidescricao";

	tiposDeCampos[0] 	= "select";
	tiposDeCampos[1] 	= "select";
	tiposDeCampos[2] 	= "textarea";
	
	// validação para não permitir que usuário insira um texto contendo mais de 4000 caracteres.
	var ptidescricao = document.getElementsByName("ptidescricao")[0];
	
	if( Number(ptidescricao.value.length) > 4000 ) {
		alert("O conteúdo inserido no campo 'Detalhamento da Iniciativa' não deve possuir mais de 4000 caracteres.");
		ptidescricao.focus();
		return;
	}
	
	validaForm(nomeform, campos, tiposDeCampos, submeterForm );			
}

</script>
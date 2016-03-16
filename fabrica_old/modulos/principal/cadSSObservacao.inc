<?
include APPRAIZ ."includes/workflow.php";

$_REQUEST['scsid'] = $_SESSION['fabrica_var']['scsid'];

// validando o ID da ordem de serviço
if(!$_REQUEST['scsid']) {
	die("<script>alert('SS não reconhecida');window.close();</script>");
}

#Pegar Mensura - tabela fabrica.analisesolicitacao
$sql = "
	Select ana.mensuravel
	From fabrica.analisesolicitacao ana
	Where ana.scsid = {$_REQUEST['scsid']}
";
$mensuracao = $db->pegaLinha($sql);
$mensuracao = $mensuracao['mensuravel'];

$sql = "SELECT
            *
        FROM
            fabrica.solicitacaoservico  scs
        inner join
            seguranca.usuario usu
            on usu.usucpf=scs.usucpfrequisitante
        inner join
            fabrica.analisesolicitacao ans
            on ans.scsid=scs.scsid
        WHERE
            scs.scsid={$_REQUEST['scsid']}
        ";

$dados = $db->pegaLinha($sql);
extract($dados);
$usunomerequisitante = $usunome;


$sql2 = "SELECT a.ansgarantia
		FROM fabrica.analisesolicitacao a
		LEFT JOIN fabrica.solicitacaoservico s ON s.scsid=a.scsid
		WHERE a.scsid='".$_SESSION['fabrica_var']['scsid']."'";

$analisesolicitacao = $db->pegaLinha($sql2);

if($analisesolicitacao) {
	$ansgarantia = $analisesolicitacao['ansgarantia'];
}

if($_POST['obsdsc']){

	$sql = "INSERT INTO fabrica.observacoes (
				usucpf,
				scsid,
				obsdsc,
				obsstatus,
				obsdata,
				obstp
			) VALUES (
				'".$_SESSION['usucpf']."',
				'".$_POST['scsid']."',
				'".$_POST['obsdsc']."',
				'A',
				now(),
				'O'
			);";
	$db->executar($sql);

	if($_POST['scsnecessidade']){
		$sql = "UPDATE fabrica.solicitacaoservico   SET
					scsnecessidade = '".$_POST['scsnecessidade']."',
					scsjustificativa = '".$_POST['scsjustificativa']."',
                    usucpfrequisitante = '".$_POST['usucpfrequisitante']."'
				WHERE scsid = ".$_POST['scsid'];
		$db->executar($sql);

                $sql = "UPDATE fabrica.analisesolicitacao SET ansdsc='".$_POST['ansdsc']."' WHERE scsid = ".$_POST['scsid'];
                $db->executar($sql);
	}

	if($_POST['ansgarantia']=='TRUE'){
		if ($_POST['odsidorigem']!=0){
			$sql = "UPDATE fabrica.analisesolicitacao SET ansgarantia='t' where scsid = ".$_POST['scsid'];
			$db->executar($sql);
			
			$sql = "UPDATE fabrica.solicitacaoservico SET odsidorigem=" . $_POST['odsidorigem'] . " where scsid = ".$_POST['scsid'];
			$db->executar($sql);
		}
	}
	if ($_POST['ansgarantia']=='FALSE') {
		$sql = "UPDATE fabrica.analisesolicitacao SET ansgarantia='f' where scsid = ".$_POST['scsid'];
		$db->executar($sql);
		
		$sql = "UPDATE fabrica.solicitacaoservico SET odsidorigem=null where scsid = ".$_POST['scsid'];
		$db->executar($sql);
	}
	
	if ($_POST['mensuravel']=='t'){
		$sql = "UPDATE fabrica.analisesolicitacao SET mensuravel = 't' where scsid = ".$_POST['scsid'];
		$db->executar($sql);
	}
	if($_POST['mensuravel']=='f'){
		$sql = "UPDATE fabrica.analisesolicitacao SET mensuravel = 'f' where scsid = ".$_POST['scsid'];
		$db->executar($sql);
	}
	$db->commit();

	echo "<script>
				alert('Operação realizada com sucesso!');
				location.href = window.location;
		   </script>";
	exit;
}



//permissao para edição da tela
$habil = 'S';
if(!verificaPermissaoEdicao()) $habil = 'N';



?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (BOX) -->
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<!--<script type="text/javascript" src="./js/fabrica.js"></script>-->
<script language="javascript" type="text/javascript">
messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow

function salvarObs(){

	if(document.formulario_.chkeditar.checked == true){
		if(document.formulario_.scsnecessidade.value.length == 0){
			alert("Preencha o campo Necessidade");
			document.formulario_.scsnecessidade.focus();
			return false;
		}
		if(document.formulario_.scsjustificativa.value.length == 0){
			alert("Preencha o campo Justificativa");
			document.formulario_.scsjustificativa.focus();
			return false;
		}
		if(document.formulario_.ansdsc.value.length == 0){
			alert("Preencha o campo Descrição Detalhada");
			document.formulario_.ansdsc.focus();
			return false;
		}
	}
	
	if(document.formulario_.obsdsc.value.length == 0){
		alert("Preencha o campo Observação");
		document.formulario_.obsdsc.focus();
		return false;
	}

	if($('#ansgarantia:checked').val() == true) {
		alert('teste');
		if(document.getElementById('odsidorigem').value.length == 0) {
			alert('Preencha a O.S. garantia');
			return false;
		}
	}

	document.formulario_.submit();
}

function monstraCampos(){
	var tr_necessidade = document.getElementById("tr_necessidade");
	var tr_justificativa = document.getElementById("tr_justificativa");
	var tr_garantia = document.getElementById("tr_garantia");
	var tr_osfinalizadas = document.getElementById("tr_osfinalizadas");
    var tr_requisitante = document.getElementById("tr_requisitante");
    var tr_unidrequisitante = document.getElementById("tr_unidrequisitante");
    var tr_descricaodetalhada = document.getElementById("tr_descricaodetalhada");
    var tr_mensuravel = document.getElementById("tr_mensuravel");
	if(document.formulario_.chkeditar.checked == true){
		tr_necessidade.style.display = '';
		tr_justificativa.style.display = '';
		tr_mensuravel.style.display = '';
		<?php if($mensuracao == 't'){ ?> 
			jQuery("#mensuravel_sim").attr("checked", "checked");
		<?php }elseif($mensuracao == 'f'){?>
			jQuery("#mensuravel_nao").attr("checked", "checked");
		<?php }?>

		tr_requisitante.style.display = '';
		tr_garantia.style.display = '';
		<? if($ansgarantia == "t") { ?>
				$(document).ready(function() {
					IsGarantia('TRUE');
					$("#tr_osfinalizadas").show();
				});
		<? } ?>
		tr_unidrequisitante.style.display = '';
        tr_descricaodetalhada.style.display = '';
	}
	else{
		tr_necessidade.style.display = 'none';
		tr_justificativa.style.display = 'none';
		tr_garantia.style.display = 'none';
		tr_osfinalizadas.style.display = 'none';
        tr_requisitante.style.display = 'none';
        tr_unidrequisitante.style.display = 'none';
        tr_descricaodetalhada.style.display = 'none';
		tr_mensuravel.style.display = 'none';
}
}

function IsGarantia(valor) {

	if(valor == "TRUE") {
		if(document.getElementById('sidid').value == "") {
			alert('Selecione um sistema');
			document.getElementsByName('ansgarantia')[1].checked='checked';
			return false;
		}
		document.getElementById('tr_osfinalizadas').style.display = "";
		document.getElementById('td_osfinalizadas').innerHTML = "Carregando...";

		$.ajax({
	   		type: "POST",
	   		url: "fabrica.php?modulo=principal/analiseDemanda&acao=A",
	   		data: "requisicao=pegarOsProjetoFinalizada&sidid="+document.getElementById('sidid').value,
	   		success: function(msg){
	   			document.getElementById('td_osfinalizadas').innerHTML = msg;
	   		}
	 		});

	 } else {

		document.getElementById('td_osfinalizadas').innerHTML = "";
		document.getElementById('tr_osfinalizadas').style.display = "none";

	 }
}

</script>

<?

include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

if( ($_REQUEST['tipoobs'] == 'analiseDemanda') ){

	$menu = carregarMenuAnaliseSolicitacao();

	if($habil=='N') {
		unset($menu[2],$menu[3],$menu[4]);
	}

	echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

}
elseif( ($_REQUEST['tipoobs'] == 'cadAvaliacaoAprovacao') ){
 	$menu = carregarMenuAvaliacaoSolicitacao();

	if($habil=='N') {
		unset($menu[2],$menu[3]);
	}

	echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
}
elseif( ($_REQUEST['tipoobs'] == 'cadDetalhamento') ){
 	$menu = carregarMenuDetalhamentoSolicitacao();

	if($habil=='N') {
		unset($menu[2],$menu[3],$menu[4]);
	}

	echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
}
elseif( ($_REQUEST['tipoobs'] == 'cadExecucao') ){
 	$menu = carregarMenuExecucaoSolicitacao();
	/*
	if($habil=='N') {
		unset($menu[2],$menu[3],$menu[4]);
	}
	*/
	echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);

}


// buscando dados da OS
$sql = "
	SELECT	scsid, 
			docid, 
			usucpfrequisitante,
			usunome,
			usucpforigem, 
			scsnecessidade, 
			scsjustificativa, 
			sidid
					FROM fabrica.solicitacaoservico ss
	INNER JOIN seguranca.usuario usu
		on (ss.usucpfrequisitante = usu.usucpf)
	WHERE scsid='".$_REQUEST['scsid']."'
";
$ss = $db->pegaLinha($sql);

//Pegar Situação da SS
$sql = "
	SELECT  wkd.esdid,
    		ans.ansid
    FROM fabrica.solicitacaoservico as fss
	INNER JOIN  workflow.documento as wkd on wkd.docid = fss.docid
	INNER JOIN  workflow.estadodocumento as wed on wed.esdid = wkd.esdid
	LEFT JOIN fabrica.analisesolicitacao ans ON ans.scsid = fss.scsid
	WHERE fss.scsid = {$ss['scsid']}
";
$situacaoSolicitacao = $db->pegaLinha($sql);

$link = "<span style='cursor:pointer; color: #0066CC;' onclick=window.location.href='fabrica.php?modulo=principal/abrirSolicitacao&acao=A&scsid=".$ss['scsid']."&ansid=".$situacaoSolicitacao['ansid']."'>".$ss['scsid']."</span>";

// validando se a ordem de serviço existe
if(!$ss) {
	die("<script>alert('SS não existe');window.close();</script>");
}

$estado_wf = wf_pegarEstadoAtual($ss['docid']);
$titulo_modulo = "Observações - SS Nº {$link}";

monta_titulo($titulo_modulo, $estado_wf['esddsc']);


$pfls = arrayPerfil();
$mostraCampo = false;
if(in_array(PERFIL_SUPER_USUARIO, $pfls) ||
   in_array(PERFIL_FISCAL_CONTRATO,  $pfls) ||
   in_array(PERFIL_ADMINISTRADOR, $pfls)){
	$mostraCampo = true;
   }



//verifica se é o requisitante da SS
if($ss['usucpforigem'] == $_SESSION['usucpf']){
	$habil = 'S';
	$mostraCampo = true;
}

//verifica estado do documento
if($estado_wf['esdid'] == WF_ESTADO_FINALIZADA) {
	$habil = "N";
}

?>

<form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">
<input id="sidid" type="hidden" name="sidid" value="<?=$ss['sidid']?>">
<input id="ss-scsid" type="hidden" name="scsid" value="<?=$ss['scsid']?>">
<input type="hidden" name="tipoobs" value="<?=$_REQUEST['tipoobs']?>">
<?if(!$mostraCampo) echo '<input type="hidden" name="chkeditar" value=""> ';?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<?if($mostraCampo && $habil == 'S'){?>
	<tr>
		<td width="35%" class="SubTituloDireita">Editar SS:</td>
		<td>
			<input type="checkbox" name="chkeditar" onclick="monstraCampos();">
		</td>
	</tr>
	
	<?php if($estado_wf['esddsc'] == 'Em Avaliação') { ?>
		<tr id="tr_garantia" style="display:none;">
			<td class="SubTituloDireita">Garantia:</td>
			<td>
				<input type="radio" name="ansgarantia" value="TRUE" 
					<? echo (($ansgarantia=="t")?"checked ":""); echo $disabled_; ?> 
					<? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsGarantia(this.value);}" > Sim
				<input type="radio" name="ansgarantia" value="FALSE" 
					<? echo (($ansgarantia=="f"||!$ansgarantia)?"checked ":""); echo $disabled_; ?> 
					<? echo (($habil=='S')?"":"disabled"); ?> onclick="if(this.checked){IsGarantia(this.value);}" > Não
			</td>
		</tr>
		<tr id="tr_osfinalizadas" style="display:none;">
			<td class="SubTituloDireita">O.S. garantia:</td>
			<td id="td_osfinalizadas"></td>
		</tr>
	<?php } ?>
	
	
	<tr id="tr_necessidade" style="display:none;">
		<td width="35%" class="SubTituloDireita">Necessidade:</td>
		<td>
			<?php $scsnecessidade = $ss['scsnecessidade'];?>
			<? echo campo_textarea( 'scsnecessidade', 'S',  $habil, '', '70', '4', '4000'); ?>
		</td>
	</tr>
	<tr id="tr_justificativa" style="display:none;">
		<td width="35%" class="SubTituloDireita">Justificativa:</td>
		<td>
			<?php $scsjustificativa = $ss['scsjustificativa'];?>
			<? echo campo_textarea( 'scsjustificativa', 'S',  $habil, '', '70', '4', '4000'); ?>
		</td>
	</tr>
    <tr id="tr_requisitante" style="display:none;">
        <td class="SubTituloDireita" width="35%">Requisitante:</td>
		<td>
			<input type="hidden" name="usucpfrequisitante" id="usucpfrequisitante" value="<?php echo $usucpfrequisitante; ?>">
			<?
	
			$usunomerequisitante = $ss['usunome'];// Retorna o nome do usuário requisitante
			echo campo_texto('usunomerequisitante', 'S', 'N', 'Requisitante', 50, 150, '', '', '', '', 0, 'id="usunomerequisitante"' );
	
			$perfis = arrayPerfil();
			if( in_array(PERFIL_SUPER_USUARIO, $perfis) ||
				in_array(PERFIL_FISCAL_CONTRATO, $perfis) ||
				in_array(PERFIL_GESTOR_CONTRATO, $perfis) ||
	            in_array(PERFIL_PREPOSTO, $perfis) ||
				in_array(PERFIL_REQUISITANTE, $perfis) ||
				in_array(PERFIL_ADMINISTRADOR, $pfls) 
			){
				echo "<input type=\"button\" name=\"buscarusuario\" value=\"Outro\" onclick=\"buscarUsuario();\">";
			}
			?>
		</td>
    </tr>
	<tr id="tr_unidrequisitante" style="display:none;">
		<td class="SubTituloDireita">Unidade do Requisitante:</td>
		<td id="unidadedorequisitante"><? pegarUnidadeUsuario(array("usucpf" => $usucpfrequisitante)); ?></td>
	</tr>
	<tr id="tr_descricaodetalhada" style="display:none;">
		<td class="SubTituloDireita">Descrição Detalhada:</td>
		<td><? echo campo_textarea( 'ansdsc', 'S', $habil, '', '70', '4', '5000'); ?></td>
	</tr>
	<?}?>
	<tr>
		<td width="35%" class="SubTituloDireita">Observação:</td>
		<td>
			<? echo campo_textarea( 'obsdsc', 'S',  $habil, '', '70', '4', '4000'); ?>
		</td>
	</tr>
	<tr id="tr_mensuravel" style="display:none;">
		<td width="35%" class="SubTituloDireita">Mensurável:</td>
		<td>
			<?php if($habil == 'S' && $mostraCampo){?>
				<input type="radio" name="mensuravel" id="mensuravel_sim" value="t" />Sim
				<input type="radio" name="mensuravel" id="mensuravel_nao" value="f" />Não
			<?php }?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloEsquerda">&nbsp;</td>
		<td class="SubTituloEsquerda">
			<?php if($habil == 'S'){?>
				<input type="button" name="btn" value="Salvar" onclick="salvarObs();">
			<?php }?>
		</td>
	</tr>
</table>
</form>

<?php

$sql = "
	SELECT	to_char(o.obsdata, 'DD/MM/YYYY HH24:MI:SS') AS data,
			o.obsdsc,
		 	CASE WHEN o.obslog = 'A' 
		 		THEN 'SISTEMA FABRICA'
		 		ELSE u.usunome
		 	END AS nome
	FROM fabrica.observacoes AS o
	LEFT JOIN seguranca.usuario AS u ON u.usucpf = o.usucpf
	WHERE o.scsid = '{$ss['scsid']}' AND o.obsstatus = 'A' AND o.obstp = 'O' AND o.odsid is null 
	ORDER BY o.obsid DESC";
$cabecalho = array("Postado","Observação","Autor");
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '', '', '100%', 'center');
?>
</body>
</html>


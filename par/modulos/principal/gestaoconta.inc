<?php

$perfil = pegaArrayPerfil($_SESSION['usucpf']);

if( !(	in_array( PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) )	){
	echo '<script>alert("Você não tem permissão de acessar essa página."); history.back(-1);</script>';
	die();
}

function carregaLista(){
	global $db;

	if( $_REQUEST['tipo'] == 'PAC' ){
		if($_REQUEST['processo'] != 0){
			$where = "where p.prostatus = 'A'  and  p.pronumeroprocesso = '".$_REQUEST['processo']."'";;
		}
		$sql = "SELECT 
				'<span style=\"white-space: nowrap\" >
				 <img style=\"cursor:pointer\" onclick=\"alteraSequencial( ' || p.proid || ',\'' || m.mundescricao || '\',' || p.pronumeroprocesso || ' ,' || p.probanco || ',' || p.proagencia || ',\'' || p.nu_conta_corrente  || '\' ,' || p.proseqconta || ' )\" src=\"/imagens/alterar.gif\" title=\"Altera Sequencial\" /></span>' AS acao, 
				m.estuf, m.mundescricao, p.pronumeroprocesso , p.probanco, p.proagencia, nu_conta_corrente, p.proseqconta as sequencial,
				CASE WHEN protipo = 'P' THEN 'Proinfância' ELSE 'Quadra' END AS tipo, seq_conta_corrente
			FROM par.processoobra p
			INNER JOIN territorios.municipio m ON m.muncod = p.muncod
			".$where."
			ORDER BY m.estuf, m.mundescricao, p.pronumeroprocesso";
	
		$cabecalho = array("Ação","UF","Município","Número do processo","Banco","Agência",'Número da Conta', 'Sequencial da Conta no SIGEF', 'Tipo da Obra', "seq_conta_corrente");
		
	} elseif( $_REQUEST['tipo'] == 'PAR' ){
		if($_REQUEST['processo'] != 0){
			$where = "where p.prpnumeroprocesso = '".$_REQUEST['processo']."'";
		}
		$sql = "SELECT 
					'<span style=\"white-space: nowrap\" ><img style=\"cursor:pointer\" onclick=\"alteraSequencial( ' || p.prpid || ',\'' || m.mundescricao || '\',\'' || p.prpnumeroprocesso || '\' ,' || p.prpbanco || ',' || p.prpagencia || ',\'' || p.nu_conta_corrente  || '\' ,' || p.prpseqconta || ' )\" src=\"/imagens/alterar.gif\" title=\"Altera Sequencial\" /></span>' AS acao, 
					m.estuf, m.mundescricao, p.prpnumeroprocesso as pronumeroprocesso  , 
				p.prpbanco as probanco, p.prpagencia as proagencia,  nu_conta_corrente, p.prpseqconta as sequencial, seq_conta_corrente
				FROM par.processopar p
				INNER JOIN territorios.municipio m ON m.muncod = p.muncod
				".$where."
				ORDER BY m.estuf, m.mundescricao, p.prpnumeroprocesso";
	
		$cabecalho = array("Ação","UF","Município","Número do processo","Banco","Agência",'Número da Conta', 'Sequencial da Conta no SIGEF', "seq_conta_corrente");
	}
	
	require_once(APPRAIZ."includes/classes/MontaListaAjax.class.inc");
	
	$ajax = new MontaListaAjax($db);
	$ajax->montaLista($sql,$cabecalho,50,5,"N","center",100);
	//dbg($sql,1);
	die();
}

if($_REQUEST['requisicao'] == "salvarSeqConta"){
	$erro = 0;
	if( $_REQUEST['empenhado'] == 'PAC' ){
		if( $_REQUEST['proid'] ){
			$sql = "SELECT proid FROM par.processoobra WHERE prostatus = 'A'  and proid =".$_REQUEST['proid'];
			if( $db->pegaUm( $sql ) ){
				$sql = "UPDATE par.processoobra SET 
							proseqconta = '".$_REQUEST['proseqconta']."',
							probanco = '".$_REQUEST['probanco']."',
							proagencia = '".$_REQUEST['proagencia']."',
							nu_conta_corrente = '".$_REQUEST['nu_conta_corrente']."',
							seq_conta_corrente = '".$_REQUEST['seq_conta_corrente']."'
						WHERE proid =".$_REQUEST['proid'];
			} else {
				$erro = 1;
			}
		} else {
			$erro = 1;
		}
	} elseif( $_REQUEST['empenhado'] == 'PAR' ){
		if( $_REQUEST['proid'] ){
			$sql = "SELECT prpid FROM par.processopar WHERE prpid = {$_REQUEST['proid']} AND prpstatus = 'A'";
			if( $db->pegaUm( $sql ) ){
				$sql = "UPDATE par.processopar SET 
							prpseqconta = '".$_REQUEST['proseqconta']."',
							prpbanco = '".$_REQUEST['probanco']."',
							prpagencia = '".$_REQUEST['proagencia']."',
							nu_conta_corrente = '".$_REQUEST['nu_conta_corrente']."',
							seq_conta_corrente = '".$_REQUEST['seq_conta_corrente']."'
						WHERE prpid =".$_REQUEST['proid'];
				
			} else {
				$erro = 1;
			}
		} else {
			$erro = 1;
		}
	} else {
		$erro = 1;
	}
	
	if( $erro == 0 ){
		$db->executar($sql);
		$db->commit();
		alert('Dados da conta atualizado com sucesso.');
	} else {
		echo '<script>alert("Faltam informações.");
				history.back(-1);</script>';
		die();
	}
	
}

if($_POST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_POST['requisicaoAjax']();
	die;
}

/***************** DECLARAÇÃO DE VARIAVEIS & INSTANCIA OBJETOS ***************/
include APPRAIZ . 'includes/cabecalho.inc';
print '<br />';
monta_titulo( 'Gestão de Contas'.$stTitulo, '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<html>
<head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>
<form name="formularioLista" id="formularioLista" enctype="multipart/form-data"  method="post" action="" >
	<input type="hidden" name="requisicao" value="" />
	<input type="hidden" name="proid" value="<?php echo $proid ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="3" >Pesquisas</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Processo:</td>
			<td colspan="2">
				<?php echo campo_texto("processo","N","S","processo","30","",'',"","","","","","",""); ?>
				<input type="button" value="Pesquisar" onclick="pesqProcesso()">
			</td> 
		</tr>
	</table>
	
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<th colspan="2" > </th>
		</tr>
		<tr>
			<td class="SubTituloDireita" >Tipo:</td>
			<td colspan="2">
				PAR: <input <?php echo $_REQUEST['tipo'] == "PAR" ? "checked='checked'" : "" ?> type="radio" onclick="selecionaTipo(this.value, 0)" name="empenhado" value="PAR" >
				PAC: <input <?php echo $_REQUEST['tipo'] == "PAC" ? "checked='checked'" : "" ?> type="radio" onclick="selecionaTipo(this.value, 0)" name="empenhado" value="PAC" >
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Município:</td>
			<td colspan="2">
				<?php echo campo_texto("mundescricao","N","N","mundescricao","30","",'',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Processo:</td>
			<td colspan="2">
				<?php echo campo_texto("pronumeroprocesso","N","N","pronumeroprocesso","30","",'',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Banco:</td>
			<td colspan="2">
				<?php echo campo_texto("probanco","S","S","probanco","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Agência:</td>
			<td colspan="2"> 
				<?php echo campo_texto("proagencia","S","S","proagencia","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Conta:</td>
			<td colspan="2">
				<?php echo campo_texto("nu_conta_corrente","S","S","nu_conta_corrente","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >Seqüencial:</td>
			<td colspan="2">
				<?php echo campo_texto("proseqconta","S","S","proseqconta","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td class="SubTituloDireita" >seq_conta_corrente</td>
			<td colspan="2">
				<?php echo campo_texto("seq_conta_corrente","N","S","seq_conta_corrente","30","",'[#]',"","","","","","",""); ?>
			</td> 
		</tr>
		<tr>
			<td  class="SubTituloDireita"></td>
			<td><input type="button" id="btn_salvar" name="btn_salvar" value="Salvar" onclick="salvar();"  /></td>
		</tr>
	</table>
	
	
</form>
<div id="lista"></div>

<script type="text/javascript">
	function alteraSequencial(proid, municipio, processo, probanco, proagencia, nu_conta_corrente, sequencial ){
		jQuery("[name=mundescricao]").val(municipio);
		jQuery("[name=pronumeroprocesso]").val(processo);
		
		jQuery("[name=probanco]").val(probanco);
		jQuery("[name=proagencia]").val(proagencia);
		jQuery("[name=nu_conta_corrente]").val(nu_conta_corrente);
		jQuery("[name=proseqconta]").val(sequencial);
		
		jQuery("[name=proid]").val(proid);
		
	}
	
	function salvar(){
		var erro = 0;
		jQuery("[class~=obrigatorio]").each(function() { 
			if(this.value == ''){
				alert('Favor preencher todos os campos obrigatório');
				this.focus();
				erro = 1;
				return false;
			}
		});
	
		if(erro == 0){
			jQuery("[name=requisicao]").val('salvarSeqConta');
			jQuery("#formularioLista").submit();
		}
	}
	
	function selecionaTipo(tipo, processo){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=carregaLista&tipo=" + tipo + "&processo="+ processo,
			success: function(res){
				jQuery("[name=mundescricao]").val('');
				jQuery("[name=pronumeroprocesso]").val('');
				jQuery("[name=proid]").val('');
				jQuery( '#lista' ).html( res );
			}
		});
	}
	
function pesqProcesso(){
	if(jQuery("[name=processo]").val() == ''){
		alert("Campo de Pesquisa Item obrigatório");
		return false;
	}
	var tipo = jQuery("[name=empenhado]").val();
	var processo = jQuery("[name=processo]").val();
	selecionaTipo(tipo, processo);
	
}
	
</script>
</body>
</html>
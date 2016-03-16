
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
    	function exibirOcultarNumeracaoUnica(value) {
	  		var visibilidade = determinarVisibilidadeProcesso(value);
	  		document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = visibilidade;
	  		document.getElementById('linhaNumeracaoUnicaJudicialAntigo').style.visibility = visibilidade;
	  	}	
	  	
	  	function determinarVisibilidadeProcesso(valorSelecionado) {
	  		if( valorSelecionado==2 ) { // 2, conforme ID na base de dados
	  			return "visible";
	  		} else {
	  			reinicializarNumeroProcessoJudicial();
	  			return "collapse";
	  		}
	  	}
	
	  	function reinicializarNumeroProcessoJudicial() {
	  		document.formulario.prcnumeroprocjudicial.value = "";
	  		document.formulario.prcnumeroprocjudantigo.value = "";
	  	}		
    </script>
  </head>
  <body>

<?php 

// ver($_POST,d);
if( $_POST && $_POST['requisicao'] != 'pesquisarprojeto' ){

	foreach ($_POST['prcid'] as $key => $value) {
		
		// verifica pre existencia
			$sql = " 
				SELECT COUNT(*) as total
				FROM cproc.processoanexado 
				WHERE 
					pro_prcid = {$value}
					AND prcid = {$_SESSION['prcidpai']['prcid']} 
			";
			$resultados = $db->carregar( $sql );
			if($resultados[0]['total'] > 0){
				continue;
			}


		$dataAnexacao = date('Y-m-d H:i:s');
		$sql = "
			INSERT INTO cproc.processoanexado ( prcid, pro_prcid, usucpf, pradtanexacao )
			VALUES ( {$_SESSION['prcidpai']['prcid']}, {$value}, '{$_SESSION['usucpf']}', '{$dataAnexacao}' );
		";
		$db->executar( $sql );

		if( !$db->commit() ){
			echo "<script>
				alert('Houve um erro ao salvar anexação de processo. Tente novamente mais tarde.');
				window.history.back();
			</script>";
		}
	}
	echo "<script>
		alert('Processo anexado com sucesso!');
		window.opener.location.reload();
	</script>";
	exit;
}


if( $_GET['prcid'] ){
	$_SESSION['prcidpai'] = $_GET['prcid'];
}

if( !$_GET['prcid'] ){
	echo "<script>
		alert('Processo não identificado.');
		self.close();
	</script>";
	exit;
}

?>


<form name="formulario" id="formulario" method="post">
<input type="hidden" name="requisicao" value="pesquisarprojeto">
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" colspan="2">
			<b>Argumentos da Pesquisa</b>
		</td>
	</tr>
 	<tr>
		<td class="SubTituloDireita">Tipo de Processo :</td>
		<td><?
			$tprid = $_REQUEST['tprid'];
			$sqlTipoProcesso = "SELECT tprid as codigo, tprdsc as descricao 
								FROM cproc.tipoprocesso 
								ORDER BY tprdsc";
			$db->monta_combo('tprid', $sqlTipoProcesso, 'S', 'Selecione...', 'exibirOcultarNumeracaoUnica', '', '', '', 'N', 'tprid');
			?></td>
	</tr>
 	<tr>
		<td style="width:30%;" class="SubTituloDireita">Número do Processo SIDOC :</td>
		<td><?php
			$pcjnumerosidoc = $_REQUEST['prcnumsidoc'];
			echo campo_texto('prcnumsidoc', 'N', 'S', '', 35, 25, '#####.######/####-##', '', 'left', '', 0, '','conjurJs_digitoVerificador(this);','',''); ?></td>
	</tr>
	<tr>
		<td bgcolor="#CCCCCC" colspan="2" align="center">
			<input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="this.disabled=true;document.getElementById('formulario').submit();">  
			<input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="this.disabled=true;window.location='?modulo=principal/vincular_processo&acao=A&prcid=<?=$_GET['prcid']?>&vertodos=sim';">
		</td>
	</tr>
</form>
	<tr>
		<td colspan="2">
		<form name="formulario2" id="formulario2" method="post">
		<?php
		if($_REQUEST['tprid']) {
			$filtroprocesso[] = "tpr.tprid='".$_REQUEST['tprid']."'";
		}
		if($_REQUEST['prcnumsidoc']) {
			$filtroprocesso[] = "prcnumsidoc ilike '%".$_REQUEST['prcnumsidoc']."%'";
		}

		$cabecalho = array('Ações','Nº do Processo SIDOC','Tipo','Interessado');
		$sql = array();
		if( count($filtroprocesso) > 0 || $_REQUEST['vertodos']=='sim' ) {	
			$sql = "
				SELECT '<input type=\'checkbox\' name=\'prcid[]\' id=\'prcid\' value=\''||prc.prcid||'\'>' as img,
						prcnumsidoc,
						tpr.tprdsc,
						esd.esddsc  
		    	FROM cproc.processo prc 
		    	LEFT JOIN cproc.tipoprocesso tpr ON tpr.tprid = prc.tprid 
		    	LEFT JOIN workflow.documento doc ON doc.docid = prc.docid 
		    	LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid  
		    	".((count($filtroprocesso) > 0)?"WHERE ".implode(" AND ", $filtroprocesso):"")."
		    	GROUP BY prc.prcid, prcnumsidoc, tpr.tprdsc, esddsc";
		}
		// ver($sql,d);
		$db->monta_lista_simples($sql,$cabecalho,50,20,'N','100%','');
		?>
		</form>
		</td>
	</tr>
</table>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	<tr>
		<td bgcolor="#CCCCCC" align="left">
			<input type="button" name="vincularProcesso" onclick="submeteFormAnexarProcesso();" value="Anexar" style="cursor:pointer;" onclick="this.disabled=true;window.location.href = '?modulo=principal/cadastrarprocesso&acao=A';">
		</td>
	</tr>
</table>

<script language="javascript" type="text/javascript">

	function submeteFormPesquisa(tipo) {
		document.getElementById("formulario").submit();
	}

	function submeteFormAnexarProcesso() {
		document.getElementById("formulario2").submit();
	}

</script>

</body>
</html>
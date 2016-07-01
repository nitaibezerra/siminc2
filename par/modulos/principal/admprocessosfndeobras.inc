<?php
if( $_REQUEST['req'] == 'carregaDados' && $_REQUEST['proid'] ){
	$sql = "SELECT 
				pp.pronumeroprocesso, 
				pp.muncod, 
				pp.probanco, 
				pp.proagencia, 
				pp.nu_conta_corrente, 
				pp.protipo,
				CASE WHEN m.estuf IS NOT NULL THEN m.estuf ELSE pp.estuf END as estuf,
				pp.procnpj,
				CASE WHEN sisid = 14 THEN 1 ELSE CASE WHEN sisid = 23 THEN 2 ELSE 3 END END as protipoprocesso,
				pp.proseqconta
			FROM 
				par.processoobraspar pp
			LEFT JOIN territorios.municipio m ON m.muncod = pp.muncod
			WHERE
				pp.prostatus = 'A'  and
				pp.proid = ".$_REQUEST['proid'];
	$dados = $db->pegaLinha( $sql );
	echo $dados['pronumeroprocesso'].'|||'.$dados['muncod'].'|||'.$dados['probanco'].'|||'.$dados['proagencia'].'|||'.$dados['nu_conta_corrente'].'|||'.$dados['protipo'].'|||'.$dados['estuf'].'|||'.$dados['procnpj'].'|||'.$dados['protipoprocesso'].'|||'.$dados['proseqconta'];
	die();
}

if( $_REQUEST['req'] == 'carregaMunicipio' && $_REQUEST['estuf'] ){
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf = '".$_REQUEST['estuf']."' ORDER BY mundescricao";
	$db->monta_combo( "muncod", $sql, 'S', 'Selecione', '', '', '', '','N','muncod' );
	die();
}

function listaDados(){
	
	global $db;
	
	$sql = "SELECT 
				'<center> <a style=\"margin: 0 -5px 0 5px;\" href=\"#\" onclick=\"abreDados('|| pp.proid ||')\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a> </center>' as acao,
				pp.pronumeroprocesso, 
				CASE WHEN pp.estuf IS NOT NULL THEN pp.estuf ELSE m.estuf END AS estuf,
				m.mundescricao,
				pp.procnpj,
				pp.probanco, 
				pp.proagencia, 
				pp.prodatainclusao, 
				pp.proseqconta, 
				pp.seq_conta_corrente, 
				pp.nu_conta_corrente, 
				CASE WHEN pp.protipo = 'T' THEN 'Termo de Compromisso' ELSE 'Convênio' END as tipo
			FROM 
				par.processoobraspar pp
			LEFT JOIN territorios.municipio m ON m.muncod = pp.muncod
			WHERE pp.prostatus = 'A' 
			ORDER BY
				m.mundescricao, pp.pronumeroprocesso";

	$cabecalho = Array('Ação','Numero do Processo','Estado','Município','CNPJ','Banco','Agência', 'Data de Inclusão', 'Sequencial da Conta', 'Sequencial da Conta Corrente', 'Número da Conta Corrente', 'Tipo de Execução');
	$db->monta_lista( $sql, $cabecalho, 50, 20, 'N', 'center','N');
}

if( $_POST['pronumeroprocesso'] ){
	if( $_POST['proid'] ){ //UPDATE
		
		if( $_POST['muncod'] ){
			$str = " muncod = '".$_POST['muncod']."', ";
		} else {
			$str = " estuf = '".$_POST['estuf']."', ";
		}
		
		$str2 = '';
		if( $_SESSION['usucpf'] == '' ){
			$str2 = " proseqconta = '".$_POST['proseqconta']."', ";
		}
		
		$sql = "UPDATE
					par.processoobraspar
				SET
					pronumeroprocesso = '".$_POST['pronumeroprocesso']."', 
					{$str}
					{$str2}
					probanco = '".$_POST['probanco']."',
					proagencia = '".$_POST['proagencia']."',
					nu_conta_corrente = '".$_POST['nu_conta_corrente']."',
					protipo = '".$_POST['protipo']."',
					procnpj = '".$_POST['procnpj']."'
				WHERE
					proid = ".$_POST['proid'];
					
		$db->executar($sql);
		$db->commit();

		echo "<script>
				alert('Operação realizada com sucesso!');
				window.location.reload();
			</script>";
		die();	
		
	} else { //INSERT
		
		if( $_POST['protipoprocesso'] == 2 ){ //PAR
			$sistema = 'par';
			$condicao = '';
			$sisid = 23;
		} elseif( $_POST['protipoprocesso'] == 3 ) { // Emendas
			$sistema = 'par';
			$condicao = '';
			$sisid = 57;
		} else { // Brasil-pro
			$sistema = 'cte';
			$condicao = ' AND itrid IN (3,4)';
			$sisid = 14;
		}
		
		$str3 = '';
		$str4 = '';
		if( $_SESSION['usucpf'] == '' ){
			$str3 = ", proseqconta ";
			$str4 = ", '".$_POST['proseqconta']."' ";
		}
		
		if($_POST['muncod']){ //municipal
			$inuid = $db->pegaUm( "SELECT inuid FROM {$sistema}.instrumentounidade WHERE muncod = '".$_POST['muncod']."' {$condicao}" );
			$str = ", muncod ";
			$str2 = ", '".$_POST['muncod']."' ";
		} else { //estadual
			$inuid = $db->pegaUm( "SELECT inuid FROM {$sistema}.instrumentounidade WHERE estuf = '".$_POST['estuf']."' {$condicao}" );
			$str = ", estuf ";
			$str2 = ", '".$_POST['estuf']."' ";
		}
		
		$sql = "SELECT pronumeroprocesso FROM par.processoobraspar WHERE pronumeroprocesso = '".$_POST['pronumeroprocesso']."' and prostatus = 'A' ";
		if( $db->pegaUm( $sql ) ){
			echo "<script>
				alert('O Processo já existe!');
				window.location.reload();
			</script>";
			die();
		} else {
		
			$sql = "INSERT INTO par.processoobraspar(
				            pronumeroprocesso, probanco, proagencia, prodatainclusao, 
				            usucpf, nu_conta_corrente, inuid, protipo, procnpj, sisid {$str} {$str3})
					VALUES
						(
						'".$_POST['pronumeroprocesso']."','".$_POST['probanco']."','".$_POST['proagencia']."',NOW(),
						'".$_SESSION['usucpf']."','".$_POST['nu_conta_corrente']."',{$inuid},'".$_POST['protipo']."','".$_POST['procnpj']."', ".$sisid." {$str2} {$str4})";
	
			$db->executar($sql);
			$db->commit();
	
			echo "<script>
					alert('Operação realizada com sucesso!');
					window.location.reload();
				</script>";
			die();	
		}
		
	}
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../includes/calendario.js"></script>
<div id="loader-container">
	<div id="loader">
		<img src="../imagens/wait.gif" border="0" align="middle">
		<span>Aguarde! Carregando Dados...</span>
	</div>
</div>
<div id="container">
	<form action="" method="post" name="anexaResolucao" id="anexaResolucao" enctype="multipart/form-data" > 
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<input type="hidden" id="resid" name="resid" value="">
			<input type="hidden" id="proid" name="proid" value="">
			<input type="hidden" id="arquivoexistente" name="arquivoexistente" value="">
			<tr>
				<td class="subtituloDireita">
					Número do Processo:
				</td>
				<td>
					<? echo campo_texto('pronumeroprocesso', 'S', 'S', '', 30, 25, '#################', '', 'right', '', 0, 'id="pronumeroprocesso" style="text-align:right;" '); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Estado:
				</td>
				<td>
					<? 
					$sql = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione', 'carregaMunicipio', '', '', '','S','estuf' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Município:
				</td>
				<td id="tdmunicipio">
					<?
					$arr = array(); 
					$db->monta_combo( "muncod", $arr, 'S', 'Selecione', '', '', '', '','N','muncod' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					CNPJ:
				</td>
				<td>
					<? echo campo_texto('procnpj', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="procnpj" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Banco:
				</td>
				<td>
					<? echo campo_texto('probanco', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="probanco" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Agência:
				</td>
				<td>
					<? echo campo_texto('proagencia', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="proagencia" style="text-align:right;" ' ); ?>
				</td>
			</tr><!--
			<tr>
				<td class="subtituloDireita">
					Sequencial da Conta:
				</td>
				<td>
					<? echo campo_texto('proseqconta', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="proseqconta" style="text-align:right;" ' ); ?>
				</td>
			</tr> -->
			<?php if( $_SESSION['usucpf'] == '' ){ ?>
			<tr>
				<td class="subtituloDireita">
					Sequencial da Conta Corrente:
				</td>
				<td>
					<? echo campo_texto('proseqconta', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="proseqconta" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<?php } ?>
			<tr>
				<td class="subtituloDireita">
					Número da Conta Corrente:
				</td>
				<td>
					<? echo campo_texto('nu_conta_corrente', 'N', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="nu_conta_corrente" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Tipo de Execução:
				</td>
				<td>
					<? 
					$arr = array(
								array('codigo' => 'T', 'descricao' => 'Termo de Compromisso'),
								array('codigo' => 'C', 'descricao' => 'Convênio')
							);
					$db->monta_combo( "protipo", $arr, 'S', 'Selecione', '', '', '', '','S','protipo' );
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Tipo de Processo - Obra:
				</td>
				<td>
					<? 
					$arr = array(
								array('codigo' => '1', 'descricao' => 'Brasil-Pró'),
								array('codigo' => '2', 'descricao' => 'PAR'),
								array('codigo' => '3', 'descricao' => 'Emenda')
							);
					$valor = 2;
					$db->monta_combo( "protipoprocesso", $arr, 'S', 'Selecione', '', '', '', '','S','protipoprocesso', '', $valor );
					?>
				</td>
			</tr>
			<!--<tr>
				<td class="subtituloDireita">
					Número do Convênio FNDE:
				</td>
				<td>
					<? echo campo_texto('pronumeroconveniofnde', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="pronumeroconveniofnde" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Ano do Convênio FNDE:
				</td>
				<td>
					<? echo campo_texto('proanoconveniofnde', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="proanoconveniofnde" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Número do Convênio SIAFI:
				</td>
				<td>
					<? echo campo_texto('pronumeroconveniosiafi', 'S', 'S', '', 30, 25, '', '', 'right', '', 0, 'id="pronumeroconveniosiafi" style="text-align:right;" ' ); ?>
				</td>
			</tr>
			-->
			<tr>
				<td class="subtituloDireita">
				</td>
				<td>
					<input type="button" name="salvar" value="Salvar" onclick="valida( this.form )">&nbsp;<input type="button" name="limpar" value="Limpar" onclick="limpa()">
				</td>
			</tr>
		</table>
	</form>
	<tr>
		<td id="tdLista" colspan="2">
			<?php listaDados();?>
		</td>
	</tr>
</div>
<script>
$('loader-container').hide();

var tdLista = $('tdLista');



function carregaMunicipio( estuf ){
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaMunicipio&estuf="+estuf,
		onComplete: function (msg){
	   		$('tdmunicipio').innerHTML = msg.responseText;
	   	}
	 });	
}

function abreDados( proid ){
	
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaDados&proid="+proid,
		onComplete: function (msg){
	   		mensagem = msg.responseText;
	   		dados = mensagem.split('|||');
	   		document.getElementById('proid').value = proid;
	   		document.getElementById('pronumeroprocesso').value = dados[0];
	   		document.getElementById('estuf').value = dados[6];
	   		document.getElementById('probanco').value = dados[2];
	   		document.getElementById('proagencia').value = dados[3];
	   		document.getElementById('nu_conta_corrente').value = dados[4];
	   		document.getElementById('protipo').value = dados[5];
	   		document.getElementById('protipoprocesso').value = dados[8];
	   		document.getElementById('procnpj').value = dados[7];
	   		carregaMunicipio( dados[6] );
	   		document.getElementById('muncod').value = dados[1];
	   		document.getElementById( 'protipoprocesso' ).disabled = true;
	   		document.getElementById('proseqconta').value = dados[9];
	   	}
	 });	

}

function limpa(){
	document.getElementById('proid').value = "";
	document.getElementById('pronumeroprocesso').value = "";
   	document.getElementById('estuf').value = "";
   	document.getElementById('muncod').value = "";
   	document.getElementById('probanco').value = "";
   	document.getElementById('proagencia').value = "";
   	document.getElementById('nu_conta_corrente').value = "";
   	document.getElementById('procnpj').value = "";
   	document.getElementById('protipo').value = "";
   	document.getElementById('protipoprocesso').disabled = false;
   	document.getElementById('proseqconta').value = "";
}


function valida( form ){
	var pronumeroprocesso = document.getElementById('pronumeroprocesso');
	var probanco 	  	  = document.getElementById('probanco');
	var proagencia 		  = document.getElementById('proagencia');
	var nu_conta_corrente = document.getElementById('nu_conta_corrente');
	var protipo	  		  = document.getElementById('protipo');
	var protipoprocesso   = document.getElementById('protipoprocesso');
	var procnpj			  = document.getElementById('procnpj');
	var estuf			  = document.getElementById('estuf');

	if( pronumeroprocesso.value == '' ){
		alert('Campo Número do Processo Obrigatório!');
		pronumeroprocesso.focus();
		return false;
	}

	if( pronumeroprocesso.value.length != 17 ){
		alert('O Número do Processo está Errado!');
		pronumeroprocesso.focus();
		return false;
	}

	if( pronumeroprocesso.value.substring(11, 15) > 2015 || pronumeroprocesso.value.substring(11, 15) < 2000 ){
		alert('Verifique o Ano do Número do Processo!');
		pronumeroprocesso.focus();
		return false;
	}
	
//	if( probanco.value == '' ){
//		alert('Campo Banco Obrigatório!');
//		resdescricao.focus();
//		return false;
//	}
//	if( proagencia.value == '' ){
//		alert('Campo Agência Obrigatório!');
//		resdatapublicacao.focus();
//		return false;
//	}
//	if( nu_conta_corrente.value == '' ){
//		alert('Campo Número da Conta Corrente Obrigatório!');
//		treid.focus();
//		return false;
//	}
	if( protipo.value == '' ){
		alert('Campo Tipo de Execução Obrigatório!');
		protipo.focus();
		return false;
	}
	if( protipoprocesso.value == '' ){
		alert('Campo Tipo de Processo - Obra Obrigatório!');
		protipoprocesso.focus();
		return false;
	}
	if( estuf.value == '' ){
		alert('Campo Estado Obrigatório!');
		estuf.focus();
		return false;
	}
	if( protipoprocesso.value == 1 ){
		if( procnpj.value == '' ){
			alert('Campo CNPJ Obrigatório!');
			procnpj.focus();
			return false;
		}
	}
	form.submit();
}

</script>

<?php
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_REQUEST['req'] == 'download' ){
	$file = new FilesSimec('resolucao',null,'par');
	return $file->getDownloadArquivo($_REQUEST['arqid']);
	die();
}

if( $_REQUEST['req'] == 'carregaItem' && $_REQUEST['residAjax'] && $_REQUEST['arqidAjax'] ){
	$sql = "SELECT * FROM par.resolucao res INNER JOIN public.arquivo ar ON ar.arqid = res.arqid WHERE resid = ".$_REQUEST['residAjax'];
	$res = $db->pegaLinha( $sql );
	echo $res['resnumero'].'|||'.$res['resanoreferencia'].'|||'.$res['resdescricao'].'|||'.date("d/m/Y",strtotime($res['resdatapublicacao'])).'|||'.$res['treid'].'|||'.$res['arqid'].'|||'.$res['resid'].'|||'.$res['arqnome'].'.'.$res['arqextensao'];
	die();
}

function listaResolucao(){
	
	global $db;
	
	$sql = "SELECT 
				'<img onclick=\"alterarResolucao('|| res.arqid ||', '|| resid ||')\" src=\"../imagens/alterar.gif\" align=\"absmiddle\" title=\"Alterar resolução\" style=\"cursor:pointer;padding-right:5px;padding-bottom:5px;\" />' 
				||' '|| '<img onclick=\"excluirResolucao('|| res.arqid ||')\" src=\"../imagens/excluir.gif\" align=\"absmiddle\" title=\"Excluir resolução\" style=\"cursor:pointer;padding-right:5px;padding-bottom:5px;\" />' as acao,
				resnumero, 
				'<a onclick=\"baixarArquivo('|| res.arqid ||')\" cursor=\"pointer\">' || arqnome || '.' || arqextensao || '</a>'as arquivo,
				to_char(resdatapublicacao,'DD/MM/YYYY'), 
				tredescricao,
				CASE
					WHEN resanoreferencia IS NOT NULL THEN resanoreferencia
					ELSE 0
				END as resanoreferencia
			FROM 
				par.resolucao res
			INNER JOIN par.tiporesolucao tre ON tre.treid = res.treid
			INNER JOIN public.arquivo    arq ON arq.arqid = res.arqid
			WHERE
				resstatus = 'A'";

	$cabecalho = Array('Ação','Numero da Resolução','Resolução','Data da publicação','Tipo da Resolução', 'Ano Referência');
	$db->monta_lista( $sql, $cabecalho, 20, 10, 'N', 'center','N','anexaResolucao',Array('10%','20%','30%','20%','20%'),Array('center','right','center','center','center') );
}

if( $_POST['resnumero']) {
	if( $_POST['arquivoexistente'] ){
		//update resolucao
		$sql = "UPDATE 
					par.resolucao 
				SET 
					resnumero 			= ".$_POST['resnumero'].", 
					resdatapublicacao 	= '".formata_data_sql($_POST['resdatapublicacao'])."', 
					treid 				= ".$_POST['treid'].", 
					resstatus 			= 'A', 
					resdatainclusao 	= '".date("Y-m-d G:i:s")."', 
					usucpf 				= '".$_SESSION['usucpf']."', 
					resdescricao 		= '".$_POST['resdescricao']."', 
					resanoreferencia 	= ".$_POST['resanoreferencia']."
				WHERE
					resid 				= ".$_REQUEST['resid'];
//		dbg($sql,1);	
		$db->executar($sql);
		$db->commit();

		echo "<script>
				alert('Operação realizada com sucesso!');
				window.location.href = window.location;
			</script>";
		die();	
	
	} elseif( $_FILES['arqid']['error'] == 0 ){
		if( $_REQUEST['resid'] ){
			$sql = "DELETE FROM par.resolucao WHERE resid = ".$_REQUEST['resid'];
			$db->executar( $sql );
		}
		$campos	= array("resnumero"		    => $_POST['resnumero'],
						"resanoreferencia"	=> $_POST['resanoreferencia'],
						"resdescricao"		=> "'".$_POST['resdescricao']."'",
					    "resdatapublicacao" => "'".formata_data_sql($_POST['resdatapublicacao'])."'",
						"treid"             => $_POST['treid'],
					    "resstatus"			=> "'A'",
						"resdatainclusao"	=> "'".date("Y-m-d G:i:s")."'",
						"usucpf"			=> "'".$_SESSION['usucpf']."'");
		$file = new FilesSimec("resolucao", $campos, 'par');
	//	if( substr($_FILES["Arquivo"]["type"],0,5) == 'image' ){
			$arquivoSalvo = $file->setUpload($_POST['fotdescricao']);	
	//	}else{
	//		echo '<script type="text/javascript"> 
	//					alert("Tipo de arquivo inválido. \n  ");
	//			  </script>';
	//	}
		if($arquivoSalvo){
			$_REQUEST['acao'] = 'I';
			$db->sucesso("sistema/cadastro/cadastroresolucao");
		}
	}
}

if( $_POST['req'] == 'excluirRel' ){
	$sql = "UPDATE par.resolucao SET
				resstatus = 'I'
			WHERE
				arqid = {$_POST['arqid']};
			UPDATE public.arquivo SET
				arqstatus = 'I'
			WHERE
				arqid = {$_POST['arqid']};";
	$db->executar($sql);
	$db->commit();
	listaResolucao();
	die();
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

echo carregaAbasResolucao('par.php?modulo=sistema/cadastro/cadastroresolucao&acao=I');
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
			<input type="hidden" id="arquivoexistente" name="arquivoexistente" value="">
			<tr>
				<td class="subtituloDireita">
					Numero:
				</td>
				<td>
					<? echo campo_texto('resnumero', 'S', 'S', '', 30, 25, '####################', '', 'right', '', 0, 'id="resnumero" style="text-align:right;" '); ?>
				</td>
			</tr>
			<tr>
				<td width="25%" class="SubtituloDireita" >Ano Referência:</td>
				<td>
					<select id="resanoreferencia" name="resanoreferencia">
						<option value="2011" selected="selected">2011</option>
						<option value="2010">2010</option>
						<option value="2009">2009</option>
						<option value="2008">2008</option>
						<option value="2007">2007</option>
					</select>
				</td>
			</tr>
			<tr>
			<tr>
				<td class="subtituloDireita">
					Descrição:
				</td>
				<td>
					<? echo campo_texto('resdescricao', 'S', 'S', '', 100, 100, '', '', 'right', '', 0, 'id="resdescricao" style="text-align:left;" '); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Data da Publicação:
				</td>
				<td>
					<?= campo_data2( 'resdatapublicacao', 'S', 'S', '', 'S', '', '' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Tipo de Resolução:
				</td>
				<td>
					<?php 
						$sql = "SELECT
									treid as codigo,
									tredescricao as descricao
								FROM
									par.tiporesolucao
								WHERE
									trestatus = 'A'";
					
						echo $db->monta_combo('treid', $sql, 'S', 'Selecione...', '', '', '', '190', 'S', 'treid' ); 
					?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Resolução(Arquivo):
				</td>
				<td id="arquivo">
					<input type="file" name="arqid" id="arqid">
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
				</td>
				<td>
					<input type="button" name="anexar" value="Anexar Resolução" onclick="valida( this.form )">
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr style="background-color: rgb(230,230,230); ">
				<td align="center" style="font-size: 12px;">
					Lista de Resoluções
				</td>
			</tr>
			<tr>
				<td id="tdLista" colspan="2">
					<?php listaResolucao();?>
				</td>
			</tr>
		</table>
	</form>
</div>
<script>
$('loader-container').hide();

var tdLista = $('tdLista');

function valida( form ){
	var resnumero 		  = document.getElementById('resnumero');
	var resanoreferencia  = document.getElementById('resanoreferencia');
	var resdescricao 	  = document.getElementById('resdescricao');
	var resdatapublicacao = document.getElementById('resdatapublicacao');
	var treid			  = document.getElementById('treid');
//	var arqid			  = document.getElementById('arqid');

	if( resnumero.value == '' ){
		alert('Compo Obrigatório!');
		resnumero.focus();
		return false;
	}
	if( resanoreferencia == '' ){
		alert('Compo Obrigatório!');
		resanoreferencia.focus();
		return false;
	}
	if( resdescricao.value == '' ){
		alert('Compo Obrigatório!');
		resdescricao.focus();
		return false;
	}
	if( resdatapublicacao.value == '' ){
		alert('Compo Obrigatório!');
		resdatapublicacao.focus();
		return false;
	}
	if( treid.value == '' ){
		alert('Compo Obrigatório!');
		treid.focus();
		return false;
	}
/*	if( arqid.value == '' ){
		alert('Compo Obrigatório!');
		arqid.focus();
		return false;
	}*/
	form.submit();
}

function alterarResolucao( arqid, resid ){
	//$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: "req=carregaItem&arqidAjax="+arqid+"&residAjax="+resid,
		onComplete: function (msg){
	   		mensagem = msg.responseText;
	   		dados = mensagem.split('|||');
	   		document.getElementById('resnumero').value = dados[0];
	   		document.getElementById('resanoreferencia').value = dados[1];
	   		document.getElementById('resdescricao').value = dados[2];
	   		document.getElementById('resdatapublicacao').value = dados[3];
	   		document.getElementById('treid').value = dados[4];
	   		document.getElementById('arquivoexistente').value = dados[5];
	   		document.getElementById('arquivo').innerHTML = '';
	   		document.getElementById('arquivo').innerHTML = dados[7] + '&nbsp;&nbsp;<img title="Alterar anexo" style="border: 0pt none ; cursor: pointer;" onclick="novoAnexo();" src="../imagens/alterar.gif">';
	   		document.getElementById('resid').value = dados[6];
	   		document.getElementById('arqid').style.display = 'none';
	   	}
	 });
}

function novoAnexo(){
	if( confirm( 'Tem certeza que deseja alterar o arquivo?' ) ){
	   	document.getElementById('arquivo').innerHTML = '<input type="file" id="arqid" name="arqid">';
	   	document.getElementById('arquivoexistente').value = '';
	}
}

function excluirResolucao( arqid ){

	if( confirm('Realmente deseja excluir esta resolução?') ){
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=excluirRel&arqid='+arqid,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				alert('Resolução excluida.');
				tdLista.innerHTML = res.responseText;
			}
		});	
	}
}

function baixarArquivo( arqid ){

	window.open('par.php?modulo=sistema/cadastro/cadastroresolucao&acao=I&req=download&arqid='+arqid, 
			   'modelo', 
			   "height=1,width=1,scrollbars=yes,top=0,left=0" );
}
</script>

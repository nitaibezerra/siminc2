<?php
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $_REQUEST['req'] == 'download' ){
	
	$sql = "SELECT arqid FROM par.resolucao WHERE resid = ".$_REQUEST['resid'];
	
	$arqid = $db->pegaUm($sql);
	
	if( $arqid ){
		$file = new FilesSimec('resolucao',null,'par');
		return $file->getDownloadArquivo($arqid);
	}else{
		echo "<script type=\"text/javascript\">alert('Sem arquivo anexado.');window.close;</script>";
	}
	die();
}

if( $_REQUEST['req'] == 'desvinculaObra' ){
	
	if( $_POST['preid'] ){
		$sql = "UPDATE obras.preobra
				SET
					resid = NULL
				WHERE
					preid = ".$_REQUEST['preid'];
		
		$db->executar($sql);
		$db->commit();
		
		lista_obra( $_POST['resid'] );
	}
	die();
}

if( $_POST['resid'] && $_POST['preid'] ){
	
	$sql = '';
	foreach( $_POST['preid'] as $preid ){
		$sql .= "UPDATE obras.preobra
				 SET 
					resid = ".$_POST['resid']."
				 WHERE 
					preid = ".$preid.";\n";
	}
	$db->executar($sql);
	$db->commit();
	$_REQUEST['acao'] = 'I';
	$db->sucesso("sistema/cadastro/vincularesolucao");
}

function lista_obra( $resid ){
	global $db;

	$sql = "SELECT 
				'<img onclick=\"desvincularObra('|| pre.preid ||','|| pre.resid ||')\" src=\"../imagens/reject.png\" align=\"absmiddle\" title=\"Desvincular Obra\" style=\"cursor:pointer;padding-right:5px;padding-bottom:5px;\" />' as acao,
				preid as codigo, 
				predescricao || ' - ' || mun.mundescricao || ' / ' || mun.estuf as descricao
			FROM 
				obras.preobra pre
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
			WHERE
				pre.resid = {$resid}
			ORDER BY
				mun.estuf,mun.mundescricao,pre.predescricao";
	
	if( $resid != '' ){
		return $db->monta_lista($sql, Array('Ação','Código','Obra'), 100, 100, '', '', '', '', Array('5%','5%','90%'), Array('center','center','left') ); 
	}else{
		return ''; 
	}
}

if( $_POST['req'] == 'lista_obra' ){
	
	echo lista_obra( $_POST['resid'] );
	die;
}

function combo_municipio( $uf ){
	global $db;
	
	$sql = "SELECT 
				muncod AS codigo, 
				estuf || ' - ' || mundescricao AS descricao
		  	FROM 
		  		territorios.municipio
		  	WHERE
		  		estuf = '{$uf}'
		  	ORDER BY
		  		mundescricao;";
	
	if( $uf != '' ){
		return $db->monta_combo('muncod', $sql, 'S', 'Selecione...', 'filtraObra', '', '', '190', 'N', 'muncod' ); 
	}else{
		return $db->monta_combo('muncod', $sql, 'N', 'Selecione os filtros', '', '', '', '190', 'N', 'muncod' ); 
	}
}

if( $_POST['req'] == 'combo_municipio' ){
	
	echo combo_municipio( $_POST['uf'] );
	die;
}

function combo_obra( $muncod ){
	global $db;
	
	$sql = "SELECT 
				preid as codigo, 
				predescricao || ' - ' || mun.mundescricao || ' / ' || mun.estuf as descricao
			FROM 
				obras.preobra pre
			INNER JOIN territorios.municipio mun ON mun.muncod = pre.muncod
			WHERE
				mun.muncod = '{$muncod}' AND pre.resid IS NULL";
	
	$stSqlCarregados = "";
	if( $muncod != '' ){
		return combo_popup( 'preid', $sql, 'Selecione a(s) Obras(s)', '400x400', 0, array(), '', 'S', false, false, 5, 400, null, null, false, null, null, true, false);
	}else{
		return $db->monta_combo('preid[]', $sql, 'N', 'Selecione os filtros', '', '', '', '190', 'N', 'preid[]' );
	}
	
	
}
if( $_POST['req'] == 'combo_obra' ){
	
	echo combo_obra( $_POST['muncod'] );
	die;
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';

echo carregaAbasResolucao('par.php?modulo=sistema/cadastro/vinculoresolucao&acao=I');
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
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3"><!--
			<tr>
				<td class="subtituloDireita">
					Tipo de Resolução:
				</td>
				<td>
					<? 
//						$sql = "SELECT 
//									muncod AS codigo, 
//									mundescricao AS descricao
//							  	FROM 
//							  		territorios.municipio
//							  	ORDER BY
//							  		mundescricao";
//						
//						$stSqlCarregados = "";
//						$arWhere = array();
//						$arWhere[] = array("codigo" => "estuf",
//										   "descricao" => "Estado");
//						mostrarComboPopup( 'Município', 'muncod',  $sql, $stSqlCarregados, 'Selecione o(s) Município(s)', $arWhere );
					?>
				</td>
			</tr>
			--><tr>
				<td class="subtituloDireita">
					Resolução:
				</td>
				<td>
					<?php 
						$sql = "SELECT 
									resid as codigo, 
									resdescricao as descricao
								FROM 
									par.resolucao
								WHERE
									resstatus = 'A'";
					
						echo $db->monta_combo('resid', $sql, 'S', 'Selecione...', 'listaObra', '', '', '190', 'N', 'resid' ); 
					?>
					&nbsp;
					<img id="imgRes" style="cursor: pointer; display: none" title="Baixar Resolução" onclick="baixarArquivo()" src="/imagens/consultar.gif"/>
					<a id="aRes"  style="cursor: pointer; display: none"  onclick="baixarArquivo()">Vizualizar resolução.</a>
				</td>
			</tr>
			<tr style="background-color: rgb(230,230,230); ">
				<td align="center" style="font-size: 12px;" colspan="2">
					Filtro de Obras a Vincular:
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Estado(s)</td>
				<td>
					<div id="divEstuf">
					<?php
				
						$sql = "SELECT 
									estuf AS codigo, 
									estdescricao AS descricao
					 			FROM 
					 				territorios.estado;";
						
						echo $db->monta_combo('estid', $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', '190', 'N', 'estid' ); 
					?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top">Municipio(s)</td>
				<td>
					<div id="divMunicipio">
					<?php
				
						$sql = "SELECT 
									muncod AS codigo, 
									mundescricao AS descricao
							  	FROM 
							  		territorios.municipio
							  	ORDER BY
							  		mundescricao;";
						
						echo $db->monta_combo('muncod', $sql, 'N', 'Selecione os filtros', '', '', '', '190', 'N', 'muncod' ); 
					?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
					Obras:
				</td>
				<td>
					<div id="divObra">
						<?php 
							echo $db->monta_combo('preid[]', $sql, 'N', 'Selecione os filtros', '', '', '', '190', 'N', 'preid' ); 
						?>
					</div>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita">
				</td>
				<td>
					<input type="button" value="Vincular" onclick="valida(this.form)" />
				</td>
			</tr>
		</table>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr style="background-color: rgb(230,230,230); ">
				<td align="center" style="font-size: 12px;">
					Lista de Obras da Resolução:
				</td>
			</tr>
			<tr>
				<td colspan="2">
					<div id="divObraRes">
					</div>
				</td>
			</tr>
		</table>
	</form>
</div>
<script>
$('loader-container').hide();

function valida( form ){

	var resid = $('resid');
	var preid = $('preid');

	if( resid.value == '' ){
		alert('Escolha uma resolução!');
		return false;
	}
	
	var obras = '';
	if( !preid.disabled && preid.options[0].value != '' ){
		for(x=0;x<preid.length;x++){
			obras += '- '+preid.options[x].text+'\n';
		}
		if( confirm('Reamente deseja vincular a resolução '+resid.options[resid.selectedIndex].text+' as obras abaixo?\n'+obras) ){
			selectAllOptions( preid );
			
			form.submit();
		}
	}else{
		alert('Escolha, pelo meons, uma obra!');
		return false;
	}
}

function filtraMunicipio( uf ){

	var div = $('divMunicipio');
	
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: '&req=combo_municipio&uf='+uf,
		onLoading: function (){
			$('loader-container').show();
		},
		onComplete: function(res){
			div.innerHTML = res.responseText;
			$('divObra').innerHTML = '<select id="muncod" disabled="disabled" style="width: 190px;" class="CampoEstilo" name="muncod_disable">'+
										'<option value="">Selecione os filtros</option>'+
									 '</select>';
		}
	});	
}

function filtraObra( muncod ){

	var div = $('divObra');
	
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: '&req=combo_obra&muncod='+muncod,
		onLoading: function (){
			$('loader-container').show();
		},
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});	
}

function abreArquivo( resid ){
	var img = $('imgRes');
	var a   = $('aRes');
	if( resid != '' ){
		img.style.display = '';
		a.style.display = '';
	}else{
		img.style.display = 'none';
		a.style.display = 'none';
	}
}

function desvincularObra( preid, resid ){
	
	var div = $('divObraRes');
	if( confirm('Realmente deseja desvincular a obra \''+preid+'\'?') ){
		new Ajax.Request(window.location.href,{
			method: 'post',
			asynchronous: false,
			parameters: '&req=desvinculaObra&preid='+preid+'&resid='+resid,
			onLoading: function (){
				$('loader-container').show();
			},
			onComplete: function(res){
				div.innerHTML = res.responseText;
			}
		});	
	}
}

function listaObra( resid ){

	abreArquivo( resid );
	
	var div = $('divObraRes');
	
	new Ajax.Request(window.location.href,{
		method: 'post',
		asynchronous: false,
		parameters: '&req=lista_obra&resid='+resid,
		onLoading: function (){
			$('loader-container').show();
		},
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});	
}

function baixarArquivo( ){

	var resid = $('resid');

	if( resid.value == '' ){
		alert('Escolha uma resolução');
		resid.focus();
		return false;
	}
	window.open('par.php?modulo=sistema/cadastro/vinculoresolucao&acao=I&req=download&resid='+resid.value, 
			    'modelo', 
			    "height=1,width=1,scrollbars=yes,top=0,left=0" );
}
</script>

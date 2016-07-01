<?php 
if( $_POST['iescodResp']){
	$_SESSION['iescodSession'] = array();
	$arResp = explode( ",", $_POST['iescodResp']);
			
	echo "<table class=\"tabela_listagem\" width=\"600px\" id=\"listaInstituicao\">
          		  	<tr>
          		  		<th>Ação</th>
          		  		<th>Sigla</th>
          		  		<th>Instituição</th>
          		  		<th>UF</th>
          		  	</tr>";
          		  	 
          	   
	for( $i = 0; $i< count( $arResp ); $i++ ){
		if( $arResp[$i] != '' ){
			
			array_push($_SESSION['iescodSession'] , $arResp[$i]);
			$sql = "select iesid, iescodigo, iessigla, iesnome, iesuf FROM ies.ies where iesid = ".$arResp[$i];
			$rs = $db->carregar( $sql );
			
			echo "<tr>
					<td> <img src=\"/imagens/excluir.gif\" onclick=\"excluiIescodigo(".$rs[0]['iesid'] .");\" border=0 style=\"cursor: pointer;\"></img> </td>
					<td> ".$rs[0]['iessigla'] ."</td>
					<td> ".$rs[0]['iesnome'] ."</td>
					<td> ".$rs[0]['iesuf'] ."</td>
				  </tr>"; 
		}
	} 
	echo "</table>";
	die();
}
if( $_POST['ajaxestuf'] ){	
 
	header('content-type: text/html; charset=ISO-8859-1'); 
	$sql = "select
			 mundescricao
			from
			 territorios.municipio 
			where
			 estuf = '".$_POST['ajaxestuf']."' 
			order by
			 mundescricao asc";

	$rs = $db->carregar( $sql );
	echo "<select name=\"mundescricao\" id=\"mundescricao\" class=\"CampoEstilo\">";
	echo "<option value=\"\">Selecione...</option>";
	foreach( $rs as $r){
		echo "<option value=\"".$r['mundescricao']."\">".$r['mundescricao']."</option>";
	}
	echo "</select>";
	die();
} 

if( is_numeric($_POST['iescodRadio']) ){
	$sql = "select iesid, iescodigo, iessigla, iesnome, iesuf FROM ies.ies where iesid = ".$_POST['iescodRadio'];
	$rs  = $db->carregar( $sql );
	if( $rs ){
		$_SESSION['iescodSessionEntidade'] = $_POST['iescodRadio'];
		$resp = "<a href=\"javascript: abrirPopupInstituicao('radio');\">".$rs[0]['iescodigo']." - ".$rs[0]['iessigla']." - ".$rs[0]['iesnome']."</a>";
		die( $resp );
	}
}

if( $_POST['acaoForm'] == 'calcelar' ){
	unset( $_SESSION['iescodSession'] );
}

?>
<html>
  <head>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
  </head>
<br>    
<form name="formulario" id="formulario" method="post">
<table  bgcolor="#f5f5f5" align="center" class="tabela" >
<input type="hidden" name="acaoForm" id="acaoForm" value="1" />
<input type="hidden" name="tarid" id="tarid" value="" />
	<tr>
		<td class = "subtitulodireita" colspan="2">
			<center> 
			<h3>Busca de instituição</h3>
			</center>
		</td> 
	</tr>
	<tr>
		<td class = "subtitulodireita"> 
			Código:
		</td>
		<td>
			<?= campo_texto( 'iescodigo', 'N', $habilitado, 'Tema', 60, 50, '###################', '','','','','id="iescodigo"', '', ''); ?>
		</td>
	</tr>
	<tr id="tr_tipo_pessoa">
		<td class = "subtitulodireita"> 
			 Nome:
		</td>
		<td>
			<?= campo_texto( 'iesnome', 'N', $habilitado, 'Tema', 60, 50, '', '','','','','id="iesnome"', '', ''); ?>
		</td>
	</tr>
	<tr id="tr_estado">
		<td class = "subtitulodireita"> 
			 Estado:
		</td>
		<td>
		<?
			 $estuf = $obSolicitante->estuf;
			 $sql = "select
					 e.estuf as codigo, e.estdescricao as descricao 
					from
					 territorios.estado e 
					order by
					 e.estdescricao asc";
			 $db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '', '', '', 'N', 'estuf',false,null,'Estado');
			 ?>						
		</td>
	</tr>
	<tr id="tr_municipio" >
	<td class = "subtitulodireita">
			Município:
			<br/>
		</td>
		<td  id="municipio">
			<?
			$sql = "select
					  mundescricao   
					from
					 territorios.municipio  
					order by
					 mundescricao asc";
			//die($db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '', 'N', 'muncod',false,null,'Município'));
			$rsa = $db->carregar( $sql );
			echo "<select name=\"mundescricao\" id=\"mundescricao\" class=\"CampoEstilo\">";
			echo "<option value=\"\">Selecione...</option>";
			foreach( $rsa as $ra){
				echo "<option value=\"".$ra['mundescricao']."\">".$ra['mundescricao']."</option>";
			}
			echo "</select>";
			?>	
		</td>	
	</tr>
	
	<tr>
		<td class="SubTituloDireita" colspan="2" style="text-align:center">
		 	 <input type="button" name="btnPesquisar" value="Pesquisar" id="btnPesquisar" onclick="buscar();">  
		 	 <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvar('<?=$_REQUEST['type']?>');">  
			 <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
		</td>
	</tr>
	</form>
	<? 
	if( $_POST['iescodigo'] != '' ){ 
		$AND .= " AND iescodigo = ".$_POST['iescodigo'];
	}
	if( $_POST['iesnome'] != '' ) {
		$AND .= " AND iesnome ILIKE '%".$_POST['iesnome']."%' "; 
	}
	if( $_POST['iessigla'] != '' ) {
		$AND .= " AND iessigla ILIKE '%".$_POST['iessigla']."%' "; 
	}
	
	if( $_POST['mundescricao'] != '' && $_POST['estuf'] != '' ) {
		$AND .= " AND iesmunicipio ILIKE '%".$_POST['mundescricao']."%'"; 
	}elseif( $_POST['estuf'] != '') {
		$AND .= " AND iesuf = '".$_POST['estuf']."' "; 
	}
	$sql = "SELECT  
				'<input type=\"".$_REQUEST['type']."\" name=\"iesid\" id=\"iesid_'||iesid||'\" value=\"'||iesid||'\"  > &nbsp;'|| iescodigo as acao, 
				iessigla as sigla ,
				iesnome  as nome,
				iesuf as uf,
				iesmunicipio as muncod
 	 		FROM ies.ies 
 	 		WHERE iescodigo IS NOT NULL 
 	 		$AND 
 	 		ORDER BY iesnome 
	"; 
 	 		
	$cabecalho = array( "Código &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;","Sigla", "Nome", "UF", "Cidade"  );  
	$db->monta_lista( $sql, $cabecalho, 3000, 10, 'S', '', '', 'form2');
	if( $_REQUEST['type'] == 'checkbox' ){
		marcarInstituicaoCadastrada();
	}elseif( trim( $_REQUEST['type']) == 'radio'){ 
		marcarInstituicaoEntidade(); 
	}	
	?>    
</table>
<script type="text/javascript">
function fecharPopup(){
	return window.close();
}
function buscar(){
	document.formulario.acaoForm.value = 'busca'; 
	document.formulario.submit();
}
function cancelar(){
 
 	document.formulario.acaoForm.value = 'cancelar'; 
	document.formulario.submit();
}
function salvar( tipo ){
 
	if( tipo == 'radio' ){
		var iesid = document.getElementsByName('iesid');
 		  
		for ( var i=0; i<iesid.length; i++  ){ 
			if( iesid[i].checked ){
				 var resp = iesid[i].value;
				 continue;
			} 
		} 
		var req = new Ajax.Request('tarefa.php?modulo=principal/popupInstituicoes&acao=A', {
								        method:     'post',
								        parameters: '&iescodRadio=' + resp,
								        onComplete: function (res)
								        {
								        	opener.document.getElementById("div_instituicao").innerHTML = res.responseText;		  
								        	opener.document.getElementById("entid").value=0;
								        	opener.document.getElementById("tr_estado").style.display = 'none';
								        	opener.document.getElementById("tr_municipio").style.display = 'none';
											window.close();
								        }
								  });			
	}else if( tipo == 'checkbox' ){
		var resp = ''; 
		var iesid = document.getElementsByName('iesid');
		for ( var i=0; i<iesid.length; i++  ){ 
			if( iesid[i].checked ){
				  resp += iesid[i].value+',';
				 continue;
			} 
		} 
 
		var req = new Ajax.Request('tarefa.php?modulo=principal/popupInstituicoes&acao=A', {
								        method:     'post',
								        parameters: '&iescodResp=' + resp,
								        onComplete: function (res)
								        { 
								        	opener.document.getElementById("div_tabela_instituicao").innerHTML = res.responseText;		  
											window.close();
								        }
								  });
	}
}
function filtraTipo(estuf) {
	if( !estuf ){
		return false;
	}
	td 	   = document.getElementById('municipio');
	select = document.getElementsByName('mundescricao')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	var req = new Ajax.Request('tarefa.php?modulo=principal/popupInstituicoes&acao=A', {
							        method:     'post',
							        parameters: '&ajaxestuf=' + estuf,
							        onComplete: function (res)
							        {			  
										td.innerHTML = res.responseText;
										td.style.visibility = 'visible';
							        }
							  });
    
}
</script>
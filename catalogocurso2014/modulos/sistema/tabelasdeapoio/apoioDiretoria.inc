<?php 
function listarDiretoria(){
    global $db;

    $aryWhere[] = "corstatus = 'A'";
    
    $acao = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| cr.corid ||'\" onclick=\"carregarDiretoria('|| cr.corid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
	 		  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| cr.corid ||'\" onclick=\"excluirDiretoria('|| cr.corid ||', \'' || cr.corsigla || '\');\" style=\"cursor:pointer;\"/>' AS acao,";   
	
	$sql = "SELECT 		$acao 
						c.coordsigla,
						cr.corsigla, 
						cr.cordesc, 						
						cr.corano
  			FROM 		catalogocurso2014.coordresponsavel cr 
  			INNER JOIN	catalogocurso2014.coordenacao c	ON c.coordid = cr.coordid	
  			" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
	
    $cabecalho = array('Ação','Coordenação','Sigla','Descrição', 'Ano');
    $tamanho = array('10%', '30%','15%', '30%', '15%');
    $alinhamento = array('center', 'left','center', 'left', 'center', 'center');	
	$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);
}

function salvarDiretoria($post){
	global $db;
	
	extract($post);
	
	$sql = "INSERT INTO catalogocurso2014.coordresponsavel(coordid, corsigla, cordesc, corstatus, corano)
   			VALUES ({$coordid},'{$corsigla}','".pg_escape_string($cordesc)."','A', {$corano})";
	
	$db->executar($sql);
	
	if($db->commit()){
		$al = array('alert'=>'Diretoria cadastrada com sucesso!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A');
	} else {
		$al = array('alert'=>'Não foi possível realizar o cadastro!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A');
	}
	alertlocation($al);		
}

function alterarDiretoria($post){
    global $db;	
	
    extract($post);
    
    $sql = "UPDATE 		catalogocurso2014.coordresponsavel
   			SET 		coordid = {$coordid},
   						corsigla = '{$corsigla}', 
   						cordesc = '".pg_escape_string($cordesc)."',
   						corano = {$corano}
 			WHERE 		corid = {$corid}";	
    
	$db->executar($sql);
	
	if($db->commit()){
		$al = array('alert'=>'Diretoria alterada com sucesso!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A');
	} else {
		$al = array('alert'=>'Não foi possível realizar a alteração!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A');
	}
	alertlocation($al);		    
}

function excluirDiretoria($post){
    global $db;	
    	
	extract($post);
	
	if($corid){
	    $sql = "UPDATE 		catalogocurso2014.coordresponsavel
	   			SET 		corstatus = 'I'
	 			WHERE 		corid = {$corid}";
	    $db->executar($sql);
	}    
	$db->commit();
}

function carregarDiretoria($post){
    global $db;		
    
	extract($post);
        
	if($corid){
		$aryWhere[] = "corid = {$corid}";
	}
	
	$sql = "SELECT 		corid,
						coordid, 
						corsigla, 
						cordesc, 
						corano
  			FROM 		catalogocurso2014.coordresponsavel
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
	
	$diretoria = $db->pegaLinha($sql);
	$diretoria['cordesc'] = iconv("ISO-8859-1", "UTF-8", $diretoria['cordesc']);
	$diretoria['corsigla'] = iconv("ISO-8859-1", "UTF-8", $diretoria['corsigla']);
	
    echo simec_json_encode($diretoria);
}

if($_REQUEST['requisicao']=='salvarDiretoria'){
	salvarDiretoria($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarDiretoria'){
	alterarDiretoria($_POST);
	exit();
}

if($_REQUEST['requisicao']=='carregarDiretoria'){
	carregarDiretoria($_POST);
	exit();
}

if($_REQUEST['requisicao']=='excluirDiretoria'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirDiretoria($_POST);
	listarDiretoria();
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc'; 

monta_titulo("Diretoria",""); ?>

<script language="javascript" type="text/javascript">
	function salvar(){
		if(jQuery('#coordid').val() == ''){
			alert('O campo "Coordenação" é obrigatório!');
			jQuery('#coordid').focus();
			return false;
		}		
		if(jQuery('#cordesc').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			jQuery('#cordesc').focus();
			return false;
		}
		if(jQuery('#corsigla').val() == ''){
			alert('O campo "Sigla" é obrigatório!');
			jQuery('#corsigla').focus();
			return false;
		}		
		if(jQuery('#corano').val() == ''){
			alert('O campo "Ano" é obrigatório!');
			jQuery('#corano').focus();
			return false;
		}					
		jQuery('#formulario').submit();
	}

	function excluirDiretoria(corid,corsigla){
		if(confirm("Deseja realmente excluir a Diretoria '"+corsigla+"'?")){
			divCarregando();
			jQuery.ajax({
				type: 'POST',
				url: 'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A',
				data: { requisicao: 'excluirDiretoria', corid: corid},
				async: false,
				success: function(data) {
					alert('Diretoria excluída com sucesso!');
					jQuery("#divListarDiretoria").html(data);
					divCarregado();
			    }
			});
		}
	}

	function carregarDiretoria(corid){
		jQuery.ajax({
			type: 'POST',
			url: 'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioDiretoria&acao=A',
			dataType: 'json',
			data: { requisicao: 'carregarDiretoria', corid: corid},
			async: false,
			success: function(data){
				if(data){
					jQuery('#corid').val(data.corid);
					jQuery('#coordid').val(data.coordid);
					jQuery('#cordesc').val(trim(data.cordesc));
					jQuery('#corsigla').val(trim(data.corsigla));
					jQuery('#corano').val(data.corano);
					jQuery('#requisicao').val('alterarDiretoria');
				} else {
					jQuery('#corid').val('');
					jQuery('#coordid').val('');
					jQuery('#cordesc').val('');
					jQuery('#corsigla').val('');
					jQuery('#corano').val(data.corano);
					jQuery('#requisicao').val('');
					alert("Diretoria não encontrada!");
				}	
			}
		});
	}
</script>

<form id="formulario" method="post" name="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value="salvarDiretoria"/>
	<input type="hidden" id="corid" name="corid" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="25%">Coordenação:</td>
			<td>
				<?php  $sql = "SELECT		coordid AS codigo,
											coordsigla || '( ' || coorddesc || ' )' AS descricao
							   FROM			catalogocurso2014.coordenacao
							   ORDER BY		coorddesc ASC";
					  
				$db->monta_combo('coordid', $sql, 'S', "Selecione...", '', '', '', '470', 'S', 'coordid'); ?>	
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo campo_texto('cordesc', 'S', 'S', '', '71', '250', '', 'N', 'left', '', '', 'id="cordesc"', '', NULL, '' ); ?>
		</tr>	
		<tr>
			<td class="subtituloDireita">Sigla:</td>
			<td><?php echo campo_texto('corsigla', 'S', 'S', '', '30', '50', '', 'N', 'left', '', '', 'id="corsigla"', '', NULL, '' ); ?>
		</tr>				
		<tr>
			<td class="subtituloDireita">Ano:</td>
			<td><?php echo campo_texto('corano', 'S', 'S', '', '10', '250', '####', 'N', 'left', '', '', 'id="corano"', '', NULL, '' ); ?>
		</tr>			
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvar();"/>
				<input type="button" value="Novo" id="btnSalvar" onclick="location.reload();"/>
			</td>
		</tr>				
	</table>	
</form>

<div id="divListarDiretoria"><?php listarDiretoria(); ?></div>
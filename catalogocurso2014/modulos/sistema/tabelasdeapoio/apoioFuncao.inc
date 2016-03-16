<?php 
function listarFuncao(){
    global $db;

    $aryWhere[] = "fuestatus = 'A'";
    
    $acao = "'<img border=\"0\" src=\"../imagens/alterar.gif\" id=\"'|| fueid ||'\" onclick=\"carregarFuncao('|| fueid ||');\" style=\"cursor:pointer;\"/>&nbsp;&nbsp;
	 		  <img border=\"0\" src=\"../imagens/excluir.gif\" id=\"'|| fueid ||'\" onclick=\"excluirFuncao('|| fueid ||');\" style=\"cursor:pointer;\"/>' AS acao,";   

	$sql = "SELECT 		$acao 
						fuedesc, 
						fueatribuicao, 
						CASE WHEN fuesecretaria = '1' THEN 'SEB'
							 WHEN fuesecretaria = '2' THEN 'SECADI'
							 ELSE '' END AS fuesecretaria,
						array_to_string(array(
						SELECT 		esc.descricao
						FROM 		catalogocurso2014.funcaoequipeescolaridade func
						INNER JOIN 	((SELECT		to_char(pk_cod_escolaridade,'9') AS codigo, 
									 				no_escolaridade AS descricao
								  	 FROM 			educacenso_2013.tab_escolaridade e)
								  	 UNION ALL
								  	(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo, 
													no_pos_graduacao AS descricao				
									 FROM 			educacenso_2013.tab_pos_graduacao e)) AS esc ON esc.codigo::integer = func.cod_escolaridade
						WHERE		func.fueid = f.fueid
						ORDER BY 	esc.descricao
						), ',') AS escolaridade,							
						fueexperiencia,
						fueano
  			FROM 		catalogocurso2014.funcaoequipe f
  						" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
	
    $cabecalho = array('Ação','Descrição', 'Secretaria', 'Atribuição', 'Escolaridade','Experiência Miníma','Ano');
    $tamanho = array('10%', '15%', '25%', '25%', '10%','10%','5%');
    $alinhamento = array('center', 'left', 'left', 'left', 'left','left','center');	
	$db->monta_lista($sql, $cabecalho, '50', '10', 'N', 'center', 'N', '', $tamanho, $alinhamento);
}

function salvarFuncao($post){
	global $db;
	
	extract($post);
	
	$sql = "INSERT INTO catalogocurso2014.funcaoequipe(fueatribuicao, fuedesc, fuestatus, fueexperiencia, fueano, fuesecretaria)
   			VALUES ('".pg_escape_string($fueatribuicao)."', '{$fuedesc}','A', '{$fueexperiencia}', {$fueano},'{$fuesecretaria}') RETURNING fueid";
	
	$fueid = $db->pegaUm($sql);
	
	if($fueid){
	    $sql = "DELETE FROM catalogocurso2014.funcaoequipeescolaridade WHERE fueid = {$fueid};";
	    if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
	        foreach($cod_escolaridade as $id){
	            $sql .= "INSERT INTO catalogocurso2014.funcaoequipeescolaridade(fueid, cod_escolaridade, feestatus, feeano) VALUES ({$fueid}, {$id}, 'A', {$_SESSION['exercicio']}) ;";
	        }
	    }	
		$db->executar($sql);
	}
	
	if($db->commit()){
		$al = array('alert'=>'Função cadastrada com sucesso!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A');
	} else {
		$al = array('alert'=>'Não foi possível realizar o cadastro!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A');
	}
	alertlocation($al);		
}

function alterarFuncao($post){
    global $db;	
	
    extract($post);
    
    $sql = "UPDATE 		catalogocurso2014.funcaoequipe
   			SET 		fueatribuicao = '".pg_escape_string($fueatribuicao)."', 
   						fuedesc = '{$fuedesc}',
   						fueexperiencia = '{$fueexperiencia}',
   						fueano = {$fueano},
   						fuesecretaria = '{$fuesecretaria}'
 			WHERE 		fueid = {$fueid} RETURNING fueid";	
    
	$fueid = $db->pegaUm($sql);
	
	if($fueid){
	    $sql = "DELETE FROM catalogocurso2014.funcaoequipeescolaridade WHERE fueid = {$fueid};";
	    if(is_array($cod_escolaridade) && count($cod_escolaridade) > 0 ){
	        foreach($cod_escolaridade as $id){
	            $sql .= "INSERT INTO catalogocurso2014.funcaoequipeescolaridade(fueid, cod_escolaridade, feestatus, feeano) VALUES ({$fueid}, {$id}, 'A', {$_SESSION['exercicio']}) ;";
	        }
	    }	
		$db->executar($sql);
	}
	
	if($db->commit()){
		$al = array('alert'=>'Função alterada com sucesso!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A');
	} else {
		$al = array('alert'=>'Não foi possível realizar a alteração!','location'=>'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A');
	}
	alertlocation($al);		    
}


function excluirFuncao($post){
    global $db;	
    	
	extract($post);
	
	if($fueid){
	    $sql = "UPDATE 		catalogocurso2014.funcaoequipe
	   			SET 		fuestatus = 'I'
	 			WHERE 		fueid = {$fueid}";
	    $db->executar($sql);
	    
	    $sql = "DELETE FROM catalogocurso2014.funcaoequipeescolaridade WHERE fueid = {$fueid};";
	    $db->executar($sql);
	}    
	$db->commit();
}


function carregarFuncao($post){
    global $db;		
    
	extract($post);
        
	if($fueid){
		$aryWhere[] = "fueid = {$fueid}";
	}
	
	$sql = "SELECT 		fueid, 
						fueatribuicao,
						fuedesc, 
						fueano,
						fueexperiencia,
						fuesecretaria
  			FROM 		catalogocurso2014.funcaoequipe
  			" . (is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '') . "";
	
	$funcao = $db->pegaLinha($sql);
	
    $sql = "SELECT 		codigo
			FROM 		catalogocurso2014.funcaoequipeescolaridade func
			INNER JOIN 	((SELECT		to_char(pk_cod_escolaridade,'9') AS codigo, 
						 				no_escolaridade AS descricao
					  	 FROM 			educacenso_2013.tab_escolaridade e)
						 UNION ALL
						(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo, 
										no_pos_graduacao AS descricao				
						 FROM 			educacenso_2013.tab_pos_graduacao e)) AS esc ON esc.codigo::integer = func.cod_escolaridade
		   WHERE		 func.fueid = {$fueid}";
        
    $funcao['cod_escolaridade'] = $db->carregar($sql);
	return $funcao;
}

if($_REQUEST['requisicao']=='salvarFuncao'){
	salvarFuncao($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarFuncao'){
	alterarFuncao($_POST);
	exit();
}

if($_REQUEST['requisicao']=='excluirFuncao'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirFuncao($_POST);
	listarFuncao();
	exit();
}

if($_REQUEST['fueid']){
	$funcao = carregarFuncao($_REQUEST);
	if($funcao){
		extract($funcao);
	}
}

include APPRAIZ . 'includes/cabecalho.inc'; 

monta_titulo("Função",""); ?>

<script language="javascript" type="text/javascript">
	function salvar(){
		if(jQuery('#fuedesc').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			jQuery('#fuedesc').focus();
			return false;
		}
		if(jQuery('#fuesecretaria').val() == ''){
			alert('O campo "Secretaria" é obrigatório!');
			jQuery('#fuesecretaria').focus();
			return false;
		}	
		if(jQuery('#fueatribuicao').val() == ''){
			alert('O campo "Atribuição SEB" é obrigatório!');
			jQuery('#fueatribuicao').focus();
			return false;
		}	
		if(jQuery('[name=cod_escolaridade[]]:checked').length <= 0){
			alert('O campo "Escolaridade" é obrigatório!');
			jQuery('[name=cod_escolaridade[]]').focus();
			return false;
		}		
		if(jQuery('#fueexperiencia').val() == ''){
			alert('O campo "Experiência Mínima" é obrigatório!');
			jQuery('#fueexperiencia').focus();
			return false;
		}						
		if(jQuery('#fueano').val() == ''){
			alert('O campo "Ano" é obrigatório!');
			jQuery('#fueano').focus();
			return false;
		}					
		jQuery('#formulario').submit();
	}

	function excluirFuncao(fueid){
		if(confirm("Deseja realmente excluir a Função?")){
			divCarregando();
			jQuery.ajax({
				type: 'POST',
				url: 'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A',
				data: { requisicao: 'excluirFuncao', fueid: fueid},
				async: false,
				success: function(data) {
					alert('Função excluída com sucesso!');
					jQuery("#divListarFuncao").html(data);
					divCarregado();
			    }
			});
		}
	}

	function carregarFuncao(fueid){
		window.location.href = 'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A&fueid='+fueid;
	}
	
	function reload(){
		window.location.href = 'catalogocurso2014.php?modulo=sistema/tabelasdeapoio/apoioFuncao&acao=A';
	}
</script>

<form id="formulario" method="post" name="formulario">
	<?php if($fueid){ ?>
	<input type="hidden" id="requisicao" name="requisicao" value="alterarFuncao"/>
	<input type="hidden" id="fueid" name="fueid" value="<?php echo $fueid; ?>"/>
	<?php } else { ?>
	<input type="hidden" id="requisicao" name="requisicao" value="salvarFuncao"/>
	<?php } ?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo campo_texto('fuedesc', 'S', 'S', '', '71', '250', '', 'N', 'left', '', '', 'id="fuedesc"', '', NULL, '' ); ?>
		</tr>	
		<tr>
			<td class="subtituloDireita">Secretaria:</td>
			<td>
	        	<?php 	
	        		$arySecretaria = array(
							array('codigo' => '1', 'descricao' => 'SEB'),
							array('codigo' => '2', 'descricao' => 'SECADI')); ?>
	        	<?php $db->monta_combo('fuesecretaria',$arySecretaria,'S','Selecione a Secretaria','','','','550','S','fuesecretaria',''); ?>			
			</td>

		</tr>				
		<tr>
			<td class="subtituloDireita">Atribuição:</td>
			<td><?php echo campo_textarea('fueatribuicao', 'S', 'S', 'Atribuição', '80', '5', ''); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Escolaridade:</td>
			<td>
			<?php 
			$sql = "(SELECT			to_char(pk_cod_escolaridade,'9') AS codigo, 
									pk_cod_escolaridade||' - '||no_escolaridade AS descricao
				  	 FROM 			educacenso_2013.tab_escolaridade e)
				  	 UNION ALL
				  	(SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo, 
									pk_pos_graduacao||'0 - '||no_pos_graduacao AS descricao				
					 FROM 			educacenso_2013.tab_pos_graduacao e)";

			$niveis = $db->carregar($sql);
            $nv = Array();
            if(is_array($niveis)){
            	foreach($niveis as $nivel){
                	if(!in_array($nivel['nivel'],$nv)){
                    	array_push($nv, $nivel['nivel']);
                    }
                    $checked = '';
                    if( is_array($cod_escolaridade) ){
                    	foreach($cod_escolaridade as $resp){
                        	if( $resp['codigo'] == $nivel['codigo'] ){
                            	$checked = 'checked="checked"';
                            }
                        }
                    }
                    echo "<br>
					<input type=\"checkbox\" id=\"".$nivel['nivel']."\" class=\"".implode(" ",$nv)."\" value=\"".$nivel['codigo']."\" $checked name=\"cod_escolaridade[]\"> ".$nivel['descricao'];
               }
          }	?>
			</td>
		</tr>						
		<tr>
			<td class="subtituloDireita">Experiência Mínima:</td>
			<td><?php echo campo_textarea('fueexperiencia', 'S', 'S', 'Experiência Mínima', '80', '3', '250'); ?></td>
		</tr>			
		<tr>
			<td class="subtituloDireita">Ano:</td>
			<td><?php echo campo_texto('fueano', 'S', 'S', '', '10', '250', '####', 'N', 'left', '', '', 'id="fueano"', '', NULL, '' ); ?>
		</tr>			
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvar();"/>
				<input type="button" value="Novo" id="btnSalvar" onclick="reload();"/>
			</td>
		</tr>				
	</table>	
</form>

<div id="divListarFuncao"><?php listarFuncao(); ?></div>
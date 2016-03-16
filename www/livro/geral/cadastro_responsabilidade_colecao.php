<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$colecoesresp   = $_REQUEST["acao"];

if ($_REQUEST["colecoesresp"]){
	$colecoesresp = $_REQUEST["colecoesresp"];
	atribuiColecoes($usucpf, $pflcod, $colecoesresp);
}

/*
 * Função que lista as coleções
 */
function listaColecoes(){
	
	global $db;
	
	$sql = "SELECT 
				col.colid,
				col.coltitulo 
			FROM
				livro.colecao col
			where
				col.colstatus = 'A'
			ORDER BY 
				col.coltitulo";
	
	$colecaosExistentes = $db->carregar($sql);
	
	$count = count($colecaosExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		
		$codigo    = $colecaosExistentes[$i]["colid"];
		$descricao = $colecaosExistentes[$i]["coltitulo"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		//onclick=\"retorna('".$i."');\"
		echo "<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"checkbox\" name=\"colid\" id=\"".$codigo."\" value=\"".$codigo."\" class=\"valorOpcao\">
					<input type=\"hidden\" name=\"coltitulo\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
				<td>".$descricao."</td>
			</tr>";
	}			
}

function atribuiColecoes($usucpf, $pflcod, $colecoesresp){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE 
				livro.usuarioresponsabilidade 
			SET 
				rpustatus = 'I' 
			WHERE 
				usucpf = '". $usucpf ."' 
			AND 
				pflcod = '". $pflcod ."'";
	
	$db->executar($sql);
	
	if ($colecoesresp[0]){
		
		foreach($colecoesresp as $colid) {
			
			$sql = "SELECT 
						* 
					FROM 
						livro.usuarioresponsabilidade 
					WHERE 
						usucpf = '". $usucpf ."' 
					AND 
						pflcod = '". $pflcod ."' 
					AND 
						colid = '". $colid ."'";
			
			$dadosur = $db->carregar($sql);
			
			if($dadosur) {
				
				$sql = "UPDATE 
							livro.usuarioresponsabilidade
   						SET 
   							rpustatus = 'A', rpudata_inc= NOW()
 						WHERE 
 							usucpf = '". $usucpf ."' 
 						AND 
 							pflcod = '". $pflcod ."' 
 						AND 
 							colid = '". $colid ."'";
				
				// Se existir registro atualizar para ativo
				$db->carregar($sql);
				
			} else {
				
				$sql = "INSERT INTO livro.usuarioresponsabilidade
							(pflcod, usucpf, colid, rpustatus, rpudata_inc)
    					VALUES 
    						('". $pflcod ."', '". $usucpf ."', '". $colid ."', 'A', NOW());";
				
				// Se não existir, inserir novo
				$db->executar($sql);
			}
		}
	}
	$db->commit();
	
	echo '<script>
			alert(\'Operação realizada com sucesso!\');
			window.parent.opener.location.reload();
			self.close();
		  </script>';
	
}

function buscaColecoesAtribuidas($usucpf, $pflcod){
	
	global $db;
	
	$sql = "SELECT DISTINCT 
				col.colid AS codigo, 
				col.coltitulo AS descricao
			FROM 
				livro.usuarioresponsabilidade ur 
			INNER JOIN 
				livro.colecao col ON ur.colid = col.colid 
			WHERE 
				ur.rpustatus = 'A' 
			AND 
				ur.usucpf = '$usucpf' 
			AND 
				ur.pflcod = $pflcod 
			AND 
				col.colstatus = 'A'";
	
	$rs = $db->carregar($sql);

	if($rs) {
		foreach($rs as $dados){
			echo " <option value=\"{$dados['codigo']}\">{$dados['codigo']} - {$dados['descricao']}</option>";
		}		
	} else {
		echo '<option value="">Clique na coleção para selecionar.</option>';
	}
}

?>

<?flush();?>
<html>
	<head>
		<meta http-equiv="Pragma" content="no-cache">
		<title>Ações</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript">
		
			$(function(){
				if($('#colresp option').length > 0){
					$.each($('#colresp option'), function(i,v){
						if(v.value){						
							$('#'+v.value).attr('checked',true);
						}
					});
				}

				$('.valorOpcao').click(function(){
										
					var texto = $(this).parent().next().next().html();
					
					if(this.checked){
						
						if($("#colresp option[value='']"))	
							$("#colresp option[value='']").remove();
						
						$('#colresp').append(new Option(texto, this.value, true, true));
						
					}else{
						
						$("#colresp option[value='"+this.value+"']").remove();
					}

					if($('#colresp option').length == 0)
						$('#colresp').append(new Option('Clique na coleção para selecionar.', '', true, true));
					
					sortSelect($('#colresp'));
				});

				$('#bt_ok').click(function(){
					selectAllOptions(document.getElementById('colresp'));
					$('#formassocia').submit();					
				});
			});	

			function abreconteudo(objeto)
			{
				if (document.getElementById('img'+objeto).name=='+'){
					document.getElementById('img'+objeto).name='-';
				    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
					document.getElementById(objeto).style.visibility = "visible";
					document.getElementById(objeto).style.display  = "";
				}else{
					document.getElementById('img'+objeto).name='+';
				    document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
					document.getElementById(objeto).style.visibility = "hidden";
					document.getElementById(objeto).style.display  = "none";
				}
			}	
			
		</script>	
	</head>
	<body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
	
		<!-- Lista de Estados -->
		<div style="overflow:auto; width:496px; height:350px; border:2px solid #ececec; background-color: #ffffff;">
			<form name="formulario">
				<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
					<thead>
						<tr>
							<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione o tipo de ensino</strong></td>		
						</tr>
					</thead>
					<?php listaColecoes(); ?>
				</table>
			</form>
		</div>
		
		<!-- Submit do Formulário -->
		<table width="496" align="left" border="0" cellspacing="0" cellpadding="2">
			<tr bgcolor="#c0c0c0">
				<td align="right" style="padding:3px;" colspan="3">
					<input type="Button" name="ok" value="OK" id="bt_ok">
				</td>
			</tr>
			<tr>
				<td>
					<!-- Estados Selecionadas -->
					<form name="formassocia" id="formassocia" action="cadastro_responsabilidade_colecao.php" method="post">
						<input type="hidden" name="usucpf" value="<?=$usucpf?>">
						<input type="hidden" name="pflcod" value="<?=$pflcod?>">
						<select multiple size="8" name="colecoesresp[]" id="colresp" style="width:500px;" class="CampoEstilo">
							<?php buscaColecoesAtribuidas($usucpf, $pflcod); ?>				
						</select>
					</form>
				</td>				
			</tr>
		</table>
		<br/>
	</body>
</html>
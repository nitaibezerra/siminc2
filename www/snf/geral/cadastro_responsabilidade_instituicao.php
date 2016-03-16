<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";

$db     = new cls_banco();
$usucpf = $_REQUEST['usucpf'];
$pflcod = $_REQUEST['pflcod'];
$instituicoesresp   = $_REQUEST["acao"];

if ($_REQUEST["instituicoesresp"]){
	$instituicoesresp = $_REQUEST["instituicoesresp"];
	atribuiinstituicoes($usucpf, $pflcod, $instituicoesresp);
}

/*
 * Função que lista as instituicoes
 */
function listaInstituicoes(){
	
	global $db;
	
	$sql = "SELECT 
				ins.insid,
				CASE 
       			WHEN inssigla is null then insnome
       			ELSE insnome || ' - ' || inssigla 
   				END as insnome 
			FROM
				snf.instituicaoensino ins
			where
				ins.insstatus = 'A'
			ORDER BY 
				ins.insnome";
	
	$instituicoesExistentes = $db->carregar($sql);
	
	$count = count($instituicoesExistentes);

	// Monta as TR e TD com as unidades
	for ($i = 0; $i < $count; $i++){
		
		$codigo    = $instituicoesExistentes[$i]["insid"];
		$descricao = $instituicoesExistentes[$i]["insnome"];
		
		if (fmod($i,2) == 0){ 
			$cor = '#f4f4f4';
		} else {
			$cor='#e0e0e0';
		}
		//onclick=\"retorna('".$i."');\"
		echo "<tr bgcolor=\"".$cor."\">
				<td align=\"right\" width=\"10%\">
					<input type=\"checkbox\" name=\"insid\" id=\"".$codigo."\" value=\"".$codigo."\" class=\"valorOpcao\">
					<input type=\"hidden\" name=\"insnome\" value=\"".$codigo." - ".$descricao."\">
				</td>
				<td align=\"right\" style=\"color:blue;\" width=\"10%\">".$codigo."</td>
				<td>".$descricao."</td>
			</tr>";
	}			
}

function atribuiinstituicoes($usucpf, $pflcod, $instituicoesresp){
	
	global $db;
	
	$data = date("Y-m-d H:i:s");
	
	$sql = "UPDATE 
				snf.usuarioresponsabilidade 
			SET 
				rpustatus = 'I' 
			WHERE 
				usucpf = '". $usucpf ."' 
			AND 
				pflcod = '". $pflcod ."'";
	
	$db->executar($sql);
	
	if ($instituicoesresp[0]){
		
		foreach($instituicoesresp as $insid) {
			
			$sql = "SELECT 
						* 
					FROM 
						snf.usuarioresponsabilidade 
					WHERE 
						usucpf = '". $usucpf ."' 
					AND 
						pflcod = '". $pflcod ."' 
					AND 
						insid = '". $insid ."'";
			
			$dadosur = $db->carregar($sql);
			
			if($dadosur) {
				
				$sql = "UPDATE 
							snf.usuarioresponsabilidade
   						SET 
   							rpustatus = 'A', rpudata_inc= NOW()
 						WHERE 
 							usucpf = '". $usucpf ."' 
 						AND 
 							pflcod = '". $pflcod ."' 
 						AND 
 							insid = '". $insid ."'";
				
				// Se existir registro atualizar para ativo
				$db->carregar($sql);
				
			} else {
				
				$sql = "INSERT INTO snf.usuarioresponsabilidade
							(pflcod, usucpf, insid, rpustatus, rpudata_inc)
    					VALUES 
    						('". $pflcod ."', '". $usucpf ."', '". $insid ."', 'A', NOW());";
				
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

function buscaInstituicoesAtribuidas($usucpf, $pflcod){
	
	global $db;
	
	$sql = "SELECT DISTINCT 
				ins.insid AS codigo, 
				ins.insnome AS descricao
			FROM 
				snf.usuarioresponsabilidade ur 
			INNER JOIN 
				snf.instituicaoensino ins ON ur.insid = ins.insid 
			WHERE 
				ur.rpustatus = 'A' 
			AND 
				ur.usucpf = '$usucpf' 
			AND 
				ur.pflcod = $pflcod 
			AND 
				ins.insstatus = 'A'";
	
	$rs = $db->carregar($sql);

	if($rs) {
		foreach($rs as $dados){
			echo " <option value=\"{$dados['codigo']}\">{$dados['codigo']} - {$dados['descricao']}</option>";
		}		
	} else {
		echo '<option value="">Clique na instituição para selecionar.</option>';
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
				
				if($('#insresp option').length > 0){
					$.each($('#insresp option'), function(i,v){
						if(v.value){						
							$('#'+v.value).attr('checked',true);
						}
					});
				}

				$('.valorOpcao').click(function(){

					if($('[name=insid]:checked').length > 1){
						alert('O usuário não pode ter mais que uma instituição vinculada!');
						return false;
					}
										
					var texto = $(this).parent().next().next().html();
					
					if(this.checked){
						
						if($("#insresp option[value='']"))	
							$("#insresp option[value='']").remove();
						
						$('#insresp').append(new Option(texto, this.value, true, true));
						
					}else{
						
						$("#insresp option[value='"+this.value+"']").remove();
					}

					if($('#insresp option').length == 0)
						$('#insresp').append(new Option('Clique na instituição para selecionar.', '', true, true));
					
					sortSelect($('#insresp'));
				});

				$('#bt_ok').click(function(){
					selectAllOptions(document.getElementById('insresp'));
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
					<?php listaInstituicoes(); ?>
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
					<form name="formassocia" id="formassocia" action="cadastro_responsabilidade_instituicao.php" method="post">
						<input type="hidden" name="usucpf" value="<?=$usucpf?>">
						<input type="hidden" name="pflcod" value="<?=$pflcod?>">
						<select multiple size="8" name="instituicoesresp[]" id="insresp" style="width:500px;" class="CampoEstilo">
							<?php buscaInstituicoesAtribuidas($usucpf, $pflcod); ?>				
						</select>
					</form>
				</td>				
			</tr>
		</table>
		<br/>
	</body>
</html>
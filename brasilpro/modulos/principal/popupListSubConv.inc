<?php
# AÇÕES
/* POST
 * $_POST['manager'] = 1 | voltar para análise
 * $_POST['manager'] = 2 | gerar convênio 
 */
if ($_POST['manager'] == 1){
	/*
	*  Deletar subacaoconvenio
	*/
	if ($_POST['delSbaid']){
		$itens = explode(';', $_POST['delSbaid']);
		array_pop($itens);
		$itens = (string) implode(',', $itens);
		$sql = "DELETE FROM cte.subacaoconvenio WHERE sbaid IN ($itens);";
		$db->executar($sql);
		$db->commit();
		die ('<script type="text/javascript">
				alert(\'Operação realizada com sucesso!\');
				window.location.replace(window.location);
			  </script>');
	}
}

?>
<html>
  <head>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
	<script type="text/javascript">
	/*
	* Function, check/ñ check todos os itens da lista
	*/
	function allSelect(obj) {
		element = document.getElementsByName('sbaid[]');	
		param = obj.checked ? obj.checked : obj.checked;
		document.getElementById('delSbaid').value = '';
		
		for (i = 0; i < element.length; i++) {
			element[i].checked = param;
			capturaCheckbox(element[i]);
		} 
	}
	/*
	* Function, Tira o checked do botão selecionar todos
	*/
	function notCheckButton (obj){
		if (!obj.checked)
			document.getElementById('selectAll').checked = false;
	}
	/*
	* Function, que confirma/define a operação a ser executada no submit
	*/
	function confirmar (param,mens) {
		if (confirm(mens) && param != '') {
		    document.getElementById('manager').value = param;
			formulario.submit();
			return true;
		}
		return false;
	}
	/*
	* Function, Captura os checkbox desmarcados
	*/
	function capturaCheckbox (obj){
		delSbaidElement = document.getElementById('delSbaid');
		if (!obj.checked)
			delSbaidElement.value += obj.value+';' 
		else
			delSbaidElement.value = delSbaidElement.value.replace(obj.value+';','');
		return;
	}	
	/*
	* Function, Abrir tela de parecer
	*/
	function openParecer (mens, parecer, param){
		if (confirm(mens)) {
		    elementParecer = document.getElementsByName('sbaid[]');
		    param = param ? param : '';
		    parSubacao = '';
		    if (!parecer){
			    for (i=0; i < elementParecer.length; i++) {
			    	parSubacao += elementParecer[i].checked ? elementParecer[i].value+';' : '';
			    }
			    parecer = '';
		    }
			location.href='?modulo=principal/parecerGrupoSubacao&acao=A&parecer='+parecer+'&sbaid='+parSubacao+'&param='+param;
		}
	
//		winParecer = window.open('http://<?=$_SERVER['SERVER_NAME']?>/brasilpro/brasilpro.php?modulo=principal/parecerGrupoSubacao&acao=A','cadastrarParecer','width=450,height=550,scrollbars=1');
//		winParecer.focus();
	}
	</script>
  </head>	
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?php
monta_titulo( 'Geração de convênio', '&nbsp;'  );
?>
<form action="" method="post" name="formulario">
<?php 
$sql = sprintf("SELECT
				 COUNT(*) 
				FROM
				 cte.pontuacao p
				 INNER JOIN cte.acaoindicador ai ON p.ptoid = ai.ptoid
				 INNER JOIN cte.subacaoindicador sa ON sa.aciid = ai.aciid 
				 INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid  AND spt.ssuid = 3 AND spt.sptano = to_char(now(),'yyyy')
				 INNER JOIN cte.subacaoconvenio sac ON sac.sbaid = sa.sbaid
				WHERE
				 p.inuid =%d AND
				 p.ptostatus = 'A' AND
				 sac.cnvid IS NULL AND
				 sbcid NOT IN (SELECT sbcid FROM cte.subacaoconvenioparecerpar);",
				$_SESSION['inuid']);
$sub = $db->pegaUm($sql);
unset($sql);	
$sql = sprintf("SELECT
				 DISTINCT
				 '<INPUT TYPE=\'checkbox\' TITLE=\'Marcar/Desmarcar subação\' checked NAME=\'sbaid[]\' VALUE=\'' || sa.sbaid || '\' onclick=\'notCheckButton(this);capturaCheckbox(this);\'>
				 <img src=\'/imagens/gif_inclui.gif\' title=\'Abrir subação\' onclick=\'javascript:opener.alterarSubacao(' || sa.sbaid || ');\'>' as check,
				 sbadsc		 
				FROM
				 cte.pontuacao p
				 INNER JOIN cte.acaoindicador ai ON p.ptoid = ai.ptoid
				 INNER JOIN cte.subacaoindicador sa ON sa.aciid = ai.aciid 
				 INNER JOIN cte.subacaoparecertecnico spt ON spt.sbaid = sa.sbaid  AND spt.ssuid = 3 AND spt.sptano = to_char(now(),'yyyy')
				 INNER JOIN cte.subacaoconvenio sac ON sac.sbaid = sa.sbaid
				WHERE
				 p.inuid =%d AND
				 p.ptostatus = 'A' AND
				 sac.cnvid IS NULL AND
				 sbcid NOT IN (SELECT sbcid FROM cte.subacaoconvenioparecerpar);",
				$_SESSION['inuid']);
#echo "<pre>$sql";
$cabecalho = array('<INPUT TYPE=\'checkbox\' checked NAME=\'selectAll\' ID=\'selectAll\' VALUE=\'\' onclick=\'allSelect(this);\'>','<b>Subação</b>');
$db->monta_lista_simples($sql,$cabecalho, 1000, 1000, 'N', '95%');
?>
<table border="0" width="95%" cellspacing="0" cellpadding="2" align="center">
	<tr bgcolor="#cdcdcd">
		<td width="1%">
			<input type="hidden" name="delSbaid" id="delSbaid" value="" />
			<input type="hidden" name="manager" id="manager" value="" />
		</td>
		<td align="center">
		<?php
		if ($sub){ 
		?>
			<input type="button" value="Parecer Físico/Finânceiro"  onclick="javascript:openParecer('Será dado o parecer FÍSICO/FINÂNCEIRO em todas as subações marcadas!\nConfirma operação?','',1);" />
			&nbsp;&nbsp;
			<BR>	
			<input type="button" value="Voltar para Análise" onclick="confirmar(1,'Será retornado para Análise todas as SubAcões NÃO marcadas!\nConfirma Operação?');" />
			&nbsp;&nbsp;
		<?php
		}
		?>	
			<input type="button" value="Gerar Convênio" onclick="confirmar(2,'Será gerado convênio das SubAções marcadas!\nConfirma Operação?');" />
		</td>
	</tr>
</table>
</form>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<colgroup>
		<col/>
	</colgroup>
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
				<div id="bloco" style="overflow: hidden;">
					<p>
						<a href="javascript: arvore.openAll();">Abrir Todos</a>
						&nbsp;|&nbsp;
						<a href="javascript: arvore.closeAll();">Fechar Todos</a>
					</p>
					<div id="_arvore"></div>
				</div>
			</td>
			<td style="background-color:#fafafa; color:#404040; width: 1px;" valign="top">
			</td>
		</tr>
	</tbody>
</table>
<script type="text/javascript">
	arvore = new dTree( 'arvore' );
	arvore.config.folderLinks = true;
	arvore.config.useIcons = true;
	arvore.config.useCookies = true; 
	
	arvore.add( 0, -1, 'Grupo de Subações (Por parecer)' );
	
<?php
####### MONTAR ÁRVORE #######
// SQL => Pega subações que foi dado o parecer 
// Agrupa por parecer

	$sql =sprintf("SELECT
					 DISTINCT
					 sacp.parid,
					 sa.sbaid,
					 sa.sbadsc,
					 eng.parid AS pareng
					FROM
					 cte.pontuacao p
					 INNER JOIN cte.acaoindicador ai ON p.ptoid = ai.ptoid
					 INNER JOIN cte.subacaoindicador sa ON sa.aciid = ai.aciid 
					 INNER JOIN cte.subacaoconvenio sac ON sac.sbaid = sa.sbaid
					 INNER JOIN cte.subacaoconvenioparecerpar sacp ON sacp.sbcid = sac.sbcid
					 INNER JOIN cte.parecerpar pp ON pp.parid = sacp.parid
					 LEFT  JOIN (SELECT 
									sbcid,
									sacp1.parid
								 FROM
									cte.subacaoconvenioparecerpar sacp1
									INNER JOIN cte.parecerpar pp1 ON pp1.parid = sacp1.parid
								 WHERE 
									pp1.tppid = 2) eng ON eng.sbcid = sac.sbcid
					WHERE
					 p.inuid = %d AND
					 p.ptostatus = 'A' AND
					 sac.cnvid IS NULL AND
					 pp.tppid = 1", 
					$_SESSION['inuid']);

	$trees = $db->carregar($sql);
	if (!empty($trees)){
		$i = 0;
		foreach ($trees as $tree):
			if ($tree['parid'] != $parid || !$parid){
				$i++;
				$onclick  = "onclick=\"javascript:openParecer('Deseja alterar o parecer FÍSICO/FINÂNCEIRO deste grupo de subações?',{$tree['parid']},1);\" ";
				
				if (!$tree['pareng']){
					$onclick2 = "onclick=\"javascript:openParecer('Deseja cadastrar o parecer de ENGENHARIA deste grupo de subações?',{$tree['parid']},2);\" ";  
					$src	  = "/imagens/gif_inclui.gif";
					$title    = "Cadastrar Parecer de ENGENHARIA destas Subações";
				}else{
					$onclick2 = "onclick=\"javascript:openParecer('Deseja alterar o parecer de ENGENHARIA deste grupo de subações?',{$tree['parid']},2);\" ";  
					$src	  = "/imagens/alterar_1.gif";	
					$title	  = "Alterar Parecer de ENGENHARIA destas Subações";		
				}

				echo "arvore.add({$tree['parid']}, 0, \"<img src='/imagens/alterar.gif\' title='Alterar parecer FÍSICO/FINÂNCEIRO destas Subações' ".str_replace('"', '\"', $onclick)." >&nbsp;&nbsp;<img src='".$src."\' title='".$title."' ".str_replace('"', '\"', $onclick2)." >&nbsp;&nbsp;{$i}. Subações agrupadas por parecer\",'javascript:void(0)');\n";
				$parid = $tree['parid'];
			}
			echo "arvore.add({$tree['sbaid']}, {$tree['parid']}, '{$tree['sbadsc']}');\n";
		endforeach;
	}

?>	
	
	
	elemento = document.getElementById( '_arvore' );
	elemento.innerHTML = arvore;	
</script>
</body>
</html>
<?php

if($_REQUEST['valida'] == 'gravaForm'){
		
		$sql = "UPDATE cte.programa SET prgmonitorainterno = 'false';";
		$db->executar($sql);
		
		$sql = "UPDATE cte.programa SET prgmonitorainterno = 'true' WHERE prgid IN ('" . implode("', '", $_REQUEST['prgid']) . "')";
		$db->executar($sql);
		$db->commit();					
}

// Monta Cabeçalho
//include APPRAIZ."includes/cabecalho.inc";

// Monta título da página
//echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Lista de Programas do PAR','');

extract($_POST);
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form id="formPesquisa" name="formPesquisa" method="POST"  action="cte.php?modulo=principal/monitoramentoInterno/listaProgramasMonitoraInterno&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	  
	<tr>
    	<td align='right' colspan="2" class="SubTituloCentro">Filtro de Pesquisa</td>
    </tr>
    <tr>
       <td align='right' width="30%" class="SubTituloDireita">Nome do programa:</td>
       <td>
       	<input type="hidden" id="pesquisa" name="pesquisa" value="true">
		<?=campo_texto('prgdsc','N','S','',80,200,'','');?>				
       </td>
    </tr>    
 	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td></td>
	</tr>
</table>
</form>   
<form id="formulario" name="formulario" method="POST" action="cte.php?modulo=principal/monitoramentoInterno/listaProgramasMonitoraInterno&acao=A">
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">	
	<tr>
		<td colspan="2">
		<input type="hidden" id="valida" name="valida" value="gravaForm">
		<div style="height: 360px; overflow: auto;">
			<?php
			if($_REQUEST['pesquisa'] == 'true'){
				
				$sql = "SELECT 
					'<input type=\"checkbox\" id=\"prgid_' || '' || prgid || '' || '\" name=\"prgid[]\" 
					value=\"' || prgid || '\" ' || 
						CASE WHEN prgmonitorainterno = 't' THEN 'checked=\"checked\"'
						ELSE ''
						END || 
						' \>',
					prgid, 
					prgdsc 
				FROM cte.programa 
				WHERE prgbrasilpro = false
				AND prgstatus = 'A'
				AND prgdsc iLIKE '%".$_REQUEST['prgdsc']."%'
				ORDER BY prgdsc ASC";
				
			} else {
			
				$sql = "SELECT 
					'<input type=\"checkbox\" id=\"prgid_' || '' || prgid || '' || '\" name=\"prgid[]\" 
					value=\"' || prgid || '\" ' || 
						CASE WHEN prgmonitorainterno = 't' THEN 'checked=\"checked\"'
						ELSE ''
						END || 
						' \>',
					prgid, 
					prgdsc 
				FROM cte.programa 
				WHERE prgbrasilpro = false
				AND prgstatus = 'A'
				ORDER BY prgdsc ASC";
				
			}
			//ver($sql, $_REQUEST);
			
			$db->monta_lista( $sql, array( "", "Código", "Descrição" ), 5000, 10, 'N', '', '' );
			?>
		</div>		
		</td>
	</tr>
	           
</table>
</form>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' colspan="2" class="SubTituloDireita">
			<center>
				<input type="button" name="btalterar" value="Pesquisar" onclick="pesquisarPrograma()" class="botao">
				<input type="button" name="btgravar" value="Gravar" onclick="atualizaProgramas()" class="botao">
			</center>	
		</td>
	</tr>
</table>
<script type="text/javascript">
function atualizaProgramas(){
	
	document.formulario.submit();
	alert("Dados gravados com sucesso!");
	window.opener.location.reload();	
	
}

function pesquisarPrograma(){

	document.formPesquisa.submit();
	
}
</script>
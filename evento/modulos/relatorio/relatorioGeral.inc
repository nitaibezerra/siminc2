<?php
header('content-type: text/html; charset=ISO-8859-1');

if ($_REQUEST['exibir'] == 1){
	include "os_resultado.inc";
	//echo 'teste';
	exit;	
}
include APPRAIZ . 'includes/cabecalho.inc';


echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( "Relatório Geral - Por Item", '' );
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript">
		
		function carregarRelatorio()
		{
			var processo = document.getElementById('idprocesso');

			if(!processo.value)
			{
				alert('Selecione um processo');
				return false;
			}
			
				formulario.target = 'resultadoGeral';
				var janela = window.open( '', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
				
				document.getElementById('exibir').value = 1;
				document.getElementById('idproc').value = processo.value;
				document.getElementById('filtro').submit();
		}
	</script>
<form action="" method="post" name="formulario" id="filtro"> 	
<input type="hidden" name='exibir' id='exibir' value=''>
<input type="hidden" name='idproc' id='idproc' value=''>
<table  align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
 <tr>
      	 <td class="subtitulodireita"> 	
      	 	Processo:
      	 </td>
    	<td align="left">
   			<?
				$sql = "SELECT 
						copid as codigo,
						copnumprocesso || ' - ' || copdsc as descricao 
					FROM 
						evento.coprocesso
					WHERE
						copstatus = 'A'	
						";
				
				$db->monta_combo('Processo',$sql,'S','Selecione uma opção','','','','','','idprocesso');
   			?>
   		</td>
   <tr>
   <td>
   </td>
   	<td>
   		<input type="button" onclick="carregarRelatorio();" value="Gerar relatório">
   	</td>
   </tr>		
 </table>  	


	
</form>

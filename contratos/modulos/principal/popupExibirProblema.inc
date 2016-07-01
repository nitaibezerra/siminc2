<?php

monta_titulo( 'Problemas na execução','');

$ctproblemas = new CtProblemas();
$arrDados = $ctproblemas->pesquisaProblema($_GET['prbid']);
$arrDados = $arrDados[0];
extract($arrDados);

?>
<script	type="text/javascript" src="../includes/JQuery/jquery-1.10.2.min.js"></script>
 
<script type="text/javascript"src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form name="formulario" id="formulario" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
			<tr>            
				<td class="SubTituloDireita" align="right"><b> Data de Inclusão: </b></td>
					<td>
						<?php 
							echo $prbdatainclusao;
						?>
					</td>
			</tr>	

			<tr>            
				<td class="SubTituloDireita" align="right"><b> Usuário: </b></td>
					<td>
						<?php 
							echo $usunome;
						?>
					</td>
			</tr>	
			
			<tr>            
				<td class="SubTituloDireita" align="right"><b> Data da Ocorrência: </b></td>
					<td>
						<?php 
							echo $prbdataocorr;
						?>
					</td>
			</tr>	
			
			<tr>            
				<td class="SubTituloDireita" align="right"><b> Título do Problema: </b></td>
					<td>
						<?php 
							echo $prbtitulo;
						?>
					</td>
			</tr>	
			
			<tr>            
				<td class="SubTituloDireita" align="right"><b> Descrição do Problema: </b></td>
					<td>
						<?php 
							echo campo_textarea('prbdsc', 'N', 'N', '', '80', '5', null,'','','','','',$prbdsc);
						?>
					</td>
			</tr>	
			
	</form>
	<table class="Rtabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
		<tr>
	        <td class="SubTituloCentro" colspan="2" >
	        	<input type="button" value="Fechar" onclick="fecharPopup()" />
	        </td>
		</tr>
	</table>

<script>
function fecharPopup(){
	window.close();
}
</script>	
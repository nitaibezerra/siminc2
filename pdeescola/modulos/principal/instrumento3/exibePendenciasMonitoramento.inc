<?php 
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();
?>
<html>
<head>
<title>
Verificação de pendências em subações
</title>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
<link rel="stylesheet" type="text/css" href="/includes/listagem.css">
<script>

//function abrePlanoDeAcao()
//{
//	     return window.open('pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A',
//                  'abrePlanoDeAcao',
//                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
//}

function vaiPraRaiz()
{
	     return window.open('pdeescola.php?modulo=principal/propostaMonitoramento&acao=A',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

</script>
</head>
<body>
<div style="width:100%;height:100%;overflow-y:scroll;">
<table class="tabela">
<tr>
<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
O sistema verificou:
<br/>
<br/>
<?
	//pre( $_SESSION['erros'] );
	/*******************************
	* 	FUNCTION: exibeErrosSubAcao();
	* 	26/06/2008
	*   DESCRIÇÃO:
	*	Ao gerar convênio com FNDE monta uma tela com
	*   os erros de validações de envio. 
	*   (Dados que faltam ser preencher na tela Subaçãoes.)
	*   @PARAM  $erros - array (array de erros)
	* 
	*******************************/
	function exibeErros($erros) {
		foreach($erros as $idsubacao => $dados) {
			echo "<tr style=\"background-color: #d9d9d9;\">";
			echo "<td>";
			if($dados['parecerGeral']) {
				echo "<b> - ".$dados['parecerGeral']."</b><br />";
			}
			echo "</td></tr>";
		}
		echo "</table>";
	}

?>
<span style="font-weight:normal;">
</span>
</td>
</tr>
<tr>
	<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900"></td>
</tr>
<tr style="background-color: #d9d9d9;">
	<td>
		<img src="/imagens/consultar.gif" onclick="vaiPraRaiz();">
	</td>
	<td>
		<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="vaiPraRaiz();">
	 		- Pendências no preenchimento das questões do Monitoramento:
	 	</strong>
	</td>
</tr>
<tr>
	<td colspan="2">
		<ul style="margin-bottom:0px; margin-top:0px;">
		 	<li style="font-size:12px; margin-bottom:0px; margin-top:0px;">
				<?
				$sql = "
					SELECT 
						count( q.qapid ) 
					FROM 
						pdeescola.questaoavaliacaoplano as q 
					INNER JOIN 
						pdeescola.avaliacaoplano as a on a.qapid = q.qapid
					WHERE 
						a.pdeid = ".$_SESSION['pdeid'];
				
				$qtdQuestoes = $db->pegaUm( $sql );
				$maxQuestoes = 55; 
				
				if($qtdQuestoes < $maxQuestoes){ 
					echo' <b>'.$qtdQuestoes .'</b> questão(ões) preenchidas de um total de <b>'.$maxQuestoes.'</b> ';  
				}else{ 
					echo' Não foram encontradas pendências.'; 
				} 
				?>
		 	</li>
	 	</ul>
	</td>
</tr>
</body>
</table>
</html><!-- Tempo de Execução: 0.474992990494 --> 
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

function cadastraJustificativa()
{
	     return window.open('pdeescola.php?modulo=principal/sintese_autoavaliacao/cadastraJustificativa&acao=A',
                  'cadastraJustificativa',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function abrePlanoDeAcao()
{
	     return window.open('pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A',
                  'abrePlanoDeAcao',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function vaiPraRaiz()
{
	     return window.open('pdeescola.php?modulo=principal/estrutura_avaliacao&acao=A',
                  'abrePlanoDeAcao',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

//pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A
</script>
</head>
<body>
<div style="width:100%;height:100%;overflow-y:scroll;">
<table class="tabela">
<tr>
<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
O sistema verificou que há algumas pendências no preenchimento do PDE Escola:
<br/>
<br/>

<?
/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );

if($estado ==(ENVIADO_PARA_PAGAMENTO_WF)){
?>
	<tr align="center" style="background-color: #d9d9d9;">
		<td>
			<strong style="padding-left:5px;font-size:13px;cursor:pointer;" >
	 			Os planos desta escola foram Enviados para pagamento - Parcela Complementar
	 		</strong>
	 	</td>
	</tr>
<?php } else{
			confereValorPaf();
			$num = 0;

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
<?
if( $_SESSION['erros']['preenchimento'] == 'erro1')
{
	$num ++;
?>
	<tr>
	<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
 
	 
	</td>
	</tr>
	<tr style="background-color: #d9d9d9;">
	<td>
	<img src="/imagens/consultar.gif" onclick="vaiPraRaiz();">
	</td>
	<td>
	<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="vaiPraRaiz();">
	 - Pendencias no preenchimento das questões do PDE Escola:
	 </strong>
	 </td>
	 </tr>
	 <tr>
	 <td colspan="2">
	 <ul style="margin-bottom:0px; margin-top:0px;">
	 <li style="font-size:12px; margin-bottom:0px; margin-top:0px;">
	<?
	$sql = "SELECT count(pprid) FROM pdeescola.pdepreenchimento WHERE pdeid = '{$_SESSION['pdeid']}'";
	$qtdQuestoes = $db->pegaUm( $sql );
	 
	$maxQuestoes = retornaNumeroQuestoes($_SESSION['pdeid']);
	
	echo '<b>'.$qtdQuestoes .'</b> questões preenchidas de um total de <b>'.$maxQuestoes.'</b>';
	?>
	 </li>
	 </ul>
	 </td>
 	</tr>
<?
}
 
if( $_SESSION['erros']['custeio'] != '' )
{
	$num ++;
?>
	<tr>
	<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
 
	 
	</td>
	</tr>
	<tr style="background-color: #d9d9d9;">
	<td>
	<img src="/imagens/consultar.gif" onclick="abrePlanoDeAcao();">
	</td>
	<td>
	<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="abrePlanoDeAcao()">
	 - Dados inconsistentes no plano de ação na declaração do valor do PAF:
	 </strong>
	 </td>
	 </tr>
	 <tr>
	 <td colspan="2">
	 <ul style="margin-bottom:0px; margin-top:0px;">
	 <li style="font-size:12px; margin-bottom:0px; margin-top:0px;">
	<?

	$arrValores = explode( "_", $_SESSION['erros']['dados']['paf'] );
		
	echo $_SESSION['erros']['custeio'];
		
	echo '<br>';
	echo '<br>';
	echo 'Valor informado para o custeio: '.number_format($arrValores[3],2,',','.');
	echo '<br>';
	echo 'Valor exigido para o custeio:   '.number_format($arrValores[2],2,',','.');
	echo '<br>';
	echo '<br>';
	echo 'Valor informado para o capital: '.number_format($arrValores[1],2,',','.');
	echo '<br>';
	echo 'Valor exigido para o capital:   '.number_format($arrValores[0],2,',','.');
	?>
	 </li>
	 </ul>
	 </td>
 	</tr>
<?
}

if( $_SESSION['erros']['itens'] != '' )
{
	$num ++;
?>
	<tr>
	<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
 
	 
	</td>
	</tr>
	<tr style="background-color: #d9d9d9;">
	<td>
	<img src="/imagens/consultar.gif" onclick="abrePlanoDeAcao();">
	</td>
	<td>
	<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="abrePlanoDeAcao()">
	 - Dados inconsistentes no plano de ação na declaração de ítens de contratação de serviços:
	 </strong>
	 </td>
	 </tr>
	 <tr>
	 <td colspan="2">
	 <ul style="margin-bottom:0px; margin-top:0px;">
	 <li style="font-size:12px; margin-bottom:0px; margin-top:0px;">
	<?
		
	echo $_SESSION['erros']['itens'];
		
	?>
	 </li>
	 </ul>
	 </td>
 	</tr>
<?
}

if( $_SESSION['erros']['naodeclarados'] != '' )
{
	$num ++;
?>
	<tr>
	<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
 
	 
	</td>
	</tr>
	<tr style="background-color: #d9d9d9;">
	<td>
	<img src="/imagens/consultar.gif" onclick="abrePlanoDeAcao();">
	</td>
	<td>
	<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="abrePlanoDeAcao()">
	 - Dados inconsistentes no plano de ação: informação não declarada para o PAF. 
	 </strong>
	 </td>
	 </tr>
	 <tr>
	 <td colspan="2">
	 <ul style="margin-bottom:0px; margin-top:0px;">
	 <li style="font-size:12px; margin-bottom:0px; margin-top:0px;">
	<?
		
	echo $_SESSION['erros']['naodeclarados'];
		
	?>
	 </li>
	 </ul>
	 </td>
 	</tr>
<?
}

if( count( $_SESSION['erros']['programas_pendentes']) > 0 )
{
	$num ++;
?>
	<tr>
		<td colspan="2" style="text-align:center;font-size:14px;font-weight:bold;color:#900">
	 
		 
		</td>
	</tr>
	<tr style="background-color: #d9d9d9;">
		<td>
		<img src="/imagens/consultar.gif" onclick="cadastraJustificativa()">
		</td>
	<td>
	<strong style="padding-left:5px;font-size:13px;cursor:pointer;" onclick="cadastraJustificativa()">
	 - Falta informar ou justificar valores não informados no Plano de Ação:
	 </strong>
	 </td>
	 </tr>
	 <tr>
	 <td colspan="2">
	 <ul style="margin-bottom:0px; margin-top:0px;">
	 <li style="font-size:12px; margin-bottom:0px; margin-top:0px;" onclick="cadastraJustificativa()">
	<?
	echo 'Você deve preencher ou justificar o não preenchimento os programas financiados no plano de ação.';  
 
	?>
	 </li>
	 </ul>
	 </td>
 	</tr>
<?
} 

if( $num > 0 )
{
	echo '<tr><td></td>';
	echo '<td>';
	echo '<br>';
	echo'<center>Foram encontradas '. $num.' pendências.  </center>'; 
	echo '</td></tr>';
}

else
{
	echo '<tr><td></td>';
	echo '<td>';
	echo' <center>Não foram encontradas pendências.</center>';
	echo '</td></tr>';
}
}
?>
 </table></body></html><!-- Tempo de Execução: 0.474992990494 --> 
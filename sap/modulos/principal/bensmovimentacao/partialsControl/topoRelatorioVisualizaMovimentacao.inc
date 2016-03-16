<script language="JavaScript" src="../includes/funcoes.js"></script>		 
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<style type="text/css" media="print">

.noPrint{ 
	display:none; 
} 

</style>

<style type="text/css">
table.bordasimples {border-collapse: collapse; width:95%;}
table.bordasimples tr {border:1px solid #cccccc;}
</style>

<?php
$oMovimentacao = new BensMovimentacao();
$oTipoMov = new TipoMovimentacao();

$cabecalho = '<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">'
				.'	<tr bgcolor="#ffffff">' 	
				.'		<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>'			
				.'		<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">'				
				.'			SIMEC- Sistema Integrado do Ministério da Educação<br/>'				
				.'			Ministério da Educação <br />'
				.'			Secretaria Executiva<br />'
				.'			Subsecretaria de Assuntos Administrativos<br />'
				.'			Coordenação Geral de Recursos Logísticos'
				.'		</td>'
				.'		<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">'	
				.'			<a class="noPrint" href="javascript:imprimir();"><img src="../imagens/print2.gif" border="0" width="20"></a><br/>'				
				.'			Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>'					
				.'			Hora da Impressão: ' . date( 'd/m/Y - H:i:s' ) . '<br />'					
				.'		</td>'					
				.'	</tr><tr>'
				.'		<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">'
				.'			<b><font style="font-size:14px;">' . $_REQUEST["titulo"] . '</font></b>'
				.'		</td>'
				.'	</tr>'					
				.'</table>';

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
						"<td colspan=\"100\" align=\"right\">" .			
							$cabecalho .
						"</td>" .
				  	"</tr>
				  	</table>";

echo $cabecalhoBrasao;

?>
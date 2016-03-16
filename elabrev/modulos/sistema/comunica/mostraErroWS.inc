<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloCentro">Log de Erro WS</td>
	</tr>
</table>
<?php

$sql = "SELECT 
		  logtipo,
		  acacod||' ',
		  unicod,
		  prgcod,
		  loccod,
		  prgano,
		  usucpf,
		  logerro
		FROM 
		  elabrev.logerrows l 
		/*WHERE to_char(l.logdatainclusao, 'DD/MM/YYYY') = to_char(now(), 'DD/MM/YYYY')*/
		WHERE l.logdatainclusao = (select max(logdatainclusao) from elabrev.logerrows)
		GROUP BY 
          logtipo,
		  acacod,
		  unicod,
		  prgcod,
		  loccod,
		  prgano,
		  usucpf,
		  logerro
		order by unicod";

$cabecalho = array('Tipo de Função', 'Ação', 'UO', 'Programa', 'Localizador', 'Ano', 'Usuário', 'Log');
$db->monta_lista( $sql, $cabecalho, 100000, 20, '', '' ,'' );
?>
<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align="center" bgcolor="#D5D5D5" colspan="2">
			<input type="button" name="btn_cancelar" onclick="javascript: window.close();" value="Fechar" />
		</td>
	</tr>
</table>
<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="JavaScript" src="./js/sispacto.js"></script>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Relatório Final</td></tr>		
		<tr>
			<td><? 
			$sql = "select uu.unisigla||' - '||uu.uninome as universidade, e.esddsc from sispacto.relatoriofinal r
			inner join workflow.documento d on d.docid = r.docid
			inner join workflow.estadodocumento e on e.esdid = d.esdid
			inner join sispacto.universidadecadastro u on u.uncid = r.uncid
			inner join sispacto.universidade uu on uu.uniid = u.uniid
			order by e.esddsc";
			
			$cabecalho = array("Universidade","Situação do relatório final");
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','',true, false, false, true);
			
			
			?></td>
		</tr>
</table>
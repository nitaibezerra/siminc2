<?
/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
        <tr><td class="SubTituloEsquerda">Informações sobre adesões</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "SELECT

					CASE WHEN i.estuf  IS NOT NULL THEN 'ESTADUAL'
					     WHEN i.muncod IS NOT NULL THEN 'MUNICIPAL' END as rede,
					CASE WHEN i.estuf  IS NOT NULL THEN est.estuf||'/'||est.estdescricao 
					     WHEN i.muncod IS NOT NULL THEN mun.estuf||'/'||mun.mundescricao END as descricao,
					us.usucpf||' - '||us.usunome as resp,
					to_char(adpdataresposta,'dd/mm/YYYY') as dataadesao
					
					FROM par.pfadesaoprograma p 
					INNER JOIN workflow.documento d on d.docid = p.docid 
					INNER JOIN workflow.estadodocumento e on e.esdid = d.esdid
					INNER JOIN par.instrumentounidade i on i.inuid = p.inuid 
					LEFT JOIN seguranca.usuario us ON us.usucpf = p.usucpf
					LEFT JOIN territorios.estado est ON est.estuf = i.estuf 
					LEFT JOIN territorios.municipio mun ON mun.muncod = i.muncod 
					WHERE p.pfaid=8 AND p.adpresposta='S' 
					
					ORDER BY 1,2";
			
			
			$cabecalho = array("Rede","Estado/Município","Quem fez adesão","Data da adesão");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>
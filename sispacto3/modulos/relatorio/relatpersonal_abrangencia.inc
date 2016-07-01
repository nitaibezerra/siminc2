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
        <tr><td class="SubTituloEsquerda">Abrangência das IES</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select m.estuf, m.mundescricao, case when a.esfera='M' then 'Municipal' when a.esfera='E' then 'Estadual' end as esfera, uu.unisigla, uu.uninome from territorios.municipio m 
left join sispacto3.abrangencia a on a.muncod = m.muncod 
left join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
left join sispacto3.universidadecadastro u on u.uncid = e.uncid 
left join sispacto3.universidade uu on uu.uniid = u.uniid 
order by 3,1,2";
			
			
				
			$cabecalho = array("UF","Município","Esfera","Sigla IES", "IES");

			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>
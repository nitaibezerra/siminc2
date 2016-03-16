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
        <tr><td class="SubTituloEsquerda">Dados Gerais das turmas</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select 
					m.estuf || ' - ' || m.mundescricao as polo,
					turdesc as nometurma,
					i.iusnome as formador
					from sispacto.turmas tu 
					inner join territorios.municipio m on m.muncod = tu.muncod
					inner join sispacto.identificacaousuario i on i.iusd = tu.iusd 
					inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=848 
					order by 1,2";
			
			$cabecalho = array("Poló","Turma","Formador");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
</table>
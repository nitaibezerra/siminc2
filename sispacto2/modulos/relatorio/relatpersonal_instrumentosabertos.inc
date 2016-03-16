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
        <tr><td class="SubTituloEsquerda">Relatório dos instrumentos dissertativos</td></tr>		
		<tr>
			<td>
			<? 
			$sql = "select uu.unisigla||' - '||uu.uninome as universidade, p.pfldsc, i.iusnome, it.irapergunta1, it.irapergunta2, it.irapergunta3, it.irapergunta4, it.irapergunta5 
from sispacto2.instrumentosrespostasabertas it 
inner join sispacto2.identificacaousuario i on i.iusd = it.iusd 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
inner join seguranca.perfil p on p.pflcod = t.pflcod 
inner join sispacto2.universidadecadastro u on u.uncid = i.uncid 
inner join sispacto2.universidade uu on uu.uniid = u.uniid 
order by 1,2,3";
			
			$cabecalho = array("IES",
							   "Perfil",
							   "Nome",
							   "1. Quais as principais dificuldades na execução do processo de formação continuada no âmbito do PNAIC?",
							   "2. Quais as principais facilidades na execução do processo de formação continuada no âmbito do PNAIC?",
							   "3. Como você analisa a qualidade pedagógica do material de formação do PNAIC?",
							   "4. Quais são as barreiras e facilitadores que influem na realização das suas atribuições no âmbito do PNAIC?",
							   "5. Em sua opinião, qual é a contribuição real do PNAIC para a alfabetização das crianças?");
			
			$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
			 
			?>
			</td>
		</tr>
		

</table>
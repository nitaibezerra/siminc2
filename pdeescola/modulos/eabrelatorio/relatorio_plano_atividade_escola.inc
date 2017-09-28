<?

ini_set( "memory_limit", "512M" );
set_time_limit(0);

?>

<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<style>

table.tabelaRelatorio {
	border-left: 1px solid #000000;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 9px;
}

table.tabelaRelatorioCompleta {
	border-right: 1px solid #000000;
	border-left: 1px solid #000000;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 9px;
}

table.tabelaRelatorioEsquerda {
	border-left: 1px solid #000000;
	font-size: 9px;
}


td.tituloRelatorio {
	border-right: 1px solid #000000;
	background-color:#c0c0c0;
	border-top: 1px solid #000000;
	border-bottom: 1px solid #000000;
	
}

td.bordaDireita {
	border-right: 1px solid #000000;
	font-size: 9px;
}

td.bordaBaixo {
	border-bottom: 1px solid #000000;
	font-size: 9px;
}

td.bordaDireitaBaixo {
	border-right: 1px solid #000000;
	border-bottom: 1px solid #000000;
	font-size: 9px; 
}

</style>
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td valign="top" width="50" rowspan="2">
			<img src="../imagens/brasao.gif" width="45" height="45" border="0">
		</td>
		<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				FNDE - Fundo Nacional de Desenvolvimento da Educação<br />
				<?php echo NOME_SISTEMA; ?><br/>
				Programa Dinheiro Direto na Escola(PDDE)<br />
				Funcionamento das Escolas nos Finais de Semana
			</font>
		</td>
		<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
			<font style="font-size:11px;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Data/Hora da Impressão: <b><?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
			</font>
		</td>
	</tr>
		<tr><td>&nbsp;</td></tr>
</table>
<?php 

		$sql = "SELECT DISTINCT
				est.estdescricao as est,
				est.estuf,
				mun.mundescricao as mun,
				ent.entnome as esc
			FROM
				entidade.entidade ent 
			INNER JOIN 
				entidade.endereco ende ON ent.entid = ende.entid
			INNER JOIN 
				territorios.municipio mun ON mun.muncod = ende.muncod
			INNER JOIN 
				territorios.estado est ON est.estuf = mun.estuf		
			WHERE
				--ent.funid = 3 and
			  	ent.tpcid IN (1,3) AND
		    	ent.entid IN ('{$_SESSION['entid']}')";
		
		$dadosEscola = $db->pegaLinha( $sql );
		
		$edrid = $db->pegaUm("select edrid from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		
?>
<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" colspan="10" class="bordaDireita"><font size="3"><b>PLANO DE ATIVIDADES DA ESCOLA - <?= $_SESSION["exercicio"] ?></b></font></td>
	</tr>
	<tr>
		<td align="center" colspan="10" class="tituloRelatorio"><b>PDDE/FEFS</b></td>
	</tr>
	<tr>
	<?php 
		$entcodent = $db->pegaUm("select entcodent from pdeescola.eabescolaaberta where entid = ".$_SESSION['entid']);
		?>
		<td align="left" colspan="3" class="bordaDireitaBaixo"><b>CÓDIGO DA ESCOLA (Censo/INEP):</b><br /><?= ($entcodent) ? ($entcodent) : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="6" class="bordaDireitaBaixo"><b>ESCOLA:</b><br /><?= ($dadosEscola['esc']) ? $dadosEscola['esc'] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="1" class="bordaDireitaBaixo"><b>Nº DE ALUNOS</b></td>		
	</tr>
	<tr>
		<td align="left" class="bordaDireitaBaixo" width="10%" ><b>UF:</b><br /><?= ($dadosEscola['estuf']) ? $dadosEscola['estuf'] : "NÃO CADASTRADO" ?></td>
		<td align="left" colspan="6" class="bordaDireitaBaixo"><b>MUNICÍPIO</b><br /><?= ($dadosEscola['mun']) ? $dadosEscola['mun'] : "NÃO CADASTRADO" ?></td>
		<?php 
		$entid = $db->pegaUM("select tpcid from entidade.entidade where entid = ".$_SESSION['entid']);
		?>
		<td align="left" colspan="3" class="bordaDireitaBaixo"><b>ESFERA</b><br /><?php if( $entid == 1 ){ echo 'Estadual'; } elseif( $entid == 3 ){ echo 'Munincipal'; } ?></td>
	</tr>
	<tr>
		<?php 
			$sql = "SELECT
						eaa.eaadescricao
					FROM
						pdeescola.eabtipoanoanterior eaa
					INNER JOIN
						pdeescola.eabescolaanoanterior eea ON eea.eaaid = eaa.eaaid
														  AND eea.eabid = ".$_SESSION["eabid"]."
					WHERE
						eaa.eaastatus = 'A'";
			$anosEab = $db->carregarColuna($sql);
			
			$eabqtduexatende = $db->pegaUm("select eabqtduexatende from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		?>
		<td align="left" colspan="6" class="bordaDireitaBaixo"><b>FUNCIONAMENTO DO ESCOLA ABERTA EM ANOS ANTERIORES</b><br />
		<?php
			echo implode(", ", $anosEab);
		?>
		</td>
		<?php 
		$eabanoatual = $db->pegaUm("select eabanoatual from pdeescola.eabescolaaberta where eabid = ".$_SESSION["eabid"]);
		?>
		<td align="left" colspan="4" class="bordaDireitaBaixo"><b>FUNCIONAMENTO NOS FINAIS DE SEMANA EM <?=date('Y')?></b><br /><?php echo ($eabanoatual == 't') ? 'SIM': 'NÃO' ; ?></td>
	</tr>
	<tr>
		<td align="left" colspan="7" class="bordaDireita"><b>DIAS DE FUNCIONAMENTO EM <?=date('Y')?>/2012:</b><br /><?php if( $edrid == 1 ){ echo 'Sábado'; } elseif( $edrid == 2 ){ echo 'Domingo'; } else { echo 'Sábado e Domingo'; } ?></td>
		<td align="left" colspan="3" class="bordaDireita"><b>UEx CENTRAL:</b><br /><?= ($eabqtduexatende) ? "Sim: " . $eabqtduexatende . " Escola(s)" : "Não" ?></td>
	</tr>
</table>
<br />
<table class="tabelaRelatorioEsquerda" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="center" colspan="17" class="tituloRelatorio"><font size="1"><b>PLANO DE ATIVIDADES DA ESCOLA</b></font></td>
	</tr>
	<tr>
		<td align="center" width="25%" class="bordaDireitaBaixo" >ÁREA TEMÁTICA<br /></td>
		<td align="center" width="15%" class="bordaDireitaBaixo" >NOME DA ATIVIDADE/OFICINA<br /></td>
		<td align="center" width="10%" class="bordaDireitaBaixo" >TURNO</td>
		<td align="center" width="5%" class="bordaDireitaBaixo" >DIA DE REALIZAÇÃO</td>
		<td align="center" width="10%" class="bordaDireitaBaixo" >DURAÇÃO DA ATIVIDADE/OFICINA<br /></td>
		<td align="center" width="5%" class="bordaDireitaBaixo" >RESSARCIDA</td>
		<td align="center" width="20%" class="bordaDireitaBaixo" >ESPAÇO UTILIZADO<br /></td>
		<td align="center" width="10%" class="bordaDireitaBaixo" >Nº DE PARTICIPANTES POR OFICINA<br /></td>
	</tr>

<?php
 
$sql = "SELECT
			
			eat.eatdescricao,
			eao.eaodescricao,
			ead.eaddescricao,
			eaa.eaaqtdparticipante,
			CASE WHEN eaaturno = 'M'
			THEN 'Manhã' ELSE 'Tarde' END as eaaturno,
			edr.edrdescricao,
			eae.eaedescricao,
			eaa.eabressarcida
		FROM
			pdeescola.eabatividade eaa
		INNER JOIN
			pdeescola.eabtipoatividade eat ON eat.eatid = eaa.eatid
		INNER JOIN
			pdeescola.eabtipooficina eao ON eao.eaoid = eaa.eaoid
		INNER JOIN
			pdeescola.eabtipoduracaooficina ead ON ead.eadid = eaa.eadid
		INNER JOIN
			pdeescola.eabdiarealizacao edr ON edr.edrid = eaa.edrid
		INNER JOIN
			pdeescola.eabtipoespaco eae ON eae.eaeid = eaa.eaeid
		INNER JOIN
			pdeescola.eabescolaaberta eab ON eab.eabid = eaa.eabid
		WHERE
			eaa.eabid = ".$_SESSION["eabid"];

$dados = $db->carregar( $sql );
//ver($dados,d);
$dados = $dados ? $dados : array();
		
foreach( $dados as $dado ){

echo "<tr>
		<td align='center' 'width=25%' class='bordaDireitaBaixo' >".$dado['eatdescricao']."<br /></td>
		<td align='center' 'width=15%' class='bordaDireitaBaixo' >".$dado['eaodescricao']."<br /></td>
		<td align='center' 'width=10%' class='bordaDireitaBaixo' >".$dado['eaaturno']."<br /></td>
		<td align='center' 'width=5%' class='bordaDireitaBaixo' >".$dado['edrdescricao']."<br /></td>
		<td align='center' 'width=10%' class='bordaDireitaBaixo' >".$dado['eaddescricao']."<br /></td>
		<td align='center' 'width=5%' class='bordaDireitaBaixo' >".($dado['eabressarcida']=='t' ? 'Sim' : 'Não')."<br /></td>
		<td align='center' 'width=20%' class='bordaDireitaBaixo' >".$dado['eaedescricao']."<br /></td>
		<td align='center' 'width=10%' class='bordaDireitaBaixo' >".$dado['eaaqtdparticipante']."<br /></td>
	</tr>";
}

?>	
</table>

<br />

<table class="tabelaRelatorioCompleta" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" style="background-color:#c0c0c0;" class="bordaBaixo"><b>
		JUSTIFICATIVA DE MUDANÇA DE FUNCIONAMENTO PARA APENAS 01 DIA<br /> 
		OBS: Opção válida apenas para as escolas que receberam recursos em 2011 para 02 (dois) dias de funcionamento, pois as demais, obrigatoriamente, só poderão optar por 01 (um) dia.</b></td>
	</tr>
	<tr valign="bottom" style="height:50px;">
		<td align="center" valign="middle">
		<?php 
		$etjdescricao = $db->pegaUm("select eajoutrajustificativa
									 from pdeescola.eabjustificativa 
									 where eabid = ".$_SESSION["eabid"]);
		echo $etjdescricao;
		?>
		</td>
	</tr>
</table>
<br />
<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" style="background-color:#c0c0c0;" class="bordaDireitaBaixo"><b>LOCAL E DATA</b></td>
	</tr>
	<tr valign="bottom" style="height:50px;">
		<td align="center" valign="middle" class="bordaDireita">
		_________________________________________________(____) , _______ de __________________________ de <?=date('Y')?>.
		</td>
	</tr>
</table>
<br />
<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" style="background-color:#c0c0c0;" class="bordaDireitaBaixo"><b>APROVAÇÃO DO CONSELHO ESCOLAR</b></td>
	</tr>
	<tr valign="bottom" style="height:80px;">
		<td align="center" class="bordaDireita">
		__________________________________________________________________________________
		<br />
		Assinatura de membro <b>da escola</b> pertencente ao Conselho Escolar
		<br /><br />
		Função no Conselho: _________________________________
		</td>
		<td align="center" class="bordaDireita">
		__________________________________________________________________________________
		<br />
		Assinatura de membro <b>da comunidade</b> pertencente ao Conselho Escolar
		<br /><br />
		Função no Conselho: _________________________________
		</td>
	</tr>
</table>
<br />
<table class="tabelaRelatorio" border="0" width="100%" align="center" cellspacing="0" cellpadding="2">
	<tr>
		<td align="left" colspan="5" style="background-color:#c0c0c0;" class="bordaDireitaBaixo"><b>APROVAÇÃO DA EEX</b></td>
	</tr>
	<tr valign="bottom" style="height:80px;">
		<td align="center" class="bordaDireita">
		__________________________________________________________________________________
		<br />
		Assinatura do(a) Coordenador(a) Interlocutor(a) do Programa Escola Aberta
		</td>
	</tr>
</table>
<br />
<table align="center">
	<tr>
		<td>
		¹A escola que não recebeu recursos do PDDE/FEFS em 2011 só pode abrir 01 (um) dia no final de semana (sábado ou domingo). <br />
		²Para valorizar a diversidade de atividades nos finais de semana, a escola deve realizar, no mínimo, 01 (uma) oficina de cada área temática. 
		As áreas temáticas que classificam as atividades / oficinas do Programa são: (A)Cultura e Arte; (B)Esporte/ Lazer e Recreação; (C)Qualificação para o Trabalho/Geração de Renda e 
		(D)Formação Educativa Complementar. A tabela de referência para colaborar com a classificação das atividades / oficinas desenvolvidas pelas escolas pode ser encontrada no final do 
		documento que traz as instruções de preenchimento deste Plano de Atividades da Escola. <br />
		³A tabela de referência para colaborar com a indicação do tipo de espaço utilizado para a realização da atividade / oficina também está disponível nas instruções de preenchimento.
		</td>
	</tr>
</table>

<?
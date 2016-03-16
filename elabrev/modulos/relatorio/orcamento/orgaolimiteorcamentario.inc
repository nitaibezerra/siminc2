<?
	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: consultalimites.inc
		Finalidade: relatório para os valores limites da proposta agrupado por Grupo de matrizes, Matrizes e Recursos (Fontes - Tesouro e Próprios)

$grupo = codigo do grupo matriz valor default = 0
	--> O valor 0 indica que é o total geral e se for diferente de 0 é o somatório por grupo
$ppoid = código da proposta
$unicod = código da unidade
Função para retornar o somatório da proposta por grupo matriz e/ou com todos os grupos matrizes, por unidade
*/



function ExecutaQuery ( $grupo )
{
	global $db;
	
	$Query ="select ";
			if ($grupo!=0)
	$Query .= " codGrupo, descGrupo, ";

	$Query .= "		SUM ( coalesce ( valorTesouro, 0) ) as valorTesouro, SUM ( coalesce ( valorOutros, 0) ) as valorOutros ".
			"	from ".
			"	( ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, ".
			"			Case m.mtrmatriz ".
			"				when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) ".
			"						else SUM ( coalesce ( lf.lmfvalor, 0) ) ".
			"				end as valorTesouro, ".
			"			0 as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte lf ON ".
			"		lf.mtmid = mm.mtmid and (lf.foncod like'1%' or lf.foncod like'3%') ".
			"   inner join elabrev.propostaorcamento po on po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			//"	where mm.ppoid = ".$ppoid." ".
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, m.mtrmatriz ".
			"	union all ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, ".
			"			0 as valorOutros, ".
			"			Case m.mtrmatriz ".
			"				when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) ".
			"						else SUM ( coalesce ( lf.lmfvalor, 0) ) ".
			"				end as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte lf ON ".
			"		lf.mtmid = mm.mtmid and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') ".
			"   inner join elabrev.propostaorcamento po on po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			//"	where mm.ppoid = ".$ppoid." ".
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, m.mtrmatriz ".
			"	) a ";
			if ($grupo!=0)
			{
				$Query .= "	where a.codGrupo = ".$grupo." ".
							"	group by ".
				      		"	codGrupo, descGrupo ";
			}

		$RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
	
			for ($i=0; $i<=$nlinhas;$i++)
			{
			  	$res = $db->carrega_registro($RS,$i);
			}
	

		}
			
			return $res;
}



 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com)
   Módulo:conMontante.inc
   Finalidade: permitir a consulta do cadastro da proposta por Montante Categoria Econômica e Montante por Coluna
 */

 include APPRAIZ."includes/cabecalho.inc"; 
 
?>
<br/>
<? $db->cria_aba( $abacod_tela, $url, '' ); ?>
<script type="text/javascript">

    function Consultar(codigo) {
	    document.formulario.act.value = "consultar";
	    document.formulario.unidade.value = codigo;
	    document.formulario.submit();
    }
    function SubmeterConsulta(){
        document.formulario.act.value = "listar";
	    document.formulario.unidade.value = '';
	    document.formulario.submit();
    }
    function mudaProposta(){
        document.formulario.act.value = "";
	    document.formulario.unidade.value = '';
	    document.formulario.submit();
    }

</script>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<?

	$sql =
		" Select count(ppostatus) as codigo " .
		" from elabrev.propostaorcamento " .
		" where ppoanoexercicio = '".$_SESSION['exercicio']."'";
	$qtdPropostas =(integer)$db->pegaUm( $sql );
	
?>


<form method="POST" name="formulario" style="margin:0px">

<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value="">
<input type=hidden name="unidade" value="">
<? monta_titulo( $titulo_modulo, '' ); ?>
</form>


						<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="0"  cellPadding="3" border="0" align="center">
						<tr>
							<td align="center" valign="middle">
							<b>LIMITES ORÇAMENTÁRIOS</b>
							</td>
						</tr>
						<tr>
						<tr>
							<td align="center" valign="middle">
							<b>ATIVIDADES - PROJETOS E OPERAÇÕES ESPECIAIS</b>
							</td>
						</tr>
						<tr>
							<td align="center" valign="middle"><b>
							<font color="black" size="2">MINISTÉRIO DA EDUCAÇÃO</font></b>
								</td>
						</tr>
						</TABLE>


			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1"  align="center">
				
				
				
				
				<tr bgcolor="#DCDCDC">
					<td align="center" width="55%" rowspan="2" valign="middle"><b>DISCRIMINAÇÃO</b></td>
					<td colspan="3" width="45%" align="center"><b>LIMITES</b></td>
				</tr>
				<tr bgcolor="#DCDCDC">
					<td align="center" width="15%"><b>TESOURO<br>(a)</b></td>
					<td align="center" width="15%"><b>PRÓPRIOS<br>(b)</b></td>
					<td align="center" width="15%"><b>TOTAL<br>(c=a+b)</b></td>
				</tr>
				<?  

	$Query ="select codGrupo, descGrupo, codigo, descricao, ".
			"		SUM ( coalesce ( valorTesouro, 0) ) as valorTesouro, SUM ( coalesce ( valorOutros, 0) ) as valorOutros ".
			"	from ".
			"	( ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, gm.gpmordem, ".
			"			Case m.mtrmatriz ".
			"				when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) ".
			"						else SUM ( coalesce ( lf.lmfvalor, 0) ) ".
			"				end as valorTesouro, ".
			"			0 as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte lf ON ".
			"		lf.mtmid = mm.mtmid and (lf.foncod like'1%' or lf.foncod like'3%') ".
			"   inner join elabrev.propostaorcamento po on po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			//"	where mm.ppoid = ".$ppoid." ".
			//"	where mm.ppostatus = 'A' and ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, m.mtrmatriz, gm.gpmordem ".
			"	union all ".
			"	select 	gm.gpmid as codGrupo, gm.gpmdsc as descGrupo, m.mtrid as codigo, m.mtrdsc as descricao, gm.gpmordem, ".
			"		0 as valorTesouro, 			".
			"			Case m.mtrmatriz ".
			"				when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) ".
			"						else SUM ( coalesce ( lf.lmfvalor, 0) ) ".
			"				end as valorOutros ".
			"	from ".
			"	elabrev.grupomatriz gm ".
			"	inner join elabrev.matriz m ON ".
			"		m.gpmid = gm.gpmid ".
			"	inner join elabrev.montantematriz mm ON ".
			"		mm.mtrid = m.mtrid ".
			"	left join elabrev.limitefonte lf ON ".
			"		lf.mtmid = mm.mtmid and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') ".
			"   inner join elabrev.propostaorcamento po on po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			//"	where mm.ppoid = ".$ppoid." ".
			//"	where mm.ppostatus = 'A' and ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
			"	group by gm.gpmid, gm.gpmdsc, m.mtrid, m.mtrdsc, m.mtrmatriz, gm.gpmordem ".
			"	) a ".
			"	group by ".
			"	codGrupo, descGrupo, codigo, descricao, gpmordem having ( SUM ( coalesce ( valorTesouro, 0) ) > 0 or SUM ( coalesce ( valorOutros, 0) ) > 0 ) ".
			"	order by ".
			"	gpmordem, descGrupo, descricao ";
		$RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
	
		  	$grupo = 0;
		  	$grupoNovo = 0;
			for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
			$grupoNovo = $res[0];
		  	if ($i == 0)
		  	{
			  	$grupo = $res[0];
			  	$grupoNovo = $res[0];
			  	$a = ExecutaQuery ($res[0]);
?>
	<tr bgcolor="#DCDCDC"><td ><b><?=$res[1]?></b></td><td align="right"><b><?=number_format($a[2], 0, ',', '.')?></b></td><td align="right"><b><?=number_format($a[3], 0, ',', '.')?></b></td><td align="right"><b><?=number_format( ( $a[2]+$a[3] ) , 0, ',', '.')?></b></td></tr>	
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
<?		  	}

		  	if ($grupoNovo == $grupo)
		  	{
			  	$grupo = $grupoNovo;
			}
			else 
		  	{
			  	$b = ExecutaQuery ($res[0]);
?>
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
	<tr bgcolor="#DCDCDC"><td><b><?=$res[1]?></b></td><td align="right"><b><?=number_format($b[2], 0, ',', '.')?></b></td><td align="right"><b><?=number_format($b[3], 0, ',', '.')?></b></td><td align="right"><b><?=number_format( ( $b[2]+$b[3] ) , 0, ',', '.')?></b></td></tr>	
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
<?		  	

				$grupo = $res[0];
			}

?>

<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';"><td class="SemBordaCimaBaixo">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$res[3]?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format($res[4], 0, ',', '.')?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format($res[5], 0, ',', '.')?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format( ( $res[4]+$res[5] ) , 0, ',', '.')?></td> </tr>						

<?
			}
			  	$c = ExecutaQuery (0);
?>
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
	<tr bgcolor="#DCDCDC"><td align="center"><b>TOTAL</b></td><td align="right"><b><?=number_format($c[0], 0, ',', '.')?></b></td><td align="right"><b><?=number_format($c[1], 0, ',', '.')?></b></td><td align="right"><b><?=number_format( ($c[0]+$c[1]) , 0, ',', '.')?></b></td></tr>	

<?
		}
?>
	</table>
<br>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1" align="center">	
		<tr bgcolor="#DCDCDC">
			<td colspan="2" width="55%" align="center"><b>FONTES</b></td>
			<td align="center" width="15%"><b>LIMITE</b></td>
			<td align="center" width="15%"><b>DESPESAS PROGRAMADAS</b></td>
			<td align="center" width="15%"><b>SALDO A PROGRAMAR</b></td>
		</tr>		
<?
		$Query = "select  ".
				 "			codigo, ".
				 "			descricao, ".
				 "			SUM ( coalesce ( vlLimite, 0) ) as vlLimite, ".
				 "			SUM ( coalesce ( vlDespesa, 0) ) as vlLimite ".
				 "		from ".
				 "		( SELECT 	 ".
				 "	 		fr.foncod as Codigo, ".
				 "			fr.fondsc as Descricao, ".
				 "			SUM ( coalesce ( lf.lmfvalor, 0) ) as vlLimite, ".
				 "			0 as vlDespesa ".
				 "		FROM elabrev.limitefonte_calc2 lf ".
				 "			inner join unidade u ON ".
				 "				u.unicod = lf.unicod and u.unitpocod = lf.unitpocod ".
				 "			inner join elabrev.montantematriz mm ON ".
				 "				mm.mtmid = lf.mtmid ".
				 "			inner join elabrev.matriz m ON ".
				 "				m.mtrid = mm.mtrid ".
				 "			inner join elabrev.propostaorcamento po ON ".
				 "				po.ppoid = lf.ppoid and po.ppoid = mm.ppoid and po.ppostatus = 'A' and po.ppoanoexercicio = '" . $_SESSION['exercicio'] . "'".
				 "			inner join public.fonterecurso fr ON ".
				 "				fr.foncod = lf.foncod ".
				 "		group by fr.foncod, fr.fondsc ".
				 "		union all ".
				 "		select  ".
				 "			fr2.foncod, ".
				 "			fr2.fondsc, ".
				 "			0 as vlLimite, ".
				 "			SUM(coalesce (da.dpavalor,0)) as vlDespesa ".
				 "		from ".
				 "				elabrev.despesaacao da ".
				 "			inner join elabrev.ppaacao_orcamento ac ON ".
				 "				ac.acaid = da.acaid ".
				 "			inner join  public.fonterecurso fr2 ON ".
				 "				fr2.foncod = da.foncod ".
				 "			group by ".
				 "				fr2.foncod, fr2.fondsc ) as foo ".
				 "		group by ".
				 "			codigo, ".
				 "			descricao order by codigo ";
		$RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
?>
		<tr>
			<td align="center" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="left" width="40%" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
			<td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
		</tr>

<?
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;
		
		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
		  	$vlDespesa = $vlDespesa + $res[3];
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="center" class="SemBordaCimaBaixo"><?=$res[0]?></td>
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[2], 0, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[3], 0, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format(($res[2]-$res[3]), 0, ',', '.')?></td>
		</tr>
<?
			}
?>
		<tr>
			<td align="center" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="left" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
			<td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td colspan="2" align="center"><b>TOTAL</b></td>
			<td align="right"><b><?=number_format($vlTotal, 0, ',', '.')?></b></td>
			<td align="right"><b><?=number_format($vlDespesa, 0, ',', '.')?></b></td>
			<td align="right"><b><?=number_format(($vlTotal-$vlDespesa), 0, ',', '.')?></b></td>
		</tr>
<?
		}
?>
		<tr bgcolor="#DCDCDC">
			<td colspan="5" align="center"><INPUT TYPE="button" NAME="Voltar" VALUE="Voltar" class="notprint" onclick="history.back();"></td>
		</tr>
	</table>
<?
	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: mostraSaldosFontesUnidade.inc
		
	*/

	$unidade = $_REQUEST['unidade'];
	$NomeUnidade =$db->pegaUm( "select unicod || '  ' || unidsc from unidade where unicod = '".$unidade."'");
	$acao = $_REQUEST['acao'];
	$ppoid =$db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '".$_SESSION['exercicio']."'");
	if (!$ppoid)
	{
?>
		<script type="text/javascript">
			alert('Não foi encontrado a proposta ativa para o Exercício de <?=$_SESSION['exercicio']?> !');
			self.close();
		</script>
<?
	}
?>

<script type="text/javascript">
	window.focus();
</script>

<form method="POST" name="formulario">
<input type="hidden" name="ppoid" value="<?=$ppoid?>">
<input type="hidden" name="unidade" value="<?=$unidade?>">
<input type="hidden" name="acao" value="">
	<table cellSpacing="0" width="95%" cellPadding="2" border="0" align="center">	
		<tr>
		<td align="left" class="TituloTela" style="color:#000000;">Unidade: <?=$NomeUnidade?></td>
		</tr>
	</table>
<br>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1" align="center">
		<tr bgcolor="#DCDCDC">
			<td colspan="2" align="center"><b>FONTES</b></td>
			<td align="center"><b>Limite</b></td>
			<td align="center"><b>Despesa</b></td>
			<td align="center"><b>Despesa Adicional</b></td>
			<td align="center"><b>Saldo</b></td>
		</tr>
<?
                /**
                 * Query replicada em <despesadetalhamentosub.inc>
                 * @see despesadetalhamentosub.inc
                 */
		$Query = <<<DML
SELECT codigo,
       descricao,
       SUM(COALESCE(vlLimite, 0)) AS vlLimite,
       SUM(COALESCE(vlDespesa, 0)) AS vlDespesa,
       SUM(COALESCE(vlDespesa_adicional, 0)) AS vlDespesa_adicional
  FROM (SELECT fr.foncod AS Codigo,
               fr.fondsc AS Descricao,
               SUM(COALESCE(lf.lmfvalor, 0)) AS vlLimite,
               0 AS vlDespesa,
               0 AS vlDespesa_adicional
          FROM elabrev.limitefonte_calc2 lf
            INNER JOIN unidade u ON u.unicod = lf.unicod
              AND u.unitpocod = lf.unitpocod
            INNER JOIN elabrev.montantematriz mm ON mm.mtmid = lf.mtmid
            INNER JOIN elabrev.matriz m ON m.mtrid = mm.mtrid
            INNER JOIN elabrev.propostaorcamento po ON po.ppoid = mm.ppoid and po.ppostatus = 'A'
              AND po.ppoanoexercicio = '{$_SESSION['exercicio']}'
            INNER JOIN public.fonterecurso fr ON fr.foncod = lf.foncod
          WHERE po.ppoid = {$ppoid} AND lf.ppoid = {$ppoid} AND u.unicod = '{$unidade}' 
          GROUP BY fr.foncod, fr.fondsc
        UNION ALL
        SELECT fr2.foncod,
               fr2.fondsc,
               0 as vlLimite,
               SUM(coalesce(da.dpavalor,0)) AS vlDespesa,
               SUM(coalesce(da.dpavalorexpansao,0)) as vlDespesa_adicional
          from elabrev.despesaacao da 
            INNER JOIN elabrev.ppaacao_orcamento ac ON ac.acaid = da.acaid 
            INNER JOIN public.fonterecurso fr2 ON fr2.foncod = da.foncod
          WHERE ac.unicod = '{$unidade}' AND da.ppoid = {$ppoid}
          GROUP BY fr2.foncod, fr2.fondsc) AS foo 
  GROUP BY codigo, descricao
  ORDER BY codigo
DML;
//          ver($Query);
		$RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
?>
		<tr>
			<td align="center" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="left" width="40%" class="SemBordaCimaBaixoDireita">&nbsp;&nbsp;</td>
			<td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" width="10%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" width="10%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
			<td align="right" width="15%" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
		</tr>

<?
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;
		  	$vlDespesa_adicional = 0;
		
		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
		  	$vlDespesa = $vlDespesa + $res[3];
		  	$vlDespesa_adicional = $vlDespesa_adicional + $res[4];
		  	
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="center" class="SemBordaCimaBaixo"><?=$res[0]?></td>
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[2], 0, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[3], 0, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[4], 0, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format(($res[2]-($res[3]+$res[4])), 0, ',', '.')?></td>
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
			<td align="right" class="SemBordaCimaBaixo">&nbsp;&nbsp;</td>
		</tr>
		<tr bgcolor="#DCDCDC">
			<td colspan="2" align="center"><b>TOTAL</b></td>
			<td align="right"><b><?=number_format($vlTotal, 0, ',', '.')?></b></td>
			<td align="right"><b><?=number_format($vlDespesa, 0, ',', '.')?></b></td>
			<td align="right"><b><?=number_format($vlDespesa_adicional, 0, ',', '.')?></b></td>
			<td align="right"><b><?=number_format(($vlTotal-($vlDespesa + $vlDespesa_adicional)), 0, ',', '.')?></b></td>
		</tr>
<?
		}
?>
		<tr bgcolor="#DCDCDC">
			<td colspan="6" align="center"><input type="button" name="voltar" value="Fechar" class="notprint" onclick="self.close();"></td>
		</tr>
	</table>
	<table cellSpacing="0" width="95%" cellPadding="2" border="0" align="center">	
		<tr>
		<td align="right" style="color:#000000;">Emitido em <?=date("d/m/Y - H:i:s")?></td>
		</tr>
	</table>

</form>


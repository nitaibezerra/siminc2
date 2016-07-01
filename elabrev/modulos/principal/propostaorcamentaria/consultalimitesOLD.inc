<script>


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



function ExecutaQuery ( $grupo , $ppoid , $unicod)
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
			"		lf.mtmid = mm.mtmid and lf.unicod = ".$unicod." and (lf.foncod like'1%' or lf.foncod like'3%') ".
			"	where mm.ppoid = ".$ppoid." ".
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
			"		lf.mtmid = mm.mtmid and lf.unicod = ".$unicod." and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') ".
			"	where mm.ppoid = ".$ppoid." ".
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
$ppoid = $_REQUEST['ppoid'];
$unicod = $_REQUEST['unicod'];
$unidsc = $_REQUEST['unidsc'];
$acao = $_REQUEST['act'];

if ($_SESSION['unicod_visualizacao'] && !$db->usuarioPossuiPermissaoTodasUnidades())
{
	$acao = 'consultar';
	$unidade = $_SESSION['unicod_visualizacao'];
	$ppoid =$db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and ppoanoexercicio = '".$_SESSION['exercicio']."'");
}
else
	$unidade = $_REQUEST['unidade'];

include APPRAIZ."includes/cabecalho.inc";
?>
<script type="text/javascript" src="/includes/livesearch.js"></script>
<br>
<?
$db->cria_aba($abacod_tela,$url,$parametros);
if ($_REQUEST['acao']== 'A') $titulo_modulo='Consultar Propostas Orçamentárias';
if ($_REQUEST['acao']== 'C') $titulo_modulo='Consultar Propostas Orçamentárias';



	$sql="Select count(ppostatus) as codigo from elabrev.propostaorcamento where ppoanoexercicio = '".$_SESSION['exercicio']."'";

	
	$qtdPropostas =(integer)$db->pegaUm( $sql );
	
?>


<form method="POST" name="formulario" style="margin:0px">

<?
	if ($db->usuarioPossuiPermissaoTodasUnidades() && $qtdPropostas <> 1) 
	{
?>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value="">
<input type=hidden name="unidade" value="">

<?
	if ($unidade == '') { 
	 	monta_titulo($titulo_modulo,'');
?>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
<?
	$sql="Select ppoid as codigo, ppoano || ' - ' || ppodsc as descricao from elabrev.propostaorcamento where ppoanoexercicio = '".$_SESSION['exercicio']."' order by ppoano || ' - ' || ppodsc";

?>
	<td align='right' width="40%" class="SubTituloDireita">Proposta :</td>
    <td><? $db->monta_combo('ppoid', $sql, 'S', '<-- Escolha a proposta -->',"SubmeterConsulta", '' );?>
    </td>
  </tr>

<?	
	if ($ppoid != '') 
	{
	$sql3="Select distinct u.unicod as valor, u.unicod || ' - ' || u.unidsc as descricao from unidade u inner join elabrev.unidadematriz um ON um.unicod = u.unicod inner join elabrev.matriz m ON m.mtrid = um.mtrid inner join elabrev.montantematriz mm ON mm.mtrid = m.mtrid inner join elabrev.propostaorcamento po ON po.ppoid = mm.ppoid where u.unicod like '%s%' and po.ppoid = ".$ppoid." order by u.unicod ";
?>
  <tr>
    <td align='right' width="40%" class="SubTituloDireita">Código da UO (ou parte do código):</td>
    <td>	<?= campo_texto_ajax( $sql3, "unicod", "unicod", "Digite o código da unidade", '', 15, '', '', 'left'); ?></td>
    
  </tr>
<?
	$sql4="Select distinct u.unidsc as valor, u.unidsc as descricao from unidade u inner join elabrev.unidadematriz um ON um.unicod = u.unicod inner join elabrev.matriz m ON m.mtrid = um.mtrid inner join elabrev.montantematriz mm ON mm.mtrid = m.mtrid where upper(u.unidsc) like upper('%s%') and mm.ppoid = ".$ppoid." order by u.unidsc ";
?>
  <tr>
    <td align='right' width="40%" class="SubTituloDireita">Descrição da UO (ou parte da descrição):</td>
    <td>	<?= campo_texto_ajax( $sql4, "unidsc", "unidsc", "Digite a descrição da unidade", '', 75, '', '', 'left'); ?>
    </td>
  </tr>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td><input type='button' class="botao" name='consultar' value='Consultar' onclick="JavaScript:SubmeterConsulta();"></td>
  </tr>
<?
	}
	else
	{
?>
  <tr bgcolor="#C0C0C0">
    <td ></td>
    <td>&nbsp;&nbsp;</td>
  </tr>
<?
	}
?>

</table>

<?	
	
	}
	
	}
	else
	{
	$sql = "select distinct count(unicod) from elabrev.usuarioresponsabilidade inner join unidade using ( unicod ) where usucpf = '" . $_SESSION['usucpf'] . "' and unistatus = 'A' and orgcod = '26000' and unicod != '26100' and rpustatus = 'A' ";
	$qtdunidades =(integer)$db->pegaUm( $sql );
	$ppoid =$db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and ppoanoexercicio = '".$_SESSION['exercicio']."'");

	if (!$_REQUEST['act']) $acao = 'listar';
dbg($qtdunidades);
dbg($qtdPropostas);
	if ($qtdunidades != 1 && $qtdPropostas != 1)
	{
		$acao = 'listar';
	}
	elseif ($qtdunidades != 1 && $qtdPropostas == 1)
	{
		$unidade =$_REQUEST['unidade'];
		$acao = 'consultar';
	}
	elseif ($qtdunidades == 1 && $qtdPropostas == 1)
	{
	$sql = "select distinct unicod from elabrev.usuarioresponsabilidade inner join unidade using ( unicod ) where usucpf = '" . $_SESSION['usucpf'] . "' and unistatus = 'A' and orgcod = '26000' and unicod != '26100' ";
	$unidade =$db->pegaUm($sql);
//	$unidade = $db->usuarioUnidadesPermitidas();
	$acao = 'consultar';
	}
?>
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="act" value="">
<input type=hidden name="ppoid" value="<?=$ppoid?>">
<input type=hidden name="unidade" value="<?=$unidade?>">
<?
	}
	
?>
</form>

<?
if ($acao == 'listar')	
{
	unset($nlinhas);
	unset($sql);
	
	$cabecalho = array('Ações','Código','Descrição','Total');
	$texto = "";
	if ($_REQUEST['unicod']) $texto = " and u.unicod like '%".$_REQUEST['unicod']."%' ";
	if ($_REQUEST['unidsc']) 
		if ($texto == '') 
			$texto.= " and upper(u.unidsc) like upper('%".strtoupper($_REQUEST['unidsc'])."%')";
		else
			$texto.= " or upper(u.unidsc) like upper('%".strtoupper($_REQUEST['unidsc'])."%')";

	$texto.= " and mm.ppoid = ".$ppoid." ";
	
	$unicods = implode( "','", $db->usuarioUnidadesPermitidas() );
	if ( strlen( $unicods ) > 0 )
	{
		$unicods = "'" . $unicods . "'";
	}
	else
	{
		$unicods = 'null';
	}

		$sql= "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Proposta \" onclick=\"Consultar(' || A.unicod || ')\">', A.unicod, A.unidsc, to_char(SUM ( coalesce ( a.valor, 0) ), '99999999990.99') as valor from (select u.unicod, u.unidsc, Case m.mtrmatriz when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) else SUM ( coalesce ( lf.lmfvalor, 0) ) end as valor from unidade u left join elabrev.limitefonte lf ON lf.unicod = u.unicod left join elabrev.montantematriz mm ON mm.mtmid = lf.mtmid inner join elabrev.matriz m ON m.mtrid = mm.mtrid inner join elabrev.grupomatriz gm ON gm.gpmid = m.gpmid where u.unicod in (".$unicods.")  $texto group by u.unicod, u.unidsc, m.mtrmatriz) as A group by a.unicod, a.unidsc ";

		$db->monta_lista($sql,$cabecalho,100,20,'','','');
	
}

 
 
	if ($acao == 'consultar' )
	{

?>

						<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="0"  cellPadding="3" border="0" align="center">
						<tr>
							<td class="TituloTela" style="color:#000000;" align="center" valign="middle">
							<b>Consultar Propostas Orçamentárias</b>
							</td>
						</tr>
						<tr >
							<td align="center" valign="middle"><b>
							<?	$NomeProposta=$db->pegaUm( "select ppodsc || ' - ' || ppoano as descricao from elabrev.propostaorcamento where ppoid = ".$ppoid.""); 
								print str_to_upper($NomeProposta);
							?></b>
								</td>
						</tr>
						<tr>
							<td align="center" valign="middle">
							<b>LIMITES ORÇAMENTÁRIOS</b>
							</td>
						</tr>
						<tr>
							<td align="center" valign="middle">
							<b>ATIVIDADES - PROJETOS E OPERAÇÕES ESPECIAIS</b>
							</td>
						</tr>
						<tr>
							<td align="center" valign="middle"><b>
							<font color="black" size="2">
							<?	
							$NomeUnidade=$db->pegaUm( "select unidsc as descricao from unidade where unicod = ".$unidade.""); 
								print str_to_upper($unidade.' - '.$NomeUnidade);
							?></font></b>
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
			"		lf.mtmid = mm.mtmid and lf.unicod = ".$unidade." and (lf.foncod like'1%' or lf.foncod like'3%') ".
			"	where mm.ppoid = ".$ppoid." ".
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
			"		lf.mtmid = mm.mtmid and lf.unicod = ".$unidade." and (lf.foncod like'2%' or lf.foncod like'6%' or lf.foncod like'9%') ".
			"	where mm.ppoid = ".$ppoid." ".
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
			  	$a = ExecutaQuery ($res[0],$ppoid,$unidade);
?>
	<tr bgcolor="#DCDCDC"><td ><b><?=$res[1]?></b></td><td align="right"><b><?=number_format($a[2], 2, ',', '.')?></b></td><td align="right"><b><?=number_format($a[3], 2, ',', '.')?></b></td><td align="right"><b><?=number_format( ( $a[2]+$a[3] ) , 2, ',', '.')?></b></td></tr>	
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
<?		  	}

		  	if ($grupoNovo == $grupo)
		  	{
			  	$grupo = $grupoNovo;
			}
			else 
		  	{
			  	$b = ExecutaQuery ($res[0],$ppoid,$unidade);
?>
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
	<tr bgcolor="#DCDCDC"><td><b><?=$res[1]?></b></td><td align="right"><b><?=number_format($b[2], 2, ',', '.')?></b></td><td align="right"><b><?=number_format($b[3], 2, ',', '.')?></b></td><td align="right"><b><?=number_format( ( $b[2]+$b[3] ) , 2, ',', '.')?></b></td></tr>	
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
<?		  	

				$grupo = $res[0];
			}

?>

<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';"><td class="SemBordaCimaBaixo">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<?=$res[3]?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format($res[4], 2, ',', '.')?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format($res[5], 2, ',', '.')?></td><td class="SemBordaCimaBaixo" align="right"><?=number_format( ( $res[4]+$res[5] ) , 2, ',', '.')?></td> </tr>						

<?
			}
			  	$c = ExecutaQuery (0,$ppoid,$unidade);
?>
	<tr><td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td> <td class="SemBordaCimaBaixo">&nbsp;&nbsp;</td></tr>
	<tr bgcolor="#DCDCDC"><td align="center"><b>TOTAL</b></td><td align="right"><b><?=number_format($c[0], 2, ',', '.')?></b></td><td align="right"><b><?=number_format($c[1], 2, ',', '.')?></b></td><td align="right"><b><?=number_format( ($c[0]+$c[1]) , 2, ',', '.')?></b></td></tr>	

<?
		}
?>
	</table>
<br>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="2" border="1" align="center">	
		<tr bgcolor="#DCDCDC">
			<td colspan="2" width="55%" align="center"><b>FONTES</b></td>
			<td align="center" width="15%"><b>Limite</b></td>
			<td align="center" width="15%"><b>Despesa</b></td>
			<td align="center" width="15%"><b>Saldo</b></td>
		</tr>		
<?
		$Query = "Select A.codigo, A.Descricao, SUM( vlLimite ) as vlLimite ,  SUM( vlDespesa ) as vlDespesa from (SELECT 	fr.foncod as Codigo, ".
				 "			fr.fondsc as Descricao, ".
				 "			Case m.mtrmatriz ".
				 "				when 't' then SUM ( coalesce ( (lf.lmfvalor*mm.mtmvlrlimite)/100, 0) ) ".
				 "					else SUM ( coalesce ( lf.lmfvalor, 0) ) end as vlLimite, ".
				 "			coalesce (A.vlDespesa,0) as vlDespesa ".
				 "		FROM elabrev.limitefonte lf ".
				 "		inner join unidade u ON ".
				 "			u.unicod = lf.unicod and u.unitpocod = lf.unitpocod ".
				 "		inner join elabrev.montantematriz mm ON ".
				 "			mm.mtmid = lf.mtmid ".
				 "		inner join elabrev.matriz m ON ".
				 "			m.mtrid = mm.mtrid ".
				 "		inner join elabrev.propostaorcamento po ON ".
				 "			po.ppoid = mm.ppoid ".
				 "		inner join public.fonterecurso fr ON ".
				 "			fr.foncod = lf.foncod ".
				 "		left join ".
				 "			(select ".
				 "				fr2.foncod, ".
				 "				SUM( coalesce (da.dpavalor,0) ) as vlDespesa ".
				 "			from ".
				 "			elabrev.despesaacao da ".
				 "			inner join monitora.acao ac ON ".
				 "				ac.acaid = da.acaid ".
				 "			inner join public.fonterecurso fr2 ON ".
				 "				fr2.foncod = da.foncod ".
				 "				where ac.unicod = ".$unidade." ".
				 "			group by ".
				 "				fr2.foncod, ".
				 "				fr2.fondsc) A ON ".
				 "			A.foncod = lf.foncod ".
				 "		WHERE po.ppoid = ".$ppoid." and u.unicod = ".$unidade." ".
				 "		group by fr.foncod, fr.fondsc, m.mtrmatriz, A.vlDespesa order by fr.foncod) as A group by a.codigo, a.descricao order by a.codigo ";
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
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[2], 2, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format($res[3], 2, ',', '.')?></td>
			<td align="right" class="SemBordaCimaBaixo"><?=number_format(($res[2]-$res[3]), 2, ',', '.')?></td>
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
			<td align="right"><b><?=number_format($vlTotal, 2, ',', '.')?></b></td>
			<td align="right"><b><?=number_format($vlDespesa, 2, ',', '.')?></b></td>
			<td align="right"><b><?=number_format(($vlTotal-$vlDespesa), 2, ',', '.')?></b></td>
		</tr>
<?
		}
?>
		<tr bgcolor="#DCDCDC">
			<td colspan="5" align="center"><INPUT TYPE="button" NAME="Voltar" VALUE="Voltar" class="notprint" onclick="history.back();"></td>
		</tr>
	</table>
<?	
//$_SESSION['unicod_visualizacao'] = $unidade;
	}?>  




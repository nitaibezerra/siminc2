<?php
/**
 * Sistema Simec
 * Setor responsável: SPO-MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
 * Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
 * Módulo: relatorioQuadroCreditos.inc
 * Finalidade: relatório dos quadros orçamentarios, detalha os gastos da unidade por ação
 */

if ('planilha_lista' == $_REQUEST['relat']) {
    unset($_REQUEST['relat']);
    if ('4' == $_REQUEST['planilha']) {
        header('Content-type: application/xls');
        header('Content-Disposition: attachment; filename="planilha_simec.xls"');
    } else if ('6' == $_REQUEST['planilha']) {
        header('Content-type: application/csv');
        header('Content-Disposition: attachment; filename="planilha_simec.csv"');
    }

    for ($i = 0; $i < count($_SESSION['Query']); $i++) {
        $RS = $db->record_set($_SESSION['Query'][$i]);
        $_SESSION['conteudo'] = $db->carrega_tudo($RS);
        $_SESSION['LINHA'] = $db->conta_linhas($RS);
        $_SESSION['title'] = $tit;
        $_SESSION['TITULO'] = 'Relatório Gerencial de Ação';
        $_SESSION['TIT'][0][0] = 'Código ';
        $_SESSION['TIT'][0][1] = 'Título';
        $_SESSION['data'] = date('(h:m) d/m/Y');
        $_SESSION['LIN'] = array();

        $_SESSION['cab'] = Array();
        for ($a = 0; $a < pg_num_fields($RS); $a++) {
            $_SESSION['cab'][$a] = pg_field_name($RS, $a);
        }

        for ($a = 0; $a < count($_SESSION['cab']); $a++) {
            if ($a < count($_SESSION['cab']) - 1) {
                print trim(($_SESSION['cab'][$a])) . ' ' . "\t";
            } else {
                print($_SESSION['cab'][$a]);
            }
        }

        print chr(13);

        if ($_SESSION['LINHA'] >= 0) {
            for ($a = 0; $a <= $_SESSION['LINHA']; $a++) {
                $res = $db->carrega_registro($RS,$a);
                // a linha abaixo transforma em variáveis todos os campos do array
                if (is_array($res)) {
                    foreach ($res as $k => $v) {
                        ${$k} = $v;
                    }
                }
            }
        }

        if (count($_SESSION['conteudo'][0]) > 1) {
            for ($a = 0; $a < count($_SESSION['conteudo']); $a++) {
                foreach ($_SESSION['conteudo'][$a] as $k => $v) {
                    if ($k <> 'acao') {
                        ${$k} = $v;
                        if ($v == 't') {
                            $v='1';
                        } elseif ($v=='f') {
                            $v = '0';
                        }
                        if ($v <>'') {
                            $v=$v;
                            print $v."\t";
                        }
                    }
                }
                print chr(13);
            }
        }
    }
    exit();
}

if (!isset($_POST['requisicao'])) {

    $exercicio = $_SESSION['exercicio'];
        $unidade = $_SESSION['unicod_visualizacao'];
        $NomeUnidade =$db->pegaUm( "select unicod || '  ' || unidsc from unidade where unicod = '".$unidade."'");
        $ppoid =$db->pegaUm( "select ppoid as codigo from elabrev.propostaorcamento where ppostatus = 'A' and tppid=1 and ppoanoexercicio = '{$exercicio}'");
        if (!$ppoid)
        {
    ?>
            <script>
            alert('Não foi encontrado a proposta ativa para o Exercício de <?=$exercicio?> !');
            history.back(-1);
            self.close();
            </script>
    <?
        exit;
        }

    include APPRAIZ . "includes/cabecalho.inc";

    if ( !$unidade and $_SESSION["unicod_visualizacao"] )
    {
        $unidade = $_SESSION["unicod_visualizacao"];
    }
    else if ( !$unidade and !$_SESSION["unicod_visualizacao"] )
    {
        exit(
            "<script type=\"text/javascript\">
                alert( 'Selecione a Unidade para Visualizar a Síntese das Despesas!' );
            </script>
            <script type=\"text/javascript\">
                document.location.href = \"elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A\";
            </script>"
        );
    }
} else {
    $ppoid = $_POST['dados']['ppoid'];
    $unidade = $_POST['dados']['unicod'];
    $exercicio = $_POST['dados']['exercicio'];
}
?>
<br/>
<?php
if (!isset($_POST['requisicao'])) {
    $db->cria_aba($abacod_tela, $url, '');
    monta_titulo('Relatório da Unidade', 'Quadro Síntese de Despesas da Ação');
}
?>
<form method="POST" name="formulario" style="margin: 0;">
<input type="hidden" name="ppoid" value="<?=$ppoid?>">
<input type="hidden" name="unidade" value="<?=$unidade?>">
<input type="hidden" name="acao" value="">

<input type='hidden' name="relat" >
<input type='hidden' name="planilha" >

<?php
if (isset($_POST['requisicao'])) {
    $classesAdicionais = ' table-striped table-bordered table-hover';
}
?>
	<table class="table<?php echo $classesAdicionais; ?>" cellSpacing="0" width="95%" bgcolor="#f5f5f5" cellPadding="2" border="0" align="center">
<?php
if (!isset($_POST['requisicao'])): ?>
        <tr>
            <td align="left" class="TituloTela" colspan="4" style="color:#000000;">Unidade: <?=$unidade?></td>
        </tr>
        <tr>
            <td align="left" bgcolor="#e9e9e9" colspan="4">Quadro Síntese</td>
        </tr>
<?
endif;
		if($ppoid == "") {
			$ppoid = null;
		}

		$Query = "select ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";

        $RS = $db->record_set($Query);
	    $nlinhas = $db->conta_linhas($RS);

		if ( $nlinhas < 0 ) {
			?>
			</table>
			<p align="center" style="color: #904040;">
				<br />
				Não existe detalhamento orçamentário para esta unidade.
			</p>

			<?
		} else { ?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left">&nbsp;&nbsp;</td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query1 = "select ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";
        $RS = $db->record_set($Query1);
	    $nlinhas = $db->conta_linhas($RS);

	    if ($nlinhas>=0)
		{
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
			}
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita">Total</td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($vlTotal), 0, ',', '.')?></td>
		</tr>
<?
		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Função</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query2 = "select ".
				 "		f.funcod, ".
				 "		f.fundsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppafuncao f ON ".
				 "		f.funcod = a.funcod ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		f.funcod, ".
				 "		f.fundsc ".
				 "	order by ".
				 "		f.funcod, ".
				 "		f.fundsc ";

        $RS = $db->record_set($Query2);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{
		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);

?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>SubFunção</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query3 = "select ".
				 "		f.sfucod, ".
				 "		f.sfudsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppasubfuncao f ON ".
				 "		f.sfucod = a.sfucod ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		f.sfucod, ".
				 "		f.sfudsc ".
				 "	order by ".
				 "		f.sfucod, ".
				 "		f.sfudsc ";

        $RS = $db->record_set($Query3);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Programa</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query4 = "select ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "		ppa.prgid = a.prgid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ".
				 "	order by ".
				 "		ppa.prgcod, ".
				 "		ppa.prgdsc ";

        $RS = $db->record_set($Query4);
	    $nlinhas = $db->conta_linhas($RS);

	    if ($nlinhas>=0)
		{

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);

?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}

		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Ação</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query5 = "select ".
				 "		a.acacod, ".
				 "		a.acadsc, ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "	inner join elabrev.ppaacao_orcamento a ON ".
				 "		a.acaid = ac.acaid ".
				 "	inner join unidade b on ".
				 "		a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		a.acacod, ".
				 "		a.acadsc ".
				 "	order by ".
				 "		a.acacod, ".
				 "		a.acadsc ";

		$RS = $db->record_set($Query5);
	    $nlinhas = $db->conta_linhas($RS);

		if ($nlinhas>=0)
		{
		  	$grupo = 0;
		  	$grupoNovo = 0;
		  	$vlTotal = 0;
		  	$vlDespesa = 0;

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Grupo de Natureza de Despesa (GND)</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query6 = "select ".
				 "			gnd.gndcod, ".
				 "			gnd.gnddsc,  ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "		inner join elabrev.ppaacao_orcamento a ON ".
				 "			a.acaid = ac.acaid ".
				 "		inner join naturezadespesa n ON ".
				 "			n.ndpid = ac.ndpid ".
				 "		inner join gnd ON ".
				 "			gnd.gndcod = n.gndcod ".
				 "		inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "			ppa.prgid = a.prgid ".
				 "		inner join unidade b on ".
				 "			a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		gnd.gndcod, ".
				 "		gnd.gnddsc ".
				 "	order by ".
				 "		gnd.gndcod, ".
				 "		gnd.gnddsc ";

        $RS = $db->record_set($Query6);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);
		  	$vlTotal = $vlTotal + $res[2];
		  	$vlDespesa = $vlDespesa + $res[3];

?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}
?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Fonte</b></td>
			<td align="right" width="30%" colspan="2"><b>Valor</b></td>
		</tr>
<?
		$Query7 = "select ".
				 "			f.foncod, ".
				 "			f.fondsc,  ".
				 "		coalesce( SUM(ac.dpavalor + ac.dpavalorexpansao) , 0) as valor ".
				 "	from elabrev.despesaacao ac ".
				 "		inner join elabrev.ppaacao_orcamento a ON ".
				 "			a.acaid = ac.acaid ".
				 "		inner join public.fonterecurso f ON ".
				 "			f.foncod = ac.foncod ".
				 "		inner join elabrev.ppaprograma_orcamento ppa ON ".
				 "			ppa.prgid = a.prgid ".
				 "		inner join unidade b on ".
				 "			a.unicod = b.unicod ".
				 "	where ".
				 "		a.prgano = '{$exercicio}' and ".
				 "		a.acastatus='A' and ".
				 "		b.unicod = '".$unidade."'  and ".
				 "		a.acasnrap = false and " .
				 "      ac.ppoid = " . $ppoid .
				 "	group by ".
				 "		f.foncod, ".
				 "		f.fondsc ".
				 "	order by ".
				 "		f.foncod, ".
				 "		f.fondsc ";

        $RS = $db->record_set($Query7);
	    $nlinhas = $db->conta_linhas($RS);
	    if ($nlinhas>=0)
		{

		  	for ($i=0; $i<=$nlinhas;$i++)
			{
		  	$res = $db->carrega_registro($RS,$i);

?>
		<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
			<td align="left" class="SemBordaCimaBaixoDireita"><?=str_to_upper( $res[0] )?> - <?=str_to_upper( $res[1] )?></td>
			<td align="right" class="SemBordaCimaBaixo" colspan="2"><?=number_format(($res[2]), 0, ',', '.')?></td>
		</tr>
<?
			}
		}

	$Query8 = " select " .
			" 	substr(nd.ndpcod,1,1)||'.'||substr(nd.ndpcod,2,1)||'.'||substr(nd.ndpcod,3,2)||'.'||substr(nd.ndpcod,5,2)||'.'||substr(nd.ndpcod,7,2) as codigo,  " .
			" nd.ndpdsc as descricao, " .
			" coalesce( sum( dpavalor + dpavalorexpansao), 0 ) as valor " .
		" from naturezadespesa nd " .
			" left join elabrev.despesaacao da on da.ndpid = nd.ndpid and da.ppoid = " . $ppoid .
			" left join elabrev.ppaacao_orcamento ao using ( acaid ) " .
		" where ao.unicod is null or ao.unicod = '" . $unidade . "' " .
		"group by nd.ndpcod, nd.ndpdsc having 	coalesce( sum( dpavalor + dpavalorexpansao ), 0 ) > 0 order by nd.ndpcod";

	// CAPTURA DADOS DE DESPESA POR NATUREZA
	$despesa_por_natureza = $db->carregar(
		" select " .
			" nd.ndpcod as codigo, " .
			" nd.ndpdsc as descricao, " .
			" coalesce( sum( dpavalor + dpavalorexpansao), 0 ) as valor " .
		" from naturezadespesa nd " .
			" left join elabrev.despesaacao da on da.ndpid = nd.ndpid and da.ppoid = " . $ppoid .
			" left join elabrev.ppaacao_orcamento ao using ( acaid ) " .
		" where ao.unicod is null or ao.unicod = '" . $unidade . "' " .
		"group by nd.ndpcod, nd.ndpdsc order by nd.ndpcod"
	);
	$estrutura = array();
	$valor_total_natureza = 0;

	foreach ( $despesa_por_natureza as $dados_ndp )
	{
		$codigo = $dados_ndp['codigo'];
		$descricao = $dados_ndp['descricao'];
		$valor = $dados_ndp['valor'];
		$indice_0 = substr( $codigo, 0, 1 );
		$indice_1 = substr( $codigo, 1, 1 );
		$indice_2 = substr( $codigo, 2, 2 );
		$indice_3 = substr( $codigo, 4, 2 );
		$indice_4 = substr( $codigo, 6, 2 );
		$nova_estrutura = array(
			'codigo' => $codigo,			// gndcod
			'descricao' => $descricao,		// descrição da natureza
			'valor' => $valor,				// valor gasto pela natureza
			'valor_filhos' => 0,			// soma dos valores gastos pelos filhos
			'visivel' => false,				// indica se algum filho possui valor (caso nivel 4 indica se o valor da natureza em questão é maior que zero)
			'filhos' => array()				// lista com os subitens da natureza (nível 4 sempre vazio)
		);
		$valor_total_natureza += $valor;
		switch ( true )
		{
			case substr( $codigo, 1 ) == '0000000': // nivel 0
				$estrutura[$indice_0] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['visivel'] = true;
				}
				break;
			case substr( $codigo, 2 ) == '000000': // nivel 1
				$estrutura[$indice_0]['filhos'][$indice_1] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			case substr( $codigo, 4 ) == '0000': // nivel 2
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			case substr( $codigo, 6 ) == '00': // nivel 3
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
			default: // nivel 4
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['filhos'][$indice_4] = $nova_estrutura;
				if ( $valor > 0 )
				{
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['filhos'][$indice_4]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['visivel'] = true;
					$estrutura[$indice_0]['filhos'][$indice_1]['visivel'] = true;
					$estrutura[$indice_0]['visivel'] = true;
				}
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['filhos'][$indice_3]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['filhos'][$indice_2]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['filhos'][$indice_1]['valor_filhos'] += $valor;
				$estrutura[$indice_0]['valor_filhos'] += $valor;
				break;
		}
	}

	// FIM CAPTURA DADOS DE DESPESA POR NATUREZA
	// EXIBE DADOS DE DESPESA POR NATUREZA
	$cor = 1;
	if ( $estrutura )
	{
		?>
		<tr bgcolor="#DCDCDC">
			<td width="70%" align="left"><b>Natureza de Despesa</b> (Plano de Contas)</td>
			<td align="right" width="15%"><b>Distribuído</b></td>
			<td align="right" width="15%"><b>Total</b></td>
		</tr>
		<?
		foreach ( $estrutura as $dados_0 )
		{
			if ( $dados_0['visivel'] == false )
			{
				continue;
			}
			$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
			?>
			<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
				<td align="left" class="SemBordaCimaBaixoDireita">
					<?=
						substr( $dados_0['codigo'], 0, 1 ) . '.' .
						substr( $dados_0['codigo'], 1, 1 ) . '.' .
						substr( $dados_0['codigo'], 2, 2 ) . '.' .
						substr( $dados_0['codigo'], 4, 2 ) . '.' .
						substr( $dados_0['codigo'], 6, 2 )
					?> - <?= strtoupper( $dados_0['descricao'] ) ?>
				</td>
				<td align="right">&nbsp;</td>
				<td align="right">
					<?= number_format( $dados_0['valor_filhos'] , 0, ',', '.') ?>
				</td>
			</tr>
			<?
			foreach ( $dados_0['filhos'] as $dados_1 )
			{
				if ( $dados_1['visivel'] == false )
				{
					continue;
				}
				$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
				?>
				<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
					<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 15px;">
						<img src="/imagens/seta_filho.gif" align="top"/>
						<?=
							substr( $dados_1['codigo'], 0, 1 ) . '.' .
							substr( $dados_1['codigo'], 1, 1 ) . '.' .
							substr( $dados_1['codigo'], 2, 2 ) . '.' .
							substr( $dados_1['codigo'], 4, 2 ) . '.' .
							substr( $dados_1['codigo'], 6, 2 )
						?> - <?= strtoupper( $dados_1['descricao'] ) ?>
					</td>
					<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
					<td align="right" class="SemBordaCimaBaixo">
						<?= number_format( $dados_1['valor_filhos'] , 0, ',', '.') ?>
					</td>
				</tr>
				<?
				foreach ( $dados_1['filhos'] as $dados_2 )
				{
					if ( $dados_2['visivel'] == false )
					{
						continue;
					}
					$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
					?>
					<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
						<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 30px;">
							<img src="/imagens/seta_filho.gif" align="top"/>
							<?=
								substr( $dados_2['codigo'], 0, 1 ) . '.' .
								substr( $dados_2['codigo'], 1, 1 ) . '.' .
								substr( $dados_2['codigo'], 2, 2 ) . '.' .
								substr( $dados_2['codigo'], 4, 2 ) . '.' .
								substr( $dados_2['codigo'], 6, 2 )
							?>
							<font color="#333333"> - <?= $dados_2['descricao'] ?></font>
						</td>
						<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
						<td align="right" class="SemBordaCimaBaixo">
							<font color="#333333">
								<?= number_format( $dados_2['valor_filhos'] , 0, ',', '.') ?>
							</font>
						</td>
					</tr>
					<?
					foreach ( $dados_2['filhos'] as $dados_3 )
					{
						if ( $dados_3['visivel'] == false )
						{
							continue;
						}
						$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
						?>
						<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
							<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 45px;">
								<img src="/imagens/seta_filho.gif" align="top"/>
								<?=
									substr( $dados_3['codigo'], 0, 1 ) . '.' .
									substr( $dados_3['codigo'], 1, 1 ) . '.' .
									substr( $dados_3['codigo'], 2, 2 ) . '.' .
									substr( $dados_3['codigo'], 4, 2 ) . '.' .
									substr( $dados_3['codigo'], 6, 2 )
								?>
								<font color="#555555"> - <?= $dados_3['descricao'] ?></font>
							</td>
							<td align="right" class="SemBordaCimaBaixo">
								<font color="#555555">
									<?= number_format( $dados_3['valor'] , 0, ',', '.') ?>
								</font>
							</td>
							<td align="right" class="SemBordaCimaBaixo">
								<font color="#555555">
									<?= number_format( $dados_3['valor_filhos'] + $dados_3['valor'] , 0, ',', '.') ?>
								</font>
							</td>
						</tr>
						<?
						foreach ( $dados_3['filhos'] as $dados_4 )
						{
							if ( $dados_4['visivel'] == false )
							{
								continue;
							}
							$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
							?>
							<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
								<td align="left" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 60px;">
									<img src="/imagens/seta_filho.gif" align="top"/>
									<?=
										substr( $dados_4['codigo'], 0, 1 ) . '.' .
										substr( $dados_4['codigo'], 1, 1 ) . '.' .
										substr( $dados_4['codigo'], 2, 2 ) . '.' .
										substr( $dados_4['codigo'], 4, 2 ) . '.' .
										substr( $dados_4['codigo'], 6, 2 )
									?>
									<font color="#777777"> - <?= $dados_4['descricao'] ?></font>
								</td>
								<td align="right" class="SemBordaCimaBaixo">
									<font color="#777777">
										<?= number_format( $dados_4['valor'], 0, ',', '.') ?>
									</font>
								</td>
								<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
							</tr>
							<?
						}
					}
				}
			}
		}
		$cor = $cor == 'ebebeb' ? 'f5f5f5' : 'ebebeb';
		?>
		<tr bgcolor="#<?= $cor ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='#<?= $cor ?>';">
			<td align="right" class="SemBordaCimaBaixoDireita" style="padding: 0 0 0 60px;">
				<font color="#666666">Total Geral</font>
			</td>
			<td align="right" class="SemBordaCimaBaixo">
				<?= number_format( $valor_total_natureza, 0, ',', '.' ) ?>
			</td>
			<td align="right" class="SemBordaCimaBaixo">&nbsp;</td>
		</tr>
		<?
	}
	// FIM EXIBE DADOS DE DESPESA POR NATUREZA
?>
	</table>
<?
}

// -- Armazenando queries para emissão de relatório XLS
$_SESSION['Query'] = array(
    $Query1,
    $Query2,
    $Query3,
    $Query4,
    $Query5,
    $Query6,
    $Query7,
    $Query8
);

if (!isset($_POST['requisicao'])) {
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>'.$titul. '</b></td><td>';
    if ($_SESSION['LINHA'] >=0)
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar XLS" align="left" onclick="imprime_rel(4)">';
      print '&nbsp;&nbsp;<input type="button" class="botao" name="btcsv" value ="Gerar CSV" align="left" onclick="imprime_rel(6)">';
    print '</td></tr></table>';
?>
<script>
function imprime_rel(cod)
{
    <?
       $browser = get_browser();
       $saiu = false;
       if (is_array($browser)){
       foreach ($browser as $name => $value) {
           if ($value == 'IE') {
           $saiu = true;
          }
       }}
       if ($saiu){?>
          if (cod == 1) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          };
          if (cod== 2) {
           location.href = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
          }
          if (cod>2) {
          	if (cod ==3) document.formulario.relat.value='planilha';
          	if (cod ==4) document.formulario.relat.value='planilha_lista';
          	if (cod ==5) document.formulario.relat.value='planilha';
          	if (cod ==6) document.formulario.relat.value='planilha_lista';
          	document.formulario.planilha.value=cod;
                document.formulario.submit();
            }
       <?} else { ?>
          if (cod == 1) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_geral1&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
        }
          if (cod== 2) {
           e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_gerenaca&acao=R";
          window.open(e, "relatorio","menubar=no,location=no,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'");
            }
          if (cod>2) {
          	if (cod ==3) document.formulario.relat.value='planilha';
          	if (cod ==4) document.formulario.relat.value='planilha_lista';
          	if (cod ==5) document.formulario.relat.value='planilha';
          	if (cod ==6) document.formulario.relat.value='planilha_lista';
          	document.formulario.planilha.value=cod;
                document.formulario.submit();
            }


      <? } ?>
}
</script>

</form>
<?php
}

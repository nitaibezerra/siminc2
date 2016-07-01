<?

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com)
		Módulo: unidaderesponsabilidade.inc
		Finalidade: Lista as unidades orçamentárias relacionadas ao usuário
	*/

	$unicods = implode( "','", $db->usuarioUnidadesPermitidas() );
	if ( strlen( $unicods ) > 0 )
	{
		$unicods = "'" . $unicods . "'";
	}
	else
	{
		$unicods = 'null';
	}

	//$sql_matrizes = "select '<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Coluna\" onclick=\"altera_coluna('||mtrid||')\">&nbsp;&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Coluna\" onclick=\"excluir_coluna('||mtrid||')\">' as acao, mtrdsc, mtrsigla, mtrano from elabrev.matriz ";
	
	$sql_unidade =
		" select a.codigo, a.descricao, replace( to_char( SUM ( a.limite ), '999,999,999,999' ), ',','.' )  as valor from ( " .
			"select " .
				" u.unicod as codigo, " .
				" '<a href=\"?modulo=principal/unidade&acao=A&unicod=' || u.unicod || '\">' || u.unidsc || '</a>' as descricao, " .
				" Case m.mtrmatriz " .
					" when 't' then SUM ( coalesce ( ( l.lmfvalor * mm.mtmvlrlimite ) / 100, 0 ) ) " .
					" else SUM ( coalesce ( l.lmfvalor, 0) ) " .
				" end as limite " .
			" from unidade u " .
				" left join elabrev.limitefonte l on l.unicod = u.unicod and l.unitpocod = u.unitpocod " .
				" inner join elabrev.montantematriz mm using ( mtmid ) " .
				" inner join elabrev.matriz m using ( mtrid ) " .
				" inner join elabrev.propostaorcamento p using ( ppoid ) " .
			" where " .
				" u.unicod in ( " . $unicods . " ) and " .
				" u.unistatus = 'A' and " .
				" p.ppostatus = 'A' " .
			" group by u.unicod, u.unidsc, m.mtrmatriz " .
			"  ) as a " .
		" group by codigo, descricao";

		include APPRAIZ."includes/cabecalho.inc"; 
?>
<br/>
<?if ($_REQUEST['ordemlista']=='') $_REQUEST['ordemlista']='1'?>
<? monta_titulo( $titulo_modulo, 'Clique na Unidade para ver detalhes.' ); ?>
<? $db->monta_lista( $sql_unidade, array( 'Código', 'Lista das Unidades', 'Limite' ), 200, 20, '', '', '' ); ?>
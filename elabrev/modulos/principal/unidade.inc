<?php

	/*
		Sistema Simec
		Setor responsável: SPO-MEC
		Desenvolvedor: Equipe Consultores Simec
		Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
		Programador: Henrique Xavier Couto (e-mail: henriquexcouto@gmail.com), Renan de Lima Barbosa (e-mail: renandelima@gmail.com), Fabrício Mendonça (e-mail: fabriciomendonca@gmail.com)
		Módulo: unidade.inc
		Finalidade: visualização dos dados de uma unidade
	*/
	// funções da página
	/**
	 * Captura os dados da unidade.
	 *
	 * @param string $unicod
	 * @return string[]
	 */
	function pegar_dados_unidade( $unicod )
	{
		global $db;
		if ( !$unicod )
		{
			return false;
		}
			$sql =
					"	select 	a.unicod, ".
					"			u.unitpocod, ".
					"			u.unidsc, ".
					"			u.uniano, ".
					"			o.orgdsc, ".
					" 			a.prgano, count(*) as numacoes, " .
					" 			coalesce( b.total, 0 ) as limite, " .
					" 			coalesce( c.despesa, 0 ) as despesa, " .
					" 			coalesce( c.saldo, 0 ) as saldo " .
				" from elabrev.ppaacao_orcamento a " .
					" inner join unidade u on a.unicod = u.unicod " .
					" inner join orgao o on o.orgcod = u.orgcod ".
					" left join ( " .
						" select " .
							" u.unicod, " .
							" sum( coalesce( l.lmfvalor, 0 ) ) as total " .
						" from elabrev.limitefonte_calc2 l  " .
							" inner join unidade u ON l.unicod = u.unicod and l.unitpocod = u.unitpocod " .
							" inner join elabrev.montantematriz mm using ( mtmid ) " .
							" inner join elabrev.propostaorcamento po on po.ppoid = l.ppoid and mm.ppoid=po.ppoid and po.ppoanoexercicio = '2006' and po.ppostatus='A' " .
							" inner join elabrev.matriz m using ( mtrid ) ".
						" group by u.unicod " .
						" ) as b ON b.unicod = u.unicod " .
					" left join ( " .
						" select " .
							" su.unicod, " .
							" sum( coalesce( vldespesa, 0 ) ) as despesa, " .
							" sum( coalesce( saldo, 0 ) ) as saldo " .
						" from elabrev.v_saldounidadefonte2 su " .
						" where ppoanoexercicio = '" . $_SESSION['exercicio'] . "' " .
						" group by su.unicod, su.unidsc ".
						" ) as c on c.unicod = a.unicod " .
				" where u.unicod = '" . $unicod . "' and a.acastatus='A'  and a.prgano = '" . $_SESSION['exercicio'] . "' and a.acasnrap='f' " .
				" group by a.unicod, u.unitpocod, u.unidsc, u.uniano, o.orgdsc, a.prgano, b.total, c.despesa, c.saldo " .
				" order by 2 asc ";

			return $db->recuperar( $sql );
	
	
	}
	
	// FIM funções da página
	
	$unicod = $_REQUEST['unicod'] ? $_REQUEST['unicod'] : $_SESSION['unicod_visualizacao'];
	$_SESSION['unicod_visualizacao'] = $unicod;
	
if ( !$unicod )
{
	exit(
		"<script type=\"text/javascript\">
			alert( 'Selecione a Unidade para Visualizar seus Dados!' );
		</script>
		<script type=\"text/javascript\">
			document.location.href = \"elabrev.php?modulo=principal/propostaorcamentaria/despesa&acao=A\";
		</script>"
	);
}
	
	if ( !$db->usuarioPossuiPermissaoUnidade( $unicod ) )
	{
		$db->insucesso( 'Você não possui privilégio para manipular a unidade indicada.' );
	}
	$dados_unidade = pegar_dados_unidade( $unicod );
	
	include APPRAIZ . "includes/cabecalho.inc";
	

	
	
?>
<br/>

<? $db->cria_aba( $abacod_tela, $url, '' ); ?>
<? monta_titulo( 'Dados da Unidade', '' ); ?>
<table class="tabela" cellSpacing="1"  cellPadding="3" bgcolor="#f5f5f5" align="center" >
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Código:</td>
		<td width="80%"><?= $dados_unidade['unicod'] ?></td>
	</tr>
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Nome:</td>
		<td width="80%"><?= $dados_unidade['unidsc'] ?></td>
	</tr>
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Órgão:</td>
		<td width="80%"><?= $dados_unidade['orgdsc'] ?></td>
	</tr>
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Limite:</td>
		<td width="80%">R$ <?= formata_valor( $dados_unidade['limite'], 0 ) ?></td>
	</tr>
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Despesa:</td>
		<td width="80%">R$ <?= formata_valor( $dados_unidade['despesa'], 0 ) ?></td>
	</tr>
	<tr>
		<td width="20%" align="right" class="SubTituloDireita" >Saldo:</td>
		<td width="80%">R$ <?= formata_valor( $dados_unidade['saldo'], 0 ) ?></td>
	</tr>

<?
  $sql = "select	distinct pfl.pflcod as codigo, ".
  		 "			pfl.pfldsc as descricao, ".
  		 "			pfl.pflsncumulativo, ".
  		 "			rpu.usucpf, ".
  		 "			rpu.acaid, ".
  		 "			usu.usunome || '('|| org.orgdsc ||')' as usuario ".
  		 "	from perfil pfl ".
  		 "		left join monitora.usuarioresponsabilidade rpu on ".
  		 "			rpu.pflcod = pfl.pflcod and rpu.acaid = ".$_SESSION['acaid']." and rpu.rpustatus='A' ".
  		 "		left join usuario usu on ".
  		 "			usu.usucpf=rpu.usucpf ".
  		 "		left join orgao org on ".
  		 "			org.orgcod = usu.orgcod ".
  		 "	where 	pfl.pflstatus='A' and pfl.pflresponsabilidade in ('A','X','D') and ".
  		 "			organo='".$_SESSION['exercicio']."' order by pfl.pfldsc";
  		 
	$sql = "	select ".
		   " 		distinct pfl.pflcod as codigo, ".
		   "		pfl.pfldsc as descricao, ".
		   "		pfl.pflsncumulativo, ".
		   "		rpu.usucpf, ".
		   "		usu.usunome as usuario ".
		   "		from seguranca.perfil pfl ".
		   "			inner join elabrev.usuarioresponsabilidade rpu on ".
		   "				rpu.pflcod = pfl.pflcod ".
		   "			inner join seguranca.usuario usu on ".
		   "				usu.usucpf=rpu.usucpf ".
		   "			inner join unidade u on ".
		   "				u.unicod = rpu.unicod and unitpocod = 'U' ".
		   "		where ".
		   "			pfl.pflstatus='A' and u.unicod = '" .$unicod. "' and rpu.rpustatus='A' ".
		   "		order by pfl.pflsncumulativo, pfl.pfldsc ";


    $RS2 = $db->record_set($sql);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0)
    	{
   		print "<tr><td height='1px' colspan='2' align='center' bgcolor='#cccccc'>Responsabilidades Atribuídas:</td></tr>";
        for ($i=0; $i<=$nlinhas;$i++)
        {
            $res = $db->carrega_registro($RS2,$i);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td width='20%' align='right' class='SubTituloDireita'><b>".$descricao.":</b></td><td width='80%'>".$usuario."</td></tr>";
            print $linha;
        }
     }
     else 
     {
	        $linha = "<tr><td width='20%' align='right' class='SubTituloDireita'><b>Perfil:</b></td><td width='80%'>Não existem perfis associados para a unidade selecionada! </td></tr>";
            print $linha;
     	
     }


?>
</table>
